//test_csimpTTY.cpp

#include <csimpTTY.h>

int keepRunning = 1;

void intHandler(int dummy){
    keepRunning = 0;
}

int main(int argc, char **argv)
{
printf("\n Usage: COMMAND /dev/ttyACM1 \n Testing ctty class, print info about recieved byte vectors from /dev/ttyACM0. \n Press any key to continue...\n");
getchar();


csimpTTY_buffered ctty;

if(ctty.init(B115200, argc > 1 ? argv[1] : "/dev/ttyACM0"))
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
}else printf("Serial error");

return 0;
}

