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

		state->showConsoleWrapper = std::make_unique<ConsoleCommandWrapper>(ConsoleBackend::getSingleton(), "show_entityCreator2", [=, &guiManager](const std::string& command, const std::string& args) {
			state->instance = std::make_unique<Gui::EntityCreatorWidget>(guiManager, *avatar);
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
		mEntityCreator(std::make_unique<EntityCreator>(*EmberOgre::getSingleton().getWorld())) {
	buildWidget();
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

		auto& createButton = mWidget->getWindow<CEGUI::PushButton>("Create");

		createButton.subscribeEvent(CEGUI::PushButton::EventClicked, [this]() {
			if (mEntityRecipeInstance) {
				std::map<std::string, Atlas::Message::Element> adapterValues;
				for (auto& entry: mAdapters) {
					adapterValues.emplace(entry.first, entry.second.adapter->getValue());
				}

				mEntityCreator->startCreation(adapterValues);
			}
		});

		auto& randomizeOrientationWidget = mWidget->getWindow<CEGUI::ToggleButton>("RandomizeOrientation");
		auto toggleRandomizeOrientationFn = [&]() {
			mEntityCreator->setRandomizeOrientation(randomizeOrientationWidget.isSelected());
		};
		randomizeOrientationWidget.subscribeEvent(CEGUI::ToggleButton::EventSelectStateChanged, toggleRandomizeOrientationFn);
		toggleRandomizeOrientationFn();
		auto& plantedOnGroundWidget = mWidget->getWindow<CEGUI::ToggleButton>("PlantedOnGround");
		auto togglePlantedOnGroundFn = [&]() {
			mEntityCreator->setPlantedOnGround(plantedOnGroundWidget.isSelected());
		};
		plantedOnGroundWidget.subscribeEvent(CEGUI::ToggleButton::EventSelectStateChanged, togglePlantedOnGroundFn);
		togglePlantedOnGroundFn();


		mWidget->show();
		auto& listbox = mWidget->getWindow<CEGUI::Listbox>("RecipesList");

		mListHolder = std::make_unique<ListHolder>(listbox, &mWidget->getWindow<CEGUI::Editbox>("RuleFilter"));
		mListAdapter = std::make_unique<Adapters::StringListAdapter>(*mListHolder);

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
		mModelPreviewRenderer->showModel(model);
		mModelPreviewRenderer->showFull();
		mModelPreviewRenderer->setCameraPositionMode(SimpleRenderContext::CPM_OBJECTCENTER);
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
	auto& typeService = mAvatar.getConnection().getTypeService();
	mEntityRecipeInstance = std::make_unique<Authoring::EntityRecipeInstance>(*recipe);
	mEntityRecipe = recipe;
	mEntityCreator->setRecipeInstance(mEntityRecipeInstance.get());

	auto& adaptersContainer = mWidget->getWindow<CEGUI::Window>("AdaptersContainer");
	auto& windowManager = CEGUI::WindowManager::getSingleton();

	mAdapters.clear();
	while (adaptersContainer.getChildCount()) {
		adaptersContainer.destroyChild(adaptersContainer.getChildAtIdx(0));
	}
	adaptersContainer.invalidate(true);

	for (auto& entry: mEntityRecipe->getGUIAdapters()) {
		auto& guiAdapter = entry.second;
		auto wrapper = windowManager.createWindow("HorizontalLayoutContainer");
		auto title = windowManager.createWindow("EmberLook/StaticText");
		title->setText(guiAdapter->getTitle());
		title->setWidth({0, 75});
		title->setHeight({0, 25});
		auto adapterContainer = windowManager.createWindow("DefaultWindow");
		auto adapter = entry.second->attach(adapterContainer);
		adapter->EventValueChanged.connect([this]() { refreshPreview(); });
		mAdapters.emplace(entry.first, AdapterPair{std::move(adapter), guiAdapter.get()});
		adapterContainer->setWidth({1, -85});
		wrapper->setHeight({0, adapterContainer->getHeight().d_offset});
		wrapper->addChild(title);
		wrapper->addChild(adapterContainer);

		adaptersContainer.addChild(wrapper);
	}

	refreshPreview();


}

void EntityCreatorWidget::refreshPreview() {
	auto& typeService = mAvatar.getConnection().getTypeService();
	std::map<std::string, Atlas::Message::Element> adapterValues;
	for (auto& entry: mAdapters) {
		adapterValues.emplace(entry.first, entry.second.adapter->getValue());
	}

	try {
		auto entityMap = mEntityRecipeInstance->createEntity(typeService, adapterValues);
		auto parentI = entityMap.find("parent");
		if (parentI != entityMap.end() && parentI->second.isString()) {
			auto& parent = parentI->second.String();
			Eris::TypeInfo* erisType = typeService.getTypeByName(parent);
			if (erisType && erisType->isBound()) {
				Authoring::DetachedEntity entity("0", erisType);

				entity.setFromMessage(entityMap);
				showPreview(entity);
			}
		}

	} catch (const std::exception& ex) {
		S_LOG_FAILURE("Could not create preview entity." << ex);
	}
}

void EntityCreatorWidget::showType(const std::string& typeName) {

	auto type = mAvatar.getConnection().getTypeService().getTypeByName(typeName);
	if (type && type->isBound()) {
		Authoring::DetachedEntity entity("0", type);
		showPreview(entity);
	}

	auto xml = std::make_unique<TiXmlElement>("entity");
	auto entityMap = xml->InsertEndChild(TiXmlElement("atlas"))->InsertEndChild(TiXmlElement("map"));

	TiXmlElement parentElement("string");
	parentElement.SetAttribute("name", "parent");
	parentElement.InsertEndChild(TiXmlText(typeName));
	entityMap->InsertEndChild(parentElement);


	showRecipe(std::make_shared<Authoring::EntityRecipe>(std::move(xml)));
}

}
}
}