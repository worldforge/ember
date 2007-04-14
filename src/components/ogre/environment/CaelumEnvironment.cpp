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
#include "CaelumEnvironment.h"
#include "caelum/include/Caelum.h"
#include "CaelumSky.h"
#include "CaelumSun.h"
#include "Water.h"
#include "framework/Tokeniser.h"
//#include "caelum/include/CaelumSystem.h"

namespace EmberOgre {

namespace Environment {

class CloudsUpdater : public caelum::CaelumListener {
	protected:
		float mTime;
		Ogre::MaterialPtr mMaterial;

	public:
		CloudsUpdater (float time, Ogre::MaterialPtr material) : mTime (time), mMaterial(material) { }
		bool caelumStarted (const Ogre::FrameEvent &e, caelum::CaelumSystem *sys) {
			Ogre::Technique* tech = mMaterial->getBestTechnique ();
			if (tech) {
				Ogre::Pass* pass = tech->getPass(0);
				if (pass) {
					mTime += e.timeSinceLastFrame * 30;
					if (pass->hasVertexProgram()) {
						pass->getVertexProgramParameters ()->setNamedConstant ("sunDirection", sys->getSun ()->getSunDirection ());
					}
					if (pass->hasFragmentProgram()) {
						pass->getFragmentProgramParameters ()->setNamedConstant ("sunDirection", sys->getSun ()->getSunDirection ());
						pass->getFragmentProgramParameters ()->setNamedConstant ("sunColour", sys->getSun ()->getSunColour ());
						pass->getFragmentProgramParameters ()->setNamedConstant ("time", mTime);
					}
					return true;
				}
			}
			return true;
		}
};


CaelumEnvironment::CaelumEnvironment(Ogre::SceneManager *sceneMgr, Ogre::RenderWindow* window, Ogre::Camera* camera)
: 
 SetCaelumTime("set_caelumtime",this, "Sets the caelum time. parameters: <hour> <minute>")
, mCaelumSystem(0)
, mCaelumModel(0)
, mSceneMgr(sceneMgr)
, mWindow(window)
, mCamera(camera)
, mSky(0)
, mSun(0)
, mWater(0)

//,mLensFlare(camera, sceneMgr)
{
	sceneMgr->setAmbientLight(Ogre::ColourValue(0.3f, 0.3f, 0.3f));
//		sceneMgr->setAmbientLight(Ogre::ColourValue(0.6, 0.6, 0.6));
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
	delete mCaelumModel;
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
	mCaelumSystem = new caelum::CaelumSystem (root, sceneMgr, false);
	caelum::SunPositionModel *spm = new caelum::SimpleSunPositionModel (Ogre::Degree (13));
	mCaelumSystem->getSun ()->setSunPositionModel (spm);

	// Create and configure the sky colours model to use
	
	mCaelumModel = new caelum::StoredImageElvBasedSkyColourModel ();
	mCaelumSystem->setSkyColourModel (mCaelumModel);	// Call this before changing the gradients image!!
	static_cast<caelum::StoredImageElvBasedSkyColourModel *>(mCaelumModel)->setSkyGradientsImage ("EarthClearSky2.png");

	mCaelumSystem->setManageFog (true);
// 	static_cast<caelum::StoredImageSkyColourModel *>(mCaelumModel)->setFogColoursImage ("EarthClearSkyFog.png");
	static_cast<caelum::StoredImageElvBasedSkyColourModel *>(mCaelumModel)->setFogDensity (0.005);

	// Create a sky dome
//	mDome = mCaelumSystem->createSkyDome ();
	mDome = mCaelumSystem->getSkyDome();
	

	// Create a starfield
//	window->addListener (mCaelumSystem->createStarfield ("Starfield.jpg"));
	mCaelumSystem->getStarfield ()->setInclination (::Ogre::Degree (13));

	mCaelumSystem->getSkyDome()->setFarRadius(10000);
	mCaelumSystem->getStarfield()->setFarRadius(10000);

	// Register all to the render window
	window->addListener (mCaelumSystem);
	

	// Set some time parameters
	time_t t = time (&t);
	struct tm *t2 = localtime (&t);
	mCaelumSystem->setTimeScale (1);
	int hour = t2->tm_hour;
	//little hack here. We of course want to use the server time, but currently when you log in when it's dark, you won't see much, so in order to show the world in it's full glory we'll try to set the time to day time
	if (t2->tm_hour < 6) {
		hour = 6;
	} else if (t2->tm_hour > 16) {
		hour = 15;
	}
	
	float time = 3600 * hour + 60 * t2->tm_min + t2->tm_sec;
	
	mCaelumSystem->setLocalTime (time);
	mCaelumSystem->setUpdateRate( 1 / (24 * 60)); //update every minute
	
	mSky = new CaelumSky(*this, mCaelumModel, mDome);
	mSun = new CaelumSun(*this, mCaelumSystem->getSun());
	
	std::string cloudMaterialName = "Cirrus";
	Ogre::MaterialPtr mat = static_cast<Ogre::MaterialPtr >(Ogre::MaterialManager::getSingleton ().getByName (cloudMaterialName));

	// Register our cloud updater
	if (!mat.isNull()) {
		mSceneMgr->setSkyPlane (true, Ogre::Plane (Ogre::Vector3::NEGATIVE_UNIT_Y, -1000), cloudMaterialName, 1000, 1, false, .1, 10, 10, caelum::RESOURCE_GROUP_NAME);
		mCaelumSystem->addListener (new CloudsUpdater (time, mat));
	}

/*	sceneMgr->setSkyPlane (true, Ogre::Plane (Ogre::Vector3::NEGATIVE_UNIT_Y, -100), "Altocumulus", 1000, 10, false);
	mCaelumSystem->addListener (new CloudsUpdater ());*/
	
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
	mCaelumSystem->setLocalTime (3600 * hour + 60 * minute + second);
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
