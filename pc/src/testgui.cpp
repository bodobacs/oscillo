//test_gui.cpp

#include <SDL.h>
#include <iostream>
#include <csimpLog.h>
#include <csimpSDL.h>
#include <csimpGUI.h>

std::string appname = "testgui";
bool drawing = false;
const SDL_Rect screen = {0, 0, 800, 600};

cAllWidgets widgets(screen.w, screen.h);

void init_buttons(void)
{
}
/*
void buttondownactions(void)
{
	switch widgets.get_selectedindex()
	{
//		case 
		
	}
}
*/
bool ctrl_pushed = false;
void submain(void)
{
	init_buttons();
	int sx0, sy0;

	SDL_Event e;
	bool quit = false;
	while (!quit)
	{
		while (SDL_PollEvent(&e))
		{
			switch(e.type)
			{
				case SDL_QUIT:
					quit = true;
					return;

				case SDL_KEYDOWN:
					switch(e.key.keysym.sym)
					{
						case SDLK_ESCAPE:
							quit = true;
						return;

						case SDLK_SPACE:
							widgets.printall();
						break;

						case SDLK_PAGEUP:
						break;

						case SDLK_PAGEDOWN:
						break;

						case SDLK_TAB:
						break;

						case SDLK_LCTRL:
							csimplog << "CTRL down" << std::endl;
							 ctrl_pushed = true;
						break;

						default:
						break;
					}//switch2
					break;

				case SDL_KEYUP:
					switch(e.key.keysym.sym)
					{
						case SDLK_LCTRL:
							csimplog << "CTRL up" << std::endl;
							ctrl_pushed = false;
						break;
					}//switch3
					break;

				case SDL_MOUSEBUTTONDOWN:
					if(ctrl_pushed)
					{
						sx0 = e.button.x;
						sy0 = e.button.y;
					}
					break;

				case SDL_MOUSEMOTION:
					widgets.hoover(e.motion.x, e.motion.y);
					widgets.drawall();
					break;

				case SDL_MOUSEBUTTONUP:
					if(ctrl_pushed)
					{
						cintRect b;
						b.r.x = sx0 < e.button.x ? sx0 : e.button.x;
						b.r.y = sy0 < e.button.y ? sy0 : e.button.y;
						b.r.w = sx0 < e.button.x ? e.button.x - sx0 : sx0 - e.button.x;
						b.r.h = sy0 < e.button.y ? e.button.y - sy0 : sy0 - e.button.y;
						widgets.add_button(b);
					}

					widgets.drawall();
					break;

				default:
					break;
			}
		}

		SDL_RenderPresent(csimpsdl.sdl_ren);
	}
}

int main(int, char**)
{
	if(csimplog.is_open())
	{
		csimplog << appname << " start" << std::endl;

		if(csimpsdl.init(std::string("testSDLmain"), screen.x, screen.y, screen.w, screen.h))
		{
			submain();

			csimplog << appname << " finished" << std::endl;
			return 0;
		}

		csimplog << "Error on exit" << std::endl;

	}else std::cout << "Log library not working" << std::endl;
	return 1;
}

//submain
