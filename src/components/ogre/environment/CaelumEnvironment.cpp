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
#include "framework/Tokeniser.h"
//#include "caelum/include/CaelumSystem.h"

namespace EmberOgre {

namespace Environment {


CaelumEnvironment::CaelumEnvironment(Ogre::SceneManager *sceneMgr, Ogre::RenderWindow* window, Ogre::Camera* camera)
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
	delete mCaelumSystem;
}

void CaelumEnvironment::createEnvironment()
{
	try {
		setupCaelum( Ogre::Root::getSingletonPtr(), mSceneMgr, mWindow, mCamera);
	} catch (const Ogre::Exception& ex) {
		S_LOG_FAILURE("Could not load caelum. Message: " << ex.getFullDescription());
	}
	setupWater();
	
}


void CaelumEnvironment::setupWater()
{
	mWater = new Water(mCamera, mSceneMgr);
}

void CaelumEnvironment::setupCaelum(::Ogre::Root *root, ::Ogre::SceneManager *sceneMgr, ::Ogre::RenderWindow* window, ::Ogre::Camera* camera)
{
	
	/// Pick components to use
	///We'll skip the ground fog for now...
	caelum::CaelumSystem::CaelumComponent componentMask = 
			static_cast<caelum::CaelumSystem::CaelumComponent> (
			caelum::CaelumSystem::CAELUM_COMPONENT_SKY_COLOUR_MODEL |
			caelum::CaelumSystem::CAELUM_COMPONENT_SUN |
			caelum::CaelumSystem::CAELUM_COMPONENT_SOLAR_SYSTEM_MODEL |
			caelum::CaelumSystem::CAELUM_COMPONENT_SKY_DOME |
			caelum::CaelumSystem::CAELUM_COMPONENT_STARFIELD |
			caelum::CaelumSystem::CAELUM_COMPONENT_CLOUDS |
// 			caelum::CaelumSystem::CAELUM_COMPONENT_GROUND_FOG |
			0);	
	mCaelumSystem = new caelum::CaelumSystem (root, sceneMgr, componentMask, false);


	mCaelumSystem->setManageSceneFog (true);
	mCaelumSystem->setSceneFogDensityMultiplier (0.005);

	///Get the sky dome for  Create a sky dome CaelumSky
	mDome = mCaelumSystem->getSkyDome();
	
	/// Set up some star field options
	mCaelumSystem->getStarfield ()->setInclination (::Ogre::Degree (13));

	/// Setup sun options
	if (mCaelumSystem->getSun ()) {
		mSun = new CaelumSun(*this, mCaelumSystem->getSun());
	} else {
		///use a simple sun object
	}

	mSky = new CaelumSky(*this);


	/// Register all to the render window
	window->addListener (mCaelumSystem);
	
	
	/// Set time acceleration to fit with real world time
	mCaelumSystem->getUniversalClock ()->setTimeScale (1);

	/// Set some time parameters
	///TODO: use the time from the server
	///Currently we use the local time
	time_t t = time (&t);
	struct tm *t2 = localtime (&t);
	int hour = t2->tm_hour;
	///little hack here. We of course want to use the server time, but currently when you log in when it's dark, you won't see much, so in order to show the world in it's full glory we'll try to set the time to day time
	if (t2->tm_hour < 6) {
		hour = 6;
	} else if (t2->tm_hour > 16) {
		hour = 15;
	}
	mCaelumSystem->getUniversalClock ()->setGregorianDateTime (t2->tm_year, t2->tm_mon, t2->tm_mday, hour, t2->tm_min, t2->tm_sec);
	
	///greenwich
	mCaelumSystem->getSolarSystemModel ()->setObserverLatitude  (Ogre::Degree(0));
	mCaelumSystem->getSolarSystemModel ()->setObserverLongitude(Ogre::Degree(0));
	
/*	//stockholm
	mCaelumSystem->getSolarSystemModel ()->setObserverLatitude  (Ogre::Degree(59 + 18.0 / 60));
	mCaelumSystem->getSolarSystemModel ()->setObserverLongitude(Ogre::Degree(31 + 113.0 / 60));*/
	
//  	mCaelumSystem->getUniversalClock()->setUpdateRate( 1 / (24 * 60)); //update every minute
	
	
	///advance it one second to force it to do initial updating, since other subsystems such as the terrain rendering depends on the sun postions etc.
	Ogre::FrameEvent ev;
	ev.timeSinceLastEvent = 1;
	ev.timeSinceLastFrame = 1;
	mCaelumSystem->frameStarted(ev);
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
	return 0;
	//return mFog;
}

IWater* CaelumEnvironment::getWater()
{
	return 0;
}

void CaelumEnvironment::setTime(int hour, int minute, int second)
{
	///use the local year, month and day
	time_t t = time (&t);
	struct tm *t2 = localtime (&t);
	
	mCaelumSystem->getUniversalClock ()->setGregorianDateTime (t2->tm_year, t2->tm_mon, t2->tm_mday, hour, minute, second);
}

void CaelumEnvironment::setTime(int seconds)
{
	///use the local year, month and day
	time_t t = time (&t);
	struct tm *t2 = localtime (&t);
	
	mCaelumSystem->getUniversalClock ()->setGregorianDateTime (t2->tm_year, t2->tm_mon, t2->tm_mday, 0, 0, seconds);
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
