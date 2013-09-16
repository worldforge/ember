//
// C++ Implementation: ActiveWidgetHandler
//
// Description: 
//
//
// Author: Erik Hjortsberg <erik.hjortsberg@gmail.com>, (C) 2008
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
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "ActiveWidgetHandler.h"

#include "../GUIManager.h"
#include <CEGUI/CEGUI.h>

using namespace Ember;
namespace Ember {
namespace OgreView {

namespace Gui {

ActiveWidgetHandler::ActiveWidgetHandler(GUIManager& guiManager)
: mLastMode(Input::IM_GUI), mGuiManager(guiManager)
{

	//connect to the changing of input mode since we want to deactivate the current active input window when switching from gui mode to movement mode
	Input::getSingleton().EventChangedInputMode.connect(sigc::mem_fun(*this, &ActiveWidgetHandler::Input_InputModeChanged));

}


ActiveWidgetHandler::~ActiveWidgetHandler()
{
}

bool ActiveWidgetHandler::lastActiveWindowDestructionStarted(const CEGUI::EventArgs&) {
	mLastActiveWindow = nullptr;
	mLastActiveWindowDestructionStartedConnection->disconnect();
	return true;
}

void ActiveWidgetHandler::Input_InputModeChanged(Input::InputMode mode)
{
	if (mode != Input::IM_GUI && mLastMode == Input::IM_GUI) {
		//save the current active widget
		CEGUI::Window* window = mGuiManager.getMainSheet()->getActiveChild();
		if (window) {
			mLastActiveWindow = window;
			mLastActiveWindowDestructionStartedConnection = window->subscribeEvent(CEGUI::Window::EventDestructionStarted, CEGUI::Event::Subscriber(&ActiveWidgetHandler::lastActiveWindowDestructionStarted, this));
			window->deactivate();
			//deactivate all parents
			while ((window = window->getParent())) {
				window->deactivate();
			}
		} else {
			mLastActiveWindow = nullptr;
		}
		mLastMode = mode;
	} else if (mode == Input::IM_GUI) {
		if (mLastActiveWindow) {
			//restore the previously active widget
			try {
				mLastActiveWindow->activate();
			} catch (...)
			{
				S_LOG_WARNING("Error when trying to restore previously captured window.");
			}
			mLastActiveWindow = 0;
			mLastActiveWindowDestructionStartedConnection->disconnect();
		}
		mLastMode = mode;
	}
}


}

}
}
