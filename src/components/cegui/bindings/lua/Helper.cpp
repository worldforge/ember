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
#include "Helper.h"
using namespace Ember::Cegui;
template <>
void registerLua<Helper>(sol::table& space) {
	auto helper = space.new_usertype<Helper>("Helper", sol::no_constructor);
	helper["notifyScreenAreaChanged"] = &Helper::notifyScreenAreaChanged;
	helper["Window_getChildCount"] = &Helper::Window_getChildCount;
	helper["ImageManager_getSingleton"] = &Helper::ImageManager_getSingleton;
	helper["Window_getChildRecursive"] = &Helper::Window_getChildRecursive;
	helper["Window_removeChild"] = &Helper::Window_removeChild;
	helper["calculateRenderedCentredStringVerticalExtent"] = &Helper::calculateRenderedCentredStringVerticalExtent;
}