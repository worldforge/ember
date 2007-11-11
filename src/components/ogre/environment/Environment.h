//
// C++ Interface: Environment
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
#ifndef EMBER_OGRE_ENVIRONMENTENVIRONMENT_H
#define EMBER_OGRE_ENVIRONMENTENVIRONMENT_H

#include "components/ogre/EmberOgrePrerequisites.h"
#include "framework/ConsoleObject.h"

namespace EmberOgre {

namespace Environment {

class ISun
{
public:
	virtual ~ISun() {}
	virtual void setAmbientLight(const Ogre::ColourValue& colour) = 0;
	virtual Ogre::Vector3 getSunDirection() const = 0;

};

class ISky
{
public:
};

class IFog
{
public:
	virtual ~IFog() {}
	virtual void setDensity(float density) = 0;
	virtual float getDensity() const = 0;
};

class IWater
{
public:
};

class IEnvironmentProvider
{
public:

	virtual ~IEnvironmentProvider() {}

	virtual void createEnvironment() = 0;

	virtual ISun* getSun() = 0;
	virtual ISky* getSky() = 0;
	virtual IFog* getFog() = 0;
	virtual IWater* getWater() = 0;

    virtual void setTime(int hour, int minute, int second = 0) = 0;
	virtual void setTime(int seconds) = 0;

};

/**
	@author Erik Hjortsberg <erik@katastrof.nu>
*/
class Environment : public Ember::ConsoleObject
{
public:
    Environment(IEnvironmentProvider* provider);

    ~Environment();
 	
	/**
	 *    Reimplements the ConsoleObject::runCommand method
	 * @param command 
	 * @param args 
	 */
	virtual	void runCommand(const std::string &command, const std::string &args);
 	
 	const Ember::ConsoleCommandWrapper SetTime;
	const Ember::ConsoleCommandWrapper SetFogDensity;
	const Ember::ConsoleCommandWrapper SetAmbientLight;
   
	inline ISun* getSun();
	inline ISky* getSky();
	inline IFog* getFog();
	inline IWater* getWater();
    
    
    void setTime(int hour, int minute, int second = 0);
	void setTime(int seconds);
    
    void initialize();
    
	/**
	 *    changes the ambient light
	 * @param colour 
	 */
	void setAmbientLight(const Ogre::ColourValue& colour);
    
    
   	/**
	* emitted when the world ambient light is changed
	*/
	sigc::signal<void, const Ogre::ColourValue&> EventUpdatedAmbientLight;

private:

	IEnvironmentProvider* mProvider;
// 	ISun* mSun;
// 	ISky* mSky;
// 	IFog* mFog;
// 	IWater* mWater;	

};

ISun* Environment::getSun()
{
	return mProvider->getSun();
}

ISky* Environment::getSky()
{
	return mProvider->getSky();
}

IFog* Environment::getFog()
{
	return mProvider->getFog();
}

IWater* Environment::getWater()
{
	return mProvider->getWater();
}



}

}

#endif
