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

#ifndef ICONSTYLE_H
#define ICONSTYLE_H

// Included headers from the current project

// Included custom library headers

// Included system headers

namespace dime {

/**
 * Hold's Icon Style information.
 *
 * More detailed description of the class, it's purpose, what it does,
 * how to use it and so on.
 *
 * A short piece of example code demonstarting how this class it is used,
 * and in what context, is encouraged.
 *
 * @author Lakin Wecker aka nikal
 *
 *
 * @see dime::ThemeService
 *
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

        double myHeightFactor;
        int myYOffset;
        int myXOffset;
        double myWidthFactor;
        dime::Color myColor;
        double myColorStrength;

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
		myHeightFactor = source.getHeightFactor();
		myYOffset = source.getXOffset();
		myXOffset = source.getYOffset();
		myWidthFactor = source.getWidthFactor();
		myColor = source.getColor();
		myColorStrength = source.getColorStrength();
		
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
     * Gets the value of HeightFactor of this IconStyle
     */
    double getHeightFactor() const
    {
        return myHeightFactor;
    }

    /**
     * Gets the value of YOffset of this IconStyle
     */
    int getYOffset() const
    {
        return myYOffset;
    }

    /**
     * Gets the value of XOffset of this IconStyle
     */
    int getXOffset() const
    {
        return myXOffset;
    }

    /**
     * Gets the value of WidthFactor of this IconStyle
     */
    double getWidthFactor() const
    {
        return myWidthFactor;
    }

    /**
     * Gets the value of Color of this IconStyle
     */
    dime::Color getColor() const
    {
        return myColor;
    }

    /**
     * Gets the value of ColorStrength of this IconStyle
     */
    double getColorStrength() const
    {
        return myColorStrength;
    }


    //----------------------------------------------------------------------
    // Setters

    /**
     * Sets the value of HeightFactor of this IconStyle
     */
    void setHeightFactor( double heightFactor )
    {
        myHeightFactor = heightFactor ;
    }

    /**
     * Sets the value of YOffset of this IconStyle
     */
    void setYOffset( int yOffset )
    {
        myYOffset = yOffset ;
    }

    /**
     * Sets the value of XOffset of this IconStyle
     */
    void setXOffset( int xOffset )
    {
        myXOffset = xOffset ;
    }

    /**
     * Sets the value of WidthFactor of this IconStyle
     */
    void setWidthFactor( double widthFactor )
    {
        myWidthFactor = widthFactor ;
    }

    /**
     * Sets the value of Color of this IconStyle
     */
    void setColor( dime::Color color )
    {
        myColor = color ;
    }

    /**
     * Sets the value of ColorStrength of this IconStyle
     */
    void setColorStrength( double colorStrength )
    {
        myColorStrength = colorStrength ;
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
