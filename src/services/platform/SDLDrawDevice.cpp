/*
    Copyright (C) 2002  Lakin Wecker

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
*/



#include "SDLDrawDevice.h"


// the algo for this is stolen from Frederic Ntwaniga's code, (a good friend of mine)
// and I believe he stole it from the SDL Documentation site.

void SDLDrawDevice::drawPixel(int x, int y, Uint32 color)
{
  if ( SDL_MUSTLOCK(mySurface) ) {
    if ( SDL_LockSurface(mySurface) < 0 ) {
      return;
    }
  }  
  switch (mySurface->format->BytesPerPixel) {
  case 1: { /* Assuming 8-bpp */
    Uint8 *bufp;
    bufp = (Uint8 *)mySurface->pixels + y*mySurface->pitch + x;
    *bufp = color;
  }
  break;
  
  case 2: { /* Probably 15-bpp or 16-bpp */
    Uint16 *bufp;
    bufp = (Uint16 *)mySurface->pixels + y*mySurface->pitch/2 + x;
    *bufp = color;
  }
  break;
  
  case 3: { /* Slow 24-bpp mode, usually not used */
    Uint8 *bufp;
    
    bufp = (Uint8 *)mySurface->pixels + y*mySurface->pitch + x * 3;
    if(SDL_BYTEORDER == SDL_LIL_ENDIAN) {
      bufp[0] = color;
      bufp[1] = color >> 8;
      bufp[2] = color >> 16;
    } else {
      bufp[2] = color;
      bufp[1] = color >> 8;
      bufp[0] = color >> 16;
    }
  }
  break;
  
  case 4: { /* Probably 32-bpp */
    Uint32 *bufp;
    bufp = (Uint32 *)mySurface->pixels + y*mySurface->pitch/4 + x;
    *bufp = color;
  }
  break;
  }
 
  if ( SDL_MUSTLOCK(mySurface) ) {
    SDL_UnlockSurface(mySurface);
  }
}

// the algo for this is stolen from Frederic Ntwaniga's code, (a good friend of mine)
// and I believe he stole it from some university's site, credit goes out to whoever wrote this.

void SDLDrawDevice::drawLine(int x1, int y1, int x2, int y2, Uint32 color)
{
  int dy;
  int dx;
  float t;  
  if(x1>=mySurface->w) x1=mySurface->w-1;
  if(x2>=mySurface->w) x2=mySurface->w-1;
  if(y1>=mySurface->h) y1=mySurface->h-1;
  if(y2>=mySurface->h) y2=mySurface->h-1;
 
  dy = y2 - y1; 
  dx= x2 - x1;
  t = (float) 0.5; // offset for rounding
  
  drawPixel(x1, y1,color);
  if (abs(dx) > abs(dy)) {          // slope < 1
    float m = (float) dy / (float) dx;      // compute slope
    t += y1;
    dx = (dx < 0) ? -1 : 1;
    m *= dx;
    while (x1 != x2) {
      x1 += dx;                           // step to next x value
       t += m;                             // add slope to y value
       drawPixel(x1, (int) t,color);
    }
  } else {                                    // slope >= 1
    float m = (float) dx / (float) dy;      // compute slope
    t += x1;
    dy = (dy < 0) ? -1 : 1;
    m *= dy;
    while (y1 != y2) {
      y1 += dy;                           // step to next y value
      t += m;                             // add slope to x value
      drawPixel((int) t, y1,color);
    }
  }
}

void SDLDrawDevice::blitSurface(SDL_Rect *srcRect, SDL_Rect *destRect, SDL_Surface *src)
{
  SDL_BlitSurface(src, srcRect, mySurface, destRect);
}

void SDLDrawDevice::update()
{
  SDL_UpdateRect(mySurface, 0, 0, mySurface->w, mySurface->h);
}

void SDLDrawDevice::fillRect(SDL_Rect *destRect, Uint32 color)
{
  SDL_FillRect(mySurface, destRect, color);
}