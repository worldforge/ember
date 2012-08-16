#include "ShadowLevelManager.h"

#include "AutoGraphicsLevelManager.h"
#include <services/config/ConfigListenerContainer.h>

#include "OgreSceneManager.h"
#include <OgreHardwarePixelBuffer.h>

namespace Ember
{
namespace OgreView
{

ShadowLevelManager::ShadowLevelManager(IGraphicalChangeAdapter& iGraphicalChangeAdapter, Ogre::SceneManager& sceneManager) :
		mSceneManager(sceneManager), mIGraphicalChangeAdapter(iGraphicalChangeAdapter) , mShadowFarDistance(sceneManager.getShadowFarDistance()), mConfigListenerContainer(new ConfigListenerContainer()), mShadowCameraLodBias(1.0f), mDefaultShadowDistanceStep(250), mDefaultShadowLodStep(0.3), mShadowCameraLodThreshold(3.0f), mShadowDistanceThreshold(3.0f), mMaxShadowCameraLodBias(1.0f), mMinShadowCameraLodBias(0.1f), mMaxShadowFarDistance(1000.0f), mMinShadowFarDistance(0.0f)
{
}

ShadowLevelManager::~ShadowLevelManager()
{
	if (mChangeRequiredConnection) {
		mChangeRequiredConnection.disconnect();
	}
}

bool ShadowLevelManager::setShadowCameraLodBias(float factor)
{
	int textureCount = mSceneManager.getShadowTextureCount();
	for (int i = 0; i < textureCount; i++) {
		Ogre::TexturePtr texture = mSceneManager.getShadowTexture(i);
		int viewportCount = texture->getBuffer()->getRenderTarget()->getNumViewports();
		for (int j = 0; j < viewportCount; ++j) {
			texture->getBuffer()->getRenderTarget()->getViewport(j)->getCamera()->setLodBias(factor);
		}
	}
	return true;
}

bool ShadowLevelManager::setShadowFarDistance(float distance)
{
	mSceneManager.setShadowFarDistance(distance);
	return true;
}

void ShadowLevelManager::initialize()
{
	mChangeRequiredConnection = mIGraphicalChangeAdapter.changeRequired.connect(sigc::mem_fun(*this, &ShadowLevelManager::changeLevel));
	mConfigListenerContainer->registerConfigListener("graphics", "shadowlodbias", sigc::mem_fun(*this, &ShadowLevelManager::Config_ShadowLodBias));
}

bool ShadowLevelManager::changeLevel(float level)
{
	// If the fps change required is less than any threshold, the changeMade boolean will remain false, else it indicates if a step up or down was made.
	bool changeMade = false;

	if (std::abs(level) >= mShadowDistanceThreshold) {
		if (level > 0.0f) {
			changeMade |= stepDownShadowDistance(mDefaultShadowDistanceStep);
		} else {
			changeMade |= stepUpShadowDistance(mDefaultShadowDistanceStep);
		}
	} else if (std::abs(level) >= mShadowCameraLodThreshold) {
		if (level > 0.0f) {
			changeMade |= stepDownShadowCameraLodBias(mDefaultShadowLodStep);
		} else {
			changeMade |= stepUpShadowCameraLodBias(mDefaultShadowLodStep);
		}
	}
	return changeMade;
}

bool ShadowLevelManager::stepDownShadowDistance(float step)
{
	if (mShadowFarDistance > step) { //step down only if existing shadow far distance is greater than step
		mShadowFarDistance -= step;
		return setShadowFarDistance(mShadowFarDistance);
	} else if (mShadowFarDistance < step && mShadowFarDistance > mMinShadowFarDistance) { //if there is still some positive distance left which is smaller than step, set it to the minimum distance
		mShadowFarDistance = mMinShadowFarDistance;
		return setShadowFarDistance(mShadowFarDistance);
	} else { //step down not possible
		return false;
	}
}

bool ShadowLevelManager::stepUpShadowDistance(float step)
{
	if (mShadowFarDistance + step <= mMaxShadowFarDistance) { //step up only if the step doesn't cause shadow far distance to go over maximum shadow far distance.
		mShadowFarDistance += step;
		return setShadowFarDistance(mShadowFarDistance);
	} else if (mShadowFarDistance < mMaxShadowFarDistance) { //if the shadow far distance is still below maximum shadow far distance but a default step causes it to go over it.
		mShadowFarDistance = mMaxShadowFarDistance;
		return setShadowFarDistance(mShadowFarDistance);
	} else {
		return false; //step up not possible
	}
}

bool ShadowLevelManager::stepDownShadowCameraLodBias(float step)
{
	if (mShadowCameraLodBias > step) { //step down only if existing shadow camera lod bias is greater than step
		mShadowCameraLodBias -= step;
		return setShadowCameraLodBias(mShadowCameraLodBias);
	} else if (mShadowCameraLodBias < step && mShadowCameraLodBias > mMinShadowCameraLodBias) { //if there is still some positive lod bias left which is smaller than step, set it to the minimum shadow lod
		mShadowCameraLodBias = mMinShadowCameraLodBias;
		return setShadowCameraLodBias(mShadowCameraLodBias);
	} else { //step down not possible
		return false;
	}
}

bool ShadowLevelManager::stepUpShadowCameraLodBias(float step)
{
	if (mShadowCameraLodBias + step <= mMaxShadowCameraLodBias) { //step up only if the step doesn't cause shadow lod bias to go over maximum shadow far distance.
		mShadowCameraLodBias += step;
		return setShadowCameraLodBias(mShadowCameraLodBias);
	} else if (mShadowCameraLodBias < mMaxShadowCameraLodBias) { //if the shadow lod bias is still below maximum shadow lod bias but a default step causes it to go over it.
		mShadowCameraLodBias = mMaxShadowCameraLodBias;
		return setShadowCameraLodBias(mShadowCameraLodBias);
	} else {
		return false; //step up not possible
	}
}

void ShadowLevelManager::Config_ShadowLodBias(const std::string& section, const std::string& key, varconf::Variable& variable)
{
	if (variable.is_double()) {
		float lodBias = static_cast<double>(variable);
		setShadowCameraLodBias(lodBias/100);
	}
}

void ShadowLevelManager::pause()
{
	mChangeRequiredConnection.block();
}

void ShadowLevelManager::unpause()
{
	mChangeRequiredConnection.unblock();
}

}
}
