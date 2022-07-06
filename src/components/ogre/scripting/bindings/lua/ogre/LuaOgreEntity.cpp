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
void registerLua<Entity>(sol::table& space) {
	auto entity = space.new_usertype<Entity>("Entity");
//							  "getMesh", [](Entity* self) { return std::shared_ptr(self->getMesh()); },
	entity["getMesh"] = &Entity::getMesh;
	entity["setMaterialName"] = &Entity::setMaterialName;

	auto subEntity = space.new_usertype<SubEntity>("SubEntity");
	subEntity["getMaterialName"] = &SubEntity::getMaterialName;
	subEntity["setMaterialName"] = &SubEntity::setMaterialName;
	subEntity["setVisible"] = &SubEntity::setVisible;
	subEntity["isVisible"] = &SubEntity::isVisible;
	subEntity["getMaterial"] = &SubEntity::getMaterial;
}