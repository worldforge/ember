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

#ifndef GLYPH_H
#define GLYPH_H

#include <ft2build.h>
#include FT_FREETYPE_H
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>

namespace dime {

/**
 * CachedGlyph class
 *
 * @author Lakin Wecker [nikal]
 */
class Glyph
{
    //======================================================================
    // Private Variables
    //======================================================================
private:
    int myStored;
    FT_UInt myIndex;
    FT_Bitmap myBitmap;
    FT_Bitmap myPixmap;
    int myMinX;
    int myMaxX;
    int myMinY;
    int myMaxY;
    int myYOffset;
    int myYAdvance;
    Uint16 myCharacter;
    bool myCached;
    
    
    //----------------------------------------------------------------------
    // Constructors & Destructor
    
public:    
    Glyph()
        : myCached(false)
    {
    }
    
    /** Deletes a ConfigService instance. */
    ~Glyph()
    {
    }
    
    //----------------------------------------------------------------------
    // Getters & Setters
    int getStored()
    {
        return myStored;
    }
    
    FT_UInt  getIndex()
    {
        return myIndex;
    }
    
    FT_Bitmap  getBitmap()
    {
        return myBitmap;
    }
    FT_Bitmap *getBitmapPointer()
    {
        return &myBitmap;
    }
    
    FT_Bitmap  getPixmap()
    {
        return myPixmap;
    }
    
    FT_Bitmap *getPixmapPointer()
    {
        return &myPixmap;
    }
    
    
    int  getMinX()
    {
        return myMinX;
    }
    
    int  getMaxX()
    {
        return myMaxX;
    }
    
    int  getMinY()
    {
        return myMinY;
    }
    
    int  getMaxY()
    {
        return myMaxY;
    }
    
    int getYOffset()
    {
        return myYOffset;
    }
    
    int  getYAdvance()
    {
        return myYAdvance;
    }
    
    Uint16 getCharacter()
    {
        return myCharacter;
    }
    bool getCached()
    {
        return myCached;
    }
    
     //----------------------------------------------------------------------
    // Setters & Setters
    void  setStored(int stored)
    {
        myStored = stored;
    }
    
    void setIndex(FT_UInt index)
    {
        myIndex = index;
    }
    
    void setBitmap(FT_Bitmap bitmap)
    {
        myBitmap = bitmap;
    }
    
    void  setPixmap(FT_Bitmap pixmap)
    {
        myPixmap = pixmap;
    }
    
    void  setMinX(int minX)
    {
        myMinX = minX;
    }
    
    void  setMaxX(int maxX)
    {
        myMaxX = maxX ;
    }
    
    void setMinY(int minY)
    {
        myMinY = minY;
    }
    
    void  setMaxY(int maxY)
    {
        myMaxY = maxY;
    }
    
    void  setYOffset(int yOffset)
    {
        myYOffset = yOffset;
    }
    
    void  setYAdvance(int yAdvance)
    {
        myYAdvance = yAdvance;
    }
    
    void setCharacter(Uint16 character)
    {
        myCharacter = character;
    }
    
    void setCached(bool cached)
    {
        myCached = cached;
    }
    
}; //Font
} // namespace dime

#endif
