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
#include "components/ogre/widgets/EntityCEGUITexture.h"
#include "components/ogre/SimpleRenderContext.h"


using namespace Ember::OgreView::Gui;
using namespace Ember::Lua;

template <>
void registerLua<EntityCEGUITexture>(sol::table& space) {
	auto entityCEGUITexture = space.new_usertype<EntityCEGUITexture>("EntityCEGUITexture", sol::no_constructor);
	entityCEGUITexture["getImage"] = &EntityCEGUITexture::getImage;
	entityCEGUITexture["getRenderContext"] = &EntityCEGUITexture::getRenderContext;

}