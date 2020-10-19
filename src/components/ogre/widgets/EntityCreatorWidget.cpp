/*
 Copyright (C) 2020 Erik Ogenvik

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

#include "services/EmberServices.h"
#include "services/server/ServerService.h"
#include "EntityCreatorWidget.h"
#include "framework/ConsoleBackend.h"
#include "../EmberOgre.h"
#include "components/ogre/GUIManager.h"
#include "components/ogre/authoring/EntityRecipeManager.h"
#include "ModelRenderer.h"
#include "EntityTextureManipulator.h"
#include "EntityCreatorMovement.h"
#include "EntityCreatorMovementBridge.h"
#include <components/ogre/mapping/ModelActionCreator.h>
#include <components/ogre/widgets/adapters/atlas/AdapterFactory.h>
#include "components/ogre/mapping/EmberEntityMappingManager.h"
#include "components/ogre/model/Model.h"
#include "components/ogre/World.h"
#include "AtlasHelper.h"

#include <Eris/Avatar.h>
#include <Eris/Account.h>
#include <Eris/TypeService.h>
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/classification.hpp>

#include <wfmath/atlasconv.h>

namespace Ember {
namespace OgreView {
namespace Gui {


WidgetPluginCallback EntityCreatorWidget::registerWidget(GUIManager& guiManager) {
	struct State {
		std::unique_ptr<ConsoleCommandWrapper> showConsoleWrapper;
		std::unique_ptr<Gui::EntityCreatorWidget> instance;
		std::vector<Ember::AutoCloseConnection> connections;
	};
	auto state = std::make_shared<State>();


	auto gotWorldFn = [=, &guiManager](World& world) {

		auto worldDestroyedConn = EmberOgre::getSingleton().EventWorldBeingDestroyed.connect([=]() {
			state->instance.reset();
			state->showConsoleWrapper.reset();
		});
		state->connections.emplace_back(worldDestroyedConn);

		state->showConsoleWrapper = std::make_unique<ConsoleCommandWrapper>(ConsoleBackend::getSingleton(), "show_entityCreator", [=, &world, &guiManager](const std::string& command, const std::string& args) {
			if (!state->instance) {
				state->instance = std::make_unique<Gui::EntityCreatorWidget>(guiManager, world);
			} else {
				state->instance->show();
			}
		});
	};

	auto gotWorldConn = EmberOgre::getSingleton().EventWorldCreated.connect(gotWorldFn);

	state->connections.emplace_back(gotWorldConn);

	if (EmberOgre::getSingleton().getWorld()) {
		gotWorldFn(*EmberOgre::getSingleton().getWorld());
	}


	return [state]() {
		state->connections.clear();
		state->instance.reset();
		state->showConsoleWrapper.reset();
	};
}

EntityCreatorWidget::EntityCreatorWidget(GUIManager& guiManager, World& world) :
		mWorld(world),
		mWidget(guiManager.createWidget()),
		mRulesFetcher(world.getView().getConnection(), world.getView().getAvatar().getId()),
		mUnboundType(nullptr),
		mRandomizeOrientation(true) {
	buildWidget();
	mWidget->enableCloseButton();

	mBoundTypeConnection = world.getView().getConnection().getTypeService().BoundType.connect([this](Eris::TypeInfo* type) {
		if (type == mUnboundType) {
			mUnboundType = nullptr;
			refreshPreview();
		}
	});
	mBadTypeConnection = world.getView().getConnection().getTypeService().BadType.connect([this](Eris::TypeInfo* type) {
		if (type == mUnboundType) {
			mUnboundType = nullptr;
		}
	});
}

EntityCreatorWidget::~EntityCreatorWidget() {
	mListAdapter.reset();
	mListHolder.reset();
	mWidget->getGUIManager().removeWidget(mWidget);
}

void EntityCreatorWidget::buildWidget() {
	if (mWidget->loadMainSheet("EntityCreator.layout", "EntityCreator2/")) {
		auto& modelPreview = mWidget->getWindow<CEGUI::Window>("TypePreviewImage");
		mModelPreviewRenderer = std::make_unique<ModelRenderer>(&modelPreview, "modelPreview");
		mModelPreviewManipulator = std::make_unique<CameraEntityTextureManipulator>(modelPreview, mModelPreviewRenderer->getEntityTexture());
		auto& modeWidget = mWidget->getWindow<CEGUI::Combobox>("Mode");

		mWidget->getWindow<CEGUI::Window>("CreateSection").setVisible(false);

		auto& createButton = mWidget->getWindow<CEGUI::PushButton>("Create");
		createButton.setEnabled(false);

		auto& randomizeOrientationWidget = mWidget->getWindow<CEGUI::ToggleButton>("RandomizeOrientation");
		auto toggleRandomizeOrientationFn = [&]() {
			mRandomizeOrientation = randomizeOrientationWidget.isSelected();
		};
		randomizeOrientationWidget.subscribeEvent(CEGUI::ToggleButton::EventSelectStateChanged, toggleRandomizeOrientationFn);
		toggleRandomizeOrientationFn();

		mWidget->show();
		auto& listbox = mWidget->getWindow<CEGUI::Listbox>("RecipesList");

		mListHolder = std::make_unique<ListHolder>(listbox, &mWidget->getWindow<CEGUI::Editbox>("RuleFilter"));
		mListAdapter = std::make_unique<Adapters::StringListAdapter>(*mListHolder);

		modeWidget.addItem(new ColouredListItem("planted"));
		modeWidget.addItem(new ColouredListItem("free"));
		modeWidget.addItem(new ColouredListItem("fixed"));
		modeWidget.setItemSelectState((size_t) 0, true);


		auto& posX = mWidget->getWindow<CEGUI::Editbox>("posX");
		auto& posY = mWidget->getWindow<CEGUI::Editbox>("posY");
		auto& posZ = mWidget->getWindow<CEGUI::Editbox>("posZ");


		auto& parentActiveWidget = mWidget->getWindow<CEGUI::Editbox>("ParentActive");
		auto& parentSelectionWidget = mWidget->getWindow<CEGUI::Combobox>("ParentSelection");
		parentSelectionWidget.addItem(new ColouredListItem("at cursor", 0));
		parentSelectionWidget.addItem(new ColouredListItem("manually specified", 1));
		parentSelectionWidget.addItem(new ColouredListItem("in parent entity", 2));
		parentSelectionWidget.setItemSelectState((size_t) 0, true);
		auto selectParentSelection = [&]() {
			if (parentSelectionWidget.getSelectedItem()) {
				if (parentSelectionWidget.getSelectedItem()->getID() == 0) {
					mFixedParentId = {};
					parentActiveWidget.setEnabled(false);
					posX.setEnabled(false);
					posY.setEnabled(false);
					posZ.setEnabled(false);
				} else if (parentSelectionWidget.getSelectedItem()->getID() == 1) {
					mFixedParentId = {};
					parentActiveWidget.setEnabled(true);
					if (mWorld.getView().getAvatar().getEntity()->getLocation()) {
						parentActiveWidget.setText(mWorld.getView().getAvatar().getEntity()->getLocation()->getId());
					} else {
						parentActiveWidget.setText("");
					}
					posX.setEnabled(true);
					posY.setEnabled(true);
					posZ.setEnabled(true);
				} else if (parentSelectionWidget.getSelectedItem()->getID() == 2) {
					parentActiveWidget.setEnabled(false);
					if (mWorld.getView().getAvatar().getEntity()->getLocation()) {
						mFixedParentId = mWorld.getView().getAvatar().getEntity()->getLocation()->getId();
						parentActiveWidget.setText(mWorld.getView().getAvatar().getEntity()->getLocation()->getId());
					} else {
						mFixedParentId = {};
						parentActiveWidget.setText("");
					}
					posX.setEnabled(false);
					posY.setEnabled(false);
					posZ.setEnabled(false);
				}
			}
		};
		parentSelectionWidget.subscribeEvent(CEGUI::Combobox::EventListSelectionAccepted, selectParentSelection);
		selectParentSelection();

		auto createEntitiesFn = [this](WFMath::Point<3> pos, WFMath::Quaternion orientation, boost::optional<float> offset, std::string location, std::string mode) {
			for (auto& entityMap : mEntityMaps) {
				entityMap["loc"] = location;
				if (orientation.isValid()) {
					entityMap["orientation"] = orientation.toAtlas();
				}
				if (pos.isValid()) {
					entityMap["pos"] = pos.toAtlas();
				}
				if (!mode.empty()) {
					entityMap["mode"] = mode;
					if (mode == "planted") {
						entityMap["mode_data"] = Atlas::Message::MapType{{"mode", "planted"},
																		 {"$eid", location}};
						if (offset && *offset != 0.0) {
							entityMap["planted_offset"] = *offset;
						}
					}
				}

				// Making create operation message
				Atlas::Objects::Operation::Create c;
				c->setFrom(mWorld.getView().getAvatar().getId());
				//if the avatar is a "creator", i.e. and admin, we will set the TO property
				//this will bypass all of the server's filtering, allowing us to create any entity and have it have a working mind too
				if (mWorld.getView().getAvatar().getIsAdmin()) {
					c->setTo(mWorld.getView().getAvatar().getEntityId());
				}

				c->setArgsAsList(Atlas::Message::ListType(1, entityMap), &mWorld.getView().getAvatar().getConnection().getFactories());
				mWorld.getView().getAvatar().getConnection().send(c);

				std::stringstream ss;
				ss << pos;
				S_LOG_INFO("Trying to create entity at position " << ss.str());
				S_LOG_VERBOSE("Sending entity data to server: " << AtlasHelper::serialize(c, "xml"));
			}
		};
		mCreateNewEntityFn = [&, createEntitiesFn]() {
			mCreationInstance.reset();

			mCreationInstance = std::make_unique<EntityCreatorCreationInstance>(*EmberOgre::getSingleton().getWorld(), mWorld.getView().getConnection().getTypeService(), mEntityMaps, mRandomizeOrientation);

			mCreationInstance->EventMoved.connect([&](Eris::Entity* parentEntity, const WFMath::Point<3>& pos) {
				if (pos.isValid()) {
					posX.setText(std::to_string(pos.x()));
					posY.setText(std::to_string(pos.y()));
					posZ.setText(std::to_string(pos.z()));
				} else {
					posX.setText("");
					posY.setText("");
					posZ.setText("");
				}

				if (parentEntity) {
					parentActiveWidget.setText(parentEntity->getId() + " - " + parentEntity->getType()->getName());
				}
			});
			mCreationInstance->EventAbortRequested.connect([&]() {
				mLastOrientation = mCreationInstance->getOrientation();
				mCreationInstance.reset();
			});


			mCreationInstance->EventFinalizeRequested.connect([this, createEntitiesFn, &modeWidget]() {
				auto& bridge = mCreationInstance->getMovement()->getBridge();
				auto pos = bridge->getPosition();
				auto parentEntity = bridge->mCollidedEntity;
				auto orientation = bridge->getOrientation();
				auto offset = bridge->getOffset();
				if (parentEntity) {
					createEntitiesFn(pos, orientation, offset, parentEntity->getId(), modeWidget.getText().c_str());
				}

				for (auto& entry : mAdapters) {
					if (entry.second.allowRandom) {
						entry.second.adapter->randomize();
					}
				}
				refreshEntityMap();

				mCreateNewEntityFn();

			});
			if (!mRandomizeOrientation) {
				mCreationInstance->setOrientation(mLastOrientation);
			}
			mCreationInstance->startCreation();
			mCreationInstance->getMovement()->getBridge()->mFixedParentId = mFixedParentId;
		};

		auto& entityRecipeManager = Authoring::EntityRecipeManager::getSingleton();

		auto& description = mWidget->getWindow<CEGUI::Window>("Description");


		createButton.subscribeEvent(CEGUI::PushButton::EventClicked, [this, &parentSelectionWidget, &posX, &posY, &posZ, &parentActiveWidget, &modeWidget, createEntitiesFn]() {
			if (parentSelectionWidget.getSelectedItem()) {
				if (parentSelectionWidget.getSelectedItem()->getID() == 1) {
					//"manual" parent is used, so create the entity right away.
					std::string location = parentActiveWidget.getText().c_str();
					if (location.empty()) {
						//Must at least have location set.
						return;
					}

					WFMath::Point<3> pos;
					try {
						pos = {std::stod(posX.getText().c_str()), std::stod(posY.getText().c_str()), std::stod(posZ.getText().c_str())};
					} catch (...) {
						//just ignore parser errors
					}
					createEntitiesFn(pos, WFMath::Quaternion::IDENTITY(), {}, location, modeWidget.getText().c_str());
				} else {
					mCreateNewEntityFn();
				}
			}
		});


		mListAdapter->EventSelected.connect([&](const std::string& name) {
			std::vector <std::string> pair;
			boost::algorithm::split(pair, name, boost::algorithm::is_any_of(":"));
			if (pair.size() == 2) {
				mWidget->getWindow<CEGUI::Window>("CreateSection").setVisible(true);
				createButton.setEnabled(true);
				if (pair.front() == "recipe") {
					auto I = entityRecipeManager.getEntries().find(pair.back());
					if (I != entityRecipeManager.getEntries().end()) {
						auto recipe = I->second;
						description.setText(recipe->getDescription());
						showRecipe(recipe);
					} else {
						description.setText("");
					}
				} else if (pair.front() == "rule") {
					description.setText("");
					showType(pair.back());
				}
			} else {
				createButton.setEnabled(false);
				mWidget->getWindow<CEGUI::Window>("CreateSection").setVisible(false);
				description.setText("");
			}
		});

		std::vector <std::pair<std::string, std::string>> entries;
		auto& recipes = Authoring::EntityRecipeManager::getSingleton().getEntries();
		entries.reserve(recipes.size());
		entries.emplace_back(std::make_pair("", "Recipes:"));

		for (auto& entry: recipes) {
			entries.emplace_back(std::make_pair("recipe:" + entry.first, " " + entry.first));
		}
		entries.emplace_back(std::make_pair("", "Types:"));

		mRulesFetcher.EventAllRulesReceived.connect([this] {
			mRules = mRulesFetcher.getRules();
			auto I = mRules.find("game_entity");
			if (I != mRules.end()) {
				for (auto& child: I->second.children) {
					auto childI = mRules.find(child);
					if (childI != mRules.end()) {
						addRulesToList(childI->second, 1);
					}
				}
			}
		});
		mRulesFetcher.startFetching("game_entity");


		mListAdapter->add(std::move(entries));
	}
}

void EntityCreatorWidget::showPreview(Ember::OgreView::Authoring::DetachedEntity& entity) {
	Mapping::ModelActionCreator actionCreator(entity, [&](const std::string& model) {
		mModelPreviewRenderer->setCameraPositionMode(SimpleRenderContext::CPM_OBJECTCENTER);
		mModelPreviewRenderer->showModel(model);
		mModelPreviewRenderer->showFull();
		//we want to zoom in a little
		mModelPreviewRenderer->setCameraDistance(0.7);

	}, [&](const std::string& part) {
		if (mModelPreviewRenderer->getModel()) {
			mModelPreviewRenderer->getModel()->showPart(part);
		}
	});

	auto mapping = Mapping::EmberEntityMappingManager::getSingleton().getManager().createMapping(entity, actionCreator, mWorld.getView().getTypeService(), &mWorld.getView());
	if (mapping) {
		mapping->initialize();
	}
}

void EntityCreatorWidget::addRulesToList(const Authoring::RulesFetcher::RuleEntry& entry, int level) {

	auto& rule = entry.rule;
	mListAdapter->add(std::string(level, ' ') + rule->getId(), "rule:" + rule->getId());

	for (auto& child : entry.children) {
		auto I = mRules.find(child);
		if (I != mRules.end()) {
			addRulesToList(I->second, level + 1);
		}
	}
}

void EntityCreatorWidget::showRecipe(const std::shared_ptr<Authoring::EntityRecipe>& recipe) {
	mEntityRecipe = recipe;

	auto& adaptersContainer = mWidget->getWindow<CEGUI::Window>("AdaptersContainer");
	auto& windowManager = CEGUI::WindowManager::getSingleton();

	for (auto& entry: mAdapters) {
		if (entry.second.container) {
			adaptersContainer.destroyChild(entry.second.container);
			entry.second.container = nullptr;
		}
	}
	mAdapters.clear();

	for (auto& entry: mEntityRecipe->getGUIAdapters()) {
		auto& guiAdapter = entry.second;

		auto adapterContainer = windowManager.createWindow("DefaultWindow");
		auto adapter = attachToGuiAdapter(*entry.second, adapterContainer);

		if (adapter) {
			auto wrapper = windowManager.createWindow("HorizontalLayoutContainer");
			auto titleWrapper = windowManager.createWindow("VerticalLayoutContainer");
			auto title = windowManager.createWindow("EmberLook/StaticText");
			title->setText(guiAdapter->mTitle);
			title->setWidth({0, 75});
			title->setHeight({0, 20});
			auto randomCheckbox = dynamic_cast<CEGUI::ToggleButton*>(windowManager.createWindow("EmberLook/Checkbox"));
			randomCheckbox->setText("Random");
			randomCheckbox->setTooltipText("If enabled the adapter will get a random value each time an entity is created.");
			randomCheckbox->setWidth({0, 75});
			randomCheckbox->setHeight({0, 20});
			auto entryName = entry.first;
			randomCheckbox->subscribeEvent(CEGUI::ToggleButton::EventSelectStateChanged, [this, randomCheckbox, entryName]() {
				auto I = mAdapters.find(entryName);
				if (I != mAdapters.end()) {
					I->second.allowRandom = randomCheckbox->isSelected();
				}
			});
			randomCheckbox->setSelected(entry.second->mAllowRandom);
			titleWrapper->addChild(title);
			titleWrapper->addChild(randomCheckbox);
			adapter->EventValueChanged.connect([this]() { refreshEntityMap(); });
			adapterContainer->setWidth({1, -85});
			wrapper->setHeight({0, adapterContainer->getHeight().d_offset});
			wrapper->addChild(titleWrapper);
			wrapper->addChild(adapterContainer);

			adaptersContainer.addChild(wrapper);
			mAdapters.emplace(entry.first, AdapterEntry{std::move(adapter), guiAdapter.get(), entry.second->mAllowRandom, wrapper});
		} else {
			windowManager.destroyWindow(adapterContainer);
		}
	}


	refreshEntityMap();

}

void EntityCreatorWidget::refreshEntityMap() {
	auto& typeService = mWorld.getView().getTypeService();
	std::map<std::string, Atlas::Message::Element> adapterValues;
	for (auto& entry: mAdapters) {
		adapterValues.emplace(entry.first, entry.second.adapter->getValue());
	}

	try {
		mEntityMaps.clear();
		for (auto& entitySpec : mEntityRecipe->getEntitySpecs()) {
			mEntityMaps.emplace_back(Authoring::EntityRecipe::createEntity(typeService, adapterValues, *entitySpec));
		}

	} catch (const std::exception& ex) {
		S_LOG_FAILURE("Could not create preview entity." << ex);
	}

	refreshPreview();

}

void EntityCreatorWidget::refreshPreview() {
	auto& typeService = mWorld.getView().getTypeService();
	try {
		mUnboundType = nullptr;
		if (!mEntityMaps.empty()) {
			auto& firstEntity = mEntityMaps.front();
			auto parentI = firstEntity.find("parent");
			if (parentI != firstEntity.end() && parentI->second.isString()) {
				auto& parent = parentI->second.String();
				Eris::TypeInfo* erisType = typeService.getTypeByName(parent);
				if (erisType) {
					if (erisType->isBound()) {
						Authoring::DetachedEntity entity("0", erisType);
						entity.setFromMessage(firstEntity);
						showPreview(entity);
					} else {
						mUnboundType = erisType;
					}
				}
			}
		}
	} catch (const std::exception& ex) {
		S_LOG_FAILURE("Could not create preview entity." << ex);
	}
}

void EntityCreatorWidget::showType(const std::string& typeName) {

	auto xml = std::make_unique<TiXmlElement>("map");
	TiXmlElement parentElement("string");
	parentElement.SetAttribute("name", "parent");
	parentElement.InsertEndChild(TiXmlText(typeName));
	xml->InsertEndChild(parentElement);

	std::vector<std::unique_ptr<TiXmlElement>> entities;
	entities.emplace_back(std::move(xml));


	showRecipe(std::make_shared<Authoring::EntityRecipe>(std::move(entities)));
}

std::unique_ptr<Gui::Adapters::Atlas::AdapterBase> EntityCreatorWidget::attachToGuiAdapter(Authoring::GUIAdapter& guiAdapter, CEGUI::Window* window) {
	OgreView::Gui::Adapters::Atlas::AdapterFactory factory("EntityCreator");
	auto adapter = factory.createAdapterByType(guiAdapter.mType, window, "adapterPrefix", guiAdapter.mElement);
	for (auto& suggestion : guiAdapter.mSuggestions) {
		adapter->addSuggestion(suggestion.first);
	}

	//If we have a default value set, use that
	if (!guiAdapter.mDefaultValue.empty()) {
		if (guiAdapter.mType == "string") {
			adapter->updateGui(Atlas::Message::Element(guiAdapter.mDefaultValue));
		} else if (guiAdapter.mType == "number" || guiAdapter.mType == "number_range") {
			adapter->updateGui(Atlas::Message::Element(atof(guiAdapter.mDefaultValue.c_str())));
		}
	}
	return adapter;
}

void EntityCreatorWidget::show() {
	mWidget->show();
}

}
}
}