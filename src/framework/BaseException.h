/*
 *  File:       Application.h
 *  Summary:    The class which initializes the GUI.
 *  Written by: nikal and xmp
 *
 *  Copyright (C) 2002 nikal, xmp. 
 *  This code is distributed under the GPL.
 *  See file COPYING for details. 
 *
 *  Change History (most recent first):    
 *
 */
/*
    Copyright (C) 2002  Nikal, Xmp

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


#ifndef BASEEXCEPTION_H
#define BASEEXCEPTION_H

// Included headers from the current project

// Included custom library headers

// Included system headers
#include <string>


namespace dime 
{
/**
 * The base class for all exceptions that are thrown within Dime.
 *
 * More detailed description of the class, it's purpose, what it does,
 * how to use it and so on.
 *
 * A short piece of example code demonstarting how this class it is used,
 * and in what context, is encouraged.
 *
 * @author Nikal
 * @author Xmp (Martin Pollard)
 *
 */
class Exception
{

	//======================================================================
	// Private Variables
	//======================================================================
private:
      std::string myError;
	//======================================================================
	// Public Methods
	//======================================================================
public:

    //----------------------------------------------------------------------
    // Constructors
    
    /**
    * Creates a new generic Exception using default values.
    */
	Exception()
		: myError("Unknown Exception")
	{
    }

    /**
    * Creates a new generic Exception using the specified error string.
    */
    Exception(const std::string& error)
		: myError(error)
    {
    }

    //----------------------------------------------------------------------
    // Destructor

    /**
    * Deletes an Exception instance.
    */
    virtual ~Exception()
    {
    }
 
    //----------------------------------------------------------------------
    // Getters

	/**
	 * Returns the error that caused the exception
	 */
    std::string getError()
    {
		return myError;
    }
     
    //----------------------------------------------------------------------
    // Setters

    void setError(const std::string& error)
    {
		myError = error;
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


    //======================================================================
    // Disabled constructors and operators
    //======================================================================
    private:
};


}
#endif
