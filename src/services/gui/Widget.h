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
// (1) Include the signal system in headers file.
#include <sigc++/signal_system.h>

// Included system headers


namespace Dime {

/**
 * The basic Widget for the Dime Gui
 *
 * This Widget class will have the basic functionality.
 * It will be able to be moved, resized, rethemed, and drawn.
 *
 * A short piece of example code demonstarting how this class it is used,
 * and in what context, is encouraged.
 *
 * @author  Lakin Wecker
 *
 *
 */
class Widget

{

    //======================================================================
    // Public Signals
    //======================================================================
    public:
    
    /**
     * Connect a slot here to Observe MouseClicks.
     */
    Signal1<void, MouseClick> onMouseClick;
    
    /**
     * Connect a slot here to observe MouseMotions
     */
    Signal1<void, MouseMotion> onMouseMove;
    
    /**
     * Connect a slot here to observe when MouseMoves over this Widget
     */
    Signal1<void, MouseMotion> onMouseEnter;
    
    /**
     * Connect a slot here to observe when a Mouse leaves this Widget
     */
    Signal1<void, MouseMotion> onMouseExit;
    
    /**
     * Connect a slot here to observe when a key is pressed and this Widget has focus.
     */
    Signal1<void, KeyPress> onKeyPress;

    

    //======================================================================
    // Private Variables
    //======================================================================
    private:

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
     * Cretaes a new NameOfClass using default values.
     */
    Widget()
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
    Widget &operator= ( const Widget &source )
    {
        // Copy fields from source class to this class here.

        // Return this object with new value
        return &this;
    }


    //----------------------------------------------------------------------
    // Destructor

    /**
     * Deletes a Widget instance.
     */
    virtual ~Widget()
    {
        // TODO: Free any allocated resources here.
    }


    //----------------------------------------------------------------------
    // Getters
    /**
     * Returns theX position for this Widget
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
     * returns the prefered Dimension for this Widget
     */
    virtual Dimension getPrefDimension() const
    {
        return myPrefDimension;
    }
    

    //----------------------------------------------------------------------
    // Setters

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
    virtual setPrefDimension(Dimension dimension)
    {
        myPrefDimension = dimension;
    }


}; // End of class


} // End of application namespace

#endif


