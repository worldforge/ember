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

// Included custom library headers

// Included system headers

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
 * @see OtherSubsystem::AnOtherRelatedClass
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

        Color myTextColor;
        bool myIsItalic;
        int myYOffset;
        int myXOffset;
        std::string myFontPath;
        bool myIsBold;

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

    // Example of a getter method:

    /**
     * Gets the value of TextColor of this TextStyle
     */
    Color getTextColor()
    {
        return myTextColor;
    }

    /**
     * Gets the value of IsItalic of this TextStyle
     */
    bool getIsItalic()
    {
        return myIsItalic;
    }

    /**
     * Gets the value of YOffset of this TextStyle
     */
    int getYOffset()
    {
        return myYOffset;
    }

    /**
     * Gets the value of XOffset of this TextStyle
     */
    int getXOffset()
    {
        return myXOffset;
    }

    /**
     * Gets the value of FontPath of this TextStyle
     */
    std::string getFontPath()
    {
        return myFontPath;
    }

    /**
     * Gets the value of IsBold of this TextStyle
     */
    bool getIsBold()
    {
        return myIsBold;
    }


    //----------------------------------------------------------------------
    // Setters

    /**
     * Sets the value of TextColor of this TextStyle
     */
    void setTextColor( Color )
    {
        myTextColor = TextColor;
    }

    /**
     * Sets the value of IsItalic of this TextStyle
     */
    void setIsItalic( bool )
    {
        myIsItalic = IsItalic;
    }

    /**
     * Sets the value of YOffset of this TextStyle
     */
    void setYOffset( int )
    {
        myYOffset = YOffset;
    }

    /**
     * Sets the value of XOffset of this TextStyle
     */
    void setXOffset( int )
    {
        myXOffset = XOffset;
    }

    /**
     * Sets the value of FontPath of this TextStyle
     */
    void setFontPath( std::string )
    {
        myFontPath = FontPath;
    }

    /**
     * Sets the value of IsBold of this TextStyle
     */
    void setIsBold( bool )
    {
        myIsBold = IsBold;
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
