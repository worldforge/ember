/*
 * Copyright (C) 2012 Arjun Kumar <arjun1991@gmail.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#include <sigc++/signal.h>
#include <sigc++/connection.h>

#include <string>

namespace varconf
{
class Variable;
}

namespace Ember
{
class ConfigListenerContainer;

namespace OgreView
{
class GraphicalChangeAdapter;

namespace Environment
{

class Foliage;
/**
 * @brief This class manages the detail level of foliage by responding to the changeRequired signal from IGraphicalManager.
 * Acts as a sub-component of the automatic handling of graphics system.
 */
class FoliageDetailManager
{
public:
	/**
	 * @brief Constructor.
	 * @param foliage The foliage instance.
	 * @param graphicalChangeAdapter The graphical change adapter.
	 */
	FoliageDetailManager(Foliage& foliage, GraphicalChangeAdapter& graphicalChangeAdapter);

	/**
	 * @brief Destructor.
	 */
	~FoliageDetailManager();

	/**
	 * @brief Initializes the foliage detail manager.
	 * The manager starts listening for graphics detail change required signals and acting on them.
	 */
	void initialize();

	/**
	 * @brief Signal sent out when this component makes a change in the detail level of foliage.
	 */
	sigc::signal<void, int> EventFoliageLevelChanged;

	/**
	 * @brief Used to set the density of all foliage.
	 * @param density The new density factor of all foliage in float, where 1 implies normal or full density and 0 implies no density or no grass. 
	 * @return True if foliage change was possible.
	 */
	bool setFoliageDensity(float density);

	/**
	 * @brief Used to set the furthest distance foliage is visible at as a factor.
	 * @param distance The new far distance for the nearest foliage technique where 1 implies normal distance and 0 implies minimum distance.
	 * @return True if far distance change was possible.
	 */
	bool setFoliageDistance(float distance);

	/**
	 * @brief This can be used to stop this component responding to change requests.
	 */
	void pause();

	/**
	 * @brief This can be used to restart this component responding to change requests.
	 */
	void unpause();

protected:

	/**
	 * @brief Steps the density of foliage up by value passed.
	 * @param step The amount by which the density of the foliage is stepped up.
	 * @returns True if step up was possible.
	 */
	bool stepUpFoliageDensity(float step);

	/**
	 * @brief Steps the density of foliage down by value passed.
	 * @param step The amount by which the density of the foliage is stepped down.
	 * @returns True if step down was possible.
	 */
	bool stepDownFoliageDensity(float step);

	/**
	 * @brief Steps up the foliage far distance.
	 * @param step The amount by which far distance of the foliage is stepped up.
	 * @return True if step up was possible.
	 */
	bool stepUpFoliageDistance(float step);

	/**
	 * @brief Steps down the foliage far distance.
	 * @param step The amount by which far distance of the foliage is stepped down.
	 * @return True if step down was possible.
	 */
	bool stepDownFoliageDistance(float step);

	/**
	 * @brief Changes the detail level of foliage if the asked level is above threshold level of this class.
	 * @return True if a change is possible and is made.
	 */
	bool changeLevel(float level);

	/**
	 * @brief Connected to the config service to listen for foliage density settings.
	 */
	void Config_FoliageDensity(const std::string& section, const std::string& key, varconf::Variable& variable);

	/**
	 * @brief Connected to the config service to listen for foliage far distance settings.
	 */
	void Config_FoliageFarDistance(const std::string& section, const std::string& key, varconf::Variable& variable);

	/**
	 * @brief The main foliage instance.
	 */
	Foliage& mFoliage;

	/**
	 * The threshold level of this subcomponent passing which it responds by changing the detail level of foliage.
	 */
	float mThresholdLevel;

	/**
	 * The value by which density of a foliage is stepped up or down while adjusting foliage detail.
	 */
	float mDefaultDensityStep;

	/**
	 * Holds an updated density value factor of all foliage.
	 */
	float mUpdatedDensity;

	/**
	 * The factor value by which far distance of a foliage is stepped up or down while adjusting foliage detail.
	 */
	float mDefaultDistanceStep;

	/**
	 * Holds the current/updated far distance factor of all foliage.
	 */
	float mFarDistance;

	/**
	 * Maximum far distance factor.
	 */
	float mMaxFarDistance;

	/**
	 * Minimum far distance factor.
	 */
	float mMinFarDistance;

	/**
	 * Holds the reference to the connection to the changeRequired signal. Used to disconnect the signal on destruction of this class or to pause the functioning of this component.
	 */
	sigc::connection mChangeRequiredConnection;

	/**
	 * Reference to the graphical change adapter through which graphics detail changes are requested.
	 */
	GraphicalChangeAdapter& mGraphicalChangeAdapter;

	/**
	 * @brief Used to listen for configuration changes.
	 */
	ConfigListenerContainer* mConfigListenerContainer;

};

}
}
}
