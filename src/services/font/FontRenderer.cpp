#include "FontRenderer.h"
#include "framework/Exception.h"

void dime::FontRenderer::updateTextBlended()
{
    
    int xstart;
    int width, height;
    SDL_Surface *textbuf;
    SDL_Color fg;
    Uint32 alpha;
    Uint32 pixel;
    Uint8 *src;
    Uint32 *dst;
    int row, col;
    Glyph *glyph;
    //FT_Error error;

    /* get color */
    fg.r = (Uint8)myColor.getR();
    fg.g = (Uint8)myColor.getG();
    fg.b = (Uint8)myColor.getB();

	assert(myFont);

    /* Get the dimensions of the text surface */
    if ((myFont->sizeText(myText, &width, NULL)) || !width ) {
        myTextSurface = NULL;
        THROW("Font width zero or sizeText error.")
    }
    height = myFont->getHeight();
    textbuf = SDL_AllocSurface(SDL_SWSURFACE, width, height, 32,
                               0x00FF0000, 0x0000FF00, 0x000000FF, 0xFF000000);
    if ( textbuf == NULL ) {
		THROW("Unable to allocate an SDL surface for textbuf drawing.")
    }

    /* Load and render each character */
    xstart = 0;
    pixel = (fg.r<<16)|(fg.g<<8)|fg.b;
    for ( unsigned int i = 0; i < myText.length(); ++i )
        {   
            glyph = myFont->findGlyph(myText[i]);
            assert(glyph);
            if( !glyph ) {
                SDL_FreeSurface( textbuf );
				THROW("Glyph undefined in font unable to render.")
            }
            width = glyph->getPixmap().width;
            src = (Uint8 *)glyph->getPixmap().buffer;
            for ( row = 0; row < glyph->getPixmap().rows; ++row ) {
                dst = (Uint32*) textbuf->pixels +
                    (row+glyph->getYOffset()) * textbuf->pitch/4 +
                    xstart + glyph->getMinX();
                for ( col=width; col>0; --col ) {
                    alpha = *src++;
                    *dst++ |= pixel | (alpha << 24);
                }
            }
            xstart += glyph->getYAdvance();
            if (myFont->getStyle() & dime::Font::STYLE_BOLD ) {
                xstart += myFont->getGlyphOverhang();
            }
        }
    /* Handle the underline style */
    if( myFont->getStyle() & dime::Font::STYLE_UNDERLINE ) {
        row = myFont->getAscent() - myFont->getUnderlineOffset() - 1;
        if ( row >= textbuf->h) {
            row = (textbuf->h-1) - myFont->getUnderlineHeight();
        }
        dst = (Uint32 *)textbuf->pixels + row * textbuf->pitch/4;
        pixel |= 0xFF000000; /* Amask */
        for ( row=myFont->getUnderlineHeight(); row>0; --row ) {
            for ( col=0; col < textbuf->w; ++col ) {
                dst[col] = pixel;
            }
            dst += textbuf->pitch/4;
        }
    }
    if(myTextSurface)
        {
            //Free our old surface
            SDL_FreeSurface(myTextSurface);
        }
    myTextSurface = textbuf;
}


void dime::FontRenderer::renderText(DrawDevice *device)
{
     SDL_Rect src, dest;
     src.x = 0;
     src.y = 0;
     src.w = myRectangle.getWidth();
     src.h = myRectangle.getHeight();
     dest = myRectangle.getSDL_Rect();
     if(myTextSurface)
         {
             device->blitSurface(&src, &dest, myTextSurface);
         }
    
}
