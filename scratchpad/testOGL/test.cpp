#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <iostream>
#include <services/platform/RectangleRenderer.h>
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
	dime::Rectangle myR(50,10, 500,400);
  
  dime::RectangleRenderer myRR(myR, "test.png", dime::RectangleRenderer::TILE);

  
  SDL_Surface *picture, *picture2;
  picture = IMG_Load("test.png");
  picture2 = IMG_Load("nehe.bmp");
  
  std::cout << picture->format->Rmask << " " << picture->format->Gmask << " "<< picture->format->Bmask << " " << picture->format->Amask << std::endl;
  std::cout << picture2->format->Rmask << " " << picture2->format->Gmask << " " << picture2->format->Bmask << " " << picture2->format->Amask << std::endl;
  myDevice.init();
  myDevice.clearScreen();
  myRR.render(&myDevice);
  myDevice.drawPixel(100,100,dime::Color(100,100,100));
  myDevice.drawLine(0, 0, 100, 100, dime::Color(0,0,255));
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
	
    myDevice.clearScreen();
    myRR.render(&myDevice);
    myDevice.drawPixel(100,100,dime::Color(100,100,100));
    myDevice.drawLine(0, 0, 100, 100, dime::Color(0,0,255));
    myDevice.update();
  }
	SDL_Quit();
    return 0;
}
