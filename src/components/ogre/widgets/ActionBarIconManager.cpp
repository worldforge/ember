//
// C++ Implementation: ActionBarIconManager
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
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "ActionBarIconManager.h"
#include "../GUIManager.h"
#include "ActionBarIcon.h"

#include "services/config/ConfigService.h"
#include "services/serversettings/ServerSettings.h"
#include "services/serversettings/ServerSettingsCredentials.h"

#include <Eris/ServerInfo.h>

#include <CEGUI/CEGUI.h>

namespace Ember {
namespace OgreView {

namespace Gui {

ActionBarIconManager::ActionBarIconManager(GUIManager& guiManager)
		: mGuiManager(guiManager),
		  mIconsCounter(0),
		  mSlotsCounter(0) {
}

ActionBarIconManager::~ActionBarIconManager() = default;


ActionBarIconSlot* ActionBarIconManager::createSlot(unsigned int pixelSize) {
	std::stringstream ss;
	ss << "actionBarIconSlot" << mSlotsCounter++;
	//Make the slot more visible.
	UniqueWindowPtr<CEGUI::Window> container(mGuiManager.createWindow("EmberLook/StaticImage", ss.str()));
	container->setSize(CEGUI::USize(CEGUI::UDim(0, pixelSize), CEGUI::UDim(0, pixelSize)));
	auto* slot = new ActionBarIconSlot(std::move(container));
	mSlots.emplace_back(slot);
	return slot;
}


ActionBarIcon* ActionBarIconManager::createIcon(Gui::Icons::Icon* icon, unsigned int pixelSize) {
	if (!icon) {
		S_LOG_WARNING("Trying to create an EntityIcon with an invalid Icon.");
		return nullptr;
	}
	std::stringstream ss;
	ss << "actionBarIcon" << mIconsCounter++;

	UniqueWindowPtr<CEGUI::DragContainer> item(dynamic_cast<CEGUI::DragContainer*>(mGuiManager.createWindow("DragContainer", ss.str())));
	ss << "Image";
	UniqueWindowPtr<CEGUI::Window> iconWindow(mGuiManager.createWindow("EmberLook/StaticImage", ss.str()));

	if (item && iconWindow) {
		item->setSize(CEGUI::USize(CEGUI::UDim(0, pixelSize), CEGUI::UDim(0, pixelSize)));
		//item->setTooltipText(name);

		iconWindow->setProperty("BackgroundEnabled", "false");
		iconWindow->setProperty("FrameEnabled", "false");
		iconWindow->disable();
// 			iconWindow->setProperty("FrameEnabled", "false");
		iconWindow->setProperty("Image", CEGUI::PropertyHelper<CEGUI::Image*>::toString(icon->getImage()));
		item->addChild(iconWindow.get());

		auto* actionBarIcon = new ActionBarIcon(*this, std::move(item), std::move(iconWindow), icon);
		mIcons.emplace_back(actionBarIcon);
		return actionBarIcon;
	}
	return nullptr;
}

std::string ActionBarIconManager::getSavedValue(const AvatarIdType& avatarId, const std::string& key) const {
	const Eris::ServerInfo& sInfo = avatarId.first;
	std::string accountIdKey = avatarId.second;
	accountIdKey.append(key);

	Services::ServerSettingsCredentials serverCredentials(sInfo);
	Services::ServerSettings& serverSettings = EmberServices::getSingleton().getServerSettingsService();

	if (serverSettings.findItem(serverCredentials, accountIdKey)) {
		return static_cast<std::string>(serverSettings.getItem(serverCredentials, accountIdKey));
	}
	return "null";
}

void ActionBarIconManager::saveValue(const AvatarIdType& avatarId, const std::string& key, const std::string& value) {
	const Eris::ServerInfo& sInfo = avatarId.first;
	std::string accountIdKey = avatarId.second;
	accountIdKey.append(key);

	Services::ServerSettingsCredentials serverCredentials(sInfo);
	Services::ServerSettings& serverSettings = EmberServices::getSingleton().getServerSettingsService();

	serverSettings.setItem(serverCredentials, accountIdKey, value);
	serverSettings.writeToDisk();
}

void ActionBarIconManager::eraseValue(const AvatarIdType& avatarId, const std::string& key) {
	const Eris::ServerInfo& sInfo = avatarId.first;
	std::string accountIdKey = avatarId.second;
	accountIdKey.append(key);

	Services::ServerSettingsCredentials serverCredentials(sInfo);
	Services::ServerSettings& serverSettings = EmberServices::getSingleton().getServerSettingsService();

	if (serverSettings.findItem(serverCredentials, accountIdKey)) {
		serverSettings.eraseItem(serverCredentials, accountIdKey);
	}
}

void ActionBarIconManager::destroyIcon(ActionBarIcon* icon) {
	auto I = std::find_if(mIcons.begin(), mIcons.end(), [&icon](const std::unique_ptr<ActionBarIcon>& ptr) {
		return ptr.get() == icon;
	});
	if (I != mIcons.end()) {
		mIcons.erase(I);
	}
}

void ActionBarIconManager::destroySlot(ActionBarIconSlot* slot) {
	auto I = std::find_if(mSlots.begin(), mSlots.end(), [&slot](const std::unique_ptr<ActionBarIconSlot>& ptr) {
		return ptr.get() == slot;
	});
	if (I != mSlots.end()) {
		mSlots.erase(I);
	}
}

}

}
}
