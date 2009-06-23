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
// #include "OgrePagingLandScapeOptions.h"


namespace EmberOgre {
class EmberPagingSceneManager;

namespace Model {
class Model;
}

class IPageData
{
	void createHeightData(Ogre::Real* heightData);
	Ogre::MaterialPtr getMaterial();
};


class IPageDataProvider
{
	IPageData* getPageData(Ogre::Vector2 position);
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



        /** Things that need to be allocated once 
		 */
	void InitScene( void );

    EmberPagingSceneManager(const Ogre::String &name);

// 	EmberTerrainSceneManager();
// 	virtual ~EmberTerrainSceneManager();

// 	void attachPage(Ogre::ushort pageX, Ogre::ushort pageZ, Ogre::TerrainPage* page,float maxY, float minY);
// 	Ogre::TerrainPage* getTerrainPage( const Ogre::Vector3 & pt );

	/*
	 * Resizes the octree. Do this after adding pages.
	 */
// 	void doResize();
	
	/*
	 * The scenemanager stores the pages in vectors. This does not allow
	 * for pages with negative indices.
	 * But WF uses negative terrain coordinates.
	 * Thus we need to offset the indices.
	 */
/*	int getPageOffset();
	
	void setWorldGeometry( const Ogre::String& filename );
	void setWorldGeometry(  Ogre::TerrainOptions& options );*/
	
	
 /*       const Ogre::PagingLandScapeOptions * getOptions() const
        {
            assert(mOptions);
            return mOptions;
        }*/
	
	
	/**
	 *    Utility method for creating a new Model.
	 * @param modelName the id of the model
	 * @param modelDefinitionName the name of the model defition from which the model should be created
	 * @return 
	 */
	Model::Model* createModel(
									const Ogre::String& modelName,
									const Ogre::String& modelDefinitionName );
									
	void registerProvider(IPageDataProvider* provider);
	
	const Ogre::String& getTypeName(void) const;

protected:

	/* 
	 * @see EmberOgre::EmberTerrainSceneManager::getPageOffset()
	 */
	Ogre::ushort mPageOffset;
	
// 	/*
// 	 * Stitches the neighbours, preventing gaps
// 	 */
// 	void setupPageNeighbors(Ogre::ushort pageX, Ogre::ushort pageZ, Ogre::TerrainPage* page); 

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
