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
 along with this program; if not, write to the Free Software
 Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
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

#include "EmberEntity.h"

#include <OgreSceneManager.h>

#include <Eris/View.h>

#include <sigc++/bind.h>

namespace Ember
{
namespace OgreView
{

TerrainEntityManager::TerrainEntityManager(Eris::View& view, Terrain::TerrainHandler& terrainHandler, Ogre::SceneManager& sceneManager) :
		mView(view), mTerrainHandler(terrainHandler), mSceneManager(sceneManager)
{
	view.EntitySeen.connect(sigc::mem_fun(*this, &TerrainEntityManager::entitySeen));
	view.EntityCreated.connect(sigc::mem_fun(*this, &TerrainEntityManager::entitySeen));
	view.TopLevelEntityChanged.connect(sigc::mem_fun(*this, &TerrainEntityManager::topLevelEntityChanged));
}

TerrainEntityManager::~TerrainEntityManager()
{
}

void TerrainEntityManager::topLevelEntityChanged()
{
	EmberEntity* entity = static_cast<EmberEntity*>(mView.getTopLevel());
	entity->setAttachment(new WorldAttachment(*entity, mSceneManager.getRootSceneNode()->createChildSceneNode("entity_" + entity->getId()), mTerrainHandler));

	initializeTerrain(entity);
}

void TerrainEntityManager::initializeTerrain(EmberEntity* entity)
{

//	mTerrainAfterUpdateConnection = mTerrainManager->getHandler().EventWorldSizeChanged.connect(sigc::mem_fun(*this, &WorldEmberEntity::terrain_WorldSizeChanged));
	bool hasValidShaders = false;
	Terrain::TerrainShaderParser terrainShaderParser(mTerrainHandler);
	if (entity->hasAttr("terrain")) {
		Terrain::TerrainParser terrainParser;
		const Atlas::Message::Element& terrainElement = entity->valueOfAttr("terrain");
		if (terrainElement.isMap()) {
			const Atlas::Message::MapType& terrainMap(terrainElement.asMap());
			if (terrainMap.count("surfaces")) {
				const Atlas::Message::Element& surfaceElement(terrainMap.find("surfaces")->second);
				terrainShaderParser.createShaders(surfaceElement);
				hasValidShaders = true;
			}
		}
		mTerrainHandler.updateTerrain(terrainParser.parseTerrain(terrainElement, entity->getPosition().isValid() ? entity->getPosition() : WFMath::Point<3>::ZERO()));

		entity->setHeightProvider(&mTerrainHandler);
	}

	if (!hasValidShaders) {
		terrainShaderParser.createDefaultShaders();
	}

	//TODO: Parse world location data when it's available
//	mEnvironment->setWorldPosition(mWorldPosition.LongitudeDegrees, mWorldPosition.LatitudeDegrees);

	mTopLevelTerrainConnection.disconnect();
	mTopLevelTerrainConnection = entity->observe("terrain", sigc::bind(sigc::mem_fun(*this, &TerrainEntityManager::terrainChanged), entity));
}

void TerrainEntityManager::terrainChanged(const Atlas::Message::Element& value, EmberEntity* entity)
{
	Terrain::TerrainParser terrainParser;
	updateTerrain(terrainParser.parseTerrain(value, entity->getPredictedPos()));
}

void TerrainEntityManager::updateTerrain(const std::vector<Terrain::TerrainDefPoint>& terrainDefinitionPoints)
{
	mTerrainHandler.updateTerrain(terrainDefinitionPoints);
}

void TerrainEntityManager::entitySeen(Eris::Entity* entity)
{
	registerEntity(static_cast<EmberEntity*>(entity));
}

void TerrainEntityManager::registerEntity(EmberEntity* entity)
{
	if (entity->hasAttr("area")) {
		entityAreaAttrChanged(entity->valueOfAttr("area"), entity);
	} else {
		entity->observe("area", sigc::bind(sigc::mem_fun(*this, &TerrainEntityManager::entityAreaAttrChanged), entity));
	}

	if (entity->hasAttr("terrainmod")) {
		entityTerrainModAttrChanged(entity->valueOfAttr("terrainmod"), entity);
	} else {
		entity->observe("terrainmod", sigc::bind(sigc::mem_fun(*this, &TerrainEntityManager::entityTerrainModAttrChanged), entity));
	}
}

void TerrainEntityManager::entityTerrainAttrChanged(const Atlas::Message::Element& value, EmberEntity* entity)
{

}

void TerrainEntityManager::entityTerrainModAttrChanged(const Atlas::Message::Element& value, EmberEntity* entity)
{
	if (mTerrainMods.find(entity) == mTerrainMods.end()) {
		Terrain::TerrainMod* mod = new Terrain::TerrainMod(*entity);
		mod->init();
		mTerrainMods.insert(std::make_pair(entity, mod));
		mTerrainHandler.addTerrainMod(mod);
		entity->BeingDeleted.connect(sigc::bind(sigc::mem_fun(*this, &TerrainEntityManager::entityBeingDeletedTerrainMod), entity));

	}

}

void TerrainEntityManager::entityAreaAttrChanged(const Atlas::Message::Element& value, EmberEntity* entity)
{
	if (mAreas.find(entity) == mAreas.end()) {
		Terrain::TerrainArea* area = new Terrain::TerrainArea(*entity);
		if (area->init()) {
			mAreas.insert(std::make_pair(entity, area));
			mTerrainHandler.addArea(*area);
			entity->BeingDeleted.connect(sigc::bind(sigc::mem_fun(*this, &TerrainEntityManager::entityBeingDeletedArea), entity));
		}
	}
}
void TerrainEntityManager::entityBeingDeletedTerrainMod(EmberEntity* entity)
{
	ModStore::iterator I = mTerrainMods.find(entity);
	if (I != mTerrainMods.end()) {
		delete I->second;
		mTerrainMods.erase(I);
	}
}
void TerrainEntityManager::entityBeingDeletedArea(EmberEntity* entity)
{
	AreaStore::iterator I = mAreas.find(entity);
	if (I != mAreas.end()) {
		delete I->second;
		mAreas.erase(I);
	}
}
}
}
