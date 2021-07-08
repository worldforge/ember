/*
 Copyright (C) 2011 Erik Ogenvik

 This program is free software; you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation; either version 2 of the License, or
 (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program; if not, write to the Free Software Foundation,
 Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "TerrainEntityManager.h"
#include "terrain/TerrainHandler.h"
#include "terrain/TerrainArea.h"

#include "terrain/TerrainParser.h"
#include "terrain/TerrainShaderParser.h"
#include "terrain/TerrainDefPoint.h"

#include "domain/EmberEntity.h"
#include "framework/LoggingInstance.h"

#include <OgreSceneManager.h>

#include <Eris/View.h>
#include <Eris/Avatar.h>

#include <Mercator/Area.h>

#include <sigc++/bind.h>

namespace Ember {
namespace OgreView {

/**
 * Listens to entity signal which is of interest to terrain mods and areas.
 */
struct TerrainEffectorListener {

	/**
	 * @brief Emitted when the entity has moved.
	 */
	std::function<void()> EventEntityMoved;

	/**
	 * @brief Emitted when the entity is being deleted.
	 */
	std::function<void()> EventEntityBeingDeleted;

	/**
	 * @brief Emitted when the entity is being deleted.
	 */
	std::function<void()> EventEntityModeChanged;

	std::vector<sigc::connection> connections;

	explicit TerrainEffectorListener(Ember::EmberEntity& entity) {
		connections.emplace_back(entity.Moved.connect([this]() { if (EventEntityMoved) { EventEntityMoved(); }}));
		connections.emplace_back(entity.BeingDeleted.connect([this]() { if (EventEntityBeingDeleted) { EventEntityBeingDeleted(); }}));
		connections.emplace_back(entity.observe("mode", [this](const Atlas::Message::Element&) { if (EventEntityModeChanged) { EventEntityModeChanged(); }}, false));
	}

	~TerrainEffectorListener() {
		for (auto& conn : connections) {
			conn.disconnect();
		}
	}
};


TerrainEntityManager::TerrainEntityManager(Eris::View& view, Terrain::TerrainHandler& terrainHandler, Ogre::SceneManager& sceneManager) :
		mView(view),
		mTerrainHandler(terrainHandler),
		mSceneManager(sceneManager) {

	//TODO: only react to entities that have domain="physical" and their direct children
	mTerrainListener = [&](EmberEntity& entity, const Atlas::Message::Element& element) {
		entityTerrainAttrChanged(entity, element);
	};
	mTerrainPointsListener = [&](EmberEntity& entity, const Atlas::Message::Element& element) {
		entityTerrainPointsAttrChanged(entity, element);
	};
	mTerrainModListener = [&](EmberEntity& entity, const Atlas::Message::Element& element) {
		entityTerrainModAttrChanged(entity, element);
	};
	mTerrainAreaListener = [&](EmberEntity& entity, const Atlas::Message::Element& element) {
		entityAreaAttrChanged(entity, element);
	};

	EmberEntity::registerGlobalAttributeListener("terrain", mTerrainListener);
	EmberEntity::registerGlobalAttributeListener("terrain_points", mTerrainPointsListener);
	EmberEntity::registerGlobalAttributeListener("terrainmod", mTerrainModListener);
	EmberEntity::registerGlobalAttributeListener("area", mTerrainAreaListener);
}

TerrainEntityManager::~TerrainEntityManager() {
	EmberEntity::deregisterGlobalAttributeListener("terrain", mTerrainListener);
	EmberEntity::deregisterGlobalAttributeListener("terrain_points", mTerrainPointsListener);
	EmberEntity::deregisterGlobalAttributeListener("terrainmod", mTerrainModListener);
	EmberEntity::deregisterGlobalAttributeListener("area", mTerrainAreaListener);
}

void TerrainEntityManager::parseTerrainAttribute(EmberEntity& entity, const Atlas::Message::Element& value) {
	if (value.isList()) {
		Terrain::TerrainShaderParser terrainShaderParser(mTerrainHandler);
		terrainShaderParser.createShaders(value.List());
		entity.setHeightProvider(&mTerrainHandler);
		mTerrainHandler.EventTerrainEnabled(entity);
	}
}

void TerrainEntityManager::entityTerrainAttrChanged(EmberEntity& entity, const Atlas::Message::Element& value) {
	//Only apply if the entity is both an ancestor to the avatar entity and has a domain.
	//TODO: for now we only check that it has a domain, because if it's the main parent entity then the avatar entity will not have been bound yet.
//	if (entity.isAncestorTo(*mView.getAvatar().getEntity()) && entity.hasProperty("domain")) {
	if (entity.hasProperty("domain")) {
		if (!mTerrainEntityDeleteConnection) {
			mTerrainEntityDeleteConnection = entity.BeingDeleted.connect([this]() {
				mTerrainHandler.EventTerrainDisabled();
				mTerrainEntityDeleteConnection.disconnect();
				mTerrainEntityVisibilityConnection.disconnect();
			});
		}
		if (!mTerrainEntityVisibilityConnection) {
			mTerrainEntityVisibilityConnection = entity.VisibilityChanged.connect([this, &entity](bool visible) {
				if (!visible) {
					mTerrainHandler.EventTerrainDisabled();
				} else {
					parseTerrainAttribute(entity, entity.valueOfProperty("terrain"));
				}
			});
		}

		if (entity.isVisible()) {
			parseTerrainAttribute(entity, value);
		}
	}

}


void TerrainEntityManager::entityTerrainPointsAttrChanged(EmberEntity& entity, const Atlas::Message::Element& value) {

//	if (entity.isVisible()) {
	if (value.isMap()) {
//		WFMath::Point<3> pos = entity.getPosition();
//		if (pos.isValid()) {
		//For now hard code position to ZERO
		mTerrainHandler.updateTerrain(Terrain::TerrainParser::parseTerrain(value.Map(), {0, 0, 0}));
//		}
	}
//	}
}

void TerrainEntityManager::entityTerrainModAttrChanged(EmberEntity& entity, const Atlas::Message::Element& value) {

	TerrainModEntry* terrainModEntry = nullptr;
	auto I = mTerrainMods.find(&entity);
	if (I == mTerrainMods.end()) {
		if (value.isMap()) {
			try {
				auto translator = std::make_unique<Ember::Terrain::TerrainModTranslator>(value.Map());
				auto translatorPtr = translator.get();
				auto listener = std::make_unique<TerrainEffectorListener>(entity);

				listener->EventEntityBeingDeleted = [this, &entity]() {
					mTerrainHandler.updateMod(entity.getId(), {}, {}, nullptr);
					mTerrainMods.erase(&entity);
				};
				listener->EventEntityMoved = [this, &entity, translatorPtr]() {
					if (entity.getPositioningMode() == EmberEntity::PositioningMode::PLANTED) {
						mTerrainHandler.updateMod(entity.getId(), entity.getPosition(), entity.getOrientation(), std::make_unique<Ember::Terrain::TerrainModTranslator>(*translatorPtr));
					}
				};
				listener->EventEntityModeChanged = [this, &entity, translatorPtr]() { entityModeChanged(entity, *translatorPtr); };
				auto result = mTerrainMods.emplace(&entity, TerrainModEntry{std::move(translator), std::move(listener)});
				if (result.second) {
					terrainModEntry = &result.first->second;
				}
			} catch (const std::exception& ex) {
				S_LOG_FAILURE("Error when paring terrain mod." << ex);
			}
		}
	} else {
		terrainModEntry = &I->second;
		if (value.isMap()) {
			terrainModEntry->translator = std::make_unique<Ember::Terrain::TerrainModTranslator>(value.Map());
		} else {
			terrainModEntry->translator.reset();
		}
	}

	if (terrainModEntry) {
		if (entity.getPositioningMode() == EmberEntity::PositioningMode::PLANTED) {
			mTerrainHandler.updateMod(entity.getId(), entity.getPosition(), entity.getOrientation(), std::make_unique<Ember::Terrain::TerrainModTranslator>(*terrainModEntry->translator));
		}
	} else {
		mTerrainHandler.updateMod(entity.getId(), {}, {}, nullptr);
	}
}

void TerrainEntityManager::entityAreaAttrChanged(EmberEntity& entity, const Atlas::Message::Element& value) {
	Terrain::TerrainArea* terrainArea = nullptr;
	auto I = mAreas.find(&entity);
	if (I == mAreas.end()) {
		auto newTerrainArea = std::make_unique<Terrain::TerrainArea>(entity);
		auto newTerrainAreaPtr = newTerrainArea.get();
		auto listener = std::make_unique<TerrainEffectorListener>(entity);
		listener->EventEntityBeingDeleted = [this, &entity]() { entityBeingDeleted(entity); };
		listener->EventEntityMoved = [this, &entity, newTerrainAreaPtr]() { entityMoved(entity, *newTerrainAreaPtr); };
		listener->EventEntityModeChanged = [this, &entity, newTerrainAreaPtr]() { entityModeChanged(entity, *newTerrainAreaPtr); };
		auto result = mAreas.emplace(&entity, std::make_pair(std::move(newTerrainArea), std::move(listener)));
		if (result.second) {
			terrainArea = result.first->second.first.get();
		} else {
			return;
		}
	} else {
		terrainArea = I->second.first.get();
	}

	auto area = terrainArea->parse(value);
	//Only add area if we're planted
	if (entity.getPositioningMode() == EmberEntity::PositioningMode::PLANTED) {
		mTerrainHandler.updateArea(entity.getId(), std::move(area));
	}
}

void TerrainEntityManager::entityBeingDeleted(EmberEntity& entity) {

	mTerrainHandler.updateArea(entity.getId(), nullptr);
	mAreas.erase(&entity);

}

void TerrainEntityManager::entityMoved(EmberEntity& entity, Terrain::TerrainArea& terrainArea) {

	if (entity.getPositioningMode() == EmberEntity::PositioningMode::PLANTED) {
		auto area = terrainArea.updatePosition();
		mTerrainHandler.updateArea(entity.getId(), std::move(area));
	}
}

void TerrainEntityManager::entityModeChanged(EmberEntity& entity, Terrain::TerrainArea& terrainArea) {
	//If mode changes to "planted" we should add the area, if moves to any other we should remove it.
	if (entity.getPositioningMode() == EmberEntity::PositioningMode::PLANTED) {
		auto area = terrainArea.updatePosition();
		mTerrainHandler.updateArea(entity.getId(), std::move(area));
	} else {
		mTerrainHandler.updateArea(entity.getId(), nullptr);
	}
}

void TerrainEntityManager::entityModeChanged(EmberEntity& entity, const Ember::Terrain::TerrainModTranslator& translator) {
	//If mode changes to "planted" we should add the mod, if moves to any other we should remove it.
	if (entity.getPositioningMode() == EmberEntity::PositioningMode::PLANTED) {
		mTerrainHandler.updateMod(entity.getId(), entity.getPosition(), entity.getOrientation(), std::make_unique<Ember::Terrain::TerrainModTranslator>(translator));
	} else {
		mTerrainHandler.updateMod(entity.getId(), {}, {}, nullptr);
	}
}
}
}
