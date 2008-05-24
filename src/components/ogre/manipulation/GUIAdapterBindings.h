/*
	Copyright (C) 2008  Alexey Torkhov <atorkhov@gmail.com>

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

#ifndef EMBEROGRE_GUIADAPTERBINDINGS_H
#define EMBEROGRE_GUIADAPTERBINDINGS_H

#include "components/ogre/EmberOgrePrerequisites.h"

namespace EmberOgre {

/**
 * Contains bindings for GUI adapters.
 */
class GUIAdapterBindings
{
public:
	/**
	 * Constructor.
	 */
	GUIAdapterBindings();

	/**
	 * Destructor.
	 */
	virtual ~GUIAdapterBindings();

	/**
	 * Sets binded function
	 */
	void setFunc(std::string func);

protected:
	/**
	 * Binded function name
	 */
	std::string mFunc;
};

}

#endif
