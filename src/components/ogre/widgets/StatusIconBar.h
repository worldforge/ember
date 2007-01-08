//
// C++ Interface: StatusIconBar
//
// Description: 
//
//
// Author: Erik Hjortsberg <erik@katastrof.nu>, (C) 2005
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
#ifndef EMBEROGRESTATUSICONBAR_H
#define EMBEROGRESTATUSICONBAR_H

#include "Widget.h"
#include "../GUIManager.h"
#include "../input/Input.h"
#include "../AvatarController.h"

namespace EmberOgre {
class IconBase;
/**
@author Erik Hjortsberg
*/
class StatusIconBar : public Widget {
public:
    StatusIconBar();

    ~StatusIconBar();

	virtual void buildWidget();
protected:

	bool help_MouseClick(const CEGUI::EventArgs& args);
	bool close_MouseClick(const CEGUI::EventArgs& args);
	bool movement_MouseClick(const CEGUI::EventArgs& args);


	IconBase* mMovementModeIcon;
	const CEGUI::Image* mMovementImage_walk;
	const CEGUI::Image* mMovementImage_run;
	const CEGUI::Image* mMovementImage_gui;
	
	void Input_InputModeChanged(Input::InputMode mode);
	void AvatarController_MovementModeChanged(AvatarMovementMode::Mode mode);
	
	Input::InputMode mCurrentMode;
	
	void checkMovementMode();
	
	const CEGUI::Image* mOriginalCursorImage;
	
	std::vector<IconBase*> mIconBases;
};

}

#endif
