/*
    Copyright (C) 2002  Lakin Wecker aka(nikal)

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

#ifndef DIMENSION_H
#define DIMENSION_H

// Included headers from the current project

// Included custom library headers

// Included system headers

namespace dime{

/**
 * Provides a simple api for the Dimensions of stuff.
 *
 * This class could also have been called "Size".  however
 * dimension seemed to be a more generic name which worked better.
 * This class provides the Dimensions for Widgets and Layout etc. 
 *
 * @author Lakin Wecker
 *
 */
class Dimension

{


    //======================================================================
    // Private Variables
    //======================================================================
    private:

    /**
    * The width of this Dimension
    */
    int myWidth;

    /**
    * The height of this dimensions
    */
    int myHeight;

    //======================================================================
    // Public Methods
    //======================================================================
    public:

    //----------------------------------------------------------------------
    // Constructors

    /**
    * Creates a new NameOfClass using default values.
    */
    Dimension(int w, int h)
        : myWidth(w), myHeight(h)
    {
    }


    Dimension()
        : myWidth(0), myHeight(0)
    {
    }
    
    /**
    * Copy constructor.
    */
    Dimension( const Dimension &source )
    {
        *this = source;
    }


    /**
    * Assignment operator.
    */
    Dimension &operator= ( const Dimension &source )
    {
        // Copy fields from source class to this class here.
        this->myWidth = source.getWidth();
        this->myHeight = source.getHeight();
        // Return this object with new value
        return *this;
    }


    //----------------------------------------------------------------------
    // Destructor

    /**
    * Deletes a Dimension instance.
    */
    virtual ~Dimension()
    {
        // TODO: Free any allocated resources here.
    }


    //----------------------------------------------------------------------
    // Getters

    /**
     * Returns the Width of the Dimension
     */
    virtual int getWidth() const
    {
        return myWidth;
    }

    /**
    * Returns the Height of the Dimension
    */
    virtual int getHeight() const
    {
        return myHeight;
    }

    //----------------------------------------------------------------------
    // Setters

    /**
    * Sets the width for this Dimension
    */
    virtual void setWidth(int width)
    {
        if(width>0)
            {
                myWidth = width;
            }
    }
    
    /**
    * Sets the height for this Dimension
    */
    virtual void setHeight(int height)
    {
        if(height>0)
            {
                myHeight = height;
            }
    }

}; // End of class


} // End of application namespace

#endif


