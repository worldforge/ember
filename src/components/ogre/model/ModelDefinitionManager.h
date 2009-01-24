//
// C++ Interface: ModelManager
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
#ifndef EMBEROGREMODELMANAGER_H
#define EMBEROGREMODELMANAGER_H

#include "components/ogre/EmberOgrePrerequisites.h"

#include <OgreResourceManager.h>
#include "framework/Singleton.h"
#include "ModelDefinition.h"

namespace EmberOgre {
namespace Model {

class ModelFactory;
class ModelBackgroundLoader;

/**
@brief Manages all of the ModelDefinition instances.
Responsible for creating, managing, and destroying ModelDefinition instances.
@author Erik Hjortsberg
*/
class ModelDefinitionManager: public Ogre::ResourceManager, public Ember::Singleton<ModelDefinitionManager>
{
public:
    /**
     * @brief Ctor.
     */
    ModelDefinitionManager();
    
    /**
     * @brief Dtor.
     */
    virtual ~ModelDefinitionManager();
    
	virtual Ogre::ResourcePtr create(const Ogre::String& name, const Ogre::String& group, 
		bool isManual = false, Ogre::ManualResourceLoader* loader = 0, 
		const Ogre::NameValuePairList* createParams = 0);
	
	/**
	 * @brief Parses the submitted script and creates ModelDefinition instances.
	 * @param stream The stream containing the script definition.
	 * @param groupName
	 */
	virtual void parseScript (Ogre::DataStreamPtr &stream, const Ogre::String &groupName);
	
	/**
	 * @brief Exports a modeldefinition to a file.
	 * The definition will be serialized and saved to a file by the same name of the definition.
	 * @param definition The definition to export.
	 */
	void exportScript(ModelDefnPtr definition);
	
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
	 * @brief Adds a new background loader which will be polled each frame.
	 * The loader will automatically be removed from the internal queue when the loading is complete.
	 * Note that ownership won't be transferred.
	 * Adding the same loader multiple times won't cause any errors.
	 * @param loader The background loader which will be polled every frame.
	 */
	void addBackgroundLoader(ModelBackgroundLoader* loader);
	
	/**
	 * @brief Removes a background loader from the list of loaders which will be polled every frame.
	 * This will not delete the loader.
	 * @param loader The loader that should be rmeoved. This should be an instance already submitted through a call to addBackgroundLoader(), but nothing will happen if it's not already added.
	 */
	void removeBackgroundLoader(ModelBackgroundLoader* loader);
	
	/**
	 * @brief Polls all of the background loaders.
	 * Call this each frame.
	 * All of the background loaders will have their poll() method called. If the background loader has finished loading it will be removed from the store of loaders and the model will be reloaded.
	 */
	void pollBackgroundLoaders();
	
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
	 * @brief An instance of our own model factory which we register with the ogre root object.
	 */
	ModelFactory* mModelFactory;
	
	/**
	 * @brief The collection of background loaders which will be polled each time pollBackgroundLoaders() are called.
	 */
	BackgroundLoaderStore mBackgroundLoaders;

};

}
};

#endif
