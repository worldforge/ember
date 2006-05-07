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

#include <OgreResourceManager.h>
#include "framework/Singleton.h"
#include "ModelDefinition.h"

namespace EmberOgre {
namespace Model {

/**
@author Erik Hjortsberg
*/
class ModelDefinitionManager: public Ogre::ResourceManager, public Ember::Singleton<ModelDefinitionManager>

{
public:
    /// Constructor
    ModelDefinitionManager();
    ~ModelDefinitionManager();
    
	virtual Ogre::ResourcePtr create(const Ogre::String& name, const Ogre::String& group, 
		bool isManual = false, Ogre::ManualResourceLoader* loader = 0, 
		const Ogre::NameValuePairList* createParams = 0);
	
	inline Ogre::SceneManager* getSceneManager() const;
	inline void setSceneManager(Ogre::SceneManager* sceneManager);
	virtual void parseScript (Ogre::DataStreamPtr &stream, const Ogre::String &groupName);
	
	/**
	 *    Exports a modeldefinition to a file.
	 * @param definition 
	 */
	void exportScript(ModelDefnPtr definition);
	
	/**
	 *    Gets the AreaDefinition for a certain layer.
	 *    TODO: this should be moved out from here and to a better place
	 * @param layer 
	 * @return 
	 */
	const ModelDefinition::AreaDefinition* getAreaDefinition(int layer) const;
	
	/**
	 *    Gets a vector of all mesh names.
	 * @return 
	 */
	const std::vector<std::string> getAllMeshes() const;
	
	/**
	 *    Returns whether models should be shown.
	 * @return 
	 */
	bool getShowModels() const;
	
	/**
	 *    Sets whether models should be shown.
	 * @param show 
	 */
	void setShowModels(bool show);
	
protected:

	Ogre::Resource* createImpl(const Ogre::String& name, Ogre::ResourceHandle handle, 
        const Ogre::String& group, bool isManual, Ogre::ManualResourceLoader* loader, 
        const Ogre::NameValuePairList* createParams);
	Ogre::SceneManager* mSceneManager;
	
	typedef std::map<int, ModelDefinition::AreaDefinition> AreaDefinitionStore;
	AreaDefinitionStore mAreaDefinitions;
	void loadAreas();
	/**
	Determines whether models should be shown.
	*/
	bool mShowModels;

};

///inline implementations
	Ogre::SceneManager* ModelDefinitionManager::getSceneManager() const 
	{ 
		return mSceneManager; 
	}
	
	void ModelDefinitionManager::setSceneManager(Ogre::SceneManager* sceneManager) 
	{ 
		mSceneManager = sceneManager; 
	}

}
};

#endif
