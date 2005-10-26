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
#include "EmberOgrePrerequisites.h"
#include "EmberEntityFactory.h"


#include "services/server/ServerService.h"
#include "services/EmberServices.h"


#include "EmberEntity.h"
#include "WorldEmberEntity.h"
#include "EmberPhysicalEntity.h"
#include "PersonEmberEntity.h"
#include "AvatarEmberEntity.h"
#include "EmberOgre.h"


#include "model/Model.h"
#include "model/ModelDefinitionManager.h"



#include "framework/ConsoleBackend.h"
#include "TerrainGenerator.h"





namespace EmberOgre {


EmberEntityFactory::EmberEntityFactory(TerrainGenerator* terrainGenerator, Eris::TypeService* typeService ) 
: mTerrainGenerator(terrainGenerator)
, mTypeService(typeService)
, mWorldEntity(0)
{
	mTerrainType = mTypeService->getTypeByName("world");
	Ember::ServerService* serverService = Ember::EmberServices::getInstance()->getServerService();
	loadTypeInfo();
	
	serverService->GotAvatar.connect(sigc::mem_fun(*this, &EmberEntityFactory::setAvatar));
	
}



EmberEntityFactory::~EmberEntityFactory()
{}

/// create whatever entity the client desires
Eris::Entity* EmberEntityFactory::instantiate(const Atlas::Objects::Entity::RootEntity &ge, Eris::TypeInfo* type, Eris::View* w)
{
	
//	Ember::ConsoleBackend::getMainConsole()->pushMessage("Adding entity...");
	Eris::Entity* emberEntity;
/*    Eris::TypeInfoPtr type = mTypeService->getTypeForAtlas(ge);*/

 	bool isPhysical = true;
// 	for (NonPhysicalTypeStore::const_iterator I = mNonPhysicalTypes.begin(); I != mNonPhysicalTypes.end(); ++I) {
// 		if (type->isA(mTypeService->getTypeByName(*I))) {
// 			isPhysical = false;
// 		}
// 	}

    if (ge->getId() == mAvatar->getId()) {
   	
    	AvatarEmberEntity* avatarEntity = createAvatarEntity(ge, type,  w);
    	emberEntity = avatarEntity;
 
    } else if (!isPhysical) {
    	S_LOG_VERBOSE("Creating immaterial entity.");

    	//we don't want this to have any Ogre::Entity
		emberEntity = new EmberEntity(ge->getId(), type, w, EmberOgre::getSingleton().getSceneManager());

    } else if (type->isA(mTerrainType)) {

    	emberEntity = createWorld(ge, type, w);

    } else {

    	emberEntity = createPhysicalEntity(ge, type, w);
 
    }

	S_LOG_VERBOSE("Entity added to game view.");
	return emberEntity;
}

AvatarEmberEntity* EmberEntityFactory::createAvatarEntity(const Atlas::Objects::Entity::RootEntity &ge, Eris::TypeInfo* type, Eris::View *world)
{
	Ogre::String id = ge->getId();
	id += "_scaleNode";
	Ogre::SceneNode* scaleNode = static_cast<Ogre::SceneNode*>(EmberOgre::getSingleton().getSceneManager()->createSceneNode (id));
	Model* model = new Model(ge->getId());
	model->create("settler");
/*	Model* model = Model::Create("settler.modeldef.xml", );*/
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
    	
    return new AvatarEmberEntity(ge->getId(), type, world,EmberOgre::getSingleton().getSceneManager(), scaleNode, mAvatar);
	
}

/// Accept is called by the world to test if this factory can instantiate the specified object
/** Accept is called when an entity must be constructed; this will be called every time
an object is created, so avoid lengthy processing if possible. */
bool EmberEntityFactory::accept(const Atlas::Objects::Entity::RootEntity &ge, Eris::TypeInfo* type)
{
	return true;
}


Eris::Entity* EmberEntityFactory::createWorld(const Atlas::Objects::Entity::RootEntity & ge, Eris::TypeInfo* type, Eris::View *world) {
	assert(!mWorldEntity);
	mWorldEntity = new WorldEmberEntity(ge->getId(), type, world, EmberOgre::getSingleton().getSceneManager(), mTerrainGenerator);
      // Extract base points and send to terrain        
      //TerrainEntity * te = new TerrainEntity(ge,w);
//	mTerrainGenerator->prepareSegments(-1, -1, 3, true)
	//buildTerrainAroundAvatar();
	//mTerrainSource->setHasTerrain(true);
	//mSceneManager->setViewGeometry("");
    return mWorldEntity;
}

WorldEmberEntity* EmberEntityFactory::getWorld() const
{
	return mWorldEntity;
}

void EmberEntityFactory::setAvatar(Eris::Avatar* avatar)
{
	mAvatar = avatar;	
	mAvatar->GotCharacterEntity.connect(sigc::mem_fun(*this, &EmberEntityFactory::gotAvatarCharacter));
}

void EmberEntityFactory::gotAvatarCharacter(Eris::Entity* entity)
{
	AvatarEmberEntity* avatarEntity = static_cast<AvatarEmberEntity*>(entity);
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



EmberPhysicalEntity* EmberEntityFactory::createPhysicalEntity(const Atlas::Objects::Entity::RootEntity &ge,Eris::TypeInfo* type, Eris::View *world) {
	
	Ogre::Vector3 scaler = Ogre::Vector3::UNIT_SCALE;
	Ogre::String id = ge->getId();
	id += "_scaleNode";
	Ogre::SceneNode* scaleNode = static_cast<Ogre::SceneNode*>(EmberOgre::getSingleton().getSceneManager()->createSceneNode (id));
	//mScaleNode->setInheritScale(false);
	//mScaleNode->setScale(Ogre::Vector3(0.01,0.01,0.01));	
	//scaleNode->showBoundingBox(true);

	std::string typeName = mTypeService->getTypeForAtlas(ge)->getName();
	
	Model* model = new Model(ge->getId());
	bool result = model->create(typeName);

	//try to open the model definition file
	if (!result) 
	{
		S_LOG_FAILURE( "Could not find " << typeName << ", using placeholder.");
		result = model->create("placeholder");
		assert(result); //if this fails we don't even have the placeholder and something is very wrong
	} 
	//rotate node to fit with WF space
	//perhaps this is something to put in the model spec instead?
	scaleNode->rotate(Ogre::Vector3::UNIT_Y,(Ogre::Degree)90);
	
	scaleNode->attachObject(model);

	EmberPhysicalEntity* entity;
	if (mPersonSet.find(typeName) != mPersonSet.end()) {
		entity = new PersonEmberEntity(ge->getId(), type,  world, EmberOgre::getSingleton().getSceneManager(), scaleNode);
	} else {
		entity = new EmberPhysicalEntity(ge->getId(), type, world, EmberOgre::getSingleton().getSceneManager(), scaleNode);
	}
	return entity;
	
	
}


void EmberEntityFactory::loadTypeInfo()
{
//TODO: put this in a separate xml file or something

	mPersonSet.insert("settler");
	mPersonSet.insert("merchant");
	mPersonSet.insert("mercenary");
	mPersonSet.insert("butcher");
	mPersonSet.insert("marshall");
	

		
}










/** retrieve this factory's priority level; higher priority factories
get first chance to process a recieved Atlas entity. The default implementation
returns one. */
int EmberEntityFactory::priority() {
	return 10;
}

}

