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
 *      Revision 1.16  2002-08-16 23:17:25  xmp
 *      Credits to R Steinke for this round function for use where a system's libc lacks it.
 *
 *      Revision 1.14  2002/08/16 22:37:12  xmp
 *      Script to run dime from correct dir
 *
 *      Revision 1.13  2002/08/06 23:59:50  winand
 *      Stretching render now works, but only for stretching to larger height and
 *      width, trying to shrink an image using stretch probably won't work.
 *
 *      Revision 1.12  2002/08/05 17:33:52  winand
 *      Changed the constructors to initialize variables.  Removed unneccessary
 *      calls in the constructors.  Implemented the grid constructor/renderer.
 *
 *      Revision 1.11  2002/06/08 19:01:07  nikal
 *      quick fix for the Tiling problem
 *
 *      Revision 1.10  2002/05/20 22:30:28  nikal
 *      Tiling works with RectangleRenderers
 *
 *      Revision 1.8  2002/05/20 18:49:49  nikal
 *      Quick changes to RectangleRenderer, and some theme stuff
 *
 *      Revision 1.7  2002/04/15 02:00:20  nikal
 *      Stopped RRs from updating each time they drew something.  I added the update into the GuiService.cpps draw method, which updates once after drawing everything.  This removes the flicker, but doesn't speed it up much as blits are still being done each redraw. :
 *
 *      Revision 1.6  2002/04/01 07:18:46  adamgreg
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

#include "RectangleRenderer.h"
#include <math.h>
inline int dimeroundf(float x) {return x + (x > 0) ? 0.5 : -0.5;}


/**
 * Constructor for a flat solid color RectangleRenderer.
 */
dime::RectangleRenderer::RectangleRenderer(const Rectangle &rect, Uint8 red,
	Uint8 green, Uint8 blue) : myType(FLAT_COLOR),
	myRect(rect),
	myColor(Color(red, green, blue))
{
}


/**
 * Constructor for a flat solid color RectangleRenderer.
 */
dime::RectangleRenderer::RectangleRenderer(const Rectangle &rect, 
	const dime::Color &color) : 
	myType(FLAT_COLOR),
	myRect(rect),
	myColor(color)
{
}


/**
 * Constructor for a bitmap filled RectangleRenderer
 */
dime::RectangleRenderer::RectangleRenderer(const Rectangle &rect, 
	const std::string bitmapString, BitmapStyle style) : 
	myType(BITMAP),
	myStyle(style),
	myRect(rect)
{
    SDL_Rect src, dest;
    src.x = 0;
    src.y = 0;
    dest = myRect.getSDL_Rect();

    mySurface = ImageService::getInstance()->loadImage(bitmapString);
    src.w = mySurface->w;
    src.h = mySurface->h;
    myStretchedSurface = SDL_CreateRGBSurface(SDL_SWSURFACE, dest.w,
	    dest.h, mySurface->format->BitsPerPixel, 0, 0, 0, 0);

    switch (myStyle)
    {
	case STRETCH:
	    {
		float xProportion, yProportion;

		xProportion = (float)dest.w / (float)src.w;
		yProportion = (float)dest.h / (float)src.h;

		Uint32 color;
		int yCheck=0, xCheck=0, lastYCheck=0, lastXCheck=0;
		SDL_Rect smallRect;
		for (int y=0;y < src.h;y++)
		{
		    yCheck = dimeroundf( yProportion * (float)y );
		    lastXCheck = 0;
		    for (int x=0; x < src.w; x++)
		    {
			xCheck = dimeroundf( xProportion * (float)x );
			color = getpixel(mySurface, x, y);
			if ((yCheck - lastYCheck == 1) && (xCheck 
				    - lastXCheck == 1))
			{
			    putpixel(myStretchedSurface, lastXCheck, lastYCheck, color);
			}
			else if ((yCheck - lastYCheck > 0) || (xCheck
				    - lastXCheck > 0))
			{
			    smallRect.x = lastXCheck;
			    smallRect.y = lastYCheck;
			    smallRect.w = xCheck - lastXCheck;
			    smallRect.h = yCheck - lastYCheck;
			    SDL_FillRect(myStretchedSurface, &smallRect, color);
			}
			lastXCheck = xCheck;
			
		    }
		    lastYCheck = yCheck;
		}
		
		src.w = dest.w;
		src.h = dest.h;
		break;
	    }
    case TILE:
    {
	SDL_Rect curDest;
	curDest.w = src.w;
	curDest.h = src.h;
	for(curDest.x = 0;curDest.x < dest.w; curDest.x += src.w)
	{
	    for (curDest.y = 0; curDest.y < dest.h;curDest.y += src.h)
	    {
		SDL_BlitSurface(mySurface, &src, myStretchedSurface, &curDest);
		//device->blitSurface(&src, &curDest, mySurface);
	    }
	}
	break;
	}
    }
}


/**
 * Constructor for a gradient filled RectangleRenderer
 */
dime::RectangleRenderer::RectangleRenderer(const Rectangle &rect, 
	dime::Color color1, dime::Color color2, dime::Color color3, 
	dime::Color color4) :
	myType(GRADIENT),
	myRect(rect),
	myColor(color1),
	myColor2(color2),
	myColor3(color3),
	myColor4(color4)
{
}


/**
 * Constructor for a grid object
 */
dime::RectangleRenderer::RectangleRenderer(const Rectangle &rect,
	std::vector<RectangleRenderer> &gridVector, int columns, int rows,
	std::vector<int> &lines) : 
	myType(GRID),
	myRect(rect),
	myGrid(gridVector),
	myLines(lines),
	myColumns(columns),
	myRows(rows)
{
    updateGridDimensions();
}


/**
 * Updates dimensions of all the grid's rectangles.
 */
void dime::RectangleRenderer::updateGridDimensions()
{
    int xOffset, yOffset;
    int rectHeight, rectWidth;

    yOffset = 0;
    for (int i=0;i < myRows;i++)
    {
	if (i == myRows - 1)
	{
	    rectHeight = myRect.getHeight() - myLines[myRows + myColumns - 3];
	}
	else
	{
	    rectHeight = myLines[myColumns - 1 + i];
	}

	xOffset = 0;
	for (int j=0;j < myColumns;j++)
	{
	    if (j == myColumns - 1)
	    {
		rectWidth = myRect.getWidth() - myLines[myColumns - 2];
	    }
	    else
	    {
		rectWidth = myLines[j];
	    }

	    myGrid[(i * myColumns)+j].setRect(dime::Rectangle(myRect.getX()
			+ xOffset, myRect.getY()+yOffset, rectWidth,
			rectHeight));

	    xOffset = rectWidth;
	}

	yOffset = rectHeight;
    }
}


/**
 * Setter for the myRect member
 */
void dime::RectangleRenderer::setRect(const Rectangle &rect)
{
    myRect = rect;
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
            renderGrid(device);
            break;
        }
    }

	//TODO: What should be returned here?
	return 0;
}


/**
 * Renders a bitmap
 */
void dime::RectangleRenderer::renderBitmap(dime::DrawDevice *device)
{
    SDL_Rect src, dest;
    src.x = 0;
    src.y = 0;
    src.w = myStretchedSurface->w;
    src.h = myStretchedSurface->h;
    dest = myRect.getSDL_Rect();

    switch(myStyle)
        {
        case TILE:
	    {
		device->blitSurface(&src, &dest, myStretchedSurface);
		break;
	    }
        case STRETCH:
	    {
		src.w = dest.w;
		src.h = dest.h;
		device->blitSurface(&src, &dest, myStretchedSurface);
	       break;

	    }
	case CENTER:
	default:
	    {
	    break;
	    }
        }
}


/**
 * Renders a flat color, not much error checking though.
 */
void dime::RectangleRenderer::renderFlat(dime::DrawDevice *device)
{
    device->fillRect(&myRect.getSDL_Rect(), myColor);
	
	//TODO: What should be returned here?
}


/**
 * Renders a gradient fill, blending a color from each corner.
 */
void dime::RectangleRenderer::renderGradient(dime::DrawDevice *device)
{
    device->drawGradient(&myRect.getSDL_Rect(), myColor, myColor2, myColor3, myColor4);

	//TODO: What should be returned here?
}


/**
 * Renders a grid object
 */
void dime::RectangleRenderer::renderGrid(dime::DrawDevice *device)
                                        /*
                                          int nrOfColumns, int nrOfRows, float splitLineRelativePosition[2][2],
                                          int splitLineOffsetPosition[2][2],
                                          dime::RectangleRenderer *rectangleGrid[3][3])
                                        */
{
    /*int colOffset, rowOffset;
    if(rows >0 && cols >0)
        {
            colOffset = myRect.getWidth() / cols;
            rowOffset = myRect.getHeight() / rows;
        }
    else
        {
            return -1;
        }*/
    
    for (int i=0;i < myColumns*myRows;i++)
    {
	myGrid[i].render(device);
    }
    
    /*int topY, bottomY; //The y values of the top line and bottom sides
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

	//TODO: What should be returned here?
	return 0;

	*/
}


dime::RectangleRenderer::~RectangleRenderer()
{

}


Uint32 getpixel(SDL_Surface *surface, int x, int y)
{
    int bpp = surface->format->BytesPerPixel;
    /* Here p is the address to the pixel we want to retrieve */
    Uint8 *p = (Uint8 *)surface->pixels + y * surface->pitch + x * bpp;

    switch(bpp) {
    case 1:
        return *p;

    case 2:
        return *(Uint16 *)p;

    case 3:
        if(SDL_BYTEORDER == SDL_LIL_ENDIAN)
            return p[0] << 16 | p[1] << 8 | p[2];
        else
            return p[0] | p[1] << 8 | p[2] << 16;
    case 4:
        return *(Uint32 *)p;

    default:
        return 0;       /* shouldn't happen, but avoids warnings */
    }
}


void putpixel(SDL_Surface *surface, int x, int y, Uint32 pixel)
{
    int bpp = surface->format->BytesPerPixel;
    /* Here p is the address to the pixel we want to set */

    switch(bpp) {
    case 1: {
	Uint8 *bufp;
	bufp = (Uint8 *)surface->pixels + y*surface->pitch + x;
	*bufp = pixel;
        break;
	    }
    case 2: {
	Uint16 *bufp;
	bufp = (Uint16 *)surface->pixels + y*surface->pitch/2 + x;
	*bufp = pixel;
        break;
	    }
    case 3: {
	Uint8 *bufp;
	bufp = (Uint8 *)surface->pixels + y*surface->pitch + x * 3;
        if(SDL_BYTEORDER == SDL_LIL_ENDIAN) {
	    bufp[0] = pixel;
	    bufp[1] = pixel >> 8;
	    bufp[2] = pixel >> 16;
        } else {
	    bufp[2] = pixel;
	    bufp[1] = pixel >> 8;
	    bufp[0] = pixel >> 16;
        }
        break;
	    }
    case 4: {
	Uint32 *bufp;
	bufp = (Uint32 *)surface->pixels + y*surface->pitch/4 + x;
	*bufp = pixel;
        break;
	    }
    }
}
