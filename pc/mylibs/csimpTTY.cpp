/* library to communicate, serial port in non-canonical/raw mode */

#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <signal.h>
#include <string.h>
#include <csimpTTY.h>

csimpTTY::csimpTTY()
{
	fd = -1;
	
	buffer_size = 2000;
	buffer = nullptr;
	nrecieved = 0;
}

csimpTTY::~csimpTTY()
{
	if(fd)
	{
		tcsetattr (fd, TCSANOW, &oldtty);
 		close(fd); fd = -1;
	}

	if(buffer != nullptr)
	{
		delete[] buffer; buffer = nullptr;
	}
}

bool csimpTTY::init(unsigned int baud, unsigned int nbuffer, const std::string portname)
{
	if(0 < nbuffer && buffer == nullptr)
	{
		buffer_size = nbuffer;
		buffer = new unsigned char[buffer_size];
		if(buffer != nullptr)
		{
			fd = open(portname.c_str(), O_RDWR | O_NOCTTY | O_SYNC);// | O_NONBLOCK);
			
			if(fd >= 0 && set_interface_attribs (fd, baud, 0))
			{
				return true;
			}else printf("error %d opening %s: %s", errno, portname, strerror (errno));
		}else printf("error buffer allocation");
	}else printf("init: parameter error");
	return false;
}

bool csimpTTY::set_interface_attribs(int fd, int baud, int parity)
{
        struct termios tty;
        memset (&tty, 0, sizeof tty);
        memset (&oldtty, 0, sizeof oldtty);

        if(!tcgetattr (fd, &tty) && !tcgetattr (fd, &oldtty))
        {
			tty.c_cc[VMIN]  = 255; //this c_cc is a fing unsigned char so packet size cannot be more than 255
			tty.c_cc[VTIME] = 0;   //this two line: until 255 byte recieved, blocks read(), no time limit

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
//						tcflush(fd, TCIOFLUSH); //clear buffers, NO EFFECT maybe buffered data on arduino side?
						return true;
					}else printf("tcsetattr %d", errno);
				}else printf("tcflush %d", errno);
			}else printf("cfsetospeed %d", errno);
		}else printf("tcgetattr %d", errno);

        return false;
}

int csimpTTY::readin(void)
{
	return (nrecieved = read(fd, buffer, buffer_size));
}

void csimpTTY::print(void)
{
	for(int i = 0; i<nrecieved; i++)
	{
//		if(!(i%20)) printf("\n");
		printf("%02X ", buffer[i]);

		if(!(i % 20)) printf("\n");
	}
}
