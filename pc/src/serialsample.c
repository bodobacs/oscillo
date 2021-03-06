/*
 char sttycmd[256] = "stty -F /dev/ttyACM0 cs8 9600 ignbrk -brkint -icrnl -imaxbel -opost -onlcr -isig -icanon -iexten -echo -echoe -echok -echoctl -echoke noflsh -ixon -crtscts");
 int ardfd = fopen("/dev/ttyACM0", "rw");
 simple interface for tty reading code from stackoverflow
 https://stackoverflow.com/a/6947758
*/

#include <errno.h>
#include <fcntl.h> 
#include <string.h>
#include <termios.h>
#include <unistd.h>
#include <stdio.h>
#include <signal.h>


int fd = 0;
	
int set_interface_attribs (int fd, int speed, int parity)
{
        struct termios tty;
        memset (&tty, 0, sizeof tty);
        if (tcgetattr (fd, &tty) != 0)
        {
                printf("error %d from tcgetattr", errno);
                return -1;
        }

        cfsetospeed (&tty, speed);
        cfsetispeed (&tty, speed);

        tty.c_cflag = (tty.c_cflag & ~CSIZE) | CS8;     // 8-bit chars
        // disable IGNBRK for mismatched speed tests; otherwise receive break
        // as \000 chars
        tty.c_iflag &= ~IGNBRK;         // disable break processing
        tty.c_lflag = 0;                // no signaling chars, no echo,
                                        // no canonical processing
        tty.c_oflag = 0;                // no remapping, no delays
        tty.c_cc[VMIN]  = 0;            // read doesn't block
        tty.c_cc[VTIME] = 5;            // 0.5 seconds read timeout

        tty.c_iflag &= ~(IXON | IXOFF | IXANY); // shut off xon/xoff ctrl

        tty.c_cflag |= (CLOCAL | CREAD);// ignore modem controls,
                                        // enable reading
        tty.c_cflag &= ~(PARENB | PARODD);      // shut off parity
        tty.c_cflag |= parity;
        tty.c_cflag &= ~CSTOPB;
        tty.c_cflag &= ~CRTSCTS;

        if (tcsetattr (fd, TCSANOW, &tty) != 0)
        {
                printf("error %d from tcsetattr", errno);
                return -1;
        }
        return 0;
}

void set_blocking (int fd, int should_block)
{
        struct termios tty;
        memset (&tty, 0, sizeof tty);
        if (tcgetattr (fd, &tty) != 0)
        {
                printf("error %d from tggetattr", errno);
                return;
        }

        tty.c_cc[VMIN]  = should_block ? 1 : 0;
        tty.c_cc[VTIME] = 5;            // 0.5 seconds read timeout

        if (tcsetattr (fd, TCSANOW, &tty) != 0)
                printf("error %d setting term attributes", errno);
}

int keepRunning = 1;

void intHandler(int dummy) {
    keepRunning = 0;
}

int main(int argc, char **argv)
{

	signal(SIGINT, intHandler);
	
	char *portname = "/dev/ttyACM0";
	fd = open(portname, O_RDWR | O_NOCTTY | O_SYNC);
	
	if (fd < 0){
        printf("error %d opening %s: %s", errno, portname, strerror (errno));
        return 0;
	}

set_interface_attribs (fd, B9600, 0);  // set speed to 115,200 bps, 8n1 (no parity)
set_blocking (fd, 1);           // set no blocking

//write (fd, "hello!\n", 7);    // send 7 character greeting

//usleep ((7 + 25) * 100);      // sleep enough to transmit the 7 plus


while(keepRunning){

const int buffer_size = 1000;	// receive 25:  approx 100 uS per char transmit
unsigned char buf[buffer_size];

int n = read (fd, buf, buffer_size);  // read up to 100 characters if ready to read

for(int i = 0; i<buffer_size; i++)
{
	printf("%02X ", buf[i]);
	if(!(i%20)) printf("\n");
}

printf("\n----------------------\n");

}//while

return 0;

}
