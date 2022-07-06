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
#include "components/ogre/widgets/ActionBarIconDragDropTarget.h"
#include "components/ogre/widgets/ActionBarIcon.h"
#include "components/ogre/widgets/ActionBarIconManager.h"
#include "components/ogre/widgets/ActionBarIconSlot.h"
#include "components/ogre/widgets/ActionBarInput.h"
#include "components/ogre/widgets/EntityIcon.h"

using namespace Ember::OgreView::Gui;
using namespace Ember::Lua;

template<>
void registerLua<ActionBarIcon>(sol::table& space) {
	auto actionBarInput = space.new_usertype<ActionBarInput>("ActionBarInput",
															 sol::constructors<ActionBarInput(const std::string&)>());
	actionBarInput["EventGotHotkeyInput"] = LuaConnector::make_property(&ActionBarInput::EventGotHotkeyInput);


	auto actionBarIconDragDropTarget = space.new_usertype<ActionBarIconDragDropTarget>("ActionBarIconDragDropTarget",
																					   sol::constructors<ActionBarIconDragDropTarget(CEGUI::Window*)>()
	);
	actionBarIconDragDropTarget["EventIconEntered"] = LuaConnector::make_property(&ActionBarIconDragDropTarget::EventIconEntered);
	actionBarIconDragDropTarget["EventIconLeaves"] = LuaConnector::make_property(&ActionBarIconDragDropTarget::EventIconLeaves);
	actionBarIconDragDropTarget["EventActionBarIconDropped"] = LuaConnector::make_property(&ActionBarIconDragDropTarget::EventActionBarIconDropped);
	actionBarIconDragDropTarget["EventEntityIconDropped"] = LuaConnector::make_property(&ActionBarIconDragDropTarget::EventEntityIconDropped);

	auto actionBarIcon = space.new_usertype<ActionBarIcon>("ActionBarIcon", sol::no_constructor,
														   sol::base_classes, sol::bases<ActionBarIconDragDropTarget>());
	actionBarIcon["getImage"] = &ActionBarIcon::getImage;
	actionBarIcon["getDragContainer"] = &ActionBarIcon::getDragContainer;
	actionBarIcon["getIcon"] = &ActionBarIcon::getIcon;
	actionBarIcon["setSlot"] = &ActionBarIcon::setSlot;
	actionBarIcon["getSlot"] = &ActionBarIcon::getSlot;
	actionBarIcon["setTooltipText"] = &ActionBarIcon::setTooltipText;
	actionBarIcon["defaultAction"] = &ActionBarIcon::defaultAction;
	actionBarIcon[sol::meta_function::equal_to] = [](ActionBarIcon* lhs, ActionBarIcon* rhs) { return *lhs == *rhs; };

	auto actionBarIconManager = space.new_usertype<ActionBarIconManager>("ActionBarIconManager", sol::no_constructor);
	actionBarIconManager["createSlot"] = &ActionBarIconManager::createSlot;
	actionBarIconManager["destroySlot"] = &ActionBarIconManager::destroySlot;
	actionBarIconManager["createIcon"] = &ActionBarIconManager::createIcon;
	actionBarIconManager["destroyIcon"] = &ActionBarIconManager::destroyIcon;
	actionBarIconManager["getSavedValue"] = [](ActionBarIconManager* self, ActionBarIconManager::AvatarIdType* avatarId, const std::string& name) { return self->getSavedValue(*avatarId, name); };
	actionBarIconManager["saveValue"] = [](ActionBarIconManager* self, ActionBarIconManager::AvatarIdType* avatarId, const std::string& key, const std::string& value) { self->saveValue(*avatarId, key, value); };
	actionBarIconManager["eraseValue"] = [](ActionBarIconManager* self, ActionBarIconManager::AvatarIdType* avatarId, const std::string& key) { self->eraseValue(*avatarId, key); };
	actionBarIconManager["EventIconDragStart"] = LuaConnector::make_property(&ActionBarIconManager::EventIconDragStart);
	actionBarIconManager["EventIconDragStop"] = LuaConnector::make_property(&ActionBarIconManager::EventIconDragStop);

	auto avatarIdType = space.new_usertype<ActionBarIconManager::AvatarIdType>("ActionBarIconManager::AvatarIdType",
																			   sol::meta_method::construct, [](Eris::ServerInfo* serverInfo, const std::string& avatarId) { return ActionBarIconManager::AvatarIdType{*serverInfo, avatarId}; });
	avatarIdType["serverInfo"] = &ActionBarIconManager::AvatarIdType::serverInfo;
	avatarIdType["avatarId"] = &ActionBarIconManager::AvatarIdType::avatarId;

	actionBarIconManager["AvatarIdType"] = avatarIdType;

	auto actionBarIconSlot = space.new_usertype<ActionBarIconSlot>("ActionBarIconSlot", sol::no_constructor,
																   sol::base_classes, sol::bases<ActionBarIconDragDropTarget>());
	actionBarIconSlot["addActionBarIcon"] = &ActionBarIconSlot::addActionBarIcon;
	actionBarIconSlot["removeActionBarIcon"] = &ActionBarIconSlot::removeActionBarIcon;
	actionBarIconSlot["getActionBarIcon"] = &ActionBarIconSlot::getActionBarIcon;
	actionBarIconSlot["getWindow"] = &ActionBarIconSlot::getWindow;
	actionBarIconSlot["notifyIconDraggedOff"] = &ActionBarIconSlot::notifyIconDraggedOff;
	actionBarIconSlot["EventIconDraggedOff"] = LuaConnector::make_property(&ActionBarIconSlot::EventIconDraggedOff);

}