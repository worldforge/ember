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

//we must include xerces stuff before ogre stuff, because else we'll get errors when compiling in debug mode
//this seems to be because both uses their own internal memory handlers
#include <xercesc/util/XMemory.hpp>
#include <xercesc/dom/DOM.hpp>
#include <xercesc/util/XMLString.hpp>
#include <xercesc/util/PlatformUtils.hpp>


#include "EmberEntity.h"
#include "Model.h"
#include "TerrainGenerator.h"
#include "WorldEmberEntity.h"

namespace EmberOgre {
WorldEmberEntity::WorldEmberEntity(const std::string& id, Eris::TypeInfo* ty, Eris::View* vw, Ogre::SceneManager* sceneManager, TerrainGenerator* terrainGenerator) : 
mTerrainGenerator(terrainGenerator),
EmberEntity(id, ty, vw, sceneManager)
{
	//getSceneNode()->setOrientation(Ogre::Quaternion::IDENTITY);
	mSceneManager->getRootSceneNode()->addChild(getSceneNode());
	//mOgreNode = mSceneManager->getRootSceneNode();
	//mSceneManager->getRootSceneNode()->addChild(mOgreNode);
//	this->mModel->setQueryFlag(EmberEntity::CM_TERRAIN);
}

WorldEmberEntity::~WorldEmberEntity()
{}

void WorldEmberEntity::init(const Atlas::Objects::Entity::GameEntity &ge)
{
	//mOgreNode = mSceneManager->getRootSceneNode();
	Eris::Entity::init(ge);
	mTerrainGenerator->initTerrain(this, mView);
	mTerrainGenerator->prepareAllSegments(false);
	
}

void WorldEmberEntity::adjustHeightPositionForContainedNode(EmberEntity* const entity)
{
	Ogre::SceneNode* sceneNode = entity->getSceneNode();
	Ogre::Vector3 position = sceneNode->getPosition();
	//get the height from Mercator through the TerrainGenerator
	assert(mTerrainGenerator);
	float height = mTerrainGenerator->getHeight(OGRE2WF(position.x), OGRE2WF(position.z));
	sceneNode->setPosition(position.x, WF2OGRE(height),position.z);

}

 void WorldEmberEntity::onMoved(){
 	Eris::Entity::onMoved();
 }
 void WorldEmberEntity::onTalk(const Atlas::Objects::Root& obj)
 {
 	Eris::Entity::onTalk(obj);
 }
//	virtual void setContainer(Entity *pr);
 void WorldEmberEntity::onVisibilityChanged(bool vis)
 {
 	Eris::Entity::onVisibilityChanged(vis);
 }
 void WorldEmberEntity::onLocationChanged(Eris::Entity *oldLocation)
 {
 	Eris::Entity::onLocationChanged(oldLocation);
 }



}
