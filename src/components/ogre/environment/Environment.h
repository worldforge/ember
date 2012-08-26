//
// C++ Interface: Environment
//
// Description:
//
//
// Author: Erik Hjortsberg <erik.hjortsberg@gmail.com>, (C) 2006
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

#include "IEnvironmentProvider.h"
#include "components/ogre/EmberOgrePrerequisites.h"
#include "framework/ConsoleObject.h"
#include <sigc++/signal.h>

namespace Ember
{
namespace OgreView
{
namespace Terrain
{
class TerrainManager;
}

/**
 * @brief Namespace for environment related classes and activities.
 * With "environment" we refer to things like the weather, the ocean, the foliage etc.
 */
namespace Environment
{

class Forest;


/**
 * @author Erik Hjortsberg <erik.hjortsberg@gmail.com>
 *
 * @brief Represents the environment.
 *
 * The environment is made up of a sun, a sky, water, a forest and foliage handler and fog.
 *
 * Note that the actual instances used are provided by an instance of IEnvironmentProvider.
 */
class Environment: public ConsoleObject
{
public:

	/**
	 * @brief Ctor.
	 * @param sceneMgr The main scene manager.
	 * @param terrainManager The main terrain manager.
	 * @param provider Main environment provider.
	 * @param fallbackProvider A fallback provider which is used if the main provider for some reason fails to create the environment (if for instance the hardware doesn't support it).
	 */
	Environment(Ogre::SceneManager& sceneMgr, Terrain::TerrainManager& terrainManager, IEnvironmentProvider* provider, IEnvironmentProvider* fallbackProvider = 0);

	~Environment();

	/**
	 *    Reimplements the ConsoleObject::runCommand method
	 * @param command
	 * @param args
	 */
	virtual void runCommand(const std::string &command, const std::string &args);

	const ConsoleCommandWrapper SetTime;
	const ConsoleCommandWrapper SetFogDensity;
	const ConsoleCommandWrapper SetAmbientLight;

	ISun* getSun();
	ISky* getSky();
	IFog* getFog();
	IWater* getWater();
	Forest* getForest();

	/**
	 * @brief Sets the current time.
	 * @param hour The hour.
	 * @param minute The minute.
	 * @param second The second.
	 */
	void setTime(int hour, int minute, int second = 0);

	/**
	 * @brief Sets the current time.
	 * @param seconds Seconds since midnight.
	 */
	void setTime(int seconds);

	/**
	 * @brief Sets the time multiplier, i.e. how much the time of the environment will progress per real time second.
	 * @param multiplier The time multiplier.
	 */
	void setTimeMultiplier(float multiplier);

	/**
	 * @brief Gets the time multiplier, i.e. how much the time of the environment will progress per real time second.
	 * @return The time multiplier.
	 */
	float getTimeMultiplier() const;

	/**
	 * @brief Sets whether the firmament is enabled or not.
	 * @param enabled Whether the firmament is enabled or not.
	 */
	void setFirmamentEnabled(bool enabled);

	/**
	 *    changes the ambient light
	 * @param colour
	 */
	void setAmbientLight(const Ogre::ColourValue& colour);

	/**
	 * @brief Sets the position of the world.
	 * @param longitudeDegrees The longitude, as degrees.
	 * @param latitudeDegrees The latitude, as degrees.
	 */
	void setWorldPosition(float longitudeDegrees, float latitudeDegrees);

	/**
	 * @brief Sets whether the water (ocean) is enabled or not.
	 * @param enabled True if the water should enabled.
	 */
	void setWaterEnabled(bool enabled);

	/**
	 * @brief Emitted when the world ambient light is changed.
	 * The first param is the new colour value of the ambient light.
	 */
	sigc::signal<void, const Ogre::ColourValue&> EventUpdatedAmbientLight;

private:

	IEnvironmentProvider* mProvider, *mFallbackProvider, *mEnabledFirmamentProvider;
	Forest* mForest;
};

inline ISun* Environment::getSun()
{
	return mProvider->getSun();
}

inline ISky* Environment::getSky()
{
	return mProvider->getSky();
}

inline IFog* Environment::getFog()
{
	return mProvider->getFog();
}

inline IWater* Environment::getWater()
{
	return mProvider->getWater();
}

inline Forest* Environment::getForest()
{
	return mForest;
}

}

}

}

#endif
