/*
    Copyright (C) 2002  Adam Gregory

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

#ifndef BORDERRENDERER_H
#define BORDERRENDERER_H

// Included headers from the current project
#include "services/platform/DrawDevice.h"
#include "services/platform/Rectangle.h"
#include "services/platform/RectangleRenderer.h"

// Included custom library headers

// Included system headers
#include <SDL/SDL.h>
#include <string>
#include <vector>
#include <math.h>


namespace dime {

    /**
     * This contains the BorderRenderer class.  This class creates a border around a given rectangular area
	 * This border is composed of 8 RectangleRenderers created from a prototype.
     *
     * When the instance needs to be drawn, the render() member method is called.
     *
     * Ex:
     * dime::RectangleRenderer *myRectangleRenderer;
     * Rectangle myRect(0,0,64,64);
     * myRectangleRenderer = new dime::BorderRenderer(myRect, 4, new dime::ColorRenderer(100, 100, 255));
     * myRectangleRenderer->render(myScreen);
	 *
     * 
  	 * @author Adam Gregory (Adamgreg)
     */

	class BorderRenderer : public RectangleRenderer
    
	{
	//======================================================================
	// Inner Classes, Typedefs, and Enums
	//======================================================================
    public:

    
	//======================================================================
	// Public Constants
	//======================================================================
    public:
        

	//======================================================================
	// Private Constants
	//======================================================================
    private:
        
		
	//======================================================================
	// Private Variables
	//======================================================================
    private:
	
	/**
	 * Vector of RectangleRenderers kept for lines (and corners) of the border.
	 * items of the vector from top left corner, clockwise.
	 * e.g. the line constituting the bottom of the border is myLines[5].
	 */
	std::vector<RectangleRenderer*> myLines;

	/**
	 * Width of the border.
	 */
	unsigned int myWidth;

	// ===================================================================
	// Public Methods
	// ===================================================================
    public:
	
	//----------------------------------------------------------------------
	// Constructors

	/**
	 * Creates a new empty BorderRenderer
	 */
    BorderRenderer()
	{
	}

	/**
	 * Creates a new BorderRenderer with the value provided
	 */
	BorderRenderer(const Rectangle &rect, unsigned int width, RectangleRenderer* prototype);

	//----------------------------------------------------------------------
	// Destructor

	/**
	 * Deletes a BorderRenderer instance.
	 */
	virtual ~BorderRenderer()
	{
	};

	//----------------------------------------------------------------------
	// Getters

	//----------------------------------------------------------------------
	// Setters
	
	void setRect(const Rectangle &rect);
	
	void setWidth(unsigned int width);

	//----------------------------------------------------------------------
	// Other public methods	

	/**
	 * Renders using a DrawDevice
	 */
	void render(DrawDevice *device);
	
	/**
	 * Sets up the border's RectangleRenderers based on a prototype.
	 */
	void setBorderContents(RectangleRenderer *prototype);
	
	/**
	 * Sets the positions and sizes of the border's RectangleRenderers.
	 */
	void generateBorder();


	//======================================================================
	// Protected Methods
	//======================================================================
	protected:

	//======================================================================
	// Private Methods
	//======================================================================
	private:
	
	/**
	 * Checks that the contents of myLines have been initialized
	 */
	 bool checkLines()
	 {
		 for (Uint8 i = 0; i < 8; i++)
		{
			if (myLines[i] == NULL)
			{
				// Log error with LoggingService.
				return false;
			}
		}
		return true;
	}

	//======================================================================
	// Disabled constructors and operators
	//======================================================================
	private:


	};  // End of class

}   // End of application namespace

#endif
