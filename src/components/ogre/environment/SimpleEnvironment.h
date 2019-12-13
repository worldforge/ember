//
// C++ Interface: SimpleEnvironment
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
#ifndef EMBEROGRE_ENVIRONMENTSIMPLEENVIRONMENT_H
#define EMBEROGRE_ENVIRONMENTSIMPLEENVIRONMENT_H

#include "IEnvironmentProvider.h"

namespace Ogre
{
class SceneManager;
class RenderWindow;
class Camera;
class Light;
}

namespace Ember
{
namespace OgreView
{

namespace Environment
{

/**
 A very simple sun which always will return the same direction.
 */
class SimpleSun: public ISun
{
public:
	explicit SimpleSun(Ogre::SceneManager *sceneMgr);
	void setAmbientLight(const Ogre::ColourValue& colour) override;
	Ogre::Vector3 getSunDirection() const override;
	WFMath::Vector<3> getMainLightDirection() const override;

	Ogre::ColourValue getAmbientLightColour() const override;

protected:
	Ogre::Light* mMainLight;
};

/**
 A very simple sky which won't do anything currently.
 */
class SimpleSky: public ISky
{
public:
protected:
};

/**
 A very simple fog which will always return a density of 1.0
 */
class SimpleFog: public IFog
{
public:
	explicit SimpleFog(Ogre::SceneManager *sceneMgr);
	void setDensity(float density) override;
	float getDensity() const override;
protected:
};

class Water;

/**
 @author Erik Ogenvik <erik@ogenvik.org>

 A very simple environment which can be used as a fallback environment if a more advanced environment fails to load.
 */
class SimpleEnvironment: public IEnvironmentProvider
{
public:
	SimpleEnvironment(Ogre::SceneManager *sceneMgr, Ogre::RenderWindow* window, Ogre::Camera& camera);

	~SimpleEnvironment() override;

	void createFirmament() override;
	void destroyFirmament() override;

	void setWaterEnabled(bool enabled) override;

	ISun* getSun() override;
	ISky* getSky() override;
	IFog* getFog() override;
	IWater* getWater() override;

	void setTime(int hour, int minute, int second = 0) override;
	void setTime(int seconds) override;

	void setTimeMultiplier(float multiplier) override;

	float getTimeMultiplier() const override;

	/**
	 * @brief Sets the position of the world.
	 * @param longitudeDegrees The longitude, as degrees.
	 * @param latitudeDegrees The latitude, as degrees.
	 */
	void setWorldPosition(float longitudeDegrees, float latitudeDegrees) override;

protected:
	Ogre::SceneManager *mSceneMgr;
	Ogre::RenderWindow* mWindow;
	Ogre::Camera& mCamera;

	std::unique_ptr<SimpleSun> mSun;
	std::unique_ptr<SimpleSky> mSky;
	std::unique_ptr<SimpleFog> mFog;
	std::unique_ptr<IWater> mWater;

};

}

}

}

#endif
