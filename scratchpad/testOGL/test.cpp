#include <SDL/SDL.h>
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
  screen=SDL_SetVideoMode(640,480,32,SDL_HWSURFACE|SDL_OPENGL);

  if ( screen == NULL )
  {
    printf("Unable to set 640x480 video: %s\n", SDL_GetError());
    exit(1);
  }
  int done=0;

  dime::OGLDrawDevice myDevice(screen->w, screen->h);

  //dime::Rectangle myR(50,10, 1,1);
  
  //dime::RectangleRenderer myRR(myR, "test.png", dime::RectangleRenderer::TILE);

  //myRR.render(&myDevice);
  myDevice.init();
  myDevice.clearScreen();
  myDevice.drawPixel(100,100,dime::Color(100,100,100));
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

  }
	SDL_Quit();
    return 0;
}
