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

#ifndef SOLIDBACKGROUND_H
#define SOLIDBACKGROUND_H

// Included headers from the current project
#include "BackgroundStyle.h"
// Included custom library headers

// Included system headers

namespace dime {
namespace theme {
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

class SolidBackground
{


    //======================================================================
    // Private Variables
    //======================================================================/
    private:

        Color myColor;

    //======================================================================
    // Public Methods
    //======================================================================
    public:

    //----------------------------------------------------------------------
    // Constructors

    /**
     * Creates a new SolidBackground using default values.
     */
     SolidBackground(double r, double g, double b)
	     : myColor(r,g,b)
     {
     }
     SolidBackground(Color myColor)
	     : myColor(myColor)
     {
     }

    /**
     * Copy constructor.
     */
    SolidBackground( const SolidBackground &source )
    {
        // Use assignment operator to do the copy
        // NOTE: If you need to do custom initialization in the constructor this may not be enough.
        *this = source;
    }


    /**
     * Assignment operator.
     */
    SolidBackground &operator= ( const SolidBackground &source )
    {
        // Copy fields from source class to this class here.
	myColor = source.getColor();
        // Return this object with new value
        return *this;
    }


    //----------------------------------------------------------------------
    // Destructor

    /**
     * Deletes a SolidBackground instance.
     */
    virtual ~SolidBackground ()
    {
        // TODO: Free any allocated resources here.
    }


    //----------------------------------------------------------------------
    // Getters

    /**
     * Gets the value of Color of this SolidBackground
     */
    Color getColor() const
    {
        return myColor;
    }


    //----------------------------------------------------------------------
    // Setters

    /**
     * Sets the value of Color of this SolidBackground
     */
    void setColor( Color color )
    {
        myColor = color ;
    }


    //----------------------------------------------------------------------
    // Other public methods
   
    virtual RectangleRenderer *createRectangleRenderer(Rectangle &rect)
    {
        return new RectangleRenderer(myColor); 
    }
    

}; // End of SolidBackground

} // End of theme namespace
} // End of dime namespace

#endif
