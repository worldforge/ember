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
#include <Eris/Task.h>


using namespace Eris;
using namespace Ember::Lua;

template <>
void registerLua<Task>(sol::table& space) {
	auto task = space.new_usertype<Task>("Task", sol::no_constructor);
	task["name"] = &Task::name;
	task["progress"] = &Task::progress;
	task["progressRate"] = &Task::progressRate;
	task["isComplete"] = &Task::isComplete;
	task["Completed"] = LuaConnector::make_property(&Task::Completed);
	task["Progressed"] = LuaConnector::make_property(&Task::Progressed);
	task["UsagesChanged"] = LuaConnector::make_property(&Task::UsagesChanged);
	task["usagesSize"] = [](Task* self) { return static_cast<int>(self->getUsages().size()); };
	task["usageName"] = [](Task* self, int index) {
		if (index < static_cast<int>(self->getUsages().size())) {
			return self->getUsages()[index].name;
		}
		return std::string("");
	};


}