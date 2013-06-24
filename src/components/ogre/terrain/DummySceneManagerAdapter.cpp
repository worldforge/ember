/*
 Copyright (C) 2013 Samuel Kogler

 This program is free software; you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation; either version 2 of the License, or
 (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program; if not, write to the Free Software
 Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#include "DummySceneManagerAdapter.h"
#include "DummyTerrainObserver.h"

#include <string>
#include <OgreAxisAlignedBox.h>
#include <OgreSceneManager.h>

namespace Ember {
namespace OgreView {
namespace Terrain {

DummySceneManagerAdapter::DummySceneManagerAdapter(Ogre::SceneManager& sceneManager)
: mSceneManager(sceneManager)
{
}

DummySceneManagerAdapter::~DummySceneManagerAdapter()
{
}

int DummySceneManagerAdapter::getPageSize()
{
	//FIXME SK: need to return at least 64 here, otherwise terraininfo will crash
	return 128;
}

Ogre::Real DummySceneManagerAdapter::getHeightAt(const Ogre::Real x, const Ogre::Real z)
{
	return Ogre::Real(100.0f);
}

void DummySceneManagerAdapter::setWorldPagesDimensions(int numberOfPagesWidth, int numberOfPagesHeight, int widthOffsetInPages, int heightOffsetInPages)
{
}

void DummySceneManagerAdapter::setCamera(Ogre::Camera* camera)
{
}

void DummySceneManagerAdapter::setResourceGroupName(const std::string& groupName)
{
}

void DummySceneManagerAdapter::loadOptions(const std::string& filePath)
{
}

void DummySceneManagerAdapter::resize(Ogre::AxisAlignedBox newSize, int levels)
{
}

void DummySceneManagerAdapter::loadScene()
{
}

void DummySceneManagerAdapter::reset()
{
}

void DummySceneManagerAdapter::setOption(const std::string& strKey, const void* pValue)
{
}

void DummySceneManagerAdapter::getOption(const std::string& strKey, void* pDestValue)
{
}

void DummySceneManagerAdapter::setUninitializedHeight(float height)
{
}

Ogre::SceneManager& DummySceneManagerAdapter::getSceneManager() const
{
	return mSceneManager;
}

void DummySceneManagerAdapter::reloadAllPages()
{
}

void DummySceneManagerAdapter::reloadPage(unsigned int x, unsigned int z)
{
}

void DummySceneManagerAdapter::loadFirstPage()
{
}

std::string DummySceneManagerAdapter::getDebugInfo()
{
	return std::string();
}

ITerrainObserver* DummySceneManagerAdapter::createObserver()
{
	return new DummyTerrainObserver;
}

void DummySceneManagerAdapter::destroyObserver(ITerrainObserver* observer)
{
}

}
}
}



