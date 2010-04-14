//
// C++ Implementation: SimpleEnvironment
//
// Description:
//
//
// Author: Erik Hjortsberg <erik.hjortsberg@gmail.com>, (C) 2008
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

#include "SimpleEnvironment.h"
#include "Water.h"
#include "SimpleWater.h"
#include "components/ogre/Convert.h"
#include <OgreSceneManager.h>
#include <OgreLight.h>
namespace EmberOgre {

namespace Environment {

SimpleSun::SimpleSun(Ogre::SceneManager *sceneMgr)
{

	mMainLight = sceneMgr->createLight ("SimpleSun");
	mMainLight->setType (Ogre::Light::LT_DIRECTIONAL);
	// HDR power scale, REALLY bright:
// 	mMainLight->setPowerScale (10);

	mMainLight->setVisible(true);
	mMainLight->setDiffuseColour(Ogre::ColourValue(0.9, 0.9, 0.9));
	mMainLight->setSpecularColour(Ogre::ColourValue(0.9, 0.9, 0.9));
	mMainLight->setDirection (Ogre::Vector3(1, -1, 0));
}

void SimpleSun::setAmbientLight(const Ogre::ColourValue& colour)
{
}

Ogre::Vector3 SimpleSun::getSunDirection() const
{
	return Ogre::Vector3(-0.5f, -1.0f, -0.5f);
}

WFMath::Vector<3> SimpleSun::getMainLightDirection() const
{
	return Convert::toWF<WFMath::Vector<3> >(getSunDirection());
}

Ogre::ColourValue SimpleSun::getAmbientLightColour() const
{
	return mMainLight->getDiffuseColour();
}



SimpleFog::SimpleFog(Ogre::SceneManager *sceneMgr)
{
	sceneMgr->setFog(Ogre::FOG_EXP2, Ogre::ColourValue(0.8, 0.8, 0.8), 0.005);

}


void SimpleFog::setDensity(float density)
{
}

float SimpleFog::getDensity() const
{
	return 1.0f;
}


SimpleEnvironment::SimpleEnvironment(Ogre::SceneManager *sceneMgr, Ogre::RenderWindow* window, Ogre::Camera& camera)
: mSceneMgr(sceneMgr), mWindow(window), mCamera(camera)
,mSun(0), mSky(0), mFog(0), mWater(0)
{
}


SimpleEnvironment::~SimpleEnvironment()
{
	delete mSun;
	delete mSky;
	delete mFog;
	delete mWater;
}

void SimpleEnvironment::createEnvironment()
{
	mSun = new SimpleSun(mSceneMgr);
	mSky = new SimpleSky();
	mFog = new SimpleFog(mSceneMgr);
	mWater = new SimpleWater(mCamera, *mSceneMgr);
	mWater->initialize();
}

ISun* SimpleEnvironment::getSun()
{
	return mSun;
}

ISky* SimpleEnvironment::getSky()
{
	return mSky;
}

IFog* SimpleEnvironment::getFog()
{
	return mFog;
}

IWater* SimpleEnvironment::getWater()
{
	return mWater;
}

void SimpleEnvironment::setTime(int hour, int minute, int second)
{
}

void SimpleEnvironment::setTime(int seconds)
{
}

void SimpleEnvironment::setWorldPosition(float longitudeDegrees, float latitudeDegrees)
{
}



}

}
