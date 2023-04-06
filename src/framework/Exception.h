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
    Copyright (C) 2008  Erik Ogenvik

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software Foundation,
    Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
*/


#ifndef EMBER_EXCEPTION_H
#define EMBER_EXCEPTION_H

#include <string>
#include <stdexcept>

namespace Ember {

/**
 * The base class for all exceptions that are thrown within Ember.
 *
 * @author Nikal
 * @author Xmp (Martin Pollard)
 * @author Erik Ogenvik <erik@ogenvik.org>
 *
 */
class Exception : public std::runtime_error {

public:

	/**
	* Creates a new generic Exception using default values.
	*/
	Exception() : std::runtime_error("Unknown Exception") {}

	/**
	* Creates a new generic Exception using the specified error description.
	* @param error A descriptive string of the error.
	*/
	explicit Exception(const std::string& error)
			: std::runtime_error(error) {}

};


}
#endif
