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

#include <Eris/Avatar.h>
#include <Eris/Account.h>
#include <Eris/TypeService.h>
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/classification.hpp>
#include <components/ogre/mapping/ModelActionCreator.h>
#include <components/ogre/widgets/adapters/atlas/AdapterFactory.h>
#include "components/ogre/mapping/EmberEntityMappingManager.h"
#include "components/ogre/model/Model.h"

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


	auto gotAvatarFn = [=, &guiManager](Eris::Avatar* avatar) {
		auto deactivatedAvatarConn = avatar->getAccount().AvatarDeactivated.connect([=](const std::string& id) {
			if (avatar->getId() == id) {
				state->instance.reset();
				state->showConsoleWrapper.reset();
			}
		});
		state->connections.emplace_back(deactivatedAvatarConn);

		state->showConsoleWrapper = std::make_unique<ConsoleCommandWrapper>(ConsoleBackend::getSingleton(), "show_entityCreator", [=, &guiManager](const std::string& command, const std::string& args) {
			if (!state->instance) {
				state->instance = std::make_unique<Gui::EntityCreatorWidget>(guiManager, *avatar);
			} else {
				state->instance->show();
			}
		});
	};

	auto gotAvatarConn = EmberServices::getSingleton().getServerService().GotAvatar.connect(gotAvatarFn);
	state->connections.emplace_back(gotAvatarConn);

	if (EmberServices::getSingleton().getServerService().getAvatar()) {
		gotAvatarFn(EmberServices::getSingleton().getServerService().getAvatar());
	}


	return [state]() {
		state->connections.clear();
		state->instance.reset();
		state->showConsoleWrapper.reset();
	};
}

EntityCreatorWidget::EntityCreatorWidget(GUIManager& guiManager, Eris::Avatar& avatar) :
		mAvatar(avatar),
		mWidget(guiManager.createWidget()),
		mRulesFetcher(avatar.getConnection(), avatar.getId()),
		mEntityCreator(std::make_unique<EntityCreator>(*EmberOgre::getSingleton().getWorld())),
		mUnboundType(nullptr) {
	buildWidget();
	mWidget->enableCloseButton();

	mEntityCreator->EventCreationCompleted.connect([this]() {
		for (auto& entry : mAdapters) {
			if (entry.second.allowRandom) {
				entry.second.adapter->randomize();
			}
		}
		refreshEntityMap();

		mEntityCreator->startCreation(mEntityMaps);
	});

	mBoundTypeConnection = avatar.getConnection().getTypeService().BoundType.connect([this](Eris::TypeInfo* type) {
		if (type == mUnboundType) {
			mUnboundType = nullptr;
			refreshPreview();
		}
	});
	mBadTypeConnection = avatar.getConnection().getTypeService().BadType.connect([this](Eris::TypeInfo* type) {
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

		auto& createButton = mWidget->getWindow<CEGUI::PushButton>("Create");

		createButton.subscribeEvent(CEGUI::PushButton::EventClicked, [&]() {
			for (auto& entry: mEntityMaps) {
				entry.emplace("mode", modeWidget.getText().c_str());
			}
			mEntityCreator->startCreation(mEntityMaps);
		});

		auto& randomizeOrientationWidget = mWidget->getWindow<CEGUI::ToggleButton>("RandomizeOrientation");
		auto toggleRandomizeOrientationFn = [&]() {
			mEntityCreator->setRandomizeOrientation(randomizeOrientationWidget.isSelected());
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
		parentSelectionWidget.setItemSelectState((size_t) 0, true);
		auto selectParentSelection = [&]() {
			if (parentSelectionWidget.getSelectedItem()) {
				if (parentSelectionWidget.getSelectedItem()->getID() == 0) {
					parentActiveWidget.setEnabled(false);
					posX.setEnabled(false);
					posY.setEnabled(false);
					posZ.setEnabled(false);
				} else {
					parentActiveWidget.setEnabled(true);
					posX.setEnabled(true);
					posY.setEnabled(true);
					posZ.setEnabled(true);
				}
			}
		};
		parentSelectionWidget.subscribeEvent(CEGUI::Combobox::EventListSelectionAccepted, selectParentSelection);
		selectParentSelection();

		mEntityCreator->EventMoved.connect([&](Eris::Entity* parentEntity, const WFMath::Point<3>& pos) {
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
				parentActiveWidget.setText(parentEntity->getId());
			}
		});

		auto& entityRecipeManager = Authoring::EntityRecipeManager::getSingleton();

		auto& description = mWidget->getWindow<CEGUI::Window>("Description");

		mListAdapter->EventSelected.connect([&](const std::string& name) {
			std::vector<std::string> pair;
			boost::algorithm::split(pair, name, boost::algorithm::is_any_of(":"));
			if (pair.size() == 2) {
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
				description.setText("");
			}
		});

		std::vector<std::pair<std::string, std::string>> entries;
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

	auto mapping = Mapping::EmberEntityMappingManager::getSingleton().getManager().createMapping(entity, actionCreator, mAvatar.getView().getTypeService(), &mAvatar.getView());
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
	auto& typeService = mAvatar.getConnection().getTypeService();
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
	auto& typeService = mAvatar.getConnection().getTypeService();
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