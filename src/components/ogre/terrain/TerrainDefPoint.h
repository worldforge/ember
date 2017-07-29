/*
 Copyright (C) 2009 Erik Ogenvik

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

#ifndef TERRAINDEFPOINT_H_
#define TERRAINDEFPOINT_H_

#include <wfmath/point.h>
#include "domain/Types.h"

namespace Ember
{
namespace OgreView
{

namespace Terrain
{

/**
@brief Defines the height of a special "base point" in the terrain.
These base points are then user by Mercator::Terrain for generating the actual terrain.
*/
struct TerrainDefPoint
{
	public:

	/**
	The position of the point, in world units.
	*/
	TerrainPosition position;

	/**
	The height of the point, in world units.
	*/
	float height;

	float roughness;

	float falloff;
};

}
}

}

#endif /* TERRAINDEFPOINT_H_ */
