#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <iostream>
#include <services/platform/BitmapRenderer.h>
#include <services/platform/OGLDrawDevice.h>

int main(int argc, char **argv) {

 if ( SDL_Init(SDL_INIT_VIDEO) < 0 )
  {
    printf("Unable to init SDL: %s\n", SDL_GetError());
    exit(1);
  }

    /* Sets up OpenGL double buffering */
  SDL_GL_SetAttribute( SDL_GL_DOUBLEBUFFER, 1 );
  SDL_Surface *screen;
  screen=SDL_SetVideoMode(640,480,32,SDL_HWSURFACE|SDL_OPENGL|SDL_GL_DOUBLEBUFFER);

  if ( screen == NULL )
  {
    printf("Unable to set 640x480 video: %s\n", SDL_GetError());
    exit(1);
  }
  int done=0;

	dime::OGLDrawDevice myDevice(screen->w, screen->h);
	dime::Rectangle myR(300,300, 100,100);
  
  dime::BitmapRenderer myRR(myR, "test.png", dime::BitmapRenderer::TILE);

  
  SDL_Surface *picture, *picture2;
  
  myDevice.init();
  myDevice.clearScreen();
  SDL_Rect myRect;
  SDL_Rect gradientRect;
  myRect.x = 100; myRect.y = 100; myRect.w = 100; myRect.h = 100;
  gradientRect.x = 200; gradientRect.y = 100; gradientRect.w = 100; gradientRect.h = 100;
  myDevice.update();
  
  while(done == 0)
  {
    SDL_Event event;

    while ( SDL_PollEvent(&event) )
    {
      if ( event.type == SDL_QUIT )  {  done = 1;  }

      if ( event.type == SDL_KEYDOWN )
      {
        if ( event.key.keysym.sym == SDLK_ESCAPE ) { done = 1; }
      }
    }
	
    //myDevice.clearScreen();
    myRR.render(&myDevice);
    myDevice.drawPixel(25,100,dime::Color(100.0f,100.0f,100.0f));
    myDevice.drawPixel(25,102,dime::Color(100.0f,100.0f,100.0f));
    myDevice.drawPixel(25,101,dime::Color(100.0f,100.0f,100.0f));
    myDevice.fillRect(&myRect, dime::Color(255.0f, 0.0f, 0.0f));
    myDevice.drawGradient(&gradientRect,
		           dime::Color(255.0f, 0.0f, 0.0f),
			   dime::Color(0.0f, 255.0f, 0.0f),
			   dime::Color(0.0f, 0.0f, 255.0f),
			   dime::Color(255.0f, 0.0f, 255.0f));
    myDevice.drawLine(200, 200, 250, 250, dime::Color(0.0f,0.0f,255.0f));
    myDevice.update();
  }
	SDL_Quit();
    return 0;
}
