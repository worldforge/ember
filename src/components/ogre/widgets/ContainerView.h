/*
 Copyright (C) 2020 Erik Ogenvik

 This program is free software; you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation; either version 2 of the License, or
 (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program; if not, write to the Free Software
 Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#ifndef EMBER_CONTAINERVIEW_H
#define EMBER_CONTAINERVIEW_H

#include <sigc++/connection.h>
#include "Widget.h"
#include "EntityIconSlot.h"

namespace Ember {
class EmberEntity;
namespace OgreView {
class GUIManager;
namespace Gui {
class EntityIconManager;
namespace Icons {
class IconManager;
}
class ContainerView {

public:
	ContainerView(EntityIconManager& entityIconManager,
				  Icons::IconManager& iconManager,
				  CEGUI::Tooltip& tooltip,
				  CEGUI::Window& iconContainer,
				  int slotSize = 32);

	~ContainerView();


	void showEntityContents(EmberEntity* entity);

	void addEntityIcon(EntityIcon* entityIcon);

	EntityIcon* getEntityIcon(const std::string& entityId);

	sigc::signal<void, EmberEntity*> EventEntityPicked;
	sigc::signal<void, EntityIcon*> EventIconAdded;

protected:
	EntityIconManager& mEntityIconManager;
	Icons::IconManager& mIconManager;
	CEGUI::Tooltip& mTooltip;
	CEGUI::Window& mIconContainer;
	int mSlotSize;
	std::vector<std::unique_ptr<EntityIconSlot>> mSlots;
	std::vector<std::unique_ptr<EntityIcon>> mIcons;
	sigc::connection mActionConnection;
	sigc::connection mChildAddedConnection;
	sigc::connection mChildRemovedConnection;
	sigc::connection mBeingDeletedConnection;
	EmberEntity* mObservedEntity;

	EntityIconSlot* addSlot();

	EntityIconSlot* getFreeSlot();

	EntityIcon* createEntityIcon(EmberEntity* entity);

	void clearShownContent();

	void layoutSlots();

};
}
}
}

#endif //EMBER_CONTAINERVIEW_H
