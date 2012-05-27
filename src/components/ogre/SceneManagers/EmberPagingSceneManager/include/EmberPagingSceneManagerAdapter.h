//
// C++ Interface: EmberPagingSceneManagerAdapter
//
// Description: 
//
//
// Author: Erik Hjortsberg <erik.hjortsberg@gmail.com>, (C) 2006
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

#include "../../../EmberOgrePrerequisites.h"

#include "../../../terrain/ISceneManagerAdapter.h"
#include "OgrePagingLandScapeCallBackEvent.h"

#include <unordered_map>

namespace Ogre
{
class PagingLandScapeOptions;
class SceneManager;
}

namespace Ember {
namespace OgreView {

namespace Terrain {
class ITerrainObserver;
}

class EmberPagingSceneManager;
class EmberPagingTerrainObserver;

/**
An adapter specific for the PagingLandscapeSceneManager.
@author Erik Hjortsberg
*/
class EmberPagingSceneManagerAdapter : public Terrain::ISceneManagerAdapter
{

public:

	EmberPagingSceneManagerAdapter(EmberPagingSceneManager& scenemanager);
	
	virtual ~EmberPagingSceneManagerAdapter();
	
	virtual int getPageSize();
	virtual Ogre::Real getHeightAt(const Ogre::Real x, const Ogre::Real z);
	
	virtual void setWorldPagesDimensions(int numberOfPagesWidth, int numberOfPagesHeight, int widthOffsetInPages, int heightOffsetInPages);
	
	virtual void setCamera(Ogre::Camera* camera);
	virtual void setResourceGroupName(const std::string& groupName);
	virtual void loadOptions(const std::string& filePath);
	virtual void resize(Ogre::AxisAlignedBox newSize, int levels);
	virtual void loadScene();
	virtual void reset();

	virtual void setOption(const std::string& strKey, const void* pValue);
	virtual void getOption(const std::string& strKey, void* pDestValue);

	virtual void setUninitializedHeight(float height);

	virtual Ogre::SceneManager& getSceneManager() const;
	virtual void reloadAllPages();
	virtual void reloadPage(unsigned int x, unsigned int z);
	
	virtual void loadFirstPage();

	virtual std::string getDebugInfo();

	/**
	 * @brief Creates a new terrain observer.
	 * @return A new terrain observer.
	 */
	virtual Terrain::ITerrainObserver* createObserver();

	/**
	 * @brief Destroys a terrain observer.
	 * @param observer A terrain observer.
	 */
	virtual void destroyObserver(Terrain::ITerrainObserver* observer);

private:

	typedef std::unordered_map<EmberPagingTerrainObserver*, Ogre::PagingLandscapeDelegate*> TerrainObserverStore;

	EmberPagingSceneManager& mSceneManager;

	/**
	 * @brief The terrain observers handled by this instance.
	 */
	TerrainObserverStore mTerrainObservers;

	Ogre::PagingLandScapeOptions* getOptions();



};

}

}
