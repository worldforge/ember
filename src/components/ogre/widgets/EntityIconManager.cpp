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
: mGuiManager(guiManager), mIconsCounter(0), mSlotsCounter(0)
{
}
	
EntityIconManager::~EntityIconManager()
{
	for (EntityIconSlotStore::iterator I = mSlots.begin(); I != mSlots.end(); ++I) {
		delete *I;
	}
	
	for (EntityIconStore::iterator I = mIcons.begin(); I != mIcons.end(); ++I) {
		delete *I;
	}
}



EntityIconSlot* EntityIconManager::createSlot(unsigned int pixelSize)
{
	std::stringstream ss;
	ss << "entityIconSlot" << mSlotsCounter++;
	//Make the slot more visible.
	CEGUI::Window* container = mGuiManager.createWindow("EmberLook/StaticImage", ss.str());
	container->setSize(CEGUI::USize(CEGUI::UDim(0, pixelSize), CEGUI::UDim(0, pixelSize)));
	EntityIconSlot* slot = new EntityIconSlot(container);
	mSlots.push_back(slot);
	return slot;
}


EntityIcon* EntityIconManager::createIcon(Gui::Icons::Icon* icon, EmberEntity* entity, unsigned int pixelSize)
{
	if (!icon) {
		S_LOG_WARNING("Trying to create an EntityIcon with an invalid Icon.");
		return 0;
	}
	std::stringstream ss;
	ss << "entityIcon" << mIconsCounter++;
	
	CEGUI::DragContainer* item = static_cast<CEGUI::DragContainer*>(mGuiManager.createWindow("DragContainer", ss.str()));
	
	if (item) {
		item->setSize(CEGUI::USize(CEGUI::UDim(0, pixelSize), CEGUI::UDim(0, pixelSize)));
		//item->setTooltipText(name);
		
		ss << "Image" ;
		CEGUI::Window* iconWindow = mGuiManager.createWindow("EmberLook/StaticImage", ss.str());
		if (iconWindow) {
			iconWindow->setProperty("BackgroundEnabled", "false");
 			iconWindow->setProperty("FrameEnabled", "false");
 			iconWindow->setProperty("InheritsAlpha", "true");
			iconWindow->disable();
// 			iconWindow->setProperty("FrameEnabled", "false");
			iconWindow->setProperty("Image", CEGUI::PropertyHelper<CEGUI::Image*>::toString(icon->getImage()));
			item->addChild(iconWindow);
			
			EntityIcon* entityIcon = new EntityIcon(*this, item, iconWindow, icon, entity);
			mIcons.push_back(entityIcon);
			return entityIcon;
		}
	}
	return 0;
}

void EntityIconManager::destroyIcon(EntityIcon* icon)
{
	EntityIconStore::iterator I = std::find(mIcons.begin(), mIcons.end(), icon);
	if (I != mIcons.end()) {
		mIcons.erase(I);
		//TODO: make sure to delete the cegui elements
		delete icon;
	}
}

void EntityIconManager::destroySlot(EntityIconSlot* slot)
{
	EntityIconSlotStore::iterator I = std::find(mSlots.begin(), mSlots.end(), slot);
	if (I != mSlots.end()) {
		mSlots.erase(I);
		delete slot;
	}
}

}

}
}
