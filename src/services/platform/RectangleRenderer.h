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
#include <services/platform/Color.h>
#include <services/platform/DrawDevice.h>
#include <services/platform/Rectangle.h>

// Included custom library headers

// Included system headers
#include <SDL/SDL.h>
#include <string>


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
        // Private Constants
        //======================================================================
    private:
        /**
         * Flags to specify which type of rendering should be used
         */
        enum RenderType
            {
                FLAT_COLOR,
                GRADIENT,
                BITMAP,
                GRID
            };

        
        //======================================================================
        // Private Variables
        //======================================================================
    private:
        
        /**
         * The fill type of the rectangle
         */
        RenderType  myType;
        
        /**
         * The surface that this RectangleRenderer acts upon
         */
        SDL_Surface *mySurface;
        
        /**
         * The bitmap style
         */
        BitmapStyle myStyle;
        
        /**
         * Coordinates/size of the rectangle
         */
        Rectangle myRect;
        
        /*
         * The colors
         */
        Color myColor;
        Color myColor2;
        Color myColor3;
        Color myColor4;  
        
        
        int myColor1X;
        int myColor1Y;
        int myColor2X;
        int myColor2Y;

        // ===================================================================
        // Public Methods
        // ===================================================================
    public:

        //----------------------------------------------------------------------
        // Constructors

		/**
		 * Creates a new empty RectangleRenderer
		 */
        RectangleRenderer()
		{
		}
        /**
         * Creates a new RectangleRenderer for a solid color based on red, green and blue.
         */
        RectangleRenderer(const Rectangle &rect,
                          Uint8 red, Uint8 green, Uint8 blue);
        /**
         * Creates a new RectangleRenderer with a solid color.
         */
        RectangleRenderer(const Rectangle &rect, Color color);
        
        /**
         * Creates a new RectangleRenderer with a bitmap background
         */
        RectangleRenderer(const Rectangle &rect, const std::string bitmapString, BitmapStyle syle);

        /**
         * Creates a new RectangleRenderer as a gradient
         */
        RectangleRenderer(const Rectangle &rect,
                          Color color1, Color color2, Color color3, Color color4);

        
        
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

        //----------------------------------------------------------------------
        // Other public methods	

       /**
		 * Sets this RectangleRenderer using supplied values..
		 */
        void solidColor(Uint8 red, Uint8 green, Uint8 blue);

        /**
		 * Sets this RectangleRenderer using supplied values..
		 */
        void solidColor(Color color);

        /**
		 * Sets this RectangleRenderer using supplied values..
		 */
        void gradient(Color color1, Color color2, Color color3, Color color4);
		
        /**
		 * Sets this RectangleRenderer using supplied values..
		 */
		void bitmap(const std::string bitmapString);

        /**
		 * Calls appropriate private function to render 
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
        //----------------------------------------------------------------------
        // API

        /**
		 * Renders a bitmap, for now an SDL bitmap.
		 */
        int renderBitmap(DrawDevice *device);

        /**
		 * Renders a solid color.
		 */
        int renderFlat(DrawDevice *device);

        /**
		 * Renders a gradient
		 */
        int renderGradient(DrawDevice *device);

        /**
		 * Renders a grid.  Going to be hard to do this one.
		 */
        int renderGrid(DrawDevice *device,
                       int cols,
                       int rows);
                       /*int numOfColumns, int numOfRows, float splitLineRelativePosition[2][2],
                         int splitLineOffsetPosition[2][2],
                         dime::RectangleRenderer *rectangleGrid[3][3]);*/
                       
        
        //======================================================================
        // Disabled constructors and operators
        //======================================================================
    private:


    };  // End of class

}   // End of application namespace


#endif
