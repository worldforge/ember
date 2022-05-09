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
#include "components/ogre/authoring/RuleEditor.h"
#include "components/ogre/Scene.h"
#include "components/ogre/Screen.h"
#include "components/ogre/World.h"


#include <Eris/Entity.h>
#include <Eris/Avatar.h>
#include <Eris/Connection.h>

#include <wfmath/point.h>
#include <wfmath/vector.h>

#include <Ogre.h>

#include <CEGUI/CEGUI.h>
#include <Atlas/Objects/Root.h>
#include <Atlas/Objects/SmartPtr.h>

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

void registerBindingsEmberOgre(sol::state_view& lua) {
	lua.new_usertype<ActionBarInput>("Ember::OgreView::Gui::ActionBarInput",
									 sol::constructors<ActionBarInput(const std::string&)>(),
									 "EventGotHotkeyInput",
									 sol::property([](ActionBarInput* self) {
										 return Connector::makeConnector<StringValueAdapter>(self->EventGotHotkeyInput);
									 }));
	lua.new_usertype<ActionBarIconDragDropTarget>("Ember::OgreView::Gui::ActionBarIconDragDropTarget",
												  sol::constructors<ActionBarIconDragDropTarget(CEGUI::Window*)>(),
												  "EventIconEntered",
												  sol::property([](ActionBarIconDragDropTarget* self) {
													  return Connector::makeConnector<PtrValueAdapter<ActionBarIcon>>(self->EventIconEntered);
												  }),
												  "EventIconLeaves",
												  sol::property([](ActionBarIconDragDropTarget* self) {
													  return Connector::makeConnector<PtrValueAdapter<ActionBarIcon>>(self->EventIconLeaves);
												  }),
												  "EventActionBarIconDropped",
												  sol::property([](ActionBarIconDragDropTarget* self) {
													  return Connector::makeConnector<PtrValueAdapter<ActionBarIcon>>(self->EventActionBarIconDropped);
												  }),
												  "EventEntityIconDropped",
												  sol::property([](ActionBarIconDragDropTarget* self) {
													  return Connector::makeConnector<PtrValueAdapter<EntityIcon>>(self->EventEntityIconDropped);
												  }));

	lua.new_usertype<ActionBarIcon>("Ember::OgreView::Gui::ActionBarIcon",
									"getImage", &ActionBarIcon::getImage,
									"getDragContainer", &ActionBarIcon::getDragContainer,
									"getIcon", &ActionBarIcon::getIcon,
									"setSlot", &ActionBarIcon::setSlot,
									"setTooltipText", &ActionBarIcon::setTooltipText,
									"defaultAction", &ActionBarIcon::defaultAction,
									sol::meta_function::equal_to, [](ActionBarIcon* lhs, ActionBarIcon* rhs) { return *lhs == *rhs; },
									sol::base_classes, sol::bases<ActionBarIconDragDropTarget>());


	lua.new_usertype<ActionBarIconManager::AvatarIdType>("Ember::OgreView::Gui::ActionBarIconManager::AvatarIdType",
														 "new", [](const Eris::ServerInfo serverInfo, std::string avatarId) { return ActionBarIconManager::AvatarIdType{serverInfo, avatarId}; },
														 "serverInfo", &ActionBarIconManager::AvatarIdType::serverInfo,
														 "avatarId", &ActionBarIconManager::AvatarIdType::avatarId
	);

	lua.new_usertype<ActionBarIconManager>("Ember::OgreView::Gui::ActionBarIconManager",
										   "createSlot", &ActionBarIconManager::createSlot,
										   "destroySlot", &ActionBarIconManager::destroySlot,
										   "createIcon", &ActionBarIconManager::createIcon,
										   "getSavedValue", &ActionBarIconManager::getSavedValue,
										   "saveValue", &ActionBarIconManager::saveValue,
										   "eraseValue", &ActionBarIconManager::eraseValue,
										   "EventIconDragStart",
										   sol::property([](ActionBarIconManager* self) {
											   return Connector::makeConnector<PtrValueAdapter<ActionBarIcon>>(self->EventIconDragStart);
										   }),
										   "EventIconDragStop",
										   sol::property([](ActionBarIconManager* self) {
											   return Connector::makeConnector<PtrValueAdapter<ActionBarIcon>>(self->EventIconDragStop);
										   })
	);

	lua.new_usertype<ActionBarIconSlot>("Ember::OgreView::Gui::ActionBarIconSlot",
										"addActionBarIcon", &ActionBarIconSlot::addActionBarIcon,
										"removeActionBarIcon", &ActionBarIconSlot::removeActionBarIcon,
										"getActionBarIcon", &ActionBarIconSlot::getActionBarIcon,
										"getWindow", &ActionBarIconSlot::getWindow,
										"notifyIconDraggedOff", &ActionBarIconSlot::notifyIconDraggedOff,
										"EventIconDraggedOff",
										sol::property([](ActionBarIconSlot* self) {
											return Connector::makeConnector<PtrValueAdapter<ActionBarIcon>>(self->EventIconDraggedOff);
										}),
										sol::base_classes, sol::bases<ActionBarIconDragDropTarget>());

	lua.new_usertype<TexturePair>("Ember::OgreView::Gui::TexturePair",
								  "hasData", &TexturePair::hasData,
								  "ogreTexture", &TexturePair::ogreTexture,
								  "textureImte", &TexturePair::textureImage);

	lua.new_usertype<AssetsManager>("Ember::OgreView::Gui::AssetsManager",
									"showTexture", &AssetsManager::showTexture,
									"createTextureImage", &AssetsManager::createTextureImage,
									"materialAsText", &AssetsManager::materialAsText,
									"resolveResourceNameFromFilePath", &AssetsManager::resolveResourceNameFromFilePath,
									"resolveFilePathForMesh", &AssetsManager::resolveFilePathForMesh,
									"exportMesh", &AssetsManager::exportMesh,
									"createModel", &AssetsManager::createModel,
									"getLodInjectorSignaler", &AssetsManager::getLodInjectorSignaler
	);

	lua.new_usertype<AtlasHelper>("Ember::OgreView::Gui::AtlasHelper",
								  "serialize", &AtlasHelper::serialize);


	lua.new_usertype<AuthoringManager>("Ember::OgreView::Authoring::AuthoringManager",
									   "displayAuthoringVisualization", &AuthoringManager::displayAuthoringVisualization,
									   "hideAuthoringVisualization", &AuthoringManager::hideAuthoringVisualization,
									   "displaySimpleEntityVisualization", &AuthoringManager::displaySimpleEntityVisualization,
									   "hideSimpleEntityVisualization", &AuthoringManager::hideSimpleEntityVisualization,
									   "hasSimpleEntityVisualization", &AuthoringManager::hasSimpleEntityVisualization,
									   "startMovement", &AuthoringManager::startMovement,
									   "stopMovement", &AuthoringManager::stopMovement
	);

	lua.new_usertype<Avatar>("Ember::OgreView::Avatar",
							 "getAvatarSceneNode", &Avatar::getAvatarSceneNode,
							 "getEmberEntity", &Avatar::getEmberEntity,
							 "getErisAvatar", &Avatar::getErisAvatar,
							 "deleteEntity", &Avatar::deleteEntity,
							 "setMinIntervalOfRotationChanges", &Avatar::setMinIntervalOfRotationChanges,
							 "isAdmin", &Avatar::isAdmin,
							 "getClientSideAvatarPosition", &Avatar::getClientSideAvatarPosition,
							 "getClientSideAvatarOrientation", &Avatar::getClientSideAvatarOrientation,
							 "useTool", [](Avatar* self,
										   const Ember::EmberEntity& tool,
										   const std::string& operation,
										   const Eris::Entity* target,
										   const WFMath::Point<3>& pos,
										   const WFMath::Vector<3>& direction) {
				self->useTool(tool, operation, target, pos, direction);
			},
							 "EventAddedEntityToInventory",
							 sol::property([](Avatar* self) {
								 return Connector::makeConnector<PtrValueAdapter<Ember::EmberEntity>>(self->EventAddedEntityToInventory);
							 }),
							 "EventRemovedEntityFromInventory",
							 sol::property([](Avatar* self) {
								 return Connector::makeConnector<PtrValueAdapter<Eris::Entity>>(self->EventRemovedEntityFromInventory);
							 }),
							 "EventAvatarEntityDestroyed",
							 sol::property([](Avatar* self) {
								 return Connector::makeConnector(self->EventAvatarEntityDestroyed);
							 })
	);

	lua.new_usertype<ColouredListItem>("Ember::OgreView::Gui::ColouredListItem",
									   "new", sol::factories(
					[](const std::string& text) { return std::make_unique<ColouredListItem>(text); },
					[](const std::string& text, unsigned int item_id) { return std::make_unique<ColouredListItem>(text, item_id); },
					[](const std::string& text, unsigned int item_id, void* item_data) { return std::make_unique<ColouredListItem>(text, item_id, item_data); }));

	lua.new_usertype<ColouredTreeItem>("Ember::OgreView::Gui::ColouredTreeItem",
									   "new", sol::factories(
					[](const std::string& text) { return std::make_unique<ColouredTreeItem>(text); },
					[](const std::string& text, unsigned int item_id) { return std::make_unique<ColouredTreeItem>(text, item_id); },
					[](const std::string& text, unsigned int item_id, void* item_data) { return std::make_unique<ColouredTreeItem>(text, item_id, item_data); }));

	auto compass = lua.new_usertype<Compass>("Ember::OgreView::Gui::Compass",
											 sol::constructors<Compass(Ember::OgreView::Gui::ICompassImpl*, Ogre::SceneManager&, ITerrainAdapter&)>()
	);

	compass["getMap"] = &Compass::getMap;
	compass["reposition"] = &Compass::reposition;
	compass["refresh"] = &Compass::refresh;

	lua.new_usertype<RenderedCompassImpl>("Ember::OgreView::Gui::RenderedCompassImpl",
										  sol::constructors<RenderedCompassImpl(CEGUI::Window*)>(),
										  "getTexture", &RenderedCompassImpl::getTexture,
										  sol::base_classes, sol::bases<ICompassImpl>());

	lua.new_usertype<CompassCameraAnchor>("Ember::OgreView::Gui::CompassCameraAnchor",
										  sol::constructors<CompassCameraAnchor(Ember::OgreView::Gui::Compass&, Ogre::Camera*)>());
	lua.new_usertype<CompassSceneNodeAnchor>("Ember::OgreView::Gui::CompassSceneNodeAnchor",
											 sol::constructors<CompassSceneNodeAnchor(Ember::OgreView::Gui::Compass&, Ogre::SceneNode*)>());
	lua.new_usertype<CompassThirdPersonCameraAnchor>("Ember::OgreView::Gui::CompassThirdPersonCameraAnchor",
													 sol::constructors<CompassThirdPersonCameraAnchor(Ember::OgreView::Gui::Compass&, Ogre::Camera*, Ogre::SceneNode*)>());
	lua.new_usertype<ConsoleAdapter>("Ember::OgreView::Gui::ConsoleAdapter",
									 sol::constructors<ConsoleAdapter(CEGUI::Editbox*)>(),
									 "EventCommandExecuted",
									 sol::property([](ConsoleAdapter* self) {
										 return Connector::makeConnector<StringValueAdapter>(self->EventCommandExecuted);
									 })
	);

	lua.new_usertype<ContainerView>("Ember::OgreView::Gui::ContainerView",
									sol::constructors<ContainerView(Ember::OgreView::Gui::EntityIconManager&,
																	Ember::OgreView::Gui::Icons::IconManager&,
																	CEGUI::Tooltip&,
																	CEGUI::Window&,
																	int)>(),
									"showEntityContents", &ContainerView::showEntityContents,
									"getEntityIcon", &ContainerView::getEntityIcon,
									"addEntityIcon", &ContainerView::addEntityIcon,
									"EventEntityPicked",
									sol::property([](ContainerView* self) {
										return Connector::makeConnector<PtrValueAdapter<Ember::EmberEntity>>(self->EventEntityPicked);
									}),
									"EventIconAdded",
									sol::property([](ContainerView* self) {
										return Connector::makeConnector<PtrValueAdapter<EntityIcon>>(self->EventIconAdded);
									}),
									"EventIconDropped",
									sol::property([](ContainerView* self) {
										return Connector::makeConnector<PtrValueAdapter<EntityIcon>, PtrValueAdapter<EntityIconSlot>>(self->EventIconDropped);
									})
	);

	sol::table convert;
	convert["toWF"] = [](const Ogre::Vector3& point) { return Convert::toWF<WFMath::Point<3>>(point); };

	lua["Convert"] = convert;

	lua.new_usertype<EmberOgre>("Ember::OgreView::EmberOgre",
								"getSingleton", &EmberOgre::getSingleton,
								"getWorld", &EmberOgre::getWorld,
								"getScreen", &EmberOgre::getScreen,
								"getRenderWindow", &EmberOgre::getRenderWindow,
								"EventCreatedAvatarEntity",
								sol::property([](EmberOgre* self) {
									return Connector::makeConnector<RefValueAdapter<EmberEntity>>(self->EventCreatedAvatarEntity);
								}),
								"EventGUIManagerCreated",
								sol::property([](EmberOgre* self) {
									return Connector::makeConnector<RefValueAdapter<GUIManager>>(self->EventGUIManagerCreated);
								}),
								"EventGUIManagerBeingDestroyed",
								sol::property([](EmberOgre* self) {
									return Connector::makeConnector(self->EventGUIManagerBeingDestroyed);
								}),
								"EventMotionManagerCreated",
								sol::property([](EmberOgre* self) {
									return Connector::makeConnector<RefValueAdapter<MotionManager>>(self->EventMotionManagerCreated);
								}),
								"EventTerrainManagerCreated",
								sol::property([](EmberOgre* self) {
									return Connector::makeConnector<RefValueAdapter<TerrainManager>>(self->EventTerrainManagerCreated);
								}),
								"EventTerrainManagerBeingDestroyed",
								sol::property([](EmberOgre* self) {
									return Connector::makeConnector(self->EventTerrainManagerBeingDestroyed);
								}),
								"EventMotionManagerDestroyed",
								sol::property([](EmberOgre* self) {
									return Connector::makeConnector(self->EventMotionManagerDestroyed);
								}),
								"EventMovementControllerCreated",
								sol::property([](EmberOgre* self) {
									return Connector::makeConnector(self->EventMovementControllerCreated);
								}),
								"EventMovementControllerDestroyed",
								sol::property([](EmberOgre* self) {
									return Connector::makeConnector(self->EventMovementControllerDestroyed);
								}),
								"EventWorldCreated",
								sol::property([](EmberOgre* self) {
									return Connector::makeConnector<RefValueAdapter<World>>(self->EventWorldCreated);
								}),
								"EventWorldBeingDestroyed",
								sol::property([](EmberOgre* self) {
									return Connector::makeConnector(self->EventWorldBeingDestroyed);
								}),
								"EventWorldDestroyed",
								sol::property([](EmberOgre* self) {
									return Connector::makeConnector(self->EventWorldDestroyed);
								})
	);

	lua.new_usertype<EntityCEGUITexture>("Ember::OgreView::Gui::EntityCEGUITexture",
										 "getImage", &EntityCEGUITexture::getImage,
										 "getRenderContext", &EntityCEGUITexture::getRenderContext);


	lua.new_usertype<EntityCreatorTypeHelper>("Ember::OgreView::Gui::EntityCreatorTypeHelper",
											  sol::constructors<EntityCreatorTypeHelper(Eris::Avatar&,
																						CEGUI::Tree&,
																						CEGUI::Editbox&,
																						CEGUI::PushButton&,
																						CEGUI::Window&,
																						CEGUI::Combobox&,
																						CEGUI::Window&,
																						CEGUI::ToggleButton&,
																						CEGUI::Editbox&)>(),
											  "EventCreateFromType",
											  sol::property([](EntityCreatorTypeHelper* self) {
												  return Connector::makeConnector<RefValueAdapter<const Atlas::Message::MapType>>(self->EventCreateFromType);
											  })
	);
	lua.new_usertype<EntityEditor>("Ember::OgreView::Gui::EntityEditor",
								   sol::constructors<EntityEditor(Ember::OgreView::World&, Eris::Entity&, Ember::OgreView::Gui::Adapters::Atlas::MapAdapter*)>(),
								   "submitChanges", &EntityEditor::submitChanges,
								   "createMapElement", &EntityEditor::createMapElement,
								   "createListElement", &EntityEditor::createListElement,
								   "createStringElement", &EntityEditor::createStringElement,
								   "createIntElement", &EntityEditor::createIntElement,
								   "createFloatElement", &EntityEditor::createFloatElement,
								   "createPosition2dElement", &EntityEditor::createPosition2dElement,
								   "addGoal", &EntityEditor::addGoal,
								   "updateGoal", &EntityEditor::updateGoal,
								   "removeGoal", &EntityEditor::removeGoal,
								   "addKnowledge", &EntityEditor::addKnowledge,
								   "addMarker", &EntityEditor::addMarker,
								   "removeMarker", &EntityEditor::removeMarker,
								   "createPoint", &EntityEditor::createPoint,
								   "getThoughts", &EntityEditor::getThoughts,
								   "getPath", &EntityEditor::getPath,
								   "getGoalInfo", &EntityEditor::getGoalInfo,
								   "parseElementMap", &EntityEditor::parseElementMap,
								   "EventGotThought",
								   sol::property([](EntityEditor* self) {
									   return Connector::makeConnector<RefValueAdapter<const Atlas::Message::Element>>(self->EventGotThought);
								   }),
								   "EventGotGoal",
								   sol::property([](EntityEditor* self) {
									   return Connector::makeConnector<RefValueAdapter<const Atlas::Message::Element>>(self->EventGotGoal);
								   }),
								   "EventGotEmptyGoals",
								   sol::property([](EntityEditor* self) {
									   return Connector::makeConnector(self->EventGotEmptyGoals);
								   }),
								   "EventGotGoalInfo",
								   sol::property([](EntityEditor* self) {
									   return Connector::makeConnector<RefValueAdapter<const Atlas::Message::Element>>(self->EventGotGoalInfo);
								   })
	);


	lua.new_usertype<EntityIconDragDropTarget>("Ember::OgreView::Gui::EntityIconDragDropTarget",
											   sol::constructors<EntityIconDragDropTarget(CEGUI::Window*)>(),
											   "EventIconEntered",
											   sol::property([](EntityIconDragDropTarget* self) {
												   return Connector::makeConnector<PtrValueAdapter<EntityIcon>>(self->EventIconEntered);
											   }),
											   "EventIconLeaves",
											   sol::property([](EntityIconDragDropTarget* self) {
												   return Connector::makeConnector<PtrValueAdapter<EntityIcon>>(self->EventIconLeaves);
											   }),
											   "EventIconDropped",
											   sol::property([](EntityIconDragDropTarget* self) {
												   return Connector::makeConnector<PtrValueAdapter<EntityIcon>>(self->EventIconDropped);
											   })
	);
	//TODO: check if we still need to == operator with sol
	lua.new_usertype<EntityIcon>("Ember::OgreView::Gui::EntityIcon",
								 "getImage", &EntityIcon::getImage,
								 "getDragContainer", &EntityIcon::getDragContainer,
								 "getIcon", &EntityIcon::getIcon,
								 "setSlot", &EntityIcon::setSlot,
								 "getSlot", &EntityIcon::getSlot,
								 "setTooltipText", &EntityIcon::setTooltipText,
								 "getEntity", &EntityIcon::getEntity,
								 "setTag", &EntityIcon::setTag,
								 "getTag", &EntityIcon::getTag,
								 sol::base_classes, sol::bases<EntityIconDragDropTarget>());

	lua.new_usertype<EntityIconDragDropPreview>("Ember::OgreView::Gui::EntityIconDragDropPreview",
												sol::constructors<EntityIconDragDropPreview(Ember::OgreView::World&)>(),
												"createPreview", &EntityIconDragDropPreview::createPreview,
												"cleanupCreation", &EntityIconDragDropPreview::cleanupCreation,
												"getDropPosition", &EntityIconDragDropPreview::getDropPosition,
												"getDropOrientation", &EntityIconDragDropPreview::getDropOrientation,
												"EventEntityFinalized",
												sol::property([](EntityIconDragDropPreview* self) {
													return Connector::makeConnector<PtrValueAdapter<EmberEntity>>(self->EventEntityFinalized);
												}));
	lua.new_usertype<EntityIconManager>("Ember::OgreView::Gui::EntityIconManager",
										"createSlot", &EntityIconManager::createSlot,
										"destroySlot", &EntityIconManager::destroySlot,
										"createIcon", &EntityIconManager::createIcon,
										"destroyIcon", &EntityIconManager::destroyIcon,
										"EventIconDragStart",
										sol::property([](EntityIconManager* self) {
											return Connector::makeConnector<PtrValueAdapter<EntityIcon>>(self->EventIconDragStart);
										}),
										"EventIconDragStop",
										sol::property([](EntityIconManager* self) {
											return Connector::makeConnector<PtrValueAdapter<EntityIcon>>(self->EventIconDragStop);
										}));
	lua.new_usertype<EntityIconSlot>("Ember::OgreView::Gui::EntityIconSlot",
									 "addEntityIcon", &EntityIconSlot::addEntityIcon,
									 "removeEntityIcon", &EntityIconSlot::removeEntityIcon,
									 "getEntityIcon", &EntityIconSlot::getEntityIcon,
									 "getWindow", &EntityIconSlot::getWindow,
									 "notifyIconDraggedOff", &EntityIconSlot::notifyIconDraggedOff,
									 "EventIconDraggedOff",
									 sol::property([](EntityIconSlot* self) {
										 return Connector::makeConnector<PtrValueAdapter<EntityIcon>>(self->EventIconDraggedOff);
									 }),
									 sol::base_classes, sol::bases<EntityIconDragDropTarget>());

	lua.new_usertype<EntityMoveManager>("Ember::OgreView::Authoring::EntityMoveManager",
										"startMove", &EntityMoveManager::startMove,
										"EventStartMoving",
										sol::property([](EntityMoveManager* self) {
											return Connector::makeConnector<RefValueAdapter<EmberEntity>, RefValueAdapter<EntityMover>>(self->EventStartMoving);
										}),
										"EventFinishedMoving",
										sol::property([](EntityMoveManager* self) {
											return Connector::makeConnector(self->EventFinishedMoving);
										}),
										"EventCancelledMoving",
										sol::property([](EntityMoveManager* self) {
											return Connector::makeConnector(self->EventCancelledMoving);
										}));

	lua.new_usertype<EntityRecipe>("Ember::OgreView::Authoring::EntityRecipe",
								   "setAuthor", &EntityRecipe::setAuthor,
								   "getAuthor", &EntityRecipe::getAuthor,
								   "setDescription", &EntityRecipe::setDescription,
								   "getDescription", &EntityRecipe::getDescription
	);

	lua.new_usertype<EntityTextureManipulator>("Ember::OgreView::Gui::EntityRecipe",
											   "EventMovementStarted",
											   sol::property([](EntityTextureManipulator* self) {
												   return Connector::makeConnector(self->EventMovementStarted);
											   }),
											   "EventMovementStopped",
											   sol::property([](EntityTextureManipulator* self) {
												   return Connector::makeConnector(self->EventMovementStopped);
											   })
	);

	lua.new_usertype<DirectEntityTextureManipulator>("Ember::OgreView::Gui::DirectEntityTextureManipulator",
													 sol::constructors<DirectEntityTextureManipulator(CEGUI::Window&, Ember::OgreView::Gui::EntityCEGUITexture&)>(),
													 sol::base_classes, sol::bases<EntityTextureManipulator>()
	);
	lua.new_usertype<CameraEntityTextureManipulator>("Ember::OgreView::Gui::CameraEntityTextureManipulator",
													 sol::constructors<CameraEntityTextureManipulator(CEGUI::Window&, Ember::OgreView::Gui::EntityCEGUITexture&)>(),
													 sol::base_classes, sol::bases<EntityTextureManipulator>()
	);
	lua.new_usertype<CombinedEntityTextureManipulator>("Ember::OgreView::Gui::CombinedEntityTextureManipulator",
													   sol::constructors<CombinedEntityTextureManipulator(CEGUI::Window&, Ember::OgreView::Gui::EntityCEGUITexture&)>(),
													   "EventEditingStarted",
													   sol::property([](CombinedEntityTextureManipulator* self) {
														   return Connector::makeConnector(self->EventEditingStarted);
													   }),
													   "EventEditingStopped",
													   sol::property([](CombinedEntityTextureManipulator* self) {
														   return Connector::makeConnector(self->EventEditingStopped);
													   }),
													   sol::base_classes, sol::bases<EntityTextureManipulator>()
	);

	lua.new_usertype<EntityTooltip>("Ember::OgreView::Gui::EntityTooltip",
									"getTooltipWindow", &EntityTooltip::getTooltipWindow
	);

	lua.new_usertype<EntityPickResult>("Ember::OgreView::EntityPickResult",
									   "entity", &EntityPickResult::entity,
									   "position", &EntityPickResult::position,
									   "distance", &EntityPickResult::distance
	);
	lua.new_usertype<EntityWorldPickListener>("Ember::OgreView::EntityWorldPickListener",
											  "EventPickedEntity",
											  sol::property([](EntityWorldPickListener* self) {
												  return Connector::makeConnector<RefValueAdapter<const std::vector<Ember::OgreView::EntityPickResult>>, RefValueAdapter<const Ember::OgreView::MousePickerArgs>>(self->EventPickedEntity);
											  })
	);


	lua.new_usertype<ISun>("Ember::OgreView::Environment::ISun",
						   "setAmbientLight", &ISun::setAmbientLight

	);
	lua.new_usertype<IFog>("Ember::OgreView::Environment::IFog",
						   "setDensity", &IFog::setDensity,
						   "getDensity", &IFog::getDensity
	);
	lua.new_usertype<Ember::OgreView::Environment::Environment>("Ember::OgreView::Environment::Environment",
																"getSun", &Ember::OgreView::Environment::Environment::getSun,
																"getFog", &Ember::OgreView::Environment::Environment::getFog,
																"setTime", [](Ember::OgreView::Environment::Environment* self, int seconds) { self->setTime(seconds); },
																"setTimeMultiplier", &Ember::OgreView::Environment::Environment::setTimeMultiplier,
																"getTimeMultiplier", &Ember::OgreView::Environment::Environment::getTimeMultiplier,
																"setAmbientLight", &Ember::OgreView::Environment::Environment::setAmbientLight
	);
	lua.new_usertype<GUIManager>("Ember::OgreView::GUIManager",
								 "getSingleton", &GUIManager::getSingleton,
								 "appendAvatarImaginary", [](GUIManager* self, const std::string& message) { self->AppendAvatarImaginary.emit(message); },
								 "removeWidget", &GUIManager::removeWidget,
								 "addWidget", &GUIManager::addWidget,
								 "EmitEntityAction", &GUIManager::EmitEntityAction,
								 "getMainSheet", &GUIManager::getMainSheet,
								 "isInGUIMode", &GUIManager::isInGUIMode,
								 "isInMovementKeysMode", &GUIManager::isInMovementKeysMode,
								 "getLayoutDir", &GUIManager::getLayoutDir,
								 "createWindow", [](GUIManager* self, const std::string& windowType) { return self->createWindow(windowType); },
								 "createWidget", &GUIManager::createWidget,
								 "destroyWidget", &GUIManager::destroyWidget,
								 "getDefaultScheme", &GUIManager::getDefaultScheme,
								 "getIconManager", &GUIManager::getIconManager,
								 "getEntityIconManager", &GUIManager::getEntityIconManager,
								 "getActionBarIconManager", &GUIManager::getActionBarIconManager,
								 "getEntityTooltip", &GUIManager::getEntityTooltip,
								 "AppendIGChatLine",
								 sol::property([](GUIManager* self) {
									 return Connector::makeConnector<RefValueAdapter<const EntityTalk>, PtrValueAdapter<EmberEntity>>(self->AppendIGChatLine);
								 }),
								 "AppendOOGChatLine",
								 sol::property([](GUIManager* self) {
									 return Connector::makeConnector<StringValueAdapter, PtrValueAdapter<EmberEntity>>(self->AppendOOGChatLine);
								 }),
								 "AppendAvatarImaginary",
								 sol::property([](GUIManager* self) {
									 return Connector::makeConnector<StringValueAdapter>(self->AppendAvatarImaginary);
								 }),
								 "EventEntityAction",
								 sol::property([](GUIManager* self) {
									 return Connector::makeConnector<StringValueAdapter, PtrValueAdapter<EmberEntity>>(self->EventEntityAction);
								 }),
								 "EventFrameStarted",
								 sol::property([](GUIManager* self) {
									 return Connector::makeConnector<NumberValueAdapter<float>>(self->EventFrameStarted);
								 }));

	lua.new_usertype<Ember::OgreView::Gui::Icons::Icon>("Ember::OgreView::Gui::Icons::Icon",
														"getImage", &Icon::getImage
	);

	lua.new_usertype<IconBar>("Ember::OgreView::Gui::IconBar",
							  sol::constructors<IconBar(const std::string&)>(),
							  "addIcon", &IconBar::addIcon,
							  "removeIcon", &IconBar::removeIcon,
							  "getWindow", &IconBar::getWindow,
							  "setIconPadding", &IconBar::setIconPadding,
							  "getAbsoluteHeight", &IconBar::getAbsoluteHeight,
							  "getAbsoluteWidth", &IconBar::getAbsoluteWidth
	);

	lua.new_usertype<IconBase>("Ember::OgreView::Gui::IconBase",
							   sol::constructors<IconBase(const std::string&, const CEGUI::Image*, const CEGUI::Image*, const CEGUI::Image*, const CEGUI::Image*),
									   IconBase(const std::string&, const CEGUI::Image*, const CEGUI::Image*, const CEGUI::Image*, const CEGUI::Image*, CEGUI::USize)>(),
							   "getContainer", &IconBase::getContainer,
							   "getButton", &IconBase::getButton,
							   "setForeground", &IconBase::setForeground,
							   "loadImageFromImageset", &IconBase::loadImageFromImageset
	);

	lua.new_usertype<IconManager>("Ember::OgreView::Gui::Icons::IconManager",
								  "getIcon", sol::overload([](IconManager* self, int pixelWidth, Ember::EmberEntity* entity) { return self->getIcon(pixelWidth, entity); },
														   [](IconManager* self, int pixelWidth, Eris::TypeInfo* erisType) { return self->getIcon(pixelWidth, erisType); }),
								  "render", sol::overload([](IconManager* self, Icon& icon, const std::string& modelName) { self->render(icon, modelName); },
														  [](IconManager* self, Icon& icon, Ember::EmberEntity& entity) { self->render(icon, entity); },
														  [](IconManager* self, Icon& icon, Eris::TypeInfo& erisType) { self->render(icon, erisType); })
	);

	lua["Ember::OgreView::MousePickType"] = lua.create_table_with(
			"MPT_CLICK", MousePickType::MPT_CLICK,
			"MPT_DOUBLECLICK", MousePickType::MPT_DOUBLECLICK,
			"MPT_PRESSED", MousePickType::MPT_PRESSED,
			"MPT_HOVER", MousePickType::MPT_HOVER,
			"MPT_PRESS", MousePickType::MPT_PRESS
	);
	lua.new_usertype<MousePickerArgs>("Ember::OgreView::MousePickerArgs",
									  "windowX", &MousePickerArgs::windowX,
									  "windowY", &MousePickerArgs::windowY,
									  "pickType", &MousePickerArgs::pickType

	);

	lua.new_usertype<ListHolder>("Ember::OgreView::Gui::ListHolder",
								 sol::constructors<ListHolder(CEGUI::Listbox&, CEGUI::Editbox*)>(),
								 "addItem", [](ListHolder* self, CEGUI::ListboxItem* item) { self->addItem(item); },
								 "insertItem", &ListHolder::insertItem,
								 "removeItem", &ListHolder::removeItem,
								 "resetList", &ListHolder::resetList

	);

	lua.new_usertype<LodDistance>("Ember::OgreView::Lod::LodDistance",
								  "meshName", &LodDistance::meshName,
								  "reductionMethod", &LodDistance::reductionMethod,
								  "reductionValue", &LodDistance::reductionValue
	);

	auto lodDefinition = lua.new_usertype<LodDefinition>("Ember::OgreView::Lod::LodDefinition",
														 "getUseAutomaticLod", &LodDefinition::getUseAutomaticLod,
														 "getUseAutomaticLod", &LodDefinition::getUseAutomaticLod,
														 "setUseAutomaticLod", &LodDefinition::setUseAutomaticLod,
														 "getType", &LodDefinition::getType,
														 "setType", &LodDefinition::setType,
														 "getStrategy", &LodDefinition::getStrategy,
														 "setStrategy", &LodDefinition::setStrategy,
														 "addLodDistance", &LodDefinition::addLodDistance,
														 "hasLodDistance", &LodDefinition::hasLodDistance,
														 "getLodDistance", &LodDefinition::getLodDistance,
														 "getLodDistanceCount", &LodDefinition::getLodDistanceCount,
														 "createListOfDistances", &LodDefinition::createListOfDistances,
														 "createDistance", &LodDefinition::createDistance,
														 "removeLodDistance", &LodDefinition::removeLodDistance,
														 sol::base_classes, sol::bases<Ogre::Resource>()
	);

	lodDefinition["LodType"] = lua.create_table_with(
			"LT_AUTOMATIC_VERTEX_REDUCTION", Ember::OgreView::Lod::LodDefinition::LT_AUTOMATIC_VERTEX_REDUCTION,
			"LT_USER_CREATED_MESH", Ember::OgreView::Lod::LodDefinition::LT_USER_CREATED_MESH
	);

	lodDefinition["LodStrategy"] = lua.create_table_with(
			"LS_DISTANCE", Ember::OgreView::Lod::LodDefinition::LS_DISTANCE,
			"LS_PIXEL_COUNT", Ember::OgreView::Lod::LodDefinition::LS_PIXEL_COUNT
	);
	lua.new_usertype<LodDefinitionPtr>("Ember::OgreView::Lod::LodDefinitionPtr",
									   "get", &LodDefinitionPtr::get
	);

	lua.new_usertype<LodDefinitionManager>("Ember::OgreView::Lod::LodDefinitionManager",
										   "getSingleton", &LodDefinitionManager::getSingleton,
										   "exportScript", &LodDefinitionManager::exportScript,
										   "create", &LodDefinitionManager::create,
										   "getByName", &LodDefinitionManager::getByName,
										   sol::base_classes, sol::bases<Ogre::ResourceManager>()

	);

	lua.new_usertype<LodManager>("Ember::OgreView::Lod::LodManager",
								 "getSingleton", &LodManager::getSingleton,
								 "loadLod", [](LodManager* self, Ogre::MeshPtr mesh, const LodDefinition& def) { self->loadLod(mesh, def); }
	);

	lua.new_usertype<MainCamera>("Ember::OgreView::Camera::MainCamera",
								 "getCamera", &MainCamera::getCamera,
								 "getOrientation", &MainCamera::getOrientation,
								 "getPosition", &MainCamera::getPosition,
								 "setClosestPickingDistance", &MainCamera::setClosestPickingDistance,
								 "getClosestPickingDistance", &MainCamera::getClosestPickingDistance,
								 "enableCompositor", &MainCamera::enableCompositor,
								 "MovedCamera",
								 sol::property([](MainCamera* self) {
									 return Connector::makeConnector<RefValueAdapter<Ogre::Camera>>(self->MovedCamera);
								 })
	);

	lua.new_usertype<Map>("Ember::OgreView::Terrain::Map",
						  sol::constructors<Map(Ogre::SceneManager&)>(),
						  "initialize", &Map::initialize,
						  "getTexture", &Map::getTexture,
						  "render", &Map::render,
						  "reposition", [](Map* self, float x, float y) { self->reposition(x, y); },
						  "setDistance", &Map::setDistance,
						  "getDistance", &Map::getDistance,
						  "getResolution", &Map::getResolution,
						  "setResolution", &Map::setResolution,
						  "getResolutionMeters", &Map::getResolutionMeters

	);
	lua.new_usertype<MeshInfoProvider>("Ember::OgreView::Gui::MeshInfoProvider",
									   sol::constructors<MeshInfoProvider(OgreEntityRenderer*)>(),
									   "getInfo", &MeshInfoProvider::getInfo,
									   "getPreviewInfo", &MeshInfoProvider::getPreviewInfo,
									   "postqueueEntityMeshLodChanged", &MeshInfoProvider::postqueueEntityMeshLodChanged,
									   "getLodIndex", &MeshInfoProvider::getLodIndex,
									   "calcUniqueVertexCount", [](const Ogre::Mesh* mesh) { return MeshInfoProvider::calcUniqueVertexCount(mesh); }
	);
	lua.new_usertype<AnimationSet>("Ember::OgreView::Model::AnimationSet",
								   "addTime", [](AnimationSet* self, float timeSlice) { self->addTime(timeSlice); },
								   "addAnimation", &AnimationSet::addAnimation,
								   "reset", &AnimationSet::reset,
								   "setSpeed", &AnimationSet::setSpeed,
								   "getSpeed", &AnimationSet::getSpeed
	);

	lua.new_usertype<Action>("Ember::OgreView::Model::Action",
							 "name", &Action::name,
							 "animations", &Action::animations
	);
	lua.new_usertype<Ember::OgreView::Model::Model>("Ember::OgreView::Model::Model",
													"reload", &Ember::OgreView::Model::Model::reload,
													"removeSubmodel", &Ember::OgreView::Model::Model::removeSubmodel,
													"getAction", sol::resolve<Action*(const std::string&)>(&Ember::OgreView::Model::Model::getAction),
													"showPart", &Ember::OgreView::Model::Model::showPart,
													"hidePart", &Ember::OgreView::Model::Model::hidePart,
													"setVisible", &Ember::OgreView::Model::Model::setVisible,
													"getRotation", &Ember::OgreView::Model::Model::getRotation,
													"getName", &Ember::OgreView::Model::Model::getName,
													"getDefinition", &Ember::OgreView::Model::Model::getDefinition,
													"hasAttachPoint", &Ember::OgreView::Model::Model::hasAttachPoint,
													"hasParticles", &Ember::OgreView::Model::Model::hasParticles,
													"getSubModel", &Ember::OgreView::Model::Model::getSubModel,
													"setDisplaySkeleton", &Ember::OgreView::Model::Model::setDisplaySkeleton,
													"getDisplaySkeleton", &Ember::OgreView::Model::Model::getDisplaySkeleton
	);

	lua.new_usertype<ViewDefinition>("Ember::OgreView::Model::ViewDefinition",
									 "Name", &ViewDefinition::Name,
									 "Rotation", &ViewDefinition::Rotation,
									 "Distance", &ViewDefinition::Distance

	);
	lua.new_usertype<PoseDefinition>("Ember::OgreView::Model::PoseDefinition",
									 "Rotate", &PoseDefinition::Rotate,
									 "Translate", &PoseDefinition::Translate
	);
	lua.new_usertype<AttachPointDefinition>("Ember::OgreView::Model::AttachPointDefinition",
											"Name", &AttachPointDefinition::Name,
											"BoneName", &AttachPointDefinition::BoneName,
											"Pose", &AttachPointDefinition::Pose,
											"Rotation", &AttachPointDefinition::Rotation,
											"Translation", &AttachPointDefinition::Translation
	);
	lua.new_usertype<SubEntityDefinition>("Ember::OgreView::Model::SubEntityDefinition",
										  "subEntityName", &SubEntityDefinition::subEntityName,
										  "subEntityIndex", &SubEntityDefinition::subEntityIndex,
										  "materialName", &SubEntityDefinition::materialName

	);
	lua.new_usertype<PartDefinition>("Ember::OgreView::Model::PartDefinition",
									 "name", &PartDefinition::name,
									 "show", &PartDefinition::show,
									 "getSubEntityDefinitions", &PartDefinition::getSubEntityDefinitions,
									 "removeSubEntityDefinition", &PartDefinition::removeSubEntityDefinition
	);
	lua.new_usertype<SubModelDefinition>("Ember::OgreView::Model::SubModelDefinition",
										 "meshName", &SubModelDefinition::meshName,
										 "shadowCaster", &SubModelDefinition::shadowCaster,
										 "getPartDefinitions", &SubModelDefinition::getPartDefinitions,
										 "removePartDefinition", &SubModelDefinition::removePartDefinition
	);

	lua.new_usertype<AnimationPartDefinition>("Ember::OgreView::Model::AnimationPartDefinition",
											  "Name", &AnimationPartDefinition::Name,
											  "BoneGroupRefs", &AnimationPartDefinition::BoneGroupRefs
	);

	lua.new_usertype<BoneGroupDefinition>("Ember::OgreView::Model::BoneGroupDefinition",
										  "Name", &BoneGroupDefinition::Name,
										  "Bones", &BoneGroupDefinition::Bones
	);

	lua.new_usertype<SoundDefinition>("Ember::OgreView::Model::SoundDefinition",
									  "groupName", &SoundDefinition::groupName,
									  "playOrder", &SoundDefinition::playOrder
	);

	lua.new_usertype<BoneGroupRefDefinition>("Ember::OgreView::Model::BoneGroupRefDefinition",
											 "Name", &BoneGroupRefDefinition::Name,
											 "Weight", &BoneGroupRefDefinition::Weight
	);

	lua.new_usertype<AnimationDefinition>("Ember::OgreView::Model::AnimationDefinition",
										  "getAnimationPartDefinitions", sol::resolve<const std::vector<AnimationPartDefinition>&() const>(&AnimationDefinition::getAnimationPartDefinitions),
										  "removeAnimationPartDefinition", &AnimationDefinition::removeAnimationPartDefinition,
										  "iterations", &AnimationDefinition::iterations
	);

	lua.new_usertype<ActionDefinition>("Ember::OgreView::Model::ActionDefinition",
									   "getAnimationDefinitions", sol::resolve<const std::vector<AnimationDefinition>&() const>(&ActionDefinition::getAnimationDefinitions),
									   "removeAnimationDefinition", &ActionDefinition::removeAnimationDefinition,
									   "getSoundDefinitions", sol::resolve<const std::vector<SoundDefinition>&() const>(&ActionDefinition::getSoundDefinitions),
									   "removeSoundDefinition", &ActionDefinition::removeSoundDefinition,
									   "getActivationDefinitions", sol::resolve<const std::vector<ActivationDefinition>&() const>(&ActionDefinition::getActivationDefinitions),
									   "name", &ActionDefinition::name,
									   "animationSpeed", &ActionDefinition::animationSpeed
	);
	lua.new_usertype<ModelDefinition>("Ember::OgreView::Model::ModelDefinition",
									  "getOrigin", &ModelDefinition::getOrigin,
									  "setOrigin", &ModelDefinition::setOrigin,
									  "isValid", &ModelDefinition::isValid,
									  "setValid", &ModelDefinition::setValid,
									  "getScale", &ModelDefinition::getScale,
									  "setScale", &ModelDefinition::setScale,
									  "getUseScaleOf", &ModelDefinition::getUseScaleOf,
									  "setUseScaleOf", &ModelDefinition::setUseScaleOf,
									  "getTranslate", &ModelDefinition::getTranslate,
									  "setTranslate", &ModelDefinition::setTranslate,
									  "getContentOffset", &ModelDefinition::getContentOffset,
									  "setContentOffset", &ModelDefinition::setContentOffset,
									  "getRotation", &ModelDefinition::getRotation,
									  "setRotation", &ModelDefinition::setRotation,
									  "getSubModelDefinitions", &ModelDefinition::getSubModelDefinitions,
									  "removeSubModelDefinition", &ModelDefinition::removeSubModelDefinition,
									  "getActionDefinitions", &ModelDefinition::getActionDefinitions,
									  "removeActionDefinition", &ModelDefinition::removeActionDefinition,
									  "getAttachPointsDefinitions", &ModelDefinition::getAttachPointsDefinitions,
									  "addAttachPointDefinition", &ModelDefinition::addAttachPointDefinition,
									  "getViewDefinitions", &ModelDefinition::getViewDefinitions,
									  "removeViewDefinition", &ModelDefinition::removeViewDefinition,
									  "getPoseDefinitions", &ModelDefinition::getPoseDefinitions,
									  "addPoseDefinition", &ModelDefinition::addPoseDefinition,
									  "removePoseDefinition", &ModelDefinition::removePoseDefinition,
									  "reloadAllInstances", &ModelDefinition::reloadAllInstances
	);

	lua.new_usertype<ModelDefinitionAtlasComposer>("Ember::OgreView::Model::ModelDefinitionAtlasComposer",
												   "compose", &ModelDefinitionAtlasComposer::compose,
												   "composeToStream", &ModelDefinitionAtlasComposer::composeToStream,
												   "composeToFile", &ModelDefinitionAtlasComposer::composeToFile
	);

	lua.new_usertype<ModelDefinitionManager>("Ember::OgreView::Model::ModelDefinitionManager",
											 "getSingleton", &ModelDefinitionManager::getSingleton,
											 "exportScript", &ModelDefinitionManager::exportScript,
											 "addDefinition", &ModelDefinitionManager::addDefinition,
											 "getByName", &ModelDefinitionManager::getByName,
											 "getAllMeshes", &ModelDefinitionManager::getAllMeshes,
											 "getShowModels", &ModelDefinitionManager::getShowModels,
											 "setShowModels", &ModelDefinitionManager::setShowModels
	);
	lua.new_usertype<ModelEditHelper>("Ember::OgreView::Gui::ModelEditHelper",
									  sol::constructors<ModelEditHelper(Ember::OgreView::Model::Model*, SimpleRenderContext&)>(),
									  "showAttachPointHelperEntity", sol::overload([](ModelEditHelper* self, const std::string& attachPointName, const std::string& meshName) { return self->showAttachPointHelperEntity(attachPointName, meshName); },
																				   [](ModelEditHelper* self, const std::string& attachPointName) { return self->showAttachPointHelperEntity(attachPointName); }),
									  "showAttachPointHelperModel", &ModelEditHelper::showAttachPointHelperModel,
									  "hideAttachPointHelper", &ModelEditHelper::hideAttachPointHelper,
									  "startInputRotate", &ModelEditHelper::startInputRotate,
									  "startInputTranslate", &ModelEditHelper::startInputTranslate
	);

	lua.new_usertype<ModelRenderer>("Ember::OgreView::Gui::ModelRenderer",
									sol::constructors<ModelRenderer(CEGUI::Window* image, const std::string&)>(),
									"showModel", sol::overload(sol::resolve<const std::string&, const Ogre::Vector3&, const Ogre::Quaternion&>(&ModelRenderer::showModel),
															   sol::resolve<const ModelDefinitionPtr&, const Ogre::Vector3&, const Ogre::Quaternion&>(&ModelRenderer::showModel)),
									"getModel", &ModelRenderer::getModel,
									"repositionSceneNode", &ModelRenderer::repositionSceneNode,
									sol::base_classes, sol::bases<MovableObjectRenderer>()
	);

	lua.new_usertype<ModelRepresentation>("Ember::OgreView::ModelRepresentation",
										  "getTypeNameForClass", &ModelRepresentation::getTypeNameForClass,
										  "getEntity", &ModelRepresentation::getEntity,
										  "getModel", &ModelRepresentation::getModel,
										  "getModelForEntity", &ModelRepresentation::getModelForEntity,
										  "getRepresentationForEntity", &ModelRepresentation::getRepresentationForEntity
	);
	auto motionManager = lua.new_usertype<MotionManager>("Ember::OgreView::MotionManager",
														 "getInfo", &MotionManager::getInfo
	);

	motionManager["MotionManagerInfo"] = lua.new_usertype<MotionManager::MotionManagerInfo>("Ember::OgreView::MotionManager::MotionManagerInfo",
																							"AnimatedEntities", &MotionManager::MotionManagerInfo::AnimatedEntities,
																							"MovingEntities", &MotionManager::MotionManagerInfo::MovingEntities
	);

	lua.new_usertype<EntityPickResult>("Ember::OgreView::EntityPickResult",
									   "entity", &EntityPickResult::entity,
									   "position", &EntityPickResult::position,
									   "distance", &EntityPickResult::distance
	);
//	auto mousePicker = lua.new_usertype<MousePicker>("Ember::OgreView::MousePicker",
//													 "doMousePicking", &MousePicker::doMousePicking,
//													 "getLastPickedEntity", &MousePicker::getLastPickedEntity,
//													 "EventPickedEntity",
//													 sol::property([](MousePicker* self) {
//														 return Connector::makeConnector<RefValueAdapter<const Ember::OgreView::EntityPickResult>, RefValueAdapter<const Ember::OgreView::MousePickerArgs>>(self->EventPickedEntity);
//													 }),
//													 "EventPickedNothing",
//													 sol::property([](MousePicker* self) {
//														 return Connector::makeConnector<RefValueAdapter<const Ember::OgreView::MousePickerArgs>>(self->EventPickedNothing);
//													 })
//	);
//
//	mousePicker["ClickMasks"] = lua.create_table_with(
//			"CM_AVATAR", MousePicker::CM_AVATAR,
//			"CM_ENTITY", MousePicker::CM_ENTITY,
//			"CM_NATURE", MousePicker::CM_NATURE,
//			"CM_UNDEFINED", MousePicker::CM_UNDEFINED
//	);
	lua.new_usertype<MovableObjectRenderer>("Ember::OgreView::Gui::MovableObjectRenderer",
											"showFull", &MovableObjectRenderer::showFull,
											"setCameraDistance", &MovableObjectRenderer::setCameraDistance,
											"getCameraDistance", &MovableObjectRenderer::getCameraDistance,
											"getAbsoluteCameraDistance", &MovableObjectRenderer::getAbsoluteCameraDistance,
											"getCameraOrientation", &MovableObjectRenderer::getCameraOrientation,
											"setAutoShowFull", &MovableObjectRenderer::setAutoShowFull,
											"getAutoShowFull", &MovableObjectRenderer::getAutoShowFull,
											"getActive", &MovableObjectRenderer::getActive,
											"setActive", &MovableObjectRenderer::setActive,
											"getEntityRotation", &MovableObjectRenderer::getEntityRotation,
											"setEntityRotation", &MovableObjectRenderer::setEntityRotation,
											"getEntityTranslation", &MovableObjectRenderer::getEntityTranslation,
											"setEntityTranslation", &MovableObjectRenderer::setEntityTranslation,
											"resetCameraOrientation", &MovableObjectRenderer::resetCameraOrientation,
											"pitchCamera", &MovableObjectRenderer::pitchCamera,
											"yawCamera", &MovableObjectRenderer::yawCamera,
											"rollCamera", &MovableObjectRenderer::rollCamera,
											"updateRender", &MovableObjectRenderer::updateRender,
											"showAxis", &MovableObjectRenderer::showAxis,
											"hideAxis", &MovableObjectRenderer::hideAxis,
											"getCameraPositionMode", &MovableObjectRenderer::getCameraPositionMode,
											"setCameraPositionMode", &MovableObjectRenderer::setCameraPositionMode,
											"getEntityTexture", &MovableObjectRenderer::getEntityTexture,
											"setShowMovingLight", &MovableObjectRenderer::setShowMovingLight

	);
	auto movementControllerMode = lua.new_usertype<MovementControllerMode>("Ember::OgreView::MovementControllerMode");
	movementControllerMode["Mode"] = lua.create_table_with(
			"MM_WALK", MovementControllerMode::MM_WALK,
			"MM_RUN", MovementControllerMode::MM_RUN
	);
	lua.new_usertype<MovementController>("Ember::OgreView::MovementController",
										 "moveToPoint", &MovementController::moveToPoint,
										 "teleportTo", &MovementController::teleportTo,
										 "getMode", &MovementController::getMode,
										 "EventMovementModeChanged",
										 sol::property([](MovementController* self) {
											 return Connector::makeConnector<NumberValueAdapter<MovementControllerMode::Mode>>(self->EventMovementModeChanged);
										 })
	);
	lua.new_usertype<OgreEntityRenderer>("Ember::OgreView::Gui::OgreEntityRenderer",
										 sol::constructors<OgreEntityRenderer(CEGUI::Window*)>(),
										 "showEntity", &OgreEntityRenderer::showEntity,
										 "unloadEntity", &OgreEntityRenderer::unloadEntity,
										 "getEntity", &OgreEntityRenderer::getEntity,
										 "setAutoShowFull", &OgreEntityRenderer::setAutoShowFull,
										 "getWireframeMode", &OgreEntityRenderer::getWireframeMode,
										 "setWireframeMode", &OgreEntityRenderer::setWireframeMode,
										 "setForcedLodLevel", &OgreEntityRenderer::setForcedLodLevel,
										 "clearForcedLodLevel", &OgreEntityRenderer::clearForcedLodLevel,
										 "enableAnimation", &OgreEntityRenderer::enableAnimation,
										 "getEntityAnimationNames", &OgreEntityRenderer::getEntityAnimationNames,
										 "setShowSkeleton", &OgreEntityRenderer::setShowSkeleton,
										 sol::base_classes, sol::bases<MovableObjectRenderer>()
	);
	lua.new_usertype<OgreInfo>("Ember::OgreView::OgreInfo",
							   "isIndirect", &OgreInfo::isIndirect,
							   "createUniqueResourceName", &OgreInfo::createUniqueResourceName
	);

	lua.new_usertype<PMInjectorSignaler>("Ember::OgreView::Lod::PMInjectorSignaler",
										 "LodInjected",
										 sol::property([](PMInjectorSignaler* self) {
											 return Connector::makeConnector<PtrValueAdapter<Ogre::LodConfig>>(self->LodInjected);
										 })
	);
	lua.new_usertype<QuaternionAdapter>("Ember::OgreView::Gui::QuaternionAdapter",
										sol::constructors<QuaternionAdapter(CEGUI::Window*, CEGUI::Window*, CEGUI::Window*, CEGUI::Window*)>(),
										"getValue", &QuaternionAdapter::getValue,
										"getOriginalValue", &QuaternionAdapter::getOriginalValue,
										"setValue", &QuaternionAdapter::setValue,
										"updateGui", &QuaternionAdapter::updateGui,
										"EventValueChanged",
										sol::property([](QuaternionAdapter* self) {
											return Connector::makeConnector(self->EventValueChanged);
										})
	);
	lua.new_usertype<HelpMessage>("Ember::OgreView::Gui::HelpMessage",
								  sol::constructors<HelpMessage(std::string, std::string, const std::string&, std::string)>(),
								  "mTitle", &HelpMessage::mTitle,
								  "mMessage", &HelpMessage::mMessage,
								  "mTags", &HelpMessage::mTags,
								  "mId", &HelpMessage::mId,
								  "getTags", &HelpMessage::getTags
	);

	lua.new_usertype<QuickHelp>("Ember::OgreView::Gui::QuickHelp",
								"getSingleton", &QuickHelp::getSingleton,
								"updateText", &QuickHelp::updateText,
								"showWidget", &QuickHelp::showWidget,
								"EventToggleWidgetVisibility",
								sol::property([](QuickHelp* self) {
									return Connector::makeConnector(self->EventToggleWidgetVisibility);
								})
	);

	lua.new_usertype<QuickHelpCursor>("Ember::OgreView::Gui::QuickHelpCursor",
									  "nextMessage", &QuickHelpCursor::nextMessage,
									  "previousMessage", &QuickHelpCursor::previousMessage,
									  "getSize", &QuickHelpCursor::getSize,
									  "getCursorLocation", &QuickHelpCursor::getCursorLocation,
									  "EventUpdateText",
									  sol::property([](QuickHelpCursor* self) {
										  return Connector::makeConnector<RefValueAdapter<const Ember::OgreView::Gui::HelpMessage>>(self->EventUpdateText);
									  })
	);

	lua.new_usertype<ResourceListAdapter>("Ember::OgreView::Gui::Adapters::Ogre::ResourceListAdapter",
										  sol::constructors<ResourceListAdapter(Ember::OgreView::Gui::ListHolder*, Ogre::ResourceManager*)>(),
										  "update", &ResourceListAdapter::update
	);

	lua.new_usertype<RuleEditor>("Ember::OgreView::Authoring::RuleEditor",
								 sol::constructors<RuleEditor(Eris::Avatar&)>(),
								 "updateOrCreateRule", &RuleEditor::updateOrCreateRule,
								 "EventRuleCreated",
								 sol::property([](RuleEditor* self) {
									 return Connector::makeConnector<NumberValueAdapter<long>>(self->EventRuleCreated);
								 }),
								 "EventRuleUpdated",
								 sol::property([](RuleEditor* self) {
									 return Connector::makeConnector<NumberValueAdapter<long>>(self->EventRuleUpdated);
								 }),
								 "EventRuleEditError",
								 sol::property([](RuleEditor* self) {
									 return Connector::makeConnector<NumberValueAdapter<long>>(self->EventRuleEditError);
								 })
	);

	lua.new_usertype<RuleTreeAdapter>("Ember::OgreView::Gui::Adapters::Eris::RuleTreeAdapter",
									  sol::constructors<RuleTreeAdapter(Eris::Connection&, std::string, CEGUI::Tree&)>(),
									  "refresh", &RuleTreeAdapter::refresh,
									  "getSelectedRule", &RuleTreeAdapter::getSelectedRule,
									  "EventNewRuleReceived",
									  sol::property([](RuleTreeAdapter* self) {
										  return Connector::makeConnector<NumberValueAdapter<int>>(self->EventNewRuleReceived);
									  }),
									  "EventAllRulesReceived",
									  sol::property([](RuleTreeAdapter* self) {
										  return Connector::makeConnector(self->EventAllRulesReceived);
									  })
	);
	lua.new_usertype<Scene>("Ember::OgreView::Scene",
							"getSceneManager", &Scene::getSceneManager,
							"getMainCamera", &Scene::getMainCamera
	);
	lua.new_usertype<Screen>("Ember::OgreView::Screen",
							 "toggleRenderMode", &Screen::toggleRenderMode,
							 "takeScreenshot", &Screen::takeScreenshot,
							 "getFrameStats", &Screen::getFrameStats
	);

	auto simpleRenderContext = lua.new_usertype<SimpleRenderContext>("Ember::OgreView::SimpleRenderContext",
																	 sol::constructors<SimpleRenderContext(const std::string&, int, int), SimpleRenderContext(const std::string&, Ogre::TexturePtr)>(),
																	 "getSceneNode", &SimpleRenderContext::getSceneNode,
																	 "getCamera", &SimpleRenderContext::getCamera,
																	 "getDefaultCameraDistance", &SimpleRenderContext::getDefaultCameraDistance,
																	 "setActive", &SimpleRenderContext::setActive,
																	 "repositionCamera", &SimpleRenderContext::repositionCamera,
																	 "pitchCamera", &SimpleRenderContext::pitchCamera,
																	 "yawCamera", &SimpleRenderContext::yawCamera,
																	 "rollCamera", &SimpleRenderContext::rollCamera,
																	 "setCameraDistance", &SimpleRenderContext::setCameraDistance,
																	 "getCameraDistance", &SimpleRenderContext::getCameraDistance,
																	 "getAbsoluteCameraDistance", &SimpleRenderContext::getAbsoluteCameraDistance,
																	 "getCameraOrientation", &SimpleRenderContext::getCameraOrientation,
																	 "resetCameraOrientation", &SimpleRenderContext::resetCameraOrientation,
																	 "setCameraPositionMode", &SimpleRenderContext::setCameraPositionMode
	);
	simpleRenderContext["CPM_OBJECTCENTER"] = SimpleRenderContext::CPM_OBJECTCENTER;
	simpleRenderContext["CPM_WORLDCENTER"] = SimpleRenderContext::CPM_WORLDCENTER;
	simpleRenderContext["CPM_FREE"] = SimpleRenderContext::CPM_FREE;

	lua.new_usertype<SubModel>("Ember::OgreView::Model::SubModel",
							   "getEntity", &SubModel::getEntity
	);
	lua.new_usertype<BasePointUserObject>("Ember::OgreView::Terrain::BasePointUserObject",
										  "getBasePoint", &BasePointUserObject::getBasePoint,
										  "getBasePointMarkerNode", &BasePointUserObject::getBasePointMarkerNode,
										  "getPosition", &BasePointUserObject::getPosition,
										  "getHeight", &BasePointUserObject::getHeight,
										  "translate", &BasePointUserObject::translate,
										  "getRoughness", &BasePointUserObject::getRoughness,
										  "getFalloff", &BasePointUserObject::getFalloff,
										  "EventUpdatedPosition",
										  sol::property([](BasePointUserObject* self) {
											  return Connector::makeConnector(self->EventUpdatedPosition);
										  })
	);

	lua.new_usertype<TerrainEditBasePointMovement>("Ember::OgreView::Terrain::TerrainEditBasePointMovement",
												   "mVerticalMovement", &TerrainEditBasePointMovement::mVerticalMovement,
												   "mPosition", &TerrainEditBasePointMovement::mPosition

	);

	lua.new_usertype<TerrainEditor>("Ember::OgreView::Terrain::TerrainEditor",
									sol::constructors<TerrainEditor(Ember::OgreView::Terrain::TerrainManager&, Ember::OgreView::Camera::MainCamera&)>(),
									"showOverlay", &TerrainEditor::showOverlay,
									"hideOverlay", &TerrainEditor::hideOverlay,
									"isOverlayShown", &TerrainEditor::isOverlayShown,
									"getRadius", &TerrainEditor::getRadius,
									"setRadius", &TerrainEditor::setRadius,
									"getFalloff", &TerrainEditor::getFalloff,
									"setFalloff", &TerrainEditor::setFalloff,
									"EventOverlayCreated",
									sol::property([](TerrainEditor* self) {
										return Connector::makeConnector<RefValueAdapter<Ember::OgreView::Terrain::TerrainEditorOverlay>>(self->EventOverlayCreated);
									}),
									"EventOverlayDestroyed",
									sol::property([](TerrainEditor* self) {
										return Connector::makeConnector(self->EventOverlayDestroyed);
									})
	);

	lua.new_usertype<TerrainEditorOverlay>("Ember::OgreView::Terrain::TerrainEditorOverlay",
										   "commitAction", &TerrainEditorOverlay::commitAction,
										   "getCurrentBasePointUserObject", &TerrainEditorOverlay::getCurrentBasePointUserObject,
										   "sendChangesToServer", &TerrainEditorOverlay::sendChangesToServer,
										   "createAction", &TerrainEditorOverlay::createAction,
										   "undoLastAction", &TerrainEditorOverlay::undoLastAction,
										   "redoAction", &TerrainEditorOverlay::redoAction,
										   "setRoughness", &TerrainEditorOverlay::setRoughness,
										   "setFalloff", &TerrainEditorOverlay::setFalloff,
										   "EventPickedBasePoint",
										   sol::property([](TerrainEditorOverlay* self) {
											   return Connector::makeConnector<PtrValueAdapter<Ember::OgreView::Terrain::BasePointUserObject>>(self->EventPickedBasePoint);
										   }),
										   "EventActionCreated",
										   sol::property([](TerrainEditorOverlay* self) {
											   return Connector::makeConnector<PtrValueAdapter<const Ember::OgreView::Terrain::TerrainEditAction>>(self->EventActionCreated);
										   }),
										   "EventSelectedBasePointUpdatedPosition",
										   sol::property([](TerrainEditorOverlay* self) {
											   return Connector::makeConnector<PtrValueAdapter<Ember::OgreView::Terrain::BasePointUserObject>>(self->EventSelectedBasePointUpdatedPosition);
										   })
	);

	lua.new_usertype<TerrainHandler>("Ember::OgreView::Terrain::TerrainHandler",
									 "getMax", &TerrainHandler::getMax,
									 "getMin", &TerrainHandler::getMin,
									 "getPageMetersSize", &TerrainHandler::getPageMetersSize
	);
	lua.new_usertype<TerrainLayerDefinition>("Ember::OgreView::Terrain::TerrainLayerDefinition",
											 "mAreaId", &TerrainLayerDefinition::mAreaId,
											 "mName", &TerrainLayerDefinition::mName,
											 "mDiffuseTextureName", &TerrainLayerDefinition::mDiffuseTextureName,
											 "mNormalMapTextureName", &TerrainLayerDefinition::mNormalMapTextureName,
											 "mShaderName", &TerrainLayerDefinition::mShaderName,
											 "mTileSize", &TerrainLayerDefinition::mTileSize
	);

	lua.new_usertype<TerrainLayerDefinitionManager>("Ember::OgreView::Terrain::TerrainLayerDefinitionManager",
													"getSingleton", &TerrainLayerDefinitionManager::getSingleton,
													"getDefinitions", &TerrainLayerDefinitionManager::getDefinitions,
													"getDefinitionForArea", &TerrainLayerDefinitionManager::getDefinitionForArea,
													"getDefinitionForShader", &TerrainLayerDefinitionManager::getDefinitionForShader
	);

	lua.new_usertype<TerrainManager>("Ember::OgreView::Terrain::TerrainManager",
									 "getHandler", &TerrainManager::getHandler,
									 "getTerrainAdapter", &TerrainManager::getTerrainAdapter,
									 "getScene", &TerrainManager::getScene,
									 "EventTerrainPageGeometryUpdated",
									 sol::property([](TerrainManager* self) {
										 return Connector::makeConnector<RefValueAdapter<TerrainPage>>(self->EventTerrainPageGeometryUpdated);
									 })
	);

	lua.new_usertype<Vector3Adapter>("Ember::OgreView::Gui::Vector3Adapter",
									 sol::constructors<Vector3Adapter(CEGUI::Window*, CEGUI::Window*, CEGUI::Window*)>(),
									 "getValue", &Vector3Adapter::getValue,
									 "getOriginalValue", &Vector3Adapter::getOriginalValue,
									 "setValue", &Vector3Adapter::setValue,
									 "updateGui", &Vector3Adapter::updateGui,
									 "EventValueChanged",
									 sol::property([](Vector3Adapter* self) {
										 return Connector::makeConnector(self->EventValueChanged);
									 })
	);

	lua.new_usertype<Widget>("Ember::OgreView::Gui::Widget",
							 "show", &Widget::show,
							 "hide", &Widget::hide,
							 "isVisible", &Widget::isVisible,
							 "isActive", &Widget::isActive,
							 "getWindow", [](Widget* self, const std::string& name) { return &self->getWindow<CEGUI::Window>(name); },
							 "enableCloseButton", &Widget::enableCloseButton,
							 "registerConsoleVisibilityToggleCommand", &Widget::registerConsoleVisibilityToggleCommand,
							 "getMainSheet", &Widget::getMainSheet,
							 "getMainWindow", &Widget::getMainWindow,
							 "createWindow", sol::resolve<CEGUI::Window*(const std::string&)>(&Widget::createWindow),
							 "loadMainSheet", &Widget::loadMainSheet,
							 "getPrefix", &Widget::getPrefix,
							 "getIsActiveWindowOpaque", &Widget::getIsActiveWindowOpaque,
							 "setIsActiveWindowOpaque", &Widget::setIsActiveWindowOpaque,
							 "getDefaultScheme", &Widget::getDefaultScheme,
							 "addTabbableWindow", &Widget::addTabbableWindow,
							 "closeTabGroup", &Widget::closeTabGroup,
							 "EventFrameStarted",
							 sol::property([](Widget* self) {
								 return Connector::makeConnector<NumberValueAdapter<float>>(self->EventFrameStarted);
							 }),
							 "EventFirstTimeShown",
							 sol::property([](Widget* self) {
								 return Connector::makeConnector(self->EventFirstTimeShown);
							 })
	);

	lua.new_usertype<World>("Ember::OgreView::World",
							"getSceneManager", &World::getSceneManager,
							"getView", &World::getView,
							"getAvatar", &World::getAvatar,
							"getMotionManager", &World::getMotionManager,
							"getMainCamera", &World::getMainCamera,
							"getMovementController", &World::getMovementController,
							"getMoveManager", &World::getMoveManager,
							"getEmberEntity", &World::getEmberEntity,
							"getEntityPickListener", &World::getEntityPickListener,
							"getAuthoringManager", &World::getAuthoringManager,
							"getEnvironment", &World::getEnvironment,
							"getTerrainManager", &World::getTerrainManager,
							"EventGotAvatar",
							sol::property([](World* self) {
								return Connector::makeConnector(self->EventGotAvatar);
							})
	);


}
