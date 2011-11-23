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
#include "terrain/TerrainManager.h"
#include "terrain/TerrainHandler.h"
#include "terrain/TerrainMod.h"
#include "terrain/TerrainArea.h"

#include "EmberEntity.h"

#include <Eris/View.h>

#include <sigc++/bind.h>

namespace Ember
{
namespace OgreView
{

TerrainEntityManager::TerrainEntityManager(Eris::View& view, Terrain::TerrainHandler& terrainHandler) :
		mTerrainHandler(terrainHandler)
{
	view.EntitySeen.connect(sigc::mem_fun(*this, &TerrainEntityManager::entitySeen));
	view.EntityCreated.connect(sigc::mem_fun(*this, &TerrainEntityManager::entitySeen));
}

TerrainEntityManager::~TerrainEntityManager()
{
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
