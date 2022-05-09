//
// C++ Interface: Environment
//
// Description:
//
//
// Author: Erik Ogenvik <erik@ogenvik.org>, (C) 2006
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
#ifndef EMBER_OGRE_ENVIRONMENTENVIRONMENT_H
#define EMBER_OGRE_ENVIRONMENTENVIRONMENT_H

#include "IEnvironmentProvider.h"
#include "components/ogre/EmberOgrePrerequisites.h"
#include "framework/ConsoleCommandWrapper.h"
#include <wfmath/vector.h>
#include <sigc++/signal.h>
#include <sigc++/trackable.h>

namespace Ember {
class EmberEntity;
namespace OgreView {
namespace Terrain {
class TerrainManager;
}

/**
 * @brief Namespace for environment related classes and activities.
 * With "environment" we refer to things like the weather, the ocean, the foliage etc.
 */
namespace Environment {

class Forest;


/**
 * @author Erik Ogenvik <erik@ogenvik.org>
 *
 * @brief Represents the environment.
 *
 * The environment is made up of a sun, a sky, water, a forest and foliage handler and fog.
 *
 * Note that the actual instances used are provided by an instance of IEnvironmentProvider.
 */
class Environment : public ConsoleObject, public ILightning, public virtual sigc::trackable {
public:

	/**
	 * @brief Ctor.
	 * @param sceneMgr The main scene manager.
	 * @param terrainManager The main terrain manager.
	 * @param provider Main environment provider.
	 * @param fallbackProvider A fallback provider which is used if the main provider for some reason fails to create the environment (if for instance the hardware doesn't support it).
	 */
	Environment(Ogre::SceneManager& sceneMgr,
				Terrain::TerrainManager& terrainManager,
				std::unique_ptr<IEnvironmentProvider> provider,
				std::unique_ptr<IEnvironmentProvider> fallbackProvider);

	~Environment() override;

	/**
	 *    Reimplements the ConsoleObject::runCommand method
	 * @param command
	 * @param args
	 */
	void runCommand(const std::string& command, const std::string& args) override;

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
	 * TODO: rename to setTimeInSeconds
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
	 * @brief Gets the direction of the main light, in world space.
	 * @returns The direction of the main light, in world space.
	 */
	WFMath::Vector<3> getMainLightDirection() const override;

	/**
	 * @brief Gets the default ambient light colour.
	 * @returns The default ambient light colour.
	 */
	Ogre::ColourValue getAmbientLightColour() const override;


	/**
	 * @brief Emitted when the world ambient light is changed.
	 * The first param is the new colour value of the ambient light.
	 */
	sigc::signal<void, const Ogre::ColourValue&> EventUpdatedAmbientLight;

	/**
	 * @brief Emitted when the environment has been setup.
	 */
	sigc::signal<void> EventEnvironmentSetup;

private:

	std::unique_ptr<IEnvironmentProvider> mProvider;
	std::unique_ptr<IEnvironmentProvider> mFallbackProvider;
	std::unique_ptr<Forest> mForest;

	/**
	 * Called when the terrain handler signals that the terrain is enabled.
	 *
	 * This signals that we should show the firmament.
	 *
	 * @param entity The entity to which the terrain belongs.
	 */
	void terrainEnabled(EmberEntity& entity);

	/**
	 * Called when the terrain handler signals that the terrain is disabled.
	 *
	 * This signals that we should hdie the firmament.
	 */
	void terrainDisabled();
};

inline ISun* Environment::getSun() {
	return mProvider->getSun();
}

inline ISky* Environment::getSky() {
	return mProvider->getSky();
}

inline IFog* Environment::getFog() {
	return mProvider->getFog();
}

inline IWater* Environment::getWater() {
	return mProvider->getWater();
}

inline Forest* Environment::getForest() {
	return mForest.get();
}

}

}

}

#endif
