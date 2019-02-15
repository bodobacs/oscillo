//csimpsdl.h

#include <SDL.h>
#include <string>

class csimpSDL
{
	SDL_Window *sdl_win;
	
public:
	SDL_Renderer *sdl_ren;

	int init(const std::string window_name = "dummy", unsigned int x = 10, unsigned int y = 10, unsigned int w = 400, unsigned int h = 400);
	csimpSDL();
	~csimpSDL();

	void setcolor(Uint8 r, Uint8 g, Uint8 b, Uint8 a);
	void drawrect(void);

};

extern csimpSDL csimpsdl;
