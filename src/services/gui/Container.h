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

#ifndef CONTAINER_H
#define CONTAINER_H

// Included headers from the current project
#include "Widget.h"

// Included custom library headers

// Included system headers
#include <vector>

namespace dime {
    
/**
 * The interface class for containers.
 *
 * This interface allows widgets to be added and deleted.
 *
 *
 * @author Lakin Wecker
 * @author Adam Gregory
 *
 */
class Container : public Widget
{
    //======================================================================
    // Private Variables
    //======================================================================
    private:
		
	/**
	* Vector of the Container's children
	*/
	std::vector<Widget*> myChildren;
	
    //======================================================================
    // Public Methods
    //======================================================================
    public:

    //----------------------------------------------------------------------
    // Constructors

    /**
     * Creates a new Container using default values.
     */
    Container() : Widget()
    {
    }


    /**
     * Copy constructor.
     */
    Container( const Container &source )
    {
        // Use assignment operator to do the copy
        // NOTE: If you need to do custom initialization in the constructor this may not be enough.
        *this = source;
    }


    /**
     * Assignment operator.
     */
    Container &operator= ( const Container &source )
    {
        // Copy fields from source class to this class here.

        // Return this object with new value
        return *this;
    }


    //----------------------------------------------------------------------
    // Destructor

    /**
     * Deletes a Container instance.
     */
    virtual ~Container()
    {
		if (myParent != NULL) myParent->removeWidget(this);
        // TODO: Free any allocated resources here.
    }


    //----------------------------------------------------------------------
    // Getters


    //----------------------------------------------------------------------
    // Setters

    //----------------------------------------------------------------------
    // Other public methods

	/**
	 * Tells the children widgets in myChildren to draw themselves.
	 */
	virtual int draw(SDLDrawDevice *target);

	/**
	 * Checks if a mouse event has occured within the boundaries of the widget, and fires the appropriate signals
	 */
	virtual bool checkMouseEvent(std::vector<int> coords);
	
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


