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
#include "components/ogre/widgets/EntityEditor.h"
#include "components/ogre/World.h"
#include <Eris/Entity.h>

using namespace Ember::OgreView::Gui;
using namespace Ember::Lua;

template <>
void registerLua<EntityEditor>(sol::table& space) {
	auto entityEditor = space.new_usertype<EntityEditor>("EntityEditor",
													   sol::constructors<EntityEditor(Ember::OgreView::World&, Eris::Entity&, Ember::OgreView::Gui::Adapters::Atlas::MapAdapter*)>());
	entityEditor["submitChanges"] = &EntityEditor::submitChanges;
	entityEditor["createMapElement"] = &EntityEditor::createMapElement;
	entityEditor["createListElement"] = &EntityEditor::createListElement;
	entityEditor["createStringElement"] = &EntityEditor::createStringElement;
	entityEditor["createIntElement"] = &EntityEditor::createIntElement;
	entityEditor["createFloatElement"] = &EntityEditor::createFloatElement;
	entityEditor["createPosition2dElement"] = &EntityEditor::createPosition2dElement;
	entityEditor["addGoal"] = &EntityEditor::addGoal;
	entityEditor["updateGoal"] = &EntityEditor::updateGoal;
	entityEditor["removeGoal"] = &EntityEditor::removeGoal;
	entityEditor["addKnowledge"] = &EntityEditor::addKnowledge;
	entityEditor["addMarker"] = &EntityEditor::addMarker;
	entityEditor["removeMarker"] = &EntityEditor::removeMarker;
	entityEditor["createPoint"] = &EntityEditor::createPoint;
	entityEditor["getGoals"] = &EntityEditor::getGoals;
	entityEditor["getThoughts"] = &EntityEditor::getThoughts;
	entityEditor["getPath"] = &EntityEditor::getPath;
	entityEditor["getGoalInfo"] = &EntityEditor::getGoalInfo;
	entityEditor["parseElementMap"] = &EntityEditor::parseElementMap;
	entityEditor["EventGotThought"] = LuaConnector::make_property(&EntityEditor::EventGotThought);
	entityEditor["EventGotGoal"] = LuaConnector::make_property(&EntityEditor::EventGotGoal);
	entityEditor["EventGotEmptyGoals"] = LuaConnector::make_property(&EntityEditor::EventGotEmptyGoals);
	entityEditor["EventGotGoalInfo"] = LuaConnector::make_property(&EntityEditor::EventGotGoalInfo);


}