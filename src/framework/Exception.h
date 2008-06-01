/*
 *  File:       Exception.h
 *  Summary:    The class which defines the base class for exceptions.
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


#ifndef EXCEPTION_H
#define EXCEPTION_H

// Included headers from the current project

// Included custom library headers

// Included system headers
#include <string>
#include <stdarg.h>
#include <stdio.h>

namespace Ember 
{
class Exception;
/**
 *	The default way to throw exceptions is to use one of the following macros.
 */

// #define THROW(message) throw Exception(message, __FILE__, __LINE__);
// #define THROW1(message, p1) throw Exception(message, __FILE__, __LINE__, p1);
// #define THROW2(message, p1, p2) throw Exception(message, __FILE__, __LINE__, p1, p2);
// #define THROW3(message, p1, p2, p3) throw Exception(message, __FILE__, __LINE__, p1, p2, p3);

/**
 * The base class for all exceptions that are thrown within Ember.
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

const int EXCEPTION_TEXT_SIZE = 1024;

class Exception
{

	//======================================================================
	// Private Variables
	//======================================================================
private:
    std::string myError;
	std::string myFile;
	int	myLine;

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
		myLine = -1;
		myFile = "";
    }

    /**
    * Creates a new generic Exception using the specified error string, file and line
	* occurence.
    */
    Exception(const std::string& error, const std::string & file, int line, ...)
		: myFile(file), myLine(line)
    {
		char buffer[EXCEPTION_TEXT_SIZE];
		va_list va;
		va_start (va, line);
		vsprintf(buffer, error.c_str(), va);
		myError = buffer;
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
    const std::string& getError() const
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
