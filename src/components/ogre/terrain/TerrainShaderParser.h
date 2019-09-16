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

#ifndef TERRAINSHADERPARSER_H_
#define TERRAINSHADERPARSER_H_

namespace Atlas {
namespace Message {
class Element;
}
}


namespace Ember {
namespace OgreView {

namespace Terrain {

class TerrainHandler;

/**

 @brief Parses terrain surface information from Atlas data and updates the terrain generator accordingly.

 @author Erik Ogenvik <erik@ogenvik.org>

 */
class TerrainShaderParser {
public:

	/**
	 * @brief Ctor.
	 * @param terrainHandler The terrain handler.
	 */
	explicit TerrainShaderParser(TerrainHandler& terrainHandler);

	virtual ~TerrainShaderParser();

	/**
	 * @brief Parses surface data and creates appropriate Mercator::Shader instances.
	 * @param surfaces The element containing the terrain data.
	 */
	void createShaders(const Atlas::Message::Element& terrain);

	/**
	 * @brief A fall back method which will create default shaders. This is used only if no valid surface information could be found (for example if a very old version of the server is used).
	 */
	void createDefaultShaders();

private:

	/**
	 * @brief The terrain generator instance used in the system.
	 */
	TerrainHandler& mTerrainHandler;
};

}

}

}

#endif /* TERRAINSHADERPARSER_H_ */
