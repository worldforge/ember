//
// C++ Interface: EntityIcon
//
// Description: 
//
//
// Author: Erik Hjortsberg <erik.hjortsberg@iteam.se>, (C) 2007
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
#ifndef EMBEROGRE_GUIENTITYICON_H
#define EMBEROGRE_GUIENTITYICON_H


#include "icons/Icon.h"
#include "EntityIconManager.h"

namespace CEGUI {
class DragContainer;
class Window;
class EventArgs;
}

namespace EmberOgre {

namespace Gui {

class EntityIconSlot;
class EntityIcon;

/**
	@author Erik Hjortsberg <erik.hjortsberg@iteam.se>
*/
class EntityIconUserData
{
public:
	EntityIconUserData(EntityIcon& entityIcon);
	
	EntityIcon& getEntityIcon();
private:
	EntityIcon& mEntityIcon;
};

class EntityIcon
{
friend class EntityIconManager;
public:
	
	CEGUI::Window* getImage();
	CEGUI::DragContainer* getDragContainer();
	Gui::Icons::Icon* getIcon();
	void setSlot(EntityIconSlot* slot);
	EntityIconSlot* getSlot();
	void setTooltipText(const std::string& text);
	

protected:
	EntityIcon(EntityIconManager& manager, CEGUI::DragContainer* dragContainer, CEGUI::Window* image, Gui::Icons::Icon* icon);
	
	EntityIconManager& mManager;
	CEGUI::DragContainer* mDragContainer;
	CEGUI::Window* mImage;
	Gui::Icons::Icon* mIcon;
	EntityIconUserData mUserData;
	EntityIconSlot* mCurrentSlot;
	
	bool dragContainer_DragStarted(const CEGUI::EventArgs& args);
	bool dragContainer_DragStopped(const CEGUI::EventArgs& args);
};

}

}

#endif
