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
#include "LuaFunctor.h"

using namespace CEGUI;
template <>
void registerLua<PropertySet>(sol::table& space) {
	auto propertySet = space.new_usertype<PropertySet>("PropertySet", sol::no_constructor);

	propertySet["setProperty"] = [](Element* self, const char* name, const char* value) { self->setProperty(name, value); };
	//Can't use string_getter_setter because of templated getProperty, even with sol::resolve.
	propertySet["getProperty"] = [](const Element* self, const char* name) { return std::string(self->getProperty(name).c_str()); };
	propertySet["getPropertyDefault"] = [](const Element* self, const char* name) { return std::string(self->getPropertyDefault(name).c_str()); };
	propertySet["getPropertyHelp"] = [](const Element* self, const char* name) { return std::string(self->getPropertyHelp(name).c_str()); };
	propertySet["isPropertyPresent"] = [](const Element* self, const char* name) { return self->isPropertyPresent(name); };
	propertySet["isPropertyDefault"] = [](const Element* self, const char* name) { return self->isPropertyDefault(name); };
}