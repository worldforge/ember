/*
    Copyright (C) 2002  Adam Gregory

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

#ifndef RECTANGLE_H
#define RECTANGLE_H

// Included headers from the current project
#include <services/platform/Dimension.h>

// Included custom library headers

// Included system headers
#include <SDL/SDL.h>
#include <vector>

namespace dime{

/**
 * Provides a simple api to describe rectangular areas.
 *
 * 
 *
 * @author Adam Gregory
 *
 */
class Rectangle

{


    //======================================================================
    // Private Variables
    //======================================================================
    private:

	
	/**
    * The horizontal position of this Rectangle
    */
    int myX;

    /**
    * The vertical position of this Rectangle
    */
    int myY;
	
    /**
    * The width of this Rectangle
    */
    int myWidth;

    /**
    * The height of this Rectangle
    */
    int myHeight;

	/**
	 * SDL_Rect representation of this Rectangle.
	 */
	SDL_Rect mySDL_Rect;

    //======================================================================
    // Public Methods
    //======================================================================
    public:

    //----------------------------------------------------------------------
    // Constructors

    /**
    * Creates a new NameOfClass using default values.
    */
    Rectangle(int x, int y, int w, int h)
    {
		setX(x);
		setY(y);
		setWidth(w);
		setHeight(h);
    }


    Rectangle()
        : myX(0), myY(0), myWidth(0), myHeight(0)
    {
    }
    
    /**
    * Copy constructor.
    */
    Rectangle( const Rectangle &source )
    {
        *this = source;
    }


    /**
    * Assignment operator.
    */
    Rectangle &operator= ( const Rectangle &source )
    {
        // Copy fields from source class to this class here.
		this->myX = source.getX();
		this->myY = source.getY();
        this->myWidth = source.getWidth();
        this->myHeight = source.getHeight();
		this->mySDL_Rect = source.getSDL_Rect();
        // Return this object with new value
        return *this;
    }


    //----------------------------------------------------------------------
    // Destructor

    /**
    * Deletes a Rectangle instance.
    */
    virtual ~Rectangle()
    {
        // TODO: Free any allocated resources here.
    }


    //----------------------------------------------------------------------
    // Getters

    /**
    * Returns the horizontal position of the Rectangle
    */
    virtual int getX() const
    {
        return myX;
    }

    /**
    * Returns the vertical position of the Rectangle
    */
    virtual int getY() const
    {
        return myY;
    }

    /**
    * Returns the Width of the Rectangle
    */
    virtual int getWidth() const
    {
        return myWidth;
    }

    /**
    * Returns the Height of the Rectangle
    */
    virtual int getHeight() const
    {
        return myHeight;
    }
	
	/**
	 * Sets the dimensions for this Rectangle
	 */
	virtual Dimension getDimensions() const
	{
		return Dimension(getWidth(),getHeight());
	}
	
	/**
	 * Returns the SDL_Rect representation of this Rectangle
	 */
	virtual SDL_Rect getSDL_Rect() const
	{
		return mySDL_Rect;
	}

    //----------------------------------------------------------------------
    // Setters

    /**
    * Sets the horizontal position for this Rectangle
    */
	virtual void setX(int x)
	{
		myX = x;
		mySDL_Rect.x = x;
	}
    
    /**
    * Sets the vertical position of this Rectangle
    */
    virtual void setY(int y)
    {
		myY = y;
		mySDL_Rect.y = y;
    }

    /**
    * Sets the width for this Rectangle
    */
    virtual void setWidth(int width)
    {
        if(width>0)
            {
                myWidth = width;
				mySDL_Rect.w = width;
            }
    }
    
    /**
    * Sets the height for this Rectangle
    */
    virtual void setHeight(int height)
    {
        if(height>0)
            {
                myHeight = height;
                mySDL_Rect.h = height;
            }
    }
	
	/**
	 * Sets the dimensions for this Rectangle
	 */
	virtual void setDimensions(Dimension dimension)
	{
		setWidth(dimension.getWidth());
		setHeight(dimension.getHeight());
	}
	
    //----------------------------------------------------------------------
    // Other public functions
	
	/**
	 * Tests if a set of co-ordinates lies inside this rectangle
	 */
	virtual bool contains(std::vector<int> coords)
	{
		if (coords.size() != 2)
		{
			return false; // Invalid coordinates vector.
		}
		if ((coords[0] > getX()) && (coords[0] < getX() + getWidth()) && (coords[1] > getY()) && (coords[1] < getY() + getHeight()))
		{
			return true;
		}
		return false;
	}
    /**
     * Test if these coordinates lie inside this rectangle
     */
    virtual bool contains(int x, int y)
    {
        if((x > getX()) && (x < (getX() + getWidth())) &&
           (y > getY()) && (y < (getY() + getHeight())))
            {
                return true;
            }
        else
            {
                return false;
            }
    }
    

}; // End of class


} // End of application namespace

#endif


