/*
    Copyright (C) 2002  Nikal

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

#ifndef FONTRENDERER_H
#define FONTRENDERER_H

// Included headers from the current project
#include "Font.h"
#include <services/logging/LoggingService.h>
#include <services/platform/Color.h>
#include <services/platform/Rectangle.h>
#include <services/platform/DrawDevice.h>
#include <framework/Exception.h>
// Included custom library headers

// Included system headers
#include <string>

namespace dime {

/**
 * Short sentence with a general description of the class, ending in period.
 *
 * More detailed description of the class, it's purpose, what it does,
 * how to use it and so on.
 *
 * A short piece of example code demonstarting how this class it is used,
 * and in what context, is encouraged.
 *
 * @author Nikal
 * @author Xmp (Martin Pollard)
 *
 * @see Font
 *
 */
class FontRenderer

{
    //======================================================================
    // Inner Classes, Typedefs, and Enums
    //======================================================================
    public:
    enum Type 
        {
            MONO,
            BLENDED
        };
    

    //======================================================================
    // Public Constants
    //======================================================================
    public:


    //======================================================================
    // Private Constants
    //======================================================================
    private:


    //======================================================================
    // Private Variables
    //======================================================================/
    private:

    // NOTE: Class variables are prefixed with "my", static variables are
    //       prefixed with "the".
    Type myType;
    Font::FontString myText;
    SDL_Surface *myTextSurface;
    Font *myFont;
    Color myColor;
    Rectangle myRectangle;
    bool myUpdate;
    bool myIsReady;



    //======================================================================
    // Public Methods
    //======================================================================
    public:

    //----------------------------------------------------------------------
    // Constructors

    /**
     * Creates a new FontRenderer using default values.
     *
     *@param type MONO = no anti-aliasing.  BLENDED = antialiasing which fades to transparent
     *@param text the text for this FontRenderer
     *@param color the color for the text.
     */

    FontRenderer(const Type type, const Font::FontString text, Font *font, const Color &color, const Rectangle &rectangle)
        : myType(type), myText(text), myTextSurface(NULL), myFont(font), myColor(color),  myUpdate(false), myRectangle(rectangle)
    {
        updateTextBlended();
    }


    /**
     * Copy constructor.
     */
    FontRenderer( const FontRenderer &source )
    {
        // Use assignment operator to do the copy
        // NOTE: If you need to do custom initialization in the constructor this may not be enough.
        *this = source;
    }


    /**
     * Assignment operator.
     */
    FontRenderer &operator= ( const FontRenderer &source )
    {
        // Copy fields from source class to this class here.

        // Return this object with new value
        return *this;
    }


    //----------------------------------------------------------------------
    // Destructor

    /**
     * Deletes a FontRenderer instance.
     */
    virtual ~FontRenderer()
    {
        // TODO: Free any allocated resources here.
    }


    //----------------------------------------------------------------------
    // Getters
    Type getType()
    {
        return myType;
    }
    
    Font::FontString getText() const
    {
        return myText;
    }
    
    SDL_Surface *getTextSurface()
    {
        return myTextSurface;
    }
    
    Color getColor()
    {
        return myColor;
    }
    
    Rectangle getRectangle()
    {
        return myRectangle;
    }
    
    Font *getFont()
    {
        return myFont;
    }

    //----------------------------------------------------------------------
    // Setters
    void setType(const Type &type)
    {
        myType;
        myUpdate = true;
    }
    
    void setText(const Font::FontString &text)
    {
        myText = text;
        myUpdate = true;
    }
    
    void setColor(const Color &color)
    {
        myColor = color;
        myUpdate = true;
    }
    
    void setRectangle(const Rectangle &rectangle)
    {
        myRectangle = rectangle;
        myUpdate = true;
    }
    
    void getFont(Font *font)
    {
        myFont = font;
        myUpdate = true;
    }
    //----------------------------------------------------------------------
    // Other public methods

    int render(DrawDevice *device)
    {
        switch(myType)
            {
            case MONO:
                
                break;
            case BLENDED:
                if(myUpdate)
                    {
                        try
                            {
                                updateTextBlended();
                            }
                        
                        catch (Exception e)
                            {
                                LoggingService::getInstance()->slog(__FILE__, __LINE__, LoggingService::WARNING) << e.getError() << ENDM;
                            }
                        
                    }
                renderText(device);
            }
        return 0;
    }
    
    
    
    
    //======================================================================
    // Protected Methods
    //======================================================================
    protected:


    //======================================================================
    // Private Methods
    //======================================================================
    private:
    void updateTextBlended();
    void renderText(DrawDevice *device);
    /* Create a 32-bit ARGB surface and render the given text at high quality,
       using alpha blending to dither the font with the given color.
       This function returns the new surface, or NULL if there was an error.
    */
    //SDL_Surface *renderTextBlended(const char *text, SDL_Color fg);
    //SDL_Surface *renderUTF8_Blended(const char *text, SDL_Color fg);
    //SDL_Surface *renderUNICODE_Blended(const Uint16 *text, SDL_Color fg);
    
    /* Create a 32-bit ARGB surface and render the given glyph at high quality,
       using alpha blending to dither the font with the given color.
       The glyph is rendered without any padding or centering in the X
       direction, and aligned normally in the Y direction.
       This function returns the new surface, or NULL if there was an error.
    */
    //    SDL_Surface *renderGlyphBlended(Uint16 ch, SDL_Color fg);

    //======================================================================
    // Disabled constructors and operators
    //======================================================================
    private:

}; // End of class

} // End of application namespace

#endif


