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
#include "components/ogre/World.h"
#include "components/ogre/Avatar.h"
#include "components/ogre/MotionManager.h"
#include "components/ogre/MovementController.h"
#include "components/ogre/authoring/EntityMoveManager.h"
#include "components/ogre/authoring/AuthoringManager.h"
#include "components/ogre/terrain/TerrainManager.h"
#include "components/ogre/camera/MainCamera.h"
#include "components/ogre/environment/Environment.h"
#include "domain/EmberEntity.h"
#include <Eris/View.h>


using namespace Ember::OgreView;
using namespace Ember::Lua;

template <>
void registerLua<World>(sol::table& space) {
	auto world = space.new_usertype<World>("World", sol::no_constructor);
	world["getSceneManager"] = &World::getSceneManager;
	world["getView"] = &World::getView;
	world["getAvatar"] = &World::getAvatar;
	world["getMotionManager"] = &World::getMotionManager;
	world["getMainCamera"] = &World::getMainCamera;
	world["getMovementController"] = &World::getMovementController;
	world["getMoveManager"] = &World::getMoveManager;
	world["getEmberEntity"] = &World::getEmberEntity;
	world["getEntityPickListener"] = &World::getEntityPickListener;
	world["getAuthoringManager"] = &World::getAuthoringManager;
	world["getEnvironment"] = &World::getEnvironment;
	world["getTerrainManager"] = &World::getTerrainManager;
	world["EventGotAvatar"] = LuaConnector::make_property(&World::EventGotAvatar);


}