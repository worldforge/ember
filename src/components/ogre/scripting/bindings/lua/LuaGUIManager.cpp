/*
 Copyright (C) 2022 Erik Ogenvik

 This program is free software; you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation; either version 2 of the License, or
 (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program; if not, write to the Free Software
 Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */
#include "RegisterLua.h"
#include "components/ogre/GUIManager.h"
#include "components/ogre/widgets/Widget.h"
#include "components/ogre/widgets/ActionBarIconManager.h"
#include "components/ogre/widgets/EntityIconManager.h"
#include "components/ogre/widgets/EntityTooltip.h"
#include "components/ogre/widgets/icons/IconManager.h"
#include "domain/EmberEntity.h"
#include "domain/EntityTalk.h"


using namespace Ember::OgreView;
using namespace Ember::Lua;

template <>
void registerLua<GUIManager>(sol::table& space) {
	auto guiManager = space.new_usertype<GUIManager>("GUIManager", sol::no_constructor);
	guiManager["getSingleton"] = &GUIManager::getSingleton;
	guiManager["appendAvatarImaginary"] = [](GUIManager* self, const std::string& message) { self->AppendAvatarImaginary.emit(message); };
	guiManager["removeWidget"] = &GUIManager::removeWidget;
	guiManager["addWidget"] = &GUIManager::addWidget;
	guiManager["EmitEntityAction"] = &GUIManager::EmitEntityAction;
	guiManager["getMainSheet"] = &GUIManager::getMainSheet;
	guiManager["isInGUIMode"] = &GUIManager::isInGUIMode;
	guiManager["isInMovementKeysMode"] = &GUIManager::isInMovementKeysMode;
	guiManager["getLayoutDir"] = &GUIManager::getLayoutDir;
	guiManager["createWindow"] = [](GUIManager* self, const std::string& windowType) { return self->createWindow(windowType); };
	guiManager["createWidget"] = &GUIManager::createWidget;
	guiManager["destroyWidget"] = &GUIManager::destroyWidget;
	guiManager["getDefaultScheme"] = &GUIManager::getDefaultScheme;
	guiManager["getIconManager"] = &GUIManager::getIconManager;
	guiManager["getEntityIconManager"] = &GUIManager::getEntityIconManager;
	guiManager["getActionBarIconManager"] = &GUIManager::getActionBarIconManager;
	guiManager["getEntityTooltip"] = &GUIManager::getEntityTooltip;
	guiManager["AppendIGChatLine"] = LuaConnector::make_property(&GUIManager::AppendIGChatLine);
	guiManager["AppendOOGChatLine"] = LuaConnector::make_property(&GUIManager::AppendOOGChatLine);
	guiManager["AppendAvatarImaginary"] = LuaConnector::make_property(&GUIManager::AppendAvatarImaginary);
	guiManager["EventEntityAction"] = LuaConnector::make_property(&GUIManager::EventEntityAction);
	guiManager["EventFrameStarted"] = LuaConnector::make_property(&GUIManager::EventFrameStarted);

}