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

#ifndef COLOURRECRENDERER_H
#define COLOURRECRENDERER_H

// Included headers from the current project
#include "services/platform/Color.h"
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
     * This contains the ColourRecRenderer class.  This class does the drawing
     * of backgrounds using a gradient or solid color.  These backgrounds
     * can make up a larger background.
     *
     * When the instance needs to be drawn, the render() member method is called.
     *
     * Ex:
     * dime::RectangleRenderer *myRectangleRenderer;
     * Rectangle myRect(0,0,64,64);
     * myRectangleRenderer = new dime::ColourRecRenderer(&myRect, 100, 100, 255);
     * myRectangleRenderer->render(myScreen);
	 *
     * 
     * @author Dean Dickison (Winand)
	 * @author Adam Gregory (Adamgreg)
     */

	class ColourRecRenderer : public RectangleRenderer
    
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

    /*
     * The color
     */
    Color myColor;

	// ===================================================================
	// Public Methods
	// ===================================================================
    public:
	
	//----------------------------------------------------------------------
	// Constructors

	/**
	 * Creates a new empty ColourRecRenderer
	 */
    ColourRecRenderer()
	{
	}
        
	/**
	 * Creates a new ColourRecRenderer for a solid color based on red,
	 * green and blue.
	 */
	ColourRecRenderer(const Rectangle &rect,
                          Uint8 red, Uint8 green, Uint8 blue);
        
	/**
	 * Creates a new ColourRecRenderer with a solid color.
	 */
	ColourRecRenderer(const Rectangle &rect, const Color &color);

	//----------------------------------------------------------------------
	// Destructor

	/**
	 * Deletes a ColourRecRenderer instance.
	 */
	virtual ~ColourRecRenderer();

	//----------------------------------------------------------------------
	// Getters

	//----------------------------------------------------------------------
	// Setters
	
	void setRect(const Rectangle &rect);

	//----------------------------------------------------------------------
	// Other public methods	

	/**
	 * Renders using a DrawDevice
	 */
	int render(DrawDevice *device);

	/**
	 * Sets this RectangleRenderer using supplied values..
	 */
	void solidColor(Uint8 red, Uint8 green, Uint8 blue);
        
	/**
	 * Sets this RectangleRenderer using supplied values..
	 */
	void solidColor(Color color);

	//======================================================================
	// Protected Methods
	//======================================================================
	protected:

	//======================================================================
	// Private Methods
	//======================================================================
	private:

	//======================================================================
	// Disabled constructors and operators
	//======================================================================
	private:


	};  // End of class

}   // End of application namespace

#endif
