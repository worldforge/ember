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
#include "TerrainGenerator.h"

#include "DimeTerrainPageSource.h"

using namespace Ogre;
namespace DimeOgre {
	
DimeTerrainPageSource::DimeTerrainPageSource(TerrainGenerator* const generator ) : 
mGenerator(generator)
, mHasTerrain(false)
, mX(0)
, mZ(0)
{}
DimeTerrainPageSource::~DimeTerrainPageSource()
{}


void DimeTerrainPageSource::requestPage(Ogre::ushort x, Ogre::ushort z) 
{
	if (mHasTerrain) {
		for (int i = x - 1; i <= x + 1; ++i) {
			for (int j = z - 1; j <= z + 1; ++j) {
				mX = i;
				mZ = j;
				generatePage(i, j);

			}
		}
	}
}

void DimeTerrainPageSource::expirePage(Ogre::ushort x, Ogre::ushort z)
{
}

void DimeTerrainPageSource::generatePage(int x, int y)
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
	TerrainPage* page = buildPage(heightData, mGenerator->getMaterialForSegment(x, -y - 1 ));
	mSceneManager->attachPage(x, y, page);
	
}

void DimeTerrainPageSource::setHasTerrain(bool hasTerrain)
{
	mHasTerrain = hasTerrain;	
}


}
