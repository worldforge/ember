/*
 Copyright (C) 2002  Martin Pollard (Xmp)
	
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

#ifndef ICONSTYLE_H
#define ICONSTYLE_H

// Included headers from the current project
#include "services/platform/Color.h"
#include "services/font/Font.h

// Included custom library headers

// Included system headers

namespace dime {

/**
 * Container class for Theme's IconStyle's.
 *
 * More detailed description of the class, it's purpose, what it does,
 * how to use it and so on.
 *
 * A short piece of example code demonstarting how this class it is used,
 * and in what context, is encouraged.
 *
 * @author Martin Pollard aka Xmp
 *
 * NOTE: You can also specify the author for individual methods
 * if different persons have created them.
 * It is also possible to have multiple @author tags for a method.
 * Only add yourself as an @author if you have done serious work
 * on a class/method, and can help fixing bugs in it, etc.
 * If you just fixed a bug or added a short code snipplet you
 * don't need to add yourself.
 *
 * @see dime::Theme
 *
 * NOTE: Add other related classes here, doxygen will create links to them.
 */

class IconStyle
{
    //======================================================================
    // Inner Classes, Typedefs, and Enums
    //======================================================================
    public:


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

        int myXOffset;
        int myYOffset;
        Color myTextColour;
        Font myFont;
        unsigned int myFontSize;
        bool myBold;
        bool myItalic;

    //======================================================================
    // Public Methods
    //======================================================================
    public:

    //----------------------------------------------------------------------
    // Constructors

    /**
     * Creates a new IconStyle using default values.
     */
     IconStyle()
     {
     }

    /**
     * Copy constructor.
     */
    IconStyle( const IconStyle &source )
    {
        // Use assignment operator to do the copy
        // NOTE: If you need to do custom initialization in the constructor this may not be enough.
        *this = source;
    }


    /**
     * Assignment operator.
     */
    IconStyle &operator= ( const IconStyle &source )
    {
        // Copy fields from source class to this class here.

        // Return this object with new value
        return *this;
    }


    //----------------------------------------------------------------------
    // Destructor

    /**
     * Deletes a IconStyle instance.
     */
    virtual ~IconStyle ()
    {
        // TODO: Free any allocated resources here.
    }


    //----------------------------------------------------------------------
    // Getters

    // Example of a getter method:

    /**
     * Gets the value of textColour of this IconStyle
     */
    Color getTextColor() const
    {
        return myTextColor;
    }

    /**
     * Gets the value of italic of this IconStyle
     */
    bool getItalic() const
    {
        return myItalic;
    }

    /**
     * Gets the value of font of this IconStyle
     */
    Font getFont() const
    {
        return myFont;
    }

    /**
     * Gets the value of fontSize of this IconStyle
     */
    unsigned int getFontSize() const
    {
        return myFontSize;
    }

    /**
     * Gets the value of yOffset of this IconStyle
     */
    int getYOffset() const
    {
        return myYOffset;
    }

    /**
     * Gets the value of xOffset of this IconStyle
     */
    int getXOffset() const
    {
        return myXOffset;
    }

    /**
     * Gets the value of bold of this IconStyle
     */
    bool getBold() const
    {
        return myBold;
    }


    //----------------------------------------------------------------------
    // Setters

    /**
     * Sets the value of textColour of this IconStyle
     */
    void setTextColour( Color textColour )
    {
        myTextColour = textColour;
    }

    /**
     * Sets the value of italic of this IconStyle
     */
    void setItalic( bool italic )
    {
        myItalic = italic;
    }

    /**
     * Sets the value of font of this IconStyle
     */
    void setfont( Font font )
    {
        myFont = font;
    }

    /**
     * Sets the value of fontSize of this IconStyle
     */
    void setfontSize( unsigned int fontSize )
    {
        myFontSize = fontSize;
    }

    /**
     * Sets the value of yOffset of this IconStyle
     */
    void setyOffset( int yOffset )
    {
        myYOffset = yOffset;
    }

    /**
     * Sets the value of xOffset of this IconStyle
     */
    void setxOffset( int xOffset )
    {
        myXOffset = xOffset;
    }

    /**
     * Sets the value of bold of this IconStyle
     */
    void setbold( bool bold )
    {
        myBold = bold;
    }


    //----------------------------------------------------------------------
    // Other public methods
    // NOTE: Group related public methods together and crate a separator comment like above for them.
    
    //======================================================================
    // Protected Methods
    //======================================================================
    protected:


    //======================================================================
    // Private Methods
    //======================================================================
    private:


    //======================================================================
    // Disabled constructors and operators
    //======================================================================
    private:


}; // End of IconStyle

} // End of dime namespace

#endif
