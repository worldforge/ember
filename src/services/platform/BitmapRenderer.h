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

#ifndef BITMAPRENDERER_H
#define BITMAPRENDERER_H

// Included headers from the current project
#include "services/image/ImageService.h"
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
     * This contains the BitmapRenderer class.  This class does the drawing
     * of backgrounds using a bitmap.  These backgrounds
     * can make up a larger background.
     *
     * When the instance needs to be drawn, the render() member method is called.
     *
     * Ex:
     * dime::Renderer *myRenderer;
     * Rectangle myRect(0,0,64,64);
     * myRenderer = new dime::BitmapRenderer(&myRect, 100, 100, 255);
     * myRenderer->render(myScreen);
	 *
     * 
     * @author Dean Dickison (Winand)
	 * @author Adam Gregory (Adamgreg)
     */

	class BitmapRenderer : public Renderer
    
	{
	//======================================================================
	// Inner Classes, Typedefs, and Enums
	//======================================================================
    public:

	/**
	 * bitmap rendering flags
	 */
	enum BitmapStyle
	{
		STRETCH,
		TILE,
		CENTER
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
        
	/**
	 * The source surface that this BitmapRenderer acts upon
	 */
	SDL_Surface *mySourceSurface;

    /*
     * The filename of the source image
     */
    std::string myImageName;
	
	/**
	 * The style of this BitmapRenderer
	 */
	BitmapStyle myStyle;

	// ===================================================================
	// Public Methods
	// ===================================================================
    public:
	
	//----------------------------------------------------------------------
	// Constructors

	/**
	 * Creates a new empty BitmapRenderer
	 */
    BitmapRenderer()
	{
	}
        
	/**
	 * Creates a new BitmapRenderer from filename of the given style.
	 */
	BitmapRenderer(const Rectangle &rect, const std::string filename, BitmapStyle style);


	//----------------------------------------------------------------------
	// Destructor

	/**
	 * Deletes a BitmapRenderer instance.
	 */
	virtual ~BitmapRenderer();

	//----------------------------------------------------------------------
	// Getters

	//----------------------------------------------------------------------
	// Setters
	
	void setRect(const Rectangle &rect);
	
	void setStyle(BitmapStyle style);
	
	void setImage(std::string filename);

	//----------------------------------------------------------------------
	// Other public methods	

	/**
	 * Renders using a DrawDevice
	 */
	void render(DrawDevice *device);

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
