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
#include <signal.h>

const std::string appname("oscsdla");

csimpTTY_packet ctty;
sserialpacket paks[max_packet_per_msg];

//clock_t lasttime = 0; //CLOCKS_PER_SEC
unsigned int vtimer = 0;

const int width = max_packet_per_msg*packet_buffer_size;
const int height = 300;
unsigned char trigger_level = 255;


int keepRunning = 1;
void intHandler(int dummy){
	std::cout << appname << " Exit code SIGUSR2: " << dummy << std::endl;
    keepRunning = 0;
}

inline void draw_continous(void)
{
	int zero = height - 5;

	SDL_SetRenderDrawColor(csimpsdl.sdl_ren, 0,0,0,0);
	SDL_RenderClear(csimpsdl.sdl_ren);

	SDL_SetRenderDrawColor(csimpsdl.sdl_ren, 255,0,0,0);
	SDL_RenderDrawLine(csimpsdl.sdl_ren, 0, zero, width, zero);

	SDL_SetRenderDrawColor(csimpsdl.sdl_ren, 0, 255, 255,0);
	SDL_RenderDrawLine(csimpsdl.sdl_ren, 0,0, vtimer = vtimer+1 < width ? vtimer + 1 : 0, 0);

	SDL_SetRenderDrawColor(csimpsdl.sdl_ren, 200, 200,200,0);

	for(int i = 0; i < max_packet_per_msg; i++)
	{
		for(int j = 0; j < packet_buffer_size; j++)
		{
			SDL_RenderDrawPoint(csimpsdl.sdl_ren, ctty.get_packet(i).h.n_th * packet_buffer_size + j, zero - ctty.get_packet(i).buffer[j]);
		}
	}
}

inline void draw_triggered(void)
{
	int zero = height - 5;
	int trigger = ((ctty.get_packet(0).h.trigged_H & 0xff) << 8) | (0xff & ctty.get_packet(0).h.trigged_L);
//	csimplog << "Trig pos=" << trigger << ", val=" << (int)ctty.get_packet(trigger / packet_buffer_size).buffer[trigger % packet_buffer_size] << std::endl;

	SDL_SetRenderDrawColor(csimpsdl.sdl_ren, 0,0,0,0);
	SDL_RenderClear(csimpsdl.sdl_ren);

	SDL_SetRenderDrawColor(csimpsdl.sdl_ren, 255,0,0,0);
	SDL_RenderDrawLine(csimpsdl.sdl_ren, 0,zero,width,zero);

	SDL_SetRenderDrawColor(csimpsdl.sdl_ren, 255, 255, 0,0);
	SDL_RenderDrawLine(csimpsdl.sdl_ren, 0,0, vtimer = vtimer+1 < width ? vtimer + 1 : 0, 0);

	SDL_SetRenderDrawColor(csimpsdl.sdl_ren, 0xBB, 0x26, 0xDD, 0);
	SDL_RenderDrawLine(csimpsdl.sdl_ren, 0, zero - trigger_level, width, zero - trigger_level);
	
	SDL_SetRenderDrawColor(csimpsdl.sdl_ren, 200, 200,200,0);

	for(int j = 0; j < width; j++)
	{
		int i = (trigger + width / 2 + j) % width;
		if(i == trigger)
		{
			SDL_SetRenderDrawColor(csimpsdl.sdl_ren, 255, 255, 0,0);
			SDL_RenderDrawLine(csimpsdl.sdl_ren, j,0,j,25);
		}

		SDL_RenderDrawPoint(csimpsdl.sdl_ren,
		j,
		zero - ctty.get_packet(i / packet_buffer_size).buffer[i % packet_buffer_size]);
	}
}

inline void submain(void)
{
	SDL_Event e;

	while (keepRunning)
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
							keepRunning = 0;
							return;

							case SDLK_PAGEUP:
							{
								sscommandpacket s;
								s.command = c_set_trig_level;
								trigger_level += 255 >= trigger_level + 10 ? 10 : 0;
								s.data = trigger_level;
								ctty.send_command(s);
							}
							break;

							case SDLK_PAGEDOWN:
							{
								sscommandpacket s;
								s.command = c_set_trig_level;
								trigger_level -= 0 <= trigger_level - 10 ? 10 : 0;
								s.data = trigger_level;
								ctty.send_command(s);
							}
							break;

							case SDLK_TAB:
								ctty.print_summary();
							break;

							case SDLK_t:
							{
								sscommandpacket s;
								s.command = c_toggle_triggered;
								ctty.send_command(s);
							}
							break;

							case SDLK_x: //dummy
							{
								sscommandpacket s;
								s.command = 5;
								ctty.send_command(s);
							}
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
			if(ctty.get_packet(0).h.flags & FLAG_TRIGGERED)
			{
				draw_triggered();
			}else{
				draw_continous();
			}

			SDL_RenderPresent(csimpsdl.sdl_ren);
		}
	}
}

int main(int argc, char **argv)
{
	signal(SIGUSR2, intHandler);

	if(csimplog.is_open())
	{
		csimplog << appname << " start" << std::endl;

		if(csimpsdl.init(std::string("testSDLmain"), 0, 0, width, 300))
		{
			if(ctty.init(B115200, argc > 1 ? argv[1] : "/dev/ttyACM0"))
			{
				submain();

				return 0;
			}
		}
	}

//	csimplog << "Error on exit" << std::endl;
	return 1;
}
