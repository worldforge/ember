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
#include <Eris/View.h>
#include <Eris/Avatar.h>
#include "domain/EmberEntity.h"

using namespace Eris;
using namespace Ember::Lua;

template <>
void registerLua<View>(sol::table& space) {
	auto view = space.new_usertype<View>("View", sol::no_constructor);
	view["getAvatar"] = &View::getAvatar;
	view["lookQueueSize"] = &View::lookQueueSize;
	view["getTopLevel"] = &View::getTopLevel;
	view["getEntity"] = [](View* self, const std::string& eid) { return static_cast<Ember::EmberEntity*>(self->getEntity(eid)); };
	view["TopLevelEntityChanged"] = LuaConnector::make_property(&View::TopLevelEntityChanged);


}