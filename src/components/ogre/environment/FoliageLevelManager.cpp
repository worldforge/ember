#include "FoliageLevelManager.h"

#include "components/ogre/EmberOgre.h"

#include <components/ogre/AutoGraphicsLevelManager.h>
#include "components/ogre/environment/Foliage.h"
#include "components/ogre/terrain/TerrainLayerDefinition.h"
#include "components/ogre/terrain/TerrainLayerDefinitionManager.h"

#include "sstream"
#include "cmath"

namespace Ember
{
namespace OgreView
{
namespace Environment
{

FoliageLevelManager::FoliageLevelManager(Foliage& foliage, AutomaticGraphicsLevelManager& automaticGraphicsLevelManager) :
		mThresholdLevel(1), mDefaultDensityStep(0.2), mDefaultDistanceStep(0.2), mUpdatedDensity(1), mFoliage(foliage), mAutomaticGraphicsLevelManager(automaticGraphicsLevelManager)
{
}

FoliageLevelManager::~FoliageLevelManager()
{
	mChangeRequiredConnection.disconnect();
}

void FoliageLevelManager::initialize()
{
	mChangeRequiredConnection = mAutomaticGraphicsLevelManager.getGraphicalAdapter().changeRequired.connect(sigc::mem_fun(*this, &FoliageLevelManager::changeLevel));
}

void FoliageLevelManager::updateFoliageDensity()
{
	foliageDensityChanged.emit(mUpdatedDensity);
}

bool FoliageLevelManager::changeLevel(float level)
{
	if (std::abs(level) < mThresholdLevel) {
		return false;
	}

	//holds whether a change in density is made at the end of trying to step up or step down
	bool changeMade = false;

	if (level > 0) { //decreasing foliage density since more fps is required
		changeMade = changeMade || stepDownFoliageDensity(mDefaultDensityStep) || stepDownFoliageDistance(mDefaultDistanceStep);
	} else if (level < 0) { //increasing foliage density since less fps is required
		changeMade = changeMade || stepUpFoliageDensity(mDefaultDensityStep) || stepUpFoliageDistance(mDefaultDistanceStep);
	}

	return changeMade;

}

bool FoliageLevelManager::stepDownFoliageDensity(float step)
{
	if (mUpdatedDensity > step) { //step down only if existing density is greater than step
		mUpdatedDensity -= step;
		updateFoliageDensity();
		return true;
	} else if (mUpdatedDensity < step && mUpdatedDensity > 0) { //if there is still some positive density left which is smaller than step, set it to 0
		mUpdatedDensity = 0;
		updateFoliageDensity();
		return true;
	} else { //step down not possible
		return false;
	}
}

bool FoliageLevelManager::stepUpFoliageDensity(float step)
{
	if (mUpdatedDensity + step <= 1) { //step up only if the step doesn't cause density to go over default density
		mUpdatedDensity += step;
		updateFoliageDensity();
		return true;
	} else if (mUpdatedDensity <= 1) { //if the density is still below default density but a default step causes it to go over default density
		mUpdatedDensity = 1;
		updateFoliageDensity();
		return true;
	} else {
		return false; //step up not possible
	}
}

bool FoliageLevelManager::stepDownFoliageDistance(float step)
{
	if (mFarDistance > step) { //step down only if existing far distance is greater than step
		mFarDistance -= step;
		foliageFarDistanceChanged.emit(mFarDistance);
		return true;
	} else if (mFarDistance < step && mFarDistance > 0) { //if there is still some positive far distance left which is smaller than step, set it to 0
		mFarDistance = 0;
		foliageFarDistanceChanged.emit(mFarDistance);
		return true;
	} else { //step down not possible
		return false;
	}
}

bool FoliageLevelManager::stepUpFoliageDistance(float step)
{
	if (mFarDistance + step <= 1) { //step up only if the step doesn't cause density to go over 1.
		mFarDistance += step;
		foliageFarDistanceChanged.emit(mFarDistance);
		return true;
	} else if (mFarDistance <= 1) { //if the far distance is still below 1 but a default step causes it to go over 1.
		mFarDistance = 1;
		foliageFarDistanceChanged.emit(mFarDistance);
		return true;
	} else {
		return false; //step up not possible
	}
}

bool FoliageLevelManager::changeFoliageDistance(float distance)
{
	if (distance >= 0) { //change the density as long as it is above 0
		mFarDistance = distance;
		foliageFarDistanceChanged.emit(mFarDistance);
		return true;
	} else {
		return false;
	}
}

bool FoliageLevelManager::changeFoliageDensity(float density)
{
	if (density >= 0) { //change the density as long as it is above 0
		mUpdatedDensity = density;
		updateFoliageDensity();
		return true;
	} else {
		return false;
	}
}

}
}
}
