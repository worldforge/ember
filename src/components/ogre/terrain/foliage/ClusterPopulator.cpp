/*
 * ClusterPopulator.cpp
 *
 *  Created on: 8 mar 2010
 *      Author: Erik Hjortsberg
 */

#include "ClusterPopulator.h"
#include "components/ogre/terrain/PlantAreaQueryResult.h"
#include "components/ogre/terrain/PlantAreaQuery.h"
#include "components/ogre/terrain/SegmentReference.h"
#include "components/ogre/terrain/Segment.h"
#include "components/ogre/terrain/Buffer.h"
#include "components/ogre/terrain/PlantInstance.h"
#include "components/ogre/Convert.h"
#include <wfmath/ball.h>
#include <wfmath/intersect.h>
#include <wfmath/randgen.h>
#include <cmath>
#include <boost/shared_ptr.hpp>

#include <Mercator/Segment.h>
#include <Mercator/Surface.h>
#include <Mercator/Shader.h>
namespace EmberOgre
{

namespace Terrain
{

namespace Foliage
{

ClusterPopulator::ClusterPopulator(unsigned int layerIndex, IScaler* scaler, unsigned int plantIndex) :
	PlantPopulator::PlantPopulator(layerIndex, scaler, plantIndex), mThreshold(0)
{
}

ClusterPopulator::~ClusterPopulator()
{
}

void ClusterPopulator::populate(PlantAreaQueryResult& result, SegmentRefPtr segmentRef)
{
	Mercator::Segment& mercatorSegment = segmentRef->getSegment().getMercatorSegment();
	if (!mercatorSegment.isValid()) {
		mercatorSegment.populate();
	}

	const PlantAreaQuery& query = result.getQuery();
	PlantAreaQueryResult::PlantStore& plants = result.getStore();

	const WFMath::AxisBox<2>& area = Convert::toWF(query.getArea());

	//Make a small list of surfaces in order
	std::list<int> indexSort;
	for (Mercator::Segment::Surfacestore::const_iterator I = mercatorSegment.getSurfaces().begin(); I != mercatorSegment.getSurfaces().end(); ++I) {
		if (I->first >= mLayerIndex) {
			if (I->second->m_shader.checkIntersect(mercatorSegment)) {
				if (!I->second->isValid()) {
					I->second->populate();
				}
				indexSort.push_back(I->first);
			}
		}
	}

	indexSort.sort();
	//Check that there actually is a valid surface on which the plants can be placed
	if (indexSort.size() > 0 && indexSort.front() == mLayerIndex) {
		Buffer<unsigned char> combinedCoverage(mercatorSegment.getSize(), 1);
		unsigned char* combinedCoverageData = combinedCoverage.getData();
		size_t size = combinedCoverage.getSize();
		//The first layer should be copied just as it is
		std::list<int>::const_iterator I = indexSort.begin();
		{
			Mercator::Surface* surface = mercatorSegment.getSurfaces()[*I];
			memcpy(combinedCoverageData, surface->getData(), combinedCoverage.getSize());
		}
		++I;
		for (; I != indexSort.end(); ++I) {
			Mercator::Surface* surface = mercatorSegment.getSurfaces()[*I];
			unsigned char* surfaceData = surface->getData();
			for (size_t i = 0; i < size; ++i) {
				combinedCoverageData[i] -= std::min<unsigned char>(surfaceData[i], combinedCoverageData[i]);
			}
		}

		ClusterStore store;
		getClustersForArea(segmentRef, area, store);
		populateWithClusters(segmentRef, result, area, store, combinedCoverage);
	}
}

void ClusterPopulator::getClustersForArea(const SegmentRefPtr& segmentRef, const WFMath::AxisBox<2>& area, ClusterStore& store)
{
	//Generate clusters for the current page and all surrounding pages and check if any of these are contained or intersect our local area

	Mercator::Segment& mercatorSegment = segmentRef->getSegment().getMercatorSegment();
	int res = mercatorSegment.getResolution();
	int clustersPerSegment = (res * res) / (mClusterDistance * mClusterDistance);
	float clusterRadiusRange = mMaxClusterRadius - mMinClusterRadius;
	float xRef = mercatorSegment.getXRef();
	float yRef = mercatorSegment.getYRef();

	WFMath::MTRand rng;

	for (int i = -1; i < 2; ++i) {
		for (int j = -1; j < 2; ++j) {
			int currentSegmentX = xRef + (i * res);
			int currentSegmentY = yRef + (j * res);
			WFMath::MTRand::uint32 seed(mPlantIndex + (static_cast<WFMath::MTRand::uint32> (currentSegmentX) << 4) + (static_cast<WFMath::MTRand::uint32> (currentSegmentY) << 8));
			rng.seed(seed);
			for (int k = 0; k < clustersPerSegment; ++k) {
				WFMath::Ball<2> cluster(WFMath::Point<2>(rng.rand(res) + currentSegmentX, rng.rand(res) + currentSegmentY), rng.rand(clusterRadiusRange) + mMinClusterRadius);
				if (WFMath::Contains(area, cluster.center(), true) || WFMath::Intersect(area, cluster, true)) {
					store.push_back(cluster);
				}
			}
		}
	}
}

void ClusterPopulator::populateWithClusters(const SegmentRefPtr& segmentRef, PlantAreaQueryResult& result, const WFMath::AxisBox<2>& area, const ClusterStore& clusters, const Buffer<unsigned char>& combinedCoverage)
{
	for (ClusterStore::const_iterator I = clusters.begin(); I != clusters.end(); ++I) {
		populateWithCluster(segmentRef, result, area, *I, combinedCoverage);
	}

}

void ClusterPopulator::populateWithCluster(const SegmentRefPtr& segmentRef, PlantAreaQueryResult& result, const WFMath::AxisBox<2>& area, const WFMath::Ball<2>& cluster, const Buffer<unsigned char>& combinedCoverage)
{
	const PlantAreaQuery& query = result.getQuery();
	PlantAreaQueryResult::PlantStore& plants = result.getStore();
	Mercator::Segment& mercatorSegment = segmentRef->getSegment().getMercatorSegment();

	float volume = (cluster.radius() * cluster.radius()) * WFMath::Pi;
	unsigned int instancesInEachCluster = volume * mDensity;
	WFMath::MTRand::uint32 seed(mPlantIndex + (static_cast<WFMath::MTRand::uint32> (cluster.center().x()) << 4) + (static_cast<WFMath::MTRand::uint32> (cluster.center().y()) << 8));
	WFMath::MTRand rng(seed);

	unsigned int res = combinedCoverage.getResolution();
	const unsigned char* data = combinedCoverage.getData();

	float height = 0;
	WFMath::Vector<3> normal;
	///place one cluster
	for (unsigned int j = 0; j < instancesInEachCluster; ++j) {
		float theta = rng.rand(WFMath::Pi * 2);
		float length = rng.rand(mMaxClusterRadius);

		WFMath::Point<2> pos(cos(theta) * length, sin(theta) * length);
		pos.shift(WFMath::Vector<2>(cluster.getCenter()));
		float rotation = rng.rand(360.0);
		Ogre::Vector2 scale;
		mScaler->scale(rng, pos, scale);

		if (WFMath::Contains(area, pos, true)) {
			WFMath::Point<2> localPos(pos.x() - mercatorSegment.getXRef(), pos.y() - mercatorSegment.getYRef());
			if (data[((unsigned int)localPos.y() * res) + ((unsigned int)localPos.x())] >= mThreshold) {
				mercatorSegment.getHeightAndNormal(localPos.x(), localPos.y(), height, normal);
				plants.push_back(PlantInstance(Ogre::Vector3(pos.x(), height, -pos.y()), rotation, scale));
			}
		}
	}
}

float ClusterPopulator::getMinClusterRadius() const
{
	return mMinClusterRadius;
}

void ClusterPopulator::setMinClusterRadius(float theValue)
{
	mMinClusterRadius = theValue;
}

float ClusterPopulator::getMaxClusterRadius() const
{
	return mMaxClusterRadius;
}

void ClusterPopulator::setMaxClusterRadius(float theValue)
{
	mMaxClusterRadius = theValue;
}

float ClusterPopulator::getDensity() const
{
	return mDensity;
}

void ClusterPopulator::setDensity(float theValue)
{
	mDensity = theValue;
}

float ClusterPopulator::getFalloff() const
{
	return mFalloff;
}

void ClusterPopulator::setFalloff(float theValue)
{
	mFalloff = theValue;
}

float ClusterPopulator::getClusterDistance() const
{
	return mClusterDistance;
}

void ClusterPopulator::setClusterDistance(float theValue)
{
	mClusterDistance = theValue;
}

void ClusterPopulator::setThreshold(unsigned char theValue)
{
	mThreshold = theValue;
}

float ClusterPopulator::getTreshold() const
{
	return mThreshold;
}

}

}

}
