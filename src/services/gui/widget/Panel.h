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

#ifndef PANEL_H
#define PANEL_H

// Included headers from the current project
#include "Widget.h"
#include <services/platform/RectangleRenderer.h>

// Included custom library headers

// Included system headers


namespace dime {

/**
 * Panel widget Arranges children widgets on top of a background.
 *
 * Derived from Widget. Useful for creating dialogs.
 * 
 * TODO: A short piece of example code demonstrating how this class is used,
 * and in what context, is encouraged.
 *
 * @author Adam Gregory
 */
class Panel : public Widget

{
    //======================================================================
    // Public Signals
    //======================================================================
    public:
    
    /**
    * Connect a slot here to observe when a mouse button is pressed.
    */
    SigC::Signal1<void, Panel*, SigC::Marshal<void> > onMouseDown;
	
    /**
    * Connect a slot here to observe when a mouse button is released.
    */
    SigC::Signal1<void, Panel*, SigC::Marshal<void> > onMouseUp;
    
    /**
    * Connect a slot here to observe MouseMotions
    */
    SigC::Signal1<void, Panel*, SigC::Marshal<void> > onMouseMove;
    
    /**
    * Connect a slot here to observe when MouseMoves over this Widget
    */
    SigC::Signal1<void, Panel*, SigC::Marshal<void> > onMouseEnter;
    
    /**
    * Connect a slot here to observe when a Mouse leaves this Widget
    */
    SigC::Signal1<void, Panel*, SigC::Marshal<void> > onMouseExit;
    
    /**
    * Connect a slot here to observe when a key is pressed and this Widget has focus.
    */
    SigC::Signal2<void, Panel*, SDLKey, SigC::Marshal<void> > onKeyPress;

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
	
    /**
     * RectangleRenderer to display background of Panel.
     */
    RectangleRenderer *myBackground;

    //======================================================================
    // Public Methods
    //======================================================================
    public:

    //----------------------------------------------------------------------
    // Constructors

    /**
    * Creates a new Panel using default values.
    */
    Panel() : Widget(), myBackground(NULL)
    {
    }

    /**
    * Creates a new Panel using rect
    */
    Panel(Rectangle rect) : Widget(rect), myBackground(NULL)
    {
    }

    /**
    * Creates a new Panel using rect and renderer
    */
    Panel(Rectangle rect, RectangleRenderer* rr) : Widget(rect),
      myBackground(rr)
    {
    }

    /**
    * Copy constructor.
    */
    Panel( const Panel &source )
    {
        // Use assignment operator to do the copy
        // NOTE: If you need to do custom initialization in the constructor this may not be enough.
        *this = source;
    }

    /**
    * Assignment operator.
    */
    Panel &operator= ( const Panel &source )
    {
        // Copy fields from source class to this class here.

        // Return this object with new value
        return *this;
    }

    //----------------------------------------------------------------------
    // Destructor

    /**
    * Deletes a Panel instance.
    */
    virtual ~Panel()
    {
		if (myParent != NULL) myParent->removeWidget(this);
        // TODO: Free any allocated resources here.
    }


    //----------------------------------------------------------------------
    // Getters


    //----------------------------------------------------------------------
    // Setters

	/**
    * Sets the standard background RectangleRenderer of this Widget
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
    virtual int draw(DrawDevice *target);

	/**
	 * Checks if a mouse event has occured within the boundaries of the widget, and fires the appropriate signals
	 */
	virtual bool checkMouseEvent(std::vector<int> coords);

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

} // End of application namespace

#endif
