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

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "services/server/ServerService.h"
#include "services/DimeServices.h"

#include "framework/ConsoleBackend.h"
//#include "MathConverter.h"
#include "TerrainGenerator.h"
//#include "DimeTerrainSceneManager.h"
#include "DimeTerrainPageSource.h"

#include "DimeEntity.h"
#include "DimePhysicalEntity.h"
#include "AvatarDimeEntity.h"
#include "DimeEntityFactory.h"
#include "WorldDimeEntity.h"

namespace DimeOgre {


DimeEntityFactory::DimeEntityFactory(Ogre::TerrainSceneManager* sceneManager,TerrainGenerator* terrainGenerator, Eris::TypeService* typeService ) 
: mSceneManager(sceneManager)
, mTerrainGenerator(terrainGenerator)
, mTypeService(typeService)
{
	mTerrainType = mTypeService->getTypeByName("world");
	dime::ServerService* serverService = dime::DimeServices::getInstance()->getServerService();
	loadMeshDefinitions();
	
	serverService->GotAvatar.connect(SigC::slot(*this, &DimeEntityFactory::setAvatar));
}



DimeEntityFactory::~DimeEntityFactory()
{}

/// create whatever entity the client desires
/*eris 1.3
Entity* DimeEntityFactory::instantiate(const Atlas::Objects::Entity::GameEntity &ge, Eris::TypeInfo* type, Eris::World* w)
*/
Eris::Entity* DimeEntityFactory::instantiate(const Atlas::Objects::Entity::GameEntity &ge, Eris::World *world)
{
	
//	dime::ConsoleBackend::getMainConsole()->pushMessage("Adding entity...");
	Eris::Entity* dimeEntity;
    Eris::TypeInfoPtr type = mTypeService->getTypeForAtlas(ge);
    if (ge.getId() == mAvatar->getID()) {
   	
    	AvatarDimeEntity* avatarEntity = createAvatarEntity(ge, world);
    	CreatedAvatarEntity.emit(avatarEntity);
    	dimeEntity = avatarEntity;
 
    } else if (type->safeIsA(mTypeService->getTypeByName("boundary"))) {

    	//we don't want this to have any Ogre::Entity
		dimeEntity = new DimeEntity(ge, world, mSceneManager);

    } else if (type->safeIsA(mTerrainType)) {

    	dimeEntity = createWorld(ge, world);

    } else {

    	dimeEntity = createPhysicalEntity(ge, world);
 
    }

	fprintf(stderr, "TRACE - ENTITY ADDED TO THE GAMEVIEW\n");
	return dimeEntity;
}

AvatarDimeEntity* DimeEntityFactory::createAvatarEntity(const Atlas::Objects::Entity::GameEntity &ge, Eris::World *world)
{
	Ogre::String id = ge.getId();
	id += "_scaleNode";
	Ogre::SceneNode* scaleNode = static_cast<Ogre::SceneNode*>(mSceneManager->createSceneNode (id));
	Ogre::Entity* ogreEntity = mSceneManager->createEntity(ge.getId(), "robot.mesh");

	// attach the node to the entity
	scaleNode->attachObject(ogreEntity);
    	
    return new AvatarDimeEntity(ge, world,mSceneManager, scaleNode, Ogre::Vector3::UNIT_SCALE);
	
}

/// Accept is called by the world to test if this factory can instantiate the specified object
/** Accept is called when an entity must be constructed; this will be called every time
an object is created, so avoid lengthy processing if possible. */
bool DimeEntityFactory::accept(const Atlas::Objects::Entity::GameEntity &ge, Eris::World *world)
{
	return true;
}


Eris::Entity* DimeEntityFactory::createWorld(const Atlas::Objects::Entity::GameEntity & ge, Eris::World *world) {
    WorldDimeEntity *we = new WorldDimeEntity(ge, world, mSceneManager, mTerrainGenerator);
      // Extract base points and send to terrain        
      //TerrainEntity * te = new TerrainEntity(ge,w);
	mTerrainGenerator->initTerrain(we, world);
	mTerrainGenerator->prepareAllSegments(false);
	//buildTerrainAroundAvatar();
	//mTerrainSource->setHasTerrain(true);
	//mSceneManager->setWorldGeometry("");
    return we;
}

void DimeEntityFactory::setAvatar(Eris::Avatar* avatar)
{
	mAvatar = avatar;	
}

void DimeEntityFactory::buildTerrainAroundAvatar()
{
	int size = 4;
	//for now we'll only build terrain around position 0:0
	mTerrainGenerator->prepareSegments(-size, -size, (size * 2) + 1, true);
	
	
/*	WFMath::Point<3> point = mAvatar->getEntity()->getPosition();
	//decide how many segments we need
    long lowXBound = lrintf(point.x() / TerrainGenerator::segSize) - size,
         lowYBound = lrintf(point.y() / TerrainGenerator::segSize) - size;
	mTerrainGenerator->prepareSegments(lowXBound, lowYBound, (size * 2) + 1);
*/
}



DimePhysicalEntity* DimeEntityFactory::createPhysicalEntity(const Atlas::Objects::Entity::GameEntity &ge, Eris::World *world) {
	
	Ogre::Vector3 scaler = Ogre::Vector3::UNIT_SCALE;
	Ogre::Entity* ogreEntity;
	Ogre::String id = ge.getId();
	id += "_scaleNode";
	Ogre::SceneNode* scaleNode = static_cast<Ogre::SceneNode*>(mSceneManager->createSceneNode (id));
	//mScaleNode->setInheritScale(false);
	//mScaleNode->setScale(Ogre::Vector3(0.01,0.01,0.01));	
	//scaleNode->showBoundingBox(true);

	std::string typeName = mTypeService->getTypeForAtlas(ge)->getName();
	if (typeName == "pig") {
		int i = 0;
	}
	MeshDefinitionMap::const_iterator I = meshDefinitions.find(typeName);
	if (I != meshDefinitions.end()) {
		ogreEntity = mSceneManager->createEntity(ge.getId(), I->second.modelName);
		scaler = I->second.scaler;
	} else {
		//create placeholder
		ogreEntity = mSceneManager->createEntity(ge.getId(), "razor.mesh");
	}
	ogreEntity->setVisible(false);
	
	scaleNode->attachObject(ogreEntity);
	return new DimePhysicalEntity(ge, world, mSceneManager, scaleNode, scaler);
	
	
/*		
	MeshDefiniton def

	if(!strcmp(getType()->getName().c_str(),"settler"))	// 0 if strings are equal
	{
		mOgreEntity = sceneManager->createEntity(getID(), "robot.mesh");
		mAnimationState_Walk = mOgreEntity->getAnimationState("Walk");	
//		mScaleNode->setScale(Ogre::Vector3(0.02,0.02,0.02));
	}
	else if(!strcmp(getType()->getName().c_str(),"merchant"))
	{
		mOgreEntity = sceneManager->createEntity(getID(), "robot.mesh");
		mAnimationState_Walk = mOgreEntity->getAnimationState("Walk");	
		//mScaleNode->setScale(Ogre::Vector3(0.02,0.02,0.02));
	}
	else if(!strcmp(getType()->getName().c_str(),"pig"))
	{
		mOgreEntity = sceneManager->createEntity(getID(), "pig.mesh");
//		mScaleNode->setScale(0.4,0.4,0.4);
		mAnimationState_Walk = mOgreEntity->getAnimationState("Walk");	
	}
	else if(!strcmp(getType()->getName().c_str(),"sty"))
	{
		mOgreEntity = sceneManager->createEntity(getID(), "Sty.mesh");
//		mScaleNode->setScale(1,1,1);
	}
	else if(!strcmp(getType()->getName().c_str(),"squirrel"))
	{
		mOgreEntity = sceneManager->createEntity(getID(), "squirrel.mesh");
	}
	else if(!strcmp(getType()->getName().c_str(),"fir"))
	{
		mOgreEntity = sceneManager->createEntity(getID(), "Fir.mesh");
//		mScaleNode->setScale(0.02,0.02,0.02);
	}
	else if(!strcmp(getType()->getName().c_str(),"oak"))
	{
		mOgreEntity = sceneManager->createEntity(getID(), "Oak.mesh");
//		mScaleNode->setScale(0.04,0.04,0.04);
	}
	else
	{
		// TODO: razor should be a coin
		//fprintf(stderr, "TRACE - FOUND ANYTHING ELSE - RAZOR MESH: ");
		fprintf(stderr, getType()->getName().c_str());
		fprintf(stderr, "\n");

		mOgreEntity = sceneManager->createEntity(getID(), "razor.mesh");
		//ogreNode->setScale(1,1,1);
		//mOgreNode->setScale(0.1,0.1,0.1);
	}

*/	
	


	// attach the node to the entity
//	mScaleNode->attachObject(mOgreEntity);
//	scaleNode(0.01);

}


void DimeEntityFactory::loadMeshDefinitions()
{
	MeshDefinition def;
	def.scaler = Ogre::Vector3::UNIT_SCALE;
	
	def.modelName = "robot.mesh";
	meshDefinitions["settler"] = def;
	
	def.modelName = "robot.mesh";
	meshDefinitions["merchant"] = def;

	def.modelName = "pig.mesh";
	def.scaler = Ogre::Vector3(5,5,5);
	meshDefinitions["pig"] = def;

	def.modelName = "Sty.mesh";
	def.scaler = Ogre::Vector3::UNIT_SCALE;
	meshDefinitions["sty"] = def;

	def.modelName = "squirrel.mesh";
	meshDefinitions["squirrel"] = def;
	
	def.modelName = "Fir.mesh";
	meshDefinitions["fir"] = def;
	
	def.modelName = "Oak.mesh";
	meshDefinitions["oak"] = def;

	
}


/* namespace Sear */


/* eris 1.3
bool DimeEntityFactory::accept(const Atlas::Objects::Entity::GameEntity &ge, Eris::TypeInfo* type)
{
	return true;
}
*/





/** retrieve this factory's priority level; higher priority factories
get first chance to process a recieved Atlas entity. The default implementation
returns one. */
/*eris 1.3
int DimeEntityFactory::DimeEntityFactory::priority() {
	return 10;
}
*/
}

