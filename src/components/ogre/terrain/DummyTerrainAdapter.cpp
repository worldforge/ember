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

#include "DummyTerrainAdapter.h"
#include "DummyTerrainObserver.h"

#include <string>
#include <OgreAxisAlignedBox.h>
#include <OgreSceneManager.h>

namespace Ember {
namespace OgreView {
namespace Terrain {

DummyTerrainAdapter::DummyTerrainAdapter(Ogre::SceneManager& sceneManager)
: mSceneManager(sceneManager)
{
}

DummyTerrainAdapter::~DummyTerrainAdapter()
{
}

int DummyTerrainAdapter::getPageSize()
{
	//FIXME SK: need to return at least 64 here, otherwise terraininfo will crash
	return 128;
}

Ogre::Real DummyTerrainAdapter::getHeightAt(const Ogre::Real x, const Ogre::Real z)
{
	return Ogre::Real(100.0f);
}

void DummyTerrainAdapter::setWorldPagesDimensions(int numberOfPagesWidth, int numberOfPagesHeight, int widthOffsetInPages, int heightOffsetInPages)
{
}

void DummyTerrainAdapter::setCamera(Ogre::Camera* camera)
{
}

void DummyTerrainAdapter::setResourceGroupName(const std::string& groupName)
{
}

void DummyTerrainAdapter::loadOptions(const std::string& filePath)
{
}

void DummyTerrainAdapter::resize(Ogre::AxisAlignedBox newSize, int levels)
{
}

void DummyTerrainAdapter::loadScene()
{
}

void DummyTerrainAdapter::reset()
{
}

void DummyTerrainAdapter::setOption(const std::string& strKey, const void* pValue)
{
}

void DummyTerrainAdapter::getOption(const std::string& strKey, void* pDestValue)
{
}

void DummyTerrainAdapter::setUninitializedHeight(float height)
{
}

void DummyTerrainAdapter::reloadAllPages()
{
}

void DummyTerrainAdapter::reloadPage(unsigned int x, unsigned int z)
{
}

void DummyTerrainAdapter::loadFirstPage()
{
}

std::string DummyTerrainAdapter::getDebugInfo()
{
	return std::string();
}

ITerrainObserver* DummyTerrainAdapter::createObserver()
{
	return new DummyTerrainObserver;
}

void DummyTerrainAdapter::destroyObserver(ITerrainObserver* observer)
{
}

std::pair<bool, Ogre::Vector3> DummyTerrainAdapter::rayIntersects(Ogre::Ray ray) const
{
	//TODO SK: return something meaningful
	return std::pair<bool, Ogre::Vector3>(true, ray.getDirection() + 100);
}

}
}
}



