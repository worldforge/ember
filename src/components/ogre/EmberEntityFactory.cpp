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
#include "Model.h"
#include "services/server/ServerService.h"
#include "services/DimeServices.h"

#include "framework/ConsoleBackend.h"
//#include "MathConverter.h"
#include "TerrainGenerator.h"
//#include "DimeTerrainSceneManager.h"
#include "DimeTerrainPageSource.h"

#include "DimeEntity.h"
#include "DimePhysicalEntity.h"
#include "PersonDimeEntity.h"
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
	loadTypeInfo();
	
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
 
    } else if (type->safeIsA(mTypeService->getTypeByName("boundary")) 
    || type->safeIsA(mTypeService->getTypeByName("weather"))) {

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
	Model* model = new Model(mSceneManager, ge.getId());
	
	//rotate node to fit with WF space
	//perhaps this is something to put in the model spec instead?
	scaleNode->rotate(Ogre::Vector3::UNIT_Y,(Ogre::Degree)90);
	
	model->createFromXML("modeldefinitions/settler.modeldef.xml");
	
//	Ogre::Entity* ogreEntity = mSceneManager->createEntity(ge.getId(), "robot.mesh");
//	mAnimStateWalk = model->getAnimationState("walk");	
//	MotionManager::getSingleton().addAnimation(mAnimStateWalk);

	// attach the node to the entity
	scaleNode->attachObject(model);
    	
    return new AvatarDimeEntity(ge, world,mSceneManager, scaleNode, mAvatar);
	
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
//	mTerrainGenerator->prepareSegments(-1, -1, 3, true)
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
	Ogre::String id = ge.getId();
	id += "_scaleNode";
	Ogre::SceneNode* scaleNode = static_cast<Ogre::SceneNode*>(mSceneManager->createSceneNode (id));
	//mScaleNode->setInheritScale(false);
	//mScaleNode->setScale(Ogre::Vector3(0.01,0.01,0.01));	
	//scaleNode->showBoundingBox(true);

	std::string typeName = mTypeService->getTypeForAtlas(ge)->getName();
	Model* model = new Model(mSceneManager, ge.getId());

	//try to open the model definition file
	bool result = model->createFromXML(std::string("modeldefinitions/") + typeName + ".modeldef.xml");
	if (!result) 
	{
		std::cout << "Could not find " << typeName << ", using placeholder.";
		model->createFromXML("modeldefinitions/placeholder.modeldef.xml");
	}
	
	//rotate node to fit with WF space
	//perhaps this is something to put in the model spec instead?
	scaleNode->rotate(Ogre::Vector3::UNIT_Y,(Ogre::Degree)90);

	scaleNode->attachObject(model);

	if (mPersonSet.find(typeName) != mPersonSet.end()) {
		return new PersonDimeEntity(ge, world, mSceneManager, scaleNode);
	} else {
		return new DimePhysicalEntity(ge, world, mSceneManager, scaleNode);
	}
	
}


void DimeEntityFactory::loadTypeInfo()
{
	mPersonSet.insert("settler");
	mPersonSet.insert("merchant");
	mPersonSet.insert("mercenary");
	mPersonSet.insert("butcher");
		
}



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

