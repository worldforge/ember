//
// C++ Interface: ShaderManager
//
// Description: 
//
//
// Author: Alexey Torkhov <atorkhov@gmail.com>, (C) 2009
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
#ifndef EMBEROGRE_SHADERMANAGER_H
#define EMBEROGRE_SHADERMANAGER_H

namespace EmberOgre {

/**
 * @brief Utility class for setup shaders
 *
 * @author Alexey Torkhov <atorkhov@gmail.com>
 */
class ShaderManager {
public:
	/**
	 * Constructor.
	 */
	ShaderManager();

	/**
	 * Destructor.
	 */
	~ShaderManager();

	/**
	 * Shaders initialization
	 */
	void init();

	/**
	 * Checks whether material is supported in current scheme
	 */
	void checkMaterial(std::string materialName);
}

}

#endif
