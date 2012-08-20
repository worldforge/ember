/*
 * Copyright (C) 2012 Arjun Kumar <arjun1991@gmail.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#include "ShaderDetailManager.h"

#include "components/ogre/AutoGraphicsLevelManager.h"
#include "components/ogre/ShaderManager.h"

#include "framework/LoggingInstance.h"

#include "sigc++/signal.h"

namespace Ember
{
namespace OgreView
{

ShaderDetailManager::ShaderDetailManager(IGraphicalChangeAdapter& graphicalChangeAdapter, Ember::OgreView::ShaderManager& shaderManager) :
		mShaderManager(shaderManager), mGraphicalChangeAdapter(graphicalChangeAdapter), mShaderThresholdLevel(8.0f)
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

ShaderDetailManager::~ShaderDetailManager()
{
	mChangeRequiredConnection.disconnect();
}

void ShaderDetailManager::initialize()
{
	mChangeRequiredConnection = mGraphicalChangeAdapter.changeRequired.connect(sigc::mem_fun(*this, &ShaderDetailManager::changeLevel));
}

void ShaderDetailManager::changeShaderLevel(const std::string& level)
{
	mShaderLevel = level;
	mShaderManager.setGraphicsLevel(mShaderManager.getLevelByName(level));
}

bool ShaderDetailManager::stepUpShaderLevel()
{
	const std::map<ShaderManager::GraphicsLevel, std::string>& schemes = mShaderManager.getGraphicsScheme();
	//Check if any shader schemes are available to switch to
	if (schemes.empty()) {
		S_LOG_WARNING("Shader schemes are empty, shader detail manager cannot step up shader detail");
		return false;
	}

	if (mShaderLevel == mMaxShaderLevel) {
		S_LOG_VERBOSE("Shader scheme is already set to maximum shader detail, cannot step up shader detail.");
		return false;
	} else {
		std::map<ShaderManager::GraphicsLevel, std::string>::const_iterator level = schemes.find(mShaderManager.getLevelByName(mShaderLevel));
		++level;
		changeShaderLevel(level->second);
		S_LOG_VERBOSE("Setting shader scheme to " << level->second);
		return true;
	}
}

bool ShaderDetailManager::stepDownShaderLevel()
{
	const std::map<ShaderManager::GraphicsLevel, std::string>& schemes = mShaderManager.getGraphicsScheme();
	//Check if any shader schemes are available to switch to
	if (schemes.empty()) {
		S_LOG_WARNING("Shader schemes are empty, shader detail manager cannot step down shader detail");
		return false;
	}

	if (mShaderLevel == mMinShaderLevel) {
		S_LOG_VERBOSE("Shader scheme is already set to minimum shader detail, cannot step down shader detail.");
		return false;
	} else {
		std::map<ShaderManager::GraphicsLevel, std::string>::const_iterator level = schemes.find(mShaderManager.getLevelByName(mShaderLevel));
		--level;
		S_LOG_VERBOSE("Setting shader scheme to " << level->second);
		changeShaderLevel(level->second);
		return true;
	}
}

void ShaderDetailManager::pause()
{
	mChangeRequiredConnection.block();
}

void ShaderDetailManager::unpause()
{
	mChangeRequiredConnection.unblock();
}

bool ShaderDetailManager::changeLevel(float level)
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