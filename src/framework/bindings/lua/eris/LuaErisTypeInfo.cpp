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
#include <Eris/TypeInfo.h>


using namespace Eris;
using namespace Ember::Lua;

template<>
void registerLua<TypeInfo>(sol::table& space) {
	auto typeInfo = space.new_usertype<TypeInfo>("TypeInfo", sol::no_constructor);
	typeInfo["isBound"] = &TypeInfo::isBound;
	typeInfo["hasUnresolvedChildren"] = &TypeInfo::hasUnresolvedChildren;
	typeInfo["resolveChildren"] = &TypeInfo::resolveChildren;
	typeInfo["getName"] = &TypeInfo::getName;
	typeInfo["getChildren"] = &TypeInfo::getChildren;
}