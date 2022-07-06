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

using namespace Ogre;

template<>
void registerLua<Texture>(sol::table& space) {
	space.new_usertype<Texture>("Texture", sol::no_constructor,
								sol::base_classes, sol::bases<Resource>()
	);

	auto textureManager = space.new_usertype<TextureManager>("TextureManager", sol::no_constructor,
															 sol::base_classes, sol::bases<ResourceManager>()
	);

	textureManager["getSingleton"] = &TextureManager::getSingleton;
	textureManager["load"] = [](TextureManager* self, const String& name, const String& group) { return self->load(name, group); };
	textureManager["getByName"] = [](TextureManager* self, const String& name, const String& group) { return self->getByName(name, group); };
	textureManager["create"] = [](TextureManager* self, const String& name, const String& group) { return self->create(name, group); };
}