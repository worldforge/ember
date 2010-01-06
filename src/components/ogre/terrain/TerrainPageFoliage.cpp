//
// C++ Implementation: TerrainPageFoliage
//
// Description:
//
// Author: Erik Hjortsberg <erik.hjortsberg@gmail.com>, (C) 2008
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
#include "TerrainManager.h"
#include "TerrainShader.h"
#include "TerrainPageGeometry.h"
#include "PlantAreaQuery.h"
#include "PlantAreaQueryResult.h"

#include "components/ogre/Convert.h"

#include "framework/LoggingInstance.h"
#include "services/config/ConfigService.h"

#include <Mercator/Surface.h>
#include <Mercator/Segment.h>
#include <Mercator/Terrain.h>

#include <wfmath/MersenneTwister.h>

#include <OgreMath.h>
#include <OgreVector2.h>

namespace EmberOgre
{

namespace Terrain
{

TerrainPageFoliage::TerrainPageFoliage(TerrainManager& manager, TerrainPage& page) :
	mManager(manager), mTerrainPage(page), mFoliageCoverageDataStream(0), mCoverageMapPixelWidth(mTerrainPage.getAlphaTextureSize())
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
	unsigned int foliageBatchSize = mManager.getFoliageBatchSize();
	for (TerrainLayerDefinitionManager::DefinitionStore::const_iterator I = TerrainLayerDefinitionManager::getSingleton().getDefinitions().begin(); I != TerrainLayerDefinitionManager::getSingleton().getDefinitions().end(); ++I) {
		const TerrainLayerDefinition* layerDef = *I;
		for (TerrainLayerDefinition::TerrainFoliageDefinitionStore::const_iterator I = layerDef->getFoliages().begin(); I != layerDef->getFoliages().end(); ++I) {
			PlantBatchStore& plants = mPlantStores[I->getPlantType()];

			if (I->getPopulationTechnique() == "cluster") {
				ClusterPopulator populator(*this);
				populator.setClusterDistance(atof(I->getParameter("clusterDistance").c_str()));
				populator.setMinClusterRadius(atof(I->getParameter("minClusterRadius").c_str()));
				populator.setMaxClusterRadius(atof(I->getParameter("maxClusterRadius").c_str()));
				populator.setDensity(atof(I->getParameter("density").c_str()));
				populator.setFalloff(atof(I->getParameter("falloff").c_str()));

				populator.populate(plants, index++, foliageBatchSize);
			}
		}
	}
}

void TerrainPageFoliage::generateCoverageMap()
{
	///we've disable the functionality for keeping a coverage map since it's faster to do the checkups through Mercator
	return;
#if 0
	S_LOG_VERBOSE("Starting generation of foliage coverage map for page at position x: " << mTerrainPage.getWFPosition().x() << " y: " << mTerrainPage.getWFPosition().y() << ".");
	if (!mFoliageCoverageDataStream) {
		mFoliageCoverageDataStream = new Ogre::MemoryDataStream(mCoverageMapPixelWidth * mCoverageMapPixelWidth * 1, true);
		mFoliageCoverageDataStreamPtr = Ogre::DataStreamPtr(mFoliageCoverageDataStream);
	}

	size_t foliageBufferSize = mFoliageCoverageDataStream->size();
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
		if (!grassLayer && I->second->getSurfaceIndex() == mManager.getFoliageShader()->getTerrainIndex()) {
			if (I->second->getCoverageImage()->getSize() == foliageBufferSize) {
				grassLayer = I->second;
				memcpy(mFoliageCoverageDataStream->getPtr(), grassLayer->getCoverageImage()->getData(), foliageBufferSize);
			}
		}
	}
#endif

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

void TerrainPageFoliage::getPlantsForArea(const TerrainPageGeometry& geometry, PlantAreaQueryResult& queryResult) const
{
	const PlantAreaQuery& query = queryResult.getQuery();
	PlantAreaQueryResult::PlantStore& store = queryResult.getStore();
	//const PlantBatchStore& plantBatchStore = mPlantStores[plantType];
	PlantStoreMap::const_iterator plantStoreMapIt = mPlantStores.find(query.getPlantType());
	if (plantStoreMapIt == mPlantStores.end()) {
		S_LOG_WARNING("Plant type '" << query.getPlantType() << "' doesn't exist in PlantStore");
		return;
	}

	Ogre::TRect<float> ogrePageExtent = Convert::toOgre(mTerrainPage.getExtent());
	Ogre::TRect<float> adjustedBounds = Ogre::TRect<float>(query.getArea().left - ogrePageExtent.left, query.getArea().top - ogrePageExtent.top, query.getArea().right - ogrePageExtent.left, query.getArea().bottom - ogrePageExtent.top);

	TerrainPosition localPositionInSegment;
	const int batchX = Ogre::Math::Floor(adjustedBounds.left / mManager.getFoliageBatchSize());
	const int batchY = Ogre::Math::Floor(adjustedBounds.top / mManager.getFoliageBatchSize());

	//const PlantStore& plants = plantBatchStore[batchX][batchY];
	PlantBatchStore::const_iterator plantBatchStoreIt = (*plantStoreMapIt).second.find(batchX);
	//Note that the plantBatchStore is parsely populated, so we expect that there will be batches missing (i.e. no plants have been placed there).
	if (plantBatchStoreIt == (*plantStoreMapIt).second.end()) {
		return;
	}
	PlantBatchColumn::const_iterator plantBatchColumnIt = (*plantBatchStoreIt).second.find(batchY);
	if (plantBatchColumnIt == (*plantBatchStoreIt).second.end()) {
		return;
	}
	const PlantStore& plants = (*plantBatchColumnIt).second;
	store.reserve(plants.size());

	float height;
	WFMath::Vector<3> normal;
	for (PlantStore::const_iterator I = plants.begin(); I != plants.end(); ++I) {
		if (I->x >= adjustedBounds.left && I->x <= adjustedBounds.right && I->y >= adjustedBounds.top && I->y <= adjustedBounds.bottom) {

#if 1
			unsigned char combinedCoverage(0);
			float x = I->x;
			float y = mCoverageMapPixelWidth - I->y;

			const Mercator::Segment* segment = geometry.getSegmentAtLocalPosition(TerrainPosition(x, y), localPositionInSegment);
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
						unsigned char localCoverage((*surface)(static_cast<unsigned int> (localPositionInSegment.x()), static_cast<unsigned int> (localPositionInSegment.y()), 0));
						combinedCoverage -= std::min<unsigned char>(localCoverage, combinedCoverage);
					}
				} else if (!activeLayer && &currentLayerDef == &query.getLayerDef()) {
					Mercator::Surface* surface = J->second->getSurfaceForSegment(segment);
					if (surface && surface->isValid()) {
						combinedCoverage = (*surface)(static_cast<unsigned int> (localPositionInSegment.x()), static_cast<unsigned int> (localPositionInSegment.y()), 0);
						if (combinedCoverage >= query.getThreshold()) {
							activeLayer = J->second;
						} else {
							break;
						}
					} else {
						break;
					}
				}
			}
			if (combinedCoverage >= query.getThreshold()) {
				segment->getHeightAndNormal(localPositionInSegment.x(), localPositionInSegment.y(), height, normal);
				store.push_back(Ogre::Vector3(ogrePageExtent.left + I->x, height, ogrePageExtent.top + I->y));
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

void TerrainPageFoliage::setupBatches()
{
}

}

}

float EmberOgre::Terrain::ClusterPopulator::getMinClusterRadius() const
{
	return mMinClusterRadius;
}

void EmberOgre::Terrain::ClusterPopulator::setMinClusterRadius(float theValue)
{
	mMinClusterRadius = theValue;
}

float EmberOgre::Terrain::ClusterPopulator::getMaxClusterRadius() const
{
	return mMaxClusterRadius;
}

void EmberOgre::Terrain::ClusterPopulator::setMaxClusterRadius(float theValue)
{
	mMaxClusterRadius = theValue;
}

float EmberOgre::Terrain::ClusterPopulator::getDensity() const
{
	return mDensity;
}

void EmberOgre::Terrain::ClusterPopulator::setDensity(float theValue)
{
	mDensity = theValue;
}

float EmberOgre::Terrain::ClusterPopulator::getFalloff() const
{
	return mFalloff;
}

void EmberOgre::Terrain::ClusterPopulator::setFalloff(float theValue)
{
	mFalloff = theValue;
}

float EmberOgre::Terrain::ClusterPopulator::getClusterDistance() const
{
	return mClusterDistance;
}

void EmberOgre::Terrain::ClusterPopulator::setClusterDistance(float theValue)
{
	mClusterDistance = theValue;
}

EmberOgre::Terrain::PlantPopulator::PlantPopulator(const TerrainPageFoliage & terrainPageFoliage) :
	mTerrainPageFoliage(terrainPageFoliage)
{

}

EmberOgre::Terrain::PlantPopulator::~PlantPopulator()
{
}

EmberOgre::Terrain::ClusterPopulator::ClusterPopulator(const TerrainPageFoliage& terrainPageFoliage) :
	EmberOgre::Terrain::ClusterPopulator::PlantPopulator(terrainPageFoliage)
{
}

EmberOgre::Terrain::ClusterPopulator::~ClusterPopulator()
{
}

void EmberOgre::Terrain::ClusterPopulator::populate(EmberOgre::Terrain::TerrainPageFoliage::PlantBatchStore & plantBatchStore, int plantIndex, unsigned int batchSize)
{
	unsigned int coverageMapPixelWidth(mTerrainPageFoliage.getCoverageMapPixelWidth());
	float clustersPersAxis(coverageMapPixelWidth / mClusterDistance);
	unsigned int clustersPerPage(static_cast<unsigned int> (clustersPersAxis * clustersPersAxis));

	WFMath::MTRand::uint32 seed(plantIndex + (static_cast<WFMath::MTRand::uint32> (mTerrainPageFoliage.getTerrainPage().getWFPosition().x()) << 4) + (static_cast<WFMath::MTRand::uint32> (mTerrainPageFoliage.getTerrainPage().getWFPosition().y()) << 8));

	// 	((mTerrainPageFoliage.getTerrainPage().getWFPosition().x() * mTerrainPageFoliage.getTerrainPage().getWFPosition().x()) + (mTerrainPageFoliage.getTerrainPage().getWFPosition().y() * mTerrainPageFoliage.getTerrainPage().getWFPosition().y() + mTerrainPageFoliage.getTerrainPage().getWFPosition().y())) * (plantIndex * plantIndex * plantIndex * plantIndex) );
	WFMath::MTRand rng(seed);

	unsigned int plantCount(0);
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
						const int batchX = Ogre::Math::Floor(plantX / batchSize);
						const int batchY = Ogre::Math::Floor(plantY / batchSize);
						EmberOgre::Terrain::TerrainPageFoliage::PlantStore& plantStore = plantBatchStore[batchX][batchY];

						plantStore.push_back(Ogre::Vector2(plantX, plantY));
						plantCount++;
					}
				}
			}
		}
	}
	S_LOG_VERBOSE("Placed " << plantCount << " plants.");
}

EmberOgre::Terrain::TerrainPage& EmberOgre::Terrain::TerrainPageFoliage::getTerrainPage() const
{
	return mTerrainPage;
}

unsigned int EmberOgre::Terrain::TerrainPageFoliage::getCoverageMapPixelWidth() const
{
	return mCoverageMapPixelWidth;
}
