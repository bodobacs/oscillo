/* library to communicate, serial port in non-canonical/raw mode */

#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <signal.h>
#include <string.h>
#include <csimpTTY.h>
#include <csimpLog.h>
#include <sys/ioctl.h>

csimpTTY::csimpTTY()
{
	fd = -1;
}

csimpTTY::~csimpTTY()
{
	if(fd)
	{
		tcsetattr (fd, TCSANOW, &oldtty);
 		close(fd); fd = -1;
	}
}

bool csimpTTY::init(unsigned int baud, const std::string portname)
{
	if(csimplog.is_open())
	{
		fd = open(portname.c_str(), O_RDWR | O_NOCTTY | O_SYNC);// | O_NONBLOCK);

		if(fd >= 0 && set_interface_attribs (fd, baud)){
			ttyname = portname;
			return true;
		}else{
			csimplog << "Cannot open port: \"" << portname.c_str() << "\", errorcode:" << errno << " - " << strerror (errno) << std::endl;
		}
	}else printf("\ncsimplog not ready");

	return false;
}

bool csimpTTY::set_interface_attribs(int fd, int baud)
{
        struct termios tty;
        memset (&tty, 0, sizeof tty);
        memset (&oldtty, 0, sizeof oldtty);

        if(!tcgetattr (fd, &tty) && !tcgetattr (fd, &oldtty))
        {
			tty.c_cc[VMIN]  = 255; //this c_cc is an unsigned char so packet size cannot be more than 255
			tty.c_cc[VTIME] = 0;   //= VTIME * 100ms :until 255 byte recieved or ms passed blocks read(), no time limit

			//INPUT flags
			tty.c_iflag &= ~(BRKINT | IGNBRK | PARMRK | INPCK
						| ISTRIP | INLCR | IGNCR | ICRNL | IXON | IXOFF );
			// Ignore break, Ignore parity, Ignore parity
			// Don't mask, No <cr> or <lf>, Ignore STOP
			tty.c_iflag |= IGNBRK;

			//OUTPUT flags
			tty.c_oflag &= -(OPOST); // No output flags
			
			//LOCAL flags
			tty.c_lflag &= ~(ECHO | ECHOE | ECHOK | ECHONL | ICANON | ISIG | NOFLSH | TOSTOP);
			
			//CONTROL flags
			tty.c_cflag &= ~(CSIZE | CSTOPB | HUPCL | PARENB);
			tty.c_cflag |= CLOCAL | CREAD | CS8;

			if(-1 != cfsetospeed (&tty, baud)
			&& -1 != cfsetispeed (&tty, baud))
			{
				if(-1 != tcflush(fd, TCIFLUSH))
				{
					if(-1 != tcsetattr(fd, TCSANOW, &tty))
					{
						tcflush(fd, TCIOFLUSH); //clear buffers, NO EFFECT maybe buffered data on arduino side?
						return true;
					}else printf("tcsetattr %d", errno);
				}else printf("tcflush %d", errno);
			}else printf("cfsetospeed %d", errno);
		}else printf("tcgetattr %d", errno);

        return false;
}

/*
 ************************ csimpTTY_buffered ****************************
*/

csimpTTY_buffered::csimpTTY_buffered()
{
	nrecieved = 0;
}

csimpTTY_buffered::~csimpTTY_buffered()
{
}

int csimpTTY_buffered::readin(void)
{
	return (nrecieved = read(fd, buffer, sizeof(buffer)));
}

void csimpTTY_buffered::print(void)
{
	for(int i = 0; i<nrecieved; i++)
	{
		printf("%02X ", buffer[i]);

		if(!(i % 20)) printf("\n");
	}
}

/*
 ************************ csimpTTY_packet ****************************
*/

csimpTTY_packet::csimpTTY_packet()
{
	waitingfor_n_th = 0;
}

csimpTTY_packet::~csimpTTY_packet()
{
}

bool csimpTTY_packet::readpacket(const int &r)
{
	sserialpacket &sp = packets[r];

	int waiting = 0;
	ioctl(fd, FIONREAD, &waiting);
	
//	csimplog << "waiting: " << waiting << std::endl;
	
	if(packet_size <= waiting)
	{
		if(packet_size == read(fd, &sp, sizeof(sserialpacket)))
		{
			if(
			sp.h.k1 == packet_k1 &&
			sp.h.k2 == packet_k2 &&
			sp.h.k3 == packet_k3 )
			{
				collect_packetdata(sp);
				return true;
			}else{
				flush();
			}
		}else waitingfor_n_th = 0;
	}
	return false;
}

bool csimpTTY_packet::readinmessage(void)
{
	if(readpacket(waitingfor_n_th))
	{
		if(get_packet(waitingfor_n_th).h.n_th + 1 < max_packet_per_msg)
		{
			waitingfor_n_th++;
		}else{
			waitingfor_n_th = 0;
			++dg_msgs;
			return true;
		}
	}

	return false;
}

void csimpTTY_packet::collect_packetdata(const sserialpacket &sp)
{
	dg_last_command = sp.h.last_command;
	dg_last_command_data = sp.h.last_command_data;
	dg_triggered_packets += (sp.h.flags & FLAG_TRIGGERED) ? 1 : 0;
	++dg_packets;
}

void csimpTTY_packet::print_summary(void)
{
	csimplog << "-- Diagnostics --" << std::endl;
	csimplog << "Recieved packets: " << dg_packets << std::endl;
	csimplog << "Recieved messages: " << dg_msgs << std::endl;
	csimplog << "Flushed: " << dg_flushed << " times" << std::endl;
	csimplog << "Triggered packets: " << dg_triggered_packets << std::endl;
	csimplog << "Last command: " << (int)dg_last_command << std::endl;
	csimplog << "Last command data: " << (int)dg_last_command_data << std::endl << "------" << std::endl;
}

bool csimpTTY_packet::printpacket(int n)
{
	sserialpacket &sp = packets[n];
	csimplog << "packet \"" << n << "\" :"<< std::endl;
	csimplog << "keys k1: " << int(sp.h.k1) << ", k2: " << int(sp.h.k2) << ", k3: " << int(sp.h.k3) << std::endl;
	csimplog << ", n_th: " << int(sp.h.n_th) << std::endl;

	csimplog << "buffer:" << std::endl;
	for(int i; i < packet_buffer_size; i++)
	{
		csimplog << sp.buffer[i];
	}

	csimplog << "end." << std::endl;
}

/*reading until stable packets arrive*/
void csimpTTY_packet::flush(void)
{
	unsigned char buf[500];
	int cnt = 10;
	
	while(packet_buffer_size != read(fd, buf, 500) &&
			buf[0] != packet_k1 &&
			buf[1] != packet_k2 &&
			buf[2] != packet_k3 &&
			cnt--
	);

	++dg_flushed;
}

void csimpTTY_packet::send_command(sscommandpacket &ssc)
{
	write(fd, (unsigned char *)&ssc, sizeof(sscommandpacket));
}
