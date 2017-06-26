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

#include "domain/EmberEntity.h"

#include <OgreSceneManager.h>

#include <Eris/View.h>

#include <Mercator/Area.h>

#include <sigc++/bind.h>

namespace Ember
{
namespace OgreView
{

TerrainEntityManager::TerrainEntityManager(Eris::View& view, Terrain::TerrainHandler& terrainHandler, Ogre::SceneManager& sceneManager) :
		mView(view), mTerrainHandler(terrainHandler), mSceneManager(sceneManager)
{
	view.TopLevelEntityChanged.connect(sigc::mem_fun(*this, &TerrainEntityManager::topLevelEntityChanged));

	mTerrainListener = std::bind(&TerrainEntityManager::entityTerrainAttrChanged, *this, std::placeholders::_1, std::placeholders::_2);
	mTerrainModListener = std::bind(&TerrainEntityManager::entityTerrainModAttrChanged, *this, std::placeholders::_1, std::placeholders::_2);
	mTerrainAreaListener = std::bind(&TerrainEntityManager::entityAreaAttrChanged, *this, std::placeholders::_1, std::placeholders::_2);

	EmberEntity::registerGlobalAttributeListener("terrain", mTerrainListener);
	EmberEntity::registerGlobalAttributeListener("terrainmod", mTerrainModListener);
	EmberEntity::registerGlobalAttributeListener("area", mTerrainAreaListener);
}

TerrainEntityManager::~TerrainEntityManager()
{
	EmberEntity::deregisterGlobalAttributeListener("terrain", mTerrainListener);
	EmberEntity::deregisterGlobalAttributeListener("terrainmod", mTerrainModListener);
	EmberEntity::deregisterGlobalAttributeListener("area", mTerrainAreaListener);
}

void TerrainEntityManager::topLevelEntityChanged()
{
	EmberEntity* entity = static_cast<EmberEntity*>(mView.getTopLevel());
	entity->setAttachment(nullptr);
	entity->setAttachment(new WorldAttachment(*entity, mSceneManager.getRootSceneNode()->createChildSceneNode("entity_" + entity->getId())));
}

void TerrainEntityManager::entityTerrainAttrChanged(EmberEntity& entity, const Atlas::Message::Element& value)
{
	if (!mTerrainEntityDeleteConnection) {
		mTerrainEntityDeleteConnection = entity.BeingDeleted.connect([this](){
			mTerrainHandler.EventTerrainDisabled();
			mTerrainEntityDeleteConnection.disconnect();
		});
	}

	Terrain::TerrainShaderParser terrainShaderParser(mTerrainHandler);
	terrainShaderParser.createShaders(value);
	Terrain::TerrainParser terrainParser;
	WFMath::Point<3> pos = entity.getPosition().isValid() ? entity.getPredictedPos() : WFMath::Point<3>::ZERO();
	mTerrainHandler.updateTerrain(terrainParser.parseTerrain(value, pos));
	entity.setHeightProvider(&mTerrainHandler);
	mTerrainHandler.EventTerrainEnabled(entity);
}

void TerrainEntityManager::entityTerrainModAttrChanged(EmberEntity& entity, const Atlas::Message::Element& value)
{
	if (value.isMap() && mTerrainMods.find(&entity) == mTerrainMods.end()) {
		Terrain::TerrainMod* mod = new Terrain::TerrainMod(entity, value.Map());
		mod->init();
		mTerrainMods.insert(std::make_pair(&entity, mod));
		mTerrainHandler.addTerrainMod(mod);
		entity.BeingDeleted.connect(sigc::bind(sigc::mem_fun(*this, &TerrainEntityManager::entityBeingDeletedTerrainMod), &entity));
	}
}

void TerrainEntityManager::entityAreaAttrChanged(EmberEntity& entity, const Atlas::Message::Element& value)
{
	Terrain::TerrainArea* terrainArea;
	auto I = mAreas.find(&entity);
	if (I == mAreas.end()) {
		terrainArea = new Terrain::TerrainArea(entity);
		mAreas.insert(std::make_pair(&entity, terrainArea));
		terrainArea->EventEntityBeingDeleted.connect(sigc::bind(sigc::mem_fun(*this, &TerrainEntityManager::entityBeingDeletedArea), &entity));
		terrainArea->EventEntityMoved.connect(sigc::bind(sigc::mem_fun(*this, &TerrainEntityManager::entityMovedArea), &entity));
	} else {
		terrainArea = I->second;
	}

	Mercator::Area* area = nullptr;
	terrainArea->parse(value, &area);

	mTerrainHandler.updateArea(entity.getId(), area);
	delete area;
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
		mTerrainHandler.updateArea(entity->getId(), nullptr);
		delete I->second;
		mAreas.erase(I);
	}
}

void TerrainEntityManager::entityMovedArea(EmberEntity* entity)
{
	AreaStore::iterator I = mAreas.find(entity);
	if (I != mAreas.end()) {
		Mercator::Area* area = nullptr;
		I->second->updatePosition(&area);
		mTerrainHandler.updateArea(entity->getId(), area);
		delete area;
	}
}

}
}
