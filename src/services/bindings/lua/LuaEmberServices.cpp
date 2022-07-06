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
#include "services/EmberServices.h"
#include "services/config/ConfigService.h"
#include "services/metaserver/MetaserverService.h"
#include "services/server/ServerService.h"
#include "services/serversettings/ServerSettings.h"
#include "services/scripting/ScriptingService.h"

using namespace Ember;
using namespace Ember::Lua;

template <>
void registerLua<EmberServices>(sol::table& space) {
	auto emberServices = space.new_usertype<EmberServices>("EmberServices", sol::no_constructor);
	emberServices["getSingleton"] = &EmberServices::getSingleton;
	emberServices["getConfigService"] = &EmberServices::getConfigService;
	emberServices["getMetaserverService"] = &EmberServices::getMetaserverService;
	emberServices["getServerService"] = &EmberServices::getServerService;
	emberServices["getScriptingService"] = &EmberServices::getScriptingService;
	emberServices["getServerSettingsService"] = &EmberServices::getServerSettingsService;
}