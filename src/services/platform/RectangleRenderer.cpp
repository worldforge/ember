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
 *      Revision 1.1  2002-03-22 01:33:00  winand
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
dime::RectangleRenderer::RectangleRenderer(int renderFlag, Rectangle *rect,
        Uint8 red, Uint8 green, Uint8 blue)
{
    myType          = renderFlag;
    myRect          = *rect;
    myColor.red     = red;
    myColor.green   = green;
    myColor.blue    = blue;

}

/**
 * Constructs a bitmap instance
 * Saves the bitmap or loads it into myBitmap.
 */
dime::RectangleRenderer::RectangleRenderer(int renderFlag, Rectangle *rect,
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

/**
 * Sets up a gradient instance
 */
dime::RectangleRenderer::RectangleRenderer(int renderFlag, int gradientFlag, 
        Rectangle *rect, Color *color1, Color *color2, Color *color3,
        Color *color4, int color1X, int color1Y, int color2X, int color2Y)
{
    myType = renderFlag;
    myGradientType = gradientFlag;
    myRect = *rect;
    myColor1 = *color1;
    myColor2 = *color2;
    myColor3 = *color3;
    myColor4 = *color4;

    myColor1X = color1X;
    myColor1Y = color1Y;
    myColor2X = color2X;
    myColor2Y = color2Y;
}


/**
 * Looks at the render flag and call the appropriate rendering function
 */
int dime::RectangleRenderer::render(SDL_Surface *renderSurface)
        /**
         * This doesn't work very well, heh
        Rectangle *rect, Color *color=NULL, int color1=0, int color2=0,
        int color3=0, int color4=0, SDL_Surface *bitmap=NULL,
        int nrOfColumns=0, int nrOfRows=0,
        float splitLineRelativePosition[2][2]=NULL,
        int splitLineOffsetPosition[2][2]=NULL,
        dime::RectangleRenderer *rectangleGrid[3][3]=NULL)
        */
{
    switch (myType)
    {
        case FLAT_COLOR:
        {
            renderFlat(renderSurface);
            break;
        }
        case GRADIENT:
        {
            if (myGradientType != DIAGONAL)
            {
                renderGradient(renderSurface);
            }
            else
            {
                renderDiagonalGradient(renderSurface);
            }
            break;
        }
        case BITMAP:
        {
            renderBitmap(renderSurface);
            break;
        }
        case GRID:
        {
            /*renderGrid(renderSurface, nrOfColumns, nrOfRows, 
                    splitLineRelativePosition, splitLineOffsetPosition,
                    rectangleGrid);*/
            break;
        }
    }

}

/**
 * Renders a bitmap specified by the first argument
 */
int dime::RectangleRenderer::renderBitmap(SDL_Surface *rootSurface)
{
    SDL_Rect src, dest;

    dest.x = myRect.x;
    dest.y = myRect.y;

    src.x = 0;
    src.y = 0;
    dest.w = src.w = myBitmap->w;
    dest.h = src.h = myBitmap->h;

    SDL_BlitSurface(myBitmap , &src, rootSurface, &dest);

    if ( SDL_MUSTLOCK(rootSurface) ) {
        SDL_UnlockSurface(rootSurface);
    }
    SDL_UpdateRect(rootSurface, dest.x, dest.y, dest.w, dest.h);

    return (1);
}

/**
 * Renders a flat color, not much error checking though.
 */
int dime::RectangleRenderer::renderFlat(SDL_Surface *rootSurface)
{
    Uint32 fullColor;
    SDL_Rect flatRect, blahRect;

    flatRect.x = myRect.x;
    flatRect.y = myRect.y;
    flatRect.h = myRect.height;
    flatRect.w = myRect.width;
    blahRect = flatRect;

    fullColor = SDL_MapRGB(rootSurface->format, myColor.red, myColor.green,
            myColor.blue);
    SDL_FillRect(rootSurface, &flatRect, fullColor);

    if ( SDL_MUSTLOCK(rootSurface) ) {
        SDL_UnlockSurface(rootSurface);
    }
    SDL_UpdateRect(rootSurface, myRect.x, myRect.y, (myRect.x + myRect.width),
            (myRect.y + myRect.height));
}

/**
 * I really have no idea how to do a gradient.
 * This is going to be a real pain.
 */
int dime::RectangleRenderer::renderGradient(SDL_Surface *rootSurface)
{
    int redDistance, red;
    int greenDistance, green;
    int blueDistance, blue;
    Uint32 fullColor;

    // slow code
    // all variables are int
    redDistance   = myColor1.red - myColor2.red;
    greenDistance = myColor1.green - myColor2.green;
    blueDistance = myColor1.blue - myColor2.blue;

    printf("%d, %d, %d\n", redDistance, greenDistance, blueDistance);
    printf("%d, %d, %d\n", myColor1.red, myColor.green, myColor.blue);
    printf("%d, %d, %d, %d\n", myRect.x, myRect.y, myRect.width, myRect.height);

    int len = myRect.height;
    int width = myRect.width;
    for (int j=0;j<width;j++)
    {
        for(int i=0;i<len;i++)
        {
            /* 255 + (255*0)/64 = 255
             * 0 + (0*0)/64 = 0
             * 0 + (0*0)/64 = 0 */
            red = myColor1.red + ((redDistance*i)/len);
            green = myColor1.green + ((greenDistance*i)/len);
            blue = myColor1.blue + ((blueDistance*i)/len);

            fullColor = SDL_MapRGB(rootSurface->format, red, green, blue);
            drawPixel(rootSurface, myRect.x + j, myRect.y + i, fullColor);
        }
    }
    drawPixel(rootSurface, myRect.x+32, myRect.y+32,
            fullColor);
    SDL_UpdateRect(rootSurface, myRect.x, myRect.y, myRect.x+myRect.width,
            myRect.y+myRect.height);
    //drawScene(rootSurface);

}

int dime::RectangleRenderer:renderDiagonalGradient(SDL_Surface *rootSurface)
{
}

int dime::RectangleRenderer::renderGrid(SDL_Surface *rootSurface,
        int nrOfColumns, int nrOfRows, float splitLineRelativePosition[2][2],
        int splitLineOffsetPosition[2][2],
        dime::RectangleRenderer *rectangleGrid[3][3])
{


}

void dime::RectangleRenderer::drawPixel(SDL_Surface *screen, int x, int y,
        Uint32 color)
{
    switch (screen->format->BytesPerPixel)
    {
        case 1: // Assuming 8-bpp
        {
            Uint8 *bufp;
            bufp = (Uint8 *)screen->pixels + y*screen->pitch + x;
            *bufp = color;
        }
        break;
        case 2: // Probably 15-bpp or 16-bpp
        {
            Uint16 *bufp;
            bufp = (Uint16 *)screen->pixels + y*screen->pitch/2 + x;
            *bufp = color;
        }
        break;
        case 3: // Slow 24-bpp mode, usually not used
        {
            Uint8 *bufp;
            bufp = (Uint8 *)screen->pixels + y*screen->pitch + x * 3;
            if(SDL_BYTEORDER == SDL_LIL_ENDIAN)
            {
                bufp[0] = color;
                bufp[1] = color >> 8;
                bufp[2] = color >> 16;
            } else {
                bufp[2] = color;
                bufp[1] = color >> 8;
                bufp[0] = color >> 16;
            }
        }
        break;
        case 4: // Probably 32-bpp
        {
            Uint32 *bufp;
            bufp = (Uint32 *)screen->pixels + y*screen->pitch/4 + x;
            *bufp = color;
        }
        break;
    }
}

int dime::RectangleRenderer::drawScene(SDL_Surface *rootSurface)
{
    int nothing;
    Uint32 fullColor;
    nothing = sLock(rootSurface);
    for(int x=myRect.x;x<myRect.x+myRect.width;x++)
    {
        for(int y=myRect.y;y<myRect.x+myRect.width;y++)
        {
            fullColor = SDL_MapRGB(rootSurface->format, y/2, y/2,
                x/3);
            drawPixel(rootSurface, x,y,fullColor);
        }
    }
    sULock(rootSurface);
    SDL_UpdateRect(rootSurface, myRect.x, myRect.y, myRect.x+myRect.width,
            myRect.y+myRect.height);
}

int dime::RectangleRenderer::sLock(SDL_Surface *rootSurface)
{
    if ( SDL_MUSTLOCK(rootSurface) )
    {
        if ( SDL_LockSurface(rootSurface) < 0 )
        {
            return 0;
        }
    }
}

int dime::RectangleRenderer::sULock(SDL_Surface *rootSurface)
{
    if ( SDL_MUSTLOCK(rootSurface) )
    {
        SDL_UnlockSurface(rootSurface);
    }
}

dime::RectangleRenderer::~RectangleRenderer()
{

}
