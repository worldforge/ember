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

#include "LodLevelManager.h"

#include "components/ogre/AutoGraphicsLevelManager.h"
#include <services/config/ConfigListenerContainer.h>

#include "OgreCamera.h"

#include "sigc++/signal.h"

namespace Ember
{
namespace OgreView
{
namespace Lod
{

LodLevelManager::LodLevelManager(IGraphicalChangeAdapter& graphicalChangeAdapter, Ogre::Camera& mainCamera) :
		mLodThresholdLevel(1.0f), mLodFactor(1.0f), mDefaultStep(0.4f), mMinLodFactor(0.2f), mMaxLodFactor(2.0f), mGraphicalChangeAdapter(graphicalChangeAdapter), mMainCamera(mainCamera), mConfigListenerContainer(new ConfigListenerContainer())
{
}

LodLevelManager::~LodLevelManager()
{
	mChangeRequiredConnection.disconnect();
}

void LodLevelManager::initialize()
{
	mChangeRequiredConnection = mGraphicalChangeAdapter.changeRequired.connect(sigc::mem_fun(*this, &LodLevelManager::changeLevel));
	mConfigListenerContainer->registerConfigListener("graphics", "lodbias", sigc::mem_fun(*this, &LodLevelManager::Config_LodBias));
}

bool LodLevelManager::setLodBiasAll(float factor)
{
	//do not allow lod bias to be set to 0 or a negative value.
	if (factor <= 0.0f) {
		factor = 0.01;
	}
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

void LodLevelManager::Config_LodBias(const std::string& section, const std::string& key, varconf::Variable& variable)
{
	if (variable.is_double()) {
		float lodBias = static_cast<double>(variable);
		setLodBiasAll(lodBias / 100);
	}
}

void LodLevelManager::pause()
{
	mChangeRequiredConnection.block();
}

void LodLevelManager::unpause()
{
	mChangeRequiredConnection.unblock();
}

}
}
}
