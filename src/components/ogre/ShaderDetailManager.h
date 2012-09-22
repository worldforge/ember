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

#include "sigc++/connection.h"

#include "string"

namespace Ember
{
namespace OgreView
{
class GraphicalChangeAdapter;
class ShaderManager;

/**
 * @brief Handles changes in shader level.
 * Acts as a subcomponent of the automatic handling of graphics system.
 */
class ShaderDetailManager
{
public:
	/**
	 * @brief Constructor.
	 */
	ShaderDetailManager(GraphicalChangeAdapter& graphicalChangeAdapter, ShaderManager& shaderManager);

	/**
	 * @brief Destructor.
	 */
	~ShaderDetailManager();

	/**
	 * @brief Initializes the shader detail manager.
	 * The manager starts listening for graphics detail change requests.
	 */
	void initialize();

	/**
	 * @brief Sets the shader scheme to the required level.
	 * @param level The needed shader level (high, medium, low).
	 * If the level doesn't exist, it will switch to default.
	 */
	void changeShaderLevel(const std::string& level);

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
	 * @brief Checks level against this component's threshold value to determine change in material lod.
	 * This function is used to listen to the changes required by the automatic graphics detail system.
	 */
	bool changeLevel(float level);

	/**
	 * @brief Steps up the shader scheme level.
	 * @returns Whether shader scheme could be stepped up.
	 */
	bool stepUpShaderLevel();

	/**
	 * @brief Steps down the shader scheme level.
	 * @returns Whether shader scheme could be stepped down.
	 */
	bool stepDownShaderLevel();

	/**
	 * The maximum shader level this component can switch to.
	 */
	std::string mMaxShaderLevel;

	/**
	 * The minimum shader level this component can switch to.
	 */
	std::string mMinShaderLevel;

	/**
	 * The current shader level.
	 */
	std::string mShaderLevel;

	/**
	 * The threshold fps change after which the shader detail is changed to respond to performance change requested.
	 */
	float mShaderThresholdLevel;

	/**
	 * Holds the reference to the connection to the changeRequired signal. Used to disconnect the signal on destruction of this class or to pause the functioning of this component.
	 */
	sigc::connection mChangeRequiredConnection;

	/**
	 * Reference to the graphical change adapter through which graphics detail changes are requested.
	 */
	GraphicalChangeAdapter& mGraphicalChangeAdapter;

	/**
	 * Reference to the Shader Manager used to make scheme changes.
	 */
	ShaderManager& mShaderManager;

};

}
}
