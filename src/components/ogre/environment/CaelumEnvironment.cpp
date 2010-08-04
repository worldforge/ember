//
// C++ Implementation: CaelumEnvironment
//
// Description:
//
//
// Author: Erik Hjortsberg <erik.hjortsberg@gmail.com>, (C) 2006
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
// along with this program; if not, write to the Free Software
// Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.//
//
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "CaelumEnvironment.h"
#include "caelum/include/Caelum.h"
#include "CaelumSky.h"
#include "CaelumSun.h"
#include "Water.h"
#include "SimpleWater.h"
//#include "HydraxWater.h"
#include "framework/Tokeniser.h"
//#include "Caelum/include/CaelumSystem.h"

#include "services/time/TimeService.h"

namespace EmberOgre
{

namespace Environment
{

CaelumEnvironment::CaelumEnvironment(Ogre::SceneManager *sceneMgr, Ogre::RenderWindow* window, Ogre::Camera& camera) :
	SetCaelumTime("set_caelumtime", this, "Sets the caelum time. parameters: <hour> <minute>"), mCaelumSystem(0), mSceneMgr(sceneMgr), mWindow(window), mCamera(camera), mSky(0), mSun(0), mWater(0)

//,mLensFlare(camera, sceneMgr)
{
	//	sceneMgr->setAmbientLight(Ogre::ColourValue(0.3f, 0.3f, 0.3f));
	sceneMgr->setAmbientLight(Ogre::ColourValue(0.6, 0.6, 0.6));
	//		setupCaelum(root, sceneMgr, window , camera);
	/*		mLensFlare.setNode(mCaelumSystem->getSun()-	} catch (const Ogre::Exception& ex) {
	 S_LOG_FAILURE("Could not load caelum. Message: " << ex.getFullDescription());
	 }
	 >getNode());
	 mLensFlare.initialize();*/
	//		mLensFlare.setVisible(false);
	//Ogre::::Root::getSingleton().addFrameListener(this);
}

CaelumEnvironment::~CaelumEnvironment()
{
	delete mSky;
	delete mSun;
	delete mWater;
	mWindow->removeListener(mCaelumSystem);
	if (mCaelumSystem) {
		mCaelumSystem->shutdown(true);

		// 		delete mCaelumSystem; //calling shutdown() will delete the instance, so no need to do it again
	}
}

void CaelumEnvironment::createEnvironment()
{
	try {
		setupCaelum(Ogre::Root::getSingletonPtr(), mSceneMgr, mWindow, mCamera);
	} catch (const std::exception& ex) {
		S_LOG_FAILURE("Could not load caelum." << ex);
		throw ;
	}
	try {
		setupWater();
	} catch (const std::exception& ex) {
		S_LOG_FAILURE("Could not load water." << ex);
		delete mWater;
		mWater = 0;
	}

}

void CaelumEnvironment::setupWater()
{

	//mWater = new Water(mCamera, mSceneMgr);
	//	mWater = new HydraxWater(mCamera, *mSceneMgr);
	///NOTE: we default to simple water for now since there are a couple of performance problems with hydrax
	mWater = new SimpleWater(mCamera, *mSceneMgr);
	if (mWater->isSupported()) {
		mWater->initialize();
	} else {
		delete mWater;
		mWater = new SimpleWater(mCamera, *mSceneMgr);
		mWater->initialize();
	}

}

void CaelumEnvironment::setupCaelum(::Ogre::Root *root, ::Ogre::SceneManager *sceneMgr, ::Ogre::RenderWindow* window, ::Ogre::Camera& camera)
{

	mCaelumSystem = new Caelum::CaelumSystem(root, sceneMgr, Caelum::CaelumSystem::CAELUM_COMPONENTS_NONE);

	try {
		mCaelumSystem->setSkyDome(new Caelum::SkyDome(mSceneMgr, mCaelumSystem->getCaelumCameraNode()));
	} catch (const Caelum::UnsupportedException& ex) {
		S_LOG_WARNING("Error when creating Caelum sky dome." << ex);
	}
	try {
		mCaelumSystem->setSun(new Caelum::SphereSun(mSceneMgr, mCaelumSystem->getCaelumCameraNode()));
		mSun = new CaelumSun(*this, mCaelumSystem->getSun());
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

	mCaelumSystem->setManageSceneFog (true);
	mCaelumSystem->setManageAmbientLight(true);
	mCaelumSystem->setGlobalFogDensityMultiplier (0.005);
	mCaelumSystem->setMinimumAmbientLight(Ogre::ColourValue(0.2, 0.2, 0.2, 1.0));

	mCaelumSystem->setEnsureSingleShadowSource(true); ///we want to use only one shadow caster source, for now at least
	mCaelumSystem->setEnsureSingleLightSource(true); ///We want to only use the brightest light source only, even if another is closer. This is to make sure the main light is taken from the sun instead of the moon (which will result in a dark landscape).

	mSky = new CaelumSky(*this);

	/// Register all to the render window
	mCaelumSystem->attachViewport(window->getViewport(0));
	window->addListener(mCaelumSystem);

	/// Set time acceleration to fit with real world time
	mCaelumSystem->getUniversalClock()->setTimeScale (1);

	int year, month, day, hour, minute, second;
	bool usingServerTime = Ember::EmberServices::getSingleton().getTimeService()->getServerTime(year, month, day, hour, minute, second);

	if (!usingServerTime) {
		S_LOG_WARNING("Could not get server time, using local time for environment.");
	} else {
		mCaelumSystem->getUniversalClock ()->setGregorianDateTime (year, month, day, hour, minute, second);
	}

	///little hack here. We of course want to use the server time, but currently when you log in when it's dark, you won't see much, so in order to show the world in it's full glory we'll try to set the time to day time
	/*
	 if (hour < 6) {
	 hour = 6;
	 } else if (hour > 16) {
	 hour = 15;
	 }
	 */

	///advance it one second to force it to do initial updating, since other subsystems such as the terrain rendering depends on the sun postions etc.
	Ogre::FrameEvent ev;
	ev.timeSinceLastEvent = 1;
	ev.timeSinceLastFrame = 1;
	mCaelumSystem->updateSubcomponents(1000);

	//This will make caelum update itself automatically each frame
	Ogre::Root::getSingleton().addFrameListener(mCaelumSystem);
}

ISun* CaelumEnvironment::getSun()
{
	return mSun;
}

ISky* CaelumEnvironment::getSky()
{
	return mSky;
}

IFog* CaelumEnvironment::getFog()
{
	return mSky;
	//return mFog;
}

IWater* CaelumEnvironment::getWater()
{
	return mWater;
}

void CaelumEnvironment::setTime(int hour, int minute, int second)
{
	if (mCaelumSystem && mCaelumSystem->getUniversalClock ()) {
		int year, month, day, _hour, _minute, _second;
		Ember::EmberServices::getSingleton().getTimeService()->getServerTime(year, month, day, _hour, _minute, _second);

		mCaelumSystem->getUniversalClock ()->setGregorianDateTime(year, month, day, hour, minute, second);
	}
}

void CaelumEnvironment::setTime(int seconds)
{
	if (mCaelumSystem && mCaelumSystem->getUniversalClock ()) {
		int year, month, day, _hour, _minute, _second;
		Ember::EmberServices::getSingleton().getTimeService()->getServerTime(year, month, day, _hour, _minute, _second);

		mCaelumSystem->getUniversalClock ()->setGregorianDateTime(year, month, day, 0, 0, seconds);
	}
}

void CaelumEnvironment::setWorldPosition(float longitudeDegrees, float latitudeDegrees)
{
	if (mCaelumSystem) {
		mCaelumSystem->setObserverLatitude(Ogre::Degree(latitudeDegrees));
		mCaelumSystem->setObserverLongitude(Ogre::Degree(longitudeDegrees));
	}
}

void CaelumEnvironment::runCommand(const std::string &command, const std::string &args)
{
	if (SetCaelumTime == command) {
		Ember::Tokeniser tokeniser;
		tokeniser.initTokens(args);
		std::string hourString = tokeniser.nextToken();
		std::string minuteString = tokeniser.nextToken();

		int hour = ::Ogre::StringConverter::parseInt( hourString);
		int minute = ::Ogre::StringConverter::parseInt( minuteString);
		setTime(hour, minute);
	}
}

}

}
