//
// C++ Implementation: EmberPagingSceneManager
//
// Description: 
//
//
// Author: Erik Hjortsberg <erik.hjortsberg@gmail.com>, (C) 2005
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

#include "EmberPagingSceneManager.h"
#include "EmberOgrePrerequisites.h"

#include "OgrePagingLandScapeData2DManager.h"
#include "OgrePagingLandScapeTextureManager.h"
#include "OgrePagingLandScapeOptions.h"
#include "EmberPagingLandScapeData2D_HeightField.h"
#include "EmberPagingLandScapeTexture.h"
#include "model/Model.h"

namespace EmberOgre {

    const Ogre::String EmberPagingSceneManagerFactory::FACTORY_TYPE_NAME = "EmberPagingSceneManager";
    //-----------------------------------------------------------------------
    void EmberPagingSceneManagerFactory::initMetaData(void) const
    {
 	   mMetaData.typeName = FACTORY_TYPE_NAME;
 	   mMetaData.description = "Uses the PagingLandscapeSceneManager.";
 	   mMetaData.sceneTypeMask = Ogre::ST_EXTERIOR_REAL_FAR; // support all types
 	   mMetaData.worldGeometrySupported = false;
    }
    //-----------------------------------------------------------------------
    Ogre::SceneManager* EmberPagingSceneManagerFactory::createInstance(
 	   const Ogre::String& instanceName)
    {
 	   return new EmberPagingSceneManager(instanceName);
    }
    //-----------------------------------------------------------------------
    void EmberPagingSceneManagerFactory::destroyInstance(Ogre::SceneManager* instance)
    {
 	   delete instance;
    }    
    
    
    EmberPagingSceneManager::EmberPagingSceneManager(const Ogre::String &name): PagingLandScapeSceneManager(name)
    {
        if (!mOptions)
            mOptions = new Ogre::PagingLandScapeOptions(this);
    }
    
    
    //-----------------------------------------------------------------------
    void EmberPagingSceneManager::InitScene ()
	{        
		PagingLandScapeSceneManager::InitScene ();
		
		getData2DManager()->registerDataType (new EmberPagingLandScapeData2D_HeightField (getData2DManager()));
		getTextureManager()->registerTextureType (new EmberPagingLandScapeTexture (getTextureManager()));

	}
	
	void EmberPagingSceneManager::registerProvider(IPageDataProvider* provider)
	{
		mProvider = provider;
	}
	
	Model::Model* EmberPagingSceneManager::createModel(
									const Ogre::String& modelName,
									const Ogre::String& modelDefinitionName )
	{
		// delegate to factory implementation
		Ogre::NameValuePairList params;
		params["modeldefinition"] = modelDefinitionName;
		return static_cast<Model::Model*>(
			createMovableObject(modelName, Model::ModelFactory::FACTORY_TYPE_NAME, 
				&params));
	
	}
	
	const Ogre::String& EmberPagingSceneManager::getTypeName(void) const
	{
		return EmberPagingSceneManagerFactory::FACTORY_TYPE_NAME;
	}
	
}

