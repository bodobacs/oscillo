//csimpTTY.h
//based on OReilly__POSIX_Programmers_Guide

#include <string>
#include <termios.h>
#include "../../commonheader.h"

class csimpTTY
{
protected:
	struct termios oldtty; //for exit reset

	int fd = 0;
	bool set_interface_attribs (int fd, int speed);
	void set_blocking (int fd, int should_block);

public:
	csimpTTY();
	~csimpTTY();
	
/* BAUD rate constants from termios.h
B0, B50, B75, B110, B134, B150, B200, B300, B600, B1200, B1800, B2400, B4800,
B9600, B19200, B38400, B57600, B115200, B230400, B460800, B500000, B576000,
B921600, B1000000, B1152000, B1500000, B2000000, B2500000, B3000000, B3500000, B4000000*/

	//return 0 error, 1 success
	bool init(unsigned int baud = B9600,std::string filename = "/dev/ttyACM0");
};

class csimpTTY_buffered : public csimpTTY
{
protected:
	unsigned char buffer[1024];
	unsigned int nrecieved;

public:
	csimpTTY_buffered();
	~csimpTTY_buffered();

	int readin(void);
	void print(void);
	unsigned char *get_buffer(void){ return buffer; }
	unsigned int get_nrecieved(void){ return nrecieved; }
	unsigned int get_buffer_size(void){ return sizeof(buffer); }
};

class csimpTTY_packet : public csimpTTY
{
protected:
	sserialpacket packets[max_packet_per_msg];
	int waitingfor_n_th = 0;

	bool readpacket(const int &);
	bool printpacket(int n);
	void flush(void);

public:
	csimpTTY_packet();
	~csimpTTY_packet();

	sserialpacket &get_packet(int i){ return packets[i]; }
	bool readinmessage(void);
};
