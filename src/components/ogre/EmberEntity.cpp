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
#include "framework/ConsoleBackend.h"
#include "EmberEntityFactory.h"
#include "MotionManager.h"
#include "GUIManager.h"
#include "EmberEntity.h"

#include "EmberOgre.h"

using namespace Ogre;

namespace EmberOgre {

/*eris 1.3
EmberEntity::EmberEntity(const Atlas::Objects::Entity::GameEntity &ge, Eris::TypeInfo* ty, Eris::View* vw, , Ogre::Entity* ogreEntity) : Eris::Entity(ge, ty, vw) 
*/
EmberEntity::EmberEntity(const std::string& id, Eris::TypeInfo* ty, Eris::View* vw,Ogre::SceneManager* sceneManager)
:
mSceneManager(sceneManager)
, mView(vw)
, Eris::Entity(id, ty, vw) 
{
	createSceneNode();
}

EmberEntity::~EmberEntity()
{
}

void EmberEntity::init(const Atlas::Objects::Entity::GameEntity &ge)
{
	Eris::Entity::init(ge);
	
	
	// set the Ogre node position and orientation based on Atlas data
	getSceneNode()->setPosition(WF2OGRE_VECTOR3(1,1,1) * Atlas2Ogre(getPosition()));
	Ogre::Quaternion orientation = Atlas2Ogre(getOrientation());
	getSceneNode()->setOrientation(orientation);
	std::cout << "Entity " << getId() << "(" << getName() << ") placed at (" << getPosition().x() << "," << getPosition().y() << "," << getPosition().x() << ")" << std::endl;
}


void EmberEntity::createSceneNode()
{
	EmberEntity* container = dynamic_cast<EmberEntity*>(getLocation());
	if (container == NULL) {
		std::cout << "ENTITY CREATED IN LIMBO: "<< this->getId() << " (" << this->getName() << ") \n" << std::endl;

		//mSceneManager->createSceneNode(getId());
		mOgreNode = static_cast<Ogre::SceneNode*>(mSceneManager->createSceneNode(getId()));
//		mOgreNode = static_cast<Ogre::SceneNode*>(mSceneManager->getRootSceneNode()->createChild(getId()));
		
	} else {
		Ogre::SceneNode * node = container->getSceneNode();
		//node->createChild(getId());
		mOgreNode = static_cast<Ogre::SceneNode*>(node->createChild(getId()));
/*		if (node) {
			mOgreNode = static_cast<Ogre::SceneNode*>(node->createChild(getId()));
		} else {
			mOgreNode = static_cast<Ogre::SceneNode*>(mSceneManager->getRootSceneNode()->createChild(getId()));
		}*/
	}		
}




void EmberEntity::onMoved()
{
	getSceneNode()->setPosition(WF2OGRE_VECTOR3(1,1,1) * Atlas2Ogre(getPosition()));
	getSceneNode()->setOrientation(Atlas2Ogre(getViewOrientation()));
	Eris::Entity::onMoved();
	/*
	getSceneNode()->setPosition(WF2OGRE_VECTOR3(1,1,1) * Atlas2Ogre(getPosition()));
	getSceneNode()->setOrientation(Atlas2Ogre(getOrientation()));
	MotionManager* motionManager = &MotionManager::getSingleton();
	if (getVelocity() != WFMath::Vector<3>().zero()) {
		motionManager->addEntity(this);
		if (mAnimationState_Walk) {
			mAnimationState_Walk->setEnabled(true);
//			motionManager->addAnimation(mAnimationState_Walk);
		}
	} else {
		motionManager->removeEntity(this);
		if (mAnimationState_Walk) {
			mAnimationState_Walk->setEnabled(false);
//			motionManager->removeAnimation(mAnimationState_Walk);
		}
	}
	//Root::getSingleton().getAutoCreatedWindow()->setDebugText(std::string("Moved: " + _id) );
	 */
}

void EmberEntity::onTalk(const Atlas::Objects::Root& talkArgs)
{
	
    if (!talkArgs->hasAttr("say")) {
        Eris::Entity::onTalk(talkArgs);
		return;
    }
	std::string msg = talkArgs->getAttr("say").asString();
    


	std::string message = "<";
	message.append(getName());
	message.append("> ");
	message.append(msg);
	GUIManager::getSingleton().AppendIGChatLine.emit(message);
	std::cout << "TRACE - ENTITY SAYS: [" << message << "]\n" << std::endl;
	Ember::ConsoleBackend::getMainConsole()->pushMessage("TRACE - ENTITY SPEAKS");
	Eris::Entity::onTalk(talkArgs);
}


void EmberEntity::onVisibilityChanged(bool vis)
{
	checkVisibility(vis);
	Eris::Entity::onVisibilityChanged(vis);
//	mOgreEntity->setVisible(vis);	
}

void EmberEntity::checkVisibility(bool vis)
{
	EmberEntity* container = dynamic_cast<EmberEntity*>(getLocation());
	if (container) {
		//check with the parent first if we should show ourselves
		if (vis && container->allowVisibilityOfMember(this)) {
			getSceneNode()->setVisible(true);	
		} else {
			getSceneNode()->setVisible(false);	
		}
		
	} else {
		getSceneNode()->setVisible(vis);
	}
}

void EmberEntity::adjustHeightPositionForContainedNode(EmberEntity* const entity) 
{
	//for generic entities we set the height to 0
	Ogre::SceneNode* sceneNode = entity->getSceneNode();
	Ogre::Vector3 position = sceneNode->getPosition();
	sceneNode->setPosition(position.x, 0,position.z);
}

void EmberEntity::onLocationChanged(Eris::Entity *oldLocation)
{
//	return Eris::Entity::onLocationChanged(oldLocation, newLocation);
	
	if (getLocation() == oldLocation)
	{
		std::cout << "SAME NEW LOCATION AS OLD FOR ENTITY: " << this->getId() << " (" << this->getName() << ")" << std::endl;
		return Eris::Entity::onLocationChanged(oldLocation);
	
	}

	EmberEntity* newLocationEntity = dynamic_cast<EmberEntity*>(getLocation());
	
	Ogre::Vector3 oldWorldPosition = getSceneNode()->getWorldPosition();
	
	if (getSceneNode()->getParentSceneNode()) {
		//detach from our current object
		getSceneNode()->getParentSceneNode()->removeChild(getSceneNode()->getName());
	}
	if (newLocationEntity) {

		
			// add to the new entity
			newLocationEntity->getSceneNode()->addChild(getSceneNode());
			std::cout << "ENTITY: " << this->getId() << " (" << this->getName() << ") RELOCATED TO: "<< newLocationEntity->getId() << " (" << newLocationEntity->getName() << ")" << std::endl;
			
			getSceneNode()->setPosition(Atlas2Ogre(getPosition()));
			getSceneNode()->setOrientation(Atlas2Ogre(getOrientation()));
	} else {
		//add to the world
		std::cout << "ENTITY RELOCATED TO LIMBO: "<< this->getId() << " (" << this->getName() << ")" << std::endl;
//		mSceneManager->getRootSceneNode()->addChild(getSceneNode());
	}		
	
	checkVisibility(isVisible());
	
	std::cout << "ENTITY HAS POSITION: " << getPosition() << " AND ORIENTATION: " << getOrientation() << std::endl;

	//we adjust the entity so it retains it's former position in the world
	Ogre::Vector3 newWorldPosition = getSceneNode()->getWorldPosition();
	getSceneNode()->translate(oldWorldPosition - newWorldPosition);
	
	Eris::Entity::onLocationChanged(oldLocation);
}

void EmberEntity::onAction(const Atlas::Objects::Root& act)
{
	GUIManager::getSingleton().setDebugText(std::string("Entity (") + getName() + ":" + getId() + ") action: " + act->getAttr("action").asString());
	std::cout << std::string("Entity (") + getName() + ":" + getId() + ") action: " + act->getAttr("action").asString() << "\n";
}

void EmberEntity::onImaginary(const Atlas::Objects::Root& act)
{
	GUIManager::getSingleton().setDebugText(std::string("Entity (") + getName() + ":" + getId() + ") imaginary: "+act->getName());
}


bool EmberEntity::allowVisibilityOfMember(EmberEntity* entity) {
	return true;
}

/*
void EmberEntity::addMember(Entity *e) 
{
	try{
		SceneNode* sceneNode = mSceneManager->getSceneNode(e->getID());

	
		//sceneNode->getParent()->removeChild(sceneNode->getName());	
		getSceneNode()->addChild(sceneNode);
	} catch(Ogre::Exception ex) {
		int i = 1;
		//the contained item haven't been initialised yet
		//this is no problem though because when it do get initialized
		//it's container will be set
	}

	Entity::addMember(e);
	
}

void EmberEntity::rmvMember(Entity *e)
{
	try{
		getSceneNode()->removeChild(e->getID());
	} catch(Ogre::Exception ex) {
		int i = 1;
		//the contained item haven't been initialised yet
		//this is no problem though because when it do get initialized
		//it's container will be set
	}
	Entity::rmvMember(e);	
}
*/
/*
void EmberEntity::markAsMainAvatar(Ogre::SceneManager* sceneManager)
{
	mIsMainAvatar = true;
	if (mOgreEntity != NULL) {
		delete mOgreEntity;
	}
	mOgreEntity = sceneManager->createEntity(getID(), "robot.mesh");
	mOgreNode->attachObject(mOgreEntity);
	mOgreEntity->setUserObject(this);
}

*/
}

