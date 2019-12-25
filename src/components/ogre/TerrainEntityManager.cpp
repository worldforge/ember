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
#include "WorldAttachment.h"
#include "terrain/TerrainHandler.h"
#include "terrain/TerrainMod.h"
#include "terrain/TerrainArea.h"

#include "terrain/TerrainParser.h"
#include "terrain/TerrainShaderParser.h"
#include "terrain/TerrainDefPoint.h"

#include "domain/EmberEntity.h"

#include <OgreSceneManager.h>
#include <OgreSceneNode.h>

#include <Eris/View.h>

#include <Mercator/Area.h>

#include <sigc++/bind.h>

namespace Ember {
namespace OgreView {

/**
 * Listens to entity signal which is of interest to terrain mods and areas.
 */
class TerrainEffectorListener : public sigc::trackable {
public:

	/**
	 * @brief Emitted when the entity has moved.
	 */
	sigc::signal<void> EventEntityMoved;

	/**
	 * @brief Emitted when the entity is being deleted.
	 */
	sigc::signal<void> EventEntityBeingDeleted;

	/**
	 * @brief Emitted when the entity is being deleted.
	 */
	sigc::signal<void> EventEntityModeChanged;

	explicit TerrainEffectorListener(Ember::EmberEntity& entity) {
		entity.Moved.connect([this]() { EventEntityMoved.emit(); });
		entity.BeingDeleted.connect([this]() { EventEntityBeingDeleted.emit(); });
		entity.observe("mode", [&](const Atlas::Message::Element&) { EventEntityModeChanged.emit(); });
	}
};


TerrainEntityManager::TerrainEntityManager(Eris::View& view, Terrain::TerrainHandler& terrainHandler, Ogre::SceneManager& sceneManager) :
		mView(view),
        mTopLevelEntity(nullptr),
		mTerrainHandler(terrainHandler),
		mSceneManager(sceneManager) {
	view.TopLevelEntityChanged.connect(sigc::mem_fun(*this, &TerrainEntityManager::topLevelEntityChanged));

	mTerrainListener = [&](EmberEntity& entity, const Atlas::Message::Element& element) {
		entityTerrainAttrChanged(entity, element);
	};
	mTerrainModListener = [&](EmberEntity& entity, const Atlas::Message::Element& element) {
		entityTerrainModAttrChanged(entity, element);
	};
	mTerrainAreaListener = [&](EmberEntity& entity, const Atlas::Message::Element& element) {
		entityAreaAttrChanged(entity, element);
	};

	EmberEntity::registerGlobalAttributeListener("terrain", mTerrainListener);
	EmberEntity::registerGlobalAttributeListener("terrainmod", mTerrainModListener);
	EmberEntity::registerGlobalAttributeListener("area", mTerrainAreaListener);
}

TerrainEntityManager::~TerrainEntityManager() {
	EmberEntity::deregisterGlobalAttributeListener("terrain", mTerrainListener);
	EmberEntity::deregisterGlobalAttributeListener("terrainmod", mTerrainModListener);
	EmberEntity::deregisterGlobalAttributeListener("area", mTerrainAreaListener);
}

void TerrainEntityManager::topLevelEntityChanged() {
	auto entity = dynamic_cast<EmberEntity*>(mView.getTopLevel());
	if (mTopLevelEntity) {
        mTopLevelEntity->setAttachment({});
	}
	if (entity) {
        entity->setAttachment(std::make_unique<WorldAttachment>(*entity, mSceneManager.getRootSceneNode()->createChildSceneNode("entity_" + entity->getId())));
    }
	mTopLevelEntity = entity;
}

void TerrainEntityManager::parseTerrainAttribute(EmberEntity& entity, const Atlas::Message::Element& value) {
	Terrain::TerrainShaderParser terrainShaderParser(mTerrainHandler);
	terrainShaderParser.createShaders(value);
	Terrain::TerrainParser terrainParser;
	WFMath::Point<3> pos = entity.getPosition().isValid() ? entity.getPredictedPos() : WFMath::Point<3>::ZERO();
	mTerrainHandler.updateTerrain(terrainParser.parseTerrain(value, pos));
	entity.setHeightProvider(&mTerrainHandler);
	mTerrainHandler.EventTerrainEnabled(entity);
}

void TerrainEntityManager::entityTerrainAttrChanged(EmberEntity& entity, const Atlas::Message::Element& value) {
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

void TerrainEntityManager::entityTerrainModAttrChanged(EmberEntity& entity, const Atlas::Message::Element& value) {
	Terrain::TerrainMod* mod;
	auto I = mTerrainMods.find(&entity);
	if (mTerrainMods.find(&entity) == mTerrainMods.end()) {
		mod = new Terrain::TerrainMod(entity, value.Map());
		auto listener = new TerrainEffectorListener(entity);

		listener->EventEntityBeingDeleted.connect([this, &entity, mod]() {
			mod->reset();
			mTerrainHandler.updateMod(mod);
			mTerrainMods.erase(&entity);
		});
		listener->EventEntityMoved.connect([this, &entity, mod]() {
			if (entity.getPositioningMode() == EmberEntity::PositioningMode::PLANTED) {
				mTerrainHandler.updateMod(mod);
			}
		});
		listener->EventEntityModeChanged.connect([this, &entity, mod]() { entityModeChanged(entity, *mod); });
		mTerrainMods.insert(std::make_pair(&entity, std::make_pair(std::unique_ptr<Terrain::TerrainMod>(mod), std::unique_ptr<TerrainEffectorListener>(listener))));
	} else {
		mod = I->second.first.get();
		mod->parse(value);
	}

	if (entity.getPositioningMode() == EmberEntity::PositioningMode::PLANTED) {
		mTerrainHandler.updateMod(mod);
	}
}

void TerrainEntityManager::entityAreaAttrChanged(EmberEntity& entity, const Atlas::Message::Element& value) {
	Terrain::TerrainArea* terrainArea;
	auto I = mAreas.find(&entity);
	if (I == mAreas.end()) {
		terrainArea = new Terrain::TerrainArea(entity);
		auto listener = new TerrainEffectorListener(entity);
		mAreas.insert(std::make_pair(&entity, std::make_pair(std::unique_ptr<Terrain::TerrainArea>(terrainArea), std::unique_ptr<TerrainEffectorListener>(listener))));
		listener->EventEntityBeingDeleted.connect([this, &entity]() { entityBeingDeleted(entity); });
		listener->EventEntityMoved.connect([this, &entity, terrainArea]() { entityMoved(entity, *terrainArea); });
		listener->EventEntityModeChanged.connect([this, &entity, terrainArea]() { entityModeChanged(entity, *terrainArea); });
	} else {
		terrainArea = I->second.first.get();
	}

	auto area = terrainArea->parse(value);
	//Only add area if we're planted
	if (entity.getPositioningMode() == EmberEntity::PositioningMode::PLANTED) {
		mTerrainHandler.updateArea(entity.getId(), area.get());
	}
}

void TerrainEntityManager::entityBeingDeleted(EmberEntity& entity) {

	mTerrainHandler.updateArea(entity.getId(), nullptr);
	mAreas.erase(&entity);

}

void TerrainEntityManager::entityMoved(EmberEntity& entity, Terrain::TerrainArea& terrainArea) {

	if (entity.getPositioningMode() == EmberEntity::PositioningMode::PLANTED) {
		auto area = terrainArea.updatePosition();
		mTerrainHandler.updateArea(entity.getId(), area.get());
	}
}

void TerrainEntityManager::entityModeChanged(EmberEntity& entity, Terrain::TerrainArea& terrainArea) {
	//If mode changes to "planted" we should add the area, if moves to any other we should remove it.
	auto I = mTerrainHandler.getAreas().find(entity.getId());
	if (entity.getPositioningMode() == EmberEntity::PositioningMode::PLANTED) {
		if (I == mTerrainHandler.getAreas().end()) {
			auto area = terrainArea.updatePosition();
			if (area) {
				mTerrainHandler.updateArea(entity.getId(), area.get());
			}
		}
	} else {
		if (I != mTerrainHandler.getAreas().end()) {
			mTerrainHandler.updateArea(entity.getId(), nullptr);
		}
	}
}

void TerrainEntityManager::entityModeChanged(EmberEntity& entity, Terrain::TerrainMod& terrainMod) {
	//If mode changes to "planted" we should add the mod, if moves to any other we should remove it.
	if (entity.getPositioningMode() == EmberEntity::PositioningMode::PLANTED) {
		if (entity.hasProperty("terrainmod")) {
			terrainMod.parse(entity.valueOfProperty("terrainmod"));
			mTerrainHandler.updateMod(&terrainMod);
		}
	} else {
		if (terrainMod.getTranslator()) {
			terrainMod.reset();
			mTerrainHandler.updateMod(&terrainMod);
		}
	}
}
}
}
