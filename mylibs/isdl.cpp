#include "isdl.h"
#include <SDL.h>
#include <ctime>
#include "liblog.h"
#include <SDL_opengl.h>

const Uint32 wmode =  SDL_OPENGL | SDL_NOFRAME | SDL_DOUBLEBUF | SDL_HWSURFACE; // SDL_FULLSCREEN SDL_RESIZABLE

csdlapp::csdlapp()
{
	window_width, window_height;//current window size
	noerror = setup_sdl();
}

csdlapp::~csdlapp()
{
	SDL_Quit();
}

bool csdlapp::resize_viewport(const unsigned int &_width, const unsigned int &_height)
{
	window_width = _width;
	window_height = _height;

	if(SDL_SetVideoMode(window_width, window_height, 0, wmode) != 0)
	{
		glViewport((GLint)(0), (GLint)(0), (GLint)_width, (GLint)_height);//szétnyújtott kép

		log_debug("new window size: %dx%d", window_width, window_height);

		return true;
	}else log_error("failed to set video mode: %s", SDL_GetError());
	return false;
}

bool csdlapp::setup_sdl(int _w, int _h)
{
	const SDL_VideoInfo* videoinfo;

	if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER)){

		/* Get the current video information */
		videoinfo = SDL_GetVideoInfo();
		if( videoinfo != NULL ){
				log_debug("SDL_GetVideoInfo: hardware surfaces=%u, window manager=%u, video memory=%u, size=%ux%u, bpp=%u, bytes/pixel=%u", videoinfo->hw_available, videoinfo->wm_available, videoinfo->video_mem, videoinfo->current_w, videoinfo->current_h, videoinfo->vfmt->BitsPerPixel, videoinfo->vfmt->BytesPerPixel);

			if(_w && _h)//ha meg van adva ablakmeret, akkor azt fogja hasznalni
			{
				window_width = _w;
				window_height = _h;
			}else{
				window_width = videoinfo->current_w;
				window_height = videoinfo->current_h;
			}

			SDL_GL_SetAttribute( SDL_GL_DOUBLEBUFFER, 1 );

			// SDL_ShowCursor(SDL_DISABLE);
			videoinfo = SDL_GetVideoInfo();

			if(resize_viewport(window_width, window_height))
			{
				log_debug("\nsdl init ok");
				return true;
			}
		}else log_error("\nCouldn't get video information: %s", SDL_GetError());
	}else log_error("\nCouldn't initialize SDL: %s", SDL_GetError());

	return false;
}

void csdlapp::pixel_to_2Dpoint(int x_, int y_, float &x, float &y)
{
//		x_ -= viewx; y_ -= viewy;
//		x = (float)x_ / (float)vieww;
//		y = (float)y_ / (float)viewh;
}

void csdlapp::wnd_title(const char *title)
{
	SDL_WM_SetCaption(title, title);
}


enum{SDL_USER_FRAMEREFRESH};

Uint32 timercallback_frame(Uint32 interval, void *param)
{
	SDL_Event event;
	event.type = SDL_USEREVENT;
//	even.code = SDL_USER_FRAMEREFRESH; //az egyetlen user event eddig, szal nem kell ellenorizni
	SDL_PushEvent(&event);

	return interval;
}

bool csdlapp::procevents(void)
{
	bool retval = true;

	SDL_Event event;
	while(retval && SDL_WaitEvent(&event))
	{
		switch( event.type ){
			case SDL_KEYDOWN:
				switch ( event.key.keysym.sym ){
					case SDLK_ESCAPE:
						retval = false;

					default:
					      //no default key processing
					      //(stops compiler warnings for unhandled SDL keydefs
					      break;
				}
				break;

			case SDL_MOUSEMOTION:
				break;

			case SDL_MOUSEBUTTONDOWN:
				break;
			case SDL_MOUSEBUTTONUP:
				if(event.button.button == SDL_BUTTON_LEFT){}
				if(event.button.button == SDL_BUTTON_RIGHT){}
				break;

			case SDL_VIDEORESIZE:
				log_debug("\nSDL_VIDEORESIZE:%dx%d", event.resize.w, event.resize.h);
				resize_viewport(event.resize.w, event.resize.h);
				break;

			case SDL_QUIT:
				retval = false;

			case SDL_USEREVENT:
//				SDL_USER_FRAMEREFRESH
				glClearColor (.0f, .0f, .0f, 1.f);
				glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
				draw();
				SDL_GL_SwapBuffers();
				break;
		}
	}//while

	return retval;
}

void csdlapp::draw(void)
{
	glMatrixMode(GL_PROJECTION); glLoadIdentity();
	gluPerspective(45.0f,(GLfloat)window_width/(GLfloat)window_height,0.1f,100.0f);
	glTranslatef(0.,0.,-10);
//	glOrtho(0.,25.0,0., 18.75, -2., 2.);
//	GLfloat shapearrow[] = {0.5f, -1.f, 0.f, -0.5f, -1.f, 0.f, -0.5f, -3.f, 0.f, 0.5f, -3.f, 0.f, -1.f, -1.f, 0.f, 1.f, -1.f, 0.f, 0.f, 0.f, 0.f};
	
	GLfloat shapearrow[] = {
		0.500000, -1.000000, 0.000000, -0.500000, -3.000000, 0.000000, 0.500000, -3.000000, 0.000000,
		0.500000, -1.000000, 0.000000, -0.500000, -1.000000, 0.000000, -0.500000, -3.000000, 0.000000, 
		0.500000, -1.000000, 0.000000, 1.000000, -1.000000, 0.000000, 0.000000, 0.000000, 0.000000, 
		0.500000, -1.000000, 0.000000, 0.000000, 0.000000, 0.000000, -0.500000, -1.000000, 0.000000, 
		-0.500000, -1.000000, 0.000000, 0.000000, 0.000000, 0.000000, -1.000000, -1.000000, 0.000000
	};

	glMatrixMode(GL_MODELVIEW); glLoadIdentity();

/*mukodik frankon de nem egyenletes
	static GLdouble angle = 0;
	angle += 1; if (angle > 360) angle -= 360;

	log_debug("\nangle=%f, %d",angle, CLOCKS_PER_SEC);
// */
// A clock() csak másodperceket ad vissza, NAGYON SZAR
//* FASZA!!! clock nem jó semmire, mert csak másodperces a felbontása linuxon legalábbis
	GLfloat angle = 0.36f * (GLfloat)(SDL_GetTicks() % 1000);
//	log_debug("\nangle=%f", angle);
//*/
	glRotatef(angle, 0.f,1.f,0.f);
	glScalef(0.1f, 0.1f, 0.1f);

	glColor3f(.4f,.3f,.2f);
	float size = 1.f;
		glBegin(GL_TRIANGLES);
			int elemek = sizeof(shapearrow)/sizeof(GLfloat);
			for( int i = 0; i < elemek; i+=3)
			{
//				glColor3f(0.f,(GLfloat)i/21.f,0.f);
				glVertex3f(shapearrow[i], shapearrow[i+1], shapearrow[i+2]);
			}
		glEnd();

		glMatrixMode(GL_MODELVIEW); glLoadIdentity();

		glTranslatef(-5.,0.,0.);

/*		static bool blink = true;
		blink = !blink;
		if(blink){
			glBegin(GL_QUADS);
				glVertex3f(.0f, .0f, 0.0f);
				glVertex3f(1.f, 0.0f, 0.0f);			
				glVertex3f(1.f, 1.0f, 0.0f);			
				glVertex3f(0.0f, 1.0f, 0.0f);
			glEnd();
		}
//*/
}

int csdlapp::run(void)
{
	if(noerror)
		{	
		SDL_TimerID timer_frame = SDL_AddTimer(20, timercallback_frame, 0);
		if(NULL == timer_frame){
			log_error("cannot initialize frame timer\n");
			return 1;
		}

		while(procevents());

		SDL_RemoveTimer(timer_frame);
		return 0;
	}

	return 1;
}

