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

#ifndef SDLDRAWDEVICE_H
#define SDLDRAWDEVICE_H

#include "DrawDevice.h"

#include <SDL/SDL.h>
#include <SDL/SDL_endian.h>
#include <assert.h>

namespace dime {

/**
 * The SDLDrawDevice which uses SDL to do its drawing/blitting.
 *
 * The SDLDrawDevice uses SDL to draw lines, gradients, rects and blit surfaces
 * It requires and SDL_Surface pointer to the surface on which it will be operating.
 * It doesn't update the surface unless told to do so. 
 *
 *
 * dime::SDLDrawDevice *myDevice = new SDLDrawDevice(mySurface);
 * dime::Color myColor(0, 0, 255); // for blue
 * myDevice->drawLine(0, 0, 20, 20, myColor); //draws a small blue line.
 *
 * @author Lakin Wecker
 *
 */
    class SDLDrawDevice : public DrawDevice 
    {
        //======================================================================
        // Private Variables
        //======================================================================
    private:
		/*
		 * the surface on which we perform all operations
		 */
        SDL_Surface *mySurface;

        //======================================================================
        // Public Methods
        //======================================================================   
    public:
        
        //----------------------------------------------------------------------
        // Constructors

        /*
		 * The default constructor which requires an SDL_Surface pointer on which to operate.
		 */
        SDLDrawDevice(SDL_Surface *surface)// : myDimensions(surface->w,surface->h) Why doesn't this work?
        {
            assert(surface);
            mySurface = surface;
			myDimensions = Dimension(surface->w,surface->h); // gah
        }

        //----------------------------------------------------------------------
        // Destructor

        /*
		 * The virtual destructor
		 */
        virtual ~SDLDrawDevice()
        {
		}        
		//---------------------------------------------------------------------
		// Other public methods
        /*
		 * Draw a pixel of Color "color" at (x.y)
		 */
        virtual void drawPixel(int x, int y, Color color);

        /*
         * Draw a line from (x1,y1) to (x2, y2) with Color "color"
         */
        virtual void drawLine(int x1, int y1, int x2, int y2, Color color);

        /*
         * Fill a rectangle with Color.  The rectangle is defined by destRect
         */
        virtual void fillRect(SDL_Rect *destRect, Color color);

        /*
         * blits the src surface to our surface.  Src rect defines which portion of the src to blit.
         * 
         *@param srcRect the SDL_Rect to blit from the src surface.  Use the whole surface if it's NULL
         *@param destRect the destination on our surface.  if Null it uses (0,0) as a starting point.
         *@param src the SDL_Surface from which we will blit.
         */
        virtual void blitSurface(SDL_Rect *srcRect, SDL_Rect *destRect, SDL_Surface *src);

        /*
         * creates a gradient rect on our surface size and position equal to destRect
         */
        virtual void drawGradient(SDL_Rect *destRect, 
                                  Color leftTop, 
                                  Color rightTop, 
                                  Color leftBottom, 
                                  Color rightBottom);

        /*
         * does a full update of our surface
         */
        virtual void update();
   	
    };

}

#endif