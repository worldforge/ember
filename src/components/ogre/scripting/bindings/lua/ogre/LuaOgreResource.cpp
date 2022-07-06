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
void registerLua<Resource>(sol::table& space) {
	space.new_usertype<Resource>("Resource", sol::no_constructor,
								 "load", [](Resource* self) { self->load(); },
								 "reload", [](Resource* self) { self->reload(); },
								 "isReloadable", &Resource::isReloadable,
								 "getName", &Resource::getName,
								 "getGroup", &Resource::getGroup,
								 "getOrigin", &Resource::getOrigin
	);

	auto resourceGroupManager = space.new_usertype<ResourceGroupManager>("ResourceGroupManager");
	resourceGroupManager["DEFAULT_RESOURCE_GROUP_NAME"] = sol::var(ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME.c_str());
	resourceGroupManager["INTERNAL_RESOURCE_GROUP_NAME"] = sol::var(ResourceGroupManager::INTERNAL_RESOURCE_GROUP_NAME.c_str());
	resourceGroupManager["AUTODETECT_RESOURCE_GROUP_NAME"] = sol::var(ResourceGroupManager::AUTODETECT_RESOURCE_GROUP_NAME.c_str());


	space.new_usertype<ResourceManager>("ResourceManager", sol::no_constructor,
										"getResourceByName", [](ResourceManager* self, const std::string& name, const std::string& group) { return self->getResourceByName(name, group); }
	);

}