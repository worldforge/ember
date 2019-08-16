//
// C++ Implementation: EntityIconManager
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

#include "EntityIconManager.h"
#include "../GUIManager.h"
#include "EntityIcon.h"
#include "EntityIconSlot.h"

#include <CEGUI/CEGUI.h>

namespace Ember {
namespace OgreView {

namespace Gui {

EntityIconManager::EntityIconManager(GUIManager& guiManager)
		: mGuiManager(guiManager),
		  mIconsCounter(0),
		  mSlotsCounter(0) {
}

EntityIconManager::~EntityIconManager() {
	for (auto& slot : mSlots) {
		delete slot;
	}

	for (auto& icon : mIcons) {
		delete icon;
	}
}


EntityIconSlot* EntityIconManager::createSlot(unsigned int pixelSize) {
	std::stringstream ss;
	ss << "entityIconSlot" << mSlotsCounter++;
	//Make the slot more visible.
	UniqueWindowPtr<CEGUI::Window> container(mGuiManager.createWindow("EmberLook/StaticImage", ss.str()));
	container->setSize(CEGUI::USize(CEGUI::UDim(0, pixelSize), CEGUI::UDim(0, pixelSize)));
	auto slot = new EntityIconSlot(std::move(container));
	mSlots.push_back(slot);
	return slot;
}


EntityIcon* EntityIconManager::createIcon(Gui::Icons::Icon* icon, EmberEntity* entity, unsigned int pixelSize) {
	if (!icon) {
		S_LOG_WARNING("Trying to create an EntityIcon with an invalid Icon.");
		return nullptr;
	}
	std::stringstream ss;
	ss << "entityIcon" << mIconsCounter++;

	UniqueWindowPtr<CEGUI::DragContainer> item(dynamic_cast<CEGUI::DragContainer*>(mGuiManager.createWindow("DragContainer", ss.str())));
	ss << "Image";
	UniqueWindowPtr<CEGUI::Window> iconWindow(mGuiManager.createWindow("EmberLook/StaticImage", ss.str()));
	ss << "Amount";
	UniqueWindowPtr<CEGUI::Window> amountWindow(mGuiManager.createWindow("EmberLook/StaticText", ss.str()));

	if (item && iconWindow && amountWindow) {
		item->setSize(CEGUI::USize(CEGUI::UDim(0, pixelSize), CEGUI::UDim(0, pixelSize)));
		//item->setTooltipText(name);

		iconWindow->setProperty("BackgroundEnabled", "false");
		iconWindow->setProperty("FrameEnabled", "false");
		iconWindow->setProperty("InheritsAlpha", "true");
		iconWindow->disable();
// 			iconWindow->setProperty("FrameEnabled", "false");
		iconWindow->setProperty("Image", CEGUI::PropertyHelper<CEGUI::Image*>::toString(icon->getImage()));
		item->addChild(iconWindow.get());

		amountWindow->setPosition(CEGUI::UVector2(CEGUI::UDim(0, 0), CEGUI::UDim(1, -20)));
		amountWindow->setWidth(CEGUI::UDim(1, 0));
		amountWindow->setHeight(CEGUI::UDim(0, 20));
		amountWindow->setProperty("TextColours", "FFFFFFFF");
		amountWindow->setProperty("BackgroundColours", "FFFFFF");
		amountWindow->setProperty("BackgroundEnabled", "true");
		amountWindow->setProperty("HorzFormatting", "CentreAligned");
		amountWindow->setMousePassThroughEnabled(true);

		item->addChild(amountWindow.get());

		auto* entityIcon = new EntityIcon(*this, std::move(item), std::move(iconWindow), std::move(amountWindow), icon, entity);
		mIcons.push_back(entityIcon);
		return entityIcon;
	}
	return nullptr;
}

void EntityIconManager::destroyIcon(EntityIcon* icon) {
	auto I = std::find(mIcons.begin(), mIcons.end(), icon);
	if (I != mIcons.end()) {
		mIcons.erase(I);
		//TODO: make sure to delete the cegui elements
		delete icon;
	}
}

void EntityIconManager::destroySlot(EntityIconSlot* slot) {
	auto I = std::find(mSlots.begin(), mSlots.end(), slot);
	if (I != mSlots.end()) {
		mSlots.erase(I);
		delete slot;
	}
}

}

}
}
