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
#include "components/ogre/OgreInfo.h"
#include "helpers/OgreUtils.h"
#include "components/ogre/Convert.h"


using namespace Ember::OgreView;
using namespace Ember::Lua;

template <>
void registerLua<OgreInfo>(sol::table& space) {
	auto ogreInfo = space.new_usertype<OgreInfo>("OgreInfo", sol::no_constructor);
	ogreInfo["isIndirect"] = &OgreInfo::isIndirect;
	ogreInfo["createUniqueResourceName"] = &OgreInfo::createUniqueResourceName;



	auto ogreUtils = sol::state_view(space.lua_state()).create_table();
	ogreUtils["getSubMeshName"] = [](Ogre::Mesh* mesh, unsigned int subMeshIndex) { return OgreUtils::getSubMeshName(mesh, subMeshIndex); };
	space["OgreUtils"] = ogreUtils;

	auto convert = sol::state_view(space.lua_state()).create_table();
	convert["toWF"] = [](const Ogre::Vector3& point) { return Convert::toWF<WFMath::Point<3 >>(point); };

	space["Convert"] = convert;
}