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
#include "Dimension.h"

// Included custom library headers

// Included system headers
#include <vector>
#include <SDL/SDL.h>
// Include the signal system in headers file.
#include <sigc++/signal_system.h>

namespace dime {

// Forward declarations
class Container;
	
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
    // Public Signals
    //======================================================================
    public:
    
    /**
     * Connect a slot here to observe when a mouse button is pressed.
     */
    SigC::Signal1<void, Widget*> onMouseDown;
	
    /**
     * Connect a slot here to observe when a mouse button is released.
     */
    SigC::Signal1<void, Widget*> onMouseUp;
    
    /**
     * Connect a slot here to observe MouseMotions
     */
    SigC::Signal1<void, Widget*> onMouseMove;
    
    /**
     * Connect a slot here to observe when MouseMoves over this Widget
     */
    SigC::Signal1<void, Widget*> onMouseEnter;
    
    /**
     * Connect a slot here to observe when a Mouse leaves this Widget
     */
    SigC::Signal1<void, Widget*> onMouseExit;
    
    /**
     * Connect a slot here to observe when a key is pressed and this Widget has focus.
     */
    SigC::Signal2<void, Widget*, SDLKey> onKeyPress;


    //======================================================================
    // Protected Variables
    //======================================================================
	protected:
	
    /**
	 * Whether the mouse cursor is inside this widget or not
	 */
	bool myMouseIsInside;

    //======================================================================
    // Private Variables
    //======================================================================
    private:

	/**
	 * Pointer to the parent Container of this Widget. NULL if none
	 */
	 
	 Container* myParent;

    /**
     * The X position of this widget relative to the application
     */
    int myX;
    
    /**
     *The Y position of this widget relative to the application
     */
    int myY;

    /**
     * The dimensions of this Widget
     */
    Dimension myDimension;

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
     * Cretaes a new Widget using default values.
     */
    Widget()
    {
		myParent = NULL;
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
	Container* getParent()
	{
		return myParent;
	}
	
    /**
     * Returns the X position for this Widget
     */
    virtual int getX() const
    {
        return myX;
    }

    /**
     * Returns the Y position for this Widget
     */
    virtual int getY() const
    {
        return myY;
    }
    
    /**
     * Returns the dimensions of this Widget
     */
    virtual Dimension getDimension() const
    {
        return myDimension;
    }
    
    /**
     * Returns the maximum Dimension for this Widget
     */
    virtual Dimension getMaxDimension() const
    {
        return myMaxDimension;
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
	void setParent(Container* parent);

	/**
	 * Sets theX position for this Widget
	 */
	virtual void setX(int x)
    {
        myX = x;
    }

    /**
     * Sets the Y position for this Widget
     */
    virtual void setY(int y)
    {
        myY = y;
    }
    
    /**
     * Sets the dimensions of this Widget
     */
    virtual void setDimension(Dimension dimension) 
    {
        myDimension = dimension;
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
    virtual int draw() = 0;

	
	/**
	 * Checks if a mouse event has occured within the boundaries of the widget, and fires the appropriate signals
	 */
	virtual bool checkMouseEvent(std::vector<int> coords);


}; // End of class


} // End of application namespace

#endif


