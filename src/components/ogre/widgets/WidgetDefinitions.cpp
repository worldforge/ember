//
// C++ Implementation: WidgetDefinitions
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
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif


#include <framework/MainLoopController.h>
#include "WidgetDefinitions.h"

#include "ServerWidget.h"
#include "InspectWidget.h"
#include "IngameChatWidget.h"
#include "Help.h"
#include "Quit.h"
#include "MeshPreview.h"
#include "ContainerWidget.h"

#include "services/EmberServices.h"
#include "services/config/ConfigService.h"
#include "framework/FileSystemObserver.h"


namespace Ember {
namespace OgreView {
namespace Gui {

WidgetDefinitions::WidgetDefinitions() {
	WidgetLoader::registerWidgetFactory("Widget", &WidgetLoader::createWidgetInstance<Widget>);
	WidgetLoader::registerWidgetFactory("InspectWidget", &WidgetLoader::createWidgetInstance<InspectWidget>);
	WidgetLoader::registerWidgetFactory("Help", &WidgetLoader::createWidgetInstance<Help>);
	WidgetLoader::registerWidgetFactory("MeshPreview", &WidgetLoader::createWidgetInstance<MeshPreview>);

}

WidgetDefinitions::~WidgetDefinitions() {
	auto pluginDirPath = EmberServices::getSingleton().getConfigService().getPluginDirectory();
	Ember::FileSystemObserver::getSingleton().remove_directory(pluginDirPath);
}


void WidgetDefinitions::registerWidgets(GUIManager& guiManager) {
	ContainerWidget::registerWidget(guiManager);
	Gui::IngameChatWidget::registerWidget(guiManager);


	auto pluginDirPath = EmberServices::getSingleton().getConfigService().getPluginDirectory();
	Ember::FileSystemObserver::getSingleton().add_directory(pluginDirPath, [&](const Ember::FileSystemObserver::FileSystemEvent& event) {
		if (event.ev.type == boost::asio::dir_monitor_event::added || event.ev.type == boost::asio::dir_monitor_event::modified) {
			auto I = mPlugins.find(event.ev.path);
			if (I != mPlugins.end()) {
				//Mark the path and wait two seconds for file system to settle down.
				mDirtyPluginPaths.emplace(event.ev.path);
				MainLoopController::getSingleton().getEventService().runOnMainThreadDelayed([this, &guiManager]() {
					for (auto& path: mDirtyPluginPaths) {
						auto J = mPlugins.find(path);
						if (J != mPlugins.end()) {
							S_LOG_INFO("Reloading plugin '" << path.string() << "'.");
							//First remove the existing plugin.
							mPlugins.erase(J);
							//Then add the new one
							registerPlugin(guiManager, path);
						}
					}
					mDirtyPluginPaths.clear();

				}, boost::posix_time::seconds(2));


			}


		}
	});
	registerPluginWithName(guiManager, "ServerWidget");

}

void WidgetDefinitions::registerPluginWithName(GUIManager& guiManager, const std::string& pluginName) {
	auto pluginDirPath = EmberServices::getSingleton().getConfigService().getPluginDirectory();

	auto pluginPath = pluginDirPath / (std::string(PLUGIN_PREFIX) + pluginName + std::string(PLUGIN_SUFFIX));

	registerPlugin(guiManager, pluginPath);
}


void WidgetDefinitions::registerPlugin(GUIManager& guiManager, const boost::filesystem::path& pluginPath) {

	auto registerFn = boost::dll::import<std::function<void()>(GUIManager&)>(
			pluginPath, "registerWidget"
	);
	auto disconnectFn = registerFn(guiManager);
	PluginEntry pluginEntry{std::move(registerFn), std::move(disconnectFn)};
	mPlugins.emplace(pluginPath, std::move(pluginEntry));
}


WidgetDefinitions::PluginEntry::~PluginEntry() {
	if (pluginCallback) {
		pluginCallback();
	}
}

}
}
}
