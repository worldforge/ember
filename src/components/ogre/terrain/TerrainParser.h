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

#ifndef TERRAINPARSER_H_
#define TERRAINPARSER_H_

#include "Types.h"

namespace Atlas
{
namespace Message
{
class Element;
}
}

namespace WFMath
{
template<int> class Point;
}

namespace Ember
{
namespace OgreView
{

namespace Terrain
{

class TerrainManager;
struct TerrainDefPoint;

/**

 @brief Responsible for parsing terrain information from the Atlas data.

 @author Erik Ogenvik <erik@worldforge.org>

 */
class TerrainParser
{
public:
	TerrainParser();

	/**
	 * @brief Extracts terrain updates from the element and updates the terrain.
	 * @param terrain The element containing the terrain data.
	 */
	TerrainDefPointStore parseTerrain(const Atlas::Message::Element& terrain, const WFMath::Point<3>& offset) const;

};

}

}

}

#endif /* TERRAINPARSER_H_ */
