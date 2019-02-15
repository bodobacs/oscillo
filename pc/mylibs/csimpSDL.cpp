//csimpSDL.cpp
#include <csimpSDL.h>
#include <csimpLog.h>

csimpSDL csimpsdl;

csimpSDL::csimpSDL()
{
	sdl_ren = nullptr;
	sdl_win = nullptr;
}

csimpSDL::~csimpSDL()
{
	SDL_DestroyRenderer(sdl_ren);
	SDL_DestroyWindow(sdl_win);
	SDL_Quit();
}

int csimpSDL::init(const std::string window_name, unsigned int x, unsigned int y, unsigned int w, unsigned int h)
{
	csimplog << "csimpSDL init start" << std::endl;
	if(csimplog.is_open())
	{
		if(SDL_Init(SDL_INIT_VIDEO) == 0)
		{
			sdl_win = SDL_CreateWindow(window_name.c_str(), x, y, w, h, SDL_WINDOW_SHOWN);
			if (sdl_win != nullptr)
			{
					sdl_ren = SDL_CreateRenderer(sdl_win, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
					if (sdl_ren != nullptr){
						csimplog << "csimpSDL init success" << std::endl;
						return 1;
					}else csimplog << "SDL_CreateRenderer failed" << std::endl;

				SDL_DestroyWindow(sdl_win);
			}else csimplog << "SDL_CreateWindow failed" << std::endl;
		}else csimplog << "SDL_Init failed" << std::endl;
	}

	csimplog << "csimpSDL init failed" << std::endl;
	csimplog.logSDLerror();
	
	return 0;
}
