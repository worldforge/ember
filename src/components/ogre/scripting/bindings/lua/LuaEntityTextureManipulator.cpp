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
#include "components/ogre/widgets/EntityTextureManipulator.h"
#include "components/ogre/widgets/EntityCEGUITexture.h"

using namespace Ember::OgreView::Gui;
using namespace Ember::Lua;

template <>
void registerLua<EntityTextureManipulator>(sol::table& space) {
	auto entityTextureManipulator = space.new_usertype<EntityTextureManipulator>("EntityTextureManipulator");
	entityTextureManipulator["EventMovementStarted"] = LuaConnector::make_property(&EntityTextureManipulator::EventMovementStarted);
	entityTextureManipulator["EventMovementStopped"] = LuaConnector::make_property(&EntityTextureManipulator::EventMovementStopped);

	space.new_usertype<DirectEntityTextureManipulator>("DirectEntityTextureManipulator",
													 sol::constructors<DirectEntityTextureManipulator(CEGUI::Window&, Ember::OgreView::Gui::EntityCEGUITexture&)>(),
													 sol::base_classes, sol::bases<EntityTextureManipulator>()
	);
	space.new_usertype<CameraEntityTextureManipulator>("CameraEntityTextureManipulator",
													 sol::constructors<CameraEntityTextureManipulator(CEGUI::Window&, Ember::OgreView::Gui::EntityCEGUITexture&)>(),
													 sol::base_classes, sol::bases<EntityTextureManipulator>()
	);
	auto combinedEntityTextureManipulator = space.new_usertype<CombinedEntityTextureManipulator>("CombinedEntityTextureManipulator",
																							   sol::constructors<CombinedEntityTextureManipulator(CEGUI::Window&, Ember::OgreView::Gui::EntityCEGUITexture&)>(),
																							   sol::base_classes, sol::bases<EntityTextureManipulator>()
	);
	combinedEntityTextureManipulator["EventEditingStarted"] = LuaConnector::make_property(&CombinedEntityTextureManipulator::EventEditingStarted);
	combinedEntityTextureManipulator["EventEditingStopped"] = LuaConnector::make_property(&CombinedEntityTextureManipulator::EventEditingStopped);

}