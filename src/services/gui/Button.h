/*
    Copyright (C) 2002  Lakin Wecker aka (nikal)

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

#ifndef BUTTON_H
#define BUTTON_H

// Included headers from the current project
#include "Widget.h"
#include <services/platform/RectangleRenderer.h>
#include <services/platform/SDLDrawDevice.h>

// Included custom library headers
// (1) Include the signal system in headers file.
#include <sigc++/signal_system.h>

// Included system headers

namespace dime {

/**
 * The Button interface.
 *
 * This is the Button Widget Interface.
 * This Class should never be used directly, 
 * but rather extended as a binding to a particular Widget Set.
 *
 * 
 *
 * @author Lakin Wecker
 * @author Adam Gregory
 *
 *
 */
class Button : public Widget, public SigC::Object

{
	//======================================================================
    // Public Signals
    //======================================================================
    public:
    
    /**
     * Connect a slot here to observe when a mouse button is pressed.
     */
    SigC::Signal1<void, Button*> onMouseDown;
	
    /**
     * Connect a slot here to observe when a mouse button is released.
     */
    SigC::Signal1<void, Button*> onMouseUp;
    
    /**
     * Connect a slot here to observe MouseMotions
     */
    SigC::Signal1<void, Button*> onMouseMove;
    
    /**
     * Connect a slot here to observe when MouseMoves over this Widget
     */
    SigC::Signal1<void, Button*> onMouseEnter;
    
    /**
     * Connect a slot here to observe when a Mouse leaves this Widget
     */
    SigC::Signal1<void, Button*> onMouseExit;
    
    /**
     * Connect a slot here to observe when a key is pressed and this Widget has focus.
     */
    SigC::Signal2<void, Button*, SDLKey> onKeyPress;

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
    //======================================================================
    private:

		RectangleRenderer *myBackground;

    //======================================================================
    // Public Methods
    //======================================================================
    public:

    //----------------------------------------------------------------------
    // Constructors

    /**
     * Creates a new Button using default values.
     */
    Button() : Widget()
    {
    }


    /**
     * Copy constructor.
     */
    Button( const Button &source )
    {
        // Use assignment operator to do the copy
        // NOTE: If you need to do custom initialization in the constructor this may not be enough.
        *this = source;
    }


    /**
     * Assignment operator.
     */
    Button &operator= ( const Button &source )
    {
        // Copy fields from source class to this class here.

        // Return this object with new value
        return *this;
    }


    //----------------------------------------------------------------------
    // Destructor

    /**
     * Deletes a Button instance.
     */
    virtual ~Button()
    {
        // TODO: Free any allocated resources here.
    }


    //----------------------------------------------------------------------
    // Getters


    //----------------------------------------------------------------------
    // Setters

    /**
     * Sets the background RectangleRenderer of this Widget
     */	
	virtual void setBackground(RectangleRenderer *background)
	{
		myBackground = background;
	}

    //----------------------------------------------------------------------
    // Other public methods

	/**
	 * Draws the widget, and/or its children.
	 */
    virtual int draw();

	/**
	 * Checks if a mouse event has occured within the boundaries of the widget, and fires the appropriate signals
	 */
	virtual bool checkMouseEvent(std::vector<int> coords);
	
	virtual void highlight();
	
	virtual void lowlight();

	
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


}; // End of class
} // End of Dime namespace

#endif


