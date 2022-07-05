/*
 Copyright (C) 2022 Erik Ogenvik

 This program is free software; you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation; either version 2 of the License, or
 (at your option) any later version.

 This program is distributed in the hope that it will be useful; but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program; if not, write to the Free Software
 Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#include "BindingsEmberOgre.h"
#include "RegisterLua.h"

namespace Ember::OgreView {
class Avatar;

class GUIManager;

class EmberOgre;

class EntityWorldPickListener;

class MovementController;

class MotionManager;

class OgreInfo;

class Scene;

class SimpleRenderContext;

class World;

namespace Environment {
class Environment;
}
namespace Gui {
class ActionBarIcon;

class AtlasHelper;

class AssetsManager;

class ColouredListItem;

class Compass;

class ContainerView;

class ConsoleAdapter;

class EntityCEGUITexture;

class EntityCreatorTypeHelper;

class EntityEditor;

class EntityIcon;

class EntityTextureManipulator;

class EntityTooltip;

struct HelpMessage;

class IconBar;

class ListHolder;

class ModelRenderer;

class ModelEditHelper;

class MovableObjectRenderer;

class MeshInfoProvider;

class OgreEntityRenderer;

class QuaternionAdapter;

class QuickHelp;

class Vector3Adapter;

class Widget;
namespace Adapters {
class ModelDefinitionsAdapter;
namespace Eris {
class RuleTreeAdapter;
}
namespace Ogre {
class ResourceListAdapter;
}
}

namespace Icons {
class Icon;
}
}
namespace Authoring {
class AuthoringManager;

class RuleEditor;
}
namespace Lod {
class LodManager;
}
namespace Model {
class Model;

class ModelRepresentation;
}
namespace Camera {
class MainCamera;
}
namespace Terrain {
class Map;

class TerrainEditor;

class TerrainLayerDefinitionManager;

class TerrainManager;
}
}

void registerBindingsEmberOgre(sol::state_view& lua) {
	auto Ember = lua["Ember"].get_or_create<sol::table>();
	auto OgreView = Ember["OgreView"].get_or_create<sol::table>();
	auto Gui = OgreView["Gui"].get_or_create<sol::table>();
	auto Icons = Gui["Icons"].get_or_create<sol::table>();
	auto Adapters = Gui["Adapters"].get_or_create<sol::table>();
	auto AdaptersOgre = Adapters["Ogre"].get_or_create<sol::table>();
	auto AdaptersEris = Adapters["Eris"].get_or_create<sol::table>();
	auto AdaptersAtlas = Adapters["Atlas"].get_or_create<sol::table>();
	auto Authoring = OgreView["Authoring"].get_or_create<sol::table>();
	auto Environment = OgreView["Environment"].get_or_create<sol::table>();
	auto Lod = OgreView["Lod"].get_or_create<sol::table>();
	auto Camera = OgreView["Camera"].get_or_create<sol::table>();
	auto Terrain = OgreView["Terrain"].get_or_create<sol::table>();
	auto Model = OgreView["Model"].get_or_create<sol::table>();

	registerLua<Ember::OgreView::EmberOgre>(OgreView);
	registerLua<Ember::OgreView::Avatar>(OgreView);
	registerLua<Ember::OgreView::GUIManager>(OgreView);
	registerLua<Ember::OgreView::EntityWorldPickListener>(OgreView);
	registerLua<Ember::OgreView::MotionManager>(OgreView);
	registerLua<Ember::OgreView::MovementController>(OgreView);
	registerLua<Ember::OgreView::OgreInfo>(OgreView);
	registerLua<Ember::OgreView::Scene>(OgreView);
	registerLua<Ember::OgreView::SimpleRenderContext>(OgreView);
	registerLua<Ember::OgreView::World>(OgreView);

	registerLua<Ember::OgreView::Authoring::AuthoringManager>(Authoring);
	registerLua<Ember::OgreView::Authoring::RuleEditor>(Authoring);

	registerLua<Ember::OgreView::Camera::MainCamera>(Camera);

	registerLua<Ember::OgreView::Environment::Environment>(Environment);

	registerLua<Ember::OgreView::Gui::ActionBarIcon>(Gui);
	registerLua<Ember::OgreView::Gui::AtlasHelper>(Gui);
	registerLua<Ember::OgreView::Gui::AssetsManager>(Gui);
	registerLua<Ember::OgreView::Gui::ColouredListItem>(Gui);
	registerLua<Ember::OgreView::Gui::Compass>(Gui);
	registerLua<Ember::OgreView::Gui::ContainerView>(Gui);
	registerLua<Ember::OgreView::Gui::ConsoleAdapter>(Gui);
	registerLua<Ember::OgreView::Gui::EntityCEGUITexture>(Gui);
	registerLua<Ember::OgreView::Gui::EntityCreatorTypeHelper>(Gui);
	registerLua<Ember::OgreView::Gui::EntityEditor>(Gui);
	registerLua<Ember::OgreView::Gui::EntityIcon>(Gui);
	registerLua<Ember::OgreView::Gui::EntityTextureManipulator>(Gui);
	registerLua<Ember::OgreView::Gui::EntityTooltip>(Gui);
	registerLua<Ember::OgreView::Gui::IconBar>(Gui);
	registerLua<Ember::OgreView::Gui::Icons::Icon>(Icons);
	registerLua<Ember::OgreView::Gui::ListHolder>(Gui);
	registerLua<Ember::OgreView::Gui::MeshInfoProvider>(Gui);
	registerLua<Ember::OgreView::Gui::MovableObjectRenderer>(Gui);
	registerLua<Ember::OgreView::Gui::ModelRenderer>(Gui);
	registerLua<Ember::OgreView::Gui::ModelEditHelper>(Gui);
	registerLua<Ember::OgreView::Gui::OgreEntityRenderer>(Gui);
	registerLua<Ember::OgreView::Gui::QuickHelp>(Gui);
	registerLua<Ember::OgreView::Gui::QuaternionAdapter>(Gui);
	registerLua<Ember::OgreView::Gui::HelpMessage>(Gui);
	registerLua<Ember::OgreView::Gui::Vector3Adapter>(Gui);
	registerLua<Ember::OgreView::Gui::Widget>(Gui);
	registerLua<Ember::OgreView::Gui::Adapters::ModelDefinitionsAdapter>(Adapters);
	registerLua<Ember::OgreView::Gui::Adapters::Eris::RuleTreeAdapter>(AdaptersEris);
	registerLua<Ember::OgreView::Gui::Adapters::Ogre::ResourceListAdapter>(AdaptersOgre);


	registerLua<Ember::OgreView::Lod::LodManager>(Lod);

	registerLua<Ember::OgreView::Model::Model>(Model);
	registerLua<Ember::OgreView::Model::ModelRepresentation>(Model);

	registerLua<Ember::OgreView::Terrain::Map>(Terrain);
	registerLua<Ember::OgreView::Terrain::TerrainEditor>(Terrain);
	registerLua<Ember::OgreView::Terrain::TerrainLayerDefinitionManager>(Terrain);
	registerLua<Ember::OgreView::Terrain::TerrainManager>(Terrain);


//	auto mousePicker = lua.new_usertype<MousePicker>("MousePicker",//												authoringManager["doMousePicking"] = &MousePicker::doMousePicking;//												authoringManager["getLastPickedEntity"] = &MousePicker::getLastPickedEntity;//												authoringManager["EventPickedEntity",//													 sol::property([](MousePicker* self) {
//														 return Connector::makeConnector<RefValueAdapter<const Ember::OgreView::EntityPickResult>, RefValueAdapter<const Ember::OgreView::MousePickerArgs>>(self->EventPickedEntity);
//													 });//												authoringManager["EventPickedNothing",//													 sol::property([](MousePicker* self) {
//														 return Connector::makeConnector<RefValueAdapter<const Ember::OgreView::MousePickerArgs>>(self->EventPickedNothing);
//													 })
//	);
//
//	mousePicker["ClickMasks"] = lua.create_table_with(
//			"CM_AVATAR"] = MousePicker::CM_AVATAR;//			"CM_ENTITY"] = MousePicker::CM_ENTITY;//			"CM_NATURE"] = MousePicker::CM_NATURE;//			"CM_UNDEFINED"] = MousePicker::CM_UNDEFINED
//	);
}
