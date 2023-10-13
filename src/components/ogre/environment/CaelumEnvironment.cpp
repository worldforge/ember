//
// C++ Implementation: CaelumEnvironment
//
// Description:
//
//
// Author: Erik Ogenvik <erik@ogenvik.org>, (C) 2006
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software Foundation,
// Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.//
//
#include "CaelumEnvironment.h"
#include "caelum/include/Caelum.h"
#include "CaelumSky.h"
#include "CaelumSun.h"
#include "SimpleWater.h"
#include "framework/Tokeniser.h"
#include "framework/TimeHelper.h"
#include <Eris/Calendar.h>

//We've fixed the environment to 2016-11-14 since it's a day with a full moon. And we want the full moon for illumination during night.
#define TIME_FIXED_YEAR 2016
#define TIME_FIXED_MONTH 11
#define TIME_FIXED_DAY 14

namespace Ember {
namespace OgreView {

namespace Environment {

CaelumEnvironment::CaelumEnvironment(Ogre::SceneManager* sceneMgr, Ogre::RenderWindow* window, Ogre::Camera& camera, Eris::Calendar& calendar) :
		SetCaelumTime("set_caelumtime", this, "Sets the caelum time. parameters: <hour> <minute>"),
		mCaelumSystem(nullptr),
		mSceneMgr(sceneMgr),
		mWindow(window),
		mCamera(camera),
		mCalendar(calendar),
		mSky(nullptr),
		mSun(nullptr),
		mWater(nullptr) {
	mCalendar.Updated.connect(sigc::mem_fun(*this, &CaelumEnvironment::Calendar_Updated));
}

CaelumEnvironment::~CaelumEnvironment() {
	destroyCaelumFirmament();
}

void CaelumEnvironment::createFirmament() {
	try {
		setupCaelum(mSceneMgr, mWindow, mCamera);
	} catch (const std::exception& ex) {
		S_LOG_FAILURE("Could not load Caelum." << ex);
		throw;
	}
}

void CaelumEnvironment::destroyFirmament() {
	destroyCaelumFirmament();
}

void CaelumEnvironment::destroyCaelumFirmament() {
	mSky.reset();
	mSun.reset();
	if (mCaelumSystem) {
		mWindow->removeListener(mCaelumSystem.get());
		Ogre::Root::getSingleton().removeFrameListener(mCaelumSystem.get());
		mCaelumSystem.reset();
	}
}


void CaelumEnvironment::Calendar_Updated() {
	Eris::DateTime now = mCalendar.now();
	if (now.valid()) {
		if (mCaelumSystem) {
			mCaelumSystem->getUniversalClock()->setGregorianDateTime(TIME_FIXED_YEAR, TIME_FIXED_MONTH, TIME_FIXED_DAY, now.hours(), now.minutes(), now.seconds());
		}
	}
}

void CaelumEnvironment::setWaterEnabled(bool enabled) {
	if (enabled) {
		if (!mWater) {
			setupWater();
		}
	} else {
		if (mWater) {
			mWater.reset();
		}
	}
}

void CaelumEnvironment::setupWater() {
	try {
		//mWater = new Water(mCamera, mSceneMgr);
		//	mWater = new HydraxWater(mCamera, *mSceneMgr);
		//NOTE: we default to simple water for now since there are a couple of performance problems with hydrax
		std::unique_ptr<SimpleWater> water;
		water = std::make_unique<SimpleWater>(mCamera, *mSceneMgr, *mWindow);
		if (water->isSupported()) {
			water->initialize();
		} else {
			water = std::make_unique<SimpleWater>(mCamera, *mSceneMgr, *mWindow);
			water->initialize();
		}
		mWater = std::move(water);
	} catch (const std::exception& ex) {
		S_LOG_FAILURE("Could not load water." << ex);
	}
}

void CaelumEnvironment::setupCaelum(::Ogre::SceneManager* sceneMgr, ::Ogre::RenderWindow* window, ::Ogre::Camera&) {

	Ogre::Root* root = Ogre::Root::getSingletonPtr();

	mCaelumSystem = std::make_unique<Caelum::CaelumSystem>(root, sceneMgr, Caelum::CaelumSystem::CAELUM_COMPONENTS_NONE);

	try {
		mCaelumSystem->setSkyDome(new Caelum::SkyDome(mSceneMgr, mCaelumSystem->getCaelumCameraNode()));
	} catch (const Caelum::UnsupportedException& ex) {
		S_LOG_WARNING("Error when creating Caelum sky dome." << ex);
	}
	try {
		mCaelumSystem->setSun(new Caelum::SpriteSun(mSceneMgr, mCaelumSystem->getCaelumCameraNode()));
		mSun = std::make_unique<CaelumSun>(*this, mCaelumSystem->getSun());
	} catch (const Caelum::UnsupportedException& ex) {
		//TODO: use a simple sun object
		S_LOG_WARNING("Error when creating Caelum sun." << ex);
	}
	try {
		mCaelumSystem->setMoon(new Caelum::Moon(mSceneMgr, mCaelumSystem->getCaelumCameraNode()));
	} catch (const Caelum::UnsupportedException& ex) {
		S_LOG_WARNING("Error when creating Caelum moon." << ex);
	}
	try {
		mCaelumSystem->setCloudSystem(new Caelum::CloudSystem(mSceneMgr, mCaelumSystem->getCaelumGroundNode()));
	} catch (const Caelum::UnsupportedException& ex) {
		S_LOG_WARNING("Error when creating Caelum clouds." << ex);
	}
	try {
		mCaelumSystem->setPointStarfield(new Caelum::PointStarfield(mSceneMgr, mCaelumSystem->getCaelumCameraNode()));
	} catch (const Caelum::UnsupportedException& ex) {
		S_LOG_WARNING("Error when creating Caelum point star field." << ex);
		try {
			mCaelumSystem->setImageStarfield(new Caelum::ImageStarfield(mSceneMgr, mCaelumSystem->getCaelumCameraNode()));
		} catch (const Caelum::UnsupportedException& ex) {
			S_LOG_WARNING("Error when creating Caelum image star field." << ex);
		}
	}

	mCaelumSystem->setManageSceneFog(true);
	mCaelumSystem->setManageAmbientLight(true);
	mCaelumSystem->setGlobalFogDensityMultiplier(0.005f);
	mCaelumSystem->setMinimumAmbientLight(Ogre::ColourValue(0.2f, 0.2f, 0.2f, 1.0f));

	mCaelumSystem->setEnsureSingleShadowSource(true); //we want to use only one shadow caster source, for now at least
	mCaelumSystem->setEnsureSingleLightSource(true); //We want to only use the brightest light source only, even if another is closer. This is to make sure the main light is taken from the sun instead of the moon (which will result in a dark landscape).

	mSky = std::make_unique<CaelumSky>(*this);

	// Register all to the render window
	mCaelumSystem->attachViewport(window->getViewport(0));
	window->addListener(mCaelumSystem.get());

	// Set time acceleration to fit with real world time
	mCaelumSystem->getUniversalClock()->setTimeScale(1);

	Eris::DateTime currentServerTime = mCalendar.now();
	if (currentServerTime.valid()) {
		mCaelumSystem->getUniversalClock()->setGregorianDateTime(TIME_FIXED_YEAR, TIME_FIXED_MONTH, TIME_FIXED_DAY, currentServerTime.hours(), currentServerTime.minutes(), currentServerTime.seconds());
	} else {
		S_LOG_WARNING("Could not get server time, using local time for environment.");
		int year, month, day, hour, minute, second;
		Ember::TimeHelper::getLocalTime(year, month, day, hour, minute, second);
		mCaelumSystem->getUniversalClock()->setGregorianDateTime(TIME_FIXED_YEAR, TIME_FIXED_MONTH, TIME_FIXED_DAY, hour, minute, second);
	}

	//little hack here. We of course want to use the server time, but currently when you log in when it's dark, you won't see much, so in order to show the world in it's full glory we'll try to set the time to day time
	/*
	 if (hour < 6) {
	 hour = 6;
	 } else if (hour > 16) {
	 hour = 15;
	 }
	 */

	//advance it one second to force it to do initial updating, since other subsystems such as the terrain rendering depends on the sun postions etc.
	mCaelumSystem->updateSubcomponents(1);

	//This will make caelum update itself automatically each frame
	Ogre::Root::getSingleton().addFrameListener(mCaelumSystem.get());
}

ISun* CaelumEnvironment::getSun() {
	return mSun.get();
}

ISky* CaelumEnvironment::getSky() {
	return mSky.get();
}

IFog* CaelumEnvironment::getFog() {
	return mSky.get();
	//return mFog;
}

IWater* CaelumEnvironment::getWater() {
	return mWater.get();
}

void CaelumEnvironment::setTime(int hour, int minute, int second) {
	if (mCaelumSystem && mCaelumSystem->getUniversalClock()) {
		Eris::DateTime currentServerTime = mCalendar.now();
		if (currentServerTime.valid()) {
			mCaelumSystem->getUniversalClock()->setGregorianDateTime(TIME_FIXED_YEAR, TIME_FIXED_MONTH, TIME_FIXED_DAY, hour, minute, second);
		} else {
			int year, month, day, _hour, _minute, _second;
			Ember::TimeHelper::getLocalTime(year, month, day, _hour, _minute, _second);
			mCaelumSystem->getUniversalClock()->setGregorianDateTime(TIME_FIXED_YEAR, TIME_FIXED_MONTH, TIME_FIXED_DAY, hour, minute, second);
		}
	}
}

void CaelumEnvironment::setTime(int seconds) {
	if (mCaelumSystem && mCaelumSystem->getUniversalClock()) {
		Eris::DateTime currentServerTime = mCalendar.now();
		if (currentServerTime.valid()) {
			mCaelumSystem->getUniversalClock()->setGregorianDateTime(TIME_FIXED_YEAR, TIME_FIXED_MONTH, TIME_FIXED_DAY, 0, 0, seconds);
		} else {
			int year, month, day, hour, minute, _second;
			Ember::TimeHelper::getLocalTime(year, month, day, hour, minute, _second);
			mCaelumSystem->getUniversalClock()->setGregorianDateTime(TIME_FIXED_YEAR, TIME_FIXED_MONTH, TIME_FIXED_DAY, 0, 0, seconds);
		}
	}
}

void CaelumEnvironment::setTimeMultiplier(float multiplier) {
	if (mCaelumSystem && mCaelumSystem->getUniversalClock()) {
		mCaelumSystem->getUniversalClock()->setTimeScale(multiplier);
	}
}

float CaelumEnvironment::getTimeMultiplier() const {
	if (mCaelumSystem && mCaelumSystem->getUniversalClock()) {
		return mCaelumSystem->getUniversalClock()->getTimeScale();
	}
	return 0;
}

void CaelumEnvironment::setWorldPosition(float longitudeDegrees, float latitudeDegrees) {
	if (mCaelumSystem) {
		mCaelumSystem->setObserverLatitude(Ogre::Degree(latitudeDegrees));
		mCaelumSystem->setObserverLongitude(Ogre::Degree(longitudeDegrees));
	}
}

void CaelumEnvironment::runCommand(const std::string& command, const std::string& args) {
	if (SetCaelumTime == command) {
		Tokeniser tokeniser;
		tokeniser.initTokens(args);
		std::string hourString = tokeniser.nextToken();
		std::string minuteString = tokeniser.nextToken();

		int hour = ::Ogre::StringConverter::parseInt(hourString);
		int minute = ::Ogre::StringConverter::parseInt(minuteString);
		setTime(hour, minute, 0);
	}
}

}

}
}
