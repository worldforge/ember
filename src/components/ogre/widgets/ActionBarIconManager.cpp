//
// C++ Implementation: ActionBarIconManager
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
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "ActionBarIconManager.h"
#include <CEGUI.h>
#include "../GUIManager.h"
#include "ActionBarIcon.h"
#include "ActionBarIconSlot.h"
#include "icons/Icon.h"

#include "services/config/ConfigService.h"
#include "services/serversettings/ServerSettings.h"
#include "services/serversettings/ServerSettingsCredentials.h"
#include "services/EmberServices.h"

#include <Eris/ServerInfo.h>

namespace Ember {
namespace OgreView {

namespace Gui {

ActionBarIconManager::ActionBarIconManager(GUIManager& guiManager)
: mGuiManager(guiManager), mIconsCounter(0), mSlotsCounter(0)
{
}

ActionBarIconManager::~ActionBarIconManager()
{
	for (ActionBarIconSlotStore::iterator I = mSlots.begin(); I != mSlots.end(); ++I) {
		delete *I;
	}

	for (ActionBarIconStore::iterator I = mIcons.begin(); I != mIcons.end(); ++I) {
		delete *I;
	}
}



ActionBarIconSlot* ActionBarIconManager::createSlot(unsigned int pixelSize)
{
	std::stringstream ss;
	ss << "actionBarIconSlot" << mSlotsCounter++;
	//Make the slot more visible.
	CEGUI::Window* container = mGuiManager.createWindow("EmberLook/StaticImage", ss.str());
	container->setSize(CEGUI::UVector2(CEGUI::UDim(0, pixelSize), CEGUI::UDim(0, pixelSize)));
	ActionBarIconSlot* slot = new ActionBarIconSlot(container);
	mSlots.push_back(slot);
	return slot;
}


ActionBarIcon* ActionBarIconManager::createIcon(Gui::Icons::Icon* icon, unsigned int pixelSize)
{
	if (!icon) {
		S_LOG_WARNING("Trying to create an EntityIcon with an invalid Icon.");
		return 0;
	}
	std::stringstream ss;
	ss << "actionBarIcon" << mIconsCounter++;

	CEGUI::DragContainer* item = static_cast<CEGUI::DragContainer*>(mGuiManager.createWindow("DragContainer", ss.str()));

	if (item) {
		item->setSize(CEGUI::UVector2(CEGUI::UDim(0, pixelSize), CEGUI::UDim(0, pixelSize)));
		//item->setTooltipText(name);

		ss << "Image" ;
		CEGUI::Window* iconWindow = mGuiManager.createWindow("EmberLook/StaticImage", ss.str());
		if (iconWindow) {
			iconWindow->setProperty("BackgroundEnabled", "false");
 			iconWindow->setProperty("FrameEnabled", "false");
			iconWindow->disable();
// 			iconWindow->setProperty("FrameEnabled", "false");
			iconWindow->setProperty("Image", CEGUI::PropertyHelper::imageToString(icon->getImage()));
			item->addChildWindow(iconWindow);

			ActionBarIcon* actionBarIcon = new ActionBarIcon(*this, item, iconWindow, icon);
			mIcons.push_back(actionBarIcon);
			return actionBarIcon;
		}
	}
	return 0;
}

const std::string ActionBarIconManager::getSavedValue(const AvatarIdType& avatarId, const std::string& key) const
{
	const Eris::ServerInfo& sInfo = avatarId.first;
	std::string accountIdKey = avatarId.second;
	accountIdKey.append(key);

	Ember::Services::ServerSettingsCredentials serverCredentials(sInfo);
	Ember::Services::ServerSettings* serverSettings = Ember::EmberServices::getSingleton().getServerSettingsService();

	if (serverSettings->findItem(serverCredentials, accountIdKey)) {
		return static_cast<std::string>(serverSettings->getItem(serverCredentials, accountIdKey));
	}
	return "null";
}

void ActionBarIconManager::saveValue(const AvatarIdType& avatarId, const std::string& key, const std::string& value)
{
	const Eris::ServerInfo& sInfo = avatarId.first;
	std::string accountIdKey = avatarId.second;
	accountIdKey.append(key);

	Ember::Services::ServerSettingsCredentials serverCredentials(sInfo);
	Ember::Services::ServerSettings* serverSettings = Ember::EmberServices::getSingleton().getServerSettingsService();

	serverSettings->setItem(serverCredentials, accountIdKey, value);
	serverSettings->writeToDisk();
}

void ActionBarIconManager::eraseValue(const AvatarIdType& avatarId, const std::string& key)
{
	const Eris::ServerInfo& sInfo = avatarId.first;
	std::string accountIdKey = avatarId.second;
	accountIdKey.append(key);

	Ember::Services::ServerSettingsCredentials serverCredentials(sInfo);
	Ember::Services::ServerSettings* serverSettings = Ember::EmberServices::getSingleton().getServerSettingsService();

	if (serverSettings->findItem(serverCredentials, accountIdKey)) {
		serverSettings->eraseItem(serverCredentials, accountIdKey);
	}
}

void ActionBarIconManager::destroyIcon(ActionBarIcon* icon)
{
	ActionBarIconStore::iterator I = std::find(mIcons.begin(), mIcons.end(), icon);
	if (I != mIcons.end()) {
		mIcons.erase(I);
		//TODO: make sure to delete the cegui elements
		delete icon;
	}
}

void ActionBarIconManager::destroySlot(ActionBarIconSlot* slot)
{
	ActionBarIconSlotStore::iterator I = std::find(mSlots.begin(), mSlots.end(), slot);
	if (I != mSlots.end()) {
		mSlots.erase(I);
		delete slot;
	}
}

}

}
}
