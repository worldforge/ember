//
// C++ Implementation: EmberPagingSceneManagerAdapter
//
// Description: 
//
//
// Author: Erik Hjortsberg <erik@katastrof.nu>, (C) 2006
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

#include "EmberPagingSceneManagerAdapter.h"

#include "EmberPagingSceneManager.h"
#include "OgrePagingLandScapeOptions.h"
#include "OgrePagingLandScapePageManager.h"
#include "OgrePagingLandScapePage.h"

namespace EmberOgre {
	
	Ogre::PagingLandScapeOptions* EmberPagingSceneManagerAdapter::getOptions()
	{
		return mSceneManager->getOptions();
	}
	
	EmberPagingSceneManagerAdapter::EmberPagingSceneManagerAdapter(EmberPagingSceneManager* scenemanager) : mSceneManager(scenemanager)
	{
	}
	
	int EmberPagingSceneManagerAdapter::getPageSize()
	{
		return getOptions()->PageSize;
	}
	
	Ogre::Real EmberPagingSceneManagerAdapter::getHeightAt(const Ogre::Real x, const Ogre::Real z)
	{
		return mSceneManager->getHeightAt(x, z);
	}

	
	void EmberPagingSceneManagerAdapter::setWorldPagesDimensions(int numberOfPagesHeight, int numberOfPagesWidth)
	{
		getOptions()->world_height = numberOfPagesHeight;
		getOptions()->world_width = numberOfPagesWidth;
	
		//update the options
		getOptions()->NumPages = getOptions()->world_height * getOptions()->world_width;
		getOptions()->maxUnScaledZ = getOptions()->world_height * (getOptions()->PageSize - 1) * 0.5f;
		getOptions()->maxUnScaledX = getOptions()->world_width  * (getOptions()->PageSize - 1) * 0.5f;
	
		getOptions()->maxScaledZ = getOptions()->scale.z * getOptions()->maxUnScaledZ;
		getOptions()->maxScaledX = getOptions()->scale.x * getOptions()->maxUnScaledX;

	}
	
	void EmberPagingSceneManagerAdapter::resize(Ogre::AxisAlignedBox newSize, int levels)
	{
		mSceneManager->resize(newSize, levels);
	}

	void EmberPagingSceneManagerAdapter::setCamera(Ogre::Camera* camera)
	{
		mSceneManager->setOption("primaryCamera", camera);
	}
	
	void EmberPagingSceneManagerAdapter::setResourceGroupName(const std::string& groupName)
	{
		mSceneManager->setOption("GroupName", &groupName);
		mSceneManager->getOptions()->groupName = groupName;
		mSceneManager->getOptions()->cfgGroupName = groupName;
	}
	
	void EmberPagingSceneManagerAdapter::loadOptions(const std::string& filePath)
	{
		mSceneManager->getOptions()->loadMapOptions(filePath);
		
		mSceneManager->getOptions()->setTextureFormat("EmberTexture");
	}
	
	void EmberPagingSceneManagerAdapter::loadScene()
	{
		mSceneManager->loadScene();
		mSceneManager->getOptions()->setOption("LoadNow", 0);
	}

	void EmberPagingSceneManagerAdapter::setOption(const std::string& strKey, const void* pValue)
	{
		mSceneManager->setOption(strKey, pValue);
	}
	
	void EmberPagingSceneManagerAdapter::getOption(const std::string& strKey, void* pDestValue)
	{
		mSceneManager->getOption(strKey, pDestValue);
	}

	Ogre::SceneManager* EmberPagingSceneManagerAdapter::getSceneManager() const
	{
		return mSceneManager;
	}

	void EmberPagingSceneManagerAdapter::reloadAllPages()
	{
		mSceneManager->getPageManager()->load();
	}

	void EmberPagingSceneManagerAdapter::reloadPage(unsigned int x, unsigned int z)
	{
		Ogre::Vector2 position(x,z);
		setOption("PageUpdate", &position);
		Ogre::PagingLandScapePage* page=  mSceneManager->getPageManager()->getPage(x, z, false);
		if (page) {
//			page->reload();
			page->unload();
			page->load();
		}
	}

}
