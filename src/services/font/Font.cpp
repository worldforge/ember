/*
    Copyright (C) 2002  Lakin Wecker [nikal]

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

#include "Font.h"

/* Handy routines for converting from fixed point */
#define FT_FLOOR(X)	((X & -64) / 64)
#define FT_CEIL(X)	(((X + 63) & -64) / 64)
bool dime::Font::getGlyphMetrics(Uint16 ch, int* minx, int* maxx, int* miny, int* maxy, int* advance)
{
    Glyph *glyph = findGlyph(ch);
	if(glyph)
        {
            return false;
        }
    else
        {
            if ( minx ) {
                *minx = glyph->getMinX();
            }
            if ( maxx ) {
                *maxx =glyph->getMaxX();
            }
            if ( miny ) {
                *miny = glyph->getMinY();
            }
            if ( maxy ) {
                *maxy = glyph->getMaxY();
            }
            if ( advance ) {
                *advance = glyph->getYAdvance();
            }
        }
    return true;
}


dime::Glyph *dime::Font::findGlyph(Uint16 ch)
{
    std::map<Uint16, Glyph*>::iterator search;
    search = myGlyphCache.find(ch);
    if(search == myGlyphCache.end())
        {
            Glyph *newGlyph = new Glyph();
            bool retval = loadGlyph(ch, newGlyph);
            if(!retval)
                {
                    return NULL;
                }
            
            myGlyphCache[ch] = newGlyph;
            return newGlyph;
        }
    else 
        {
            return search->second;
        }
}

bool dime::Font::loadGlyph(Uint16 ch, dime::Glyph *cached)
{
    FT_Face face;
    FT_Error error;
    FT_GlyphSlot glyph;
    FT_Glyph_Metrics* metrics;
    FT_Outline* outline;
    assert( myFace );
    face = myFace;
    /* Load the glyph */
    if ( ! cached->getCached() ) {
        cached->setIndex( FT_Get_Char_Index( face, ch ) );
    }
    error = FT_Load_Glyph( face, cached->getIndex(), FT_LOAD_DEFAULT );
    if( error ) {
        return false;
    }
    /* Get our glyph shortcuts */
    glyph = face->glyph;
    metrics = &glyph->metrics;
    outline = &glyph->outline;
    /* Get the glyph metrics if desired */
    
    /* Get the bounding box */
    cached->setMinX( FT_FLOOR(metrics->horiBearingX));
    cached->setMaxX( cached->getMinX() + FT_CEIL(metrics->width));
    cached->setMaxY( FT_FLOOR(metrics->horiBearingY));
    cached->setMinY( cached->getMaxY() - FT_CEIL(metrics->height));
    cached->setYOffset( myAscent - cached->getMaxY());
    cached->setYAdvance( FT_CEIL(metrics->horiAdvance));
    /* Adjust for bold and italic text */
    if( myStyle & STYLE_BOLD ) {
        cached->setMaxX(cached->getMaxX() + myGlyphOverhang);
    }
    if( myStyle & STYLE_ITALIC ) {
        cached->setMaxX(cached->getMaxX() + (int)ceil(myGlyphItalics));
    }
  
    
   
    int i;
    FT_Bitmap* src;
    FT_Bitmap* dst;
    /* Handle the italic style */
    if( myStyle & STYLE_ITALIC ) {
        FT_Matrix shear;
        shear.xx = 1 << 16;
        shear.xy = (int) ( myGlyphItalics * ( 1 << 16 ) ) / myHeight;
        shear.yx = 0;
        shear.yy = 1 << 16;
        FT_Outline_Transform( outline, &shear );
    }
    /* Render the glyph */
    
    error = FT_Render_Glyph( glyph, ft_render_mode_normal );
    if( error ) {
        return false;
    }
    /* Copy over information to cache */
    src = &glyph->bitmap;
    dst = cached->getPixmapPointer(); 
    memcpy( dst, src, sizeof( *dst ) );
    
   
    /* Adjust for bold and italic text */
    if( myStyle & STYLE_BOLD ) {
        int bump = myGlyphOverhang;
        dst->pitch += bump;
        dst->width += bump;
    }
    if( myStyle & STYLE_ITALIC ) {
        int bump = (int)ceil(myGlyphItalics);
        dst->pitch += bump;
        dst->width += bump;
    }
    if (dst->rows != 0) {
        dst->buffer = (unsigned char*)malloc( dst->pitch * dst->rows );
        if( !dst->buffer ) {
            return false;
        }
        memset( dst->buffer, 0, dst->pitch * dst->rows );
        for( i = 0; i < src->rows; i++ ) {
            int soffset = i * src->pitch;
            int doffset = i * dst->pitch;
            
            memcpy(dst->buffer+doffset,
                   src->buffer+soffset, src->pitch);
        }
        /* Handle the bold style */
        if ( myStyle & STYLE_BOLD ) {
            int row;
            int col;
            int offset;
            int pixel;
            Uint8* pixmap;
            /* The pixmap is a little hard, we have to add and clamp */
            for( row = dst->rows - 1; row >= 0; --row ) {
                pixmap = (Uint8*) dst->buffer + row * dst->pitch;
                for( offset=1; offset <= myGlyphOverhang; ++offset ) {
                    for( col = dst->width - 1; col > 0; --col ) {
                        pixel = (pixmap[col] + pixmap[col-1]);
                        if( pixel > NUM_GRAYS - 1 ) {
                            pixel = NUM_GRAYS - 1;
                        }
                        pixmap[col] = (Uint8) pixel;
                    }
                }
            }
        }
        /* Mark that we rendered this format */
    }
    /* We're done, mark this glyph cached */
    cached->setCached( true );
    cached->setCharacter(ch);
    
    return true;
}

bool dime::Font::sizeText(FontString text, int *w, int *h)
{
    int status;
    int x, z;
    unsigned int index;
    int minx, maxx;
    int miny, maxy;
    Glyph *glyph;
//    FT_Error error; unused
    
    status = 0;
    minx = maxx = 0;
    miny = maxy = 0;
    /* Load each character and sum it's bounding box */
    x= 0;
    for ( index = 0; index < text.length(); ++index ) {
        glyph = findGlyph(text[index]);
        if(glyph == NULL)
            {
                return false;
            }
        
        z = x + glyph->getMinX();
        if ( minx > z ) {
            minx = z;
        }
        if ( myStyle & STYLE_BOLD ) {
            x += myGlyphOverhang;
        }
        if ( glyph->getYAdvance() > glyph->getMaxX() ) {
            z = x + glyph->getYAdvance();
        } else {
            z = x + glyph->getMaxX();
        }
        if ( maxx < z ) {
            maxx = z;
        }
        x += glyph->getYAdvance();
        if ( glyph->getMinY() < miny ) {
            miny = glyph->getMinY();
        }
        if ( glyph->getMaxY() > maxy ) {
            maxy = glyph->getMaxY();
        }
    }
    /* Fill the bounds rectangle */
    if ( w ) {
        *w = (maxx - minx);
    }
    if ( h ) {
        *h = myHeight;
    }
    return status;
    
}



const int dime::Font::STYLE_NORMAL= 0x00;
const int dime::Font::STYLE_BOLD = 0x01;
const int dime::Font::STYLE_ITALIC = 0x02;
const int dime::Font::STYLE_UNDERLINE = 0x04;
const int dime::Font::NUM_GRAYS = 256;


