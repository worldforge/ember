/*
 Copyright (C) 2009 Erik Hjortsberg

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

#ifndef TERRAINDEFPOINT_H_
#define TERRAINDEFPOINT_H_

#include <wfmath/point.h>
#include "components/ogre/Types.h"

namespace EmberOgre
{

namespace Terrain
{

/**
@brief Defines the height of a special "base point" in the terrain.
These base points are then user by Mercator::Terrain for generating the actual terrain.
*/
class TerrainDefPoint
{
	public:
	/**
	*       Ctor.
	* @param x The position of the point, on the x axis, in world units.
	* @param y The position of the point, on the y axis, in world units.
	* @param terrainHeight The height of the point, in world units.
	*/
	TerrainDefPoint(float x, float y, float terrainHeight) : mPosition(x,y), mHeight(terrainHeight) {}

	/**
	 * @brief Gets the position of the definition point, in world units.
	 * @return The position of the point.
	 */
	const TerrainPosition& getPosition() const;

	/**
	 * @brief Gets the height of the definition point, in world units.
	 * @return The height of the point.
	 */
	float getHeight() const;

	private:

	/**
	The position of the point, in world units.
	*/
	TerrainPosition mPosition;

	/**
	The height of the point, in world units.
	*/
	float mHeight;
};

}
}

#endif /* TERRAINDEFPOINT_H_ */
