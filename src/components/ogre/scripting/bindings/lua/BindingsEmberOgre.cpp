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
#include "components/ogre/authoring/AuthoringManager.h"
#include "components/ogre/authoring/EntityMover.h"
#include "components/ogre/authoring/EntityMoveManager.h"
#include "components/lua/Connector.h"
#include "components/ogre/widgets/AtlasHelper.h"
#include "components/ogre/widgets/ActionBarIconDragDropTarget.h"
#include "components/ogre/widgets/ActionBarIcon.h"
#include "components/ogre/widgets/ActionBarIconManager.h"
#include "components/ogre/widgets/ActionBarIconSlot.h"
#include "components/ogre/widgets/AssetsManager.h"
#include "components/ogre/widgets/ActionBarInput.h"
#include "components/ogre/widgets/EntityIcon.h"
#include "components/ogre/widgets/EntityIconManager.h"
#include "components/ogre/widgets/icons/IconManager.h"
#include "components/ogre/widgets/icons/Icon.h"
#include "components/ogre/widgets/IconBase.h"
#include "components/ogre/widgets/IconBar.h"
#include "domain/EmberEntity.h"
#include "components/ogre/Avatar.h"
#include "components/ogre/widgets/ColouredListItem.h"
#include "components/ogre/widgets/Compass.h"
#include "components/ogre/terrain/ITerrainAdapter.h"
#include "components/ogre/terrain/Map.h"
#include "components/ogre/widgets/ConsoleAdapter.h"
#include "components/ogre/widgets/ContainerView.h"
#include "components/ogre/Convert.h"
#include "components/ogre/EmberOgre.h"
#include "components/ogre/Screen.h"
#include "components/ogre/World.h"
#include "components/ogre/GUIManager.h"
#include "components/ogre/widgets/EntityCEGUITexture.h"
#include "components/ogre/SimpleRenderContext.h"
#include "components/ogre/widgets/EntityCreatorTypeHelper.h"
#include "components/ogre/widgets/EntityEditor.h"
#include "components/ogre/widgets/EntityIconDragDropPreview.h"
#include "components/ogre/widgets/ListHolder.h"
#include "components/ogre/authoring/EntityRecipe.h"
#include "components/ogre/widgets/EntityTextureManipulator.h"
#include "components/ogre/widgets/EntityTooltip.h"
#include "components/ogre/EntityWorldPickListener.h"
#include "components/ogre/environment/IEnvironmentProvider.h"
#include "components/ogre/environment/Environment.h"
#include "components/ogre/lod/LodDefinition.h"
#include "components/ogre/lod/LodDefinitionManager.h"
#include "components/ogre/lod/LodManager.h"
#include "components/ogre/camera/MainCamera.h"
#include "components/ogre/terrain/Map.h"
#include "components/ogre/model/Model.h"
#include "components/ogre/model/SubModel.h"
#include "components/ogre/model/SubModelPart.h"
#include "components/ogre/model/ModelDefinition.h"
#include "components/ogre/model/ModelDefinitionManager.h"
#include "components/ogre/widgets/MeshInfoProvider.h"
#include "components/ogre/model/ModelDefinitionAtlasComposer.h"
#include "components/ogre/widgets/ModelEditHelper.h"
#include "components/ogre/widgets/ModelRenderer.h"
#include "components/ogre/model/ModelRepresentation.h"
#include "components/ogre/MousePicker.h"
#include "components/ogre/MotionManager.h"
#include "components/ogre/OgreInfo.h"
#include "components/ogre/widgets/QuaternionAdapter.h"
#include "components/ogre/widgets/Vector3Adapter.h"
#include "components/ogre/widgets/adapters/eris/RuleTreeAdapter.h"
#include "components/ogre/widgets/adapters/ListBinder.h"
#include "components/ogre/widgets/adapters/eris/RuleTreeAdapter.h"
#include "components/ogre/widgets/adapters/ogre/ResourceListAdapter.h"
#include "components/ogre/MovementController.h"
#include "components/ogre/widgets/QuickHelp.h"
#include "components/ogre/widgets/QuickHelpCursor.h"
#include "components/ogre/widgets/Widget.h"
#include "components/ogre/terrain/TerrainHandler.h"
#include "components/ogre/terrain/TerrainManager.h"
#include "components/ogre/terrain/TerrainLayerDefinition.h"
#include "components/ogre/terrain/TerrainLayerDefinitionManager.h"
#include "components/ogre/terrain/TerrainEditor.h"
#include "components/ogre/terrain/TerrainEditorOverlay.h"
#include "components/ogre/terrain/TerrainPage.h"
#include "components/ogre/authoring/RuleEditor.h"
#include "components/ogre/Scene.h"
#include "components/ogre/Screen.h"
#include "components/ogre/World.h"
#include "domain/EntityTalk.h"
#include "helpers/OgreUtils.h"


#include <Eris/Entity.h>
#include <Eris/Avatar.h>
#include <Eris/Connection.h>

#include <wfmath/point.h>
#include <wfmath/vector.h>

#include <Ogre.h>
#include <OgreTexture.h>
#include <OgreTextureManager.h>
#include <OgreSharedPtr.h>

#include <CEGUI/CEGUI.h>
#include <Atlas/Objects/Root.h>
#include <Atlas/Objects/SmartPtr.h>

#include <utility>


using namespace Ember::OgreView;
using namespace Ember::OgreView::Gui;
using namespace Ember::OgreView::Gui::Icons;
using namespace Ember::Lua;
using namespace Ember::OgreView::Authoring;
using namespace Ember::OgreView::Environment;
using namespace Ember::OgreView::Lod;
using namespace Ember::OgreView::Camera;
using namespace Ember::OgreView::Terrain;
using namespace Ember::OgreView::Model;
using namespace Ember;
using namespace Ember::OgreView::Gui::Adapters::Ogre;
using namespace Ember::OgreView::Gui::Adapters::Eris;
using namespace Ember::OgreView::Gui::Adapters::Atlas;

namespace sol {
template<typename T>
struct unique_usertype_traits<Ogre::SharedPtr<T>> {
	typedef T type;
	typedef Ogre::SharedPtr<T> actual_type;
	static const bool value = true;

	static bool is_null(const actual_type& ptr) {
		return ptr == nullptr;
	}

	static type* get(const actual_type& ptr) {
		return ptr.get();
	}
};
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

	auto actionBarInput = Gui.new_usertype<ActionBarInput>("ActionBarInput",
														   sol::constructors<ActionBarInput(const std::string&)>());
	actionBarInput["EventGotHotkeyInput"] = LuaConnector::make_property(&ActionBarInput::EventGotHotkeyInput);


	auto actionBarIconDragDropTarget = Gui.new_usertype<ActionBarIconDragDropTarget>("ActionBarIconDragDropTarget",
																					 sol::constructors<ActionBarIconDragDropTarget(CEGUI::Window*)>()
	);
	actionBarIconDragDropTarget["EventIconEntered"] = LuaConnector::make_property(&ActionBarIconDragDropTarget::EventIconEntered);
	actionBarIconDragDropTarget["EventIconLeaves"] = LuaConnector::make_property(&ActionBarIconDragDropTarget::EventIconLeaves);
	actionBarIconDragDropTarget["EventActionBarIconDropped"] = LuaConnector::make_property(&ActionBarIconDragDropTarget::EventActionBarIconDropped);
	actionBarIconDragDropTarget["EventEntityIconDropped"] = LuaConnector::make_property(&ActionBarIconDragDropTarget::EventEntityIconDropped);

	auto actionBarIcon = Gui.new_usertype<ActionBarIcon>("ActionBarIcon",
														 sol::base_classes, sol::bases<ActionBarIconDragDropTarget>());
	actionBarIcon["getImage"] = &ActionBarIcon::getImage;
	actionBarIcon["getDragContainer"] = &ActionBarIcon::getDragContainer;
	actionBarIcon["getIcon"] = &ActionBarIcon::getIcon;
	actionBarIcon["setSlot"] = &ActionBarIcon::setSlot;
	actionBarIcon["getSlot"] = &ActionBarIcon::getSlot;
	actionBarIcon["setTooltipText"] = &ActionBarIcon::setTooltipText;
	actionBarIcon["defaultAction"] = &ActionBarIcon::defaultAction;
	actionBarIcon[sol::meta_function::equal_to] = [](ActionBarIcon* lhs, ActionBarIcon* rhs) { return *lhs == *rhs; };

	auto actionBarIconManager = Gui.new_usertype<ActionBarIconManager>("ActionBarIconManager");
	actionBarIconManager["createSlot"] = &ActionBarIconManager::createSlot;
	actionBarIconManager["destroySlot"] = &ActionBarIconManager::destroySlot;
	actionBarIconManager["createIcon"] = &ActionBarIconManager::createIcon;
	actionBarIconManager["destroyIcon"] = &ActionBarIconManager::destroyIcon;
	actionBarIconManager["getSavedValue"] = &ActionBarIconManager::getSavedValue;
	actionBarIconManager["saveValue"] = &ActionBarIconManager::saveValue;
	actionBarIconManager["eraseValue"] = &ActionBarIconManager::eraseValue;
	actionBarIconManager["EventIconDragStart"] = LuaConnector::make_property(&ActionBarIconManager::EventIconDragStart);
	actionBarIconManager["EventIconDragStop"] = LuaConnector::make_property(&ActionBarIconManager::EventIconDragStop);

	auto avatarIdType = Gui.new_usertype<ActionBarIconManager::AvatarIdType>("ActionBarIconManager::AvatarIdType",
																			 "new", [](Eris::ServerInfo serverInfo, std::string avatarId) { return ActionBarIconManager::AvatarIdType{std::move(serverInfo), avatarId}; });
	avatarIdType["serverInfo"] = &ActionBarIconManager::AvatarIdType::serverInfo;
	avatarIdType["avatarId"] = &ActionBarIconManager::AvatarIdType::avatarId;

	actionBarIconManager["AvatarIdType"] = avatarIdType;

	auto actionBarIconSlot = Gui.new_usertype<ActionBarIconSlot>("ActionBarIconSlot",
																 sol::base_classes, sol::bases<ActionBarIconDragDropTarget>());
	actionBarIconSlot["addActionBarIcon"] = &ActionBarIconSlot::addActionBarIcon;
	actionBarIconSlot["removeActionBarIcon"] = &ActionBarIconSlot::removeActionBarIcon;
	actionBarIconSlot["getActionBarIcon"] = &ActionBarIconSlot::getActionBarIcon;
	actionBarIconSlot["getWindow"] = &ActionBarIconSlot::getWindow;
	actionBarIconSlot["notifyIconDraggedOff"] = &ActionBarIconSlot::notifyIconDraggedOff;
	actionBarIconSlot["EventIconDraggedOff"] = LuaConnector::make_property(&ActionBarIconSlot::EventIconDraggedOff);

	auto texturePair = Gui.new_usertype<TexturePair>("TexturePair");
	texturePair["hasData"] = &TexturePair::hasData;
	texturePair["ogreTexture"] = sol::property([](TexturePair* self) { return std::shared_ptr(self->ogreTexture); });
	texturePair["textureImage"] = &TexturePair::textureImage;

	auto assetsManager = Gui.new_usertype<AssetsManager>("AssetsManager");
	assetsManager["showTexture"] = &AssetsManager::showTexture;
	assetsManager["createTextureImage"] = [](AssetsManager* self, const std::shared_ptr<Ogre::Texture>& texture, const char* imageSetName) {
		Ogre::SharedPtr<Ogre::Texture> ogreTexture(texture);
		return self->createTextureImage(ogreTexture, imageSetName);
	};
	//assetsManager["materialAsText"] = [](const std::shared_ptr <Ogre::Material>& material) { return AssetsManager::materialAsText({material}); };
	assetsManager["materialAsText"] = &AssetsManager::materialAsText;
	assetsManager["resolveResourceNameFromFilePath"] = &AssetsManager::resolveResourceNameFromFilePath;
	assetsManager["resolveFilePathForMesh"] = &AssetsManager::resolveFilePathForMesh;
	assetsManager["exportMesh"] = &AssetsManager::exportMesh;
	assetsManager["createModel"] = &AssetsManager::createModel;
	assetsManager["getLodInjectorSignaler"] = &AssetsManager::getLodInjectorSignaler;


	auto atlasHelper = Gui.new_usertype<AtlasHelper>("AtlasHelper");
	atlasHelper["serialize"] = &AtlasHelper::serialize;
	atlasHelper["deserialize"] = &AtlasHelper::deserialize;


	auto authoringManager = Authoring.new_usertype<AuthoringManager>("AuthoringManager");
	authoringManager["displayAuthoringVisualization"] = &AuthoringManager::displayAuthoringVisualization;
	authoringManager["hideAuthoringVisualization"] = &AuthoringManager::hideAuthoringVisualization;
	authoringManager["displaySimpleEntityVisualization"] = &AuthoringManager::displaySimpleEntityVisualization;
	authoringManager["hideSimpleEntityVisualization"] = &AuthoringManager::hideSimpleEntityVisualization;
	authoringManager["hasSimpleEntityVisualization"] = &AuthoringManager::hasSimpleEntityVisualization;
	authoringManager["startMovement"] = &AuthoringManager::startMovement;
	authoringManager["stopMovement"] = &AuthoringManager::stopMovement;

	auto avatar = OgreView.new_usertype<Avatar>("Avatar");
	avatar["getAvatarSceneNode"] = &Avatar::getAvatarSceneNode;
	avatar["getEmberEntity"] = &Avatar::getEmberEntity;
	avatar["getErisAvatar"] = &Avatar::getErisAvatar;
	avatar["deleteEntity"] = &Avatar::deleteEntity;
	avatar["setMinIntervalOfRotationChanges"] = &Avatar::setMinIntervalOfRotationChanges;
	avatar["isAdmin"] = &Avatar::isAdmin;
	avatar["getClientSideAvatarPosition"] = &Avatar::getClientSideAvatarPosition;
	avatar["getClientSideAvatarOrientation"] = &Avatar::getClientSideAvatarOrientation;
	avatar["useTool"] = sol::overload(
			[](Avatar* self,
			   const Ember::EmberEntity& tool,
			   const std::string& operation,
			   const Eris::Entity* target,
			   const WFMath::Point<3>& pos,
			   const WFMath::Vector<3>& direction) {
				self->useTool(tool, operation, target, pos, direction);
			},
			[](Avatar* self,
			   const Ember::EmberEntity& tool,
			   const std::string& operation,
			   const Eris::Entity* target,
			   const WFMath::Point<3>& pos) {
				self->useTool(tool, operation, target, pos);
			},
			[](Avatar* self,
			   const Ember::EmberEntity& tool,
			   const std::string& operation,
			   const Eris::Entity* target) {
				self->useTool(tool, operation, target);
			},
			[](Avatar* self,
			   const Ember::EmberEntity& tool,
			   const std::string& operation) {
				self->useTool(tool, operation);
			});
	avatar["taskUsage"] = sol::overload(sol::resolve<void(const std::string&, const std::string&)>(&Avatar::taskUsage),
										sol::resolve<void(std::string, const Eris::TaskUsage&)>(&Avatar::taskUsage));

	avatar["EventAddedEntityToInventory"] = LuaConnector::make_property(&Avatar::EventAddedEntityToInventory);
	avatar["EventRemovedEntityFromInventory"] = LuaConnector::make_property(&Avatar::EventRemovedEntityFromInventory);
	avatar["EventAvatarEntityDestroyed"] = LuaConnector::make_property(&Avatar::EventAvatarEntityDestroyed);

	auto colouredListItem = Gui.new_usertype<ColouredListItem>("ColouredListItem",
															   sol::base_classes, sol::bases<CEGUI::ListboxTextItem, CEGUI::ListboxItem>());
	colouredListItem["new"] = sol::factories(
			[](const char* text) { return ColouredListItem::createColouredListItem(text); },
			[](const char* text, unsigned int item_id) { return ColouredListItem::createColouredListItem(text, item_id); },
			[](const char* text, unsigned int item_id, void* item_data) { return ColouredListItem::createColouredListItem(text, item_id, item_data); });
	colouredListItem["tag"] = &ColouredListItem::tag;

	auto colouredTreeItem = Gui.new_usertype<ColouredTreeItem>("ColouredTreeItem",
															   sol::base_classes, sol::bases<CEGUI::TreeItem>());
	colouredTreeItem["new"] = sol::factories(
			[](const char* text) { return ColouredTreeItem::create(text); },
			[](const char* text, unsigned int item_id) { return ColouredTreeItem::create(text, item_id); },
			[](const char* text, unsigned int item_id, void* item_data) { return ColouredTreeItem::create(text, item_id, item_data); });
	colouredTreeItem["tag"] = &ColouredTreeItem::tag;

	auto compass = Gui.new_usertype<Compass>("Compass",
											 sol::constructors<Compass(Ember::OgreView::Gui::ICompassImpl*, Ogre::SceneManager&, ITerrainAdapter&)>()
	);
	compass["getMap"] = &Compass::getMap;
	compass["reposition"] = &Compass::reposition;
	compass["refresh"] = &Compass::refresh;

	auto renderedCompassImpl = Gui.new_usertype<RenderedCompassImpl>("RenderedCompassImpl",
																	 sol::constructors<RenderedCompassImpl(CEGUI::Window*)>(),
																	 sol::base_classes, sol::bases<ICompassImpl>());
	renderedCompassImpl["getTexture"] = [](RenderedCompassImpl* self) { return std::shared_ptr(self->getTexture()); };

	Gui.new_usertype<CompassCameraAnchor>("CompassCameraAnchor",
										  sol::constructors<CompassCameraAnchor(Ember::OgreView::Gui::Compass&, Ogre::Camera*)>());
	Gui.new_usertype<CompassSceneNodeAnchor>("CompassSceneNodeAnchor",
											 sol::constructors<CompassSceneNodeAnchor(Ember::OgreView::Gui::Compass&, Ogre::SceneNode*)>());
	Gui.new_usertype<CompassThirdPersonCameraAnchor>("CompassThirdPersonCameraAnchor",
													 sol::constructors<CompassThirdPersonCameraAnchor(Ember::OgreView::Gui::Compass&, Ogre::Camera*, Ogre::SceneNode*)>());
	auto consoleAdapter = Gui.new_usertype<ConsoleAdapter>("ConsoleAdapter",
														   sol::constructors<ConsoleAdapter(CEGUI::Editbox*)>());
	consoleAdapter["EventCommandExecuted"] = LuaConnector::make_property(&ConsoleAdapter::EventCommandExecuted);

	auto containerView = Gui.new_usertype<ContainerView>("ContainerView",
														 sol::constructors<ContainerView(Ember::OgreView::Gui::EntityIconManager&,
																						 Ember::OgreView::Gui::Icons::IconManager&,
																						 CEGUI::Tooltip&,
																						 CEGUI::Window&,
																						 int)>());
	containerView["showEntityContents"] = &ContainerView::showEntityContents;
	containerView["getEntityIcon"] = &ContainerView::getEntityIcon;
	containerView["addEntityIcon"] = &ContainerView::addEntityIcon;
	containerView["EventEntityPicked"] = LuaConnector::make_property(&ContainerView::EventEntityPicked);
	containerView["EventIconAdded"] = LuaConnector::make_property(&ContainerView::EventIconAdded);
	containerView["EventIconDropped"] = LuaConnector::make_property(&ContainerView::EventIconDropped);

	auto convert = lua.create_table();
	convert["toWF"] = [](const Ogre::Vector3& point) { return Convert::toWF<WFMath::Point<3 >>(point); };

	OgreView["Convert"] = convert;

	auto emberOgre = OgreView.new_usertype<EmberOgre>("EmberOgre");
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


	auto entityCEGUITexture = Gui.new_usertype<EntityCEGUITexture>("EntityCEGUITexture");
	entityCEGUITexture["getImage"] = &EntityCEGUITexture::getImage;
	entityCEGUITexture["getRenderContext"] = &EntityCEGUITexture::getRenderContext;


	auto entityCreatorTypeHelper = Gui.new_usertype<EntityCreatorTypeHelper>("EntityCreatorTypeHelper",
																			 sol::constructors<EntityCreatorTypeHelper(Eris::Avatar&,
																													   CEGUI::Tree&,
																													   CEGUI::Editbox&,
																													   CEGUI::PushButton&,
																													   CEGUI::Window&,
																													   CEGUI::Combobox&,
																													   CEGUI::Window&,
																													   CEGUI::ToggleButton&,
																													   CEGUI::Editbox&)>());
	entityCreatorTypeHelper["EventCreateFromType"] = LuaConnector::make_property(&EntityCreatorTypeHelper::EventCreateFromType);
	auto entityEditor = Gui.new_usertype<EntityEditor>("EntityEditor",
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
	entityEditor["getThoughts"] = &EntityEditor::getThoughts;
	entityEditor["getPath"] = &EntityEditor::getPath;
	entityEditor["getGoalInfo"] = &EntityEditor::getGoalInfo;
	entityEditor["parseElementMap"] = &EntityEditor::parseElementMap;
	entityEditor["EventGotThought"] = LuaConnector::make_property(&EntityEditor::EventGotThought);
	entityEditor["EventGotGoal"] = LuaConnector::make_property(&EntityEditor::EventGotGoal);
	entityEditor["EventGotEmptyGoals"] = LuaConnector::make_property(&EntityEditor::EventGotEmptyGoals);
	entityEditor["EventGotGoalInfo"] = LuaConnector::make_property(&EntityEditor::EventGotGoalInfo);


	auto entityIconDragDropTarget = Gui.new_usertype<EntityIconDragDropTarget>("EntityIconDragDropTarget",
																			   sol::constructors<EntityIconDragDropTarget(CEGUI::Window*)>());
	entityIconDragDropTarget["EventIconEntered"] = LuaConnector::make_property(&EntityIconDragDropTarget::EventIconEntered);
	entityIconDragDropTarget["EventIconLeaves"] = LuaConnector::make_property(&EntityIconDragDropTarget::EventIconLeaves);
	entityIconDragDropTarget["EventIconDropped"] = LuaConnector::make_property(&EntityIconDragDropTarget::EventIconDropped);
	//TODO: check if we still need to == operator with sol
	auto entityIcon = Gui.new_usertype<EntityIcon>("EntityIcon",
												   sol::base_classes, sol::bases<EntityIconDragDropTarget>());
	entityIcon["getImage"] = &EntityIcon::getImage;
	entityIcon["getDragContainer"] = &EntityIcon::getDragContainer;
	entityIcon["getIcon"] = &EntityIcon::getIcon;
	entityIcon["setSlot"] = &EntityIcon::setSlot;
	entityIcon["getSlot"] = &EntityIcon::getSlot;
	entityIcon["setTooltipText"] = &EntityIcon::setTooltipText;
	entityIcon["getEntity"] = &EntityIcon::getEntity;
	entityIcon["setTag"] = &EntityIcon::setTag;
	entityIcon["getTag"] = &EntityIcon::getTag;

	auto entityIconDragDropPreview = Gui.new_usertype<EntityIconDragDropPreview>("EntityIconDragDropPreview",
																				 sol::constructors<EntityIconDragDropPreview(Ember::OgreView::World&)>());
	entityIconDragDropPreview["createPreview"] = &EntityIconDragDropPreview::createPreview;
	entityIconDragDropPreview["cleanupCreation"] = &EntityIconDragDropPreview::cleanupCreation;
	entityIconDragDropPreview["getDropPosition"] = &EntityIconDragDropPreview::getDropPosition;
	entityIconDragDropPreview["getDropOrientation"] = &EntityIconDragDropPreview::getDropOrientation;
	entityIconDragDropPreview["EventEntityFinalized"] = LuaConnector::make_property(&EntityIconDragDropPreview::EventEntityFinalized);

	auto entityIconManager = Gui.new_usertype<EntityIconManager>("EntityIconManager");
	entityIconManager["createSlot"] = sol::overload(&EntityIconManager::createSlot,
													[](EntityIconManager* self) { return self->createSlot(); });
	entityIconManager["destroySlot"] = &EntityIconManager::destroySlot;
	entityIconManager["createIcon"] = sol::overload(&EntityIconManager::createIcon,
													[](EntityIconManager* self, Gui::Icons::Icon* icon, EmberEntity* entity) { return self->createIcon(icon, entity); });
	entityIconManager["destroyIcon"] = &EntityIconManager::destroyIcon;
	entityIconManager["EventIconDragStart"] = LuaConnector::make_property(&EntityIconManager::EventIconDragStart);
	entityIconManager["EventIconDragStop"] = LuaConnector::make_property(&EntityIconManager::EventIconDragStop);

	auto entityIconSlot = Gui.new_usertype<EntityIconSlot>("EntityIconSlot",
														   sol::base_classes, sol::bases<EntityIconDragDropTarget>());
	entityIconSlot["addEntityIcon"] = &EntityIconSlot::addEntityIcon;
	entityIconSlot["removeEntityIcon"] = &EntityIconSlot::removeEntityIcon;
	entityIconSlot["getEntityIcon"] = &EntityIconSlot::getEntityIcon;
	entityIconSlot["getWindow"] = &EntityIconSlot::getWindow;
	entityIconSlot["notifyIconDraggedOff"] = &EntityIconSlot::notifyIconDraggedOff;
	entityIconSlot["EventIconDraggedOff"] = LuaConnector::make_property(&EntityIconSlot::EventIconDraggedOff);

	auto entityMoveManager = Authoring.new_usertype<EntityMoveManager>("EntityMoveManager");
	entityMoveManager["startMove"] = &EntityMoveManager::startMove;
	entityMoveManager["EventStartMoving"] = LuaConnector::make_property(&EntityMoveManager::EventStartMoving);
	entityMoveManager["EventFinishedMoving"] = LuaConnector::make_property(&EntityMoveManager::EventFinishedMoving);
	entityMoveManager["EventCancelledMoving"] = LuaConnector::make_property(&EntityMoveManager::EventCancelledMoving);

	auto entityRecipe = Authoring.new_usertype<EntityRecipe>("EntityRecipe");
	entityRecipe["setAuthor"] = &EntityRecipe::setAuthor;
	entityRecipe["getAuthor"] = &EntityRecipe::getAuthor;
	entityRecipe["setDescription"] = &EntityRecipe::setDescription;
	entityRecipe["getDescription"] = &EntityRecipe::getDescription;

	auto entityTextureManipulator = Gui.new_usertype<EntityTextureManipulator>("EntityRecipe");
	entityTextureManipulator["EventMovementStarted"] = LuaConnector::make_property(&EntityTextureManipulator::EventMovementStarted);
	entityTextureManipulator["EventMovementStopped"] = LuaConnector::make_property(&EntityTextureManipulator::EventMovementStopped);

	Gui.new_usertype<DirectEntityTextureManipulator>("DirectEntityTextureManipulator",
													 sol::constructors<DirectEntityTextureManipulator(CEGUI::Window&, Ember::OgreView::Gui::EntityCEGUITexture&)>(),
													 sol::base_classes, sol::bases<EntityTextureManipulator>()
	);
	Gui.new_usertype<CameraEntityTextureManipulator>("CameraEntityTextureManipulator",
													 sol::constructors<CameraEntityTextureManipulator(CEGUI::Window&, Ember::OgreView::Gui::EntityCEGUITexture&)>(),
													 sol::base_classes, sol::bases<EntityTextureManipulator>()
	);
	auto combinedEntityTextureManipulator = Gui.new_usertype<CombinedEntityTextureManipulator>("CombinedEntityTextureManipulator",
																							   sol::constructors<CombinedEntityTextureManipulator(CEGUI::Window&, Ember::OgreView::Gui::EntityCEGUITexture&)>(),
																							   sol::base_classes, sol::bases<EntityTextureManipulator>()
	);
	combinedEntityTextureManipulator["EventEditingStarted"] = LuaConnector::make_property(&CombinedEntityTextureManipulator::EventEditingStarted);
	combinedEntityTextureManipulator["EventEditingStopped"] = LuaConnector::make_property(&CombinedEntityTextureManipulator::EventEditingStopped);

	auto entityTooltip = Gui.new_usertype<EntityTooltip>("EntityTooltip");
	entityTooltip["getTooltipWindow"] = &EntityTooltip::getTooltipWindow;

	auto entityPickResult = OgreView.new_usertype<EntityPickResult>("EntityPickResult");
	entityPickResult["entity"] = &EntityPickResult::entity;
	entityPickResult["position"] = &EntityPickResult::position;
	entityPickResult["distance"] = &EntityPickResult::distance;

	auto entityWorldPickListener = OgreView.new_usertype<EntityWorldPickListener>("EntityWorldPickListener");
	entityWorldPickListener["EventPickedEntity"] = LuaConnector::make_property(&EntityWorldPickListener::EventPickedEntity);


	auto iSun = Environment.new_usertype<ISun>("ISun");
	Environment["setAmbientLight"] = &ISun::setAmbientLight;


	auto iFog = Environment.new_usertype<IFog>("IFog");
	iFog["setDensity"] = &IFog::setDensity;
	iFog["getDensity"] = &IFog::getDensity;

	auto environment = Environment.new_usertype<Ember::OgreView::Environment::Environment>("Environment");
	environment["getSun"] = &Ember::OgreView::Environment::Environment::getSun;
	environment["getFog"] = &Ember::OgreView::Environment::Environment::getFog;
	environment["setTime"] = [](Ember::OgreView::Environment::Environment* self, int seconds) { self->setTime(seconds); };
	environment["setTimeMultiplier"] = &Ember::OgreView::Environment::Environment::setTimeMultiplier;
	environment["getTimeMultiplier"] = &Ember::OgreView::Environment::Environment::getTimeMultiplier;
	environment["setAmbientLight"] = &Ember::OgreView::Environment::Environment::setAmbientLight;

	auto guiManager = OgreView.new_usertype<GUIManager>("GUIManager");
	guiManager["getSingleton"] = &GUIManager::getSingleton;
	guiManager["appendAvatarImaginary"] = [](GUIManager* self, const std::string& message) { self->AppendAvatarImaginary.emit(message); };
	guiManager["removeWidget"] = &GUIManager::removeWidget;
	guiManager["addWidget"] = &GUIManager::addWidget;
	guiManager["EmitEntityAction"] = &GUIManager::EmitEntityAction;
	guiManager["getMainSheet"] = &GUIManager::getMainSheet;
	guiManager["isInGUIMode"] = &GUIManager::isInGUIMode;
	guiManager["isInMovementKeysMode"] = &GUIManager::isInMovementKeysMode;
	guiManager["getLayoutDir"] = &GUIManager::getLayoutDir;
	guiManager["createWindow"] = [](GUIManager* self, const std::string& windowType) { return self->createWindow(windowType); };
	guiManager["createWidget"] = &GUIManager::createWidget;
	guiManager["destroyWidget"] = &GUIManager::destroyWidget;
	guiManager["getDefaultScheme"] = &GUIManager::getDefaultScheme;
	guiManager["getIconManager"] = &GUIManager::getIconManager;
	guiManager["getEntityIconManager"] = &GUIManager::getEntityIconManager;
	guiManager["getActionBarIconManager"] = &GUIManager::getActionBarIconManager;
	guiManager["getEntityTooltip"] = &GUIManager::getEntityTooltip;
	guiManager["AppendIGChatLine"] = LuaConnector::make_property(&GUIManager::AppendIGChatLine);
	guiManager["AppendOOGChatLine"] = LuaConnector::make_property(&GUIManager::AppendOOGChatLine);
	guiManager["AppendAvatarImaginary"] = LuaConnector::make_property(&GUIManager::AppendAvatarImaginary);
	guiManager["EventEntityAction"] = LuaConnector::make_property(&GUIManager::EventEntityAction);
	guiManager["EventFrameStarted"] = LuaConnector::make_property(&GUIManager::EventFrameStarted);

	auto icon = Icons.new_usertype<Ember::OgreView::Gui::Icons::Icon>("Icon");
	icon["getImage"] = &Icon::getImage;

	auto iconBar = Gui.new_usertype<IconBar>("IconBar",
											 sol::constructors<IconBar(
													 const std::string&)>());
	iconBar["addIcon"] = &IconBar::addIcon;
	iconBar["removeIcon"] = &IconBar::removeIcon;
	iconBar["getWindow"] = &IconBar::getWindow;
	iconBar["setIconPadding"] = &IconBar::setIconPadding;
	iconBar["getAbsoluteHeight"] = &IconBar::getAbsoluteHeight;
	iconBar["getAbsoluteWidth"] = &IconBar::getAbsoluteWidth;

	auto iconBase = Gui.new_usertype<IconBase>("IconBase",
											   sol::constructors<IconBase(const std::string&, const CEGUI::Image*, const CEGUI::Image*, const CEGUI::Image*, const CEGUI::Image*),
													   IconBase(const std::string&, const CEGUI::Image*, const CEGUI::Image*, const CEGUI::Image*, const CEGUI::Image*, CEGUI::USize)>());
	iconBase["getContainer"] = &IconBase::getContainer;
	iconBase["getButton"] = &IconBase::getButton;
	iconBase["setForeground"] = &IconBase::setForeground;
	iconBase["loadImageFromImageset"] = &IconBase::loadImageFromImageset;

	auto iconManager = Icons.new_usertype<IconManager>("IconManager");
	iconManager["getIcon"] = sol::overload([](IconManager* self, int pixelWidth, Ember::EmberEntity* entity) { return self->getIcon(pixelWidth, entity); },
										   [](IconManager* self, int pixelWidth, Eris::TypeInfo* erisType) { return self->getIcon(pixelWidth, erisType); });
	iconManager["render"] = sol::overload([](IconManager* self, Icon& icon, const std::string& modelName) { self->render(icon, modelName); },
										  [](IconManager* self, Icon& icon, Ember::EmberEntity& entity) { self->render(icon, entity); },
										  [](IconManager* self, Icon& icon, Eris::TypeInfo& erisType) { self->render(icon, erisType); });

	OgreView["MPT_CLICK"] = MousePickType::MPT_CLICK;
	OgreView["MPT_DOUBLECLICK"] = MousePickType::MPT_DOUBLECLICK;
	OgreView["MPT_PRESSED"] = MousePickType::MPT_PRESSED;
	OgreView["MPT_HOVER"] = MousePickType::MPT_HOVER;
	OgreView["MPT_PRESS"] = MousePickType::MPT_PRESS;

	auto mousePickerArgs = OgreView.new_usertype<MousePickerArgs>("MousePickerArgs");
	mousePickerArgs["windowX"] = &MousePickerArgs::windowX;
	mousePickerArgs["windowY"] = &MousePickerArgs::windowY;
	mousePickerArgs["pickType"] = &MousePickerArgs::pickType;

	auto listHolder = Gui.new_usertype<ListHolder>("ListHolder",
												   sol::constructors<ListHolder(CEGUI::Listbox&, CEGUI::Editbox*)>());
	listHolder["addItem"] = [](ListHolder* self, CEGUI::ListboxItem* item) { self->addItem(item); };
	listHolder["insertItem"] = &ListHolder::insertItem;
	listHolder["removeItem"] = &ListHolder::removeItem;
	listHolder["resetList"] = &ListHolder::resetList;

	auto lodDistance = Lod.new_usertype<LodDistance>("LodDistance");
	lodDistance["meshName"] = &LodDistance::meshName;
	lodDistance["reductionMethod"] = &LodDistance::reductionMethod;
	lodDistance["reductionValue"] = &LodDistance::reductionValue;

	auto lodDefinition = Lod.new_usertype<LodDefinition>("LodDefinition",
														 sol::base_classes, sol::bases<Ogre::Resource>()
	);
	lodDefinition["getUseAutomaticLod"] = &LodDefinition::getUseAutomaticLod;
	lodDefinition["getUseAutomaticLod"] = &LodDefinition::getUseAutomaticLod;
	lodDefinition["setUseAutomaticLod"] = &LodDefinition::setUseAutomaticLod;
	lodDefinition["getType"] = &LodDefinition::getType;
	lodDefinition["setType"] = &LodDefinition::setType;
	lodDefinition["getStrategy"] = &LodDefinition::getStrategy;
	lodDefinition["setStrategy"] = &LodDefinition::setStrategy;
	lodDefinition["addLodDistance"] = &LodDefinition::addLodDistance;
	lodDefinition["hasLodDistance"] = &LodDefinition::hasLodDistance;
	lodDefinition["getLodDistance"] = &LodDefinition::getLodDistance;
	lodDefinition["getLodDistanceCount"] = &LodDefinition::getLodDistanceCount;
	lodDefinition["createListOfDistances"] = &LodDefinition::createListOfDistances;
	lodDefinition["createDistance"] = &LodDefinition::createDistance;
	lodDefinition["removeLodDistance"] = &LodDefinition::removeLodDistance;


	auto lodType = lua.create_table();
	lodType["LT_AUTOMATIC_VERTEX_REDUCTION"] = Ember::OgreView::Lod::LodDefinition::LT_AUTOMATIC_VERTEX_REDUCTION;
	lodType["LT_USER_CREATED_MESH"] = Ember::OgreView::Lod::LodDefinition::LT_USER_CREATED_MESH;
	lodDefinition["LodType"] = lodType;

	auto lodStrategy = lua.create_table();
	lodStrategy["LS_DISTANCE"] = Ember::OgreView::Lod::LodDefinition::LS_DISTANCE;
	lodStrategy["LS_PIXEL_COUNT"] = Ember::OgreView::Lod::LodDefinition::LS_PIXEL_COUNT;
	lodDefinition["LodStrategy"] = lodStrategy;

	auto lodDefinitionManager = Lod.new_usertype<LodDefinitionManager>("LodDefinitionManager",
																	   sol::base_classes, sol::bases<Ogre::ResourceManager>()
	);
	lodDefinitionManager["getSingleton"] = &LodDefinitionManager::getSingleton;
	lodDefinitionManager["exportScript"] = &LodDefinitionManager::exportScript;
	lodDefinitionManager["create"] = [](LodDefinitionManager* self, const Ogre::String& name, const Ogre::String& group) { return std::shared_ptr(self->create(name, group)); };
	lodDefinitionManager["getByName"] = [](LodDefinitionManager* self, const Ogre::String& name) { return std::shared_ptr(self->getByName(name)); };


	auto lodManager = Lod.new_usertype<LodManager>("LodManager");
	lodManager["getSingleton"] = &LodManager::getSingleton;
	lodManager["loadLod"] = [](LodManager* self, Ogre::MeshPtr mesh, const LodDefinition& def) { self->loadLod(mesh, def); };
//	lodManager["loadLod"] = sol::resolve(&LodManager::loadLod;

	auto mainCamera = Camera.new_usertype<MainCamera>("MainCamera");
	mainCamera["getCamera"] = &MainCamera::getCamera;
	mainCamera["getOrientation"] = &MainCamera::getOrientation;
	mainCamera["getPosition"] = &MainCamera::getPosition;
	mainCamera["setClosestPickingDistance"] = &MainCamera::setClosestPickingDistance;
	mainCamera["getClosestPickingDistance"] = &MainCamera::getClosestPickingDistance;
	mainCamera["enableCompositor"] = &MainCamera::enableCompositor;
	mainCamera["MovedCamera"] = LuaConnector::make_property(&MainCamera::MovedCamera);

	auto map = Terrain.new_usertype<Map>("Map",
										 sol::constructors<Map(Ogre::SceneManager&)>());
	map["initialize"] = &Map::initialize;
	map["getTexture"] = [](Map* self) { std::shared_ptr(self->getTexture()); };
	map["render"] = &Map::render;
	map["reposition"] = [](Map* self, float x, float y) { self->reposition(x, y); };
	map["setDistance"] = &Map::setDistance;
	map["getDistance"] = &Map::getDistance;
	map["getResolution"] = &Map::getResolution;
	map["setResolution"] = &Map::setResolution;
	map["getResolutionMeters"] = &Map::getResolutionMeters;

	auto meshInfoProvider = Gui.new_usertype<MeshInfoProvider>("MeshInfoProvider",
															   sol::constructors<MeshInfoProvider(OgreEntityRenderer*)>());
	meshInfoProvider["getInfo"] = &MeshInfoProvider::getInfo;
	meshInfoProvider["getPreviewInfo"] = &MeshInfoProvider::getPreviewInfo;
	meshInfoProvider["postqueueEntityMeshLodChanged"] = &MeshInfoProvider::postqueueEntityMeshLodChanged;
	meshInfoProvider["getLodIndex"] = &MeshInfoProvider::getLodIndex;
	meshInfoProvider["calcUniqueVertexCount"] = [](const Ogre::Mesh* mesh) { return MeshInfoProvider::calcUniqueVertexCount(mesh); };
	meshInfoProvider["EventLodChanged"] = LuaConnector::make_property(&MeshInfoProvider::EventLodChanged);

	auto animationSet = Model.new_usertype<AnimationSet>("AnimationSet");
	animationSet["addTime"] = [](AnimationSet* self, float timeSlice) { self->addTime(timeSlice); };
	animationSet["addAnimation"] = &AnimationSet::addAnimation;
	animationSet["reset"] = &AnimationSet::reset;
	animationSet["setSpeed"] = &AnimationSet::setSpeed;
	animationSet["getSpeed"] = &AnimationSet::getSpeed;

	auto action = Model.new_usertype<Action>("Action");
	action["name"] = &Action::name;
	action["animations"] = &Action::animations;

	auto model = Model.new_usertype<Ember::OgreView::Model::Model>("Model");
	model["reload"] = &Ember::OgreView::Model::Model::reload;
	model["removeSubmodel"] = &Ember::OgreView::Model::Model::removeSubmodel;
	model["getAction"] = sol::resolve<Action*(const std::string&)>(&Ember::OgreView::Model::Model::getAction);
	model["showPart"] = &Ember::OgreView::Model::Model::showPart;
	model["hidePart"] = &Ember::OgreView::Model::Model::hidePart;
	model["setVisible"] = &Ember::OgreView::Model::Model::setVisible;
	model["getRotation"] = &Ember::OgreView::Model::Model::getRotation;
	model["getName"] = &Ember::OgreView::Model::Model::getName;
	model["getDefinition"] = &Ember::OgreView::Model::Model::getDefinition;
	model["hasAttachPoint"] = &Ember::OgreView::Model::Model::hasAttachPoint;
	model["hasParticles"] = &Ember::OgreView::Model::Model::hasParticles;
	model["getSubModel"] = &Ember::OgreView::Model::Model::getSubModel;
	model["setDisplaySkeleton"] = &Ember::OgreView::Model::Model::setDisplaySkeleton;
	model["getDisplaySkeleton"] = &Ember::OgreView::Model::Model::getDisplaySkeleton;

	auto viewDefinition = Model.new_usertype<ViewDefinition>("ViewDefinition");
	viewDefinition["Name"] = &ViewDefinition::Name;
	viewDefinition["Rotation"] = &ViewDefinition::Rotation;
	viewDefinition["Distance"] = &ViewDefinition::Distance;

	auto poseDefinition = Model.new_usertype<PoseDefinition>("PoseDefinition");
	poseDefinition["Rotate"] = &PoseDefinition::Rotate;
	poseDefinition["Translate"] = &PoseDefinition::Translate;

	auto attachPointDefinition = Model.new_usertype<AttachPointDefinition>("AttachPointDefinition");
	attachPointDefinition["Name"] = &AttachPointDefinition::Name;
	attachPointDefinition["BoneName"] = &AttachPointDefinition::BoneName;
	attachPointDefinition["Pose"] = &AttachPointDefinition::Pose;
	attachPointDefinition["Rotation"] = &AttachPointDefinition::Rotation;
	attachPointDefinition["Translation"] = &AttachPointDefinition::Translation;

	auto subEntityDefinition = Model.new_usertype<SubEntityDefinition>("SubEntityDefinition");
	subEntityDefinition["subEntityName"] = &SubEntityDefinition::subEntityName;
	subEntityDefinition["subEntityIndex"] = &SubEntityDefinition::subEntityIndex;
	subEntityDefinition["materialName"] = &SubEntityDefinition::materialName;

	auto partDefinition = Model.new_usertype<PartDefinition>("PartDefinition");
	partDefinition["name"] = &PartDefinition::name;
	partDefinition["show"] = &PartDefinition::show;
	partDefinition["getSubEntityDefinitions"] = &PartDefinition::getSubEntityDefinitions;
	partDefinition["removeSubEntityDefinition"] = &PartDefinition::removeSubEntityDefinition;

	auto subModelDefinition = Model.new_usertype<SubModelDefinition>("SubModelDefinition");
	subModelDefinition["meshName"] = &SubModelDefinition::meshName;
	subModelDefinition["shadowCaster"] = &SubModelDefinition::shadowCaster;
	subModelDefinition["getPartDefinitions"] = &SubModelDefinition::getPartDefinitions;
	subModelDefinition["removePartDefinition"] = &SubModelDefinition::removePartDefinition;

	auto animationPartDefinition = Model.new_usertype<AnimationPartDefinition>("AnimationPartDefinition");
	animationPartDefinition["Name"] = &AnimationPartDefinition::Name;
	animationPartDefinition["BoneGroupRefs"] = &AnimationPartDefinition::BoneGroupRefs;

	auto boneGroupDefinition = Model.new_usertype<BoneGroupDefinition>("BoneGroupDefinition");
	boneGroupDefinition["Name"] = &BoneGroupDefinition::Name;
	boneGroupDefinition["Bones"] = &BoneGroupDefinition::Bones;

	auto soundDefinition = Model.new_usertype<SoundDefinition>("SoundDefinition");
	soundDefinition["groupName"] = &SoundDefinition::groupName;
	soundDefinition["playOrder"] = &SoundDefinition::playOrder;

	auto boneGroupRefDefinition = Model.new_usertype<BoneGroupRefDefinition>("BoneGroupRefDefinition");
	boneGroupRefDefinition["Name"] = &BoneGroupRefDefinition::Name;
	boneGroupRefDefinition["Weight"] = &BoneGroupRefDefinition::Weight;

	auto animationDefinition = Model.new_usertype<AnimationDefinition>("AnimationDefinition");
	animationDefinition["getAnimationPartDefinitions"] = sol::resolve<const std::vector<AnimationPartDefinition>&() const>(&AnimationDefinition::getAnimationPartDefinitions);
	animationDefinition["removeAnimationPartDefinition"] = &AnimationDefinition::removeAnimationPartDefinition;
	animationDefinition["iterations"] = &AnimationDefinition::iterations;

	auto actionDefinition = Model.new_usertype<ActionDefinition>("ActionDefinition");
	actionDefinition["getAnimationDefinitions"] = sol::resolve<const std::vector<AnimationDefinition>&() const>(&ActionDefinition::getAnimationDefinitions);
	actionDefinition["removeAnimationDefinition"] = &ActionDefinition::removeAnimationDefinition;
	actionDefinition["getSoundDefinitions"] = sol::resolve<const std::vector<SoundDefinition>&() const>(&ActionDefinition::getSoundDefinitions);
	actionDefinition["removeSoundDefinition"] = &ActionDefinition::removeSoundDefinition;
	actionDefinition["getActivationDefinitions"] = sol::resolve<const std::vector<ActivationDefinition>&() const>(&ActionDefinition::getActivationDefinitions);
	actionDefinition["name"] = &ActionDefinition::name;
	actionDefinition["animationSpeed"] = &ActionDefinition::animationSpeed;

	auto modelDefinition = Model.new_usertype<ModelDefinition>("ModelDefinition");
	modelDefinition["getOrigin"] = &ModelDefinition::getOrigin;
	modelDefinition["setOrigin"] = &ModelDefinition::setOrigin;
	modelDefinition["isValid"] = &ModelDefinition::isValid;
	modelDefinition["setValid"] = &ModelDefinition::setValid;
	modelDefinition["getScale"] = &ModelDefinition::getScale;
	modelDefinition["setScale"] = &ModelDefinition::setScale;
	modelDefinition["getUseScaleOf"] = &ModelDefinition::getUseScaleOf;
	modelDefinition["setUseScaleOf"] = &ModelDefinition::setUseScaleOf;
	modelDefinition["getTranslate"] = &ModelDefinition::getTranslate;
	modelDefinition["setTranslate"] = &ModelDefinition::setTranslate;
	modelDefinition["getContentOffset"] = &ModelDefinition::getContentOffset;
	modelDefinition["setContentOffset"] = &ModelDefinition::setContentOffset;
	modelDefinition["getRotation"] = &ModelDefinition::getRotation;
	modelDefinition["setRotation"] = &ModelDefinition::setRotation;
	modelDefinition["getSubModelDefinitions"] = &ModelDefinition::getSubModelDefinitions;
	modelDefinition["removeSubModelDefinition"] = &ModelDefinition::removeSubModelDefinition;
	modelDefinition["getActionDefinitions"] = &ModelDefinition::getActionDefinitions;
	modelDefinition["removeActionDefinition"] = &ModelDefinition::removeActionDefinition;
	modelDefinition["getAttachPointsDefinitions"] = &ModelDefinition::getAttachPointsDefinitions;
	modelDefinition["addAttachPointDefinition"] = &ModelDefinition::addAttachPointDefinition;
	modelDefinition["getViewDefinitions"] = &ModelDefinition::getViewDefinitions;
	modelDefinition["removeViewDefinition"] = &ModelDefinition::removeViewDefinition;
	modelDefinition["getPoseDefinitions"] = &ModelDefinition::getPoseDefinitions;
	modelDefinition["addPoseDefinition"] = &ModelDefinition::addPoseDefinition;
	modelDefinition["removePoseDefinition"] = &ModelDefinition::removePoseDefinition;
	modelDefinition["reloadAllInstances"] = &ModelDefinition::reloadAllInstances;


	auto modelDefinitionAtlasComposer = Model.new_usertype<ModelDefinitionAtlasComposer>("ModelDefinitionAtlasComposer");
	modelDefinitionAtlasComposer["compose"] = &ModelDefinitionAtlasComposer::compose;
	modelDefinitionAtlasComposer["composeToStream"] = &ModelDefinitionAtlasComposer::composeToStream;
	modelDefinitionAtlasComposer["composeToFile"] = &ModelDefinitionAtlasComposer::composeToFile;

	auto modelDefinitionManager = Model.new_usertype<ModelDefinitionManager>("ModelDefinitionManager");
	modelDefinitionManager["getSingleton"] = &ModelDefinitionManager::getSingleton;
	modelDefinitionManager["exportScript"] = &ModelDefinitionManager::exportScript;
	modelDefinitionManager["addDefinition"] = &ModelDefinitionManager::addDefinition;
	modelDefinitionManager["getByName"] = &ModelDefinitionManager::getByName;
	modelDefinitionManager["getAllMeshes"] = &ModelDefinitionManager::getAllMeshes;
	modelDefinitionManager["getShowModels"] = &ModelDefinitionManager::getShowModels;
	modelDefinitionManager["setShowModels"] = &ModelDefinitionManager::setShowModels;

	auto modelEditHelper = Gui.new_usertype<ModelEditHelper>("ModelEditHelper",
															 sol::constructors<ModelEditHelper(Ember::OgreView::Model::Model*, SimpleRenderContext&)>());
	modelEditHelper["showAttachPointHelperEntity"] = sol::overload([](ModelEditHelper* self, const std::string& attachPointName, const std::string& meshName) { return self->showAttachPointHelperEntity(attachPointName, meshName); },
																   [](ModelEditHelper* self, const std::string& attachPointName) { return self->showAttachPointHelperEntity(attachPointName); });
	modelEditHelper["showAttachPointHelperModel"] = &ModelEditHelper::showAttachPointHelperModel;
	modelEditHelper["hideAttachPointHelper"] = &ModelEditHelper::hideAttachPointHelper;
	modelEditHelper["startInputRotate"] = &ModelEditHelper::startInputRotate;
	modelEditHelper["startInputTranslate"] = &ModelEditHelper::startInputTranslate;

	auto modelRenderer = Gui.new_usertype<ModelRenderer>("ModelRenderer",
														 sol::constructors<ModelRenderer(CEGUI::Window* image, const std::string&)>(),
														 sol::base_classes, sol::bases<MovableObjectRenderer>()
	);
	modelRenderer["showModel"] = sol::overload(sol::resolve<const std::string&, const Ogre::Vector3&, const Ogre::Quaternion&>(&ModelRenderer::showModel),
											   sol::resolve<const ModelDefinitionPtr&, const Ogre::Vector3&, const Ogre::Quaternion&>(&ModelRenderer::showModel),
											   [](ModelRenderer* self, const char* model) { self->showModel(model); },
											   [](ModelRenderer* self, const ModelDefinitionPtr& model) { self->showModel(model); });
	modelRenderer["getModel"] = &ModelRenderer::getModel;
	modelRenderer["repositionSceneNode"] = &ModelRenderer::repositionSceneNode;

	auto modelRepresentation = Model.new_usertype<ModelRepresentation>("ModelRepresentation");
	modelRepresentation["getTypeNameForClass"] = &ModelRepresentation::getTypeNameForClass;
	modelRepresentation["getEntity"] = &ModelRepresentation::getEntity;
	modelRepresentation["getModel"] = &ModelRepresentation::getModel;
	modelRepresentation["getModelForEntity"] = &ModelRepresentation::getModelForEntity;
	modelRepresentation["getRepresentationForEntity"] = &ModelRepresentation::getRepresentationForEntity;

	auto motionManager = OgreView.new_usertype<MotionManager>("MotionManager");
	motionManager["getInfo"] = &MotionManager::getInfo;

	auto motionManagerInfo = OgreView.new_usertype<MotionManager::MotionManagerInfo>("MotionManager::MotionManagerInfo");
	motionManagerInfo["AnimatedEntities"] = &MotionManager::MotionManagerInfo::AnimatedEntities;
	motionManagerInfo["MovingEntities"] = &MotionManager::MotionManagerInfo::MovingEntities;
	motionManager["MotionManagerInfo"] = motionManagerInfo;

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
	auto movableObjectRenderer = Gui.new_usertype<MovableObjectRenderer>("MovableObjectRenderer");
	movableObjectRenderer["showFull"] = &MovableObjectRenderer::showFull;
	movableObjectRenderer["setCameraDistance"] = &MovableObjectRenderer::setCameraDistance;
	movableObjectRenderer["getCameraDistance"] = &MovableObjectRenderer::getCameraDistance;
	movableObjectRenderer["getAbsoluteCameraDistance"] = &MovableObjectRenderer::getAbsoluteCameraDistance;
	movableObjectRenderer["getCameraOrientation"] = &MovableObjectRenderer::getCameraOrientation;
	movableObjectRenderer["setAutoShowFull"] = &MovableObjectRenderer::setAutoShowFull;
	movableObjectRenderer["getAutoShowFull"] = &MovableObjectRenderer::getAutoShowFull;
	movableObjectRenderer["getActive"] = &MovableObjectRenderer::getActive;
	movableObjectRenderer["setActive"] = &MovableObjectRenderer::setActive;
	movableObjectRenderer["getEntityRotation"] = &MovableObjectRenderer::getEntityRotation;
	movableObjectRenderer["setEntityRotation"] = &MovableObjectRenderer::setEntityRotation;
	movableObjectRenderer["getEntityTranslation"] = &MovableObjectRenderer::getEntityTranslation;
	movableObjectRenderer["setEntityTranslation"] = &MovableObjectRenderer::setEntityTranslation;
	movableObjectRenderer["resetCameraOrientation"] = &MovableObjectRenderer::resetCameraOrientation;
	movableObjectRenderer["pitchCamera"] = &MovableObjectRenderer::pitchCamera;
	movableObjectRenderer["yawCamera"] = &MovableObjectRenderer::yawCamera;
	movableObjectRenderer["rollCamera"] = &MovableObjectRenderer::rollCamera;
	movableObjectRenderer["updateRender"] = &MovableObjectRenderer::updateRender;
	movableObjectRenderer["showAxis"] = &MovableObjectRenderer::showAxis;
	movableObjectRenderer["hideAxis"] = &MovableObjectRenderer::hideAxis;
	movableObjectRenderer["getCameraPositionMode"] = &MovableObjectRenderer::getCameraPositionMode;
	movableObjectRenderer["setCameraPositionMode"] = &MovableObjectRenderer::setCameraPositionMode;
	movableObjectRenderer["getEntityTexture"] = &MovableObjectRenderer::getEntityTexture;
	movableObjectRenderer["setShowMovingLight"] = &MovableObjectRenderer::setShowMovingLight;

	auto movementControllerMode = OgreView.new_usertype<MovementControllerMode>("MovementControllerMode");
	movementControllerMode["MM_WALK"] = sol::var(MovementControllerMode::MM_WALK);
	movementControllerMode["MM_RUN"] = sol::var(MovementControllerMode::MM_RUN);

	auto movementController = OgreView.new_usertype<MovementController>("MovementController");
	movementController["moveToPoint"] = &MovementController::moveToPoint;
	movementController["teleportTo"] = &MovementController::teleportTo;
	movementController["getMode"] = &MovementController::getMode;
	movementController["EventMovementModeChanged"] = LuaConnector::make_property(&MovementController::EventMovementModeChanged);

	auto ogreEntityRenderer = Gui.new_usertype<OgreEntityRenderer>("OgreEntityRenderer",
																   sol::constructors<OgreEntityRenderer(CEGUI::Window*)>(),
																   sol::base_classes, sol::bases<MovableObjectRenderer>()
	);
	ogreEntityRenderer["showEntity"] = &OgreEntityRenderer::showEntity;
	ogreEntityRenderer["unloadEntity"] = &OgreEntityRenderer::unloadEntity;
	ogreEntityRenderer["getEntity"] = &OgreEntityRenderer::getEntity;
	ogreEntityRenderer["setAutoShowFull"] = &OgreEntityRenderer::setAutoShowFull;
	ogreEntityRenderer["getWireframeMode"] = &OgreEntityRenderer::getWireframeMode;
	ogreEntityRenderer["setWireframeMode"] = &OgreEntityRenderer::setWireframeMode;
	ogreEntityRenderer["setForcedLodLevel"] = &OgreEntityRenderer::setForcedLodLevel;
	ogreEntityRenderer["clearForcedLodLevel"] = &OgreEntityRenderer::clearForcedLodLevel;
	ogreEntityRenderer["enableAnimation"] = &OgreEntityRenderer::enableAnimation;
	ogreEntityRenderer["getEntityAnimationNames"] = &OgreEntityRenderer::getEntityAnimationNames;
	ogreEntityRenderer["setShowSkeleton"] = &OgreEntityRenderer::setShowSkeleton;

	auto ogreInfo = OgreView.new_usertype<OgreInfo>("OgreInfo");
	ogreInfo["isIndirect"] = &OgreInfo::isIndirect;
	ogreInfo["createUniqueResourceName"] = &OgreInfo::createUniqueResourceName;

	auto pmInjectorSignaler = Lod.new_usertype<PMInjectorSignaler>("PMInjectorSignaler");
	pmInjectorSignaler["LodInjected"] = LuaConnector::make_property(&PMInjectorSignaler::LodInjected);
	auto quaternionAdapter = Gui.new_usertype<QuaternionAdapter>("QuaternionAdapter",
																 sol::constructors<QuaternionAdapter(CEGUI::Window*, CEGUI::Window*, CEGUI::Window*, CEGUI::Window*)>());
	quaternionAdapter["getValue"] = &QuaternionAdapter::getValue;
	quaternionAdapter["getOriginalValue"] = &QuaternionAdapter::getOriginalValue;
	quaternionAdapter["setValue"] = &QuaternionAdapter::setValue;
	quaternionAdapter["updateGui"] = &QuaternionAdapter::updateGui;
	quaternionAdapter["EventValueChanged"] = LuaConnector::make_property(&QuaternionAdapter::EventValueChanged);
	auto helpMessage = Gui.new_usertype<HelpMessage>("HelpMessage",
													 sol::constructors<HelpMessage(std::string, std::string, const std::string&, std::string)>());
	helpMessage["mTitle"] = &HelpMessage::mTitle;
	helpMessage["mMessage"] = &HelpMessage::mMessage;
	helpMessage["mTags"] = &HelpMessage::mTags;
	helpMessage["mId"] = &HelpMessage::mId;
	helpMessage["getTags"] = &HelpMessage::getTags;

	auto quickHelp = Gui.new_usertype<QuickHelp>("QuickHelp");
	quickHelp["getSingleton"] = &QuickHelp::getSingleton;
	quickHelp["updateText"] = &QuickHelp::updateText;
	quickHelp["showWidget"] = &QuickHelp::showWidget;
	quickHelp["EventToggleWidgetVisibility"] = LuaConnector::make_property(&QuickHelp::EventToggleWidgetVisibility);

	auto quickHelpCursor = Gui.new_usertype<QuickHelpCursor>("QuickHelpCursor");
	quickHelpCursor["nextMessage"] = &QuickHelpCursor::nextMessage;
	quickHelpCursor["previousMessage"] = &QuickHelpCursor::previousMessage;
	quickHelpCursor["getSize"] = &QuickHelpCursor::getSize;
	quickHelpCursor["getCursorLocation"] = &QuickHelpCursor::getCursorLocation;
	quickHelpCursor["EventUpdateText"] = LuaConnector::make_property(&QuickHelpCursor::EventUpdateText);

	auto resourceListAdapter = AdaptersOgre.new_usertype<ResourceListAdapter>("ResourceListAdapter",
																			  sol::constructors<ResourceListAdapter(Ember::OgreView::Gui::ListHolder*, Ogre::ResourceManager*)>());
	resourceListAdapter["update"] = &ResourceListAdapter::update;

	auto ruleEditor = Authoring.new_usertype<RuleEditor>("RuleEditor",
														 sol::constructors<RuleEditor(Eris::Avatar&)>());
	ruleEditor["updateOrCreateRule"] = &RuleEditor::updateOrCreateRule;
	ruleEditor["EventRuleCreated"] = LuaConnector::make_property(&RuleEditor::EventRuleCreated);
	ruleEditor["EventRuleUpdated"] = LuaConnector::make_property(&RuleEditor::EventRuleUpdated);
	ruleEditor["EventRuleEditError"] = LuaConnector::make_property(&RuleEditor::EventRuleEditError);

	auto ruleTreeAdapter = AdaptersEris.new_usertype<RuleTreeAdapter>("RuleTreeAdapter",
																	  sol::constructors<RuleTreeAdapter(Eris::Connection&, std::string, CEGUI::Tree&)>());
	ruleTreeAdapter["refresh"] = &RuleTreeAdapter::refresh;
	ruleTreeAdapter["getSelectedRule"] = &RuleTreeAdapter::getSelectedRule;
	ruleTreeAdapter["EventNewRuleReceived"] = LuaConnector::make_property(&RuleTreeAdapter::EventNewRuleReceived);
	ruleTreeAdapter["EventAllRulesReceived"] = LuaConnector::make_property(&RuleTreeAdapter::EventAllRulesReceived);

	auto scene = OgreView.new_usertype<Scene>("Scene");
	scene["getSceneManager"] = &Scene::getSceneManager;
	scene["getMainCamera"] = &Scene::getMainCamera;

	auto screen = OgreView.new_usertype<Screen>("Screen");
	screen["toggleRenderMode"] = &Screen::toggleRenderMode;
	screen["takeScreenshot"] = &Screen::takeScreenshot;
	screen["getFrameStats"] = &Screen::getFrameStats;

	auto simpleRenderContext = OgreView.new_usertype<SimpleRenderContext>("SimpleRenderContext",
																		  sol::constructors<
																				  SimpleRenderContext(const std::string&, int, int),
																				  SimpleRenderContext(const std::string&, Ogre::TexturePtr)
																		  >());
	simpleRenderContext["getSceneNode"] = &SimpleRenderContext::getSceneNode;
	simpleRenderContext["getCamera"] = &SimpleRenderContext::getCamera;
	simpleRenderContext["getDefaultCameraDistance"] = &SimpleRenderContext::getDefaultCameraDistance;
	simpleRenderContext["setActive"] = &SimpleRenderContext::setActive;
	simpleRenderContext["repositionCamera"] = &SimpleRenderContext::repositionCamera;
	simpleRenderContext["pitchCamera"] = &SimpleRenderContext::pitchCamera;
	simpleRenderContext["yawCamera"] = &SimpleRenderContext::yawCamera;
	simpleRenderContext["rollCamera"] = &SimpleRenderContext::rollCamera;
	simpleRenderContext["setCameraDistance"] = &SimpleRenderContext::setCameraDistance;
	simpleRenderContext["getCameraDistance"] = &SimpleRenderContext::getCameraDistance;
	simpleRenderContext["getAbsoluteCameraDistance"] = &SimpleRenderContext::getAbsoluteCameraDistance;
	simpleRenderContext["getCameraOrientation"] = &SimpleRenderContext::getCameraOrientation;
	simpleRenderContext["resetCameraOrientation"] = &SimpleRenderContext::resetCameraOrientation;
	simpleRenderContext["setCameraPositionMode"] = &SimpleRenderContext::setCameraPositionMode;
	simpleRenderContext["CPM_OBJECTCENTER"] = sol::var(SimpleRenderContext::CPM_OBJECTCENTER);
	simpleRenderContext["CPM_WORLDCENTER"] = sol::var(SimpleRenderContext::CPM_WORLDCENTER);
	simpleRenderContext["CPM_FREE"] = sol::var(SimpleRenderContext::CPM_FREE);

	auto subModel = Model.new_usertype<SubModel>("SubModel");
	subModel["getEntity"] = &SubModel::getEntity;

	auto basePointUserObject = Terrain.new_usertype<BasePointUserObject>("BasePointUserObject");
	basePointUserObject["getBasePoint"] = &BasePointUserObject::getBasePoint;
	basePointUserObject["getBasePointMarkerNode"] = &BasePointUserObject::getBasePointMarkerNode;
	basePointUserObject["getPosition"] = &BasePointUserObject::getPosition;
	basePointUserObject["getHeight"] = &BasePointUserObject::getHeight;
	basePointUserObject["translate"] = &BasePointUserObject::translate;
	basePointUserObject["getRoughness"] = &BasePointUserObject::getRoughness;
	basePointUserObject["getFalloff"] = &BasePointUserObject::getFalloff;
	basePointUserObject["EventUpdatedPosition"] = LuaConnector::make_property(&BasePointUserObject::EventUpdatedPosition);

	auto terrainEditBasePointMovement = Terrain.new_usertype<TerrainEditBasePointMovement>("TerrainEditBasePointMovement");
	terrainEditBasePointMovement["mVerticalMovement"] = &TerrainEditBasePointMovement::mVerticalMovement;
	terrainEditBasePointMovement["mPosition"] = &TerrainEditBasePointMovement::mPosition;

	auto terrainEditor = Terrain.new_usertype<TerrainEditor>("TerrainEditor",
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

	auto terrainEditorOverlay = Terrain.new_usertype<TerrainEditorOverlay>("TerrainEditorOverlay");
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

	auto terrainHandler = Terrain.new_usertype<TerrainHandler>("TerrainHandler");
	terrainHandler["getMax"] = &TerrainHandler::getMax;
	terrainHandler["getMin"] = &TerrainHandler::getMin;
	terrainHandler["getPageMetersSize"] = &TerrainHandler::getPageMetersSize;

	auto terrainLayerDefinition = Terrain.new_usertype<TerrainLayerDefinition>("TerrainLayerDefinition");
	terrainLayerDefinition["mAreaId"] = &TerrainLayerDefinition::mAreaId;
	terrainLayerDefinition["mName"] = &TerrainLayerDefinition::mName;
	terrainLayerDefinition["mDiffuseTextureName"] = &TerrainLayerDefinition::mDiffuseTextureName;
	terrainLayerDefinition["mNormalMapTextureName"] = &TerrainLayerDefinition::mNormalMapTextureName;
	terrainLayerDefinition["mShaderName"] = &TerrainLayerDefinition::mShaderName;
	terrainLayerDefinition["mTileSize"] = &TerrainLayerDefinition::mTileSize;

	auto terrainLayerDefinitionManager = Terrain.new_usertype<TerrainLayerDefinitionManager>("TerrainLayerDefinitionManager");
	terrainLayerDefinitionManager["getSingleton"] = &TerrainLayerDefinitionManager::getSingleton;
	terrainLayerDefinitionManager["getDefinitions"] = &TerrainLayerDefinitionManager::getDefinitions;
	terrainLayerDefinitionManager["getDefinitionForArea"] = &TerrainLayerDefinitionManager::getDefinitionForArea;
	terrainLayerDefinitionManager["getDefinitionForShader"] = &TerrainLayerDefinitionManager::getDefinitionForShader;

	auto terrainManager = Terrain.new_usertype<TerrainManager>("TerrainManager");
	terrainManager["getHandler"] = &TerrainManager::getHandler;
	terrainManager["getTerrainAdapter"] = &TerrainManager::getTerrainAdapter;
	terrainManager["getScene"] = &TerrainManager::getScene;
	terrainManager["EventTerrainPageGeometryUpdated"] = LuaConnector::make_property(&TerrainManager::EventTerrainPageGeometryUpdated);

	auto vector3Adapter = Gui.new_usertype<Vector3Adapter>("Vector3Adapter",
														   sol::constructors<Vector3Adapter(CEGUI::Window*, CEGUI::Window*, CEGUI::Window*)>());
	vector3Adapter["getValue"] = &Vector3Adapter::getValue;
	vector3Adapter["getOriginalValue"] = &Vector3Adapter::getOriginalValue;
	vector3Adapter["setValue"] = &Vector3Adapter::setValue;
	vector3Adapter["updateGui"] = &Vector3Adapter::updateGui;
	vector3Adapter["EventValueChanged"] = LuaConnector::make_property(&Vector3Adapter::EventValueChanged);

	auto widget = Gui.new_usertype<Widget>("Widget");
	widget["show"] = &Widget::show;
	widget["hide"] = &Widget::hide;
	widget["isVisible"] = &Widget::isVisible;
	widget["isActive"] = &Widget::isActive;
	widget["getWindow"] = [](Widget* self, const std::string& name) { return &self->getWindow<CEGUI::Window>(name); };
	widget["enableCloseButton"] = &Widget::enableCloseButton;
	widget["registerConsoleVisibilityToggleCommand"] = &Widget::registerConsoleVisibilityToggleCommand;
	widget["getMainSheet"] = &Widget::getMainSheet;
	widget["getMainWindow"] = &Widget::getMainWindow;
	widget["createWindow"] = sol::resolve<CEGUI::Window*(const std::string&)>(&Widget::createWindow);
	widget["loadMainSheet"] = &Widget::loadMainSheet;
	widget["getPrefix"] = &Widget::getPrefix;
	widget["getIsActiveWindowOpaque"] = &Widget::getIsActiveWindowOpaque;
	widget["setIsActiveWindowOpaque"] = &Widget::setIsActiveWindowOpaque;
	widget["getDefaultScheme"] = &Widget::getDefaultScheme;
	widget["addTabbableWindow"] = &Widget::addTabbableWindow;
	widget["closeTabGroup"] = &Widget::closeTabGroup;
	widget["EventFrameStarted"] = LuaConnector::make_property(&Widget::EventFrameStarted);
	widget["EventFirstTimeShown"] = LuaConnector::make_property(&Widget::EventFirstTimeShown);

	auto world = OgreView.new_usertype<World>("World");
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


	auto ogreUtils = lua.create_table();
	ogreUtils["getSubMeshName"] = [](Ogre::Mesh* mesh, unsigned int subMeshIndex) { return OgreUtils::getSubMeshName(mesh, subMeshIndex); };
	OgreView["OgreUtils"] = ogreUtils;

}
