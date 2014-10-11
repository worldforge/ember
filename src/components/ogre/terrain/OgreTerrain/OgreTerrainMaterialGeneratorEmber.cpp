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
#include "EmberTerrain.h"
#include "../techniques/Shader.h"
#include "../../TerrainPageDataProvider.h"
#include "framework/LoggingInstance.h"

using namespace Ogre;

namespace Ember
{
namespace OgreView
{
namespace Terrain
{
const std::string EmberTerrainProfile::ERROR_MATERIAL = "/common/primitives/texture/error";

OgreTerrainMaterialGeneratorEmber::OgreTerrainMaterialGeneratorEmber()
{
}


EmberTerrainProfile::EmberTerrainProfile(IPageDataProvider& dataProvider, TerrainMaterialGenerator* parent) :
		Ogre::TerrainMaterialGenerator::Profile(parent, "Ember", "Ember specific profile"), mDataProvider(dataProvider)
{
	//Check that our error material exists
	mErrorMaterialTemplate = Ogre::MaterialManager::getSingleton().getByName(ERROR_MATERIAL);
	assert(!mErrorMaterialTemplate.isNull());
}

EmberTerrainProfile::~EmberTerrainProfile()
{
}

void EmberTerrainProfile::requestOptions(Ogre::Terrain* terrain)
{
	terrain->_setMorphRequired(false);
	terrain->_setNormalMapRequired(true);
	terrain->_setLightMapRequired(false, false);

	EmberTerrain* emberTerrain = static_cast<EmberTerrain*>(terrain);

	// Allocate in main thread so no race conditions
	std::unique_ptr<IPageData> pageData(mDataProvider.getPageData(emberTerrain->getIndex()));
	if (!pageData || pageData->getCompositeMapMaterial().isNull() || pageData->getCompositeMapMaterial()->getNumTechniques() == 0) {
		terrain->_setCompositeMapRequired(false);
	} else {
		terrain->_setCompositeMapRequired(true);
	}
}

Ogre::MaterialPtr EmberTerrainProfile::generate(const Ogre::Terrain* terrain)
{
	const EmberTerrain* emberTerrain = static_cast<const EmberTerrain*>(terrain);

	const auto& index = emberTerrain->getIndex();

	IPageData* pageData = mDataProvider.getPageData(index);

	if (!pageData) {
		S_LOG_WARNING("Could not find corresponding page data for OgreTerrain at " << "[" << index.first << "|" << index.second << "]");
		return getOrCreateMaterialClone(mErrorMaterialTemplate, terrain->getMaterialName());
	}

	S_LOG_INFO("Loading material for terrain page: " << "[" << index.first << "|" << index.second << "]");

	Ogre::MaterialPtr mat = pageData->getMaterial();

	if (mat.isNull()) {
		S_LOG_WARNING("Terrain material was not found.");
		return getOrCreateMaterialClone(mErrorMaterialTemplate, terrain->getMaterialName());
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

Ogre::MaterialPtr EmberTerrainProfile::generateForCompositeMap(const Ogre::Terrain* terrain)
{
	const EmberTerrain* emberTerrain = static_cast<const EmberTerrain*>(terrain);

	const auto& index = emberTerrain->getIndex();

	S_LOG_VERBOSE("Loading composite map material for terrain page: " << "[" << index.first << "|" << index.second << "]");

	std::unique_ptr<IPageData> pageData(mDataProvider.getPageData(index));
	if (!pageData) {
		S_LOG_WARNING("Could not find corresponding page data for OgreTerrain at " << "[" << index.first << "|" << index.second << "]");
		return getOrCreateMaterialClone(mErrorMaterialTemplate, terrain->getMaterialName() + "_comp");
	}

	Ogre::MaterialPtr mat = pageData->getCompositeMapMaterial();

	if (!mat.isNull()) {
		return mat;
	} else {
		S_LOG_WARNING("Composite map material was not found. This might happen if the page is currently being unloaded.");
		return getOrCreateMaterialClone(mErrorMaterialTemplate, terrain->getMaterialName() + "_comp");
	}
}

Ogre::MaterialPtr EmberTerrainProfile::getOrCreateMaterialClone(Ogre::MaterialPtr templateMaterial, const std::string& suffix)
{
	std::string name = templateMaterial->getName() + suffix;

	Ogre::MaterialPtr mat = Ogre::MaterialManager::getSingleton().getByName(name);
	if (!mat.isNull()) {
		return mat;
	}

	return templateMaterial->clone(name);
}


} /* Terrain */
} /* OgreView */
} /* Ember */
