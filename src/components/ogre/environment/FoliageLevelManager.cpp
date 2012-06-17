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
		mThresholdLevel(1), mDefaultDensityStep(0.2), mUpdatedDensity(1), mFoliage(foliage), mAutomaticGraphicsLevelManager(automaticGraphicsLevelManager)
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
		changeMade = changeMade || stepDownFoliageDensity(mDefaultDensityStep);
	} else if (level < 0) { //increasing foliage density since less fps is required
		changeMade = changeMade || stepUpFoliageDensity(mDefaultDensityStep);
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
