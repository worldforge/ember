//
// C++ Implementation: TerrainPageFoliage
//
// Description: 
//
//#include "TerrainPageSurfaceLayer.h"
#include "TerrainPageSurface.h"
#include "TerrainLayerDefinition.h"

// Author: Erik Hjortsberg <erik.hjortsberg@iteam.se>, (C) 2008
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
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "TerrainPageFoliage.h"
#include "TerrainPage.h"
#include "TerrainPageSurfaceLayer.h"
#include "TerrainPageSurface.h"
#include "TerrainLayerDefinition.h"
#include "TerrainGenerator.h"
#include "TerrainShader.h"

#include "services/EmberServices.h"
#include "services/logging/LoggingService.h"
#include "services/config/ConfigService.h"

namespace EmberOgre {

namespace Terrain {

TerrainPageFoliage::TerrainPageFoliage(TerrainGenerator& generator, TerrainPage& page)
: mGenerator(generator)
, mTerrainPage(page)
, mFoliageCoverageDataStream(0)
{
}

TerrainPageFoliage::~TerrainPageFoliage()
{
	delete mFoliageCoverageDataStream;
}


void TerrainPageFoliage::generatePlantPositions()
{
// 	TerrainShader* grassShader = mGenerator.getFoliageShader();
	
	float minClusterRadius = 2;
	float maxClusterRadius = 10;
	unsigned int clustersPerPage = 400;
	float density = 2.0f;
	float falloff = 0.3;
	
	mPlants.clear();
	
	
	for (unsigned int i = 0; i < clustersPerPage; ++i) {
		///Pick a random position for our cluster
/*		float clusterX = Ogre::Math::RangeRandom(mExtent.lowCorner().x(), mExtent.highCorner().x());
		float clusterY = Ogre::Math::RangeRandom(mExtent.lowCorner().y(), mExtent.highCorner().y());*/
		float clusterX = Ogre::Math::RangeRandom(0, mTerrainPage.getAlphaTextureSize());
		float clusterY = Ogre::Math::RangeRandom(0, mTerrainPage.getAlphaTextureSize());
		float clusterRadius = Ogre::Math::RangeRandom(minClusterRadius, maxClusterRadius);
		
		float volume = (clusterRadius * clusterRadius) * Ogre::Math::PI;
		unsigned int instancesInEachCluster = volume / density;

		///place one cluster
		for (unsigned int j = 0; j < instancesInEachCluster; ++j) {
			float plantX = Ogre::Math::RangeRandom(-clusterRadius, clusterRadius) + clusterX;
			float plantY = Ogre::Math::RangeRandom(-clusterRadius, clusterRadius) + clusterY;
			
			if (plantX >= 0 && plantX < mTerrainPage.getAlphaTextureSize() && plantY >= 0 && plantY < mTerrainPage.getAlphaTextureSize()) {
/*				float val = mFoliageCoverageDataStream->getPtr()[static_cast<size_t>((getAlphaTextureSize() * plantX) + plantY)] / 255.0f;
				if (Ogre::Math::UnitRandom() < val) {*/
					mPlants.push_back(Ogre::Vector2(plantX, plantY));
// 				}
			}
		}
	}
	S_LOG_VERBOSE("Placed " << mPlants.size() << " plants.");
}

void TerrainPageFoliage::generateCoverageMap()
{
	if (!mFoliageCoverageDataStream) {
		mFoliageCoverageDataStream = new Ogre::MemoryDataStream(mTerrainPage.getAlphaTextureSize() * mTerrainPage.getAlphaTextureSize() * 1, true);
	}
	
	size_t foliageBufferSize =  mFoliageCoverageDataStream->size();
	TerrainPageSurfaceLayer* grassLayer(0);
	for (TerrainPageSurface::TerrainPageSurfaceLayerStore::const_iterator I = mTerrainPage.getSurface()->getLayers().begin(); I != mTerrainPage.getSurface()->getLayers().end(); ++I) {
// 		if (grassLayer) {
// 			Ogre::Image* layerImage(I->second->getCoverageImage());
// 			if (foliageBufferSize == layerImage->getSize()) {
//  				unsigned char* layerData = layerImage->getData();
// 				unsigned char* grassLayerData = mFoliageCoverageDataStream->getPtr();
// 				for (size_t i = 0; i < foliageBufferSize; ++i) {
// 					if (*layerData) {
// 						*grassLayerData -= std::min<unsigned char>(*layerData, *grassLayerData);
// 					}
// 					layerData++;
// 					grassLayerData++;
// 				}
// 			}
// 		}
		if (!grassLayer && I->second->getSurfaceIndex() == mGenerator.getFoliageShader()->getTerrainIndex()) {
			if (I->second->getCoverageImage()->getSize() == foliageBufferSize) {
				grassLayer = I->second;
				memcpy(mFoliageCoverageDataStream->getPtr(), grassLayer->getCoverageImage()->getData(), foliageBufferSize);
			}
		}
	}
}


const TerrainPageFoliage::PlantStore& TerrainPageFoliage::getPlants() const
{
	return mPlants;
}

void TerrainPageFoliage::getPlantsForArea(Ogre::TRect<float> area, TerrainPageFoliage::PlantStore& store)
{
	unsigned char threshold = 100;
	for (PlantStore::iterator I = mPlants.begin(); I != mPlants.end(); ++I) {
		if (I->x >= area.left && I->x <= area.right && I->y >= area.top && I->y <= area.bottom) {
			unsigned char val(mFoliageCoverageDataStream->getPtr()[static_cast<size_t>((mTerrainPage.getAlphaTextureSize() * I->y) + I->x)]);
			if (val >= threshold) {
				store.push_back(*I);
			}
		}
	}
}


}

}
