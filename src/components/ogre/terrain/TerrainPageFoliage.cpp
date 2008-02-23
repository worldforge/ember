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
#include "TerrainLayerDefinitionManager.h"
#include "TerrainGenerator.h"
#include "TerrainShader.h"

#include "services/EmberServices.h"
#include "services/logging/LoggingService.h"
#include "services/config/ConfigService.h"

#include <Mercator/Surface.h>
#include <Mercator/Segment.h>
#include <Mercator/Terrain.h>

#include <wfmath/MersenneTwister.h>

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
	int index(0);
	for (TerrainLayerDefinitionManager::DefinitionStore::const_iterator I = TerrainLayerDefinitionManager::getSingleton().getDefinitions().begin(); I != TerrainLayerDefinitionManager::getSingleton().getDefinitions().end(); ++I) {
		const TerrainLayerDefinition* layerDef = *I;
		for (TerrainLayerDefinition::TerrainFoliageDefinitionStore::const_iterator I = layerDef->getFoliages().begin(); I != layerDef->getFoliages().end(); ++I) {
			PlantStore& plants = mPlantStores[I->getPlantType()];
	
			if (I->getPopulationTechnique() == "cluster") {
				ClusterPopulator populator(*this);
				populator.setClusterDistance(atof(I->getParameter("clusterDistance").c_str()));
				populator.setMinClusterRadius(atof(I->getParameter("minClusterRadius").c_str()));
				populator.setMaxClusterRadius(atof(I->getParameter("maxClusterRadius").c_str()));
				populator.setDensity(atof(I->getParameter("density").c_str()));
				populator.setFalloff(atof(I->getParameter("falloff").c_str()));
				
				populator.populate(plants, index++);
			}
		}
	}
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


const TerrainPageFoliage::PlantStoreMap& TerrainPageFoliage::getPlants() const
{
	return mPlantStores;
}

void TerrainPageFoliage::getPlantsForArea(const TerrainLayerDefinition& layerDef, unsigned char threshold, const std::string& plantType, Ogre::TRect<float> area, TerrainPageFoliage::PlantStore& store)
{
	TerrainPosition localPositionInSegment;
	PlantStore& plants = mPlantStores[plantType];
	for (PlantStore::iterator I = plants.begin(); I != plants.end(); ++I) {
		if (I->x >= area.left && I->x <= area.right && I->y >= area.top && I->y <= area.bottom) {
			
			#if 1
			unsigned char combinedCoverage(0);
			float x = I->x;
			float y = mCoverageMapPixelWidth - I->y;
		
			Mercator::Segment* segment = mTerrainPage.getSegmentAtLocalPosition(TerrainPosition(x, y), localPositionInSegment);
			if ((segment == 0) || (!segment->isValid())) {
				continue;
			}
			
			///start from the coverage for the active layer, and substract all layers above
			///if the end result is below the threshold we'll show the plant
			TerrainPageSurfaceLayer* activeLayer(0);
			for (TerrainPageSurface::TerrainPageSurfaceLayerStore::const_iterator J = mTerrainPage.getSurface()->getLayers().begin(); J != mTerrainPage.getSurface()->getLayers().end(); ++J) {
				const TerrainLayerDefinition& currentLayerDef = J->second->getDefinition();
				if (activeLayer) {
					Mercator::Surface* surface = J->second->getSurfaceForSegment(segment);
					if (surface && surface->isValid()) {
						unsigned char localCoverage((*surface)(static_cast<unsigned int>(localPositionInSegment.x()), static_cast<unsigned int>(localPositionInSegment.y()), 0));
						combinedCoverage -= std::min<unsigned char>(localCoverage, combinedCoverage);
					}
				} else if (!activeLayer && &currentLayerDef == &layerDef) {
					Mercator::Surface* surface = J->second->getSurfaceForSegment(segment);
					if (surface && surface->isValid()) {
						combinedCoverage = (*surface)(static_cast<unsigned int>(localPositionInSegment.x()), static_cast<unsigned int>(localPositionInSegment.y()), 0);
						if (combinedCoverage >= threshold) {
							activeLayer = J->second;
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


float EmberOgre::Terrain::ClusterPopulator::getMinClusterRadius() const
{
	return mMinClusterRadius;
}


void EmberOgre::Terrain::ClusterPopulator::setMinClusterRadius ( float theValue )
{
	mMinClusterRadius = theValue;
}


float EmberOgre::Terrain::ClusterPopulator::getMaxClusterRadius() const
{
	return mMaxClusterRadius;
}


void EmberOgre::Terrain::ClusterPopulator::setMaxClusterRadius ( float theValue )
{
	mMaxClusterRadius = theValue;
}


float EmberOgre::Terrain::ClusterPopulator::getDensity() const
{
	return mDensity;
}


void EmberOgre::Terrain::ClusterPopulator::setDensity ( float theValue )
{
	mDensity = theValue;
}


float EmberOgre::Terrain::ClusterPopulator::getFalloff() const
{
	return mFalloff;
}


void EmberOgre::Terrain::ClusterPopulator::setFalloff ( float theValue )
{
	mFalloff = theValue;
}


float EmberOgre::Terrain::ClusterPopulator::getClusterDistance() const
{
	return mClusterDistance;
}


void EmberOgre::Terrain::ClusterPopulator::setClusterDistance ( float theValue )
{
	mClusterDistance = theValue;
}

EmberOgre::Terrain::PlantPopulator::PlantPopulator(TerrainPageFoliage & terrainPageFoliage)
: mTerrainPageFoliage(terrainPageFoliage)
{
	
}

EmberOgre::Terrain::PlantPopulator::~ PlantPopulator()
{
}

EmberOgre::Terrain::ClusterPopulator::ClusterPopulator(TerrainPageFoliage & terrainPageFoliage)
: EmberOgre::Terrain::ClusterPopulator::PlantPopulator(terrainPageFoliage)
{
}

EmberOgre::Terrain::ClusterPopulator::~ ClusterPopulator()
{
}

void EmberOgre::Terrain::ClusterPopulator::populate(EmberOgre::Terrain::TerrainPageFoliage::PlantStore & plantStore, int plantIndex)
{
	unsigned int coverageMapPixelWidth(mTerrainPageFoliage.getCoverageMapPixelWidth());
	float clustersPersAxis(coverageMapPixelWidth / mClusterDistance);
	unsigned int clustersPerPage(static_cast<unsigned int>(clustersPersAxis * clustersPersAxis));

	WFMath::MTRand::uint32 seed(((mTerrainPageFoliage.getTerrainPage().getWFPosition().x() * mTerrainPageFoliage.getTerrainPage().getWFPosition().x()) + (mTerrainPageFoliage.getTerrainPage().getWFPosition().y() * mTerrainPageFoliage.getTerrainPage().getWFPosition().y() + mTerrainPageFoliage.getTerrainPage().getWFPosition().y())) * (plantIndex * plantIndex * plantIndex * plantIndex) );
	WFMath::MTRand rng(seed);
	
	
	for (unsigned int i = 0; i < clustersPerPage; ++i) {
		///Pick a random position for our cluster
		float clusterX(rng.rand(coverageMapPixelWidth));
		float clusterY(rng.rand(coverageMapPixelWidth));
		float clusterRadius(rng.rand(mMaxClusterRadius - mMinClusterRadius) + mMinClusterRadius);
		
		float volume = (clusterRadius * clusterRadius) * Ogre::Math::PI;
		unsigned int instancesInEachCluster = volume * mDensity;

		///place one cluster
		for (unsigned int j = 0; j < instancesInEachCluster; ++j) {
			float offsetX(rng.rand(clusterRadius * 2) - clusterRadius);
			float offsetY(rng.rand(clusterRadius * 2) - clusterRadius);
			float distanceFromCenter = Ogre::Vector2(offsetX, offsetY).length();
			if (distanceFromCenter < clusterRadius) {
				///the farther from the center the less chance of placement, all depending on the falloff
				float chance(1.0f - (mFalloff * (distanceFromCenter / clusterRadius)));
				if (rng.rand() < chance) {
					float plantX = offsetX + clusterX;
					float plantY = offsetY + clusterY;
					
					if (plantX >= 0 && plantX < coverageMapPixelWidth && plantY >= 0 && plantY < coverageMapPixelWidth) {
						plantStore.push_back(Ogre::Vector2(plantX, plantY));
					}
				}
			}
		}
	}
	S_LOG_VERBOSE("Placed " << plantStore.size() << " plants.");
}


EmberOgre::Terrain::TerrainPage& EmberOgre::Terrain::TerrainPageFoliage::getTerrainPage() const
{
	return mTerrainPage;
}



unsigned int EmberOgre::Terrain::TerrainPageFoliage::getCoverageMapPixelWidth() const
{
	return mCoverageMapPixelWidth;
}
