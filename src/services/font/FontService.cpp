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



/* Handy routines for converting from fixed point */
#define FT_FLOOR(X)	((X & -64) / 64)
#define FT_CEIL(X)	(((X + 63) & -64) / 64)

#include "FontService.h"

dime::Font *dime::FontService::loadFontIndex(std::string fontName, int pointSize, int index)
{
    dime::Font* font;
    FT_Error error;
    FT_Face face;
    FT_Fixed scale;

    font = new Font();
    
    /* Open the font and create ancillary data */
    error = FT_New_Face( library, fontName.c_str(), 0, &face );
    if( error ) {
        myLog->slog(__FILE__, __LINE__, LoggingService::WARNING) 
            << "Couldn't load font File: [" << fontName << "], PointSize: [" << pointSize << "], Index: [" << index << "]\n";
        delete font;
        return NULL;
    }
    if ( index != 0 ) {
        if ( face->num_faces > index ) {
            FT_Done_Face( face );
            error = FT_New_Face( library, fontName.c_str(), index, &face );
            if( error ) {
                myLog->slog(__FILE__, __LINE__, LoggingService::WARNING) 
                    << "Couldn't get font face.\n";
                delete font;
                return NULL;
            }
        } else {
            myLog->slog(__FILE__, __LINE__, LoggingService::WARNING) 
                << "No Such font face.\n";
            delete font;
            return NULL;
        }
    }
    
    /* Make sure that our font face is scalable (global metrics) */
    if ( ! FT_IS_SCALABLE(face) ) {
        myLog->slog(__FILE__, __LINE__, LoggingService::WARNING) 
            << "Font face is not scalable";
        closeFont( font );
        return NULL;
    }
    /* Set the character size and use default DPI (72) */
    error = FT_Set_Char_Size( face, 0, pointSize * 64, 0, 0 );
    if( error ) {
        myLog->slog(__FILE__, __LINE__, LoggingService::WARNING) 
            << "Couldn't set font size";
        closeFont( font );
        return NULL;
    }
    /* Get the scalable font metrics for this font */
    scale = face->size->metrics.y_scale;
    font->setAscent(FT_CEIL(FT_MulFix(face->bbox.yMax, scale)));
    font->setDescent(FT_CEIL(FT_MulFix(face->bbox.yMin, scale)));
    font->setHeight(font->getAscent() - font->getDescent() + /* baseline */ 1);
    font->setLineSkip(FT_CEIL(FT_MulFix(face->height, scale)));
    font->setUnderlineOffset(FT_FLOOR(FT_MulFix(face->underline_position, scale)));
    font->setUnderlineHeight(FT_FLOOR(FT_MulFix(face->underline_thickness, scale)));
    font->setFace(face);
    
    if ( font->getUnderlineHeight() < 1 ) {
        font->setUnderlineHeight(1);
    }
    /*
      ifdef DEBUG_FONTS
      printf("Font metrics:\n");
      printf("\tascent = %d, descent = %d\n",
      font->ascent, font->descent);
      printf("\theight = %d, lineskip = %d\n",
      font->height, font->lineskip);
      printf("\tunderline_offset = %d, underline_height = %d\n",
      font->underline_offset, font->underline_height);
      endif
    */
    /* Set the default font style */
    font->setStyle(dime::Font::STYLE_NORMAL);
    font->setGlyphOverhang(face->size->metrics.y_ppem / 10);
    /* x offset = cos(((90.0-12)/360)*2*M_PI), or 12 degree angle */
    font->setGlyphItalics(0.207f);
    font->setGlyphItalics(font->getGlyphItalics() * font->getHeight());
    return font;
}

dime::Font *dime::FontService::loadFont(std::string fontName, int pointSize)
{
    return loadFontIndex(fontName, pointSize, 0);
}
void dime::FontService::flushGlyph( dime::Glyph* glyph )
{
	glyph->setStored(false);
	glyph->setIndex(0);
	if( glyph->getBitmap().buffer ) {
		delete glyph->getBitmap().buffer;
		// Work around for MSVC 6 stupidity
		FT_Bitmap temp = glyph->getBitmap();
		temp.buffer = NULL;
	}
	if( glyph->getPixmap().buffer ) {
		delete glyph->getPixmap().buffer;
		// Work around for MSVC 6 stupidity
		FT_Bitmap temp = glyph->getPixmap();
		temp.buffer = NULL;
	}
	glyph->setCached(false);
}

void dime::FontService::flushCache( dime::Font* font )
{
    /*
      int i;
      int size = sizeof( font->getCache() ) / sizeof( font->getCache()[0] );
      
      for( i = 0; i < size; ++i ) {
      if( font->getCache()[i].cached ) {
      Flush_Glyph( &font->cache[i] );
      }
      }
      if( font->getScratch().cached ) {
      Flush_Glyph( &font->scratch );
      }
    */
}



void dime::FontService::closeFont( dime::Font* font )
{
    //	Flush_Cache( font );
	FT_Done_Face( font->getFace() );
	delete  font;
}

     
dime::FontService *dime::FontService::theInstance = NULL;
