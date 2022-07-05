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
#include "components/ogre/terrain/TerrainEditor.h"
#include "components/ogre/terrain/TerrainEditorOverlay.h"
#include "components/ogre/terrain/TerrainManager.h"
#include "components/ogre/terrain/TerrainLayerDefinition.h"
#include "components/ogre/camera/MainCamera.h"

using namespace Ember::OgreView::Terrain;
using namespace Ember::Lua;

template <>
void registerLua<TerrainEditor>(sol::table& space) {
	auto basePointUserObject = space.new_usertype<BasePointUserObject>("BasePointUserObject", sol::no_constructor);
	basePointUserObject["getBasePoint"] = &BasePointUserObject::getBasePoint;
	basePointUserObject["getBasePointMarkerNode"] = &BasePointUserObject::getBasePointMarkerNode;
	basePointUserObject["getPosition"] = &BasePointUserObject::getPosition;
	basePointUserObject["getHeight"] = &BasePointUserObject::getHeight;
	basePointUserObject["translate"] = &BasePointUserObject::translate;
	basePointUserObject["getRoughness"] = &BasePointUserObject::getRoughness;
	basePointUserObject["getFalloff"] = &BasePointUserObject::getFalloff;
	basePointUserObject["EventUpdatedPosition"] = LuaConnector::make_property(&BasePointUserObject::EventUpdatedPosition);

	auto terrainEditBasePointMovement = space.new_usertype<TerrainEditBasePointMovement>("TerrainEditBasePointMovement", sol::no_constructor);
	terrainEditBasePointMovement["mVerticalMovement"] = &TerrainEditBasePointMovement::mVerticalMovement;
	terrainEditBasePointMovement["mPosition"] = &TerrainEditBasePointMovement::mPosition;

	auto terrainEditor = space.new_usertype<TerrainEditor>("TerrainEditor",
															 sol::constructors<TerrainEditor(Ember::OgreView::Terrain::TerrainManager&, Ember::OgreView::Camera::MainCamera&)>());
	terrainEditor["showOverlay"] = &TerrainEditor::showOverlay;
	terrainEditor["hideOverlay"] = &TerrainEditor::hideOverlay;
	terrainEditor["isOverlayShown"] = &TerrainEditor::isOverlayShown;
	terrainEditor["getRadius"] = &TerrainEditor::getRadius;
	terrainEditor["setRadius"] = &TerrainEditor::setRadius;
	terrainEditor["getFalloff"] = &TerrainEditor::getFalloff;
	terrainEditor["setFalloff"] = &TerrainEditor::setFalloff;
	terrainEditor["EventOverlayCreated"] = LuaConnector::make_property(&TerrainEditor::EventOverlayCreated);
	terrainEditor["EventOverlayDestroyed"] = LuaConnector::make_property(&TerrainEditor::EventOverlayDestroyed);

	auto terrainEditorOverlay = space.new_usertype<TerrainEditorOverlay>("TerrainEditorOverlay", sol::no_constructor);
	terrainEditorOverlay["commitAction"] = &TerrainEditorOverlay::commitAction;
	terrainEditorOverlay["getCurrentBasePointUserObject"] = &TerrainEditorOverlay::getCurrentBasePointUserObject;
	terrainEditorOverlay["sendChangesToServer"] = &TerrainEditorOverlay::sendChangesToServer;
	terrainEditorOverlay["createAction"] = &TerrainEditorOverlay::createAction;
	terrainEditorOverlay["undoLastAction"] = &TerrainEditorOverlay::undoLastAction;
	terrainEditorOverlay["redoAction"] = &TerrainEditorOverlay::redoAction;
	terrainEditorOverlay["setRoughness"] = &TerrainEditorOverlay::setRoughness;
	terrainEditorOverlay["setFalloff"] = &TerrainEditorOverlay::setFalloff;
	terrainEditorOverlay["EventPickedBasePoint"] = LuaConnector::make_property(&TerrainEditorOverlay::EventPickedBasePoint);
	terrainEditorOverlay["EventActionCreated"] = LuaConnector::make_property(&TerrainEditorOverlay::EventActionCreated);
	terrainEditorOverlay["EventSelectedBasePointUpdatedPosition"] = LuaConnector::make_property(&TerrainEditorOverlay::EventSelectedBasePointUpdatedPosition);

}