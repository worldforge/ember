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
#include "components/ogre/widgets/EntityCreatorTypeHelper.h"
#include <Eris/Avatar.h>

using namespace Ember::OgreView::Gui;
using namespace Ember::Lua;

template <>
void registerLua<EntityCreatorTypeHelper>(sol::table& space) {
	auto entityCreatorTypeHelper = space.new_usertype<EntityCreatorTypeHelper>("EntityCreatorTypeHelper",
																			 sol::constructors<EntityCreatorTypeHelper(Eris::Avatar&,
																													   CEGUI::Tree&,
																													   CEGUI::Editbox&,
																													   CEGUI::PushButton&,
																													   CEGUI::Window&,
																													   CEGUI::Combobox&,
																													   CEGUI::Window&,
																													   CEGUI::ToggleButton&,
																													   CEGUI::Editbox&)>());
	entityCreatorTypeHelper["EventCreateFromType"] = LuaConnector::make_property(&EntityCreatorTypeHelper::EventCreateFromType);

}