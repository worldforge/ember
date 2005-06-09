//
// C++ Implementation: ModelDefinitionManager
//
// Description: 
//
//
// Author: Erik Hjortsberg <erik@katastrof.nu>, (C) 2004
// Copyright (c) 2005 The Cataclysmos Team
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

//perhaps this will make it build in debug mode without getting memory manager compile errors...
#include "components/ogre/EmberOgrePrerequisites.h"

#include "ModelDefinitionManager.h"
#include "ModelDefinition.h"

#include "XMLModelDefinitionSerializer.h"


namespace EmberOgre
{
    //-----------------------------------------------------------------------
    template<> ModelDefinitionManager* Ember::Singleton<ModelDefinitionManager>::ms_Singleton = 0;
/*    ModelDefinitionManager* ModelDefinitionManager::getSingletonPtr(void)
    {
        return ms_Singleton;
    }
    ModelDefinitionManager& ModelDefinitionManager::getSingleton(void)
    {  
        assert( ms_Singleton );  return ( *ms_Singleton );  
    }*/
    //-----------------------------------------------------------------------


ModelDefinitionManager::ModelDefinitionManager() : mSceneManager(0)
{
    mLoadOrder = 300.0f;
    mResourceType = "ModelDefinition";
        
	mScriptPatterns.push_back("*.modeldef.xml");
	Ogre::ResourceGroupManager::getSingleton()._registerScriptLoader(this);

	Ogre::ResourceGroupManager::getSingleton()._registerResourceManager(mResourceType, this);
	
	loadAreas();
}



ModelDefinitionManager::~ModelDefinitionManager()
{
	Ogre::ResourceGroupManager::getSingleton()._unregisterResourceManager(mResourceType);
}

void ModelDefinitionManager::loadAreas() {
	ModelDefinition::AreaDefinition def;
	def.Id = 6; //oak
	def.TextureName = "3d_objects/environment/ground/textures/leaf_covered/ground.png";
	mAreaDefinitions[def.Id] = def;
	
	def = ModelDefinition::AreaDefinition();
	def.Id = 8; //ploughed field
	def.MaterialName = "/global/environment/ground/ploughed_field";
	mAreaDefinitions[def.Id] = def;
	
	def = ModelDefinition::AreaDefinition();
	def.Id = 7; //sty
	def.TextureName = "sand.png";
	mAreaDefinitions[def.Id] = def;
	
}

const ModelDefinition::AreaDefinition* ModelDefinitionManager::getAreaDefinition(int layer) const
{

	AreaDefinitionStore::const_iterator I = mAreaDefinitions.find(layer);
	if (I != mAreaDefinitions.end()) {
		return &(I->second);
	}
	return 0;
}

void ModelDefinitionManager::parseScript (Ogre::DataStreamPtr &stream, const Ogre::String &groupName)
{
    XMLModelDefinitionSerializer serializer;
	serializer.parseScript(stream, groupName);
}

Ogre::Resource* ModelDefinitionManager::createImpl(const Ogre::String& name, Ogre::ResourceHandle handle, 
    const Ogre::String& group, bool isManual, Ogre::ManualResourceLoader* loader, 
    const Ogre::NameValuePairList* createParams)
{
     return new ModelDefinition(this, name, handle, group, isManual, loader);
}

}
