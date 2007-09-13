//
// C++ Interface: EntityIconSlot
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
#ifndef EMBEROGRE_GUIENTITYICONSLOT_H
#define EMBEROGRE_GUIENTITYICONSLOT_H

#include <sigc++/signal.h>

namespace CEGUI {
class DragContainer;
class Window;
class EventArgs;
}

namespace EmberOgre {

namespace Gui {

class EntityIcon;
class EntityIconManager;

/**
	@author Erik Hjortsberg <erik.hjortsberg@iteam.se>
*/
class EntityIconSlot
{
friend class EntityIconManager;
public:
	
	bool addEntityIcon(EntityIcon* icon);
	EntityIcon* removeEntityIcon();
	
	CEGUI::Window* getWindow();
	
	void notifyIconRemoved();
	
	sigc::signal<void, EntityIcon*> EventIconEntered;
	sigc::signal<void, EntityIcon*> EventIconLeaves;
	sigc::signal<void, EntityIcon*> EventIconDropped;
protected:
	EntityIconSlot(EntityIconManager& manager, CEGUI::Window* container);
	virtual ~EntityIconSlot();
	
	EntityIconManager& mManager;
	CEGUI::Window* mContainer;
	EntityIcon* mContainedIcon;
	
	bool handleDragEnter(const CEGUI::EventArgs& args);
	bool handleDragLeave(const CEGUI::EventArgs& args);
	bool handleDragDropped(const CEGUI::EventArgs& args);
	

};

}

}

#endif
