#include "LodLevelManager.h"

#include "components/ogre/AutoGraphicsLevelManager.h"
#include "components/ogre/ShaderManager.h"

#include "OgreCamera.h"

#include "sigc++/signal.h"

namespace Ember
{
namespace OgreView
{
namespace Lod
{

LodLevelManager::LodLevelManager(AutomaticGraphicsLevelManager& automaticGraphicsLevelManager, Ogre::Camera& mainCamera, ShaderManager& shaderManager) :
		mThresholdLevel(1.0f), mLodFactor(1.0f), mDefaultStep(0.4f), mMinLodFactor(0.2f), mMaxLodFactor(2.0f), mAutomaticGraphicsLevelManager(automaticGraphicsLevelManager), mMainCamera(mainCamera), mShaderManager(shaderManager)
{
	const std::map<ShaderManager::GraphicsLevel, std::string>& schemes = mShaderManager.getGraphicsScheme();
	if (!schemes.empty()) {
		std::map<ShaderManager::GraphicsLevel, std::string>::const_iterator currentLevel = schemes.find(mShaderManager.getGraphicsLevel());
		if (currentLevel != schemes.end()) {
			mShaderLevel = currentLevel->second;
		} else {
			mShaderLevel = schemes.rbegin()->second;
		}
		mMinShaderLevel = schemes.begin()->second;
		mMaxShaderLevel = schemes.rbegin()->second;
	}
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

void LodLevelManager::changeShaderLevel(const std::string& level)
{
	mShaderLevel = level;
	mShaderManager.setGraphicsLevel(mShaderManager.getLevelByName(level));
}

bool LodLevelManager::changeLevel(float level)
{
	if (std::abs(level) < mThresholdLevel) {
		return false;
	} else {
		bool changeMade = false;
		if (level > 1.0f) {
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

bool LodLevelManager::stepDownShaderLevel()
{
	const std::map<ShaderManager::GraphicsLevel, std::string>& schemes = mShaderManager.getGraphicsScheme();
	//Check if any shader schemes are available to switch to
	if (schemes.empty()) {
		return false;
	}

	if (mShaderLevel == mMinShaderLevel) {
		return false;
	} else {
		std::map<ShaderManager::GraphicsLevel, std::string>::const_iterator level = schemes.find(mShaderManager.getLevelByName(mShaderLevel));
		--level;
		changeShaderLevel(level->second);
		return true;
	}
}

bool LodLevelManager::stepUpShaderLevel()
{
	const std::map<ShaderManager::GraphicsLevel, std::string>& schemes = mShaderManager.getGraphicsScheme();
	//Check if any shader schemes are available to switch to
	if (schemes.empty()) {
		return false;
	}

	if (mShaderLevel == mMaxShaderLevel) {
		return false;
	} else {
		std::map<ShaderManager::GraphicsLevel, std::string>::const_iterator level = schemes.find(mShaderManager.getLevelByName(mShaderLevel));
		++level;
		changeShaderLevel(level->second);
		return true;
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
