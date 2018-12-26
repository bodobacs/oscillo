//csimpTTY.h
//based on OReilly__POSIX_Programmers_Guide

#include <string>
#include <termios.h>

class csimpTTY
{
	struct termios oldtty; //for exit reset

	int fd = 0;
	int set_interface_attribs (int fd, int speed, int parity);
	void set_blocking (int fd, int should_block);
	const unsigned int buffer_size = 500;
	unsigned char *buffer = 0;
	unsigned int nrecieved = 0;

/*	struct packet{
		unsigned char bytes_sent; //recieved bytes minus 1
		unsigned char data[254]; //zero can be after that
	};*/

public:
	csimpTTY();
	~csimpTTY();

/* BAUD rate constants from termios.h
B0, B50, B75, B110, B134, B150, B200, B300, B600, B1200, B1800, B2400, B4800,
B9600, B19200, B38400, B57600, B115200, B230400, B460800, B500000, B576000,
B921600, B1000000, B1152000, B1500000, B2000000, B2500000, B3000000, B3500000, B4000000*/

//return 0 error, 1 success
int init(unsigned int baud = B9600, std::string filename = "/dev/ttyACM0");
int readin();
void print(void);

};
