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
// Included custom library headers

// Included system headers
#include <SDL/SDL.h>

/**
 * Flags to specify which type of rendering should be used
 */
#define FLAT_COLOR      1
#define GRADIENT        2
#define BITMAP          3
#define GRID            4

/**
 * Gradient types
 */
#define HORIZONTAL      1
#define VERTICAL        2
#define DIAGONAL        3


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
     * SDL_Rect myRect;
     * myRect.x = 0;
     * myRect.y = 0;
     * myRect.w = 64;
     * myRect.h = 64;
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

        int myType;

        /**
         * Coordinates/size of the rectangle
         */
        SDL_Rect myRect;
        
        /*
         * The colors
         */
        Color myColor;
        Color myColor2;
        Color myColor3;
        Color myColor4;

        int myGradientType;

       


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
         * Creates a new RectangleRenderer using supplied values..
         */
        RectangleRenderer(int renderFlag, SDL_Rect *rect,
                          Uint8 red, Uint8 green, Uint8 blue);

        /**
         * Creates a new RectangleRenderer using supplied values..
         */
        RectangleRenderer(int renderFlag, SDL_Rect *rect,
                          Color color);

        /**
         * Creates a new RectangleRenderer using supplied values..
         */
        //RectangleRenderer(int renderFlag, SDL_Rect *rect,
        //                SDL_Surface *surface, const std::string bitmapString);

        /**
         * Creates a new RectangleRenderer using supplied values..
         */
        RectangleRenderer(int renderFlag, int gradientFlag, SDL_Rect *rect,
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
        int renderBitmap(DrawDevice *device, SDL_Surface *surface);

        /**
         * Renders a solid color.
         */
        int renderFlat(DrawDevice *device);

        /**
         * Renders a gradient, might have to be several implementations
         */
        int renderGradient(DrawDevice *device);

        /**
         * Render a diagonal gradient
         */
        int renderDiagonalGradient(DrawDevice *device);

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
