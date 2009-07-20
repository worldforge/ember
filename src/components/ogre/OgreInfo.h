//
// C++ Interface: OgreInfo
//
// Description:
//
//
// Author: Erik Hjortsberg <erik.hjortsberg@gmail.com>, (C) 2006
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.//
//
#ifndef EMBEROGREOGREINFO_H
#define EMBEROGREOGREINFO_H

#include <string>

namespace EmberOgre {

/**

@brief Provides methods for getting some basic information about the Ogre environment, as well as utility method for working with Ogre.

@author Erik Hjortsberg <erik.hjortsberg@gmail.com>
*/
class OgreInfo
{
public:

    /**
     * @brief Checks if the rendering is indirect, for example when using Mesa drivers on Linux. This will result in _very_ bad performance, and is usually caused by the user not having vendor drivers installed.
     * @return True if rendering is indirect.
     */
    static bool isIndirect();

    /**
     * @brief Creates a "unique" resource name.
     * The way this is done is by appending the value of an internal counter on the supplied name.
     * This of course doesn't guarantee that the name is unique, but with the counter being a long long int it should be fairly safe.
     * @param resourceName The name of the resource.
     * @return The supplied name with the value of an internal counter suffixed.
     */
    static std::string createUniqueResourceName(const std::string& resourceName);

private:

	/**
	 * @brief Internal counter for generating "unique" resource names.
	 */
	static long long int sResourceCounter;

};

}

#endif
