/*
    Copyright (C) 2002  Dean Dickison, Martin Pollard

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

#ifndef RENDERER_H
#define RENDERER_H

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
     * This contains the Renderer class.  This class does the drawing
     * of backgrounds using an image, gradient or solid color.  These backgrounds
     * can make up a larger background.
     *
     * Instances of the class are created by one of four constructor methods,
     * each one storing appropriate variables that describe the rectangle.
     * When the instance needs to be drawn, the render() member method is called.
     * A grid of Renderers can be made using the GRID constructor.
     *
     * Ex:
     * dime::Renderer *myRenderer;
     * Rectangle myRect(0,0,64,64);
     * myRenderer = new dime::Renderer(FLAT_COLOR,
     *     &myRect, 100, 100, 255);
     * myRenderer->render(myScreen);
     * 
     * @author Dean Dickison (Winand)
     * @author Adam Gregory (Adamgreg)
     *
     * @todo Renaming. xRecRenderer -> xRenderer. RRFactory -> RFactory/RendererFactory.
     * @todo Make into base class of FontRenderer if suitable.
     */

    class Renderer
    
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
	// Protected Variables
	//======================================================================
    protected:     

	/**
	 * The surface that this Renderer acts upon
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
	
	//----------------------------------------------------------------------
	// Constructors

	/**
	 * Creates a new empty Renderer
	 */
	Renderer() {};
	
	/**
	 * Creates a new Renderer using values provided.
	 */
	Renderer(const Rectangle &rect) : myRect(rect) {};

	//----------------------------------------------------------------------
	// Destructor

	/**
	 * Deletes a Renderer instance.
	 */
	virtual ~Renderer();

	//----------------------------------------------------------------------
	// Getters

	//----------------------------------------------------------------------
	// Setters

	/**
	 * Setter for the myRect member
	 */
	void setRect(const Rectangle &rect)
	{
	  myRect = rect;
	};

	//----------------------------------------------------------------------
	// Other public methods	

	/**
	 * Calls appropriate private function to render 
	 */
	virtual void render(DrawDevice *device) = 0;

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
