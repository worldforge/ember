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
// - Leave the different separator comments of the template header
//   (such as Protected Methods, Public Constants, etc) in place even if you don't
//   use them initially.  This way there is a clear place where to put new methods,
//   variables, and constants of different types, and the code stays clearer.
//
//
//   TODO: What standard should be used for todo's, notes, and such?
//   One possibility would be TODO( user, date, importance ): <message>
//   Here different arguments can be left out, and if there's no arguments
//   the parenthesis can also be left out.  Examples:
//
//     TODO: Fix the water boiler!
//     TODO( zzorn ): Shouldn't we put tea in this container?
//     TODO( brenda, 2002-01-20 ): Added a temporary kludge to start the coffee machine.
//     TODO( zzorn, 2002-01-20, optional ): It would be nice to support coffee at some time too.
//     TODO( brenda, required ): We absolutely need to add support for coffee here!
//
//   Are user, date, and importance parameters enough?  Are there other important things,
//   that might be useful to be able to automatically process in some way?
//
//   "NOTE:" -is used similarily to TODO's, but is used to bring some detail
//   to programmers attention that may be unintuitive or easy to miss otherwise,
//   or some other meta-comment about the program (?).
//
//   Some other syntax is used in some other parts of worldforge
//   (something to do with % -characters, IIRC).   We could examine if this is suitable for us.
//
//   "Fixme" is another todo type tag used sometimes.
//
//   Whatever syntax we settle for, it would be nice to have scripts that can
//   extract the todo notes from the code, order them according to importance,
//   or some other parameter, and include them on a web page, with links
//   back to the original code (in bonsai or some other web cvs system?
//   Or at least file and line number otherwise).
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

// Include headers from the current project here

// Include custom library headers here

// Include system headers here

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

    // NOTE: Class variables are prefixed with "my", static variables are
    //       prefixed with "the".

    /**
     * NOTE: Variables can have a comment too.  You can use it to describe the
     *       purpose of the varible, or wether certain invariants
     *       should be enforced for it (parentConatiner* should newer be null, etc).
     */
    std::string  myExampleVariable;

    /**
     * Stores an unique name of this gizmo.
     */
    std::string myName;

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

        // Return this object with new value
        return &this;
    }


    //----------------------------------------------------------------------
    // Destructor

    /**
     * Deletes a NameOfClass instance.
     */
    virtual ~NameOfClass()
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

    // TODO: If you don't want to provide a copy constructor or some other
    //       of the standard constructors/operators, then delete the one in
    //       the public section at the start of the header.  Otherwise delete
    //       the one in this section.

    /**
     * Constructor not provided.
     */
    NameOfClass()
    {
    }


    /**
     * Copy constructor not provided.
     */
    NameOfClass( const NameOfClass &source )
    {
    }


    /**
     * Assignment operator not provided.
     */
    NameOfClass &operator= ( const NameOfClass &source )
    {
        return &this;
    }


}; // End of class

} // End of subsystem namespace
} // End of application namespace

#endif


