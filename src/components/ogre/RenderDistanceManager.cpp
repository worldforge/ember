#include "RenderDistanceManager.h"

#include "components/ogre/AutoGraphicsLevelManager.h"

#include "components/ogre/environment/IEnvironmentProvider.h"
#include <services/config/ConfigListenerContainer.h>

#include "OgreCamera.h"

namespace Ember
{
namespace OgreView
{

RenderDistanceManager::RenderDistanceManager(IGraphicalChangeAdapter& iGraphicalChangeAdapter, Environment::IFog& fog, Ogre::Camera& mainCamera) :
		mIGraphicalChangeAdapter(iGraphicalChangeAdapter), mFog(fog), mMainCamera(mainCamera), mConfigListenerContainer(new ConfigListenerContainer()), mDefaultFarRenderDistance(1000), mFarRenderDistance(1000), mMaxFarRenderDistanceFactor(1.5f), mMinFarRenderDistanceFactor(0.7f), mRenderDistanceThreshold(5.0f), mFarRenderDistanceFactor(1.0f), mDefaultRenderDistanceStep(0.3f)
{
}

RenderDistanceManager::~RenderDistanceManager()
{
	if (mChangeRequiredConnection) {
		mChangeRequiredConnection.disconnect();
	}
}

void RenderDistanceManager::initialize()
{
	if (!mChangeRequiredConnection) {
		mChangeRequiredConnection = mIGraphicalChangeAdapter.changeRequired.connect(sigc::mem_fun(*this, &RenderDistanceManager::changeLevel));
	}
	mConfigListenerContainer->registerConfigListener("graphics", "renderdistance", sigc::mem_fun(*this, &RenderDistanceManager::Config_FarRenderDistance));
}

bool RenderDistanceManager::setFarRenderDistance(float factor)
{
	mFarRenderDistance = factor * mDefaultFarRenderDistance;
	mMainCamera.setFarClipDistance(mFarRenderDistance);
}

bool RenderDistanceManager::setCompensatedFarRenderDistance(float factor)
{
	mFarRenderDistance = factor * mDefaultFarRenderDistance;
	mMainCamera.setFarClipDistance(mFarRenderDistance);
	float fogDensity = mMaxFarRenderDistanceFactor - factor;
	if (factor > 0.3) {
		fogDensity = 0.3;
	}
	mFog.setDensity(fogDensity);
}

bool RenderDistanceManager::changeLevel(float level)
{
	// If the fps change required is less than any threshold, the changeMade boolean will remain false, else it indicates if a step up or down was made.
	bool changeMade = false;

	if (std::abs(level) >= mRenderDistanceThreshold) {
		if (level > 0.0f) {
			changeMade |= stepDownFarRenderDistance(mDefaultRenderDistanceStep);
		} else {
			changeMade |= stepUpFarRenderDistance(mDefaultRenderDistanceStep);
		}
	}
	return changeMade;
}

bool RenderDistanceManager::stepDownFarRenderDistance(float step)
{
	bool stepPossible = false;
	if (mFarRenderDistanceFactor > step) { //step down only if existing render distance factor is greater than step
		mFarRenderDistanceFactor -= step;
		setFarRenderDistance(mFarRenderDistanceFactor);
		stepPossible = true;
	} else if (mFarRenderDistanceFactor < step && mFarRenderDistanceFactor > mMinFarRenderDistanceFactor) { //if there is still some positive distance left which is smaller than step, set it to the minimum distance
		mFarRenderDistanceFactor = mMinFarRenderDistanceFactor;
		setFarRenderDistance(mFarRenderDistanceFactor);
		stepPossible = true;
	} else { //step down not possible
		stepPossible = false;
	}
	return stepPossible;
}

bool RenderDistanceManager::stepUpFarRenderDistance(float step)
{
	bool stepPossible = false;
	if (mFarRenderDistanceFactor + step <= mMaxFarRenderDistanceFactor) { //step up only if the step doesn't cause render far distance to go over maximum render far distance.
		mFarRenderDistanceFactor += step;
		setFarRenderDistance(mFarRenderDistanceFactor);
		stepPossible = true;
	} else if (mFarRenderDistanceFactor < mMaxFarRenderDistanceFactor) { //if the render far distance is still below maximum render far distance but a default step causes it to go over it.
		mFarRenderDistanceFactor = mMaxFarRenderDistanceFactor;
		setFarRenderDistance(mFarRenderDistanceFactor);
		stepPossible = true;
	} else {
		stepPossible = false; //step up not possible
	}
	return stepPossible;
}

void RenderDistanceManager::Config_FarRenderDistance(const std::string& section, const std::string& key, varconf::Variable& variable)
{
	if (variable.is_double()) {
		float renderDistance = static_cast<double>(variable);
		setFarRenderDistance(renderDistance/100);
	}
}

void RenderDistanceManager::pause()
{
	mChangeRequiredConnection.block();
}

void RenderDistanceManager::unpause()
{
	mChangeRequiredConnection.unblock();
}

}
}
