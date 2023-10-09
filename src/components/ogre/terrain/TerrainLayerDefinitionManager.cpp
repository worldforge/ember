//
// C++ Implementation: TerrainLayerDefinitionManager
//
// Description: 
//
//
// Author: Erik Ogenvik <erik@ogenvik.org>, (C) 2007
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
// 
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software Foundation,
// Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.//
//
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "TerrainLayerDefinitionManager.h"
#include "TerrainLayerDefinition.h"
#include "XMLLayerDefinitionSerializer.h"

#include <OgreTextureManager.h>
#include <boost/algorithm/string.hpp>


namespace Ember::OgreView::Terrain {


TerrainLayerDefinitionManager::TerrainLayerDefinitionManager() {
	Ogre::ResourceGroupManager::getSingleton()._registerScriptLoader(this);

}

TerrainLayerDefinitionManager::~TerrainLayerDefinitionManager() {
	mDefinitions.clear();
	Ogre::ResourceGroupManager::getSingleton()._unregisterScriptLoader(this);
}

void TerrainLayerDefinitionManager::parseScript(Ogre::DataStreamPtr& stream, const Ogre::String& groupName) {
	XMLLayerDefinitionSerializer serializer;
	mDefinitions = serializer.parseScript(stream);
}


const std::vector<TerrainLayerDefinition>& TerrainLayerDefinitionManager::getDefinitions() const {
	return mDefinitions;
}

TerrainLayerDefinition* TerrainLayerDefinitionManager::getDefinitionForArea(unsigned int areaIndex) {
	for (auto& definition: mDefinitions) {
		if (definition.mAreaId == areaIndex) {
			return &definition;
		}
	}
	return nullptr;
}

TerrainLayerDefinition* TerrainLayerDefinitionManager::getDefinitionForShader(const std::string& shaderType) {
	for (auto& definition: mDefinitions) {
		if (definition.mShaderName == shaderType) {
			return &definition;
		}
	}
	return nullptr;
}

const Ogre::StringVector& TerrainLayerDefinitionManager::getScriptPatterns() const {
	static Ogre::StringVector patterns{"*.terrain", "*.terrain.xml"};
	return patterns;
}

Ogre::Real TerrainLayerDefinitionManager::getLoadingOrder() const {
	return 310;
}

}



