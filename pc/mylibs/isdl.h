#ifndef ISDL_H
#define ISDL_H

class csdlapp
{
bool noerror;

int window_width, window_height; //current window size

bool setup_sdl(int _w = 0, int _h = 0);
void main_loop(void);
bool procevents(void);
void draw(void);
void pixel_to_2Dpoint(int x_, int y_, float &x, float &y);
bool resize_viewport(const unsigned int &_width = 0, const unsigned int &_height = 0);

public:

csdlapp();
~csdlapp();

void wnd_title(const char *);
int run(void);

};

#endif

