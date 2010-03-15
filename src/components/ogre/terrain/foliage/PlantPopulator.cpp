/*
 * PlantPopulator.cpp
 *
 *  Created on: 8 mar 2010
 *      Author: Erik Hjortsberg
 */

#include "PlantPopulator.h"
#include <wfmath/randgen.h>
#include <OgreVector2.h>

namespace EmberOgre
{

namespace Terrain
{

namespace Foliage
{

PlantPopulator::PlantPopulator(unsigned int layerIndex, IScaler* scaler, unsigned int plantIndex)
: mLayerIndex(layerIndex), mScaler(scaler), mPlantIndex(plantIndex)
{
}

PlantPopulator::~PlantPopulator()
{
	delete mScaler;
}

UniformScaler::UniformScaler(float min, float max) :
	mMin(min), mRange(max - min)
{

}
void UniformScaler::scale(WFMath::MTRand& rnd, const WFMath::Point<2>& pos, Ogre::Vector2& scale)
{
	scale.x = scale.y = mMin + rnd.rand(mRange);
}

Scaler::Scaler(float xMin, float xMax, float yMin, float yMax) :
	mXMin(xMin), mXRange(xMax - xMin), mYMin(yMin), mYRange(yMax - yMin)
{

}
void Scaler::scale(WFMath::MTRand& rnd, const WFMath::Point<2>& pos, Ogre::Vector2& scale)
{
	scale.x = mXMin + rnd.rand(mXRange);
	scale.y = mYMin + rnd.rand(mYRange);

}

}

}

}
