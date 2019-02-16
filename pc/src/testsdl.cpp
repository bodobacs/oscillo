//testSDLmain.cpp
#include <SDL.h>
#include <iostream>
#include <csimpLog.h>
#include <csimpSDL.h>

bool drawing = false;
SDL_Rect rect = {0,0,0,0};

void drawrect(void)
{
	SDL_RenderFillRect(csimpsdl.sdl_ren, &rect);
}

void submain(void)
{
	SDL_Event e;
	bool quit = false;
	while (!quit)
	{
		while (SDL_PollEvent(&e))
		{
			if (e.type == SDL_QUIT){
				quit = true;
			}

			if (e.type == SDL_KEYDOWN){
				SDL_SetRenderDrawColor(csimpsdl.sdl_ren, 0,0,0,0);
				SDL_RenderClear(csimpsdl.sdl_ren);
				SDL_SetRenderDrawColor(csimpsdl.sdl_ren, 0,100,100,0);
			}
			
			if (e.type == SDL_MOUSEBUTTONDOWN){
				drawing = true;
				rect.x = e.button.x; rect.y = e.button.y;
				rect.w = rect.h = 1;
				drawrect();
			}
			if(drawing)
			{
				if (e.type == SDL_MOUSEMOTION){
					rect.w = e.motion.x - rect.x;
					rect.h = e.motion.y - rect.y;
					drawrect();
				}
				if (e.type == SDL_MOUSEBUTTONUP){
					rect.w = e.button.x - rect.x;
					rect.h = e.button.y - rect.y;
					drawrect();
					drawing = false;
				}
			}
		}

		SDL_RenderPresent(csimpsdl.sdl_ren);
	}

}

int main(int, char**)
{
	if(csimplog.is_open())
	{
		csimplog << "csimpSDL testing start" << std::endl;

		if(csimpsdl.init(std::string("testSDLmain"), 100, 100, 400, 400))
		{
			submain();

			csimplog << "Closing without error" << std::endl;
			return 0;
		}

		csimplog << "Error on exit" << std::endl;
	}else std::cout << "Log library not working" << std::endl;
	return 1;
}

//submain
