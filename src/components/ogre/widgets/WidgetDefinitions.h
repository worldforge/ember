//
// C++ Interface: WidgetDefinitions
//
// Description: 
//
//
// Author: Erik Ogenvik <erik@ogenvik.org>, (C) 2005
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
#ifndef WIDGETDEFINITIONS_H
#define WIDGETDEFINITIONS_H

#include <boost/dll.hpp>
#include <vector>
#include <set>
#include <map>
#include "WidgetPlugin.h"

namespace Ember {
namespace OgreView {
class GUIManager;
namespace Gui {

class Widget;

/**
@author Erik Ogenvik

Utility class for registering Widgets.
If you create a new widget, make sure you add it to this class, else it won't be linked and you cannot create it dynamically.
*/
class WidgetDefinitions{
public:
    WidgetDefinitions();

    ~WidgetDefinitions();

    void registerWidgets(GUIManager& guiManager);

    /**
     * Registers a plugin with the supplied name. The system will try to find it by looking for a shared object in the file system,
     * where plugins should be installed.
     * @param guiManager
     * @param pluginName
     */
    void registerPluginWithName(GUIManager& guiManager, const std::string& pluginName);

    /**
     * Registers a plugin from a shared object file.
     * @param guiManager
     * @param pluginPath
     */
	void registerPlugin(GUIManager& guiManager, const boost::filesystem::path& pluginPath);

	/**
	 * An entry for a loaded plugin.
	 * The pluginCallback will automatically be called on destruction.
	 */
    struct PluginEntry {
    	PluginEntry(PluginEntry&& rhs) = default;
    	~PluginEntry();
		PluginEntry& operator=(PluginEntry&& rhs) = default;
    	boost::filesystem::path path;
		WidgetPluginFunction pluginFn; //We need to hold on to this, since it the dynamic library's lifetime is bound to it.
		WidgetPluginCallback pluginCallback; //A deregistering function
    };

	static_assert(std::is_move_constructible<PluginEntry>::value, "PluginEntry must be move constructible.");

private:
	std::map<boost::filesystem::path, PluginEntry> mPlugins;

	std::set<boost::filesystem::path> mDirtyPluginPaths;

};
}
}

}

#endif
