/*
test_0to0ncurses.cpp
a célnak megfelel ne írd át!!!
teszteli a csimpTTY osztályt: az usb soros portról ("/dev/ttyACM0") olvassa be a bejövő adatokat és kiírja egy tömbbe real-time
*/

#include <csimpTTY.h>
#include <ncurses.h>

int keepRunning = 1;

/*
void intHandler(int dummy){
    keepRunning = 0;
}
*/
int main(int argc, char **argv)
{

	csimpTTY_buffered ctty;

	if(ctty.init(B115200, argc > 1 ? argv[1] : "/dev/ttyACM0"))
	{
		initscr();			/* Start curses mode 		  */
	//	noecho(); //no printf too
		mvprintw(0,0,"\n Testing ctty class, print info about recieved byte vectors from /dev/ttyACM0. \n Press any key to continue...\n");

		int maxy, maxx;
		getmaxyx(stdscr, maxy, maxx);

		refresh();			/* Print it on to the real screen */
		getch();			/* Wait for user input */

		unsigned char *pc = ctty.get_buffer();

		while(keepRunning)
		{
			int n = ctty.readin();
			if(n > 1){
				clear();
				mvprintw(0,0,"Got %d bytes", n); printw(", max shown %d bytes", maxx/4 * (maxy-1));
				int c = 0; //check printed byte count
				for(int y = 0; y < maxy && c < n; y++)
				{
					for(int x = 0; x < maxx/4 && c < n; x++)
					{
						move(y+1, 4*x);
						printw("%03u ",  *(pc + c));

						c++;
					}
				}
				refresh();
			}
		}//while

		refresh();			/* Print it on to the real screen */
		endwin();

	}else printf("Serial init error\n");


return 0;
}
