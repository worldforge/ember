//
// C++ Implementation: ActionBarIcon
//
// Description:
//
//	Author Tiberiu Paunescu <tpa12@sfu.ca>, (C) 2010
//	Based on the EntityIcon class by Erik Hjortsberg <erik.hjortsberg@gmail.com>, (C) 2007
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

#include "ActionBarIcon.h"

#include <CEGUI/widgets/DragContainer.h>
#include <CEGUI/WindowManager.h>

using namespace CEGUI;
namespace Ember {
namespace OgreView {

namespace Gui {

ActionBarIcon::ActionBarIcon(ActionBarIconManager& manager, CEGUI::DragContainer* dragContainer, CEGUI::Window* image, Gui::Icons::Icon* icon)
: ActionBarIconDragDropTarget(dragContainer), mManager(manager), mDragContainer(dragContainer), mImage(image), mIcon(icon), mUserData(*this), mUserDataWrapper(mUserData), mCurrentSlot(0)
{
	mDragContainer->setUserData(&mUserDataWrapper);
	mDragContainer->subscribeEvent(CEGUI::DragContainer::EventDragStarted, CEGUI::Event::Subscriber(& ActionBarIcon::dragContainer_DragStarted, this));
	mDragContainer->subscribeEvent(CEGUI::DragContainer::EventDragEnded, CEGUI::Event::Subscriber(& ActionBarIcon::dragContainer_DragStopped, this));
	icon->EventUpdated.connect(sigc::mem_fun(*this, &ActionBarIcon::icon_Updated));

}

ActionBarIcon::~ActionBarIcon()
{
	if (mCurrentSlot) {
		mCurrentSlot->removeActionBarIcon();
	}
	CEGUI::WindowManager::getSingleton().destroyWindow(mImage);
	CEGUI::WindowManager::getSingleton().destroyWindow(mDragContainer);
}

void ActionBarIcon::defaultAction()
{
}

CEGUI::Window* ActionBarIcon::getImage()
{
	return mImage;
}

Gui::Icons::Icon* ActionBarIcon::getIcon()
{
	return mIcon;
}

CEGUI::DragContainer* ActionBarIcon::getDragContainer()
{
	return mDragContainer;
}

void ActionBarIcon::setSlot(ActionBarIconSlot* slot)
{
	if (mCurrentSlot) {
		mCurrentSlot->notifyIconRemoved();
	}
	mCurrentSlot = slot;
}

ActionBarIconSlot* ActionBarIcon::getSlot()
{
	return mCurrentSlot;
}

void ActionBarIcon::setTooltipText(const std::string& text)
{
	mDragContainer->setTooltipText(text);
}

bool ActionBarIcon::dragContainer_DragStarted(const CEGUI::EventArgs& args)
{
	mManager.EventIconDragStart.emit(this);
	return true;
}

bool ActionBarIcon::dragContainer_DragStopped(const CEGUI::EventArgs& args)
{
	mManager.EventIconDragStop.emit(this);
	return true;
}



void ActionBarIcon::icon_Updated()
{
	//It seems that we're forced to invalidate the CEGUI Window to get it to update itself. This is perhaps a bug in CEGUI?
	mImage->invalidate();
}

bool ActionBarIcon::handleDragEnter(const CEGUI::EventArgs& args, ActionBarIcon* icon)
{
	ActionBarIconDragDropTarget::handleDragEnter(args, icon);
	if (mCurrentSlot) {
		return mCurrentSlot->handleDragEnter(args, icon);
	}
	return true;
}
bool ActionBarIcon::handleDragLeave(const CEGUI::EventArgs& args, ActionBarIcon* icon)
{
	ActionBarIconDragDropTarget::handleDragLeave(args, icon);
	if (mCurrentSlot) {
		return mCurrentSlot->handleDragLeave(args, icon);
	}
	return true;
}

bool ActionBarIcon::handleDragActionBarIconDropped(const CEGUI::EventArgs& args, ActionBarIcon* icon)
{
	ActionBarIconDragDropTarget::handleDragActionBarIconDropped(args, icon);
	if (mCurrentSlot) {
		return mCurrentSlot->handleDragActionBarIconDropped(args, icon);
	}
	return true;

}
bool ActionBarIcon::handleDragEntityIconDropped(const CEGUI::EventArgs& args, EntityIcon* icon)
{
	ActionBarIconDragDropTarget::handleDragEntityIconDropped(args, icon);
	if (mCurrentSlot) {
		return mCurrentSlot->handleDragEntityIconDropped(args, icon);
	}
	return true;
}

}
}
}
