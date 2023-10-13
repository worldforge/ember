//
// C++ Implementation: SimpleEnvironment
//
// Description:
//
//
// Author: Erik Ogenvik <erik@ogenvik.org>, (C) 2008
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
#include "SimpleEnvironment.h"
#include "Water.h"
#include "SimpleWater.h"
#include "components/ogre/Convert.h"
#include <OgreSceneManager.h>
#include <OgreRenderWindow.h>
#include <OgreSceneNode.h>

namespace Ember
{
namespace OgreView
{

namespace Environment
{

SimpleSun::SimpleSun(Ogre::SceneManager *sceneMgr)
{

	mMainLight = sceneMgr->createLight("SimpleSun");
    sceneMgr->getRootSceneNode()->createChildSceneNode()->attachObject(mMainLight);
	mMainLight->setType(Ogre::Light::LT_DIRECTIONAL);
	// HDR power scale, REALLY bright:
// 	mMainLight->setPowerScale (10);

	mMainLight->setVisible(true);
	mMainLight->setDiffuseColour(Ogre::ColourValue(0.9f, 0.9f, 0.9f));
	mMainLight->setSpecularColour(Ogre::ColourValue(0.9f, 0.9f, 0.9f));
    mMainLight->getParentSceneNode()->setDirection(Ogre::Vector3(1, -1, 0), Ogre::Node::TS_WORLD);
}

void SimpleSun::setAmbientLight(const Ogre::ColourValue& colour)
{
}

Ogre::Vector3 SimpleSun::getSunDirection() const
{
	return {-0.5f, -1.0f, -0.5f};
}

WFMath::Vector<3> SimpleSun::getMainLightDirection() const
{
	return Convert::toWF<WFMath::Vector<3>>(getSunDirection());
}

Ogre::ColourValue SimpleSun::getAmbientLightColour() const
{
	return mMainLight->getDiffuseColour();
}

SimpleFog::SimpleFog(Ogre::SceneManager *sceneMgr)
{
	sceneMgr->setFog(Ogre::FOG_EXP2, Ogre::ColourValue(0.8f, 0.8f, 0.8f), 0.005f);

}

void SimpleFog::setDensity(float density)
{
}

float SimpleFog::getDensity() const
{
	return 1.0f;
}

SimpleEnvironment::SimpleEnvironment(Ogre::SceneManager *sceneMgr, Ogre::RenderWindow* window, Ogre::Camera& camera) :
		mSceneMgr(sceneMgr),
		mWindow(window),
		mCamera(camera)
{
}

SimpleEnvironment::~SimpleEnvironment() = default;

void SimpleEnvironment::createFirmament()
{
	mSun = std::make_unique<SimpleSun>(mSceneMgr);
	mSky = std::make_unique<SimpleSky>();
	mFog = std::make_unique<SimpleFog>(mSceneMgr);
}

void SimpleEnvironment::destroyFirmament()
{
	mSun.reset();
	mSky.reset();
	mFog.reset();
}

void SimpleEnvironment::setWaterEnabled(bool enabled)
{
	if (enabled) {
		if (!mWater) {
			mWater = std::make_unique<SimpleWater>(mCamera, *mSceneMgr, *mWindow);
			mWater->initialize();

		}
	} else {
		if (mWater) {
			mWater.reset();
		}
	}
}

ISun* SimpleEnvironment::getSun()
{
	return mSun.get();
}

ISky* SimpleEnvironment::getSky()
{
	return mSky.get();
}

IFog* SimpleEnvironment::getFog()
{
	return mFog.get();
}

IWater* SimpleEnvironment::getWater()
{
	return mWater.get();
}

void SimpleEnvironment::setTime(int hour, int minute, int second)
{
}

void SimpleEnvironment::setTime(int seconds)
{
}

void SimpleEnvironment::setTimeMultiplier(float multiplier)
{
}

float SimpleEnvironment::getTimeMultiplier() const
{
	return 0;
}

void SimpleEnvironment::setWorldPosition(float longitudeDegrees, float latitudeDegrees)
{
}

}

}
}
