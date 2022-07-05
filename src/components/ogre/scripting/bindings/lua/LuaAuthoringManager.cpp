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
#include "components/ogre/authoring/AuthoringManager.h"
#include "components/ogre/authoring/EntityMover.h"
#include "components/ogre/authoring/EntityMoveManager.h"
#include "components/ogre/authoring/EntityRecipe.h"
#include "domain/EmberEntity.h"

using namespace Ember::OgreView;
using namespace Ember::OgreView::Authoring;
using namespace Ember::OgreView::Gui;
using namespace Ember::Lua;

template <>
void registerLua<AuthoringManager>(sol::table& space) {
	auto authoringManager = space.new_usertype<AuthoringManager>("AuthoringManager", sol::no_constructor);
	authoringManager["displayAuthoringVisualization"] = &AuthoringManager::displayAuthoringVisualization;
	authoringManager["hideAuthoringVisualization"] = &AuthoringManager::hideAuthoringVisualization;
	authoringManager["displaySimpleEntityVisualization"] = &AuthoringManager::displaySimpleEntityVisualization;
	authoringManager["hideSimpleEntityVisualization"] = &AuthoringManager::hideSimpleEntityVisualization;
	authoringManager["hasSimpleEntityVisualization"] = &AuthoringManager::hasSimpleEntityVisualization;
	authoringManager["startMovement"] = &AuthoringManager::startMovement;
	authoringManager["stopMovement"] = &AuthoringManager::stopMovement;

	auto entityMoveManager = space.new_usertype<EntityMoveManager>("EntityMoveManager", sol::no_constructor);
	entityMoveManager["startMove"] = &EntityMoveManager::startMove;
	entityMoveManager["EventStartMoving"] = LuaConnector::make_property(&EntityMoveManager::EventStartMoving);
	entityMoveManager["EventFinishedMoving"] = LuaConnector::make_property(&EntityMoveManager::EventFinishedMoving);
	entityMoveManager["EventCancelledMoving"] = LuaConnector::make_property(&EntityMoveManager::EventCancelledMoving);

	auto entityRecipe = space.new_usertype<EntityRecipe>("EntityRecipe", sol::no_constructor);
	entityRecipe["setAuthor"] = &EntityRecipe::setAuthor;
	entityRecipe["getAuthor"] = &EntityRecipe::getAuthor;
	entityRecipe["setDescription"] = &EntityRecipe::setDescription;
	entityRecipe["getDescription"] = &EntityRecipe::getDescription;
}