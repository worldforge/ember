/*
 * PlantPopulator.h
 *
 *  Created on: 8 mar 2010
 *      Author: Erik Hjortsberg
 */

#ifndef PLANTPOPULATOR_H_
#define PLANTPOPULATOR_H_

#include "components/ogre/terrain/Types.h"

namespace WFMath
{
class MTRand;
}

namespace Ogre {
class Vector2;
}

namespace EmberOgre
{

namespace Terrain
{
class PlantAreaQueryResult;

namespace Foliage
{

class IScaler
{
public:
	virtual void scale(WFMath::MTRand& rnd, const WFMath::Point<2>& pos, Ogre::Vector2& scale) = 0;
};

class UniformScaler : public IScaler
{
public:
	UniformScaler(float min, float max);
	virtual void scale(WFMath::MTRand& rnd, const WFMath::Point<2>& pos, Ogre::Vector2& scale);
private:
	float mMin;
	float mRange;
};

class Scaler : public IScaler
{
public:
	Scaler(float xMin, float xMax, float yMin, float yMax);
	virtual void scale(WFMath::MTRand& rnd, const WFMath::Point<2>& pos, Ogre::Vector2& scale);
private:
	float mXMin;
	float mXRange;
	float mYMin;
	float mYRange;
};

class PlantPopulator
{
public:

	PlantPopulator(unsigned int layerIndex, IScaler* scaler, unsigned int plantIndex);
	virtual ~PlantPopulator();

	virtual void populate(PlantAreaQueryResult& result, SegmentRefPtr segmentRef) = 0;

protected:

	unsigned int mLayerIndex;
	IScaler* mScaler;
	unsigned int mPlantIndex;

};

}

}

}

#endif /* PLANTPOPULATOR_H_ */
