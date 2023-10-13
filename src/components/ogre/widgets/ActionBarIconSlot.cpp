//
// C++ Implementation: ActionBarIconSlot
//
// Description:
//
//	Author Tiberiu Paunescu <tpa12@sfu.ca>, (C) 2010
//	Based on the EntityIcon class by Erik Ogenvik <erik@ogenvik.org>, (C) 2007
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
#include "ActionBarIconSlot.h"
#include "Widget.h"
#include "ActionBarIconManager.h"
#include "ActionBarIcon.h"
#include "EntityIcon.h"
#include <CEGUI/CEGUI.h>

using namespace CEGUI;
namespace Ember {
namespace OgreView {

namespace Gui {

ActionBarIconSlot::ActionBarIconSlot(UniqueWindowPtr<CEGUI::Window> container)
		: ActionBarIconDragDropTarget(container.get()),
		  mContainer(std::move(container)),
		  mContainedIcon(nullptr) {
}

ActionBarIconSlot::~ActionBarIconSlot() {
	removeActionBarIcon();
}


bool ActionBarIconSlot::addActionBarIcon(ActionBarIcon* icon) {
	if (icon) {
		if (!mContainedIcon) {
			//Resize the icon window to fit entirely in the slot window.
			icon->getDragContainer()->setSize(this->getWindow()->getSize());
			mContainedIcon = icon;
			mContainer->addChild(icon->getDragContainer());
			icon->setSlot(this);
			//we have to notify the container that things have changed else it won't update itself when we add the entity without dragging (cegui bug?)
			mContainer->notifyScreenAreaChanged();
			mContainer->invalidate();
		} else {
			S_LOG_WARNING("Trying to add entity icon to slot that already has one icon contained.");
			return false;
		}
	}
	return true;
}

ActionBarIcon* ActionBarIconSlot::removeActionBarIcon() {
	if (mContainedIcon) {
		mContainer->removeChild(mContainedIcon->getDragContainer());
		ActionBarIcon* icon = mContainedIcon;
		mContainedIcon = nullptr;
		icon->setSlot(nullptr);
		//we have to notify the container that things have changed else it won't update itself when we remove the entity without dragging (cegui bug?)
		mContainer->notifyScreenAreaChanged();
		mContainer->invalidate();
		return icon;
	} else {
		return nullptr;
	}
}

ActionBarIcon* ActionBarIconSlot::getActionBarIcon() {
	return mContainedIcon;
}


void ActionBarIconSlot::notifyIconRemoved() {
	removeActionBarIcon();
	mContainedIcon = nullptr;
}

CEGUI::Window* ActionBarIconSlot::getWindow() {
	return mContainer.get();
}

void ActionBarIconSlot::notifyIconDraggedOff(ActionBarIcon* actionBarIcon) {
	EventIconDraggedOff.emit(actionBarIcon);
}

}

}
}
