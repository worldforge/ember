/*
   Copyright (C) 2013 Samuel Kogler

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

#include "OgreTerrainMaterialGeneratorEmber.h"
#include "../../TerrainPageDataProvider.h"
#include "framework/LoggingInstance.h"

#include <OgreTerrain.h>

namespace Ember
{
namespace OgreView
{
namespace Terrain
{
OgreTerrainMaterialGeneratorEmber::OgreTerrainMaterialGeneratorEmber(IPageDataProvider& dataProvider, Ogre::Real originX, Ogre::Real originZ)
	: mDataProvider(dataProvider), mOriginX(originX), mOriginZ(originZ)
{
}

OgreTerrainMaterialGeneratorEmber::~OgreTerrainMaterialGeneratorEmber()
{
}

void OgreTerrainMaterialGeneratorEmber::requestOptions(Ogre::Terrain* terrain)
{
	// TODO SK: this stuff is useful but turned off for now
	terrain->_setMorphRequired(false);
	terrain->_setNormalMapRequired(true);
	terrain->_setLightMapRequired(false, false);
	terrain->_setCompositeMapRequired(false);
}

Ogre::MaterialPtr OgreTerrainMaterialGeneratorEmber::generate(const Ogre::Terrain* terrain)
{
	// calculate page index from position
	long indexX = (terrain->getPosition().x - mOriginX) / terrain->getSize();
	long indexY = -(terrain->getPosition().z - mOriginZ) / terrain->getSize();

	S_LOG_INFO("Loading material for terrain page: " << "[" << indexX << "|" << indexY << "]");

	Ogre::MaterialPtr mat = mDataProvider.getPageData(IPageDataProvider::OgreIndex(indexX, indexY))->getMaterial();

	assert(!mat.isNull() && "Returned terrain material must be non-empty");

	Ogre::AliasTextureNamePairList aliases;
	aliases["EmberTerrain/normalMap"] = terrain->getTerrainNormalMap()->getName();

	bool success = mat->applyTextureAliases(aliases);
	if (!success) {
		S_LOG_WARNING("Could not alias normal map for terrain material");
	}

	return mat;
}

/** Generate a material for the given composite map of the terrain using the active profile.
 */
Ogre::MaterialPtr OgreTerrainMaterialGeneratorEmber::generateForCompositeMap(const Ogre::Terrain* terrain)
{
	// TODO SK: return material
}
} /* Terrain */
} /* OgreView */
} /* Ember */
