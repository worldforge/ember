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
#include <services/platform/BitmapRenderer.h>

// Included custom library headers
// (1) Include the signal system in headers file.
#if SIGC_MAJOR_VERSION == 1 && SIGC_MINOR_VERSION == 0
#include <sigc++/signal_system.h>
#else
#include <sigc++/signal.h>
#include <sigc++/object.h>
#endif

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
class Button : public Widget

{
    //======================================================================
    // Public Signals
    //======================================================================
    public:
    
    /**
    * Connect a slot here to observe when the button is clicked.
    */
    SigC::Signal1<void, Button*, SigC::Marshal<void> > onClicked;

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
    RectangleRenderer *myPressedBackground;
    RectangleRenderer *myHighlightBackground;
    RectangleRenderer *myStandardBackground;
    RectangleRenderer **myCurrentBackground;
    bool myPressed;

    //======================================================================
    // Public Methods
    //======================================================================
    public:

    //----------------------------------------------------------------------
    // Constructors

    /**
    * Creates a new Button using default values.
    */
    Button() : Widget(), myPressedBackground(NULL),
      myHighlightBackground(NULL),
      myStandardBackground(NULL),
      myPressed(false)
    {
		myCurrentBackground = &myStandardBackground;
    }

    /**
    * Creates a new Button using rect.
    */
    Button(Rectangle rect) : Widget(rect), myPressedBackground(NULL),
      myHighlightBackground(NULL),
      myStandardBackground(NULL),
      myPressed(false)
    {
		myCurrentBackground = &myStandardBackground;
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
      myPressedBackground = source.myPressedBackground;
      myStandardBackground = source.myStandardBackground;
      myHighlightBackground = source.myHighlightBackground;
      myCurrentBackground = source.myCurrentBackground;

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
      if (myParent != NULL) myParent->removeWidget(this);
      // TODO: Free any allocated resources here.
    }


    //----------------------------------------------------------------------
    // Getters


    //----------------------------------------------------------------------
    // Setters

    /**
     * Sets the pressed background RectangleRenderer of this Widget
     */
    virtual void setPressedBackground(RectangleRenderer *background)
      {
	myPressedBackground = background;
      }

    /**
     * Sets the highlighted background RectangleRenderer of this Widget
     */	
	virtual void setHighlightBackground(RectangleRenderer *background)
	{
		myHighlightBackground = background;
	}

    /**
     * Sets the standard background RectangleRenderer of this Widget
     */	
	virtual void setBackground(RectangleRenderer *background)
	{
		myStandardBackground = background;
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

    virtual void setHighlighted() { myCurrentBackground = &myHighlightBackground; }
	
    virtual void setLowlighted(){ myCurrentBackground = &myStandardBackground; }

    virtual void setPressed(){ myCurrentBackground = &myPressedBackground; }
	
    /**
     * A mouse button was pressed.
     * button is 1 for left, 2 for right and 3 for middle.
     * win is the window at position x, y (normally 'this').
     */
    virtual void mouseDown( MouseButtonEvent *event )
      {
	myPressed = true;
	setPressed();

	// Capture the mouse
	captureMouse();

        Widget::mouseDown(event);      
      }
        
    /**
     * A mouse button was released.
     * button is 1 for left, 2 for right and 3 for middle.
     * win is the window at position x, y (normally 'this').
     */
    virtual void mouseUp( MouseButtonEvent *event );

    /**
     * The mouse has just entered win.
     * win is the window at position x, y (normally 'this').
     */
    virtual void mouseEnter( MouseMotionEvent *event )
    {
      // Change how we look
      if (myPressed)
	setPressed();
      else
	setHighlighted();

      Widget::mouseEnter( event );
    }

    /**
     * The mouse has just left win.
     * win is the window at position x, y (normally 'this').
     */
    virtual void mouseExit( MouseMotionEvent *event ) 
    {
      // Change how we look
      setLowlighted();

      Widget::mouseExit( event );
    }
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


