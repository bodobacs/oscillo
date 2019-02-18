/*
test_0to0ncurses.cpp
a célnak megfelel ne írd át!!!
teszteli a csimpTTY osztályt: az usb soros portról ("/dev/ttyACM0") olvassa be a bejövő adatokat és kiírja egy tömbbe real-time
*/

#include <SDL.h>
#include <iostream>
#include <csimpLog.h>
#include <csimpSDL.h>
#include <csimpTTY.h>
#include <cstring>

csimpTTY_packet ctty;
sserialpacket paks[max_packet_per_msg];

clock_t lasttime = 0; //CLOCKS_PER_SEC
unsigned int vtimer = 0;

void submain(void)
{
	SDL_Event e;

	while (1)
	{
		while (SDL_PollEvent(&e))
		{
			switch(e.type)
			{
				case SDL_QUIT:
					return;

				case SDL_KEYDOWN:
						switch(e.key.keysym.sym)
						{
							case SDLK_ESCAPE:
							return;

							case SDLK_SPACE: //redraw
							break;
							
							case SDLK_RETURN: //reset
							break;

							case SDLK_TAB: //clear
							break;

							default:
							break;
						}//switch2
					break;
					
				case SDL_MOUSEBUTTONDOWN:
					break;
					
				case SDL_MOUSEMOTION:
					break;
					
				case SDL_MOUSEBUTTONUP:
					break;
								
				default:
				break;
			}
		}

		if(ctty.readinmessage())
		{
/*			for(int i = 0; i < pakstodraw; i++)
			{
				std::memcpy(&paks[i], &ctty.get_packet(i), packet_size);
			}*/

			//framelimiter
			clock_t nowtime = clock();
	//		if(0 < pakstodraw && nowtime - lasttime > CLOCKS_PER_SEC * 0.04)
			{
				lasttime = nowtime;

				SDL_SetRenderDrawColor(csimpsdl.sdl_ren, 0,0,0,0);
				SDL_RenderClear(csimpsdl.sdl_ren);

				SDL_SetRenderDrawColor(csimpsdl.sdl_ren, 255,0,0,0);
				SDL_RenderDrawLine(csimpsdl.sdl_ren, 0,150,1000,150);

				SDL_SetRenderDrawColor(csimpsdl.sdl_ren, 0, 255, 0,0);
				SDL_RenderDrawLine(csimpsdl.sdl_ren, 0,0, vtimer = vtimer+1 < 1000 ? vtimer + 1 : 0, 0);

				SDL_SetRenderDrawColor(csimpsdl.sdl_ren, 200, 200,200,0);

				for(int i = 0; i < max_packet_per_msg; i++)
				{
					for(int j = 0; j < packet_buffer_size; j++)
					{
						SDL_RenderDrawPoint(csimpsdl.sdl_ren, ctty.get_packet(i).h.n_th * packet_buffer_size + j, 290 - ctty.get_packet(i).buffer[j]);
	//					SDL_RenderDrawPoint(csimpsdl.sdl_ren, x++, 150 - paks[i].buffer[j]);
	//					csimplog << "point(" << x << "," << y << ")" << std::endl;
					}
				}

				SDL_RenderPresent(csimpsdl.sdl_ren);
			}
		}
	}
}

int main(int argc, char **argv)
{
	if(csimplog.is_open())
	{
		csimplog << "csimpSDL testing start" << std::endl;

		if(csimpsdl.init(std::string("testSDLmain"), 0, 0, 1000, 300))
		{
			if(ctty.init(B115200, "/dev/ttyACM1"))
			{
				submain();

				return 0;
			}
		}
	}

//	csimplog << "Error on exit" << std::endl;
	return 1;
}
