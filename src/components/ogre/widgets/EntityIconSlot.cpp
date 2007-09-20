//
// C++ Implementation: EntityIconSlot
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
#include "EntityIconSlot.h"
#include "Widget.h"
#include <CEGUI.h>
#include "EntityIconManager.h"
#include "EntityIcon.h"

using namespace CEGUI;
namespace EmberOgre {

namespace Gui {

EntityIconSlot::EntityIconSlot(EntityIconManager& manager, CEGUI::Window* container)
: mManager(manager), mContainer(container), mContainedIcon(0)
{
	BIND_CEGUI_EVENT(container, CEGUI::Window::EventDragDropItemEnters, EntityIconSlot::handleDragEnter)
	BIND_CEGUI_EVENT(container, CEGUI::Window::EventDragDropItemLeaves, EntityIconSlot::handleDragLeave)
	BIND_CEGUI_EVENT(container, CEGUI::Window::EventDragDropItemDropped, EntityIconSlot::handleDragDropped)
}

EntityIconSlot::~EntityIconSlot()
{
}


bool EntityIconSlot::addEntityIcon(EntityIcon* icon)
{
	if (!mContainedIcon) {
		mContainedIcon = icon;
		mContainer->addChildWindow(icon->getDragContainer()); 
		icon->setSlot(this);
	} else {
		S_LOG_WARNING("Trying to add entity icon to slot that already has one icon contained.");
		return false;
	}
	return true;
}

EntityIcon* EntityIconSlot::removeEntityIcon()
{
	if (mContainedIcon) {
		mContainer->removeChildWindow(mContainedIcon->getDragContainer());
		EntityIcon* icon = mContainedIcon;
		mContainedIcon = 0;
		icon->setSlot(0);
		return icon;
	} else {
		return 0;
	}
}

EntityIcon* EntityIconSlot::getEntityIcon()
{
	return mContainedIcon;
}


void EntityIconSlot::notifyIconRemoved()
{
	removeEntityIcon();
	mContainedIcon = 0;
}

CEGUI::Window* EntityIconSlot::getWindow()
{
	return mContainer;
}

bool EntityIconSlot::handleDragEnter(const CEGUI::EventArgs& args)
{
	const DragDropEventArgs& ddea = static_cast<const DragDropEventArgs&>(args);
	DragContainer* container = ddea.dragDropItem;
	if (container) {
		EntityIconUserData* mUserData = static_cast<EntityIconUserData*>(container->getUserData());
		if (mUserData) {
			EventIconEntered.emit(&mUserData->getEntityIcon());
		}
	}
	return true;
}
bool EntityIconSlot::handleDragLeave(const CEGUI::EventArgs& args)
{
	const DragDropEventArgs& ddea = static_cast<const DragDropEventArgs&>(args);
	DragContainer* container = ddea.dragDropItem;
	if (container) {
		EntityIconUserData* mUserData = static_cast<EntityIconUserData*>(container->getUserData());
		if (mUserData) {
			EventIconLeaves.emit(&mUserData->getEntityIcon());
		}
	}
	return true;
}
bool EntityIconSlot::handleDragDropped(const CEGUI::EventArgs& args)
{
	const DragDropEventArgs& ddea = static_cast<const DragDropEventArgs&>(args);
	DragContainer* container = ddea.dragDropItem;
	if (container) {
		EntityIconUserData* mUserData = static_cast<EntityIconUserData*>(container->getUserData());
		if (mUserData) {
			EntityIcon& entityIcon = mUserData->getEntityIcon();
			EventIconDropped.emit(&entityIcon);
// 			addEntityIcon(&entityIcon);
		}
	}
	return true;
}


}

}
