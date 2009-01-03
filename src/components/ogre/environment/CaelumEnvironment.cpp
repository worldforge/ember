//
// C++ Implementation: CaelumEnvironment
//
// Description: 
//
//
// Author: Erik Hjortsberg <erik@katastrof.nu>, (C) 2006
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
#include "HydraxWater.h"
#include "framework/Tokeniser.h"
//#include "caelum/include/CaelumSystem.h"

#include "services/time/TimeService.h"


namespace EmberOgre {

namespace Environment {



CaelumEnvironment::CaelumEnvironment(Ogre::SceneManager *sceneMgr, Ogre::RenderWindow* window, Ogre::Camera& camera)
: 
 SetCaelumTime("set_caelumtime",this, "Sets the caelum time. parameters: <hour> <minute>")
, mCaelumSystem(0)
, mSceneMgr(sceneMgr)
, mWindow(window)
, mCamera(camera)
, mSky(0)
, mSun(0)
, mWater(0)

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
		setupCaelum( Ogre::Root::getSingletonPtr(), mSceneMgr, mWindow, mCamera);
	} catch (const Ogre::Exception& ex) {
		S_LOG_FAILURE("Could not load caelum. Message: " << ex.getFullDescription());
		throw;
	}
	setupWater();
	
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
	/// Pick components to use
	///We'll skip the ground fog for now...
	caelum::CaelumSystem::CaelumComponent componentMask =
			static_cast<caelum::CaelumSystem::CaelumComponent> (
			caelum::CaelumSystem::CAELUM_COMPONENT_SKY_COLOUR_MODEL |
			caelum::CaelumSystem::CAELUM_COMPONENT_SUN |
			caelum::CaelumSystem::CAELUM_COMPONENT_SOLAR_SYSTEM_MODEL |
			caelum::CaelumSystem::CAELUM_COMPONENT_SKY_DOME |
			caelum::CaelumSystem::CAELUM_COMPONENT_POINT_STARFIELD |
			caelum::CaelumSystem::CAELUM_COMPONENT_CLOUDS |
			caelum::CaelumSystem::CAELUM_COMPONENT_MOON |
// 			caelum::CaelumSystem::CAELUM_COMPONENT_GROUND_FOG |
			0);

	caelum::CaelumSystem::CaelumComponent componentMaskFallback =
			static_cast<caelum::CaelumSystem::CaelumComponent> (
			caelum::CaelumSystem::CAELUM_COMPONENT_SKY_COLOUR_MODEL |
			caelum::CaelumSystem::CAELUM_COMPONENT_SUN |
			caelum::CaelumSystem::CAELUM_COMPONENT_SOLAR_SYSTEM_MODEL |
			caelum::CaelumSystem::CAELUM_COMPONENT_SKY_DOME |
			caelum::CaelumSystem::CAELUM_COMPONENT_IMAGE_STARFIELD |	// Point starfield require shaders
			caelum::CaelumSystem::CAELUM_COMPONENT_CLOUDS |
//			caelum::CaelumSystem::CAELUM_COMPONENT_MOON |				// Moon would be ugly without shaders
			0);

	try {
		mCaelumSystem = new caelum::CaelumSystem (root, sceneMgr, componentMask, false);
	} catch (const Ogre::Exception& ex) {
		S_LOG_FAILURE("Could not load main caelum technique, will try fallback. Message: " << ex.getFullDescription());

		sceneMgr->getRootSceneNode()->removeAndDestroyChild("CaelumRoot");
		mCaelumSystem = new caelum::CaelumSystem (root, sceneMgr, componentMaskFallback, false);
	}


	mCaelumSystem->setManageSceneFog (true);
	mCaelumSystem->setManageAmbientLight(true);
	mCaelumSystem->setGlobalFogDensityMultiplier (0.005);
	mCaelumSystem->setMinimumAmbientLight(Ogre::ColourValue(0.2, 0.2, 0.2, 1.0));

	///Get the sky dome for  Create a sky dome CaelumSky
	mDome = mCaelumSystem->getSkyDome();
	
	/// Set up some star field options
// 	mCaelumSystem->getPointStarfield ()->setInclination (::Ogre::Degree (13));

	/// Setup sun options
	if (mCaelumSystem->getSun ()) {
		mSun = new CaelumSun(*this, mCaelumSystem->getSun());
	} else {
		///use a simple sun object
	}
	
	mCaelumSystem->setEnsureSingleShadowSource(true); ///we want to use only one shadow caster source, for now at least
	mCaelumSystem->setEnsureSingleLightSource(true); ///We want to only use the brightest light source only, even if another is closer. This is to make sure the main light is taken from the sun instead of the moon (which will result in a dark landscape).

	mSky = new CaelumSky(*this);


	/// Register all to the render window
	window->addListener (mCaelumSystem);
	
	
	/// Set time acceleration to fit with real world time
	mCaelumSystem->getUniversalClock ()->setTimeScale (1);

	int year, month, day, hour, minute, second;
	bool usingServerTime = Ember::EmberServices::getSingleton().getTimeService()->getServerTime(year, month, day, hour, minute, second);
	
	if (!usingServerTime) {
		S_LOG_WARNING("Could not get server time, using local time for environment.");
	}
	
	///little hack here. We of course want to use the server time, but currently when you log in when it's dark, you won't see much, so in order to show the world in it's full glory we'll try to set the time to day time
	/*
	if (hour < 6) {
		hour = 6;
	} else if (hour > 16) {
		hour = 15;
	}
	*/
	
	
	mCaelumSystem->getUniversalClock ()->setGregorianDateTime (year, month, day, hour, minute, second);
	
  	mCaelumSystem->getUniversalClock()->setUpdateRate( 1 / (24 * 60)); //update every minute
	
	
	///advance it one second to force it to do initial updating, since other subsystems such as the terrain rendering depends on the sun postions etc.
	Ogre::FrameEvent ev;
	ev.timeSinceLastEvent = 1;
	ev.timeSinceLastFrame = 1;
	mCaelumSystem->updateSubcomponents(1000);
	
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
	int year, month, day, _hour, _minute, _second;
	Ember::EmberServices::getSingleton().getTimeService()->getServerTime(year, month, day, _hour, _minute, _second);
	
	mCaelumSystem->getUniversalClock ()->setGregorianDateTime(year, month, day, hour, minute, second);
}

void CaelumEnvironment::setTime(int seconds)
{
	int year, month, day, _hour, _minute, _second;
	Ember::EmberServices::getSingleton().getTimeService()->getServerTime(year, month, day, _hour, _minute, _second);
	
	mCaelumSystem->getUniversalClock ()->setGregorianDateTime(year, month, day, 0, 0, seconds);
}


void CaelumEnvironment::setWorldPosition(float longitudeDegrees, float latitudeDegrees)
{
	if (mCaelumSystem) {
		mCaelumSystem->getSolarSystemModel ()->setObserverLatitude (Ogre::Degree(latitudeDegrees));
		mCaelumSystem->getSolarSystemModel ()->setObserverLongitude(Ogre::Degree(longitudeDegrees));
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
