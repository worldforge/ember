//
// C++ Implementation: ModelDefinitionManager
//
// Description: 
//
//
// Author: Erik Hjortsberg <erik.hjortsberg@gmail.com>, (C) 2004
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

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "ModelDefinitionManager.h"
#include "ModelDefinition.h"
#include "Model.h"
#include "ModelBackgroundLoader.h"

#include "XMLModelDefinitionSerializer.h"


template<> EmberOgre::Model::ModelDefinitionManager* Ember::Singleton<EmberOgre::Model::ModelDefinitionManager>::ms_Singleton = 0;
namespace EmberOgre
{
namespace Model {

ModelDefinitionManager::ModelDefinitionManager() : mShowModels(true), mModelFactory(0)
{
    mLoadOrder = 300.0f;
    mResourceType = "ModelDefinition";
        
	mScriptPatterns.push_back("*.modeldef");
	mScriptPatterns.push_back("*.modeldef.xml");
	Ogre::ResourceGroupManager::getSingleton()._registerScriptLoader(this);

	Ogre::ResourceGroupManager::getSingleton()._registerResourceManager(mResourceType, this);
	
	
	///register factories
	mModelFactory = new ModelFactory();
	Ogre::Root::getSingleton().addMovableObjectFactory(mModelFactory);
	
}



ModelDefinitionManager::~ModelDefinitionManager()
{
	Ogre::ResourceGroupManager::getSingleton()._unregisterResourceManager(mResourceType);
	Ogre::ResourceGroupManager::getSingleton()._unregisterScriptLoader(this);
	///we need to make sure that all Models are destroyed before Ogre begins destroying other movable objects (such as Entities)
	///this is because Model internally uses Entities, so if those Entities are destroyed by Ogre before the Models are destroyed, the Models will try to delete them again, causing segfaults and other wickedness 
	Ogre::SceneManagerEnumerator::SceneManagerIterator sceneManagerIterator =  Ogre::SceneManagerEnumerator::getSingleton().getSceneManagerIterator();
	while (sceneManagerIterator.hasMoreElements()) {
		sceneManagerIterator.getNext()->destroyAllMovableObjectsByType(Model::sMovableType);
	}
	
	if (mModelFactory) {
		Ogre::Root::getSingleton().removeMovableObjectFactory(mModelFactory);
		delete mModelFactory;
	}
	
}




Ogre::ResourcePtr ModelDefinitionManager::create(const Ogre::String& name, const Ogre::String& group, 
bool isManual, Ogre::ManualResourceLoader* loader, 
const Ogre::NameValuePairList* createParams)
{
	Ogre::ResourcePtr ret = getByName(name);
	if (ret.isNull())
	{
		return Ogre::ResourceManager::create(name, group, isManual, loader, createParams);
	}
	///Report this. We count on this happening a lot (user media overriding shared media for example), so we will not consider it a failure.
	S_LOG_INFO("ModelDefinition with name " << name << " already exists.");
	return Ogre::ResourcePtr();

}

void ModelDefinitionManager::parseScript (Ogre::DataStreamPtr &stream, const Ogre::String &groupName)
{
    XMLModelDefinitionSerializer serializer;
	serializer.parseScript(stream, groupName);
}

void ModelDefinitionManager::exportScript(ModelDefnPtr definition)
{
	XMLModelDefinitionSerializer serializer;
	serializer.exportScript(definition, definition->getName() + ".modeldef");
}

Ogre::Resource* ModelDefinitionManager::createImpl(const Ogre::String& name, Ogre::ResourceHandle handle, const Ogre::String& group, bool isManual, Ogre::ManualResourceLoader* loader, const Ogre::NameValuePairList* createParams)
{
	return new ModelDefinition(this, name, handle, group, isManual, loader);
}

const std::vector<std::string> ModelDefinitionManager::getAllMeshes() const
{
	std::vector<std::string> meshes;
	Ogre::StringVectorPtr meshesVector = Ogre::ResourceGroupManager::getSingleton().findResourceNames("General", "*.mesh");
	for (Ogre::StringVector::iterator I = meshesVector->begin(); I != meshesVector->end(); ++I) {
		meshes.push_back(std::string(*I));
	}
	meshesVector.setNull();
	return meshes;
}

bool ModelDefinitionManager::getShowModels() const
{
	return mShowModels;
}

void ModelDefinitionManager::setShowModels(bool show)
{
	mShowModels = show;
}

void ModelDefinitionManager::addBackgroundLoader(ModelBackgroundLoader* loader)
{
	if (std::find(mBackgroundLoaders.begin(), mBackgroundLoaders.end(), loader) == mBackgroundLoaders.end()) {
		mBackgroundLoaders.push_back(loader);
	}
}

void ModelDefinitionManager::removeBackgroundLoader(ModelBackgroundLoader* loader)
{
	BackgroundLoaderStore::iterator I = std::find(mBackgroundLoaders.begin(), mBackgroundLoaders.end(), loader);
	if (I != mBackgroundLoaders.end()) {
		mBackgroundLoaders.erase(I);
	}
}


void ModelDefinitionManager::pollBackgroundLoaders()
{
	for (BackgroundLoaderStore::iterator I = mBackgroundLoaders.begin(); I != mBackgroundLoaders.end();) 
	{
		BackgroundLoaderStore::iterator I_copy = I;
		ModelBackgroundLoader* loader(*I);
		++I;
		if (loader->poll()) {
			mBackgroundLoaders.erase(I_copy);
			loader->reloadModel();
		}
	}
}


}
}
