//
// C++ Implementation: Quit
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
#include "Quit.h"

#include "../GUIManager.h"

#include "framework/MainLoopController.h"
#include "services/EmberServices.h"
#include "services/server/ServerService.h"

#include <CEGUI/widgets/PushButton.h>
#include <iostream>





namespace Ember {
namespace OgreView {
namespace Gui {

WidgetPluginCallback Quit::registerWidget(GUIManager& guiManager) {

	auto widget = std::make_shared<Quit>(guiManager);


	return [widget]() mutable {
		//Just hold on to an instance.
		widget.reset();
	};

}

Quit::Quit(GUIManager& guiManager) :
		Widget(guiManager),
		SoftQuit("softquit", this, "Display a quit confirmation window.") {
	loadMainSheet("Quit.layout", "Quit/");

	MainLoopController::getSingleton().EventRequestQuit.connect(sigc::mem_fun(*this, &Quit::EmberOgre_RequestQuit));

	CEGUI::PushButton* shutdownButton = static_cast<CEGUI::PushButton*>(getWindow("ShutdownButton"));
	CEGUI::PushButton* logoutButton = static_cast<CEGUI::PushButton*>(getWindow("LogoutButton"));
	CEGUI::PushButton* cancelButton = static_cast<CEGUI::PushButton*>(getWindow("CancelButton"));

	BIND_CEGUI_EVENT(cancelButton, CEGUI::PushButton::EventClicked, Quit::Cancel_Click);
	BIND_CEGUI_EVENT(shutdownButton, CEGUI::PushButton::EventClicked, Quit::Shutdown_Click);
	BIND_CEGUI_EVENT(logoutButton, CEGUI::PushButton::EventClicked, Quit::Logout_Click);

	registerConsoleVisibilityToggleCommand("quit");
	enableCloseButton();

	mMainWindow->setVisible(false);
}


Quit::~Quit() = default;


bool Quit::Shutdown_Click(const CEGUI::EventArgs& args) {
	MainLoopController::getSingleton().quit();
	mMainWindow->setVisible(false);
	return true;
}

bool Quit::Logout_Click(const CEGUI::EventArgs& args) {
	EmberServices::getSingleton().getServerService().logout();
	mMainWindow->setVisible(false);
	return true;
}

bool Quit::Cancel_Click(const CEGUI::EventArgs& args) {
	mMainWindow->setVisible(false);
	return true;
}


void Quit::EmberOgre_RequestQuit(bool& handled) {
	handled = true;
	//if the window system twice requests a quit, do it
	if (mMainWindow->isVisible()) {
		MainLoopController::getSingleton().quit();
	} else {
		softquit();
	}
}

void Quit::softquit() {
	mMainWindow->activate();
	mMainWindow->moveToFront();
	mMainWindow->setVisible(true);

	Input::getSingleton().setInputMode(Input::IM_GUI);
	//mMainWindow->setModalState(true);
}

void Quit::hide() {
	//mMainWindow->setModalState(false);
}


void Quit::runCommand(const std::string& command, const std::string& args) {
	if (SoftQuit == command) {
		softquit();
	} else {
		Widget::runCommand(command, args);
	}

}
}
}
}
