/*
    Copyright (C) 2002  Dean Dickison

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

#ifndef RECTANGLERENDERER_H
#define RECTANGLERENDERER_H

// Included headers from the current project
#include "services/image/ImageService.h"
#include "services/platform/Color.h"
#include "services/platform/DrawDevice.h"
#include "services/platform/Rectangle.h"

// Included custom library headers

// Included system headers
#include <SDL/SDL.h>
#include <string>
#include <vector>
#include <math.h>


namespace dime {

    /**
     * This contains the RectangleRenderer class.  This class does the drawing
     * of backgrounds using an image, gradient or solid color.  These backgrounds
     * can make up a larger background.
     *
     * Instances of the class are created by one of four constructor methods,
     * each one storing appropriate variables that describe the rectangle.
     * When the instance needs to be drawn, the render() member method is called.
     * A grid of RectangleRenderers can be made using the GRID constructor.
     *
     * Ex:
     * dime::RectangleRenderer *myRectangleRenderer;
     * Rectangle myRect(0,0,64,64);
     * myRectangleRenderer = new dime::RectangleRenderer(FLAT_COLOR,
     *     &myRect, 100, 100, 255);
     * myRectangleRenderer->render(myScreen);
     * 
     * @author Dean Dickison (Winand)
     */

    class RectangleRenderer
    
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
	 * The surface that this RectangleRenderer acts upon
	 */
	SDL_Surface *mySurface;

	/**
	 * Coordinates/size of the rectangle
	 */
	Rectangle myRect;

	// ===================================================================
	// Public Methods
	// ===================================================================
    public:

	/**
	 * Calls appropriate private function to render 
	 */
	int render(DrawDevice *device);
	
	//----------------------------------------------------------------------
	// Constructors

	/**
	 * Creates a new empty RectangleRenderer
	 */
	RectangleRenderer()
	{
	}
        
        
	/**
	 * Creates a new RectangleRenderer for a solid color based on red,
	 *green and blue.
	 */
	 RectangleRenderer(const Rectangle &rect,
                          Uint8 red, Uint8 green, Uint8 blue);
        
        
	/**
	 * Creates a new RectangleRenderer with a solid color.
	 */
	RectangleRenderer(const Rectangle &rect, const Color &color);
        
        
	/**
	 * Creates a new RectangleRenderer with a bitmap background
	 */
	RectangleRenderer(const Rectangle &rect, const std::string bitmapString, BitmapStyle syle);

        
	/**
	 * Creates a new RectangleRenderer as a gradient
	 */
	RectangleRenderer(const Rectangle &rect, Color color1, Color color2,
		Color color3, Color color4);

	
	/**
	 * Constructor for a grid object.
	 */
	RectangleRenderer(const Rectangle &rect, std::vector<RectangleRenderer>
		&gridVector, int columns, int rows, std::vector<int> &lines);
        
	//----------------------------------------------------------------------
	// Destructor

	/**
	 * Deletes a RectangleRenderer instance.
	 */
	virtual ~RectangleRenderer();

	//----------------------------------------------------------------------
	// Getters

	//----------------------------------------------------------------------
	// Setters
	
	virtual void setRect(const Rectangle &rect);

	//----------------------------------------------------------------------
	// Other public methods	

	/**
	 * Sets this RectangleRenderer using supplied values..
	 */
	virtual void solidColor(Uint8 red, Uint8 green, Uint8 blue);
        

	/**
	 * Sets this RectangleRenderer using supplied values..
	 */
	virtual void solidColor(Color color);
        

	/**
	 * Sets this RectangleRenderer using supplied values..
	 */
	virtual void gradient(Color color1, Color color2, Color color3, Color color4);
		
        
	/**
	 * Sets this RectangleRenderer using supplied values..
	 */
	virtual void bitmap(const std::string bitmapString);
        

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

//what are these for? aren't they declared in the SDL header?
Uint32 getpixel(SDL_Surface *surface, int x, int y);
void putpixel(SDL_Surface *surface, int x, int y, Uint32 pixel);

#endif
