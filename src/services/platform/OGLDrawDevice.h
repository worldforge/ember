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

#ifndef OGLDRAWDEVICE_H
#define OGLDRAWDEVICE_H

#include "DrawDevice.h"
#include "services/logging/LoggingService.h"
#include <framework/Exception.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <assert.h>

namespace dime {

/**
 * The OGLDrawDevice which uses OpenGL to do its drawing/blitting.
 *
 * The OGLDrawDevice uses OpenGL to draw lines, gradients, rects and blit surfaces
 * It requires and OGL_Surface pointer to the surface on which it will be operating.
 * It doesn't update the surface unless told to do so. 
 *
 *
 * dime::OGLDrawDevice *myDevice = new OGLDrawDevice(mySurface->w, mySurface->h);
 * dime::Color myColor(0, 0, 255); // for blue
 * myDevice->drawLine(0, 0, 20, 20, myColor); //draws a small blue line.
 *
 * @author Lakin Wecker
 *
 */
    class OGLDrawDevice : public DrawDevice 
    {
        //======================================================================
        // Private Variables
        //======================================================================
    private:
        /*
         * The width of the screen
         */
        int myScreenWidth;
        /*
         * The height of the screen
         */
        int myScreenHeight;

        //======================================================================
        // Public Methods
        //======================================================================   
    public:
        
        //----------------------------------------------------------------------
        // Constructors

        /*
		 * The default constructor which requires an OGL_Surface pointer on which to operate.
		 */
        OGLDrawDevice(int width, int height) 
            : DrawDevice(width, height), myScreenWidth(width), myScreenHeight(height)// : myDimensions(surface->w,surface->h) Why doesn't this work?
        {
	  init();
	  clearScreen();
	  update();
        }

        //----------------------------------------------------------------------
        // Destructor

        /*
		 * The virtual destructor
		 */
        virtual ~OGLDrawDevice()
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
        virtual void fillRect(const SDL_Rect *destRect, Color color);

        /*
         * blits the src surface to our surface.  Src rect defines which portion of the src to blit.
         * 
         *@param srcRect the OGL_Rect to blit from the src surface.  Use the whole surface if it's NULL
         *@param destRect the destination on our surface.  if Null it uses (0,0) as a starting point.
         *@param src the OGL_Surface from which we will blit.
         */
        virtual void blitSurface(SDL_Rect *srcRect, SDL_Rect *destRect, SDL_Surface *src);

        /*
         * creates a gradient rect on our surface size and position equal to destRect
         */
        virtual void drawGradient(const SDL_Rect *destRect, 
                                  Color leftTop, 
                                  Color rightTop, 
                                  Color leftBottom, 
                                  Color rightBottom);

        /*
         * does a full update of our surface
         */
        virtual void update();
        
	virtual void clearScreen();

	/*
	 * Setup
	 */
	virtual void init();
        //---------------------------------------------------------------------------
        // Private Methods
        //---------------------------------------------------------------------------
    private:
        void saveMatrices();
        void restoreMatrices();
    };

}

#endif
