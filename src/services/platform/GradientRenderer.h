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

#ifndef GRADIENTRENDERER_H
#define GRADIENTRENDERER_H

// Included headers from the current project
#include "Color.h"
#include "DrawDevice.h"
#include "Rectangle.h"
#include "Renderer.h"

// Included custom library headers

// Included system headers
#include <SDL/SDL.h>
#include <string>
#include <vector>
#include <math.h>


namespace dime {

    /**
     * This contains the GradientRenderer class.  This class does the drawing
     * of backgrounds using a gradient or solid color.  These backgrounds
     * can make up a larger background.
     *
     * When the instance needs to be drawn, the render() member method is called.
     *
     * Ex:
     * dime::Renderer *myRectangleRenderer;
     * Rectangle myRect(0,0,64,64);
     * myRectangleRenderer = new dime::GradientRenderer(&myRect, 100, 100, 255);
     * myRectangleRenderer->render(myScreen);
	 *
     * 
     * @author Dean Dickison (Winand)
	 * @author Adam Gregory (Adamgreg)
     */

	class GradientRenderer : public Renderer
    
	{
	//======================================================================
	// Inner Classes, Typedefs, and Enums
	//======================================================================
    public:
	/**
	 * bitmap rendering flags
	 */
	enum GradientStyle
	{
	HORIZONTAL,
	VERTICAL
	};
    
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
     * The colors
     */
    Color myColor;
	Color myColor2;
	Color myColor3;
	Color myColor4;

	// ===================================================================
	// Public Methods
	// ===================================================================
    public:
	
	//----------------------------------------------------------------------
	// Constructors

	/**
	 * Creates a new empty GradientRenderer
	 */
    GradientRenderer()
	{
	}
        
	/**
	 * Creates a new GradientRenderer for a 4-corner gradient
	 */
	GradientRenderer(const Rectangle &rect, Color topleft, Color topright, Color bottomleft, Color bottomright);
        
	/**
	 * Creates a new GradientRenderer with a horizontal/vertical gradient.
	 */
	GradientRenderer(const Rectangle &rect, Color color1, Color color2, GradientStyle style);

	//----------------------------------------------------------------------
	// Destructor

	/**
	 * Deletes a GradientRenderer instance.
	 */
	virtual ~GradientRenderer()
	{
	};

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
	void render(DrawDevice *device);

	/**
	 * Sets this Renderer using supplied values..
	 */
	void Gradient(Color topleft, Color topright, Color bottomleft, Color bottomright);
        
	/**
	 * Sets this Renderer using supplied values..
	 */
	void Gradient(Color color1, Color color2, GradientStyle style);

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
