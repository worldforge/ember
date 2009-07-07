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
#include "EmberEntity.h"
#include "EmberPhysicalEntity.h"
// #include "PersonEmberEntity.h"
#include "framework/ConsoleBackend.h"
#include "framework/Tokeniser.h"
#include "Avatar.h"
#include "GUIManager.h"
#include "model/Model.h"
#include "AvatarEmberEntity.h"
#include "EmberOgre.h"
#include "MousePicker.h"

#include <Eris/Entity.h>
#include <Eris/Avatar.h>
#include <OgreTagPoint.h>

namespace EmberOgre {


AvatarEmberEntity::AvatarEmberEntity(const std::string& id, Eris::TypeInfo* type, Eris::View* vw, Ogre::SceneManager* sceneManager, Eris::Avatar* erisAvatar) : EmberPhysicalEntity(id, type, vw, sceneManager),  SetAttachedOrientation("setattachedorientation", this, "Sets the orienation of an item attached to the avatar: <attachpointname> <x> <y> <z> <degrees>"),
mAvatar(0), mErisAvatar(erisAvatar)
{
}

AvatarEmberEntity::~AvatarEmberEntity()
{}


void AvatarEmberEntity::runCommand(const std::string &command, const std::string &args)
{
	if(SetAttachedOrientation == command) {
		Ember::Tokeniser tokeniser;
		tokeniser.initTokens(args);
		std::string attachPointName = tokeniser.nextToken();
		if (attachPointName != "") {
			std::string x = tokeniser.nextToken();
			std::string y = tokeniser.nextToken();
			std::string z = tokeniser.nextToken();
			std::string degrees = tokeniser.nextToken();
			if (x != "" && y != "" && z != "" && degrees != "") {
				Ogre::Degree ogreDegrees(Ogre::StringConverter::parseReal(degrees));
				Ogre::Quaternion rotation(ogreDegrees, Ogre::Vector3(Ogre::StringConverter::parseReal(x), Ogre::StringConverter::parseReal(y), Ogre::StringConverter::parseReal(z)));
				if (getModel()) {
					const Model::Model::AttachPointWrapperStore* attachPoints = getModel()->getAttachedPoints();
					if (attachPoints) {
						for (Model::Model::AttachPointWrapperStore::const_iterator I = attachPoints->begin(); I != attachPoints->end(); ++I) {
							if (I->AttachPointName == attachPointName) {
								I->TagPoint->setOrientation(rotation);
							}
						}
					}
				}
			}
		}
	}
}

void AvatarEmberEntity::init(const Atlas::Objects::Entity::RootEntity &ge, bool fromCreateOp)
{
	EmberPhysicalEntity::init(ge, fromCreateOp);
	mModel->setQueryFlags(MousePicker::CM_AVATAR);


}

void AvatarEmberEntity::onMoved()
{
	///If there's an avatar connected we'll let that handle the update of the Ogre node, and bypass the EmberPhysicalEntity
	if (getAvatar()) {
		parseMovementMode();
		getAvatar()->movedInWorld();
		Eris::Entity::onMoved();
	} else {
		EmberPhysicalEntity::onMoved();
	}
}

void AvatarEmberEntity::onImaginary(const Atlas::Objects::Root& act)
{
    Atlas::Message::Element attr;
    if (act->copyAttr("description", attr) != 0 || !attr.isString()) {
        return;
    }

	/// Make the message appear in the chat box
	GUIManager::getSingleton().AppendAvatarImaginary.emit(getName() + " " + attr.String());
}

/*
void AvatarEmberEntity::handleTalk(const std::string &msg)
{
	std::string message = "<";
	message.append(getName());
	message.append("> ");
	message.append(msg);
	GUIManager::getSingleton().AppendIGChatLine.emit(message);
	std::cout << "TRACE - AVATAR SAYS: [" << message << "]\n" << std::endl;
	Ember::ConsoleBackend::getSingletonPtr()->pushMessage(message);
}
*/
/*
void AvatarEmberEntity::setVisible(bool vis)
{
	//TODO
	//mOgreEntity->setVisible(true);
}
*/

//void AvatarEmberEntity::addMember(Entity *e)
void AvatarEmberEntity::onChildAdded(Entity *e)
{
	//mAvatar->EventAddedEntityToInventory.emit(static_cast<EmberEntity*>(e));
	EmberOgre::getSingleton().getAvatar()->mEntitiesToBeAddedToInventory.insert(e);
	//PersonEmberEntity::addMember(e);
	EmberPhysicalEntity::onChildAdded(e);

}


/*void AvatarEmberEntity::rmvMember(Entity *e)*/
void AvatarEmberEntity::onChildRemoved(Entity *e)
{
	EmberOgre::getSingleton().getAvatar()->EventRemovedEntityFromInventory.emit(static_cast<EmberEntity*>(e));
	EmberPhysicalEntity::onChildRemoved(e);
//	mAvatar->mEntitiesToBeRemovedFromInventory.insert(e);
//	PersonEmberEntity::rmvMember(e);
}



// void AvatarEmberEntity::onLocationChanged(Eris::Entity *oldLocation, Eris::Entity *newLocation)
// {
// 	return EmberEntity::onLocationChanged(oldLocation, newLocation);
//
//
//
// 	Ogre::Vector3 oldWorldPosition = getSceneNode()->getWorldPosition();
// 	EmberEntity* EmberEntity = dynamic_cast<EmberEntity*>(newLocation);
// 	Ogre::SceneNode* newOgreParentNode = EmberEntity->getSceneNode();
//
// /*	if (EmberEntity)
// 	{
// 		newOgreParentNode = EmberEntity->getSceneNode();
// 	} else {
// 		newOgreParentNode = EmberOgre::getSingleton().getSceneManager()->getSceneNode(newLocation->getId());
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

Ogre::SceneNode* AvatarEmberEntity::getAvatarSceneNode()
{
	return getScaleNode();
}




}

