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

// Included custom library headers
// (1) Include the signal system in headers file.
#include <sigc++/signal_system.h>

// Included system headers

namespace Dime {

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
 *
 * @see Dime::ParaGuiButton
 *
 */
class Button : Dime::Widget

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
    //======================================================================
    private:

    /**
     * Stores the 
     */

    /**
     * This variable is used to keep track of the next free ID number for a new gizmo.
     */
    static int theNextId;


    //======================================================================
    // Public Methods
    //======================================================================
    public:

    //----------------------------------------------------------------------
    // Constructors

    /**
     * Cretaes a new Button using default values.
     */
    Button()
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
        return &this;
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

    // Example of a getter method:

    /**
     * Returns the name of this gizmo.
     * The name is guaranteed to be unique among all gizmos.
     */
    virtual std::string getName() const
    {
        return myName;
        // NOTE: If we just read or set a local variable, the
        //       getter/setter can be inline.  If more complex logic
        //       is needed, it may be better to have the implementation in
        //       the cpp file.
    }


    //----------------------------------------------------------------------
    // Setters

    // Example of a setter method:

    /**
     * Sets the name of this gizmo.
     * If there already is a gizmo with this name, a number will be
     * appended to the name, and the naming will be attempted again.
     *
     * @param name The new name of the gizmo.
     */
    virtual void setName( std::string name );


    //----------------------------------------------------------------------
    // Other public methods
    // NOTE: Group related public methods together and crate a separator comment like above for them.

    /**
     * NOTE: This is an example method declaration.
     *
     * Creates a new string that repeats a given string some number of times.
     * There's no extra space added between the strings in the produced string.
     * Null characters are handled correctly too.
     *
     * @param message The message string to repeat.
     * @param repeatCount How many times to repeot the message in the produced String.
     *                    Must be >= 0, an less than 2000.  If it is 0 then the produced string is empty.
     *
     * @return A string consisting of the specified number of the input strings,
     *         or null if the character copier suffered a fatal error.
     *
     * @see OtherSubsystem::SomeOtherRelatedClass
     * NOTE: Add other related classes here, doxygen will create links to them.
     *
     * @author Anonymous Coward
     */
    virtual std::string createRepeatingString( std::string message, int repeatCount ) const;


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


