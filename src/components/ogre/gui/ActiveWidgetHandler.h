//
// C++ Interface: ActiveWidgetHandler
//
// Description: 
//
//
// Author: Erik Ogenvik <erik@ogenvik.org>, (C) 2008
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
#ifndef EMBEROGRE_GUIACTIVEWIDGETHANDLER_H
#define EMBEROGRE_GUIACTIVEWIDGETHANDLER_H
#include "services/input/Input.h"

#include <CEGUI/Event.h>

namespace Ember {
namespace OgreView {
class GUIManager;

namespace Gui {

/**
	Responsible for deactivating the current input focused window when we switch to movement mode, and returning it when we switch back to gui mode.
	@author Erik Ogenvik <erik@ogenvik.org>
*/
class ActiveWidgetHandler{
public:
    ActiveWidgetHandler(GUIManager& guiManager);

    ~ActiveWidgetHandler();

protected:
	void Input_InputModeChanged(Input::InputMode mode);

	bool lastActiveWindowDestructionStarted(const CEGUI::EventArgs&);

	/**
	The window which last had input when we switched to movement mode.
	*/
	CEGUI::Window* mLastActiveWindow;
	
	CEGUI::Event::Connection mLastActiveWindowDestructionStartedConnection;

	/**
	The last mode we were in.
	*/
	Input::InputMode mLastMode;
	
	/**
	A reference to the gui manager.
	*/
	GUIManager& mGuiManager;
};

}

}

}

#endif
