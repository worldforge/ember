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

#include "components/ogre/GraphicalChangeAdapter.h"
#include "components/ogre/ShaderManager.h"

#include "framework/LoggingInstance.h"

namespace Ember
{
namespace OgreView
{

ShaderDetailManager::ShaderDetailManager(GraphicalChangeAdapter& graphicalChangeAdapter, Ember::OgreView::ShaderManager& shaderManager) :
		mShaderThresholdLevel(8.0f), mGraphicalChangeAdapter(graphicalChangeAdapter), mShaderManager(shaderManager)
{
//	mChangeRequiredConnection = mGraphicalChangeAdapter.EventChangeRequired.connect(sigc::mem_fun(*this, &ShaderDetailManager::changeLevel));
}

ShaderDetailManager::~ShaderDetailManager()
{
//	mChangeRequiredConnection.disconnect();
}

bool ShaderDetailManager::stepUpShaderLevel()
{
	ShaderManager::GraphicsLevel level;

	if (mShaderManager.getGraphicsLevel() == ShaderManager::LEVEL_MEDIUM) {
		level = ShaderManager::LEVEL_HIGH;
	} else if (mShaderManager.getGraphicsLevel() == ShaderManager::LEVEL_LOW) {
		level = ShaderManager::LEVEL_MEDIUM;
	} else {
		return false;
	}

	if (mShaderManager.getBestSupportedGraphicsLevel() >= level) {
		mShaderManager.setGraphicsLevel(level);
		return true;
	}
	return false;
}

bool ShaderDetailManager::stepDownShaderLevel()
{
	ShaderManager::GraphicsLevel level;

	if (mShaderManager.getGraphicsLevel() == ShaderManager::LEVEL_HIGH) {
		level = ShaderManager::LEVEL_MEDIUM;
	} else if (mShaderManager.getGraphicsLevel() == ShaderManager::LEVEL_MEDIUM) {
		level = ShaderManager::LEVEL_LOW;
	} else {
		return false;
	}

	mShaderManager.setGraphicsLevel(level);
	return true;
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
		if (level > 1.0f) {
			return stepDownShaderLevel();
		} else {
			return stepUpShaderLevel();
		}
	}
}

}
}
