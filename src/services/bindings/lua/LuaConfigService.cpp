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
#include "services/config/ConfigService.h"


using namespace Ember;
using namespace Ember::Lua;

template <>
void registerLua<ConfigService>(sol::table& space) {
	space["BaseDirType_DATA"] = Ember::BaseDirType_DATA;
	space["BaseDirType_CONFIG"] = Ember::BaseDirType_CONFIG;
	space["BaseDirType_CACHE"] = Ember::BaseDirType_CACHE;
	space["BaseDirType_RUNTIME"] = Ember::BaseDirType_RUNTIME;

	auto configService = space.new_usertype<ConfigService>("ConfigService", sol::no_constructor);
	configService["getValue"] = sol::resolve<varconf::Variable(const std::string&, const std::string&) const>(&ConfigService::getValue);
	configService["setValue"] = &ConfigService::setValue;
	configService["isItemSet"] = &ConfigService::isItemSet;
	configService["itemExists"] = &ConfigService::itemExists;
	configService["getHomeDirectory"] = [](ConfigService* self, BaseDirType baseDirType) { return self->getHomeDirectory(baseDirType).string(); };
	configService["EventChangedConfigItem"] = LuaConnector::make_property(&ConfigService::EventChangedConfigItem);



}