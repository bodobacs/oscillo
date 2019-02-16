//test_csimpTTY.cpp

#include <csimpTTY.h>

class tty0to0 : public csimpTTY
{
public:
	tty0to0(){}
	~tty0to0(){}

};

int keepRunning = 1;

void intHandler(int dummy){
    keepRunning = 0;
}

int main(int argc, char **argv)
{
printf("\n Testing ctty class, print info about recieved byte vectors from /dev/ttyACM0. \n Press any key to continoue...\n");
getchar();


tty0to0 ctty;

if(ctty.init(B115200, 2000, "/dev/ttyACM0"))
{
	while(keepRunning)
	{
	//	printf("\nread: %d bytes\n", csimptty.readin());
		int n = ctty.readin();
		if(n){
			printf("\n Got %d bytes\n", n);
			ctty.print();
		}
	}//while
}else printf("\n Got %d bytes\n", ctty.readin());

return 0;
}

