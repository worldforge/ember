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
#include <OgreLodConfig.h>

using namespace Ogre;

template<>
void registerLua<LodStrategy>(sol::table& space) {
	auto lodLevel = sol::state_view(space.lua_state()).create_table();
	lodLevel["VRM_PROPORTIONAL"] = LodLevel::VRM_PROPORTIONAL;
	lodLevel["VRM_CONSTANT"] = LodLevel::VRM_CONSTANT;
	lodLevel["VRM_COLLAPSE_COST"] = LodLevel::VRM_COLLAPSE_COST;

	space["LodLevel"] = lodLevel;

	auto lodStrategy = space.new_usertype<LodStrategy>("LodStrategy", sol::no_constructor);
	lodStrategy["transformUserValue"] = &LodStrategy::transformUserValue;


}