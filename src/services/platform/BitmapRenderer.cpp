#include "BitmapRenderer.h"

inline int dimeroundf(float x) {return x + (x > 0) ? 0.5 : -0.5;}

/**
 * Constructor for a bitmap filled RectangleRenderer
 */
dime::BitmapRenderer::BitmapRenderer(const dime::Rectangle &rect, 
	const std::string filename, dime::BitmapRenderer::BitmapStyle style) : 
	RectangleRenderer(rect), myImageName(filename), myStyle(style)
{
    SDL_Rect src, dest;
    src.x = 0;
    src.y = 0;
    dest = myRect.getSDL_Rect();

    mySourceSurface = ImageService::getInstance()->loadImage(filename);
    src.w = mySourceSurface->w;
    src.h = mySourceSurface->h;
    mySurface = SDL_CreateRGBSurface(SDL_SWSURFACE, dest.w,
	    dest.h, mySourceSurface->format->BitsPerPixel, 0, 0, 0, 0);

    switch (myStyle)
    {
	case BitmapRenderer::STRETCH:
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
			color = getpixel(mySourceSurface, x, y);
			if ((yCheck - lastYCheck == 1) && (xCheck 
				    - lastXCheck == 1))
			{
			    putpixel(mySurface, lastXCheck, lastYCheck, color);
			}
			else if ((yCheck - lastYCheck > 0) || (xCheck
				    - lastXCheck > 0))
			{
			    smallRect.x = lastXCheck;
			    smallRect.y = lastYCheck;
			    smallRect.w = xCheck - lastXCheck;
			    smallRect.h = yCheck - lastYCheck;
			    SDL_FillRect(mySurface, &smallRect, color);
			}
			lastXCheck = xCheck;
			
		    }
		    lastYCheck = yCheck;
		}
		
		src.w = dest.w;
		src.h = dest.h;
		break;
	    }
    case BitmapRenderer::TILE:
    {
	SDL_Rect curDest;
	curDest.w = src.w;
	curDest.h = src.h;
	for(curDest.x = 0; curDest.x < dest.w; curDest.x += src.w)
	{
	    for (curDest.y = 0; curDest.y < dest.h;curDest.y += src.h)
	    {
		SDL_BlitSurface(mySourceSurface, /*&src*/NULL, mySurface, &curDest);
		//device->blitSurface(&src, &curDest, mySurface);
	    }
	}
	break;
	}
	case BitmapRenderer::CENTER:
	{
		SDL_Rect src, dest;
		src.w = mySourceSurface->w;
		src.h = mySourceSurface->h;
		dest.w = myRect.getWidth();
		dest.h = myRect.getHeight();
		dest.y = (dest.h - src.h) / 2;
		dest.x = (dest.w - src.w) / 2;
		
		SDL_BlitSurface(mySourceSurface,NULL,mySurface,&dest);
		break;
	}
    }
}

/**
 * Renders a bitmap
 */
void dime::BitmapRenderer::render(dime::DrawDevice *device)
{
    SDL_Rect src, dest;
    src.x = 0;
    src.y = 0;
    src.w = mySurface->w;
    src.h = mySurface->h;
    dest = myRect.getSDL_Rect();

    switch(myStyle)
        {
        case TILE:
	    {
		device->blitSurface(&src, &dest, mySurface);
		break;
	    }
        case STRETCH:
		{
		src.w = dest.w;
		src.h = dest.h;
		device->blitSurface(&src, &dest, mySurface);
		break;
		}
		case CENTER:
		{
		device->blitSurface(&src, &dest, mySurface);
		}
		default:
		{
		break;
		}
	}
}

dime::BitmapRenderer::~BitmapRenderer()
{
};