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

#ifndef STYLE_H
#define STYLE_H

// Included headers from the current project
#include "services/platform/Color.h"
#include "TextStyle.h"
#include "IconStyle.h"
#include "BackgroundStyle.h"

// Included custom library headers

// Included system headers
#include <string>

namespace dime {

/**
 * Container Class for Theme's generic Styles.
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
 * @see dime::TextStyle
 * @see dime::IconStyle
 * @see dime::BackgroundStyle
 *
 * NOTE: Add other related classes here, doxygen will create links to them.
 */

class Style
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

        std::string myParentStyle;
	Color myForegroundColor;
	TextStyle myTextStyle;
        IconStyle myIconStyle;
        BackgroundStyle myBackgroundStyle;

    //======================================================================
    // Public Methods
    //======================================================================
    public:

    //----------------------------------------------------------------------
    // Constructors

    /**
     * Creates a new Style using default values.
     */
     Style()
     {
     }

    /**
     * Copy constructor.
     */
    Style( const Style &source )
    {
        // Use assignment operator to do the copy
        // NOTE: If you need to do custom initialization in the constructor this may not be enough.
        *this = source;
    }


    /**
     * Assignment operator.
     */
    Style &operator= ( const Style &source )
    {
        // Copy fields from source class to this class here.

        // Return this object with new value
        return *this;
    }


    //----------------------------------------------------------------------
    // Destructor

    /**
     * Deletes a Style instance.
     */
    virtual ~Style ()
    {
        // TODO: Free any allocated resources here.
    }


    //----------------------------------------------------------------------
    // Getters

    // Example of a getter method:

    /**
     * Gets the value of backgroundStyle of this Style
     */
    BackgroundStyle getBackgroundStyle() const
    {
        return myBackgroundStyle;
    }

    /**
     * Gets the value of textStyle of this Style
     */
    TextStyle getTextStyle() const
    {
        return myTextStyle;
    }

    /**
     * Gets the value of parentStyle of this Style
     */
    std::string getParentStyle() const
    {
        return myParentStyle;
    }

    /**
     * Gets the value of iconStyle of this Style
     */
    IconStyle getIconStyle() const
    {
        return myIconStyle;
    }

    /**
     * Gets the value of foregroundColor of this Style
     */
    Color getForegroundColor() const
    {
        return myForegroundColor;
    }


    //----------------------------------------------------------------------
    // Setters

    /**
     * Sets the value of backgroundStyle of this Style
     */
    void setBackgroundStyle( BackgroundStyle backgroundStyle )
    {
        myBackgroundStyle = backgroundStyle;
    }

    /**
     * Sets the value of textStyle of this Style
     */
    void setTextStyle( TextStyle textStyle )
    {
        myTextStyle = textStyle;
    }

    /**
     * Sets the value of parentStyle of this Style
     */
    void setParentStyle( std::string parentStyle )
    {
        myParentStyle = parentStyle;
    }

    /**
     * Sets the value of iconStyle of this Style
     */
    void setIconStyle( IconStyle iconStyle )
    {
        myIconStyle = iconStyle;
    }

    /**
     * Sets the value of foregroundColor of this Style
     */
    void setForegroundColor( Color foregroundColor )
    {
        myForegroundColor = foregroundColor;
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


}; // End of Style

} // End of dime namespace

#endif
