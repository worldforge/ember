/*
    Copyright (C) 2002  Hans Häggström

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

#ifndef COMPONENT_H
#define COMPONENT_H

#include <string>

#include "ComponentFactory.h"

namespace dime {
namespace components {

/**
 *
 */
class Component

{
    //======================================================================
    // Inner Classes
    //======================================================================


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
     * Free form name of this component instance.
     */
    std::String myName;

    /**
     * Unique ID within the current component group?
     * Butwhat happens when moved?
     */
    long myId;

    /**
     * Reference to the factory that created this component instance.
     */
    ComponentFactory myComponentFactory;

    // TODO: Reference to UI
    // TODO: List of connections
    // TODO: Parent CoponentGroup


    //======================================================================
    // Public Methods
    //======================================================================
    public:

    //----------------------------------------------------------------------
    // Constructors

    /**
     * Cretaes a new Component using default values.
     */
    Component()
    {
    }


    /**
     * Copy constructor.
     */
    Component( const Component &source )
    {
        // Use assignment operator to do the copy
        // NOTE: If you need to do custom initialization in the constructor this may not be enough.
        *this = source;
    }


    /**
     * Assignment operator.
     */
    Component &operator= ( const Component &source )
    {
        // TODO: Copy fields from source class to this class here.

        // Return result
        return &this;
    }


    //----------------------------------------------------------------------
    // Destructor

    /**
     * Deletes a Component instance.
     */
    ~Component()
    {
        // TODO: Free any allocated resources here.
    }


    //----------------------------------------------------------------------
    // Getters

    /**
     * Returns the name of this component.
     */
    std::String getName() const
    {
        return myName;
    }


    //----------------------------------------------------------------------
    // Setters

    /**
     * Sets the name of this component.
     *
     * @param name The new name of the component.
     */
    void setName( std::String name )
    {
        myName = name;
    }


    //----------------------------------------------------------------------
    // Other public methods


    //======================================================================
    // Protected Methods
    //======================================================================
    protected:


    //======================================================================
    // Private Methods
    //======================================================================
    private:


}; // End of class

} // End of subsystem namespace
} // End of application namespace

#endif


