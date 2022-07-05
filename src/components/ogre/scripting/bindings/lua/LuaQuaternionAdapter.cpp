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
#include "components/ogre/widgets/QuaternionAdapter.h"


using namespace Ember::OgreView::Gui;
using namespace Ember::Lua;

template <>
void registerLua<QuaternionAdapter>(sol::table& space) {
	auto quaternionAdapter = space.new_usertype<QuaternionAdapter>("QuaternionAdapter",
																 sol::constructors<QuaternionAdapter(CEGUI::Window*, CEGUI::Window*, CEGUI::Window*, CEGUI::Window*)>());
	quaternionAdapter["getValue"] = &QuaternionAdapter::getValue;
	quaternionAdapter["getOriginalValue"] = &QuaternionAdapter::getOriginalValue;
	quaternionAdapter["setValue"] = &QuaternionAdapter::setValue;
	quaternionAdapter["updateGui"] = &QuaternionAdapter::updateGui;
	quaternionAdapter["EventValueChanged"] = LuaConnector::make_property(&QuaternionAdapter::EventValueChanged);


}