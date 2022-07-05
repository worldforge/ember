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
#include "components/ogre/widgets/icons/Icon.h"
#include "components/ogre/widgets/icons/IconManager.h"
#include "domain/EmberEntity.h"
#include <Eris/TypeInfo.h>

using namespace Ember::OgreView::Gui::Icons;
using namespace Ember::Lua;

template <>
void registerLua<Icon>(sol::table& space) {
	auto icon = space.new_usertype<Icon>("Icon", sol::no_constructor);
	icon["getImage"] = &Icon::getImage;



	auto iconManager = space.new_usertype<IconManager>("IconManager", sol::no_constructor);
	iconManager["getIcon"] = sol::overload([](IconManager* self, int pixelWidth, Ember::EmberEntity* entity) { return self->getIcon(pixelWidth, entity); },
										   [](IconManager* self, int pixelWidth, Eris::TypeInfo* erisType) { return self->getIcon(pixelWidth, erisType); });
	iconManager["render"] = sol::overload([](IconManager* self, Icon& icon, const std::string& modelName) { self->render(icon, modelName); },
										  [](IconManager* self, Icon& icon, Ember::EmberEntity& entity) { self->render(icon, entity); },
										  [](IconManager* self, Icon& icon, Eris::TypeInfo& erisType) { self->render(icon, erisType); });

}