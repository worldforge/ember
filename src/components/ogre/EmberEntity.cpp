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

#include "EmberOgrePrerequisites.h"
#include "framework/Service.h"
#include "framework/ConsoleBackend.h"
#include "services/EmberServices.h"
#include "services/sound/SoundService.h"
#include "EmberEntityFactory.h"
#include "MotionManager.h"
#include "GUIManager.h"
#include "EmberEntity.h"

#include "EmberOgre.h"

//#include <Atlas/Objects/ObjectsFwd.h>

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
	Ogre::SceneNode *parent = dynamic_cast<Ogre::SceneNode*>(getSceneNode()->getParent());
	if (parent) {
		parent->removeAndDestroyChild(getSceneNode()->getName());
	}
	//mSceneManager->destroySceneNode(getSceneNode()->getName());
}

void EmberEntity::init(const Atlas::Objects::Entity::GameEntity &ge)
{
	Eris::Entity::init(ge);
	
	
	// set the Ogre node position and orientation based on Atlas data
	getSceneNode()->setPosition(Atlas2Ogre(getPosition()));
	Ogre::Quaternion orientation = Atlas2Ogre(getOrientation());
	getSceneNode()->setOrientation(orientation);
	S_LOG_INFO( "Entity " << getId() << "(" << getName() << ") placed at (" << getPosition().x() << "," << getPosition().y() << "," << getPosition().x() << ")")
}


void EmberEntity::createSceneNode()
{
	EmberEntity* container = dynamic_cast<EmberEntity*>(getLocation());
	if (container == NULL) {
		S_LOG_INFO( "ENTITY CREATED IN LIMBO: "<< this->getId() << " (" << this->getName() << ") \n" )

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
	getSceneNode()->setPosition(Atlas2Ogre(getPosition()));
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

void EmberEntity::setMoving(bool moving)
{
	// When a pig starts or stops moving, we'll emit a sound
	// here will be the sound emission
	std::string type = getType()->getName(); // Eris type as a string
	if(type.compare("pig")==0) { 	// if it's a pig
		if(moving) { 				// and the pig *starts* moving
			S_LOG_INFO( "THE PIG STARTS MOVING" );
			Ember::EmberServices::getInstance()->getSoundService()->playTestGrunt();
		} else {					// the pig is stopping
			S_LOG_INFO( "THE PIG STOPS MOVING" );
		}
	}
	
	// Call the overridden method 
	Eris::Entity::setMoving(moving);

}

void EmberEntity::onTalk(const Atlas::Objects::Root& talkArgs)
{
	
    if (!talkArgs->hasAttr("say")) {
        Eris::Entity::onTalk(talkArgs);
		return;
    }
	
	mSuggestedResponses.clear();
	if (talkArgs->hasAttr("responses")) {
		if (talkArgs->getAttr("responses").isList()) {
			const Atlas::Message::ListType & responseList = talkArgs->getAttr("responses").asList();
			Atlas::Message::ListType::const_iterator I = responseList.begin();
			for(; I != responseList.end(); ++I) {
				mSuggestedResponses.push_back(I->asString());
			}
		
		}
	}
	
	std::string msg = talkArgs->getAttr("say").asString();
    


	
	std::string message = "<";
	message.append(getName());
	message.append(",");
	const std::string type = getType()->getName(); // Eris type as a string
	message.append(type);
	message.append("> ");
	message.append(msg);
	S_LOG_INFO( "TRACE - ENTITY SAYS: [" << message << "]\n" )
	Ember::ConsoleBackend::getMainConsole()->pushMessage("TRACE - ENTITY SPEAKS");
	// Make the message appear in the chat box
	GUIManager::getSingleton().AppendIGChatLine.emit(msg, this);
	// Make a sound if it's the merchant speaking
	if(type.compare("merchant")==0) {
		S_LOG_INFO( "THE MERCHANT IS SPEAKING" )
		Ember::EmberServices::getInstance()->getSoundService()->playTestGYPH();
	}
	// Call the method of the base class (since we've overloaded it)
	Eris::Entity::onTalk(talkArgs);
}


void EmberEntity::onSoundAction( const Atlas::Objects::Operation::RootOperation & op )
{
	int i = 0;
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
			//don't cascade, only change the top node
			setVisible(true);	
		} else {
			setVisible(false);	
		}
		
	} else {
		setVisible(vis);
	}
}


void EmberEntity::setVisible(bool visible)
{
	getSceneNode()->setVisible(visible, false);	
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
		S_LOG_INFO( "SAME NEW LOCATION AS OLD FOR ENTITY: " << this->getId() << " (" << this->getName() << ")" );
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
			S_LOG_INFO( "ENTITY: " << this->getId() << " (" << this->getName() << ") RELOCATED TO: "<< newLocationEntity->getId() << " (" << newLocationEntity->getName() << ")" )
			
			getSceneNode()->setPosition(Atlas2Ogre(getPosition()));
			getSceneNode()->setOrientation(Atlas2Ogre(getOrientation()));
	} else {
		//add to the world
		S_LOG_INFO( "ENTITY RELOCATED TO LIMBO: "<< this->getId() << " (" << this->getName() << ")" );
//		mSceneManager->getRootSceneNode()->addChild(getSceneNode());
	}		
	
	checkVisibility(isVisible());
    std::stringstream ss;
	ss << "ENTITY HAS POSITION: " << getPosition() << " AND ORIENTATION: " <<  getOrientation();                                                      
	S_LOG_INFO( ss.str() )

	//we adjust the entity so it retains it's former position in the world
	Ogre::Vector3 newWorldPosition = getSceneNode()->getWorldPosition();
	getSceneNode()->translate(oldWorldPosition - newWorldPosition);
	
	Eris::Entity::onLocationChanged(oldLocation);
}

void EmberEntity::onAction(const Atlas::Objects::Operation::Action& act)
{
	
/*	std::string allattribs;
	
	//Atlas::Objects::BaseObjectData::const_iterator I = act->begin();
	std::list< std::string >::const_iterator I = act->getParents().begin();

	for (; I != act->getParents().end(); ++I) 
	{
		//const Atlas::Message::Element e = (const Atlas::Message::Element)(*I).second;
		allattribs.append((*I) + " : ");
	
	}*/
	
	std::string name = act->getName();
	
	GUIManager::getSingleton().setDebugText(std::string("Entity (") + getName() + ":" + getId() + ") action: " + name);
	S_LOG_INFO( std::string("Entity (") + getName() + ":" + getId() + ") action: " + name);
}

void EmberEntity::onImaginary(const Atlas::Objects::Root& act)
{
	GUIManager::getSingleton().setDebugText(std::string("Entity (") + getName() + ":" + getId() + ") imaginary: "+act->getName());
}


bool EmberEntity::allowVisibilityOfMember(EmberEntity* entity) {
	return true;
}

std::vector< std::string > EmberEntity::getSuggestedResponses( ) const
{
	return mSuggestedResponses;
}

bool EmberEntity::hasSuggestedResponses( ) const
{
	return mSuggestedResponses.size() > 0;
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






