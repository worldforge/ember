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

void SDLDrawDevice::drawPixel(int x, int y, Color color)
{
    Uint32 myColor = SDL_MapRGB(mySurface->format, 
                                (Uint8)color.getR(), 
                                (Uint8)color.getG(),
                                (Uint8)color.getB());
    if(x < mySurface->w && y < mySurface->h)
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
                *bufp = myColor;
            }
                break;
	  
            case 2: { /* Probably 15-bpp or 16-bpp */
                Uint16 *bufp;
                bufp = (Uint16 *)mySurface->pixels + y*mySurface->pitch/2 + x;
                *bufp = myColor;
            }
                break;
	  
            case 3: { /* Slow 24-bpp mode, usually not used */
                Uint8 *bufp;
	    
                bufp = (Uint8 *)mySurface->pixels + y*mySurface->pitch + x * 3;
                if(SDL_BYTEORDER == SDL_LIL_ENDIAN) {
                    bufp[0] = myColor;
                    bufp[1] = myColor >> 8;
                    bufp[2] = myColor >> 16;
                } else {
                    bufp[2	] = myColor;
                    bufp[1] = myColor >> 8;
                    bufp[0] = myColor >> 16;
                }
            }
                break;
	  
            case 4: { /* Probably 32-bpp */
                Uint32 *bufp;
                bufp = (Uint32 *)mySurface->pixels + y*mySurface->pitch/4 + x;
                *bufp = myColor;
            }
                break;
            }
	 
            if ( SDL_MUSTLOCK(mySurface) ) {
                SDL_UnlockSurface(mySurface);
            }
        }
}
// the algo for this is stolen from Frederic Ntwaniga's code, (a good friend of mine)
// and I believe he stole it from some university's site, credit goes out to whoever wrote this.

void SDLDrawDevice::drawLine(int x1, int y1, int x2, int y2, Color color)
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

void SDLDrawDevice::fillRect(SDL_Rect *destRect, Color color)
{
    Uint32 myColor = SDL_MapRGB(mySurface->format,
                                (unsigned char)color.getR(),
                                (unsigned char)color.getG(), 
                                (unsigned char)color.getB());
    SDL_FillRect(mySurface, destRect, myColor);
}

void SDLDrawDevice::gradientRect(SDL_Rect *destRect, 
                                 Color leftTop, 
                                 Color rightTop, 
                                 Color leftBottom, 
                                 Color rightBottom)
{
	int x1 = destRect->x;
	int x2 = destRect->x+destRect->w;
	int y1 = destRect->y;
	int y2 = destRect->y+destRect->h;
 	int width   = x2 - x1;
	int height  = y2 - y1; 

	Color leftDelta = (leftBottom - leftTop) / height * 1.0;
	Color rightDelta = (rightBottom - rightTop) / height * 1.0;

	Color leftColor  = leftTop;
	Color rightColor = rightTop; 

	Color pixelColor;
	Color pixelDelta;

	for (int y = y1; y <= y2; ++y)
        { 
            pixelDelta = (rightColor - leftColor) / width;
            pixelColor = leftColor;
	
            for (int x = x1; x <= x2; ++x )
                { 
                    drawPixel( x, y, pixelColor );
                    pixelColor = pixelColor + pixelDelta;
                } 
	
            leftColor = leftColor + leftDelta;
            rightColor = rightColor + rightDelta;
        }    					      						      	
}
