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


#ifndef DRAWDEVICE_H
#define DRAWDEVICE_H

#include "Color.h"
#include <services/platform/Dimension.h>

#include <SDL/SDL.h>

namespace dime {

	/**
	 * Interface for DrawDevice.
	 *
	 * @author Lakin Wecker
	 *
	 */

class DrawDevice {
	//======================================================================
	// Protected Variables
	//======================================================================
    protected:
	
	Dimension myDimensions;

    //======================================================================
    // Public Methods
    //======================================================================
	
public:
    /**
    * Creates a new DrawDevice using default values.
    */
    DrawDevice(){}
    
    /**
    * Virtual destructor
    */
    virtual ~DrawDevice(){}
		
	//----------------------------------------------------------------------
	// Getters

	const Dimension &getDimensions() const
	{
		return myDimensions;
	}
		
	//----------------------------------------------------------------------
	// Setters

	void setDimensions(Dimension dimensions)
	{
		myDimensions = dimensions;
	}
		
	//----------------------------------------------------------------------
	// Other public methods

    /**
    * Draws a pixel of color at (x,y)
    */
    virtual void drawPixel(int x, int y, Color color) = 0;
    
    /**
    * Draws a line of color from (x1,y1) to (x2,y2)
    */
    virtual void drawLine(int x1, int y1, int x2, int y2, Color color) = 0;
    
	/**
    * Fills the rectangle destRect with color
    */
    virtual void fillRect(SDL_Rect *destRect, Color color) = 0;
    
	/*
	 * Fills destRect with a gradient produces from the four colors
	 */
    virtual void drawGradient(SDL_Rect *destRect,
                              Color leftTop, 
                              Color rightTop, 
                              Color leftBottom, 
                              Color rightBottom) = 0;
                 
    /*
    * blits the surface.  if either srcRect is null it blits the entire surface,
    * if destRect is null then it starts blitting at (0,0)
    */
    virtual void blitSurface(SDL_Rect *srcRect, SDL_Rect *destRect, SDL_Surface *src) = 0;
    virtual void update() = 0;
};
 
}

#endif
