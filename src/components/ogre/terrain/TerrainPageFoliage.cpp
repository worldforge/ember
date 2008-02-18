//
// C++ Implementation: TerrainPageFoliage
//
// Description: 
//
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

#include <Mercator/Surface.h>
#include <Mercator/Segment.h>
#include <Mercator/Terrain.h>

#ifdef HAVE_LRINTF
    #define I_ROUND(_x) (::lrintf(_x)) 
#elif defined(HAVE_RINTF)
    #define I_ROUND(_x) ((int)::rintf(_x)) 
#elif defined(HAVE_RINT)
    #define I_ROUND(_x) ((int)::rint(_x)) 
#else
    #define I_ROUND(_x) ((int)(_x)) 
#endif

#ifdef HAVE_FABSF
    #define F_ABS(_x) (::fabsf(_x))
#else
    #define F_ABS(_x) (::fabs(_x))
#endif

namespace EmberOgre {

namespace Terrain {

TerrainPageFoliage::TerrainPageFoliage(TerrainGenerator& generator, TerrainPage& page)
: mGenerator(generator)
, mTerrainPage(page)
, mFoliageCoverageDataStream(0)
, mCoverageMapPixelWidth(mTerrainPage.getAlphaTextureSize())
{
}

TerrainPageFoliage::~TerrainPageFoliage()
{
///no need to delete the stream since that will be taken care of by the mFoliageCoverageDataStreamPtr instance
// 	delete mFoliageCoverageDataStream;
}


void TerrainPageFoliage::generatePlantPositions()
{
	
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
		float clusterX = Ogre::Math::RangeRandom(0, mCoverageMapPixelWidth);
		float clusterY = Ogre::Math::RangeRandom(0, mCoverageMapPixelWidth);
		float clusterRadius = Ogre::Math::RangeRandom(minClusterRadius, maxClusterRadius);
		
		float volume = (clusterRadius * clusterRadius) * Ogre::Math::PI;
		unsigned int instancesInEachCluster = volume / density;

		///place one cluster
		for (unsigned int j = 0; j < instancesInEachCluster; ++j) {
			float plantX = Ogre::Math::RangeRandom(-clusterRadius, clusterRadius) + clusterX;
			float plantY = Ogre::Math::RangeRandom(-clusterRadius, clusterRadius) + clusterY;
			
			if (plantX >= 0 && plantX < mCoverageMapPixelWidth && plantY >= 0 && plantY < mCoverageMapPixelWidth) {
				mPlants.push_back(Ogre::Vector2(plantX, plantY));
			}
		}
	}
	S_LOG_VERBOSE("Placed " << mPlants.size() << " plants.");
}

void TerrainPageFoliage::generateCoverageMap()
{
	///we've disable the functionality for keeping a coverage map since it's faster to do the checkups through Mercator
	return;
	
	S_LOG_VERBOSE("Starting generation of foliage coverage map for page at position x: " << mTerrainPage.getWFPosition().x() << " y: " << mTerrainPage.getWFPosition().y() << ".");
	if (!mFoliageCoverageDataStream) {
		mFoliageCoverageDataStream = new Ogre::MemoryDataStream(mCoverageMapPixelWidth * mCoverageMapPixelWidth * 1, true);
		mFoliageCoverageDataStreamPtr = Ogre::DataStreamPtr(mFoliageCoverageDataStream);
	}
	
	size_t foliageBufferSize =  mFoliageCoverageDataStream->size();
	TerrainPageSurfaceLayer* grassLayer(0);
	for (TerrainPageSurface::TerrainPageSurfaceLayerStore::const_iterator I = mTerrainPage.getSurface()->getLayers().begin(); I != mTerrainPage.getSurface()->getLayers().end(); ++I) {
		if (grassLayer) {
			Ogre::Image* layerImage(I->second->getCoverageImage());
			if (foliageBufferSize == layerImage->getSize()) {
 				unsigned char* layerData = layerImage->getData();
				unsigned char* grassLayerData = mFoliageCoverageDataStream->getPtr();
				for (size_t i = 0; i < foliageBufferSize; ++i) {
					if (*layerData) {
						*grassLayerData -= std::min<unsigned char>(*layerData, *grassLayerData);
					}
					layerData++;
					grassLayerData++;
				}
			}
		}
		if (!grassLayer && I->second->getSurfaceIndex() == mGenerator.getFoliageShader()->getTerrainIndex()) {
			if (I->second->getCoverageImage()->getSize() == foliageBufferSize) {
				grassLayer = I->second;
				memcpy(mFoliageCoverageDataStream->getPtr(), grassLayer->getCoverageImage()->getData(), foliageBufferSize);
			}
		}
	}
	
///activate this if you want to see the texture in game (to get debug information etc.)
#if 0
	std::stringstream ss;
	ss << "terrain_" << mTerrainPage.getWFPosition().x() << "_" << mTerrainPage.getWFPosition().y() << "_plantCoverage";
	const Ogre::String textureName(ss.str());
	Ogre::Root::getSingletonPtr()->getTextureManager()->loadRawData(textureName, "General", mFoliageCoverageDataStreamPtr, mCoverageMapPixelWidth, mCoverageMapPixelWidth, Ogre::PF_L8, Ogre::TEX_TYPE_2D, 0);
#endif

	S_LOG_VERBOSE("Ending generation of foliage coverage map.");

}


const TerrainPageFoliage::PlantStore& TerrainPageFoliage::getPlants() const
{
	return mPlants;
}

void TerrainPageFoliage::getPlantsForArea(Ogre::TRect<float> area, TerrainPageFoliage::PlantStore& store)
{
	unsigned char threshold = 100;
	TerrainPosition localPositionInSegment;
	for (PlantStore::iterator I = mPlants.begin(); I != mPlants.end(); ++I) {
		if (I->x >= area.left && I->x <= area.right && I->y >= area.top && I->y <= area.bottom) {
			
			#if 1
			unsigned char combinedCoverage(0);
			float x = I->x;
			float y = mCoverageMapPixelWidth - I->y;
		
			Mercator::Segment* segment = mTerrainPage.getSegmentAtLocalPosition(TerrainPosition(x, y), localPositionInSegment);
			if ((segment == 0) || (!segment->isValid())) {
				continue;
			}
			
			TerrainPageSurfaceLayer* grassLayer(0);
			for (TerrainPageSurface::TerrainPageSurfaceLayerStore::const_iterator J = mTerrainPage.getSurface()->getLayers().begin(); J != mTerrainPage.getSurface()->getLayers().end(); ++J) {
				if (grassLayer) {
					Mercator::Surface* surface = J->second->getSurfaceForSegment(segment);
					if (surface && surface->isValid()) {
						unsigned char localCoverage((*surface)(localPositionInSegment.x(), localPositionInSegment.y(), 0));
						combinedCoverage -= std::min<unsigned char>(localCoverage, combinedCoverage);
					}
				} else if (!grassLayer && J->second->getSurfaceIndex() == mGenerator.getFoliageShader()->getTerrainIndex()) {
					Mercator::Surface* surface = J->second->getSurfaceForSegment(segment);
					if (surface && surface->isValid()) {
						combinedCoverage = (*surface)(localPositionInSegment.x(), localPositionInSegment.y(), 0);
						if (combinedCoverage >= threshold) {
							grassLayer = J->second;
						} else {
							break;
						}
					} else {
						break;
					}
				}
			}
			if (combinedCoverage >= threshold) {
				store.push_back(*I);
			}
			
			#endif
			
			
			#if 0
			///use the combined coverage map
			size_t position = static_cast<size_t>((mCoverageMapPixelWidth * static_cast<unsigned int>(I->y)) + static_cast<unsigned int>(I->x));
			unsigned char val(mFoliageCoverageDataStream->getPtr()[position]);
			if (val >= threshold) {
				store.push_back(*I);
			}
			#endif
		}
	}
}


}

}
