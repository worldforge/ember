// This is a template that describes a layout for header files.

// Additional guidelines:
//
// - The indent is four spaces
//
// - Opening braces on new lines
//
// - if's, while's etc always have a block with opening and closing brace,
//   even if they just consist of one line.
//
// - Classes are always Capitalized for each word,
//   methods start with lower caps for first word,
//   upper caps for first letter of subsequent words.
//
// - Constants in ALL_CAPS with underscore between words.
//
// - No public variables are allowed, getter and setter methods are used instead.
//
// - Use descriptive names for classes, methods, and variables, avoid acronyms.
//   See Ottinger's naming rules for more info:
//   http://www.objectmentor.com/publications/naming.htm
//

//  ----------------------< Start of template >---------------------------
/*
    Copyright (C) 2002  <name of author>

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

#ifndef <FILENAME>_H
#define <FILENAME>_H

// Include system headers here

// Include library headers here

// Include other headers of the current program here

namespace applicationNamespace {
namespace subsystemNamespace {

/**
 * Short sentence with a general description of the class, ending in period.
 *
 * More detailed description of the class, it's purpose, what it does,
 * how to use it and so on.
 *
 * A short piece of example code demonstarting how this class it is used,
 * and in what context, is encouraged.
 *
 * @author Anonymous Coward
 * NOTE: You can also specify the author for individual methods
 * if different persons have created them.
 * It is also possible to have multiple @author tags for a method.
 * Only add yourself as an @author if you have done serious work
 * on a class/method, and can help fixing bugs in it, etc.
 * If you just fixed a bug or added a short code snipplet you
 * don't need to add yourself.
 *
 * @see OtherSubsystem::AnOtherRelatedClass
 * NOTE: Add other related classes here, doxygen will create links to them.
 */
class NameOfClass

{
    //======================================================================
    // Public Constants
    //======================================================================
    public:


    //======================================================================
    // Public Methods
    //======================================================================
    public:

    //----------------------------------------------------------------------
    // Constructors

    /**
     * Cretaes a new NameOfClass using default values.
     */
    NameOfClass()
    {
    }


    /**
     * Copy constructor.
     */
    NameOfClass( const NameOfClass &source )
    {
        // Use assignment operator to do the copy
        // NOTE: If you need to do custom initialization in the constructor this may not be enough.
        *this = source;
    }


    /**
     * Assignment operator.
     */
    NameOfClass &operator= ( const NameOfClass &source )
    {
        // Copy fields from source class to this class here.
    }


    //----------------------------------------------------------------------
    // Destructor

    /**
     * Deletes a NameOfClass instance.
     */
    ~NameOfClass()
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
    std::String getName() const
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
    void setName( std::String name );


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
    virtual std::String createRepeatingString( std::String message, int repeatCount ) const;


    //======================================================================
    // Protected Methods
    //======================================================================
    protected:


    //======================================================================
    // Private Methods
    //======================================================================
    private:


    //======================================================================
    // Private Constants
    //======================================================================
    private:


    //======================================================================
    // Private Variables
    //======================================================================
    private:

    // NOTE: Class variables are prefixed with "my", static variables are
    //       prefixed with "the".

    /**
     * NOTE: Variables can have a comment too.  You can use it to describe the
     *       purpose of the varible, or wether certain invariants
     *       should be enforced for it (parentConatiner* should newer be null, etc).
     */
    std::String  myExampleVariable;

    /**
     * Stores an unique name of this gizmo.
     */
    std::String myName;

    /**
     * This variable is used to keep track of the next free ID number for a new gizmo.
     */
    static int theNextId;


    //======================================================================
    // Inner Classes
    //======================================================================


}; // End of class

} // End of subsystem namespace
} // End of application namespace

#endif


