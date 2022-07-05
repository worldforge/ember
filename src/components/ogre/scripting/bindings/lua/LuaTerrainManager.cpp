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
#include "components/ogre/terrain/TerrainHandler.h"
#include "components/ogre/terrain/TerrainManager.h"
#include "components/ogre/terrain/ITerrainAdapter.h"
#include "components/ogre/terrain/TerrainPage.h"
#include "components/ogre/Scene.h"

using namespace Ember::OgreView::Terrain;
using namespace Ember::Lua;

template <>
void registerLua<TerrainManager>(sol::table& space) {
	auto terrainHandler = space.new_usertype<TerrainHandler>("TerrainHandler", sol::no_constructor);
	terrainHandler["getMax"] = &TerrainHandler::getMax;
	terrainHandler["getMin"] = &TerrainHandler::getMin;
	terrainHandler["getPageMetersSize"] = &TerrainHandler::getPageMetersSize;



	auto terrainManager = space.new_usertype<TerrainManager>("TerrainManager", sol::no_constructor);
	terrainManager["getHandler"] = &TerrainManager::getHandler;
	terrainManager["getTerrainAdapter"] = &TerrainManager::getTerrainAdapter;
	terrainManager["getScene"] = &TerrainManager::getScene;
	terrainManager["EventTerrainPageGeometryUpdated"] = LuaConnector::make_property(&TerrainManager::EventTerrainPageGeometryUpdated);

}