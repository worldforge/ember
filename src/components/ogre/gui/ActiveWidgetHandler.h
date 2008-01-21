//
// C++ Interface: ActiveWidgetHandler
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
#ifndef EMBEROGRE_GUIACTIVEWIDGETHANDLER_H
#define EMBEROGRE_GUIACTIVEWIDGETHANDLER_H
#include "../input/Input.h"

namespace EmberOgre {
class GUIManager;

namespace Gui {

/**
	Responsible for deactivating the current input focused window when we switch to movement mode, and returning it when we switch back to gui mode.
	@author Erik Hjortsberg <erik.hjortsberg@iteam.se>
*/
class ActiveWidgetHandler{
public:
    ActiveWidgetHandler(::EmberOgre::GUIManager& guiManager);

    ~ActiveWidgetHandler();

protected:
	void Input_InputModeChanged(Input::InputMode mode);

	/**
	The name of the window which last had input when we switched to movement mode. This is a string and not a pointer since the window might have been destroyed in between. One other possibility is to keep a pointer ref and listen for the Destroyed event.
	*/
	std::string mLastActiveWidgetName;
	
	/**
	The last mode we were in.
	*/
	Input::InputMode mLastMode;
	
	/**
	A reference to the gui manager.
	*/
	::EmberOgre::GUIManager& mGuiManager;
};

}

}

#endif
