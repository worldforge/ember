/*
    Copyright (C) 2004  Erik Hjortsberg

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
#include "components/ogre/TerrainGenerator.h"
#include "EmberTerrainSceneManager.h"

#include "EmberTerrainPageSource.h"

using namespace Ogre;
namespace EmberOgre {
	
EmberTerrainPageSource::EmberTerrainPageSource(TerrainGenerator* const generator ) : 
mGenerator(generator)
, mHasTerrain(false)
, mX(0)
, mZ(0)
{}
EmberTerrainPageSource::~EmberTerrainPageSource()
{}


void EmberTerrainPageSource::requestPage(Ogre::ushort x, Ogre::ushort z) 
{
	if (mHasTerrain) {
		//TODO: implement paging
	}
}

EmberTerrainSceneManager* EmberTerrainPageSource::getEmberTerrainSceneManager() const
{
	return static_cast<EmberTerrainSceneManager*>(mSceneManager);
}


void EmberTerrainPageSource::expirePage(Ogre::ushort x, Ogre::ushort z)
{
}

void EmberTerrainPageSource::generatePage(int x, int y)
{
	int arraySize = (mPageSize) *(mPageSize);
	Ogre::Real heightData[arraySize] ;
	long xStart = x * (mPageSize-1);
	long yStart = y * (mPageSize-1);
	long xIter, yIter;
	int pointer = 0;
	
	for (yIter = yStart; yIter < (yStart + mPageSize); ++yIter) {
		for (xIter = xStart; xIter < (xStart + mPageSize); ++xIter) {
			Ogre::Real height = mGenerator->getHeight(xIter, yIter) / 100;
			heightData[pointer++] = height;
		}
	}
	Ogre::Material* material = mGenerator->getMaterialForSegment(x, -y - 1 );
	assert(material);
	TerrainPage* page = buildPage(heightData, material);
	page->tiles[0][0]->setCastShadows(false);
	page->tiles[0][1]->setCastShadows(false);
	page->tiles[1][0]->setCastShadows(false);
	page->tiles[1][1]->setCastShadows(false);
	
	getEmberTerrainSceneManager()->attachPage(x, y, page, mGenerator->getMaxHeightForSegment(x,y), mGenerator->getMinHeightForSegment(x,y));

	
}

void EmberTerrainPageSource::setHasTerrain(bool hasTerrain)
{
	mHasTerrain = hasTerrain;	

	int size = 2;
	//this is temporary until we've got paging working
	for (int i = 0 - size; i <= 0 + size; ++i) {
		for (int j = 0 - size; j <= 0 + size; ++j) {
			mX = i;
			mZ = j;
			generatePage(i, j);

		}
	}
	resizeTerrain();
	
}

bool EmberTerrainPageSource::pushPage(int x, int y)
{
	if (mGenerator->isValidTerrainAt(x, y)) {
		generatePage(x, y);
	} else {
		return false;
	}
	return true;
}

void EmberTerrainPageSource::resizeTerrain()
{
	getEmberTerrainSceneManager()->doResize();
}



}
