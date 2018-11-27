//test_csimpTTY.cpp

#include <csimpTTY.h>

int keepRunning = 1;

void intHandler(int dummy){
    keepRunning = 0;
}

int main(int argc, char **argv)
{

csimpTTY csimptty;
csimptty.init(B115200);

while(keepRunning)
{
//	printf("\nread: %d bytes\n", csimptty.readin());
	csimptty.readin();
	csimptty.print();
}//while

return 0;

}
