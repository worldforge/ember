#include "LodLevelManager.h"

#include "components/ogre/AutoGraphicsLevelManager.h"

#include "OgreCamera.h"

#include "sigc++/signal.h"

namespace Ember
{
namespace OgreView
{
namespace Lod
{

LodLevelManager::LodLevelManager(AutomaticGraphicsLevelManager& automaticGraphicsLevelManager, Ogre::Camera& mainCamera) :
		mLodThresholdLevel(1.0f), mLodFactor(1.0f), mDefaultStep(0.4f), mMinLodFactor(0.2f), mMaxLodFactor(2.0f), mAutomaticGraphicsLevelManager(automaticGraphicsLevelManager), mMainCamera(mainCamera)
{
}

LodLevelManager::~LodLevelManager()
{
	mChangeRequiredConnection.disconnect();
}

void LodLevelManager::initialize()
{
	mChangeRequiredConnection = mAutomaticGraphicsLevelManager.getGraphicalAdapter().changeRequired.connect(sigc::mem_fun(*this, &LodLevelManager::changeLevel));
}

bool LodLevelManager::setLodBiasAll(float factor)
{
	//make it safe to access from outside by setting the factor
	mLodFactor = factor;

	mMainCamera.setLodBias(factor);
	return true;
}

bool LodLevelManager::changeLevel(float level)
{
	if (std::abs(level) < mLodThresholdLevel) {
		return false;
	} else {
		bool changeMade = false;
		if (level > 0.0f) {
			changeMade |= stepDownLodBias(mDefaultStep);
		} else {
			changeMade |= stepUpLodBias(mDefaultStep);
		}
		return changeMade;
	}
}

bool LodLevelManager::stepDownLodBias(float step)
{
	if (mLodFactor > step) { //step down only if existing lod is greater than step
		mLodFactor -= step;
		return setLodBiasAll(mLodFactor);
	} else if (mLodFactor < step && mLodFactor > mMinLodFactor) { //if there is still some positive lod left which is smaller than step, set it to the minimum lod
		mLodFactor = mMinLodFactor;
		return setLodBiasAll(mLodFactor);
	} else { //step down not possible
		return false;
	}
}

bool LodLevelManager::stepUpLodBias(float step)
{
	if (mLodFactor + step <= mMaxLodFactor) { //step up only if the step doesn't cause lod to go over maximum lod.
		mLodFactor += step;
		return setLodBiasAll(mLodFactor);
	} else if (mLodFactor < mMaxLodFactor) { //if the lod is still below maximum lod but a default step causes it to go over it.
		mLodFactor = mMaxLodFactor;
		return setLodBiasAll(mLodFactor);
	} else {
		return false; //step up not possible
	}
}

void LodLevelManager::pause()
{
	if (mChangeRequiredConnection) {
		mChangeRequiredConnection.disconnect();
	}
}

void LodLevelManager::unpause()
{
	if (!mChangeRequiredConnection) {
		mChangeRequiredConnection = mAutomaticGraphicsLevelManager.getGraphicalAdapter().changeRequired.connect(sigc::mem_fun(*this, &LodLevelManager::changeLevel));
	}
}

}
}
}
