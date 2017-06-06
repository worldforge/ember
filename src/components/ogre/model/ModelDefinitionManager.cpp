//
// C++ Implementation: ModelDefinitionManager
//
// Description:
//
//
// Author: Erik Ogenvik <erik@ogenvik.org>, (C) 2004
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
#include "Model.h"
#include "ModelBackgroundLoader.h"

#include "XMLModelDefinitionSerializer.h"

#include "framework/TimeFrame.h"
#include "framework/Tokeniser.h"

#include <OgreRoot.h>


template<> Ember::OgreView::Model::ModelDefinitionManager* Ember::Singleton<Ember::OgreView::Model::ModelDefinitionManager>::ms_Singleton = 0;
namespace Ember
{
namespace OgreView
{
namespace Model {

ModelDefinitionManager::ModelDefinitionManager(const std::string& exportDirectory, Eris::EventService& eventService)
: ShowModels("showmodels", this, "Show or hide models."), mShowModels(true), mExportDirectory(exportDirectory)
{
	mLoadOrder = 300.0f;
	mResourceType = "ModelDefinition";

	mScriptPatterns.push_back("*.modeldef");
	mScriptPatterns.push_back("*.modeldef.xml");
	Ogre::ResourceGroupManager::getSingleton()._registerScriptLoader(this);

	Ogre::ResourceGroupManager::getSingleton()._registerResourceManager(mResourceType, this);

}



ModelDefinitionManager::~ModelDefinitionManager()
{
	Ogre::ResourceGroupManager::getSingleton()._unregisterResourceManager(mResourceType);
	Ogre::ResourceGroupManager::getSingleton()._unregisterScriptLoader(this);
	//we need to make sure that all Models are destroyed before Ogre begins destroying other movable objects (such as Entities)
	//this is because Model internally uses Entities, so if those Entities are destroyed by Ogre before the Models are destroyed, the Models will try to delete them again, causing segfaults and other wickedness
	Ogre::SceneManagerEnumerator::SceneManagerIterator sceneManagerIterator =  Ogre::SceneManagerEnumerator::getSingleton().getSceneManagerIterator();
}

ModelDefinitionPtr ModelDefinitionManager::create (const Ogre::String& name, const Ogre::String& group,
        bool isManual, Ogre::ManualResourceLoader* loader,
        const Ogre::NameValuePairList* createParams)
{
    return createResource(name, group, isManual, loader, createParams).staticCast<ModelDefinition>();
}

Ogre::Resource* ModelDefinitionManager::createImpl(const Ogre::String& name, Ogre::ResourceHandle handle,
		const Ogre::String& group, bool isManual, Ogre::ManualResourceLoader* loader,
		const Ogre::NameValuePairList* params)
{
	Ogre::ResourcePtr ret = getResourceByName(name);
	if (ret.isNull())
	{
		return OGRE_NEW ModelDefinition(this, name, handle, group, isManual, loader);
	}
	//Report this. We count on this happening a lot (user media overriding shared media for example), so we will not consider it a failure.
	S_LOG_INFO("ModelDefinition with name " << name << " already exists.");
	return nullptr;
}

Ogre::ResourcePtr ModelDefinitionManager::createResource(const Ogre::String& name, const Ogre::String& group, bool isManual, Ogre::ManualResourceLoader* loader, const Ogre::NameValuePairList* params)
{
	// Call creation implementation
	Ogre::ResourcePtr ret = Ogre::ResourcePtr(
			createImpl(name, getNextHandle(), group, isManual, loader, params));

	if (ret.isNull())
		return ret;

	if (params)
		ret->setParameterList(*params);

	addImpl(ret);
	// Tell resource group manager
	Ogre::ResourceGroupManager::getSingleton()._notifyResourceCreated(ret);
	return ret;
}

void ModelDefinitionManager::parseScript (Ogre::DataStreamPtr &stream, const Ogre::String &groupName)
{
    XMLModelDefinitionSerializer serializer;
	serializer.parseScript(*this, stream, groupName);
}

std::string ModelDefinitionManager::exportScript(ModelDefinitionPtr definition)
{
	XMLModelDefinitionSerializer serializer;
	bool success = serializer.exportScript(definition, mExportDirectory, definition->getName() + ".modeldef");
	if (success) {
		return mExportDirectory + definition->getName() + ".modeldef";
	} else {
		return "";
	}
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


ModelDefinitionPtr ModelDefinitionManager::getByName(const Ogre::String& name, const Ogre::String& groupName)
{
    return getResourceByName(name, groupName).staticCast<ModelDefinition>();
}

bool ModelDefinitionManager::getShowModels() const
{
	return mShowModels;
}

void ModelDefinitionManager::setShowModels(bool show)
{
	mShowModels = show;
}


void ModelDefinitionManager::runCommand(const std::string &command, const std::string &args)
{
	if (command == ShowModels.getCommand()) {
		Tokeniser tokeniser;
		tokeniser.initTokens(args);
		std::string value = tokeniser.nextToken();
		if (value == "true") {
			S_LOG_INFO("Showing models.");
			setShowModels(true);
		} else if (value == "false") {
			S_LOG_INFO("Hiding models.");
			setShowModels(false);
		}
	}
}

void ModelDefinitionManager::populateModel(Model* model, const Ogre::SharedPtr<ModelDefinition>& definition)
{
	//TODO: do background loading
	//model->
}



}
}
}
