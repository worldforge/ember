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
#include "components/ogre/EntityWorldPickListener.h"
#include "components/ogre/MousePicker.h"


using namespace Ember::OgreView;
using namespace Ember::Lua;

template <>
void registerLua<EntityWorldPickListener>(sol::table& space) {
	auto entityPickResult = space.new_usertype<EntityPickResult>("EntityPickResult", sol::no_constructor);
	entityPickResult["entity"] = &EntityPickResult::entity;
	entityPickResult["position"] = &EntityPickResult::position;
	entityPickResult["distance"] = &EntityPickResult::distance;

	auto entityWorldPickListener = space.new_usertype<EntityWorldPickListener>("EntityWorldPickListener", sol::no_constructor);
	entityWorldPickListener["EventPickedEntity"] = LuaConnector::make_property(&EntityWorldPickListener::EventPickedEntity);


	space["MPT_CLICK"] = MousePickType::MPT_CLICK;
	space["MPT_DOUBLECLICK"] = MousePickType::MPT_DOUBLECLICK;
	space["MPT_PRESSED"] = MousePickType::MPT_PRESSED;
	space["MPT_HOVER"] = MousePickType::MPT_HOVER;
	space["MPT_PRESS"] = MousePickType::MPT_PRESS;

	auto mousePickerArgs = space.new_usertype<MousePickerArgs>("MousePickerArgs", sol::no_constructor);
	mousePickerArgs["windowX"] = &MousePickerArgs::windowX;
	mousePickerArgs["windowY"] = &MousePickerArgs::windowY;
	mousePickerArgs["pickType"] = &MousePickerArgs::pickType;

}