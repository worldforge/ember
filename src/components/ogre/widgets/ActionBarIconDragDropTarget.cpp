//
// C++ Implementation: ActionBarIconDragDropTarget
//
// Description:
//
//	Author Tiberiu Paunescu <tpa12@sfu.ca>, (C) 2010
//	Based on the EntityIconDragDropTarget class by Erik Ogenvik <erik@ogenvik.org>, (C) 2007
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
#include "ActionBarIconDragDropTarget.h"
#include "ActionBarIcon.h"
#include "EntityIcon.h"
#include <CEGUI/widgets/DragContainer.h>

using namespace CEGUI;

namespace Ember {
namespace OgreView {

namespace Gui {

ActionBarIconDragDropTarget::ActionBarIconDragDropTarget(CEGUI::Window* container)
{
	container->subscribeEvent(CEGUI::Window::EventDragDropItemEnters, CEGUI::Event::Subscriber(& ActionBarIconDragDropTarget::dragContainer_DragEnter, this));
	container->subscribeEvent(CEGUI::Window::EventDragDropItemLeaves, CEGUI::Event::Subscriber(& ActionBarIconDragDropTarget::dragContainer_DragLeave, this));
	container->subscribeEvent(CEGUI::Window::EventDragDropItemDropped, CEGUI::Event::Subscriber(& ActionBarIconDragDropTarget::dragContainer_DragDropped, this));

}

ActionBarIconDragDropTarget::~ActionBarIconDragDropTarget() = default;


bool ActionBarIconDragDropTarget::dragContainer_DragEnter(const CEGUI::EventArgs& args)
{
	ActionBarIcon* actionBarIcon = parseIcon(args);
	if (actionBarIcon) {
		return handleDragEnter(args, actionBarIcon);
	}
	return true;
}

bool ActionBarIconDragDropTarget::dragContainer_DragLeave(const CEGUI::EventArgs& args)
{
	ActionBarIcon* actionBarIcon = parseIcon(args);
	if (actionBarIcon) {
		return handleDragLeave(args, actionBarIcon);
	}
	return true;
}

bool ActionBarIconDragDropTarget::dragContainer_DragDropped(const CEGUI::EventArgs& args)
{
	const boost::any* anyData = getUserData(args);
	if (anyData)
	{
		if (typeid(GenericIconUserData<ActionBarIcon>) == anyData->type()) {
			const auto& mUserData = boost::any_cast<const GenericIconUserData<ActionBarIcon>&>(*anyData);
			return handleDragActionBarIconDropped(args, &mUserData.mGenericIcon);
		}
		if (typeid(GenericIconUserData<EntityIcon>) == anyData->type()) {
			const auto& mUserData = boost::any_cast<const GenericIconUserData<EntityIcon>&>(*anyData);
			return handleDragEntityIconDropped(args, &mUserData.mGenericIcon);
		}
	}
	return true;
}



bool ActionBarIconDragDropTarget::handleDragEnter(const CEGUI::EventArgs& args, ActionBarIcon* icon)
{
	EventIconEntered.emit(icon);
	return true;
}
bool ActionBarIconDragDropTarget::handleDragLeave(const CEGUI::EventArgs& args, ActionBarIcon* icon)
{
	EventIconLeaves.emit(icon);
	return true;
}
bool ActionBarIconDragDropTarget::handleDragActionBarIconDropped(const CEGUI::EventArgs& args, ActionBarIcon* icon)
{
	EventActionBarIconDropped.emit(icon);
	return true;
}
bool ActionBarIconDragDropTarget::handleDragEntityIconDropped(const CEGUI::EventArgs& args, EntityIcon* icon)
{
	EventEntityIconDropped.emit(icon);
	return true;
}

ActionBarIcon* ActionBarIconDragDropTarget::parseIcon(const CEGUI::EventArgs& args)
{
	const boost::any* anyData = getUserData(args);
	if (anyData)
	{
		if (typeid(GenericIconUserData<ActionBarIcon>) == anyData->type()) {
			const auto& mUserData = boost::any_cast<const GenericIconUserData<ActionBarIcon>&>(*anyData);
			return &mUserData.mGenericIcon;
		}
	}
	return nullptr;
}

const boost::any* ActionBarIconDragDropTarget::getUserData(const CEGUI::EventArgs& args) const
{
	const auto& ddea = dynamic_cast<const DragDropEventArgs&>(args);
	DragContainer* container = ddea.dragDropItem;
	if (container) {
		return static_cast<const boost::any*>(container->getUserData());
	}
	return nullptr;
}


}

}
}
