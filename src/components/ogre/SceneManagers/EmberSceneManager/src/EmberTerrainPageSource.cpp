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
#include "components/ogre/EmberOgrePrerequisites.h"
#include "components/ogre/TerrainPage.h"

using namespace Ogre;
namespace EmberOgre {

const std::string EmberTerrainPageSource::Name = "EmberTerrain";
	
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

void EmberTerrainPageSource::addPage(TerrainPage* page)
{
	Ogre::Real heightData[page->getVerticeCount()];
	page->createHeightData(heightData);
	
	Ogre::Vector3 vector = page->getOgrePosition();
		
	Ogre::TerrainPage* ogrePage = buildPage(heightData, page->getMaterial());
		
	getEmberTerrainSceneManager()->attachPage((long)vector.x, (long)vector.z, ogrePage, page->getMaxHeight(), page->getMinHeight());

}

// void EmberTerrainPageSource::generatePage(TerrainPosition& point)
// {
// 	Ogre::Material* material = mGenerator->getMaterialForSegment(point);
// 	if (material == 0) {
// 		std::cerr << "No material found for page at position: " << point << std::endl;
// 	} else {
// 		Ogre::Vector3 vector = Atlas2Ogre(point);
// 		//we have to do this in order to align the pages correctly
// 		vector.z = vector.z - 1;
// 		int arraySize = (mPageSize) *(mPageSize);
// 		Ogre::Real heightData[arraySize] ;
// 		long xStart = (long)vector.x * (mPageSize - 1);
// 		long zStart = (long)vector.z * (mPageSize - 1);
// 		long xIter, zIter;
// 		int pointer = 0;
// 		
// 		for (zIter = zStart; zIter < (zStart + mPageSize); ++zIter) {
// 			for (xIter = xStart; xIter < (xStart + mPageSize); ++xIter) {
// 				Ogre::Vector3 _pos(xIter, 0, zIter);
// 				TerrainPosition pos = Ogre2Atlas_TerrainPosition(_pos);
// 				Ogre::Real height = mGenerator->getHeight(pos);
// 				heightData[pointer++] = height / 100.0;
// 			}
// 		}
// 		TerrainPage* page = buildPage(heightData, material);
// 	/*	page->tiles[0][0]->setCastShadows(false);
// 		page->tiles[0][1]->setCastShadows(false);
// 		page->tiles[1][0]->setCastShadows(false);
// 		page->tiles[1][1]->setCastShadows(false);*/
// 		
// 		getEmberTerrainSceneManager()->attachPage((long)vector.x, (long)vector.z, page, mGenerator->getMaxHeightForSegment(point), mGenerator->getMinHeightForSegment(point));
// 	}
// 	
// }

void EmberTerrainPageSource::setHasTerrain(bool hasTerrain)
{
	//TODO: check with the maximum defined terrain first so we won't get any segfaults
	mHasTerrain = hasTerrain;	
// 	generatePage(0,0);
// 	generatePage(0,1);
// 	generatePage(-1,0);
// 	generatePage(-1,1);
// 	resizeTerrain();
// 	return;
// //	generatePage(-1, -1);
// 	int size = 1;
// //	this is temporary until we've got paging working
// 	for (int i = 0 - size; i <= 0 + size; ++i) {
// 		for (int j = 0 - size; j <= 0 + size; ++j) {
// 			mX = i;
// 			mZ = j;
// 			generatePage(i, j);
// 
// 		}
// 	}
// 	resizeTerrain();
	
}

// bool EmberTerrainPageSource::pushPage(TerrainPosition& point )
// {
// 	if (mGenerator->isValidTerrainAt(point)) {
// 		generatePage(point);
// 	} else {
// 		return false;
// 	}
// 	return true;
// }

void EmberTerrainPageSource::resizeTerrain()
{
	getEmberTerrainSceneManager()->doResize();
}



}
