/*
 Copyright (C) 2002  Lakin Wecker
	
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

#ifndef TEXTSTYLE_H
#define TEXTSTYLE_H

// Included headers from the current project
#include "services/platform/Color.h"

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
 * @author Lakin Wecker aka nikal
 *
 * NOTE: You can also specify the author for individual methods
 * if different persons have created them.
 * It is also possible to have multiple @author tags for a method.
 * Only add yourself as an @author if you have done serious work
 * on a class/method, and can help fixing bugs in it, etc.
 * If you just fixed a bug or added a short code snipplet you
 * don't need to add yourself.
 *
 * @see dime::Style
 *
 * NOTE: Add other related classes here, doxygen will create links to them.
 */

class TextStyle
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
	Color myTextColor;
        std::string myFontPath;
        bool myIsBold;
        bool myIsItalic;

    //======================================================================
    // Public Methods
    //======================================================================
    public:

    //----------------------------------------------------------------------
    // Constructors

    /**
     * Creates a new TextStyle using default values.
     */
     TextStyle()
     {
     }

    /**
     * Copy constructor.
     */
    TextStyle( const TextStyle &source )
    {
        // Use assignment operator to do the copy
        // NOTE: If you need to do custom initialization in the constructor this may not be enough.
        *this = source;
    }


    /**
     * Assignment operator.
     */
    TextStyle &operator= ( const TextStyle &source )
    {
        // Copy fields from source class to this class here.

        // Return this object with new value
        return *this;
    }


    //----------------------------------------------------------------------
    // Destructor

    /**
     * Deletes a TextStyle instance.
     */
    virtual ~TextStyle ()
    {
        // TODO: Free any allocated resources here.
    }


    //----------------------------------------------------------------------
    // Getters

    /**
     * Gets the value of textColor of this TextStyle
     */
    Color getTextColor() const
    {
        return myTextColor;
    }

    /**
     * Gets the value of isItalic of this TextStyle
     */
    bool getIsItalic() const
    {
        return myIsItalic;
    }

    /**
     * Gets the value of yOffset of this TextStyle
     */
    int getYOffset() const
    {
        return myYOffset;
    }

    /**
     * Gets the value of xOffset of this TextStyle
     */
    int getXOffset() const
    {
        return myXOffset;
    }

    /**
     * Gets the value of fontPath of this TextStyle
     */
    std::string getFontPath() const
    {
        return myFontPath;
    }

    /**
     * Gets the value of isBold of this TextStyle
     */
    bool getIsBold()
    {
        return myIsBold;
    }


    //----------------------------------------------------------------------
    // Setters

    /**
     * Sets the value of textColor of this TextStyle
     */
    void setTextColor( Color textColor )
    {
        myTextColor = textColor;
    }

    /**
     * Sets the value of isItalic of this TextStyle
     */
    void setIsItalic( bool isItalic )
    {
        myIsItalic = isItalic;
    }

    /**
     * Sets the value of yOffset of this TextStyle
     */
    void setYOffset( int yOffset )
    {
        myYOffset = yOffset;
    }

    /**
     * Sets the value of xOffset of this TextStyle
     */
    void setXOffset( int xOffset )
    {
        myXOffset = xOffset;
    }

    /**
     * Sets the value of fontPath of this TextStyle
     */
    void setFontPath( const std::string& fontPath )
    {
        myFontPath = fontPath;
    }

    /**
     * Sets the value of isBold of this TextStyle
     */
    void setIsBold( bool isBold )
    {
        myIsBold = isBold;
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


}; // End of TextStyle

} // End of dime namespace

#endif
