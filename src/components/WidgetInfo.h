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

#ifndef WIDGETINFO_H
#define WIDGETINFO_H

// Included headers from the current project
#include "services/platform/Rectangle.h"

// Included custom library headers

// Included system headers
#include <string>
#include <vector>

namespace dime {

/**
 * Container Class for Info on a Widget in a State.
 *
 * More detailed description of the class, it's purpose, what it does,
 * how to use it and so on.
 *
 * A short piece of example code demonstarting how this class it is used,
 * and in what context, is encouraged.
 *
 * @author Martin Pollard aka Xmp
 *
 * @see dime::State
 *
 */

class WidgetInfo
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

        /*
         * Unique Identifying name of Widget
         */
        std::string myName;
	
	/*
	 * Rectangle of Widget
	 */
        dime::Rectangle myPos;

	/*
	 * Type of widget (IE the theme we are getting this widget from.
	 */
        std::string myType;

	/*
	 * Children of Widget
	 */
	std::vector<WidgetInfo> myChildren;

    //======================================================================
    // Public Methods
    //======================================================================
    public:

    //----------------------------------------------------------------------
    // Constructors

    /**
     * Creates a new WidgetInfo.
     */
    WidgetInfo(const std::string& name, const dime::Rectangle& pos, const std::string& type)
      : myName(name), myPos(pos), myType(type)
     {
     }

    /**
     * Copy constructor.
     */
    WidgetInfo( const WidgetInfo &source )
    {
        // Use assignment operator to do the copy
        // NOTE: If you need to do custom initialization in the constructor this may not be enough.
        *this = source;
    }


    /**
     * Assignment operator.
     */
    WidgetInfo &operator= ( const WidgetInfo &source )
    {
        // Copy fields from source class to this class here.
        myName = source.myName;
	myPos = source.myPos;
	myType = source.myType;

        // Return this object with new value
        return *this;
    }


    //----------------------------------------------------------------------
    // Destructor

    /**
     * Deletes a WidgetInfo instance.
     */
    virtual ~WidgetInfo ()
    {
        // TODO: Free any allocated resources here.
    }


    //----------------------------------------------------------------------
    // Getters

    // Example of a getter method:

    /**
     * Gets the value of myPos of this WidgetInfo
     */
    dime::Rectangle getPos() const
    {
        return myPos;
    }

    /**
     * Gets the value of Type of this WidgetInfo
     */
    std::string getType() const
    {
        return myType;
    }

    /**
     * Gets the value of Name of this WidgetInfo
     */
    std::string getName() const
    {
        return myName;
    }


    //----------------------------------------------------------------------
    // Setters

    /**
     * Sets the value of Pos of this WidgetInfo
     */
    void setPos( dime::Rectangle Pos )
    {
        myPos = Pos;
    }

    /**
     * Sets the value of Type of this WidgetInfo
     */
    void setType( std::string type )
    {
        myType = type;
    }

    /**
     * Sets the value of Name of this WidgetInfo
     */
    void setName( std::string name )
    {
        myName = name;
    }


    //----------------------------------------------------------------------
    // Other public methods
    // NOTE: Group related public methods together and crate a separator comment like above for them.
    
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


}; // End of WidgetInfo

} // End of dime namespace

#endif
