//
// C++ Implementation: ActiveWidgetHandler
//
// Description: 
//
//
// Author: Erik Hjortsberg <erik.hjortsberg@iteam.se>, (C) 2008
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
#include <CEGUI.h>

using namespace Ember;
namespace EmberOgre {

namespace Gui {

ActiveWidgetHandler::ActiveWidgetHandler(::EmberOgre::GUIManager& guiManager)
: mLastMode(Ember::Input::IM_GUI), mGuiManager(guiManager)
{

	///connect to the changing of input mode since we want to deactivate the current active input window when switching from gui mode to movement mode
	Ember::Input::getSingleton().EventChangedInputMode.connect(sigc::mem_fun(*this, &ActiveWidgetHandler::Input_InputModeChanged));

}


ActiveWidgetHandler::~ActiveWidgetHandler()
{
}

void ActiveWidgetHandler::Input_InputModeChanged(Ember::Input::InputMode mode)
{
	if (mode != Ember::Input::IM_GUI && mLastMode == Ember::Input::IM_GUI) {
		///save the current active widget
		CEGUI::Window* window = mGuiManager.getMainSheet()->getActiveChild();
		if (window) {
			mLastActiveWidgetName = window->getName().c_str();
			window->deactivate();
			///deactivate all parents
			while ((window = window->getParent())) {
				window->deactivate();
			}
		} else {
			mLastActiveWidgetName = "";
		}
		mLastMode = mode;
	} else if (mode == Ember::Input::IM_GUI) {
		if (mLastActiveWidgetName != "") {
			///restore the previously active widget
			try {
				CEGUI::Window* window = CEGUI::WindowManager::getSingleton().getWindow(mLastActiveWidgetName);
				if (window)
				{
					window->activate();
				}
			} catch (...)
			{
				S_LOG_WARNING("Error when trying to restore previously captured window.");
			}
			mLastActiveWidgetName = "";
		}
		mLastMode = mode;
	}
}


}

}
