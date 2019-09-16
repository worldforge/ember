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

template<> Ember::OgreView::Terrain::TerrainLayerDefinitionManager* Ember::Singleton<Ember::OgreView::Terrain::TerrainLayerDefinitionManager>::ms_Singleton = nullptr;

namespace Ember {
namespace OgreView {

namespace Terrain {


TerrainLayerDefinitionManager::TerrainLayerDefinitionManager() {
	mLoadOrder = 310.0f;
	mResourceType = "TerrainLayerDefinition";

	mScriptPatterns.emplace_back("*.terrain");
// 	mScriptPatterns.push_back("*.modelmap.xml");
	Ogre::ResourceGroupManager::getSingleton()._registerScriptLoader(this);

	Ogre::ResourceGroupManager::getSingleton()._registerResourceManager(mResourceType, this);
}


TerrainLayerDefinitionManager::~TerrainLayerDefinitionManager() {
	mDefinitions.clear();
	Ogre::ResourceGroupManager::getSingleton()._unregisterScriptLoader(this);
	Ogre::ResourceGroupManager::getSingleton()._unregisterResourceManager(mResourceType);
}

void TerrainLayerDefinitionManager::parseScript(Ogre::DataStreamPtr& stream, const Ogre::String& groupName) {
	XMLLayerDefinitionSerializer serializer(*this);
	serializer.parseScript(stream, groupName);
}

void TerrainLayerDefinitionManager::addDefinition(TerrainLayerDefinition* definition) {
	mDefinitions.emplace_back(definition);
}

const TerrainLayerDefinitionManager::DefinitionStore& TerrainLayerDefinitionManager::getDefinitions() const {
	return mDefinitions;
}

Ogre::Resource* TerrainLayerDefinitionManager::createImpl(const Ogre::String& name, Ogre::ResourceHandle handle,
														  const Ogre::String& group, bool isManual, Ogre::ManualResourceLoader* loader,
														  const Ogre::NameValuePairList* createParams) {
	return nullptr;
}

TerrainLayerDefinition* TerrainLayerDefinitionManager::getDefinitionForArea(unsigned int areaIndex) {
	for (auto& definition : mDefinitions) {
		if (definition->getAreaId() == areaIndex) {
			return definition.get();
		}
	}
	return nullptr;
}

TerrainLayerDefinition* TerrainLayerDefinitionManager::getDefinitionForShader(const std::string& shaderType) {
	for (auto& definition : mDefinitions) {
		if (definition->getShaderName() == shaderType) {
			return definition.get();
		}
	}
	return nullptr;
}

void TerrainLayerDefinitionManager::resolveTextureReferences() {
	//Since we support using both the raw media repository as well as the processed media we need to make sure we
	//can load textures independent of whether they are .png or .dds.
	auto resolveTextureFn = [&](const std::string& texture) -> std::string {
		auto& textureMgr = Ogre::TextureManager::getSingleton();
		if (textureMgr.resourceExists(texture, Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME)) {
			return texture;
		}
		auto locations = Ogre::ResourceGroupManager::getSingleton().getResourceLocationList(Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
		for (auto location : locations) {
			if (location->archive->exists(texture)) {
				return texture;
			}
		}

		if (boost::algorithm::ends_with(texture, ".png")) {
			std::string newTextureName = texture.substr(0, texture.length() - 4) + ".dds";
			for (auto location : locations) {
				if (location->archive->exists(newTextureName)) {
					return newTextureName;
				}
			}
		}
		if (boost::algorithm::ends_with(texture, ".dds")) {
			std::string newTextureName = texture.substr(0, texture.length() - 4) + ".png";
			for (auto location : locations) {
				if (location->archive->exists(newTextureName)) {
					return newTextureName;
				}
			}
		}
		return "";
	};

	for (auto& def : mDefinitions) {
		if (!def->getDiffuseTextureName().empty()) {
			def->setDiffuseTextureName(resolveTextureFn(def->getDiffuseTextureName()));
		}
		if (!def->getNormalMapTextureName().empty()) {
			def->setNormalMapTextureName(resolveTextureFn(def->getNormalMapTextureName()));
		}
	}
}


}

}
}
