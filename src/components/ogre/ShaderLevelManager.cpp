#include "ShaderLevelManager.h"

#include "components/ogre/AutoGraphicsLevelManager.h"
#include "components/ogre/ShaderManager.h"

#include "sigc++/signal.h"

namespace Ember
{
namespace OgreView
{

ShaderLevelManager::ShaderLevelManager(IGraphicalChangeAdapter& iGraphicalChangeAdapter, ShaderManager& shaderManager) :
		mShaderManager(shaderManager), mIGraphicalChangeAdapter(iGraphicalChangeAdapter), mShaderThresholdLevel(8.0f)
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

ShaderLevelManager::~ShaderLevelManager()
{
	mChangeRequiredConnection.disconnect();
}

void ShaderLevelManager::initialize()
{
	mChangeRequiredConnection = mIGraphicalChangeAdapter.changeRequired.connect(sigc::mem_fun(*this, &ShaderLevelManager::changeLevel));
}

void ShaderLevelManager::changeShaderLevel(const std::string& level)
{
	mShaderLevel = level;
	mShaderManager.setGraphicsLevel(mShaderManager.getLevelByName(level));
}

bool ShaderLevelManager::stepUpShaderLevel()
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

bool ShaderLevelManager::stepDownShaderLevel()
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

void ShaderLevelManager::pause()
{
	mChangeRequiredConnection.block();
}

void ShaderLevelManager::unpause()
{
	mChangeRequiredConnection.unblock();
}

bool ShaderLevelManager::changeLevel(float level)
{
	if (std::abs(level) < mShaderThresholdLevel) {
		return false;
	} else {
		bool changeMade = false;
		if (level > 1.0f) {
			changeMade |= stepDownShaderLevel();
		} else {
			changeMade |= stepUpShaderLevel();
		}
		return changeMade;
	}
}

}
}