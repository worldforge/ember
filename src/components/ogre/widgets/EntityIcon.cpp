//
// C++ Implementation: EntityIcon
//
// Description: 
//
//
// Author: Erik Ogenvik <erik@ogenvik.org>, (C) 2007
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
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "EntityIcon.h"
#include "EntityIconSlot.h"
#include "domain/EmberEntity.h"
#include <CEGUI/widgets/DragContainer.h>
#include <CEGUI/Font.h>

using namespace CEGUI;
namespace Ember {
namespace OgreView {

namespace Gui {

EntityIcon::EntityIcon(EntityIconManager& manager,
					   UniqueWindowPtr<CEGUI::DragContainer> dragContainer,
					   UniqueWindowPtr<CEGUI::Window> image,
					   UniqueWindowPtr<CEGUI::Window> amountWindow,
					   Gui::Icons::Icon* icon,
					   EmberEntity* entity)
		: EntityIconDragDropTarget(dragContainer.get()),
		  mManager(manager),
		  mDragContainer(std::move(dragContainer)),
		  mImage(std::move(image)),
		  mAmount(std::move(amountWindow)),
		  mIcon(icon),
		  mUserData{*this},
		  mUserDataWrapper(mUserData),
		  mCurrentSlot(nullptr),
		  mEntity(entity),
		  mAmountObserver(*mEntity, "amount") {
	mDragContainer->setUserData(&mUserDataWrapper);
	mDragContainer->subscribeEvent(CEGUI::DragContainer::EventDragStarted, CEGUI::Event::Subscriber(&EntityIcon::dragContainer_DragStarted, this));
	mDragContainer->subscribeEvent(CEGUI::DragContainer::EventDragEnded, CEGUI::Event::Subscriber(&EntityIcon::dragContainer_DragStopped, this));
	icon->EventUpdated.connect(sigc::mem_fun(*this, &EntityIcon::icon_Updated));
	updateAmount();
	mAmountObserver.EventChanged.connect([&](const Atlas::Message::Element&) { this->updateAmount(); });
}

EntityIcon::~EntityIcon() {
	if (mCurrentSlot) {
		mCurrentSlot->removeEntityIcon();
	}
}

CEGUI::Window* EntityIcon::getImage() {
	return mImage.get();
}

Gui::Icons::Icon* EntityIcon::getIcon() {
	return mIcon;
}

CEGUI::DragContainer* EntityIcon::getDragContainer() {
	return mDragContainer.get();
}

void EntityIcon::setSlot(EntityIconSlot* slot) {
	if (mCurrentSlot) {
		mCurrentSlot->notifyIconRemoved();
	}
	mCurrentSlot = slot;
}

EntityIconSlot* EntityIcon::getSlot() {
	return mCurrentSlot;
}

void EntityIcon::setTooltipText(const std::string& text) {
	mDragContainer->setTooltipText(text);
}

bool EntityIcon::dragContainer_DragStarted(const CEGUI::EventArgs& args) {
	mManager.EventIconDragStart.emit(this);
	return true;
}

bool EntityIcon::dragContainer_DragStopped(const CEGUI::EventArgs& args) {
	mManager.EventIconDragStop.emit(this);
	return true;
}

EmberEntity* EntityIcon::getEntity() {
	return mEntity;
}

void EntityIcon::icon_Updated() {
	//It seems that we're forced to invalidate the CEGUI Window to get it to update itself. This is perhaps a bug in CEGUI?
	mImage->invalidate();
}

bool EntityIcon::handleDragEnter(const CEGUI::EventArgs& args, EntityIcon* icon) {
	EntityIconDragDropTarget::handleDragEnter(args, icon);
	if (mCurrentSlot) {
		return mCurrentSlot->handleDragEnter(args, icon);
	}
	return true;
}

bool EntityIcon::handleDragLeave(const CEGUI::EventArgs& args, EntityIcon* icon) {
	EntityIconDragDropTarget::handleDragLeave(args, icon);
	if (mCurrentSlot) {
		return mCurrentSlot->handleDragLeave(args, icon);
	}
	return true;
}

bool EntityIcon::handleDragDropped(const CEGUI::EventArgs& args, EntityIcon* icon) {
	EntityIconDragDropTarget::handleDragDropped(args, icon);
	if (mCurrentSlot) {
		return mCurrentSlot->handleDragDropped(args, icon);
	}
	return true;

}

void EntityIcon::updateAmount() {
	auto amountPtr = mEntity->ptrOfProperty("amount");
	if (amountPtr && amountPtr->isInt() && amountPtr->Int() > 1) {
		mAmount->show();
		std::stringstream ss;
		ss << amountPtr->Int();
		auto text = ss.str();

		auto font = mAmount->getFont();
		auto width = font->getTextExtent(text, 1.0) + 4;
		auto height = font->getFontHeight(1.0);

		//Adjust the width and height after the text.
		mAmount->setWidth({0, width});
		mAmount->setHeight({0, height});
		mAmount->setYPosition({1, -(height + 2)});
		mAmount->setXPosition({1, -(width + 2)});
		mAmount->setText(text);
	} else {
		mAmount->hide();
	}
}
}

}
}
