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
#include "Model.h"
#include "DimeEntity.h"
#include "DimePhysicalEntity.h"
#include "PersonDimeEntity.h"
#include "framework/ConsoleBackend.h"
#include "Avatar.h"
#include "GUIManager.h"
#include "AvatarDimeEntity.h"

namespace DimeOgre {


AvatarDimeEntity::AvatarDimeEntity(const Atlas::Objects::Entity::GameEntity &ge, Eris::World* vw, Ogre::SceneManager* sceneManager, Ogre::SceneNode* nodeWithModel, Eris::Avatar* erisAvatar) : 
mAvatar(NULL), mErisAvatar(erisAvatar)
,PersonDimeEntity(ge, vw, sceneManager, nodeWithModel)
{
	this->mModel->setQueryFlags(DimeEntity::CM_AVATAR);
}

AvatarDimeEntity::~AvatarDimeEntity()
{}

/*
void AvatarDimeEntity::createOgreEntity(Ogre::SceneManager* sceneManager) {
	Ogre::SceneNode* ogreNode = dynamic_cast<Ogre::SceneNode*>(sceneManager->getRootSceneNode()->createChild());

	mOgreEntity = sceneManager->createEntity(getID(), "robot.mesh");

	// attach the node to the entity
	ogreNode->attachObject(mOgreEntity);
}
*/

void AvatarDimeEntity::handleMove()
{
	if (getAvatar()) {
		getAvatar()->movedInWorld();
	}
//	getSceneNode()->setPosition(WF2OGRE_VECTOR3(1,1,1) * Atlas2Ogre(getPosition()));
//	getSceneNode()->setOrientation(Atlas2Ogre(getOrientation()));
}

void AvatarDimeEntity::handleTalk(const std::string &msg)
{
	std::string message = "<";
	message.append(getName());
	message.append("> ");
	message.append(msg);
	GUIManager::getSingleton().AppendIGChatLine.emit(message);
	std::cout << "TRACE - AVATAR SAYS: [" << message << "]\n" << std::endl;
	dime::ConsoleBackend::getMainConsole()->pushMessage(message);
}

/*
void AvatarDimeEntity::setVisible(bool vis)
{
	//TODO
	//mOgreEntity->setVisible(true);	
}
*/

void AvatarDimeEntity::addMember(Entity *e) 
{
	EventAddedEntityToInventory.emit(e);

/*	DimeEntity* dimeEntity = dynamic_cast<DimeEntity*>(e);
	if (dimeEntity) {
		EventAddedEntityToInventory.emit(dimeEntity);
	}*/
	PersonDimeEntity::addMember(e);
	
}


void AvatarDimeEntity::rmvMember(Entity *e)
{
	EventRemovedEntityFromInventory.emit(e);
/*	DimeEntity* dimeEntity = dynamic_cast<DimeEntity*>(e);
	if (dimeEntity) {
		EventRemovedEntityFromInventory.emit(dimeEntity);
	}*/
	PersonDimeEntity::rmvMember(e);	
}


void AvatarDimeEntity::setContainer(Entity *pr)
{
	Ogre::Vector3 oldWorldPosition = getSceneNode()->getWorldPosition();
	DimeEntity* dimeEntity = dynamic_cast<DimeEntity*>(pr);
	if (dimeEntity) {
		//detach from our current object and add to the new entity
		getSceneNode()->getParent()->removeChild(getSceneNode()->getName());
		dimeEntity->getSceneNode()->addChild(getSceneNode());
	} else {
		//detach from our current object and add to the world
		getSceneNode()->getParent()->removeChild(getSceneNode()->getName());
		getSceneNode()->getCreator()->getRootSceneNode()->addChild(getSceneNode());
	}		
	Entity::setContainer(pr);
	
	//we adjust the entity so it retains it's former position in the world
	Ogre::Vector3 newWorldPosition = getSceneNode()->getWorldPosition();
	getSceneNode()->translate(oldWorldPosition - newWorldPosition);
}

Ogre::SceneNode* AvatarDimeEntity::getAvatarSceneNode()
{
	return mScaleNode;	
}




}

