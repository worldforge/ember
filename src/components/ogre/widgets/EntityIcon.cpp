//
// C++ Implementation: EntityIcon
//
// Description: 
//
//
// Author: Erik Hjortsberg <erik.hjortsberg@gmail.com>, (C) 2007
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

#include "EntityIcon.h"
#include "EntityIconSlot.h"
#include <elements/CEGUIDragContainer.h>
#include <CEGUIWindowManager.h>

using namespace CEGUI;
namespace EmberOgre {

namespace Gui {

EntityIcon::EntityIcon(EntityIconManager& manager, CEGUI::DragContainer* dragContainer, CEGUI::Window* image, Gui::Icons::Icon* icon, EmberEntity* entity)
: EntityIconDragDropTarget(dragContainer), mManager(manager), mDragContainer(dragContainer), mImage(image), mIcon(icon), mUserData(*this), mCurrentSlot(0), mEntity(entity)
{
	mDragContainer->setUserData(&mUserData);
	mDragContainer->subscribeEvent(CEGUI::DragContainer::EventDragStarted, CEGUI::Event::Subscriber(& EntityIcon::dragContainer_DragStarted, this)); 
	mDragContainer->subscribeEvent(CEGUI::DragContainer::EventDragEnded, CEGUI::Event::Subscriber(& EntityIcon::dragContainer_DragStopped, this)); 
	icon->EventUpdated.connect(sigc::mem_fun(*this, &EntityIcon::icon_Updated));

}

EntityIcon::~EntityIcon()
{
	if (mCurrentSlot) {
		mCurrentSlot->removeEntityIcon();
	}
	CEGUI::WindowManager::getSingleton().destroyWindow(mImage);
	CEGUI::WindowManager::getSingleton().destroyWindow(mDragContainer);
}

CEGUI::Window* EntityIcon::getImage()
{
	return mImage;
}

Gui::Icons::Icon* EntityIcon::getIcon()
{
	return mIcon;
}

CEGUI::DragContainer* EntityIcon::getDragContainer()
{
	return mDragContainer;
}

void EntityIcon::setSlot(EntityIconSlot* slot)
{
	if (mCurrentSlot) {
		mCurrentSlot->notifyIconRemoved();
	}
	mCurrentSlot = slot;
}

EntityIconSlot* EntityIcon::getSlot()
{
	return mCurrentSlot;
}

void EntityIcon::setTooltipText(const std::string& text)
{
	mDragContainer->setTooltipText(text);
}

bool EntityIcon::dragContainer_DragStarted(const CEGUI::EventArgs& args)
{
	mManager.EventIconDragStart.emit(this);
	return true;
}

bool EntityIcon::dragContainer_DragStopped(const CEGUI::EventArgs& args)
{
	mManager.EventIconDragStop.emit(this);
	return true;
}

EmberEntity* EntityIcon::getEntity()
{
	return mEntity;
}

void EntityIcon::icon_Updated()
{
	//It seems that we're forced to invalidate the CEGUI Window to get it to update itself. This is perhaps a bug in CEGUI?
	mImage->invalidate();
}

bool EntityIcon::handleDragEnter(const CEGUI::EventArgs& args, EntityIcon* icon)
{
	EntityIconDragDropTarget::handleDragEnter(args, icon);
	if (mCurrentSlot) {
		return mCurrentSlot->handleDragEnter(args, icon);
	}
	return true;
}
bool EntityIcon::handleDragLeave(const CEGUI::EventArgs& args, EntityIcon* icon)
{
	EntityIconDragDropTarget::handleDragLeave(args, icon);
	if (mCurrentSlot) {
		return mCurrentSlot->handleDragLeave(args, icon);
	}
	return true;
}
bool EntityIcon::handleDragDropped(const CEGUI::EventArgs& args, EntityIcon* icon)
{
	EntityIconDragDropTarget::handleDragDropped(args, icon);
	if (mCurrentSlot) {
		return mCurrentSlot->handleDragDropped(args, icon);
	}
	return true;

}


EntityIconUserData::EntityIconUserData(EntityIcon& entityIcon)
: mEntityIcon(entityIcon)
{
}

EntityIcon& EntityIconUserData::getEntityIcon()
{
	return mEntityIcon;
}



}

}
