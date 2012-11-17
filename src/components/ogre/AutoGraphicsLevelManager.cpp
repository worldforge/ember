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

#include "components/ogre/AutoGraphicsLevelManager.h"

#include "services/config/ConfigListenerContainer.h"

#include "framework/MainLoopController.h"
#include "framework/Time.h"
#include "framework/LoggingInstance.h"

#include <Ogre.h>

#include <cmath>
#include <numeric>

namespace Ember
{
namespace OgreView
{

FrameTimeRecorder::FrameTimeRecorder(MainLoopController& mainLoopController) :
		mRequiredTimeSamples(boost::posix_time::seconds(2)), mTimePerFrameStore(20), mAccumulatedFrameTimes(boost::posix_time::seconds(0)), mAccumulatedFrames(0)
{
	mainLoopController.EventFrameProcessed.connect(sigc::mem_fun(*this, &FrameTimeRecorder::frameCompleted));
}

FrameTimeRecorder::~FrameTimeRecorder()
{
}

void FrameTimeRecorder::frameCompleted(const TimeFrame& timeFrame, unsigned int frameActionMask)
{
	if (frameActionMask & MainLoopController::FA_GRAPHICS) {

		mAccumulatedFrameTimes += timeFrame.getElapsedTime();
		mAccumulatedFrames++;

		if (mAccumulatedFrameTimes >= mRequiredTimeSamples) {

			mTimePerFrameStore.push_back(mAccumulatedFrameTimes / mAccumulatedFrames);
			mAccumulatedFrameTimes = boost::posix_time::seconds(0);
			mAccumulatedFrames = 0;

			boost::posix_time::time_duration averageTimePerFrame = std::accumulate(mTimePerFrameStore.begin(), mTimePerFrameStore.end(), boost::posix_time::time_duration()) / mTimePerFrameStore.size();
			EventAverageTimePerFrameUpdated(averageTimePerFrame);

		}
	}
}

AutomaticGraphicsLevelManager::AutomaticGraphicsLevelManager(MainLoopController& mainLoopController) :
		mDefaultFps(60.0f), mEnabled(false), mFrameTimeRecorder(mainLoopController), mConfigListenerContainer(new ConfigListenerContainer())
{
	mFpsUpdatedConnection = mFrameTimeRecorder.EventAverageTimePerFrameUpdated.connect(sigc::mem_fun(*this, &AutomaticGraphicsLevelManager::averageTimePerFrameUpdated));
	mConfigListenerContainer->registerConfigListener("general", "desiredfps", sigc::mem_fun(*this, &AutomaticGraphicsLevelManager::Config_DefaultFps));
	mConfigListenerContainer->registerConfigListenerWithDefaults("graphics", "autoadjust", sigc::mem_fun(*this, &AutomaticGraphicsLevelManager::Config_Enabled), false);
}

AutomaticGraphicsLevelManager::~AutomaticGraphicsLevelManager()
{
	mFpsUpdatedConnection.disconnect();
	delete mConfigListenerContainer;
}

void AutomaticGraphicsLevelManager::setFps(float fps)
{
	mDefaultFps = fps;
}

void AutomaticGraphicsLevelManager::checkFps(float currentFps)
{
	float changeRequired = mDefaultFps - currentFps;
	//This factor is used to adjust the required fps difference before a change is triggered. Lower required fpses eg. 30 will need to respond to smaller changes.
	float factor = mDefaultFps / 60.0f;
	if (std::abs(changeRequired) >= factor * 8.0f) {
		changeGraphicsLevel(changeRequired);
	}
}

void AutomaticGraphicsLevelManager::averageTimePerFrameUpdated(const boost::posix_time::time_duration timePerFrame)
{
	//Convert microseconds per frame to fps.
	checkFps(1000000.0f / timePerFrame.total_microseconds());
}

void AutomaticGraphicsLevelManager::changeGraphicsLevel(float changeInFpsRequired)
{
	//TODO: Need to implement functionality for if further change is not possible
	mGraphicalChangeAdapter.fpsChangeRequired(changeInFpsRequired);
}

GraphicalChangeAdapter& AutomaticGraphicsLevelManager::getGraphicalAdapter()
{
	return mGraphicalChangeAdapter;
}

void AutomaticGraphicsLevelManager::setEnabled(bool newEnabled)
{
	mEnabled = newEnabled;
	if (newEnabled == false) {
		mFpsUpdatedConnection.block();
	} else {
		mFpsUpdatedConnection.unblock();
	}
}

bool AutomaticGraphicsLevelManager::isEnabled() const
{
	return mEnabled;
}

void AutomaticGraphicsLevelManager::Config_DefaultFps(const std::string& section, const std::string& key, varconf::Variable& variable)
{
	if (variable.is_double()) {
		int fps = static_cast<double>(variable);
		//If set to 0, the fps the manager tries to achieve is 60
		if (fps == 0.0f) {
			fps = 60.0f;
		}
		mDefaultFps = fps;
	}
}

void AutomaticGraphicsLevelManager::Config_Enabled(const std::string& section, const std::string& key, varconf::Variable& variable)
{
	if (variable.is_bool() && static_cast<bool>(variable)) {
		setEnabled(true);
	} else {
		setEnabled(false);
	}
}

}
}
