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

#ifndef GRIDRENDERER_H
#define GRIDRENDERER_H

// Included headers from the current project
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
     * This contains the GridRenderer class.  This class acts as a container
	 * and marshaller for other Renderers, forming them into a grid. A common
	 * usage will be for bordered widgets.
     *
     * When the instance needs to be drawn, the render() member method is called.
     *
     * Ex:
     * dime::Renderer *myRenderer;
     * Rectangle myRect(0,0,64,64);
     * myRenderer = new dime::GridRenderer(&myRect, 100, 100, 255);
     * myRenderer->render(myScreen);
	 *
     * 
     * @author Dean Dickison (Winand)
	 * @author Adam Gregory (Adamgreg)
     */

	class GridRenderer : public Renderer
    
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
	std::vector<Renderer*> myGrid;

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
	 * Creates a new empty GridRenderer
	 */
    GridRenderer()
	{
	}

	/**
	 * Creates a new GridRenderer with the value provided
	 */
	GridRenderer(const Rectangle &rect, std::vector<Renderer*>
		&gridVector, int columns, int rows, std::vector<int> &lines);

	//----------------------------------------------------------------------
	// Destructor

	/**
	 * Deletes a GridRenderer instance.
	 */
	virtual ~GridRenderer()
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
	 * Updates the dimensions of the grid.
	 */	
	void updateGridDimensions();

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
