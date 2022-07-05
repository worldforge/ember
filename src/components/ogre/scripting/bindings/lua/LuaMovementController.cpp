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
#include "components/ogre/MovementController.h"


using namespace Ember::OgreView;
using namespace Ember::Lua;

template<>
void registerLua<MovementController>(sol::table& space) {
	auto movementControllerMode = space.new_usertype<MovementControllerMode>("MovementControllerMode", sol::no_constructor);
	movementControllerMode["MM_WALK"] = sol::var(MovementControllerMode::MM_WALK);
	movementControllerMode["MM_RUN"] = sol::var(MovementControllerMode::MM_RUN);

	auto movementController = space.new_usertype<MovementController>("MovementController", sol::no_constructor);
	movementController["moveToPoint"] = &MovementController::moveToPoint;
	movementController["teleportTo"] = &MovementController::teleportTo;
	movementController["getMode"] = &MovementController::getMode;
	movementController["EventMovementModeChanged"] = LuaConnector::make_property(&MovementController::EventMovementModeChanged);


}