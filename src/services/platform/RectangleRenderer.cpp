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
 *      Revision 1.3  2002-03-30 09:33:06  adamgreg
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

#include "RectangleRenderer.h"

/**
 * For now, the constructor doesn't really do anything.
 */

/**
 * Constructor for a flat solid color rectangle.
 */
dime::RectangleRenderer::RectangleRenderer(int renderFlag, SDL_Rect *rect,
        Uint8 red, Uint8 green, Uint8 blue)
{
	SolidColor(renderFlag, red, green, blue);
	myRect = *rect;
}


/**
 * Constructor for a flat solid color rectangle.
 */
dime::RectangleRenderer::RectangleRenderer(int renderFlag, SDL_Rect *rect,
        dime::Color color )
{
	SolidColor(renderFlag, color);
	myRect = *rect;
}

/**
 * Constructs a bitmap instance
 * Saves the bitmap or loads it into myBitmap.
 */
/*dime::RectangleRenderer::RectangleRenderer(int renderFlag, Rectangle *rect,
  SDL_Surface *bitmapSurface, const std::string bitmapString)
  {
  if (bitmapSurface)
  {
  myBitmap = bitmapSurface;
  }
  else
  {
  myBitmap = SDL_LoadBMP(bitmapString.c_str());
  }
  
  myType = renderFlag;
  myRect = *rect;
  }
*/

/**
 * Sets up a gradient instance
 */
dime::RectangleRenderer::RectangleRenderer(int renderFlag, 
                                           SDL_Rect *rect, dime::Color color1, dime::Color color2, dime::Color color3, dime::Color color4)
{
	Gradient(renderFlag, color1, color2, color3, color4);
	myRect = *rect;
}

/**
 * Constructor for a flat solid color rectangle.
 */
void dime::RectangleRenderer::SolidColor(int renderFlag, Uint8 red, Uint8 green, Uint8 blue)
{
	myType = renderFlag;
	myColor = Color(red,green,blue);
}


/**
 * Constructor for a flat solid color rectangle.
 */
void dime::RectangleRenderer::SolidColor(int renderFlag, dime::Color color )
{
	myType = renderFlag;
	myColor = color;
}

/**
 * Sets up a gradient
 */
void dime::RectangleRenderer::Gradient(int renderFlag, dime::Color color1, dime::Color color2, dime::Color color3, dime::Color color4)
{
	myType = renderFlag;
	myColor = color1;
	myColor2 = color2;
	myColor3 = color3;
	myColor4 = color4;
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
            //renderBitmap(device);
            break;
        }
        case GRID:
        {
            renderGrid(device,
                       3, 3);
            
            break;
        }
    }

}

/**
 * Renders a bitmap specified by the first argument
 */
int dime::RectangleRenderer::renderBitmap(dime::DrawDevice *device, SDL_Surface *surface)
{
    SDL_Rect src, dest;

    dest.x = myRect.x;
    dest.y = myRect.y;

    src.x = 0;
    src.y = 0;
    dest.w = src.w = surface->w;
    dest.h = src.h = surface->h;
    device->blitSurface(&src, &dest, surface);
    device->update();

    return (1);
}

/**
 * Renders a flat color, not much error checking though.
 */
int dime::RectangleRenderer::renderFlat(dime::DrawDevice *device)
{
    device->fillRect(&myRect, myColor);
    device->update();    
}

/**
 * I really have no idea how to do a gradient.
 * This is going to be a real pain.
 */
int dime::RectangleRenderer::renderGradient(dime::DrawDevice *device)
{
    device->drawGradient(&myRect, myColor, myColor2, myColor3, myColor4);
	device->update();
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
            colOffset = myRect.w / cols;
            rowOffset = myRect.h / rows;
        }
    else
        {
            return -1;
        }
    
    
    int topY, bottomY; //The y values of the top line and bottom sides
    int rightX, leftX;  // the x values of the right and left sides
    topY = myRect.y;
    bottomY = myRect.y+myRect.h;
    leftX = myRect.x;
    rightX = myRect.x+myRect.w;
    
    for(int index = 0; index < cols; ++index)
        {
            device->drawLine(leftX+(index*colOffset), topY, leftX+(index*colOffset), bottomY, myColor);
        }
    for(int index = 0; index < rows; ++index)
        {
            device->drawLine(leftX, topY+(index*rowOffset), rightX, topY+(index*rowOffset), myColor);
        }
    //this for loop purposely leaves off the last two lines to draw
    device->drawLine(rightX, topY, rightX, bottomY, myColor);
    device->drawLine(rightX, bottomY, leftX, bottomY, myColor);
    
    device->update();
}


dime::RectangleRenderer::~RectangleRenderer()
{

}
