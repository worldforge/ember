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

#include <SDL/SDL.h>
#include <SDL/SDL_endian.h>
#include <assert.h>

#include "DrawDevice.h"

class SDLDrawDevice : public DrawDevice 
{
  public:
    SDLDrawDevice(SDL_Surface *surface)
    {
      assert(surface);
      mySurface = surface;
    }
    virtual ~SDLDrawDevice()
    {
    }
    virtual void drawPixel(int x, int y, Uint32 color);
    virtual void drawLine(int x1, int y1, int x2, int y2, Uint32 color);
    virtual void fillRect(SDL_Rect *destRect, Uint32 color);
    virtual void blitSurface(SDL_Rect *srcRect, SDL_Rect *destRect, SDL_Surface *src);
    virtual void update();
  private:
    SDL_Surface *mySurface;
	
};