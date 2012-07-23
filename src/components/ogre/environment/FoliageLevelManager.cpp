#include "FoliageLevelManager.h"

#include <components/ogre/AutoGraphicsLevelManager.h>
#include "components/ogre/environment/Foliage.h"

#include "cmath"

namespace Ember
{
namespace OgreView
{
namespace Environment
{

FoliageLevelManager::FoliageLevelManager(Foliage& foliage, AutomaticGraphicsLevelManager& automaticGraphicsLevelManager) :
		mThresholdLevel(2.0f), mDefaultDensityStep(0.3f), mDefaultDistanceStep(0.3f), mUpdatedDensity(1.0f), mMaxFarDistance(2.0f), mMinFarDistance(0.3f), mFoliage(foliage), mAutomaticGraphicsLevelManager(automaticGraphicsLevelManager)
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

	if (level > 0.0f) { //decreasing foliage density since more fps is required
		changeMade = changeMade || stepDownFoliageDensity(mDefaultDensityStep) || stepDownFoliageDistance(mDefaultDistanceStep);
	} else if (level < 0.0f) { //increasing foliage density since less fps is required
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
	} else if (mUpdatedDensity < step && mUpdatedDensity > 0.0f) { //if there is still some positive density left which is smaller than step, set it to 0
		mUpdatedDensity = 0.0f;
		updateFoliageDensity();
		return true;
	} else { //step down not possible
		return false;
	}
}

bool FoliageLevelManager::stepUpFoliageDensity(float step)
{
	if (mUpdatedDensity + step <= 1.0f) { //step up only if the step doesn't cause density to go over default density
		mUpdatedDensity += step;
		updateFoliageDensity();
		return true;
	} else if (mUpdatedDensity < 1.0f) { //if the density is still below default density but a default step causes it to go over default density
		mUpdatedDensity = 1.0f;
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
	} else if (mFarDistance < step && mFarDistance > mMinFarDistance) { //if there is still some positive far distance left which is smaller than step, set it to 0
		mFarDistance = mMinFarDistance;
		foliageFarDistanceChanged.emit(mFarDistance);
		return true;
	} else { //step down not possible
		return false;
	}
}

bool FoliageLevelManager::stepUpFoliageDistance(float step)
{
	if (mFarDistance + step <= mMaxFarDistance) { //step up only if the step doesn't cause density to go over 1.
		mFarDistance += step;
		foliageFarDistanceChanged.emit(mFarDistance);
		return true;
	} else if (mFarDistance < mMaxFarDistance) { //if the far distance is still below 1 but a default step causes it to go over 1.
		mFarDistance = mMaxFarDistance;
		foliageFarDistanceChanged.emit(mFarDistance);
		return true;
	} else {
		return false; //step up not possible
	}
}

bool FoliageLevelManager::changeFoliageDistance(float distance)
{
	if (distance >= mMinFarDistance && distance <= mMaxFarDistance) { //change the density as long as it is above 0
		mFarDistance = distance;
		foliageFarDistanceChanged.emit(mFarDistance);
		return true;
	} else {
		return false;
	}
}

bool FoliageLevelManager::changeFoliageDensity(float density)
{
	if (density >= 0.0f && density <= 1.0f) { //change the density as long as it is above 0
		mUpdatedDensity = density;
		updateFoliageDensity();
		return true;
	} else {
		return false;
	}
}

void FoliageLevelManager::pause()
{
	if (mChangeRequiredConnection) {
		mChangeRequiredConnection.disconnect();
	}
}

void FoliageLevelManager::unpause()
{
	if (!mChangeRequiredConnection) {
		mChangeRequiredConnection = mAutomaticGraphicsLevelManager.getGraphicalAdapter().changeRequired.connect(sigc::mem_fun(*this, &FoliageLevelManager::changeLevel));
	}
}

}
}
}
