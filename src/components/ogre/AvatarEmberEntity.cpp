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


#include "DimeEntity.h"
#include "DimePhysicalEntity.h"
#include "PersonDimeEntity.h"
#include "framework/ConsoleBackend.h"
#include "Avatar.h"
#include "GUIManager.h"
#include "Model.h"
#include "AvatarDimeEntity.h"
#include "DimeOgre.h"

namespace DimeOgre {


AvatarDimeEntity::AvatarDimeEntity(const std::string& id, Eris::TypeInfo* type, Eris::View* vw, Ogre::SceneManager* sceneManager, Ogre::SceneNode* nodeWithModel, Eris::Avatar* erisAvatar) : 
mAvatar(NULL), mErisAvatar(erisAvatar)
,PersonDimeEntity(id, type, vw, sceneManager, nodeWithModel)
{
}

AvatarDimeEntity::~AvatarDimeEntity()
{}

void AvatarDimeEntity::init(const Atlas::Objects::Entity::GameEntity &ge)
{
	PersonDimeEntity::init(ge);
	mModel->setQueryFlags(DimeEntity::CM_AVATAR);
}

void AvatarDimeEntity::onMoved()
{
	if (getAvatar()) {
		getAvatar()->movedInWorld();
	}
	Eris::Entity::onMoved();
//	getSceneNode()->setPosition(WF2OGRE_VECTOR3(1,1,1) * Atlas2Ogre(getPosition()));
//	getSceneNode()->setOrientation(Atlas2Ogre(getOrientation()));
}

/*
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
*/
/*
void AvatarDimeEntity::setVisible(bool vis)
{
	//TODO
	//mOgreEntity->setVisible(true);	
}
*/

//void AvatarDimeEntity::addMember(Entity *e) 
void AvatarDimeEntity::childAdded(Entity *e, Entity *e_) 
{
	mAvatar->mEntitiesToBeAddedToInventory.insert(e);
	//PersonDimeEntity::addMember(e);
	
}


/*void AvatarDimeEntity::rmvMember(Entity *e)*/
void AvatarDimeEntity::childRemoved(Entity *e, Entity *e_)
{
	mAvatar->mEntitiesToBeRemovedFromInventory.insert(e);
//	PersonDimeEntity::rmvMember(e);	
}



// void AvatarDimeEntity::onLocationChanged(Eris::Entity *oldLocation, Eris::Entity *newLocation)
// {
// 	return DimeEntity::onLocationChanged(oldLocation, newLocation);
// 	
// 	
// 	
// 	Ogre::Vector3 oldWorldPosition = getSceneNode()->getWorldPosition();
// 	DimeEntity* dimeEntity = dynamic_cast<DimeEntity*>(newLocation);
// 	Ogre::SceneNode* newOgreParentNode = dimeEntity->getSceneNode();
// 	
// /*	if (dimeEntity)
// 	{
// 		newOgreParentNode = dimeEntity->getSceneNode();
// 	} else {
// 		newOgreParentNode = DimeOgre::getSingleton().getSceneManager()->getSceneNode(newLocation->getId());
// 	}*/
// 		
// 	if (getSceneNode()->getParent()) {
// 		//detach from our current object and add to the new entity
// 		getSceneNode()->getParent()->removeChild(getSceneNode()->getName());
// 	}
// 	newOgreParentNode->addChild(getSceneNode());
// 	
// 
// //	Entity::setContainer(pr);
// 	Eris::Entity::onLocationChanged(oldLocation, newLocation);
// 	
// 	//we adjust the entity so it retains it's former position in the world
// 	Ogre::Vector3 newWorldPosition = getSceneNode()->getWorldPosition();
// 	getSceneNode()->translate(oldWorldPosition - newWorldPosition);
// }

Ogre::SceneNode* AvatarDimeEntity::getAvatarSceneNode()
{
	return mScaleNode;	
}




}

