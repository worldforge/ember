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
 *      Revision 1.2  2002-08-23 02:38:12  adamgreg
 *      Rewrite of RectangleRenderer finished, but probably broken :). Any brave souls who feel like testing it can just copy the contents of scratchpad/RRRewRite/ into src/services/platform/ . Mostly I've just split up RR, added a polymorphic factory function (ooh!) to create the type of renderer you want based on your arguments, and added a couple of timesaver functions (e.g. horizontal/vertical gradients and I think some other stuff I can't remember :-/).
 *
 *      Revision 1.1  2002/08/21 20:52:56  adamgreg
 *      First attempt to split up RectangleRenderer to make it leaner and easier to understand (hopefully). Just the headers worked on so far.
 *
 *      Revision 1.18  2002/08/17 19:44:14  xmp
 *      Remove a define that snuck in
 *
 *      Revision 1.16  2002/08/16 23:17:25  xmp
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

/**
 * Setter for the myRect member
 */
void dime::RectangleRenderer::setRect(const Rectangle &rect)
{
    myRect = rect;
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
