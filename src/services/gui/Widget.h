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

#ifndef WIDGET_H
#define WIDGET_H

// Included headers from the current project
#include <services/platform/Dimension.h>
#include <services/platform/Rectangle.h>
#include <services/platform/DrawDevice.h>

// Included custom library headers

// Included system headers
#include <vector>
#include <SDL/SDL.h>
// Include the signal system in headers file.
#include <sigc++/signal_system.h>

namespace dime {

	
/**
 * The basic Widget for the Dime Gui
 *
 * This Widget class will have the basic functionality.
 * It will be able to be moved, resized, rethemed, and drawn.
 *
 * A short piece of example code demonstarting how this class it is used,
 * and in what context, is encouraged.
 *
 * @author Lakin Wecker
 * @author Adam Gregory
 *
 */

class Widget : public SigC::Object

{
    //======================================================================
    // Protected Variables
    //======================================================================
	protected:
	
    /**
	 * Whether the mouse cursor is inside this widget or not
	 */
	bool myMouseIsInside;
	
    /**
    * The dimensions of this Widget
    */
	Rectangle myRectangle;
	
	/**
	 * Pointer to the parent Widget of this Widget. NULL if none
	 */
	 Widget* myParent;

    std::vector<Widget *> myChildren;
    
    //======================================================================
    // Private Variables
    //======================================================================
    private:

    /**
    * The maximum Dimension of this widget
    */
    Dimension myMaxDimension;
    
    /**
    * The minimum Dimension of this widget
    */
    Dimension myMinDimension;
    
    /**
    * The prefered Dimension of this widget
    */
    Dimension myPrefDimension;


    //======================================================================
    // Public Methods
    //======================================================================
    public:

    //----------------------------------------------------------------------
    // Constructors

    /**
    * Creates a new Widget using default values.
    */
    Widget(): myParent(NULL)
    {

    }


    /**
    * Copy constructor.
    */
    Widget( const Widget &source )
    {
        // Use assignment operator to do the copy
        // NOTE: If you need to do custom initialization in the constructor this may not be enough.
        *this = source;
    }

	/**
	 * Assignment operator.
	 */
    Widget &operator= ( const Widget &source );

    //----------------------------------------------------------------------
    // Destructor

    /**
    * Deletes a Widget instance.
    */
    virtual ~Widget();

	
    //----------------------------------------------------------------------
    // Getters
	
	/**
	 * Returns a pointer to the parent of this Widget
	 */
	Widget* getParent()
	{
		return myParent;
	}
    
    /**
    * Returns the rectangular area of this Widget
    */
    virtual const Rectangle &getRectangle() const
    {
        return myRectangle;
    }
    
    /**
    * Returns the maximum Dimension for this Widget
    */
    virtual Dimension* getMaxDimension()
    {
        return &myMaxDimension;
    }
    
    /**
    * Returns the minimum Dimension for this Widget
    */
    virtual Dimension getMinDimension() const
    {
        return myMinDimension;
    }    
    
    /**
    * Returns the prefered Dimension for this Widget
    */
    virtual Dimension getPrefDimension() const
    {
        return myPrefDimension;
    }

    //----------------------------------------------------------------------
    // Setters

	/**
	 * Sets the pointer to the parent widget
	 */
	void setParent(Widget* parent);
    
    /**
    * Sets the rectangular area of this Widget
    */
    virtual void setRectangle(Rectangle rectangle) 
    {
        myRectangle = rectangle;
    }
    
    /**
    * sets the maximum Dimension for this Widget
    */
    virtual void setMaxDimension(Dimension dimension) 
    {
        myMaxDimension = dimension;
    }
    
    /**
    * Sets the minimum Dimension for this Widget
    */
    virtual void setMinDimension(Dimension dimension)
    {
       myMinDimension = dimension;
    }    
    
    /**
    * Sets the prefered Dimension for this Widget
    */
    virtual void setPrefDimension(Dimension dimension)
    {
        myPrefDimension = dimension;
    }
	
    //----------------------------------------------------------------------
    // Other public methods	
	
	/**
	 * Draws the widget, and/or its children.
	 */
    virtual int draw(DrawDevice *target);

	
	/**
	 * Checks if a mouse event has occured within the boundaries of the widget, and fires the appropriate signals
	 */
	virtual bool checkMouseEvent(std::vector<int> coords);
	
	/**
	 * Moves the widget to the co-ordinates provided
	 */
	virtual void move(std::vector<int> coords)
	{
		if (coords.size() == 2)
		{
			myRectangle.setX(coords[0]);
			myRectangle.setY(coords[1]);
		}
	}
	virtual void move(int x, int y)
	{
			myRectangle.setX(x);
			myRectangle.setY(y);
	}
	
	/**
	 * Resizes the widget to the dimensions provided
	 */
	virtual void resize(Dimension dimension)
	{
			myRectangle.setDimensions(dimension);
	}
	virtual void resize(int width, int height)
	{
			myRectangle.setWidth(width);
			myRectangle.setHeight(height);
	}

    /**
     * Adds a new child widget to myChildren.
     */
    void addWidget(Widget* source);
	
	
    /**
    * Removes pointer to widget from myChildren.
    * 
    */
    int removeWidget(Widget* target);
    
}; // End of class


} // End of application namespace

#endif


