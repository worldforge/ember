/*
 * PlantPopulator.cpp
 *
 *  Created on: 8 mar 2010
 *      Author: Erik Ogenvik
 */

#include "PlantPopulator.h"
#include <wfmath/randgen.h>
#include <OgreVector2.h>

namespace Ember
{
namespace OgreView
{

namespace Terrain
{

namespace Foliage
{

PlantPopulator::PlantPopulator(unsigned int layerIndex, IScaler* scaler, size_t plantIndex)
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
	scale.x = scale.y = mMin + (rnd.rand<float>() * mRange);
}

Scaler::Scaler(float xMin, float xMax, float yMin, float yMax) :
	mXMin(xMin), mXRange(xMax - xMin), mYMin(yMin), mYRange(yMax - yMin)
{

}
void Scaler::scale(WFMath::MTRand& rnd, const WFMath::Point<2>& pos, Ogre::Vector2& scale)
{
	scale.x = mXMin + (rnd.rand<float>() * mXRange);
	scale.y = mYMin + (rnd.rand<float>() * mYRange);

}

}

}

}
}
