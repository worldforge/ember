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

#include "services/EmberServices.h"
#include "services/config/ConfigService.h"



#include "EmberEntity.h"
#include "model/Model.h"
#include "TerrainGenerator.h"
#include "WorldEmberEntity.h"
#include "environment/Foliage.h"
#include "environment/Water.h"
#include "environment/Sun.h"
#include "environment/Sky.h"
#include "EmberOgre.h"
#include "Avatar.h"
#include "AvatarCamera.h"

//#include "components/ogre/EmberSceneManager/include/EmberTerrainSceneManager.h"
#include <Mercator/Area.h>

namespace EmberOgre {
WorldEmberEntity::WorldEmberEntity(const std::string& id, Eris::TypeInfo* ty, Eris::View* vw, Ogre::SceneManager* sceneManager, TerrainGenerator* terrainGenerator) : 
mTerrainGenerator(terrainGenerator),
mFoliage(0),
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

void WorldEmberEntity::init(const Atlas::Objects::Entity::RootEntity &ge)
{
	Eris::Entity::init(ge);
	mTerrainGenerator->initTerrain(this, mView);
	
	mSun = new Sun(EmberOgre::getSingleton().getMainCamera()->getCamera(), EmberOgre::getSingleton().getSceneManager());
	
	mWater = new Water(EmberOgre::getSingleton().getMainCamera()->getCamera(), EmberOgre::getSingleton().getSceneManager());
	mSky = new Sky(EmberOgre::getSingleton().getMainCamera()->getCamera(), EmberOgre::getSingleton().getSceneManager());
	
	//mOgreNode = mSceneManager->getRootSceneNode();
	mFoliage = new Foliage(EmberOgre::getSingleton().getSceneManager());
	mTerrainGenerator->prepareAllSegments();
	//mTerrainGenerator->prepareSegments(0,0,1,true);
	

	
}

void WorldEmberEntity::adjustHeightPositionForContainedNode(EmberEntity* const entity)
{
	Ogre::SceneNode* sceneNode = entity->getSceneNode();
	Ogre::Vector3 position = sceneNode->getPosition();
	
	if (entity->getMovementMode() == EmberEntity::MM_FLOATING) {
		sceneNode->setPosition(position.x, 0,position.z);
	} else if (entity->getMovementMode() == EmberEntity::MM_SWIMMING) {
	//if it's swimming, make sure that it's between the sea bottom and the surface
		TerrainPosition pos = Ogre2Atlas_TerrainPosition(position);
		float height = mTerrainGenerator->getHeight(pos);
		if (position.y < height) {
			sceneNode->setPosition(position.x, height,position.z);
		} else if (position.y > 0) {
			sceneNode->setPosition(position.x, 0,position.z);
		}
		
	} else {
		//get the height from Mercator through the TerrainGenerator
		assert(mTerrainGenerator);
//		TerrainPosition pos(entity->getPredictedPos().x(), entity->getPredictedPos().y());
		TerrainPosition pos = Ogre2Atlas_TerrainPosition(position);
		float height = mTerrainGenerator->getHeight(pos);
		sceneNode->setPosition(position.x, height,position.z);
	}

}

float WorldEmberEntity::getHeightPositionForContainedNode(const TerrainPosition& position, EmberEntity* const entity)
{
	return mTerrainGenerator->getHeight(position);

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
 
void WorldEmberEntity::addArea(TerrainArea* area)
{
	mTerrainGenerator->addArea(area->getArea());
}



}
