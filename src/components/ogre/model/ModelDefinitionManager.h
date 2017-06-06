//
// C++ Interface: ModelManager
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
#ifndef EMBEROGREMODELMANAGER_H
#define EMBEROGREMODELMANAGER_H

#include "components/ogre/EmberOgrePrerequisites.h"

#include "ModelDefinition.h"
#include "framework/Singleton.h"
#include "framework/ConsoleObject.h"

#include <OgreResourceManager.h>

namespace Eris {
class EventService;
}

namespace Ember {
class TimeFrame;
namespace OgreView {
namespace Model {

class ModelFactory;
class ModelBackgroundLoader;


/**
 *
 * @brief Manages all of the ModelDefinition instances.
 *
 * Responsible for creating, managing, and destroying ModelDefinition instances.
 *
 * @author Erik Ogenvik
 */
class ModelDefinitionManager: public Ogre::ResourceManager, public Singleton<ModelDefinitionManager>, public ConsoleObject
{
public:
	/**
	 * @brief Ctor.
	 *
	 * @param exportDirectory The path to the export directory, where exported models will be stored.
	 */
	ModelDefinitionManager(const std::string& exportDirectory, Eris::EventService& eventService);

	/**
	 * @brief Dtor.
	 */
	virtual ~ModelDefinitionManager();

	/// Create a new ModelDefinition
	/// @see ResourceManager::createResource
	ModelDefinitionPtr create (const Ogre::String& name, const Ogre::String& group,
			bool isManual = false, Ogre::ManualResourceLoader* loader = 0,
			const Ogre::NameValuePairList* createParams = 0);

	/** Creates a new blank resource, but does not immediately load it.
	@remarks
		Resource managers handle disparate types of resources, so if you want
		to get at the detailed interface of this resource, you'll have to 
		cast the result to the subclass you know you're creating. 
	@param name The unique name of the resource
	@param group The name of the resource group to attach this new resource to
	@param isManual Is this resource manually loaded? If so, you should really
		populate the loader parameter in order that the load process
		can call the loader back when loading is required. 
	@param loader Pointer to a ManualLoader implementation which will be called
		when the Resource wishes to load (should be supplied if you set
		isManual to true). You can in fact leave this parameter null 
		if you wish, but the Resource will never be able to reload if 
		anything ever causes it to unload. Therefore provision of a proper
		ManualLoader instance is strongly recommended.
	@param createParams If any parameters are required to create an instance,
		they should be supplied here as name / value pairs
	*/
	virtual Ogre::ResourcePtr createResource(const Ogre::String& name, const Ogre::String& group,
			bool isManual = false, Ogre::ManualResourceLoader* loader = 0, 
			const Ogre::NameValuePairList* createParams = 0);

	/**
	 * @brief Parses the submitted script and creates ModelDefinition instances.
	 * @param stream The stream containing the script definition.
	 * @param groupName
	 */
	virtual void parseScript(Ogre::DataStreamPtr& stream, const Ogre::String& groupName);
	
	/**
	 * @brief Exports a modeldefinition to a file.
	 * The definition will be serialized and saved to a file by the same name of the definition.
	 * @param definition The definition to export.
	 * @return The path to the exported script. If the export failed, the string will be empty.
	 */
	std::string exportScript(ModelDefinitionPtr definition);


	/// Get a ModelDefinition by name
	/// @see ResourceManager::getResourceByName
	ModelDefinitionPtr getByName(const Ogre::String& name, const Ogre::String& groupName = Ogre::ResourceGroupManager::AUTODETECT_RESOURCE_GROUP_NAME);
	
	/**
	 * @brief Gets a vector of all mesh names in the system.
	 * This is mainly a helper method for retrieving a list of all available meshes.
	 * @return A list of all available meshes.
	 */
	const std::vector<std::string> getAllMeshes() const;
	
	/**
	 * @brief Returns whether all models should be shown using their normal visibility settings.
	 * @return True if all models should use their normal visibility settings for determining whether they should be shown.
	 */
	bool getShowModels() const;
	
	/**
	 * @brief Sets whether models should be shown, using their normal visibility settings.
	 * This is useful for hiding all models in the system.
	 * @param show If false, all models will be hidden. If true, each model will use its normal visibility setting.
	 */
	void setShowModels(bool show);
	
	/**
	 *    Reimplements the ConsoleObject::runCommand method
	 * @param command
	 * @param args
	 */
	void runCommand(const std::string &command, const std::string &args) override;


	void populateModel(Model* model, const Ogre::SharedPtr<ModelDefinition>& definition);


	/**
	 Command for setting whether models should be shown or not.
	 */
	const ConsoleCommandWrapper ShowModels;


protected:

	/**
	 * @brief A store of background loaders.
	 */
	typedef std::list<ModelBackgroundLoader*> BackgroundLoaderStore;
	Ogre::Resource* createImpl(const Ogre::String& name, Ogre::ResourceHandle handle, 
        const Ogre::String& group, bool isManual, Ogre::ManualResourceLoader* loader, 
        const Ogre::NameValuePairList* createParams);
	
	/**
	 * @brief Determines whether models should be shown.
	 */
	bool mShowModels;

	/**
	 * @brief The collection of background loaders which will be polled each time pollBackgroundLoaders() are called.
	 */
	BackgroundLoaderStore mBackgroundLoaders;

	/**
	 * @brief The path to the export directory.
	 */
	const std::string mExportDirectory;


};

}
}
}

#endif
