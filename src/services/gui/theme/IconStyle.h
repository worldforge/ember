/*
 Copyright (C) 2002  Martin Pollard
	
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

// Included custom library headers

// Included system headers

namespace dime {

/**
 * Container Class for Theme's Styles.
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
 * @see OtherSubsystem::AnOtherRelatedClass
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
        float myColorStrength;
        Color myColor;
        float myHeightFactor;
        float myWidthFactor;

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
     * Gets the value of colorStrength of this IconStyle
     */
    float getColorStrength() const
    {
        return myColorStrength;
    }

    /**
     * Gets the value of heightFactor of this IconStyle
     */
    float getHeightFactor() const
    {
        return myHeightFactor;
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
     * Gets the value of widthFactor of this IconStyle
     */
    float getWidthFactor() const
    {
        return myWidthFactor;
    }

    /**
     * Gets the value of color of this IconStyle
     */
    Color getColor() const
    {
        return myColor;
    }


    //----------------------------------------------------------------------
    // Setters

    /**
     * Sets the value of colorStrength of this IconStyle
     */
    void setColorStrength( float colorStrength )
    {
        myColorStrength = colorStrength;
    }

    /**
     * Sets the value of heightFactor of this IconStyle
     */
    void setHeightFactor( float heightFactor )
    {
        myHeightFactor = heightFactor;
    }

    /**
     * Sets the value of yOffset of this IconStyle
     */
    void setYOffset( int yOffset )
    {
        myYOffset = yOffset;
    }

    /**
     * Sets the value of xOffset of this IconStyle
     */
    void setXOffset( int xOffset )
    {
        myXOffset = xOffset;
    }

    /**
     * Sets the value of widthFactor of this IconStyle
     */
    void setWidthFactor( float widthFactor )
    {
        myWidthFactor = widthFactor;
    }

    /**
     * Sets the value of color of this IconStyle
     */
    void setColor( Color color )
    {
        myColor = color;
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
