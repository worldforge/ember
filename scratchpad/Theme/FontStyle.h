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

#ifndef FONTSTYLE_H
#define FONTSTYLE_H

// Included headers from the current project

// Included custom library headers

// Included system headers

namespace dime {

/**
 * Defines a Fontstyle, apply this fontstyle to a font to get the desired effects.
 *
 * Defines a Fontstyle which holds information about bold, italic, font size,
 * xoffsets, yoffsets, font path, etc.  Mainly for use with a theme, but can be
 * used to acheive different font effects by applying this to a font.
 * 
 *
 * A short piece of example code demonstarting how this class it is used,
 * and in what context, is encouraged.
 *
 * @author Lakin Wecker aka nikal
 *
 * 
 * @see dime::Font
 * @see dime::FontRenderer
 * @see 
 *
 * NOTE: Add other related classes here, doxygen will create links to them.
 */

class FontStyle
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

        int myYOffset;
        int myXOffset;
        int myPointSize;
        bool myIsItalic;
        std::string myFontPath;
        dime::Color myColor;
        bool myIsBold;

    //======================================================================
    // Public Methods
    //======================================================================
    public:

    //----------------------------------------------------------------------
    // Constructors

    /**
     * Creates a new FontStyle using default values.
     */
     FontStyle()
     {
     }

    /**
     * Copy constructor.
     */
    FontStyle( const FontStyle &source )
    {
        // Use assignment operator to do the copy
        // NOTE: If you need to do custom initialization in the constructor this may not be enough.
        *this = source;
    }


    /**
     * Assignment operator.
     */
    FontStyle &operator= ( const FontStyle &source )
    {
        // Copy fields from source class to this class here.

        // Return this object with new value
        return *this;
    }


    //----------------------------------------------------------------------
    // Destructor

    /**
     * Deletes a FontStyle instance.
     */
    virtual ~FontStyle ()
    {
        // TODO: Free any allocated resources here.
    }


    //----------------------------------------------------------------------
    // Getters

    // Example of a getter method:

    /**
     * Gets the value of YOffset of this FontStyle
     */
    int getYOffset() const
    {
        return myYOffset;
    }

    /**
     * Gets the value of XOffset of this FontStyle
     */
    int getXOffset() const
    {
        return myXOffset;
    }

    /**
     * Gets the value of PointSize of this FontStyle
     */
    int getPointSize() const
    {
        return myPointSize;
    }

    /**
     * Gets the value of IsItalic of this FontStyle
     */
    bool getIsItalic() const
    {
        return myIsItalic;
    }

    /**
     * Gets the value of FontPath of this FontStyle
     */
    std::string getFontPath() const
    {
        return myFontPath;
    }

    /**
     * Gets the value of Color of this FontStyle
     */
    dime::Color getColor() const
    {
        return myColor;
    }

    /**
     * Gets the value of IsBold of this FontStyle
     */
    bool getIsBold() const
    {
        return myIsBold;
    }


    //----------------------------------------------------------------------
    // Setters

    /**
     * Sets the value of YOffset of this FontStyle
     */
    void setYOffset( int yOffset )
    {
        myYOffset = yOffset ;
    }

    /**
     * Sets the value of XOffset of this FontStyle
     */
    void setXOffset( int xOffset )
    {
        myXOffset = xOffset ;
    }

    /**
     * Sets the value of PointSize of this FontStyle
     */
    void setPointSize( int pointSize )
    {
        myPointSize = pointSize ;
    }

    /**
     * Sets the value of IsItalic of this FontStyle
     */
    void setIsItalic( bool isItalic )
    {
        myIsItalic = isItalic ;
    }

    /**
     * Sets the value of FontPath of this FontStyle
     */
    void setFontPath( std::string fontPath )
    {
        myFontPath = fontPath ;
    }

    /**
     * Sets the value of Color of this FontStyle
     */
    void setColor( dime::Color color )
    {
        myColor = color ;
    }

    /**
     * Sets the value of IsBold of this FontStyle
     */
    void setIsBold( bool isBold )
    {
        myIsBold = isBold ;
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


}; // End of FontStyle

} // End of dime namespace

#endif
