//
// C++ Implementation: Sun
//
// Description: 
//
//
// Author: Erik Hjortsberg <erik@katastrof.nu>, (C) 2004
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
#include "Sun.h"

#include "framework/Tokeniser.h"

#include "framework/ConsoleBackend.h"
#include "../EmberOgre.h"

namespace EmberOgre {


Sun::Sun(Ogre::Camera* camera, Ogre::SceneManager* sceneMgr):
SetSunPosition("setsunposition", this, "Set the position of the sun.")
,SetSunColour("setsuncolour", this, "Set the colour of the sun.")
,SetAmbientLight("setambientlight", this, "Set the ambient light of the world.")
,mLensFlare(Ogre::Vector3(-500,300,-350), camera, sceneMgr)
{
	mSun = sceneMgr->createLight("SunLight");
	mSun->setType(Ogre::Light::LT_DIRECTIONAL);
	mSunNode = EmberOgre::getSingleton().getWorldSceneNode()->createChildSceneNode();
	mSunNode->attachObject(mSun);
  
///disable for now
// 	try {
// 		Ogre::ParticleSystem* sunParticle = sceneMgr->createParticleSystem("Sun", "Space/Sun"); 
// 		mSunNode->attachObject(sunParticle);   
// 	} catch (const Ogre::Exception& ex) {
// 		S_LOG_FAILURE("Error when creating sun. Message: " << ex.getFullDescription());
// 	}
	
	setSunPosition(Ogre::Vector3(-500,300,-350));
	setSunColour(Ogre::ColourValue(1, 1, 0.7)); //yellow
	//mSun->setSpecularColour(1, 1, 0.7); //yellow
	mSun->setCastShadows(true);
	mSun->setAttenuation(1000000, 1, 0, 0);
	
	//  sceneMgr->setAmbientLight(Ogre::ColourValue(0.5, 0.5, 0.35));
	setAmbientLight(Ogre::ColourValue(0.6, 0.6, 0.6));
	
	Ogre::Root::getSingleton().addFrameListener(this);
	mLensFlare.setVisible(true);

}


Sun::~Sun()
{
	Ogre::Root::getSingleton().removeFrameListener(this);

}

void Sun::runCommand(const std::string &command, const std::string &args)
{
	if(SetSunPosition == command)
	{
		Ember::Tokeniser tokeniser;
		tokeniser.initTokens(args);
		std::string x = tokeniser.nextToken();
		std::string y = tokeniser.nextToken();
		std::string z = tokeniser.nextToken();
		
		if (x == "" || y == "" || z == "") {
			return;
		} else {
			Ogre::Vector3 position(Ogre::StringConverter::parseReal(x),Ogre::StringConverter::parseReal(y),Ogre::StringConverter::parseReal(z));
			setSunPosition(position);
		}
	} else if (SetSunColour == command)
	{
		Ember::Tokeniser tokeniser;
		tokeniser.initTokens(args);
		std::string r = tokeniser.nextToken();
		std::string b = tokeniser.nextToken();
		std::string g = tokeniser.nextToken();
		
		if (r == "" || b == "" || g == "") {
			return;
		} else {
			Ogre::ColourValue colour(Ogre::StringConverter::parseReal(r),Ogre::StringConverter::parseReal(b),Ogre::StringConverter::parseReal(g));
			setSunColour(colour);
		}
	
	} else if (SetAmbientLight == command)
	{
		Ember::Tokeniser tokeniser;
		tokeniser.initTokens(args);
		std::string r = tokeniser.nextToken();
		std::string b = tokeniser.nextToken();
		std::string g = tokeniser.nextToken();
		
		if (r == "" || b == "" || g == "") {
			return;
		} else {
			Ogre::ColourValue colour(Ogre::StringConverter::parseReal(r),Ogre::StringConverter::parseReal(b),Ogre::StringConverter::parseReal(g));
			setAmbientLight(colour);
		}
	
	}
}
	
bool Sun::frameEnded(const Ogre::FrameEvent & event)
{
	mLensFlare.update();
	return true;
}



void Sun::setSunPosition(const Ogre::Vector3& position) {
	//mSun->setPosition(position);
	mSunNode->setPosition(position);
	Ogre::Vector3 dir = -mSunNode->getPosition();
	dir.normalise();
	mSun->setDirection(dir);
	mLensFlare.setLightPosition(position);
	EventUpdatedSunPosition.emit(this, position);
}

void Sun::setSunColour(const Ogre::ColourValue& colour) {
	mSun->setDiffuseColour(colour);
	mLensFlare.setBurstColour(colour);
	mLensFlare.setHaloColour(colour);
	EventUpdatedSunColour.emit(this, colour);
}

void Sun::setAmbientLight(const Ogre::ColourValue& colour) {
	EmberOgre::getSingleton().getSceneManager()->setAmbientLight(colour);
	EventUpdatedAmbientLight.emit(this, colour);
}

};
