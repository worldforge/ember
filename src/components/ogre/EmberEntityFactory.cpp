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

#include <xercesc/util/XMemory.hpp>

#include <xercesc/dom/DOM.hpp>
#include <xercesc/util/XMLString.hpp>
#include <xercesc/util/PlatformUtils.hpp>


#include "services/server/ServerService.h"
#include "services/EmberServices.h"


#include "EmberEntity.h"
#include "WorldEmberEntity.h"
#include "EmberPhysicalEntity.h"
#include "PersonEmberEntity.h"
#include "AvatarEmberEntity.h"
#include "EmberEntityFactory.h"
#include "EmberOgre.h"


#include "model/Model.h"
#include "model/ModelDefinitionManager.h"



#include "framework/ConsoleBackend.h"
//#include "MathConverter.h"
#include "TerrainGenerator.h"
//#include "EmberTerrainSceneManager.h"
#include "EmberTerrainPageSource.h"


namespace EmberOgre {


EmberEntityFactory::EmberEntityFactory(Ogre::TerrainSceneManager* sceneManager,TerrainGenerator* terrainGenerator, Eris::TypeService* typeService ) 
: mSceneManager(sceneManager)
, mTerrainGenerator(terrainGenerator)
, mTypeService(typeService)
, mWorldEntity(0)
{
	mTerrainType = mTypeService->getTypeByName("world");
	Ember::ServerService* serverService = Ember::EmberServices::getInstance()->getServerService();
	loadTypeInfo();
	
	serverService->GotAvatar.connect(SigC::slot(*this, &EmberEntityFactory::setAvatar));
	
}



EmberEntityFactory::~EmberEntityFactory()
{}

/// create whatever entity the client desires
Eris::Entity* EmberEntityFactory::instantiate(const Atlas::Objects::Entity::GameEntity &ge, Eris::TypeInfo* type, Eris::View* w)
{
	
//	Ember::ConsoleBackend::getMainConsole()->pushMessage("Adding entity...");
	Eris::Entity* emberEntity;
/*    Eris::TypeInfoPtr type = mTypeService->getTypeForAtlas(ge);*/
    if (ge->getId() == mAvatar->getId()) {
   	
    	AvatarEmberEntity* avatarEntity = createAvatarEntity(ge, type,  w);
    	emberEntity = avatarEntity;
 
    } else if (type->isA(mTypeService->getTypeByName("boundary")) 
    || type->isA(mTypeService->getTypeByName("weather"))) {
		fprintf(stderr, "TRACE - CREATE IMMATERIAL ENTITY\n");

    	//we don't want this to have any Ogre::Entity
		emberEntity = new EmberEntity(ge->getId(), type, w, mSceneManager);

    } else if (type->isA(mTerrainType)) {

    	emberEntity = createWorld(ge, type, w);

    } else {

    	emberEntity = createPhysicalEntity(ge, type, w);
 
    }

	fprintf(stderr, "TRACE - ENTITY ADDED TO THE GAMEVIEW\n");
	return emberEntity;
}

AvatarEmberEntity* EmberEntityFactory::createAvatarEntity(const Atlas::Objects::Entity::GameEntity &ge, Eris::TypeInfo* type, Eris::View *world)
{
	Ogre::String id = ge->getId();
	id += "_scaleNode";
	Ogre::SceneNode* scaleNode = static_cast<Ogre::SceneNode*>(mSceneManager->createSceneNode (id));
	Model* model = Model::Create("settler.modeldef.xml", ge->getId());
	//Model* model = new Model(mSceneManager, ge->getId());
	
	//rotate node to fit with WF space
	//perhaps this is something to put in the model spec instead?
	scaleNode->rotate(Ogre::Vector3::UNIT_Y,(Ogre::Degree)90);
	
//	model->createFromXML("modeldefinitions/settler.modeldef.xml");
	
//	Ogre::Entity* ogreEntity = mSceneManager->createEntity(ge.getId(), "robot.mesh");
//	mAnimStateWalk = model->getAnimationState("walk");	
//	MotionManager::getSingleton().addAnimation(mAnimStateWalk);

	// attach the node to the entity
	scaleNode->attachObject(model);
    	
    return new AvatarEmberEntity(ge->getId(), type, world,mSceneManager, scaleNode, mAvatar);
	
}

/// Accept is called by the world to test if this factory can instantiate the specified object
/** Accept is called when an entity must be constructed; this will be called every time
an object is created, so avoid lengthy processing if possible. */
bool EmberEntityFactory::accept(const Atlas::Objects::Entity::GameEntity &ge, Eris::TypeInfo* type)
{
	return true;
}


Eris::Entity* EmberEntityFactory::createWorld(const Atlas::Objects::Entity::GameEntity & ge, Eris::TypeInfo* type, Eris::View *world) {
	assert(!mWorldEntity);
	mWorldEntity = new WorldEmberEntity(ge->getId(), type, world, mSceneManager, mTerrainGenerator);
      // Extract base points and send to terrain        
      //TerrainEntity * te = new TerrainEntity(ge,w);
//	mTerrainGenerator->prepareSegments(-1, -1, 3, true)
	//buildTerrainAroundAvatar();
	//mTerrainSource->setHasTerrain(true);
	//mSceneManager->setViewGeometry("");
    return mWorldEntity;
}

void EmberEntityFactory::setAvatar(Eris::Avatar* avatar)
{
	mAvatar = avatar;	
	mAvatar->GotCharacterEntity.connect(SigC::slot(*this, &EmberEntityFactory::gotAvatarCharacter));
}

void EmberEntityFactory::gotAvatarCharacter(Eris::Entity* entity)
{
	AvatarEmberEntity* avatarEntity = dynamic_cast<AvatarEmberEntity*>(entity);
   	EmberOgre::getSingleton().EventCreatedAvatarEntity.emit(avatarEntity);
}
	

void EmberEntityFactory::buildTerrainAroundAvatar()
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



EmberPhysicalEntity* EmberEntityFactory::createPhysicalEntity(const Atlas::Objects::Entity::GameEntity &ge,Eris::TypeInfo* type, Eris::View *world) {
	
	Ogre::Vector3 scaler = Ogre::Vector3::UNIT_SCALE;
	Ogre::String id = ge->getId();
	id += "_scaleNode";
	Ogre::SceneNode* scaleNode = static_cast<Ogre::SceneNode*>(mSceneManager->createSceneNode (id));
	//mScaleNode->setInheritScale(false);
	//mScaleNode->setScale(Ogre::Vector3(0.01,0.01,0.01));	
	//scaleNode->showBoundingBox(true);

	std::string typeName = mTypeService->getTypeForAtlas(ge)->getName();
	Model* model = Model::Create(typeName + ".modeldef.xml", ge->getId());
	//Model* model = new Model(mSceneManager, ge->getId());

	//try to open the model definition file
//	bool result = model->createFromXML(std::string("modeldefinitions/") + typeName + ".modeldef.xml");
	if (!model) 
	{
		std::cout << "Could not find " << typeName << ", using placeholder.";
		model = Model::Create(std::string("placeholder.modeldef.xml"), ge->getId());
		//model->createFromXML("modeldefinitions/placeholder.modeldef.xml");
	}
	
	//rotate node to fit with WF space
	//perhaps this is something to put in the model spec instead?
	scaleNode->rotate(Ogre::Vector3::UNIT_Y,(Ogre::Degree)90);

	scaleNode->attachObject(model);

	if (mPersonSet.find(typeName) != mPersonSet.end()) {
		return new PersonEmberEntity(ge->getId(), type,  world, mSceneManager, scaleNode);
	} else {
		return new EmberPhysicalEntity(ge->getId(), type, world, mSceneManager, scaleNode);
	}
	
}


void EmberEntityFactory::loadTypeInfo()
{
//TODO: put this in a separate xml file or something

	mPersonSet.insert("settler");
	mPersonSet.insert("merchant");
	mPersonSet.insert("mercenary");
	mPersonSet.insert("butcher");
		
}










/** retrieve this factory's priority level; higher priority factories
get first chance to process a recieved Atlas entity. The default implementation
returns one. */
int EmberEntityFactory::EmberEntityFactory::priority() {
	return 10;
}

}

