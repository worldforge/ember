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
#include "components/ogre/terrain/TerrainPage.h"

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

IPageData* TerrainPageDataProvider::getPageData(const Ogre::Vector2& position)
{
	return new TerrainPageData(mManager.getTerrainPageAtIndex(position));
}

int TerrainPageDataProvider::getPageIndexSize() const
{
	return mManager.getPageIndexSize();
}
void TerrainPageDataProvider::setUpTerrainPageAtIndex(const Ogre::Vector2& ogreIndexPosition, Terrain::ITerrainPageBridge& bridge)
{
	mManager.setUpTerrainPageAtIndex(ogreIndexPosition, bridge);
}

}
