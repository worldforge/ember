/*
    Copyright (C) 2004  Erik Hjortsberg

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

#include "DimeEntity.h"
#include "TerrainGenerator.h"
#include "WorldDimeEntity.h"

namespace DimeOgre {
WorldDimeEntity::WorldDimeEntity(const Atlas::Objects::Entity::GameEntity &ge, Eris::World* vw, Ogre::SceneManager* sceneManager, TerrainGenerator* terrainGenerator) : 
mTerrainGenerator(terrainGenerator),
DimeEntity(ge, vw, sceneManager)
{}

WorldDimeEntity::~WorldDimeEntity()
{}

void WorldDimeEntity::adjustHeightPositionForContainedNode(DimeEntity* const entity)
{
	Ogre::SceneNode* sceneNode = entity->getSceneNode();
	Ogre::Vector3 position = sceneNode->getPosition();
	//get the height from Mercator through the TerrainGenerator
	assert(mTerrainGenerator);
	float height = mTerrainGenerator->getHeight(OGRE2WF(position.x), OGRE2WF(position.z));
	sceneNode->setPosition(position.x, WF2OGRE(height),position.z);

}


}
