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

#include "../EmberOgrePrerequisites.h"
#include <Mercator/Shader.h>
#include <Mercator/Terrain.h>
#include <Mercator/Segment.h>
#include <Mercator/AreaShader.h>



namespace EmberOgre {
namespace Terrain {
class TerrainLayerDefinition;
class TerrainShader{
public:

	TerrainShader(Mercator::Terrain* terrain, int terrainIndex, const TerrainLayerDefinition* layerDef, const Mercator::Shader* shader);
	virtual ~TerrainShader();

	const Mercator::Shader* getShader() const;

	/**
	 * @brief Adds a texture unit with a splatting alpha texture to the supplied pass.
	 * Use this when you're using many texture units in the same pass
	 */
	void addTextureUnitsToPass(Ogre::Pass* pass, const std::string& splatTextureName);

	/**
	 * @brief Adds a pass with a splatting alpha texture to the supplied technique.
	 * Use this when you're using many passes. This is more expensive than
	 * addTextureUnitsToPass(...) but works on card with a low number of
	 * TextureUnits.
	 */
	Ogre::Pass* addPassToTechnique(Ogre::Technique* technique, const std::string& splatTextureName);

	int getTerrainIndex() const;

	void addMaterialToTechnique(Ogre::Technique*  technique, const std::string& splatTextureName);
	void addSplatToTechnique(Ogre::Technique*  technique, const std::string& splatTextureName);


	/**
	 *    returns the Surface for the given segment
	 * @param segment
	 * @return a surface, or null if no could be found
	 */
	Mercator::Surface* getSurfaceForSegment(Mercator::Segment* segment) const;


	const TerrainLayerDefinition* getLayerDefinition() const;

protected:
	const TerrainLayerDefinition* mLayerDef;
	const Mercator::Shader* mShader;
	Mercator::Terrain* mTerrain;
	int mTerrainIndex;

	Ogre::ushort getMaxTextureUnits() const;
};

inline int TerrainShader::getTerrainIndex() const
{
	return mTerrainIndex;
}


}
}

#endif // TERRAINSHADER_H
