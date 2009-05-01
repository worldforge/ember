//
// C++ Interface: SimpleEnvironment
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
#ifndef EMBEROGRE_ENVIRONMENTSIMPLEENVIRONMENT_H
#define EMBEROGRE_ENVIRONMENTSIMPLEENVIRONMENT_H

#include "Environment.h"


namespace Ogre
{
class SceneManager;
class RenderWindow;
class Camera;
}

namespace EmberOgre {

namespace Environment {

/**
A very simple sun which always will return the same direction.
*/
class SimpleSun : public ISun
{
public:
	SimpleSun(Ogre::SceneManager *sceneMgr);
	virtual void setAmbientLight(const Ogre::ColourValue& colour);
	virtual Ogre::Vector3 getSunDirection() const;
protected:
	Ogre::Light* mMainLight;
};

/**
A very simple sky which won't do anything currently.
*/
class SimpleSky : public ISky
{
public:
protected:
};

/**
A very simple fog which will always return a density of 1.0
*/
class SimpleFog : public IFog
{
public:
	SimpleFog(Ogre::SceneManager *sceneMgr);
	virtual void setDensity(float density);
	virtual float getDensity() const;
protected:
};

class Water;



/**
	@author Erik Hjortsberg <erik.hjortsberg@gmail.com>
	
	A very simple environment which can be used as a fallback environment if a more advanced environment fails to load.
*/
class SimpleEnvironment : public IEnvironmentProvider
{
public:
    SimpleEnvironment(Ogre::SceneManager *sceneMgr, Ogre::RenderWindow* window, Ogre::Camera& camera);

    virtual ~SimpleEnvironment();
    
	virtual void createEnvironment();

	virtual ISun* getSun();
	virtual ISky* getSky();
	virtual IFog* getFog();
	virtual IWater* getWater();

    virtual void setTime(int hour, int minute, int second = 0);
	virtual void setTime(int seconds);
	
	/**
	 * @brief Sets the position of the world.
	 * @param longitudeDegrees The longitude, as degrees.
	 * @param latitudeDegrees The latitude, as degrees.
	 */
	virtual void setWorldPosition(float longitudeDegrees, float latitudeDegrees);
	
protected:
	Ogre::SceneManager *mSceneMgr;
	Ogre::RenderWindow* mWindow;
	Ogre::Camera& mCamera;
	
	SimpleSun* mSun;
	SimpleSky* mSky;
	SimpleFog* mFog;
	IWater* mWater;

};

}

}

#endif
