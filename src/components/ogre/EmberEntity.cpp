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
#include "MotionManager.h"
#include "GUIManager.h"
#include "DimeEntityFactory.h"
#include "DimeEntity.h"
using namespace Ogre;

namespace DimeOgre {

/*eris 1.3
DimeEntity::DimeEntity(const Atlas::Objects::Entity::GameEntity &ge, Eris::TypeInfo* ty, Eris::View* vw, , Ogre::Entity* ogreEntity) : Eris::Entity(ge, ty, vw) 
*/
DimeEntity::DimeEntity(const Atlas::Objects::Entity::GameEntity &ge, Eris::World* vw,Ogre::SceneManager* sceneManager)
:
mSceneManager(sceneManager)
, Eris::Entity(ge, vw) 
{
	
	createSceneNode();
	
	// set the Ogre node position and orientation based on Atlas data
	getSceneNode()->setPosition(WF2OGRE_VECTOR3(1,1,1) * Atlas2Ogre(getPosition()));
	getSceneNode()->setOrientation(Atlas2Ogre(getOrientation()));
	std::cout << "Entity " << getID() << " placed at (" << getPosition().x() << "," << getPosition().y() << "," << getPosition().x() << ")" << std::endl;
	
}

DimeEntity::~DimeEntity()
{
}

void DimeEntity::createSceneNode()
{
	DimeEntity* container = dynamic_cast<DimeEntity*>(getContainer());
	if (container == NULL) {
		mOgreNode = static_cast<Ogre::SceneNode*>(mSceneManager->getRootSceneNode()->createChild(getID()));
	} else {
		Ogre::SceneNode * node = container->getSceneNode();
		if (node) {
			mOgreNode = static_cast<Ogre::SceneNode*>(node->createChild(getID()));
		} else {
			mOgreNode = static_cast<Ogre::SceneNode*>(mSceneManager->getRootSceneNode()->createChild(getID()));
		}
	}		
}

/*
 * return the scenenode to which this entity belongs
 */
SceneNode* DimeEntity::getSceneNode() {

	return mOgreNode;	
}


void DimeEntity::handleMove()
{
	getSceneNode()->setPosition(WF2OGRE_VECTOR3(1,1,1) * Atlas2Ogre(getPosition()));
	getSceneNode()->setOrientation(Atlas2Ogre(getOrientation()));
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

void DimeEntity::handleTalk(const std::string &msg)
{
	
	std::string message = "<";
	message.append(getName());
	message.append("> ");
	message.append(msg);
	GUIManager::getSingleton().appendOOGChatLine(message);
	std::cout << "TRACE - ENTITY SAYS: [" << message << "]\n" << std::endl;
	dime::ConsoleBackend::getMainConsole()->pushMessage("TRACE - ENTITY SPEAKS");
}


void DimeEntity::setVisible(bool vis)
{
//	mOgreEntity->setVisible(vis);	
}


void DimeEntity::adjustHeightPositionForContainedNode(DimeEntity* const entity) 
{
	//for generic entities we set the height to 0
	Ogre::SceneNode* sceneNode = entity->getSceneNode();
	Ogre::Vector3 position = sceneNode->getPosition();
	sceneNode->setPosition(position.x, 0,position.z);
}


void DimeEntity::setContainer(Entity *pr)
{
		
	DimeEntity* dimeEntity = dynamic_cast<DimeEntity*>(pr);
	if (getSceneNode()) {
		//detach from our current object
		getSceneNode()->getParent()->removeChild(getSceneNode()->getName());
	}
	if (dimeEntity) {
		// add to the new entity
		dimeEntity->getSceneNode()->addChild(getSceneNode());
				
	} else {
		//add to the world
		mSceneManager->getRootSceneNode()->addChild(getSceneNode());
	}		
	Eris::Entity::setContainer(pr);
}


/*
void DimeEntity::addMember(Entity *e) 
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

void DimeEntity::rmvMember(Entity *e)
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
void DimeEntity::markAsMainAvatar(Ogre::SceneManager* sceneManager)
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

