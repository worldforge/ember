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

#include "BindingsAdapters.h"
#include "components/lua/Connector.h"
#include "../../AdapterBase.h"
#include "components/ogre/widgets/adapters/ModelDefinitionsAdapter.h"
#include "components/ogre/widgets/ListHolder.h"

using namespace Ember::OgreView::Gui::Adapters;
using namespace Ember::OgreView::Gui;
using namespace Ember::Lua;

void registerBindingsAdapters(sol::state_view& lua) {
	auto Ember = lua["Ember"].get_or_create<sol::table>();
	auto OgreView = Ember["OgreView"].get_or_create<sol::table>();
	auto Gui = OgreView["Gui"].get_or_create<sol::table>();
	auto Adapters = Gui["Adapters"].get_or_create<sol::table>();
	Adapters.new_usertype<ModelDefinitionsAdapter>("ModelDefinitionsAdapter",
												   sol::constructors<ModelDefinitionsAdapter(ListHolder&)>(),
												   "update", &ModelDefinitionsAdapter::update
	);

}
