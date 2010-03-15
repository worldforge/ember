/*
 * ClusterPopulator.h
 *
 *  Created on: 8 mar 2010
 *      Author: Erik Hjortsberg
 */

#ifndef CLUSTERPOPULATOR_H_
#define CLUSTERPOPULATOR_H_

#include "PlantPopulator.h"

namespace EmberOgre
{

namespace Terrain
{

template <typename> class Buffer;

namespace Foliage
{

typedef std::vector<WFMath::Ball<2> > ClusterStore;

class ClusterPopulator : public PlantPopulator
{
public:
	ClusterPopulator(unsigned int layerIndex, IScaler* scaler, unsigned int plantIndex);
	virtual ~ClusterPopulator();

	virtual void populate(PlantAreaQueryResult& result, SegmentRefPtr segmentRef);

	void setMinClusterRadius(float theValue);
	float getMinClusterRadius() const;

	void setMaxClusterRadius(float theValue);
	float getMaxClusterRadius() const;

	void setDensity(float theValue);
	float getDensity() const;

	void setFalloff(float theValue);
	float getFalloff() const;

	void setClusterDistance(float theValue);
	float getClusterDistance() const;

	void setThreshold(unsigned char theValue);
	float getTreshold() const;
protected:

	void getClustersForArea(const SegmentRefPtr& segmentRef, const WFMath::AxisBox<2>& area, ClusterStore& clusters);

	void populateWithClusters(const SegmentRefPtr& segmentRef, PlantAreaQueryResult& result, const WFMath::AxisBox<2>& area, const ClusterStore& clusters, const Buffer<unsigned char>& combinedCoverage);
	void populateWithCluster(const SegmentRefPtr& segmentRef, PlantAreaQueryResult& result, const WFMath::AxisBox<2>& area, const WFMath::Ball<2>& cluster, const Buffer<unsigned char>& combinedCoverage);
	float mMinClusterRadius;
	float mMaxClusterRadius;
	float mClusterDistance;
	float mDensity;
	float mFalloff;
	unsigned char mThreshold;
};

}

}

}

#endif /* CLUSTERPOPULATOR_H_ */
