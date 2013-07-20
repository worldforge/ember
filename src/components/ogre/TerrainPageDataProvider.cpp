/*
 Copyright (C) 2009 erik

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

#include "TerrainPageDataProvider.h"
#include "components/ogre/terrain/TerrainHandler.h"
#include "components/ogre/terrain/TerrainInfo.h"
#include "components/ogre/terrain/TerrainPage.h"
#include "Convert.h"

namespace Ember
{
namespace OgreView
{
TerrainPageData::TerrainPageData(Terrain::TerrainPage* page) :
		mPage(page)
{
}

TerrainPageData::~TerrainPageData()
{
}

Ogre::MaterialPtr TerrainPageData::getMaterial()
{
	if (mPage) {
		return mPage->getMaterial();
	}
	return Ogre::MaterialPtr();
}

TerrainPageDataProvider::TerrainPageDataProvider(Terrain::TerrainHandler& handler) :
		mHandler(handler)
{

}

TerrainPageDataProvider::~TerrainPageDataProvider()
{
}

IPageData* TerrainPageDataProvider::getPageData(const OgreIndex& ogreIndexPosition)
{
	return new TerrainPageData(mHandler.getTerrainPageAtIndex(convertToWFTerrainIndex(ogreIndexPosition)));
}

int TerrainPageDataProvider::getPageIndexSize() const
{
	return mHandler.getPageIndexSize();
}
void TerrainPageDataProvider::setUpTerrainPageAtIndex(const OgreIndex& ogreIndexPosition, Terrain::ITerrainPageBridge* bridge)
{
	mHandler.setUpTerrainPageAtIndex(convertToWFTerrainIndex(ogreIndexPosition), bridge);
}

void TerrainPageDataProvider::removeBridge(const OgreIndex& ogreIndexPosition)
{
	mHandler.removeBridge(convertToWFTerrainIndex(ogreIndexPosition));
}

Domain::TerrainIndex TerrainPageDataProvider::convertToWFTerrainIndex(const OgreIndex& ogreIndexPosition)
{
	return Domain::TerrainIndex(ogreIndexPosition.first, ogreIndexPosition.second);
}

}
}
