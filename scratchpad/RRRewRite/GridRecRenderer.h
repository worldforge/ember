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

#ifndef GRIDRECRENDERER_H
#define GRIDRECERENDERER_H

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
     * This contains the GridRecRenderer class.  This class acts as a container
	 * and marshaller for other RectangleRenderers, forming them into a grid. A common
	 * usage will be for bordered widgets.
     *
     * When the instance needs to be drawn, the render() member method is called.
     *
     * Ex:
     * dime::RectangleRenderer *myRectangleRenderer;
     * Rectangle myRect(0,0,64,64);
     * myRectangleRenderer = new dime::GridRecRenderer(&myRect, 100, 100, 255);
     * myRectangleRenderer->render(myScreen);
	 *
     * 
     * @author Dean Dickison (Winand)
	 * @author Adam Gregory (Adamgreg)
     */

	class GridRecRenderer : public RectangleRenderer
    
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
	 * Vector of RectRenderers kept for a grid.
	 */
	std::vector<RectangleRenderer> myGrid;

	/**
	 * Where the separation between rectangles is made.
	 */
	std::vector<int> myLines;

	/**
	 * Rows and columns for the grid
	 */
	int myColumns;
	int myRows;

	// ===================================================================
	// Public Methods
	// ===================================================================
    public:
	
	//----------------------------------------------------------------------
	// Constructors

	/**
	 * Creates a new empty GridRecRenderer
	 */
    GridRecRenderer()
	{
	}

	/**
	 * Creates a new GridRecRenderer with the value provided
	 */
	GridRecRenderer(const Rectangle &rect, std::vector<RectangleRenderer>
		&gridVector, int columns, int rows, std::vector<int> &lines);
        
	/**
	 * Creates a new GridRecRenderer for a solid color based on red,
	 * green and blue.
	 */
	GridRecRenderer(const Rectangle &rect,
                          Uint8 red, Uint8 green, Uint8 blue);
        
	/**
	 * Creates a new GridRecRenderer with a solid color.
	 */
	GridRecRenderer(const Rectangle &rect, const Color &color);

	//----------------------------------------------------------------------
	// Destructor

	/**
	 * Deletes a GridRecRenderer instance.
	 */
	virtual ~GridRecRenderer();

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
