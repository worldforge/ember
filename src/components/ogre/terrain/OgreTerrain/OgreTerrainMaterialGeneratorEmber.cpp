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
	terrain->_setCompositeMapRequired(true);
}

Ogre::MaterialPtr OgreTerrainMaterialGeneratorEmber::generate(const Ogre::Terrain* terrain)
{
	// calculate page index from position
	long indexX = (terrain->getPosition().x - mOriginX) / terrain->getWorldSize();
	long indexY = -(terrain->getPosition().z - mOriginZ) / terrain->getWorldSize();

	S_LOG_INFO("Loading material for terrain page: " << "[" << indexX << "|" << indexY << "]");

	Ogre::MaterialPtr mat = mDataProvider.getPageData(IPageDataProvider::OgreIndex(indexX, indexY))->getMaterial();

	assert(!mat.isNull() && "Returned terrain material must be non-empty");
	if (mat.isNull()) {
		S_LOG_WARNING("Terrain material was not found!");
		return Ogre::MaterialManager::getSingleton().getByName("/global/primitives/axes");
	}

	Ogre::AliasTextureNamePairList aliases;
	aliases["EmberTerrain/normalMap"] = terrain->getTerrainNormalMap()->getName();
	aliases["EmberTerrain/compositeMap"] = terrain->getCompositeMap()->getName();

	bool success = mat->applyTextureAliases(aliases);
	if (!success) {
		S_LOG_WARNING("Could not apply alias for normal map and/or composite map for terrain material \"" << mat->getName() << "\"");
	}

	return mat;
}

Ogre::MaterialPtr OgreTerrainMaterialGeneratorEmber::generateForCompositeMap(const Ogre::Terrain* terrain)
{
	long indexX = (terrain->getPosition().x - mOriginX) / terrain->getWorldSize();
	long indexY = -(terrain->getPosition().z - mOriginZ) / terrain->getWorldSize();

	S_LOG_INFO("Loading composite map material for terrain page: " << "[" << indexX << "|" << indexY << "]");

	Ogre::MaterialPtr mat = mDataProvider.getPageData(IPageDataProvider::OgreIndex(indexX, indexY))->getCompositeMapMaterial();
	if (!mat.isNull()) {
		return mat;
	} else {
		S_LOG_WARNING("Composite map material was not found!");
		return Ogre::MaterialManager::getSingleton().getByName("/global/primitives/axes");
	}
}
void OgreTerrainMaterialGeneratorEmber::updateCompositeMap(const Ogre::Terrain* terrain, const Ogre::Rect& rect)
{
	size_t compSize = terrain->getCompositeMap()->getWidth();
	// The rectangle that is to be updated - Using the full texture size here
	Ogre::Rect imgRect;
	imgRect.top = 0;
	imgRect.left = 0;
	imgRect.right = compSize;
	imgRect.bottom = compSize;

	_renderCompositeMap(compSize, imgRect, terrain->getCompositeMapMaterial(), terrain->getCompositeMap());
}

void OgreTerrainMaterialGeneratorEmber::setOrigin(Ogre::Real x, Ogre::Real z)
{
	mOriginX = x;
	mOriginZ = z;
}
} /* Terrain */
} /* OgreView */
} /* Ember */
