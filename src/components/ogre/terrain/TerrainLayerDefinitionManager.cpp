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

template<> Ember::OgreView::Terrain::TerrainLayerDefinitionManager* Ember::Singleton<Ember::OgreView::Terrain::TerrainLayerDefinitionManager>::ms_Singleton = 0;

namespace Ember {
namespace OgreView {

namespace Terrain {


TerrainLayerDefinitionManager::TerrainLayerDefinitionManager()
{
    mLoadOrder = 310.0f;
    mResourceType = "TerrainLayerDefinition";
        
	mScriptPatterns.emplace_back("*.terrain");
// 	mScriptPatterns.push_back("*.modelmap.xml");
	Ogre::ResourceGroupManager::getSingleton()._registerScriptLoader(this);

	Ogre::ResourceGroupManager::getSingleton()._registerResourceManager(mResourceType, this);
}


TerrainLayerDefinitionManager::~TerrainLayerDefinitionManager()
{
	for (auto& mDefinition : mDefinitions) {
		delete mDefinition;
	}	
	Ogre::ResourceGroupManager::getSingleton()._unregisterScriptLoader(this);
	Ogre::ResourceGroupManager::getSingleton()._unregisterResourceManager(mResourceType);
}

void TerrainLayerDefinitionManager::parseScript (Ogre::DataStreamPtr &stream, const Ogre::String &groupName)
{
    XMLLayerDefinitionSerializer serializer(*this);
	serializer.parseScript(stream, groupName);
}

void TerrainLayerDefinitionManager::addDefinition(TerrainLayerDefinition* definition)
{
	mDefinitions.push_back(definition);
}

const TerrainLayerDefinitionManager::DefinitionStore& TerrainLayerDefinitionManager::getDefinitions() const
{
	return mDefinitions;
}

Ogre::Resource* TerrainLayerDefinitionManager::createImpl(const Ogre::String& name, Ogre::ResourceHandle handle, 
    const Ogre::String& group, bool isManual, Ogre::ManualResourceLoader* loader, 
    const Ogre::NameValuePairList* createParams)
{
	return nullptr;
}

TerrainLayerDefinition* TerrainLayerDefinitionManager::getDefinitionForArea(unsigned int areaIndex)
{
	for (auto& definition : mDefinitions) {
		if (definition->getAreaId() == areaIndex) {
			return definition;
		}
	}
	return nullptr;
}

TerrainLayerDefinition* TerrainLayerDefinitionManager::getDefinitionForShader(const std::string& shaderType)
{
	for (auto& definition : mDefinitions) {
		if (definition->getShaderName() == shaderType) {
			return definition;
		}
	}
	return nullptr;
}


}

}
}
