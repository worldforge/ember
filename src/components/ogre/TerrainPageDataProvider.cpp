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
#include "components/ogre/terrain/TerrainManager.h"
#include "components/ogre/terrain/TerrainInfo.h"
#include "components/ogre/terrain/TerrainPage.h"
#include "Convert.h"

namespace EmberOgre
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

TerrainPageDataProvider::TerrainPageDataProvider(Terrain::TerrainManager& manager) :
	mManager(manager)
{

}

TerrainPageDataProvider::~TerrainPageDataProvider()
{
}

IPageData* TerrainPageDataProvider::getPageData(const OgreIndex& ogreIndexPosition)
{
	return new TerrainPageData(mManager.getTerrainPageAtIndex(convertToWFTerrainIndex(ogreIndexPosition)));
}

int TerrainPageDataProvider::getPageIndexSize() const
{
	return mManager.getPageIndexSize();
}
void TerrainPageDataProvider::setUpTerrainPageAtIndex(const OgreIndex& ogreIndexPosition, Terrain::ITerrainPageBridge* bridge)
{
	mManager.setUpTerrainPageAtIndex(convertToWFTerrainIndex(ogreIndexPosition), bridge);
}

void TerrainPageDataProvider::removeBridge(const OgreIndex& ogreIndexPosition)
{
	mManager.removeBridge(convertToWFTerrainIndex(ogreIndexPosition));
}

TerrainIndex TerrainPageDataProvider::convertToWFTerrainIndex(const OgreIndex& ogreIndexPosition)
{
	///TerrainInfo deals with WF space, so we need to flip the x and y offsets here (as it's in Ogre space)
	return TerrainIndex(ogreIndexPosition.first - mManager.getTerrainInfo().getPageOffsetY(), -(ogreIndexPosition.second - mManager.getTerrainInfo().getPageOffsetX()));
}

}
