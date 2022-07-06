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
#include "services/input/Input.h"


using namespace Ember;
using namespace Ember::Lua;

template <>
void registerLua<Input>(sol::table& space) {
	auto input = space.new_usertype<Input>("Input", sol::no_constructor);
	input["getSingleton"] = &Input::getSingleton;
	input["IM_GUI"] = sol::var(Input::InputMode::IM_GUI);
	input["IM_MOVEMENT"] = sol::var(Input::InputMode::IM_MOVEMENT);
	input["isKeyDown"] = [](Input* self, int keyCode) { return self->isKeyDown(static_cast<SDL_Scancode>(keyCode)); };
	input["setInputMode"] = &Input::setInputMode;
	input["getInputMode"] = &Input::getInputMode;
	input["toggleInputMode"] = &Input::toggleInputMode;
	input["EventMouseMoved"] = LuaConnector::make_property(&Input::EventMouseMoved);
	input["EventMouseButtonPressed"] = LuaConnector::make_property(&Input::EventMouseButtonPressed);
	input["EventMouseButtonReleased"] = LuaConnector::make_property(&Input::EventMouseButtonReleased);
	input["EventChangedInputMode"] = LuaConnector::make_property(&Input::EventChangedInputMode);


}