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

#include "WidgetDefinitions.h"

#include "framework/LoggingInstance.h"
#include "services/EmberServices.h"
#include "services/config/ConfigService.h"
#include "framework/FileSystemObserver.h"
#include "framework/MainLoopController.h"

#include <CEGUI/WindowManager.h>

#ifndef WF_USE_WIDGET_PLUGINS

#include "ServerWidget.h"
#include "Quit.h"
#include "InspectWidget.h"
#include "Help.h"
#include "ContainerWidget.h"
#include "IngameChatWidget.h"
#include "EntityCreatorWidget.h"
#include "AssetsSyncWidget.h"

#endif


namespace Ember::OgreView::Gui {

WidgetDefinitions::WidgetDefinitions() = default;

WidgetDefinitions::~WidgetDefinitions() {
	auto pluginDirPath = ConfigService::getSingleton().getPluginDirectory();
	Ember::FileSystemObserver::getSingleton().remove_directory(pluginDirPath);
}


void WidgetDefinitions::registerWidgets(GUIManager& guiManager) {

#ifndef WF_USE_WIDGET_PLUGINS
	S_LOG_INFO("Loading Widgets statically");
	mPlugins.emplace("ServerWidget", PluginEntry{ServerWidget::registerWidget(guiManager)});
	mPlugins.emplace("Quit", PluginEntry{Quit::registerWidget(guiManager)});
	mPlugins.emplace("InspectWidget", PluginEntry{InspectWidget::registerWidget(guiManager)});
	mPlugins.emplace("Help", PluginEntry{Help::registerWidget(guiManager)});
	mPlugins.emplace("ContainerWidget", PluginEntry{ContainerWidget::registerWidget(guiManager)});
	mPlugins.emplace("IngameChatWidget", PluginEntry{IngameChatWidget::registerWidget(guiManager)});
	mPlugins.emplace("EntityCreatorWidget", PluginEntry{EntityCreatorWidget::registerWidget(guiManager)});
	mPlugins.emplace("AssetsSyncWidget", PluginEntry{AssetsSyncWidget::registerWidget(guiManager)});
#else
	auto pluginDirPath = ConfigService::getSingleton().getPluginDirectory();
	Ember::FileSystemObserver::getSingleton().add_directory(pluginDirPath, [&](const Ember::FileSystemObserver::FileSystemEvent& event) {
		if (event.ev.type == boost::asio::dir_monitor_event::added || event.ev.type == boost::asio::dir_monitor_event::modified) {
			auto I = mPlugins.find(event.ev.path);
			if (I != mPlugins.end()) {
				//Mark the path and wait two seconds for file system to settle down.
				mDirtyPluginPaths.emplace(event.ev.path);
				MainLoopController::getSingleton().getEventService().runOnMainThreadDelayed([&]() {
					for (auto& path: mDirtyPluginPaths) {
						auto J = mPlugins.find(path);
						if (J != mPlugins.end()) {
							S_LOG_INFO("Reloading plugin '" << path.string() << "'.");
							//First remove the existing plugin.
							mPlugins.erase(J);
							CEGUI::WindowManager::getSingleton().cleanDeadPool(); //Need to make sure there's no reference to the erased plugin.
							//Then add the new one
							registerPlugin(guiManager, path);
						}
					}
					mDirtyPluginPaths.clear();

				}, std::chrono::seconds(2));
			}
		}
	});


	S_LOG_INFO("Loading Widgets dynamically");
	registerPluginWithName(guiManager, "ServerWidget");
	registerPluginWithName(guiManager, "Quit");
	registerPluginWithName(guiManager, "InspectWidget");
	registerPluginWithName(guiManager, "Help");
	registerPluginWithName(guiManager, "ContainerWidget");
	registerPluginWithName(guiManager, "IngameChatWidget");
	registerPluginWithName(guiManager, "EntityCreatorWidget");
	registerPluginWithName(guiManager, "AssetsSyncWidget");

#endif
}

void WidgetDefinitions::registerPluginWithName(GUIManager& guiManager, const std::string& pluginName) {
	auto pluginDirPath = ConfigService::getSingleton().getPluginDirectory();

	auto pluginPath = pluginDirPath / (std::string(PLUGIN_PREFIX) + pluginName + std::string(PLUGIN_SUFFIX));

	registerPlugin(guiManager, pluginPath);
}


void WidgetDefinitions::registerPlugin(GUIManager& guiManager, const boost::filesystem::path& pluginPath) {
#ifdef WF_USE_WIDGET_PLUGINS

	try {
		auto registerFn = boost::dll::import_alias<std::function<void()>(GUIManager&)>(
				pluginPath, "registerWidget"
		);
		S_LOG_INFO("Creating Widget Plugin from '" << pluginPath.string() << "'.");
		auto disconnectFn = registerFn(guiManager);
		PluginEntry pluginEntry{pluginPath, std::move(registerFn), std::move(disconnectFn)};
		mPlugins.emplace(pluginPath, std::move(pluginEntry));
	} catch (const std::exception& ex) {
		S_LOG_FAILURE("Error when loading plugin '" << pluginPath.string() << "': " << ex);
	}
#endif
}


WidgetDefinitions::PluginEntry::~PluginEntry() {
	if (pluginCallback) {
#ifdef WF_USE_WIDGET_PLUGINS
		S_LOG_INFO("Shutting down Widget Plugin at '" << path.string() << "'.");
#endif
		pluginCallback();
		CEGUI::WindowManager::getSingleton().cleanDeadPool(); //Need to make sure there's no reference to the plugin.
	}
}

}


