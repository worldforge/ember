/*
    Copyright (C) 2004  Erik Hjortsberg

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

#ifndef TERRAINSHADER_H
#define TERRAINSHADER_H


namespace Mercator {
class Terrain;
class Shader;
class Surface;
class Segment;
}

namespace EmberOgre {
namespace Terrain {

class TerrainLayerDefinition;

class TerrainShader{
public:

	TerrainShader(Mercator::Terrain& terrain, int terrainIndex, const TerrainLayerDefinition& layerDef, const Mercator::Shader* shader);
	virtual ~TerrainShader();

	const Mercator::Shader& getShader() const;

	int getTerrainIndex() const;

	/**
	 * @brief Returns the Surface for the given segment.
	 * @param segment
	 * @return a surface, or null if no could be found
	 */
	Mercator::Surface* getSurfaceForSegment(Mercator::Segment& segment) const;

	const TerrainLayerDefinition& getLayerDefinition() const;

protected:
	const TerrainLayerDefinition& mLayerDef;
	const Mercator::Shader* mShader;
	Mercator::Terrain& mTerrain;
	int mTerrainIndex;
};

inline int TerrainShader::getTerrainIndex() const
{
	return mTerrainIndex;
}


}
}

#endif // TERRAINSHADER_H
