/*
 * Copyright (C) 2014 Peter Szucs <peter.szucs.dev@gmail.com>
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
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#ifndef OGREPLUGINLOADER_H
#define OGREPLUGINLOADER_H

#include "components/ogre/EmberOgrePrerequisites.h"

#include <OgrePlugin.h>
#include <framework/Singleton.h>

#include <string>
#include <map>
#include <vector>

namespace Ember
{
namespace OgreView
{

/**
 * @brief Loads Ogre plugins! Hides OS and build differences.
 */
class OgrePluginLoader
{
public:
	OgrePluginLoader();
	virtual ~OgrePluginLoader();

	/// @brief Adds a directory to the plugin search path list. It will be ignored on static builds.
	void addPluginDir(const std::string& dir);

	/// @brief Loads the plugin.
	/// @param pluginName The basename of the plugin without path, extension, prefix and postfix names!
	/// @returns Returns the loaded plugin. If the plugin is already loaded, it will return the loaded instance. Returns nullptr on failure.
	Ogre::Plugin* loadPlugin(const std::string& pluginName);

	/// @brief returns the plugin.
	/// @param pluginName The basename of the plugin without path, extension, prefix and postfix names!
	/// @returns Returns the loaded plugin. Returns nullptr if not found.
	Ogre::Plugin* getPlugin(const std::string& pluginName);

	/// @brief Unloads the plugin.
	/// @param pluginName The basename of the plugin without path, extension, prefix and postfix names!
	/// @returns Returns true on success.
	bool unloadPlugin(const std::string& pluginName);

	/// @brief Unloads all plugins.
	void unloadPlugins();
private:

	Ogre::Plugin* loadDynPlugin(const std::string& pluginName);

	/// @brief Sets OS dependent paths for dynamic builds. Can be extended with addPluginDir() later.
	void initPluginPaths();

	/// @brief Creates a list of plugins on static builds.
	void prepareStaticPlugins();

	/// Known Plugins, which are initialized and can be used.
	typedef std::map<std::string, Ogre::Plugin*> PluginInstanceMap;
	PluginInstanceMap mPlugins;

	/// List of plugin search dirs. When searching plugin, the list will be processed in inverse order.
	std::vector<std::string> mPluginDirs;

	std::string mPluginExtension;
};

}
}
#endif // ifndef OGREPLUGINLOADER_H
