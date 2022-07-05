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
#include "components/ogre/EmberOgre.h"
#include "components/ogre/GUIManager.h"
#include "components/ogre/World.h"
#include "components/ogre/Screen.h"
#include "components/ogre/MotionManager.h"
#include "components/ogre/terrain/TerrainManager.h"
#include "domain/EmberEntity.h"


using namespace Ember::OgreView;
using namespace Ember::Lua;

template <>
void registerLua<EmberOgre>(sol::table& space) {
	auto emberOgre = space.new_usertype<EmberOgre>("EmberOgre", sol::no_constructor);
	emberOgre["getSingleton"] = &EmberOgre::getSingleton;
	emberOgre["getWorld"] = &EmberOgre::getWorld;
	emberOgre["getScreen"] = &EmberOgre::getScreen;
	emberOgre["getRenderWindow"] = &EmberOgre::getRenderWindow;

	auto doWithEntity = [](EmberOgre* self, const std::string& entityId, sol::function entityFoundFn, sol::function entityNotFoundFn) {
		auto entity = self->getWorld()->getEmberEntity(entityId);
		if (entity) {
			auto result = entityFoundFn(entity);
			if (!result.valid()) {
				S_LOG_WARNING(result.get<sol::error>());
			}
		} else {
			if (entityNotFoundFn) {
				auto result = entityNotFoundFn(entityId);
				if (!result.valid()) {
					S_LOG_WARNING(result.get<sol::error>());
				}
			}
		}
	};

	emberOgre["doWithEntity"] = sol::overload([=](EmberOgre* self, const std::string& entityId, sol::function entityFoundFn, sol::function entityNotFoundFn) {
		doWithEntity(self, entityId, std::move(entityFoundFn), entityNotFoundFn);
	}, [=](EmberOgre* self, const std::string& entityId, sol::function entityFoundFn) {
		doWithEntity(self, entityId, std::move(entityFoundFn), sol::nil);
	});
	emberOgre["EventCreatedAvatarEntity"] = LuaConnector::make_property(&EmberOgre::EventCreatedAvatarEntity);
	emberOgre["EventGUIManagerCreated"] = LuaConnector::make_property(&EmberOgre::EventGUIManagerCreated);
	emberOgre["EventGUIManagerBeingDestroyed"] = LuaConnector::make_property(&EmberOgre::EventGUIManagerBeingDestroyed);
	emberOgre["EventMotionManagerCreated"] = LuaConnector::make_property(&EmberOgre::EventMotionManagerCreated);
	emberOgre["EventTerrainManagerCreated"] = LuaConnector::make_property(&EmberOgre::EventTerrainManagerCreated);
	emberOgre["EventTerrainManagerBeingDestroyed"] = LuaConnector::make_property(&EmberOgre::EventTerrainManagerBeingDestroyed);
	emberOgre["EventTerrainManagerDestroyed"] = LuaConnector::make_property(&EmberOgre::EventTerrainManagerDestroyed);
	emberOgre["EventMotionManagerDestroyed"] = LuaConnector::make_property(&EmberOgre::EventMotionManagerDestroyed);
	emberOgre["EventMovementControllerCreated"] = LuaConnector::make_property(&EmberOgre::EventMovementControllerCreated);
	emberOgre["EventMovementControllerDestroyed"] = LuaConnector::make_property(&EmberOgre::EventMovementControllerDestroyed);
	emberOgre["EventWorldCreated"] = LuaConnector::make_property(&EmberOgre::EventWorldCreated);
	emberOgre["EventWorldBeingDestroyed"] = LuaConnector::make_property(&EmberOgre::EventWorldBeingDestroyed);
	emberOgre["EventWorldDestroyed"] = LuaConnector::make_property(&EmberOgre::EventWorldDestroyed);

}