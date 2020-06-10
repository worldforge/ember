//
// C++ Implementation: Help
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

#include "Help.h"
#include "QuickHelp.h"
#include "../GUIManager.h"
#include "services/config/ConfigService.h"

#include "../EmberOgre.h"

#include <CEGUI/Window.h>



using namespace CEGUI;
namespace Ember {
namespace OgreView {
namespace Gui {

WidgetPluginCallback Help::registerWidget(GUIManager& guiManager) {

	struct State {
		std::shared_ptr<Help> widget;
	};
	auto state = std::make_shared<State>();

	state->widget = std::make_shared<Gui::Help>(guiManager);
	//Just hold on to an instance.
	return [state]() mutable {
		state.reset();
	};

}

Help::Help(GUIManager& guiManager)
		: Widget(guiManager),
		  HelpCommand("help", this, "Display the help.") {
	loadMainSheet("HelpWidget.layout", "Help/");

	enableCloseButton();

	ConfigService& configSrv = EmberServices::getSingleton().getConfigService();

	if (configSrv.itemExists("general", "startuphelp") && ((bool) configSrv.getValue("general", "startuphelp"))) {
		show();
	}

	//connect to the creation of the avatar, since we want to show a help blurb about the movement
	EmberOgre::getSingleton().EventCreatedAvatarEntity.connect(sigc::mem_fun(*this, &Help::EmberOgre_CreatedAvatarEntity));
}


Help::~Help() = default;


void Help::show() {
	Widget::show();
	if (mMainWindow) {
		mMainWindow->moveToFront();
	}
}

void Help::runCommand(const std::string& command, const std::string& args) {
	if (HelpCommand == command) {
		show();
	} else {
		Widget::runCommand(command, args);
	}

}

void Help::EmberOgre_CreatedAvatarEntity(EmberEntity& entity) {
	QuickHelp::getSingleton().updateText(HelpMessage("UI modes", "Click right mouse button to switch between MOVEMENT and INPUT MODE.", "input", "ui_modes"));
}

}
}
}
