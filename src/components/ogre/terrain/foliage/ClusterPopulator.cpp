/*
 * ClusterPopulator.cpp
 *
 *  Created on: 8 mar 2010
 *      Author: Erik Ogenvik
 */

#include "ClusterPopulator.h"
#include "components/ogre/terrain/PlantAreaQueryResult.h"
#include "components/ogre/terrain/PlantAreaQuery.h"
#include "components/ogre/terrain/Segment.h"
#include "components/ogre/terrain/Buffer.h"
#include "components/ogre/terrain/PlantInstance.h"
#include "components/ogre/Convert.h"
#include <wfmath/ball.h>
#include <wfmath/intersect.h>
#include <wfmath/randgen.h>

#include <Mercator/Segment.h>
#include <Mercator/Surface.h>
#include <Mercator/Shader.h>

namespace Ember {
namespace OgreView {

namespace Terrain {

namespace Foliage {

ClusterPopulator::ClusterPopulator(int layerIndex, std::unique_ptr<IScaler> scaler, size_t plantIndex) :
		PlantPopulator(layerIndex, std::move(scaler), plantIndex),
		mMinClusterRadius(1.0f),
		mMaxClusterRadius(1.0f),
		mClusterDistance(1.0f),
		mDensity(1.0f),
		mFalloff(1.0f),
		mThreshold(0) {
}

void ClusterPopulator::populate(PlantAreaQueryResult& result, SegmentRefPtr segmentRef) {
	Mercator::Segment& mercatorSegment = segmentRef->getMercatorSegment();
	if (!mercatorSegment.isValid()) {
		mercatorSegment.populate();
	}

	const PlantAreaQuery& query = result.mQuery;

	const WFMath::AxisBox<2>& area = Convert::toWF(query.mArea);

	//Make a small list of surfaces in order
	std::list<int> indexSort;
	for (auto I = mercatorSegment.getSurfaces().begin(); I != mercatorSegment.getSurfaces().end(); ++I) {
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
	if (!indexSort.empty() && indexSort.front() == mLayerIndex) {
		Buffer<unsigned char> combinedCoverage(static_cast<unsigned int>(mercatorSegment.getSize()), 1);
		unsigned char* combinedCoverageData = combinedCoverage.getData();
		size_t size = combinedCoverage.getSize();
		//The first layer should be copied just as it is
		auto I = indexSort.begin();
		{
			auto& surface = mercatorSegment.getSurfaces()[*I];
			memcpy(combinedCoverageData, surface->getData(), combinedCoverage.getSize());
		}
		++I;
		for (; I != indexSort.end(); ++I) {
			auto& surface = mercatorSegment.getSurfaces()[*I];
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

void ClusterPopulator::getClustersForArea(const SegmentRefPtr& segmentRef, const WFMath::AxisBox<2>& area, ClusterStore& store) {
	//Generate clusters for the current page and all surrounding pages and check if any of these are contained or intersect our local area

	Mercator::Segment& mercatorSegment = segmentRef->getMercatorSegment();
	int res = mercatorSegment.getResolution();
	auto clustersPerSegment = static_cast<int>((float)(res * res) / (mClusterDistance * mClusterDistance));
	auto clusterRadiusRange = mMaxClusterRadius - mMinClusterRadius;
	auto xRef = mercatorSegment.getXRef();
	auto zRef = mercatorSegment.getZRef();

	WFMath::MTRand rng;

	for (int i = -1; i < 2; ++i) {
		for (int j = -1; j < 2; ++j) {
			auto currentSegmentX = static_cast<int>(xRef + (i * res));
			auto currentSegmentZ = static_cast<int>(zRef + (j * res));
			auto seed = static_cast<WFMath::MTRand::uint32>(mPlantIndex + (static_cast<WFMath::MTRand::uint32> (currentSegmentX) << 4) + (static_cast<WFMath::MTRand::uint32> (currentSegmentZ) << 8));
			rng.seed(seed);
			for (int k = 0; k < clustersPerSegment; ++k) {
				WFMath::Ball<2> cluster(WFMath::Point<2>((rng.rand<double>() * res) + currentSegmentX,
														 (rng.rand<double>() * res) + currentSegmentZ),
										(rng.rand<double>() * clusterRadiusRange) + mMinClusterRadius);
				if (WFMath::Contains(area, cluster.center(), true) || WFMath::Intersect(area, cluster, true)) {
					store.push_back(cluster);
				}
			}
		}
	}
}

void ClusterPopulator::populateWithClusters(const SegmentRefPtr& segmentRef,
											PlantAreaQueryResult& result,
											const WFMath::AxisBox<2>& area,
											const ClusterStore& clusters,
											const Buffer<unsigned char>& combinedCoverage) {
	for (const auto& cluster : clusters) {
		populateWithCluster(segmentRef, result, area, cluster, combinedCoverage);
	}

}

void ClusterPopulator::populateWithCluster(const SegmentRefPtr& segmentRef,
										   PlantAreaQueryResult& result,
										   const WFMath::AxisBox<2>& area,
										   const WFMath::Ball<2>& cluster,
										   const Buffer<unsigned char>& combinedCoverage) {
	PlantAreaQueryResult::PlantStore& plants = result.mStore;
	Mercator::Segment& mercatorSegment = segmentRef->getMercatorSegment();

	auto volume = (cluster.radius() * cluster.radius()) * WFMath::numeric_constants<WFMath::CoordType>::pi();
	auto instancesInEachCluster = static_cast<unsigned int>(volume * mDensity);
	auto seed = static_cast<WFMath::MTRand::uint32>(mPlantIndex + (static_cast<WFMath::MTRand::uint32> (cluster.center().x()) << 4) + (static_cast<WFMath::MTRand::uint32> (cluster.center().y()) << 8));
	WFMath::MTRand rng(seed);

	unsigned int res = combinedCoverage.getResolution();
	const unsigned char* data = combinedCoverage.getData();

	float height = 0;
	WFMath::Vector<3> normal;
	//place one cluster
	for (unsigned int j = 0; j < instancesInEachCluster; ++j) {
		auto theta = rng.rand<float>() * WFMath::numeric_constants<WFMath::CoordType>::pi() * 2;
		auto length = rng.rand<float>() * mMaxClusterRadius;

		WFMath::Point<2> pos(std::cos(theta) * length, std::sin(theta) * length);
		pos.shift(WFMath::Vector<2>(cluster.getCenter()));
		auto rotation = static_cast<float>(rng.rand(360.0));
		Ogre::Vector2 scale;
		mScaler->scale(rng, pos, scale);

		if (WFMath::Contains(area, pos, true)) {
			WFMath::Point<2> localPos(pos.x() - mercatorSegment.getXRef(), pos.y() - mercatorSegment.getZRef());
			if (data[((unsigned int) localPos.y() * res) + ((unsigned int) localPos.x())] >= mThreshold) {
				mercatorSegment.getHeightAndNormal((float)localPos.x(), (float)localPos.y(), height, normal);
				plants.emplace_back(PlantInstance{Ogre::Vector3((float)pos.x(), height, (float)pos.y()), rotation, scale});
			}
		}
	}
}

float ClusterPopulator::getMinClusterRadius() const {
	return mMinClusterRadius;
}

void ClusterPopulator::setMinClusterRadius(float theValue) {
	mMinClusterRadius = theValue;
}

float ClusterPopulator::getMaxClusterRadius() const {
	return mMaxClusterRadius;
}

void ClusterPopulator::setMaxClusterRadius(float theValue) {
	mMaxClusterRadius = theValue;
}

float ClusterPopulator::getDensity() const {
	return mDensity;
}

void ClusterPopulator::setDensity(float theValue) {
	mDensity = theValue;
}

float ClusterPopulator::getFalloff() const {
	return mFalloff;
}

void ClusterPopulator::setFalloff(float theValue) {
	mFalloff = theValue;
}

float ClusterPopulator::getClusterDistance() const {
	return mClusterDistance;
}

void ClusterPopulator::setClusterDistance(float theValue) {
	mClusterDistance = theValue;
}

void ClusterPopulator::setThreshold(unsigned char theValue) {
	mThreshold = theValue;
}

float ClusterPopulator::getTreshold() const {
	return mThreshold;
}

}

}

}
}
