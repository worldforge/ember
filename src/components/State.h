/*
 Copyright (C) 2002  Martin Pollard
	
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

#ifndef STATE_H
#define STATE_H

// Included headers from the current project
#include "WidgetInfo.h"
#include "main/DimeServices.h"

// Included custom library headers

// Included system headers
#include <list>
#include <string>

namespace dime {

/**
 * Contains the information to draw a given state.
 *
 * More detailed description of the class, it's purpose, what it does,
 * how to use it and so on.
 *
 * A short piece of example code demonstarting how this class it is used,
 * and in what context, is encouraged.
 *
 * @author Martin Pollard aka Xmp
 *
 * @see dime::StateManager
 * @see dime::WidgetInfo
 *
 */

class State
{
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
    //======================================================================/
    private:

        std::string myName;
        std::list<WidgetInfo> myWidgets;

    //======================================================================
    // Public Methods
    //======================================================================
    public:

    //----------------------------------------------------------------------
    // Constructors

    /**
     * Creates a new State using default values.
     */
     State()
     {
     }

    /**
     * Copy constructor.
     */
    State( const State &source )
    {
        // Use assignment operator to do the copy
        // NOTE: If you need to do custom initialization in the constructor this may not be enough.
        *this = source;
    }


    /**
     * Assignment operator.
     */
    State &operator= ( const State &source )
    {
        // Copy fields from source class to this class here.
        myName=source.myName;
	myWidgets=source.myWidgets;

        // Return this object with new value
        return *this;
    }


    //----------------------------------------------------------------------
    // Destructor

    /**
     * Deletes a State instance.
     */
    virtual ~State ()
    {
        // TODO: Free any allocated resources here.
    }


    //----------------------------------------------------------------------
    // Getters

    // Example of a getter method:

    /**
     * Gets the value of Name of this State
     */
    std::string getName() const
    {
        return myName;
    }

    /**
     * Gets the value of Widgets of this State
     */
    std::list<WidgetInfo> getWidgets() const
    {
        return myWidgets;
    }


    //----------------------------------------------------------------------
    // Setters

    /**
     * Sets the value of Name of this State
     */
    void setName( std::string name )
    {
        myName = name ;
    }

    /**
     * Adds a WidgetInfo to this State
     */
    void addWidget( const WidgetInfo& widget )
    {
        myWidgets.insert(myWidgets.end(),WidgetInfo(widget));
    }

    /**
     * Creates and adds a WidgetInfo to this State
     */
    void addWidget(const std::string& name, const dime::Rectangle& pos, const std::string& type)
      {
        myWidgets.insert( myWidgets.end(), WidgetInfo( name, pos,type ) );
      }


    //----------------------------------------------------------------------
    // Other public methods
    // NOTE: Group related public methods together and crate a separator comment like above for them.

    void load()
      {
	Widget& root = DimeServices::getInstance()->getGuiService()->getRootWidget();

	// For each widget in myWidgets ask it to make itself and
	// children. Then return a pointer to the made Widget for
	// addition to root widget.
      }

    void unload()
      {
	DimeServices::getInstance()->getGuiService()->getRootWidget().removeAllWidgets();
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


}; // End of State

} // End of dime namespace

#endif
