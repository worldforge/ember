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
#include "../EventGenerator.h"
#include "../KeyPressEvent.h"
#include "../MouseButtonEvent.h"
#include "../MouseMotionEvent.h"

#include <services/platform/Dimension.h>
#include <services/platform/Rectangle.h>
#include <services/platform/DrawDevice.h>

// Included custom library headers
// Include the signal system in headers file.
#if SIGC_MAJOR_VERSION == 1 && SIGC_MINOR_VERSION == 0
#include <sigc++/signal_system.h>
#else
#include <sigc++/signal.h>
#include <sigc++/object.h>
#endif
#include <SDL/SDL.h>

// Included system headers
#include <vector>

namespace dime {
    
    class EventGenerator;

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

	/**
         * Children of this Widget
         */
        std::vector<Widget *> myChildren;
    
	/**
	 * Is widget visible? If a widget is not visible then it's children are not visible also
	 * (Except for the root widget who isn't called a by the draw() function)
	 */
	bool myVisible;

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

        /**
         * The EventGenerator for this widget
         */
        EventGenerator *myEventGenerator;
        

        //======================================================================
        // Public Methods
        //======================================================================
    public:

        //----------------------------------------------------------------------
        // Constructors

        /**
         * Creates a new Widget using default values.
         */
        Widget(): myParent(NULL), myVisible(true)
        {

        }

        /**
         * Creates a new Widget using rect.
         */
        Widget(const Rectangle& rect): myRectangle(rect), myParent(NULL),
	  myVisible(true)
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


        //---------------------------------------------------------------------------------------------------
        // Event Slots
        //---------------------------------------------------------------------------------------------------
        /**
         * Connect a slot here to observe when a mouse button is pressed.
         */
        SigC::Signal1<void, MouseButtonEvent *, SigC::Marshal<void> > onMouseDown;
        
        /**
         * Connect a slot here to observe when a mouse button is released.
         */
        SigC::Signal1<void, MouseButtonEvent*, SigC::Marshal<void> > onMouseUp;
        
        /**
         * Connect a slot here to observe a mouse Click
         */
        SigC::Signal1<void, MouseButtonEvent*, SigC::Marshal<void> > onMouseClick;
        
        /**
         * Connect a slot here to observer a moust multiclick
         */
        SigC::Signal1<void, MouseButtonEvent*, SigC::Marshal<void> > onMouseMultiClick;
        
        /**
         * Connect a slot here to observe MouseMotions
         */
        SigC::Signal1<void, MouseMotionEvent*, SigC::Marshal<void> > onMouseMove;
        
        /**
         * Connect a slot here to observe when MouseMoves over this Widget
         */
        SigC::Signal1<void, MouseMotionEvent*, SigC::Marshal<void> > onMouseEnter;
        
        /**
         * Connect a slot here to observe when a Mouse leaves this Widget
         */
        SigC::Signal1<void, MouseMotionEvent*, SigC::Marshal<void> > onMouseExit;
        
        /**
         * Connect a slot here to observe when a key is pressed and this Widget has focus.
         */
        SigC::Signal1<void, KeyPressEvent*, SigC::Marshal<void> > onKeyPress;
        


	
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

	/**
         * Returns whether or not this Widget is visible
         */
	virtual bool getVisible () const
	  {
	    return myVisible;
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

	/**
         * Sets whether or not a widget is visible
         */
	virtual void setVisible(bool visible)
	  {
	    myVisible = visible;
	  }

	//----------------------------------------------------------------------
	// Togglers

	/**
	 * Toggles visiblity
	 */
	virtual bool toggleVisible()
	{
	  myVisible = !myVisible;
	  //%TASK UpdateRect,3: add function to GuiService to call SDL_UpdateRect.  This should be abstracted through platformservice
	  return myVisible;
	}

        //----------------------------------------------------------------------
        // Other public methods
    
        /**
         * Draws the widget, and/or its children.
         */
        virtual int draw(DrawDevice *target);
    
        /**
         * updates the widget and it's children if necesary.
         *
         *@return bool whether something was updated or not.
         */
        virtual bool update(DrawDevice *target) { 
			//%TODO nikal,2: What should be returned here
			return false; };
        
        
        /**
         * Checks if a mouse event has occured within the boundaries of the widget, and fires the appropriate signals
         */
        virtual bool checkMouseEvent(std::vector<int> coords);

        /**
         * Sets the EventGenerator for this widget (and it's children)
         */
        virtual void setEventGenerator(EventGenerator *generator);
        
        /**
         * Returns the Widget that contains these absolute coordinates.
         * It first checks if they fall within the boundaries of this widget.
         * If not it returns NULL.  If so it checks the children, and returns the
         * the topmost widget that contains these coordinates.  If no child widgets 
         * fall in this category then it returns this.
         */
        virtual Widget *getWidgetAt(int x, int y);

        //---------------------------------------------------------------------------------------------------
        //---------------------------------------------------------------------------------------------------
        // Start Event Methods
        //---------------------------------------------------------------------------------------------------
        //---------------------------------------------------------------------------------------------------


        //----------------------------------------------------------------------
        // Dragging events
        //-----------------------------------------------------------------------
        
        /**
         * Called when an other widget is dragged and dropped on this
         * widget.  x and y are the mouse coordinates and button is the mouse
         * button used in the dragging.  obj is the window object dropped.
         * Returns bool true if the object is accepted, false if the drag should 
         * be cancelled and the object returned to its previous host and coordinates.
         *
         * @return bool true if obj is accepted false if not
         */
        //virtual bool widgetDragDrop( DragDropEvent *event ) { };
        
        
        /**
         * Called when an other window object is dragged over this window.
         * x and y are the mouse coordinates and button is the mouse
         * button used in the dragging.  obj is the window object beeing dragged.
         * Returns the cursor that the mouse cursor should change into.
         */
        // commented out till we get MouseCursor.
        //virtual MouseCursor *widgetDragOver( int x, int y, int button, widget *obj );
        
        
        //----------------------------------------------------------------------
        // High level keyboard events:
        //----------------------------------------------------------------------
        
        /**
         * A key was pressed.
         * key is the value returned by allegros readkey -function.
         * (it contains both scancode and ascii info.)
         * Returns true if the keypress was processed.
         * Override the event methods in derived classes when neccesary.
         */
        
	virtual bool keyPress( KeyPressEvent *event)
        {
	    onKeyPress.emit(event);
            return false;
        }
        

        /**
        * Captures the keyboard events, all keyboard events are sent to this window
        * until releaseKeyboard is called.  Returns true if keyboard was successfully
        * captured (if some other window already had it captureKeyboard fails).
        */
        virtual bool captureKeyboard() {
			//%TODO nikal,2: What should be returned here
			return false;
		}
        
        /**
         * Releases the keyboard if it was captured by captureKeyboard().
         */
        virtual void releaseKeyboard() { }
        


        /*----[ High level mouse events: ]--------------------------------------*/
        /**
         * A mouseclick.  button is 1 for left, 2 for right and 3 for middle.
         * win is the window at position x, y (normally 'this').
         */
        virtual void mouseClick( MouseButtonEvent* ) { }

        /**
         * A mouse button was double-clicked.
         * button is 1 for left, 2 for right and 3 for middle.
         * win is the window at position x, y (normally 'this').
         */
        virtual void mouseDblClick( MouseButtonEvent *event) { }


        /**
         * The window object was dragged with some mouse button
         * (=mouse button was pressed and mouse was moved more than a few pixels).
         * Obj is the dragged window, button is the dragging mouse button.
         * Returns true if it is ok to start the dragging (the window object
         * captures the mouse and moves when the mouse is moved).
         * False if the drag should be ignored.
         */
        //        virtual bool mouseDragStart( DragDropEvent *event) { }
        
        /**
         * The dragging was canceled by pressing any other mouse button than
         * the one that started the drag.  The window object is automatically
         * returned to its previous host object and coordinates.
         * Button is the button pressed to cancel the drag.
         */
        //virtual void mouseDragCancel( DragDropEvent *event ) { };
        
        /*
         * The mouse button used to drag the window object was released.
         * 'dest' is the window object under the mouse (and the dragged window).
         * Return true if the drag is accepted, false if the dragged window should
         * be returned to its previous host an coordinates.
         * Calls the objectDragDrop method of destination window,
         * and cancells the drag if destination->objectDragDrop returns false.
         */
        //virtual bool mouseDragEnd( DragDropEvent *event ) { }

        /**
         * The mouse has just entered win.
         * win is the window at position x, y (normally 'this').
         */
        virtual void mouseEnter( MouseMotionEvent *event )
        {
            onMouseEnter.emit(event);
        }

        /**
         * The mouse has just left win.
         * win is the window at position x, y (normally 'this').
         */
        virtual void mouseExit( MouseMotionEvent *event ) 
        {
            onMouseExit.emit(event);
        }

        /**
         * Captures the mouse events, all mouse events are sent to this window
         * until releaseMouse is called.  Returns true if mouse was successfully
         * captured (if some other window already had it captureMouse fails).
         */
        virtual bool captureMouse() { 
			//%TODO nikal,2: What should be returned here
			return false;};

        /**
         * Releases the mouse if it was captured by captureMouse().
         */
        virtual void releaseMouse() { };

        /*----[ Low level mouse events: ]---------------------------------------*/

        /**
         * A mouse button was pressed.
         * button is 1 for left, 2 for right and 3 for middle.
         * win is the window at position x, y (normally 'this').
         */
        virtual void mouseDown( MouseButtonEvent *event )
        {
            onMouseDown.emit(event);
            
        }
        
        /*
         * A mouse button was released.
         * button is 1 for left, 2 for right and 3 for middle.
         * win is the window at position x, y (normally 'this').
         */
        virtual void mouseUp( MouseButtonEvent *event ) 
        {
            onMouseUp.emit(event);
        }

        /**
         * The mouse was moved.  x and y are the new mouse coordinates.
         * win is the window at position x, y (normally 'this').
         * Check for dragging and calls mouseDragStart if neccesary.
         */
        virtual void mouseMove( MouseMotionEvent *event )
        {

        }


        //---------------------------------------------------------------------------------------------------
        //---------------------------------------------------------------------------------------------------
        // End Event methods
        //---------------------------------------------------------------------------------------------------
        //---------------------------------------------------------------------------------------------------

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

	/**
	 * Removes all child widgets
	 */
	void removeAllWidgets();
    
    }; // End of class


} // End of application namespace

#endif


