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
    Copyright (C) 2008  Erik Hjortsberg

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


#ifndef EMBER_EXCEPTION_H
#define EMBER_EXCEPTION_H

#include <string>
#include <exception>

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


const int EXCEPTION_TEXT_SIZE = 1024;

/**
 * The base class for all exceptions that are thrown within Ember.
 *
 * @author Nikal
 * @author Xmp (Martin Pollard)
 * @author Erik Hjortsberg <erik.hjortsberg@iteam.se>
 *
 */
class Exception : public std::exception
{

public:

	/**
	* Creates a new generic Exception using default values.
	*/
	Exception();

	/**
	* Creates a new generic Exception using the specified error description.
	* @param error A descriptive string of the error.
	*/
	Exception(const std::string& error);

	/**
	* Creates a new generic Exception using the specified error string, file and line
	* occurence.
	* @param error A descriptive string of the error.
	* @param file The file in which the error occured.
	* @param line The line on which the error occurred.
	*/
    Exception(const std::string& error, const std::string & file, int line, ...);

	virtual ~Exception() throw();
 
	/**
	*    @brief Gets a descriptive string of the exception.
	* @see what()
	* @return A description of the error.
	*/
	const std::string& getError() const;
     
	/**
	* @brief Sets a descriptive string for this error.
	* @param error The description of the error.
	*/
	void setError(const std::string& error);

	/**
	*   @brief Gets a descriptive string of the exception.
	* @see getError()
	* @return A description of the error.
	*/
	virtual const char* what() const throw();
	
	private:

	/**
	A description of the error.
	*/
	std::string mErrorDescription;
	
	/**
	The file in which the error occurred.
	*/
	std::string mFile;
	
	/**
	The line on which the error occurred.
	*/
	int	mLine;


};


}
#endif
