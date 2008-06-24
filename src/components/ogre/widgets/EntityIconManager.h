//
// C++ Interface: EntityIconManager
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
#ifndef EMBEROGRE_GUIENTITYICONMANAGER_H
#define EMBEROGRE_GUIENTITYICONMANAGER_H

#include <sigc++/signal.h>
#include <vector>

namespace EmberOgre {
class EmberEntity;
class GUIManager;
namespace Gui {

namespace Icons
{
class Icon;
}

class EntityIconSlot;
class EntityIcon;

/**
	@author Erik Hjortsberg <erik.hjortsberg@iteam.se>
*/
class EntityIconManager
{
public:
typedef std::vector<EntityIconSlot*> EntityIconSlotStore;
typedef std::vector<EntityIcon*> EntityIconStore;

	EntityIconManager(GUIManager& guiManager);
	virtual ~EntityIconManager();
	
	EntityIconSlot* createSlot(unsigned int pixelSize = 64);
	void destroySlot(EntityIconSlot* slot);
	
	EntityIcon* createIcon(Gui::Icons::Icon* icon, EmberEntity* entity, unsigned int pixelSize = 64);
	void destroyIcon(EntityIcon* icon);
	
	sigc::signal<void, EntityIcon*> EventIconDragStart;
	sigc::signal<void, EntityIcon*> EventIconDragStop;

protected:
	EntityIconSlotStore mSlots;
	EntityIconStore mIcons;
	GUIManager& mGuiManager;
	int mIconsCounter, mSlotsCounter;
};

}

}

#endif
