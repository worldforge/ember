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

#ifndef TERRAINPARSER_H_
#define TERRAINPARSER_H_

namespace Atlas
{
namespace Message
{
class Element;
}
}

namespace EmberOgre
{

namespace Terrain
{

class TerrainManager;

/**

 @brief Responsible for parsing terrain information from the Atlas data.

 @author Erik Hjortsberg <erik@worldforge.org>

 */
class TerrainParser
{
public:
	TerrainParser(Terrain::TerrainManager& TerrainManager);

	/**
	 * @brief Extracts terrain updates from the element and updates the terrain.
	 * @param terrain The element containing the terrain data.
	 */
	void updateTerrain(const Atlas::Message::Element& terrain);

	/**
	 * @brief Parses surface data and creates appropriate Mercator::Shader instances.
	 * @param surfaces The element containing the surface data.
	 */
	void createShaders(const Atlas::Message::Element& surfaces);

	/**
	 * @brief A fall back method which will create default shaders. This is used only if no valid surface information could be found (for example if a very old version of the server is used).
	 */
	void createDefaultShaders();

private:

	/**
	 * @brief The terrain manager instance used in the system.
	 */
	Terrain::TerrainManager& mTerrainManager;
};

}

}

#endif /* TERRAINPARSER_H_ */
