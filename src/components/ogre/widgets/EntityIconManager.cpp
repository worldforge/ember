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

EntityIconManager::~EntityIconManager() = default;


EntityIconSlot* EntityIconManager::createSlot(unsigned int pixelSize) {
	std::stringstream ss;
	ss << "entityIconSlot" << mSlotsCounter++;
	//Make the slot more visible.
	UniqueWindowPtr<CEGUI::Window> container(mGuiManager.createWindow("EmberLook/StaticImage", ss.str()));
	container->setSize(CEGUI::USize(CEGUI::UDim(0, pixelSize), CEGUI::UDim(0, pixelSize)));
	auto slot = std::make_unique<EntityIconSlot>(std::move(container));
	mSlots.emplace_back(std::move(slot));
	return mSlots.back().get();
}

std::unique_ptr<EntityIcon> EntityIconManager::createIconInstance(Gui::Icons::Icon* icon, EmberEntity* entity, unsigned int pixelSize) {
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
		item->addChild(iconWindow.release());

		amountWindow->setPosition(CEGUI::UVector2(CEGUI::UDim(0, 0), CEGUI::UDim(1, -20)));
		amountWindow->setWidth(CEGUI::UDim(1, 0));
		amountWindow->setHeight(CEGUI::UDim(0, 20));
		amountWindow->setProperty("TextColours", "FFFFFFFF");
		amountWindow->setProperty("BackgroundColours", "FFFFFF");
		amountWindow->setProperty("BackgroundEnabled", "true");
		amountWindow->setProperty("HorzFormatting", "CentreAligned");
		amountWindow->setMousePassThroughEnabled(true);

		item->addChild(amountWindow.release());

		return std::make_unique<EntityIcon>(*this, std::move(item), *item->getChildAtIdx(0), *item->getChildAtIdx(1), icon, entity);
	}
	return nullptr;
}


EntityIcon* EntityIconManager::createIcon(Gui::Icons::Icon* icon, EmberEntity* entity, unsigned int pixelSize) {
	auto entityIcon = createIconInstance(icon, entity, pixelSize);
	if (entityIcon) {
		mIcons.push_back(std::move(entityIcon));
		return mIcons.back().get();
	}
	return nullptr;
}

void EntityIconManager::destroyIcon(EntityIcon* icon) {
	auto I = std::find_if(mIcons.begin(), mIcons.end(), [icon](const std::unique_ptr<EntityIcon>& entry) { return icon == entry.get(); });
	if (I != mIcons.end()) {
		mIcons.erase(I);
	}
}

void EntityIconManager::destroySlot(EntityIconSlot* slot) {
	auto I = std::find_if(mSlots.begin(), mSlots.end(), [slot](const std::unique_ptr<EntityIconSlot>& entry) { return slot == entry.get(); });
	if (I != mSlots.end()) {
		mSlots.erase(I);
	}
}


}

}
}
