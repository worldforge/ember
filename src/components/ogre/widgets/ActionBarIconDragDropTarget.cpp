//
// C++ Implementation: ActionBarIconDragDropTarget
//
// Description:
//
//	Author Tiberiu Paunescu <tpa12@sfu.ca>, (C) 2010
//	Based on the EntityIconDragDropTarget class by Erik Hjortsberg <erik.hjortsberg@gmail.com>, (C) 2007
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
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "ActionBarIconDragDropTarget.h"
#include "ActionBarIcon.h"
#include "EntityIcon.h"
#include <elements/CEGUIDragContainer.h>
#include <CEGUIWindow.h>
#include <boost/any.hpp>

using namespace CEGUI;

namespace EmberOgre {

namespace Gui {

ActionBarIconDragDropTarget::ActionBarIconDragDropTarget(CEGUI::Window* container)
{
	container->subscribeEvent(CEGUI::Window::EventDragDropItemEnters, CEGUI::Event::Subscriber(& ActionBarIconDragDropTarget::dragContainer_DragEnter, this));
	container->subscribeEvent(CEGUI::Window::EventDragDropItemLeaves, CEGUI::Event::Subscriber(& ActionBarIconDragDropTarget::dragContainer_DragLeave, this));
	container->subscribeEvent(CEGUI::Window::EventDragDropItemDropped, CEGUI::Event::Subscriber(& ActionBarIconDragDropTarget::dragContainer_DragDropped, this));

}

ActionBarIconDragDropTarget::~ActionBarIconDragDropTarget()
{
}


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
		if (typeid(ActionBarIconUserData) == anyData->type()) {
			const ActionBarIconUserData& mUserData = boost::any_cast<const ActionBarIconUserData&>(*anyData);
			if (&mUserData.getActionBarIcon())
				return handleDragActionBarIconDropped(args, &mUserData.getActionBarIcon());
		}
		if (typeid(EntityIconUserData) == anyData->type()) {
			const EntityIconUserData& mUserData = boost::any_cast<const EntityIconUserData&>(*anyData);
			if (&mUserData.getEntityIcon())
				return handleDragEntityIconDropped(args, &mUserData.getEntityIcon());
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
		if (typeid(ActionBarIconUserData) == anyData->type()) {
			const ActionBarIconUserData& mUserData = boost::any_cast<const ActionBarIconUserData&>(*anyData);
			return &mUserData.getActionBarIcon();
		}
	}
	return 0;
}

const boost::any* ActionBarIconDragDropTarget::getUserData(const CEGUI::EventArgs& args) const
{
	const DragDropEventArgs& ddea = static_cast<const DragDropEventArgs&>(args);
	DragContainer* container = ddea.dragDropItem;
	if (container)
		return static_cast<const boost::any*>(container->getUserData());
	return 0;
}


}

}
