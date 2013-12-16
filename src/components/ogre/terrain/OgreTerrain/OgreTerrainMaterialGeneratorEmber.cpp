/*
   Copyright (C) 2013 Samuel Kogler
   Some parts Copyright (c) 2000-2013 Torus Knot Software Ltd

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
#include "EmberTerrain.h"
#include "../techniques/Shader.h"
#include "../../TerrainPageDataProvider.h"
#include "framework/LoggingInstance.h"
#include <OgreTerrain.h>

using namespace Ogre;

namespace Ember
{
namespace OgreView
{
namespace Terrain
{
const std::string OgreTerrainMaterialGeneratorEmber::ERROR_MATERIAL = "/global/primitives/error";

OgreTerrainMaterialGeneratorEmber::OgreTerrainMaterialGeneratorEmber(IPageDataProvider& dataProvider, Ogre::Real originX, Ogre::Real originZ)
	: mDataProvider(dataProvider), mOriginX(originX), mOriginZ(originZ)
{
}

OgreTerrainMaterialGeneratorEmber::~OgreTerrainMaterialGeneratorEmber()
{
}

void OgreTerrainMaterialGeneratorEmber::requestOptions(Ogre::Terrain* terrain)
{
	terrain->_setMorphRequired(false);
	terrain->_setNormalMapRequired(true);
	terrain->_setLightMapRequired(false, false);

	EmberTerrain* emberTerrain = static_cast<EmberTerrain*>(terrain);

	// Allocate in main thread so no race conditions
	IPageData* pageData = mDataProvider.getPageData(emberTerrain->getIndex());
	if (pageData->getCompositeMapMaterial().isNull() || pageData->getCompositeMapMaterial()->getNumTechniques() == 0) {
		terrain->_setCompositeMapRequired(false);
	} else {
		terrain->_setCompositeMapRequired(true);
	}

}

Ogre::MaterialPtr OgreTerrainMaterialGeneratorEmber::generate(const Ogre::Terrain* terrain)
{
	const EmberTerrain* emberTerrain = static_cast<const EmberTerrain*>(terrain);

	const auto index = emberTerrain->getIndex();

	IPageData* pageData = mDataProvider.getPageData(index);

	S_LOG_INFO("Loading material for terrain page: " << "[" << index.first << "|" << index.second << "]");

	Ogre::MaterialPtr mat = pageData->getMaterial();

	if (mat.isNull()) {
		S_LOG_WARNING("Terrain material was not found!");
		return Ogre::MaterialManager::getSingleton().getByName(ERROR_MATERIAL);
	}

	Ogre::AliasTextureNamePairList aliases;
	aliases[Techniques::Shader::NORMAL_TEXTURE_ALIAS] = terrain->getTerrainNormalMap()->getName();
	auto compositeMap = terrain->getCompositeMap();
	if (!compositeMap.isNull()) {
		aliases[Techniques::Shader::COMPOSITE_MAP_ALIAS] = compositeMap->getName();
	}

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
		S_LOG_WARNING("Composite map material was not found! This might happen if the page is currently being unloaded.");
		return Ogre::MaterialManager::getSingleton().getByName(ERROR_MATERIAL);
	}
}
void OgreTerrainMaterialGeneratorEmber::updateCompositeMap(const Ogre::Terrain* terrain, const Ogre::Rect& rect)
{

	//This code is copied Ogre::TerrainMaterialGenerator::Profile::updateCompositeMap and thus licensed under the MIT license used by Ogre

	// convert point-space rect into image space
	size_t compSize = terrain->getCompositeMap()->getWidth();
	Rect imgRect;
	Vector3 inVec, outVec;
	inVec.x = rect.left;
	inVec.y = rect.bottom - 1; // this is 'top' in image space, also make inclusive
	terrain->convertPosition(Ogre::Terrain::POINT_SPACE, inVec, Ogre::Terrain::TERRAIN_SPACE, outVec);
	imgRect.left = outVec.x * compSize;
	imgRect.top = (1.0f - outVec.y) * compSize;
	inVec.x = rect.right - 1;
	inVec.y = rect.top; // this is 'bottom' in image space
	terrain->convertPosition(Ogre::Terrain::POINT_SPACE, inVec, Ogre::Terrain::TERRAIN_SPACE, outVec);
	imgRect.right = outVec.x * (Real)compSize + 1;
	imgRect.bottom = (1.0 - outVec.y) * compSize + 1;

	imgRect.left = std::max(0L, imgRect.left);
	imgRect.top = std::max(0L, imgRect.top);
	imgRect.right = std::min((long)compSize, imgRect.right);
	imgRect.bottom = std::min((long)compSize, imgRect.bottom);


	_renderCompositeMap(
		compSize, imgRect,
		terrain->getCompositeMapMaterial(),
		terrain->getCompositeMap());

}

void OgreTerrainMaterialGeneratorEmber::setOrigin(Ogre::Real x, Ogre::Real z)
{
	mOriginX = x;
	mOriginZ = z;
}
} /* Terrain */
} /* OgreView */
} /* Ember */
