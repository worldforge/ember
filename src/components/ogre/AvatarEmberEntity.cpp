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
#include "Avatar.h"
#include "AvatarDimeEntity.h"

namespace DimeOgre {


AvatarDimeEntity::AvatarDimeEntity(const Atlas::Objects::Entity::GameEntity &ge, Eris::World* vw, Ogre::SceneManager* sceneManager) : 
mAvatar(NULL),
DimeEntity(ge, vw, sceneManager)
{
}

AvatarDimeEntity::~AvatarDimeEntity()
{}


void AvatarDimeEntity::createOgreEntity(Ogre::SceneManager* sceneManager) {
	Ogre::SceneNode* ogreNode = dynamic_cast<Ogre::SceneNode*>(sceneManager->getRootSceneNode()->createChild());

	mOgreEntity = sceneManager->createEntity(getID(), "robot.mesh");

	// attach the node to the entity
	ogreNode->attachObject(mOgreEntity);
}

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
}

void AvatarDimeEntity::setVisible(bool vis)
{
	mOgreEntity->setVisible(true);	
}
/*
void AvatarDimeEntity::addMember(Entity *e) 
{
	try{
		SceneNode* sceneNode = mSceneManager->getSceneNode(e->getID());

		getSceneNode()->addChild(sceneNode);
		//don't show it though
		//sceneNode->setVisible(false);
	} catch(Ogre::Exception ex) {
		//the contained item haven't been initialised yet
		//this is no problem though because when it do get initialized
		//it's container will be set
	}

	Entity::addMember(e);
	
}

void AvatarDimeEntity::rmvMember(Entity *e)
{
	try{
		getSceneNode()->removeChild(e->getID());
	} catch(Ogre::Exception ex) {
		//the contained item haven't been initialised yet
		//this is no problem though because when it do get initialized
		//it's container will be set
	}
	Entity::rmvMember(e);	
}
*/

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

