//
// C++ Implementation: TerrainLayerDefinitionManager
//
// Description: 
//
//
// Author: Erik Hjortsberg <erik.hjortsberg@iteam.se>, (C) 2007
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
// along with this program; if not, write to the Free Software
// Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.//
//
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "TerrainLayerDefinitionManager.h"
#include "TerrainLayerDefinition.h"
#include "XMLLayerDefinitionSerializer.h"
namespace EmberOgre {

namespace Terrain {

template<> EmberOgre::Terrain::TerrainLayerDefinitionManager* Ember::Singleton<EmberOgre::Terrain::TerrainLayerDefinitionManager>::ms_Singleton = 0;

TerrainLayerDefinitionManager::TerrainLayerDefinitionManager()
{
    mLoadOrder = 310.0f;
    mResourceType = "TerrainLayerDefinition";
        
	mScriptPatterns.push_back("*.terrain");
// 	mScriptPatterns.push_back("*.modelmap.xml");
	Ogre::ResourceGroupManager::getSingleton()._registerScriptLoader(this);

	Ogre::ResourceGroupManager::getSingleton()._registerResourceManager(mResourceType, this);
}


TerrainLayerDefinitionManager::~TerrainLayerDefinitionManager()
{
	for (DefinitionStore::iterator I = mDefinitions.begin(); I != mDefinitions.end(); ++I) {
		delete *I;
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
	return 0;
}

TerrainLayerDefinition* TerrainLayerDefinitionManager::getDefinitionForArea(unsigned int areaIndex)
{
	for (DefinitionStore::iterator I = mDefinitions.begin(); I != mDefinitions.end(); ++I) {
		if ((*I)->getAreaId() == areaIndex) {
			return *I;
		}
	}
	return 0;
}

TerrainLayerDefinition* TerrainLayerDefinitionManager::getDefinitionForShader(const std::string& shaderType)
{
	for (DefinitionStore::iterator I = mDefinitions.begin(); I != mDefinitions.end(); ++I) {
		if ((*I)->getShaderName() == shaderType) {
			return *I;
		}
	}
	return 0;
}


}

}
