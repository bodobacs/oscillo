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
	buffer = new unsigned char[buffer_size];
}

csimpTTY::~csimpTTY()
{
	if(fd)
	{
		tcsetattr (fd, TCSANOW, &oldtty);
		close(fd); fd = -1;
	}

	delete[] buffer; buffer = 0;
}

int csimpTTY::init(unsigned int baud, const std::string portname)
{
	fd = open(portname.c_str(), O_RDWR | O_NOCTTY | O_SYNC);
	
	if(fd >= 0 && set_interface_attribs (fd, baud, 0)) return 1;
	else{printf("error %d opening %s: %s", errno, portname, strerror (errno));}

	return 0;
}

int csimpTTY::set_interface_attribs(int fd, int baud, int parity)
{
        struct termios tty;
        memset (&tty, 0, sizeof tty);
        memset (&oldtty, 0, sizeof oldtty);

        if(!tcgetattr (fd, &tty) && !tcgetattr (fd, &oldtty))
        {
			tty.c_cc[VMIN]  = 100; //packet size?		// read doesn't block
			tty.c_cc[VTIME] = 1;						// 0.5 seconds read timeout

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
						return 1;
					}else{printf("tcsetattr %d", errno);}
				}else{printf("tcflush %d", errno);}
			}else{printf("cfsetospeed %d", errno);}
		}else{printf("tcgetattr %d", errno);}

        return 0;
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
		unsigned int a = *(buffer+i);
		printf("%d ", a);
	}
}
