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

#ifndef COMPONENT_FACTORY_H
#define COMPONENT_FACTORY_H

#include "Component.h"

#include <string>

namespace dime {
namespace components {

/**
 * Abstract class that defines the interface for all factory classes that are
 * used to cerate different component types.
 *
 * @see dime::components::Component
 *
 * @author Hans Häggström
 */
class ComponentFactory

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
     * Name for the types of components that this component factory produces.
     * Should only contain alphanumerical characters and underscores.
     */
    std::String myName;

    /**
     * Description of the types of components that this component factory produces.
     */
    std::String myDescription;

    // TODO: Reference to UI data?
    // TODO: List of connection slot descriptions
    // TODO: Icon


    //======================================================================
    // Public Methods
    //======================================================================
    public:

    //----------------------------------------------------------------------
    // Constructors

    /**
     * Cretaes a new ComponentFactory using default values.
     */
    ComponentFactory()
    {
    }


    /**
     * Copy constructor.
     */
    ComponentFactory( const ComponentFactory &source )
    {
        // Use assignment operator to do the copy
        // NOTE: If you need to do custom initialization in the constructor this may not be enough.
        *this = source;
    }


    /**
     * Assignment operator.
     */
    ComponentFactory &operator= ( const ComponentFactory &source )
    {
        // TODO: Copy fields from source class to this class here.

        // Return result
        return &this;
    }


    //----------------------------------------------------------------------
    // Destructor

    /**
     * Deletes a ComponentFactory instance.
     */
    virtual ~ComponentFactory()
    {
        // TODO: Free any allocated resources here.
    }


    //----------------------------------------------------------------------
    // Getters

    /**
     * Returns the name of this ComponentFactory.
     */
    virtual std::String getName() const
    {
        return myName;
    }


    //----------------------------------------------------------------------
    // Setters

    /**
     * Sets the name of this ComponentFactory.
     *
     * @param name The new name of the ComponentFactory.
     */
    virtual void setName( std::String name )
    {
        myName = name;
    }


    //----------------------------------------------------------------------
    // Other public methods

    /**
     * Creates a new Component. <p>
     *
     * This method should be overridden in implementing classes.
     *
     * TODO: How does initialization of ID, name, parent group, etc happen?  From here?
     */
    virtual Component *createComponent() = 0;


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


