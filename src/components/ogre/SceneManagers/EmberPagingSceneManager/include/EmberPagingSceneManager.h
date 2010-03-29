//
// C++ Interface: EmberPagingSceneManager
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

#ifndef EMBERPAGINGSCENEMANAGER_H
#define EMBERPAGINGSCENEMANAGER_H

#include "../../../EmberOgrePrerequisites.h"
#include "OgrePagingLandScapeSceneManager.h"


namespace EmberOgre {
class EmberPagingSceneManager;

namespace Terrain {
class ITerrainPageBridge;
}
namespace Model {
class Model;
}

/**
 * @brief Page data for one page in the terrain rendering system.
 */
class IPageData
{
public:
	virtual ~IPageData() {}
	virtual Ogre::MaterialPtr getMaterial() = 0;
};


/**
 * @brief Provides IPageData instances for the terrain rendering system.
 */
class IPageDataProvider
{
public:
	typedef std::pair<unsigned int, unsigned int> OgreIndex;
	virtual IPageData* getPageData(const OgreIndex& index) = 0;
	virtual int getPageIndexSize() const = 0;
	virtual void setUpTerrainPageAtIndex(const OgreIndex& ogreIndexPosition, ::EmberOgre::Terrain::ITerrainPageBridge* bridge) = 0;
	virtual void removeBridge(const OgreIndex& ogreIndexPosition) = 0;
};


     /// Factory for OctreeSceneManager
     class EmberPagingSceneManagerFactory : public Ogre::SceneManagerFactory
     {
     protected:
     	void initMetaData(void) const;
     public:
     	EmberPagingSceneManagerFactory() {}
     	~EmberPagingSceneManagerFactory() {}
     	/// Factory type name
       	static const Ogre::String FACTORY_TYPE_NAME;
     	Ogre::SceneManager* createInstance(const Ogre::String& instanceName);
     	void destroyInstance(Ogre::SceneManager* instance);
     };

/**
 * This is a specialization of Ogre::PagingLandScapeSceneManager.
 * 
 * @see Ogre::PagingLandScapeSceneManager
 */
class EmberPagingSceneManager  : public Ogre::PagingLandScapeSceneManager  {
public:

	void InitScene( void );

    EmberPagingSceneManager(const Ogre::String &name);
	
	/**
	 *    Utility method for creating a new Model.
	 * @param modelName the id of the model
	 * @param modelDefinitionName the name of the model defition from which the model should be created
	 * @return 
	 */
	Model::Model* createModel(const Ogre::String& modelName, const Ogre::String& modelDefinitionName);
									
	void registerProvider(IPageDataProvider* provider);
	
	IPageDataProvider* getProvider();

	const Ogre::String& getTypeName(void) const;

protected:

	/* 
	 * @see EmberOgre::EmberTerrainSceneManager::getPageOffset()
	 */
	Ogre::ushort mPageOffset;
	
	/*
	 * Max and min values for the world. Used to resize the octree.
	 */
	float mMaxX;
	float mMaxY;
	float mMaxZ;
	float mMinX;
	float mMinY;
	float mMinZ;
	
	IPageDataProvider* mProvider;
	
	virtual const Ogre::Pass* deriveShadowCasterPass(const Ogre::Pass* pass);

private:
};

}

#endif // EMBERPAGINGSCENEMANAGER_H
