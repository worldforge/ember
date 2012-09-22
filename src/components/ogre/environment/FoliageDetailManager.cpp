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

#include "FoliageDetailManager.h"

#include <components/ogre/AutoGraphicsLevelManager.h>

#include <services/config/ConfigListenerContainer.h>

#include "cmath"

namespace Ember
{
namespace OgreView
{
namespace Environment
{

FoliageDetailManager::FoliageDetailManager(GraphicalChangeAdapter& graphicalChangeAdapter) :
		mGraphicalChangeAdapter(graphicalChangeAdapter), mConfigListenerContainer(new ConfigListenerContainer()), mThresholdLevel(2.0f), mDefaultDensityStep(0.3f), mDefaultDistanceStep(0.3f), mUpdatedDensity(1.0f), mFarDistance(1.0f), mMaxFarDistance(2.0f), mMinFarDistance(0.3f)
{
}

FoliageDetailManager::~FoliageDetailManager()
{
	mChangeRequiredConnection.disconnect();
}

void FoliageDetailManager::initialize()
{
	mChangeRequiredConnection = mGraphicalChangeAdapter.changeRequired.connect(sigc::mem_fun(*this, &FoliageDetailManager::changeLevel));
	mConfigListenerContainer->registerConfigListener("graphics", "foliagedensity", sigc::mem_fun(*this, &FoliageDetailManager::Config_FoliageDensity));
	mConfigListenerContainer->registerConfigListener("graphics", "foliagefardistance", sigc::mem_fun(*this, &FoliageDetailManager::Config_FoliageFarDistance));
}

bool FoliageDetailManager::changeLevel(float level)
{
	if (std::abs(level) < mThresholdLevel) {
		return false;
	}

	//holds whether a change in density is made at the end of trying to step up or step down
	bool changeMade = false;

	if (level > 0.0f) { //decreasing foliage density since more fps is required
		changeMade = changeMade || stepDownFoliageDensity(mDefaultDensityStep) || stepDownFoliageDistance(mDefaultDistanceStep);
	} else if (level < 0.0f) { //increasing foliage density since less fps is required
		changeMade = changeMade || stepUpFoliageDensity(mDefaultDensityStep) || stepUpFoliageDistance(mDefaultDistanceStep);
	}

	return changeMade;
}

bool FoliageDetailManager::stepDownFoliageDensity(float step)
{
	if (mUpdatedDensity > step) { //step down only if existing density is greater than step
		mUpdatedDensity -= step;
		foliageDensityChanged.emit(mUpdatedDensity);
		return true;
	} else if (mUpdatedDensity < step && mUpdatedDensity > 0.0f) { //if there is still some positive density left which is smaller than step, set it to 0
		mUpdatedDensity = 0.0f;
		foliageDensityChanged.emit(mUpdatedDensity);
		return true;
	} else { //step down not possible
		return false;
	}
}

bool FoliageDetailManager::stepUpFoliageDensity(float step)
{
	if (mUpdatedDensity + step <= 1.0f) { //step up only if the step doesn't cause density to go over default density
		mUpdatedDensity += step;
		foliageDensityChanged.emit(mUpdatedDensity);
		return true;
	} else if (mUpdatedDensity < 1.0f) { //if the density is still below default density but a default step causes it to go over default density
		mUpdatedDensity = 1.0f;
		foliageDensityChanged.emit(mUpdatedDensity);
	} else {
		return false; //step up not possible
	}
}

bool FoliageDetailManager::stepDownFoliageDistance(float step)
{
	if (mFarDistance > step) { //step down only if existing far distance is greater than step
		mFarDistance -= step;
		foliageFarDistanceChanged.emit(mFarDistance);
		return true;
	} else if (mFarDistance < step && mFarDistance > mMinFarDistance) { //if there is still some positive far distance left which is smaller than step, set it to minimum far distance.
		mFarDistance = mMinFarDistance;
		foliageFarDistanceChanged.emit(mFarDistance);
		return true;
	} else { //step down not possible
		return false;
	}
}

bool FoliageDetailManager::stepUpFoliageDistance(float step)
{
	if (mFarDistance + step <= mMaxFarDistance) { //step up only if the step doesn't cause distance to go over max distance.
		mFarDistance += step;
		foliageFarDistanceChanged.emit(mFarDistance);
		return true;
	} else if (mFarDistance < mMaxFarDistance) { //if the far distance is still below max distance but a default step causes it to go over max distance.
		mFarDistance = mMaxFarDistance;
		foliageFarDistanceChanged.emit(mFarDistance);
		return true;
	} else {
		return false; //step up not possible
	}
}

bool FoliageDetailManager::setFoliageDistance(float distance)
{
	pause(); //pause the component to prevent unstability if value is changed manually during signalled automatic adjustment.
	
	if (distance < 0) { //set the distance as long as it's not negative.
		mFarDistance = 0;
	} else {
		mFarDistance = distance;
	}
	foliageFarDistanceChanged.emit(mFarDistance);
	return true;
	
	unpause();
}

bool FoliageDetailManager::setFoliageDensity(float density)
{
	pause(); //pause the listening for change requests to prevent unstability if value is changed manually during signalled automatic adjustment.
	
	if (density < 0.0f) { //check if density negative
		mUpdatedDensity = 0.0f;
	} else if (density > 1.0f) {
		mUpdatedDensity = 1.0f;
	} else {
		mUpdatedDensity = density;
	}
	foliageDensityChanged.emit(mUpdatedDensity);
	
	unpause();
}

void FoliageDetailManager::Config_FoliageDensity(const std::string& section, const std::string& key, varconf::Variable& variable)
{
	if (variable.is_double()) {
		float density = static_cast<double>(variable);
		setFoliageDensity(density / 100);
	}
}

void FoliageDetailManager::Config_FoliageFarDistance(const std::string& section, const std::string& key, varconf::Variable& variable)
{
	if (variable.is_double()) {
		float distanceFactor = static_cast<double>(variable);
		setFoliageDistance(distanceFactor / 100);
	}
}

void FoliageDetailManager::pause()
{
	mChangeRequiredConnection.block();
}

void FoliageDetailManager::unpause()
{
	mChangeRequiredConnection.unblock();
}

}
}
}
