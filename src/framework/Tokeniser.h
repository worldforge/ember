/*
 Copyright (C) 2002  Simon Goodall, Martin Pollard (Xmp)
	
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

// Rewritten for Dime by Martin Pollard (Xmp)

// Originally written for Sear by Simon Goodall, University of Southampton
// Original Copyright (C) 2001 - 2002 

#ifndef TOKENISER_H
#define TOKENISER_H

// Included headers from the current project

// Included custom library headers

// Included system headers
#include <string>

namespace dime {

/**
 * Short sentence with a general description of the class, ending in period.
 *
 * More detailed description of the class, it's purpose, what it does,
 * how to use it and so on.
 *
 * A short piece of example code demonstarting how this class it is used,
 * and in what context, is encouraged.
 *
 * @author Martin Pollard (Xmp) aka Xmp
 *
 * NOTE: You can also specify the author for individual methods
 * if different persons have created them.
 * It is also possible to have multiple @author tags for a method.
 * Only add yourself as an @author if you have done serious work
 * on a class/method, and can help fixing bugs in it, etc.
 * If you just fixed a bug or added a short code snipplet you
 * don't need to add yourself.
 *
 * @see OtherSubsystem::AnOtherRelatedClass
 *
 * NOTE: Add other related classes here, doxygen will create links to them.
 */

class Tokeniser
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
    // Protected Constants
    //======================================================================
    protected:
    std::string::size_type pos, last_pos;
    std::string token_string;
    static const std::string delimeters;
    static const std::string quotes;
    bool quoted;


    //======================================================================
    // Private Variables
    //======================================================================/
    private:


    //======================================================================
    // Public Methods
    //======================================================================
    public:

    //----------------------------------------------------------------------
    // Constructors

    /**
     * Creates a new Tokeniser using default values.
     */
     Tokeniser()
     {
     }

    /**
     * Copy constructor.
     */
    Tokeniser( const Tokeniser &source )
    {
        // Use assignment operator to do the copy
        // NOTE: If you need to do custom initialization in the constructor this may not be enough.
        *this = source;
    }


    /**
     * Assignment operator.
     */
    Tokeniser &operator= ( const Tokeniser &source )
    {
        // Copy fields from source class to this class here.

        // Return this object with new value
        return *this;
    }


    //----------------------------------------------------------------------
    // Destructor

    /**
     * Deletes a Tokeniser instance.
     */
    virtual ~Tokeniser ()
    {
        // TODO: Free any allocated resources here.
    }


    //----------------------------------------------------------------------
    // Getters

    // Example of a getter method:


    //----------------------------------------------------------------------
    // Setters


    //----------------------------------------------------------------------
    // Other public methods
    // NOTE: Group related public methods together and create a separator comment like above for them.
    void initTokens(const std::string &tokens);
    std::string Tokeniser::nextToken();
    std::string Tokeniser::remainingTokens();
    
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


}; // End of Tokeniser

} // End of dime namespace

#endif
