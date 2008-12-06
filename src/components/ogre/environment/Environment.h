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

class Forest;

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


/**
@brief Interface for all water techniques. Call isSupported() first to see if the water can be created, and then initialize() to set it all up.
@author Erik Hjortsberg <erik@worldforge.org>
*/
class IWater
{
public:
	virtual ~IWater() {}
	
	/**
	 * @brief Performs checks to see whether this technique is supported on the current hardware.
	 * @return True if the water technique is supported.
	 */
	virtual bool isSupported() const = 0;
	/**
	 * @brief Initializes the water. You must call this in order for the water to show up.
	 * @return True if the water technique could be setup, else false.
	 */
	virtual bool initialize() = 0;
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
    Environment(IEnvironmentProvider* provider, IEnvironmentProvider* fallbackProvider = 0);

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
	inline Forest* getForest();
    
    
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

	IEnvironmentProvider* mProvider, *mFallbackProvider;
	Forest* mForest;
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

Forest* Environment::getForest()
{
	return mForest;
}

}

}

#endif
