/*
 *  File:       RectangleRenderer.cpp
 *  Summary:    The class for rendering backgrounds.
 *  Written by: Dean Dickison, aka Winand
 *
 *  Copyright (C) 2002, Winand.
 *  This code is distributed under the GPL.
 *  See file COPYING for details. 
 *
 *  Change History (most recent first):    
 *
 *      $Log$
 *      Revision 1.6  2002-04-01 07:18:46  adamgreg
 *
 *      Fixed segfault when destroying some widgets. Added standard and highlight RectangleRenderers to Button. Made RectangleRenderers use the new Rectangle class. Added another (blank for now) constructor to RectangleRenderer to make it fetch themescheme info to initialize itself with.
 *
 *      Revision 1.5  2002/03/31 19:15:46  tim
 *      Bugfixes, MSVC compatibility fixes, Since boost is working ImageService is now caching
 *
 *      Revision 1.4  2002/03/31 08:42:06  adamgreg
 *
 *      Added Rectangle class for Widget to use. Later should make RR use it too so that people creating widgets don't have to worry about calling Rectangle::getSDL_Rect(). Kept Dimension, for defining max/min/pref dimensions. Quite a few functions in Rectangle so that they'll play nicely together.
 *      Messed a bit with nikal's ImageService - added image caching. There's a complete implementation using Boost shared pointers, which is commented out for now because I don't know if many people have or want the boost dev files. Incomplete implementation that loads but doesn't unload from cache properly.
 *      Did some tidying of RectangleRenderer, and tried to make it a bit safer and easier to use. And implemented bitmap type RRs using ImageService and DrawDevice.
 *      ...pretty bitmapped buttons!
 *      (now nikal won't shout at me :)
 *
 *      Revision 1.3  2002/03/30 09:33:06  adamgreg
 *
 *      Input now successfully obtained by GuiService from InputService. Button Widget added. Widget events work. Proper use of RectangleRenderers.
 *      The upshot is : pretty thing on screen that does stuff. Check it out!
 *
 *      Revision 1.2  2002/03/30 05:17:34  nikal
 *      commiting fixed RectangleRenderers, which compile and use DrawDevice's to do their rendering.  Makefiles should now also try and compile the platform stuff via automake etc.
 *
 *      Revision 1.1  2002/03/22 01:33:00  winand
 *      *** empty log message ***
 *
 *      Revision 1.0 2002/03/17 11:52:35 Winand
 *      Initial code for drawing rectangles
 *
 *      Revision 1.1 2002/03/17 14:14:05 Winand
 *      Cleanup of the code, making it more useful as well
 *
 */

#include <string>

#include <services/image/ImageService.h>

#include "RectangleRenderer.h"

/**
 * For now, the constructor doesn't really do anything.
 */

/**
 * Constructor for a flat solid color RectangleRenderer.
 */
dime::RectangleRenderer::RectangleRenderer(int renderFlag, const Rectangle &rect,
        Uint8 red, Uint8 green, Uint8 blue)
{
	solidColor(red, green, blue);
	myRect = rect;
}


/**
 * Constructor for a flat solid color RectangleRenderer.
 */
dime::RectangleRenderer::RectangleRenderer(int renderFlag, const Rectangle &rect,
        dime::Color color )
{
	solidColor(color);
	myRect = rect;
}

/**
 * Constructor for a bitmap filled RectangleRenderer
 */
dime::RectangleRenderer::RectangleRenderer(int renderFlag, const Rectangle &rect, const std::string bitmapString)
{
	bitmap(bitmapString);
	myRect = rect;
}


/**
 * Constructor for a gradient filled RectangleRenderer
 */
dime::RectangleRenderer::RectangleRenderer(int renderFlag, 
                                           const Rectangle &rect, dime::Color color1, dime::Color color2, dime::Color color3, dime::Color color4)
{
	gradient(color1, color2, color3, color4);
	myRect = rect;
}

/**
 * Sets up a flat solid color RectangleRenderer.
 */
void dime::RectangleRenderer::solidColor(Uint8 red, Uint8 green, Uint8 blue)
{
	myColor = Color(red,green,blue);
	myType = FLAT_COLOR;
}


/**
 * Sets up a flat solid color RectangleRenderer.
 */
void dime::RectangleRenderer::solidColor(dime::Color color)
{
	myColor = color;
	myType = FLAT_COLOR;
}

/**
 * Sets up a gradient
 */
void dime::RectangleRenderer::gradient(dime::Color color1, dime::Color color2, dime::Color color3, dime::Color color4)
{
	myColor = color1;
	myColor2 = color2;
	myColor3 = color3;
	myColor4 = color4;
	myType = GRADIENT;
}

/**
 * Sets up a bitmap
 */
void dime::RectangleRenderer::bitmap(std::string bitmapString)
{
	mySurface = ImageService::getInstance()->loadImage(bitmapString);
	myType = BITMAP;
}


/**
 * Looks at the render flag and call the appropriate rendering function
 */
int dime::RectangleRenderer::render(dime::DrawDevice *device)
{
    switch (myType)
    {
        case FLAT_COLOR:
        {
            renderFlat(device);
            break;
        }
        case GRADIENT:
        {
			renderGradient(device);
            break;
        }
        case BITMAP:
        {
            renderBitmap(device);
            break;
        }
        case GRID:
        {
            renderGrid(device, 3, 3);
            break;
        }
    }

	//TODO: What should be returned here?
	return 0;
}

/**
 * Renders a bitmap
 */
int dime::RectangleRenderer::renderBitmap(dime::DrawDevice *device)
{
    SDL_Rect src, dest;
	src.x = 0;
	src.y = 0;
	src.w = myRect.getWidth();
	src.h = myRect.getHeight();
	dest = myRect.getSDL_Rect();
    device->blitSurface(&src, &dest, mySurface);
    device->update();

    return (1);
}

/**
 * Renders a flat color, not much error checking though.
 */
int dime::RectangleRenderer::renderFlat(dime::DrawDevice *device)
{
    device->fillRect(&myRect.getSDL_Rect(), myColor);
    device->update();
	
	//TODO: What should be returned here?
	return 0;
}

/**
 * Renders a gradient fill, blending a color from each corner.
 */
int dime::RectangleRenderer::renderGradient(dime::DrawDevice *device)
{
    device->drawGradient(&myRect.getSDL_Rect(), myColor, myColor2, myColor3, myColor4);
	device->update();

	//TODO: What should be returned here?
	return 0;
}

int dime::RectangleRenderer::renderGrid(dime::DrawDevice *device,
                                        int cols, int rows)
                                        /*
                                          int nrOfColumns, int nrOfRows, float splitLineRelativePosition[2][2],
                                          int splitLineOffsetPosition[2][2],
                                          dime::RectangleRenderer *rectangleGrid[3][3])
                                        */
{
    int colOffset, rowOffset;
    if(rows >0 && cols >0)
        {
            colOffset = myRect.getWidth() / cols;
            rowOffset = myRect.getHeight() / rows;
        }
    else
        {
            return -1;
        }
    
    
    int topY, bottomY; //The y values of the top line and bottom sides
    int rightX, leftX;  // the x values of the right and left sides
    topY = myRect.getY();
    bottomY = myRect.getY()+myRect.getHeight();
    leftX = myRect.getX();
    rightX = myRect.getX()+myRect.getWidth();
    
	int index;

    for(index = 0; index < cols; ++index)
        {
            device->drawLine(leftX+(index*colOffset), topY, leftX+(index*colOffset), bottomY, myColor);
        }
    for(index = 0; index < rows; ++index)
        {
            device->drawLine(leftX, topY+(index*rowOffset), rightX, topY+(index*rowOffset), myColor);
        }
    //this for loop purposely leaves off the last two lines to draw
    device->drawLine(rightX, topY, rightX, bottomY, myColor);
    device->drawLine(rightX, bottomY, leftX, bottomY, myColor);
    
    device->update();

	//TODO: What should be returned here?
	return 0;
}


dime::RectangleRenderer::~RectangleRenderer()
{

}
