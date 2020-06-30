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
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#include "LodLevelManager.h"

#include "components/ogre/GraphicalChangeAdapter.h"
#include "services/config/ConfigListenerContainer.h"

#include <OgreCamera.h>

namespace Ember {
namespace OgreView {
namespace Lod {

LodLevelManager::LodLevelManager(GraphicalChangeAdapter& graphicalChangeAdapter, Ogre::Camera& mainCamera) :
		mLodThresholdLevel(1.0f),
		mMinLodFactor(0.2f),
		mMaxLodFactor(2.0f),
		mDefaultStep(0.4f),
		mGraphicalChangeAdapter(graphicalChangeAdapter),
		mMainCamera(mainCamera),
		mConfigListenerContainer(new ConfigListenerContainer()) {
	mChangeRequiredConnection = mGraphicalChangeAdapter.EventChangeRequired.connect(sigc::mem_fun(*this, &LodLevelManager::changeLevel));
	mConfigListenerContainer->registerConfigListener("graphics", "lodbias", sigc::mem_fun(*this, &LodLevelManager::Config_LodBias));
}

LodLevelManager::~LodLevelManager() {
	mChangeRequiredConnection.disconnect();
}

bool LodLevelManager::setLodBiasAll(Ogre::Real factor) {
	//do not allow lod bias to be set to 0 or a negative value.
	if (factor <= 0.0f) {
		factor = 0.1f;
	}

	mMainCamera.setLodBias(factor);
	return true;
}

bool LodLevelManager::changeLevel(float level) {
	if (std::abs(level) < mLodThresholdLevel) {
		return false;
	} else {
		if (level > 0.0f) {
			return stepUpLodBias(mDefaultStep);
		} else {
			return stepDownLodBias(mDefaultStep);
		}
	}
}

bool LodLevelManager::stepDownLodBias(float step) {
	Ogre::Real lodFactor = mMainCamera.getLodBias();
	if (lodFactor > step) { //step down only if existing lod is greater than step
		return setLodBiasAll(lodFactor - step);
	} else if (lodFactor < step && lodFactor > mMinLodFactor) { //if there is still some positive lod left which is smaller than step, set it to the minimum lod
		return setLodBiasAll(mMinLodFactor);
	} else { //step down not possible
		return false;
	}
}

bool LodLevelManager::stepUpLodBias(float step) {
	Ogre::Real lodFactor = mMainCamera.getLodBias();
	if (lodFactor + step <= mMaxLodFactor) { //step up only if the step doesn't cause lod to go over maximum lod.
		return setLodBiasAll(lodFactor + step);
	} else if (lodFactor < mMaxLodFactor) { //if the lod is still below maximum lod but a default step causes it to go over it.
		return setLodBiasAll(mMaxLodFactor);
	} else {
		return false; //step up not possible
	}
}

void LodLevelManager::Config_LodBias(const std::string& section, const std::string& key, varconf::Variable& variable) {
	if (variable.is_double()) {
		Ogre::Real lodBias = static_cast<double>(variable);
		setLodBiasAll(lodBias / 100.0f);
	}
}

void LodLevelManager::pause() {
	mChangeRequiredConnection.block();
}

void LodLevelManager::unpause() {
	mChangeRequiredConnection.unblock();
}

}
}
}
