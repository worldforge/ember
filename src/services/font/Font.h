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

#ifndef FONT_H
#define FONT_H

#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <freetype/ftoutln.h>
#include <map>
#include <math.h>
#include "Glyph.h"

namespace dime {

/**
 * Font class
 *
 * @author Lakin Wecker [nikal]
 */
class Font
{
public:
    //======================================================================
    //  Enums
    //======================================================================
    
    static const int STYLE_NORMAL;
    static const int STYLE_BOLD;
    static const int STYLE_ITALIC;
    static const int STYLE_UNDERLINE;
     
    static const int NUM_GRAYS;
    //======================================================================
    // Private Variables
    //======================================================================
private:
    
    /* Freetype2 maintains all sorts of useful info itself */
    FT_Face myFace;
    
    /* We'll cache these ourselves */
    int myHeight;
    int myAscent;
    int myDescent;
    int myLineSkip;
    
    /* The font style */
    int myStyle;
    
    /* Extra width in glyph bounds for text styles */
    int myGlyphOverhang;
    float myGlyphItalics;
    
    /* Information in the font for underlining */
    int myUnderlineOffset;
    int myUnderlineHeight;

    /* Cache for style-transformed glyphs */
    //std::map<char, Glyph *> myGlyphs;
    //std::map<char, Glyph *>::iterator myCurrentGlyph;
    Glyph *myCurrentGlyph;
    Glyph myGlyphCache[256];
    Glyph myGlyphScratch;
    //----------------------------------------------------------------------
    // private member functions.
       


    //----------------------------------------------------------------------
    // Constructors & Destructor
    
public:    
    Font()
    {
    }
    
    /** Deletes a ConfigService instance. */
    ~Font()
    {
    }
    
    //----------------------------------------------------------------------
    // Public Mehods.
    
    /* Get the number of faces of the font */
    //   extern DECLSPEC long SDLCALL TTF_FontFaces(TTF_Font *font);
    
    /* Get the font face attributes, if any */
    //extern DECLSPEC int SDLCALL TTF_FontFaceIsFixedWidth(TTF_Font *font);
    //extern DECLSPEC char * SDLCALL TTF_FontFaceFamilyName(TTF_Font *font);
    //extern DECLSPEC char * SDLCALL TTF_FontFaceStyleName(TTF_Font *font);
    
    /* Get the metrics (dimensions) of a glyph */
    bool getGlyphMetrics(Uint16 ch,
                        int *minx, int *maxx,
                        int *miny, int *maxy, int *advance);
    
    /* Get the dimensions of a rendered string of text */
    bool sizeText(const char *text, int *w, int *h);
    //int sizeUTF8(const char *text, int *w, int *h);
    //int sizeUNICODE(const Uint16 *text, int *w, int *h);
    
    Glyph *findGlyph(Uint16 ch);
    bool loadGlyph(Uint16 ch, Glyph *cached);
  
    
    
    
    
    //----------------------------------------------------------------------
    // Getters & Setters
    /* Freetype2 maintains all sorts of useful info itself */
    FT_Face getFace()
    {
        return myFace;
    }

    /* We'll cache these ourselves */
    int getHeight()
    {
        return myHeight;
    }
    
    int getAscent()
    {
        return myAscent;
    }
    
    int getDescent()
    {
        return myDescent;
    }
    
    int getLineSkip()
    {
        return myLineSkip;   
    }
    
    /* The font style */
    int getStyle()
    {
        return myStyle;
    }
    
    /* Extra width in glyph bounds for text styles */
    int getGlyphOverhang()
    {
        return myGlyphOverhang;
    }
    
    float getGlyphItalics()
    {
        return myGlyphItalics;
    }
    
    /* Information in the font for underlining */
    int getUnderlineOffset()
    {
        return myUnderlineOffset;
    }
    
    int getUnderlineHeight()
    {
        return myUnderlineHeight;
    }
    
    Glyph *getCurrentGlyph()
    {
        return myCurrentGlyph;
    }
    
    FT_Face setFace(FT_Face &face)
    {
        myFace = face;
    }
    /* We'll cache these ourselves */
    void setHeight(int height)
    {
       myHeight = height;
    }
    
    void setAscent(int ascent)
    {
         myAscent = ascent;
    }
    
    void setDescent(int descent)
    {
         myDescent = descent;
    }
    
    void setLineSkip(int lineskip)
    {
         myLineSkip = lineskip;   
    }
    
    /* The font style */
    void setStyle(int style)
    {
         myStyle = style;
    }
    
    /* Extra width in glyph bounds for text styles */
    void setGlyphOverhang(int glyphOverhang)
    {
         myGlyphOverhang = glyphOverhang;
    }
    
    void setGlyphItalics(float glyphItalics)
    {
         myGlyphItalics = glyphItalics;
    }
    
    /* Information in the font for underlining */
    void setUnderlineOffset(int underlineOffset)
    {
         myUnderlineOffset  = underlineOffset;
    }
    
    void setUnderlineHeight(int underlineHeight)
    {
         myUnderlineHeight= underlineHeight ;
    }

    
}; //Font
} // namespace dime

#endif
