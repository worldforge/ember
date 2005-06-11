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


#include "framework/ConsoleBackend.h"
#include "MotionManager.h"
#include "GUIManager.h"
#include "EmberEntityFactory.h"
#include "model/Model.h"


#include "EmberEntity.h"
#include "EmberPhysicalEntity.h"

#include "EmberOgre.h"

namespace EmberOgre {


EmberPhysicalEntity::EmberPhysicalEntity(const std::string& id, Eris::TypeInfo* ty, Eris::View* vw, Ogre::SceneManager* sceneManager, Ogre::SceneNode* nodeWithModel) : 
mAnimationState_Walk(NULL),
mScaleNode(nodeWithModel),
mModelAttachedTo(0), 
mModelMarkedToAttachTo(0),
EmberEntity(id, ty, vw, sceneManager)
{
	mModel = static_cast<Model*>(getScaleNode()->getAttachedObject(0));
	loadAnimationsFromModel();
}

EmberPhysicalEntity::~EmberPhysicalEntity()
{
	delete mModel;
	Ogre::SceneNode *parent = static_cast<Ogre::SceneNode*>(getScaleNode()->getParent());
	if (parent) {
		parent->removeAndDestroyChild(getScaleNode()->getName());
	}
/*
  	if (mAnimationState_Walk) {
		MotionManager::getSingleton().removeAnimation(mAnimationState_Walk);
	}
	mSceneManager->removeEntity(mOgreEntity);
	mSceneManager->removeEntity(mOgreEntity);
	
	delete mOgreEntity;
	delete mSceneNode;
	*/
}

void EmberPhysicalEntity::setVisible(bool visible)
{
	EmberEntity::setVisible(visible);
	getScaleNode()->setVisible(visible, false);	
	//getModel()->setVisible(visible);
}

void EmberPhysicalEntity::init(const Atlas::Objects::Entity::GameEntity &ge)
{
	EmberEntity::init(ge);
	getModel()->setQueryFlags(EmberEntity::CM_UNDEFINED);

/*	assert(mOgreNode);
	assert(mScaleNode);*/
	scaleNode();
	getSceneNode()->addChild(getScaleNode());

	getModel()->setUserObject(this);
	
	//check if we should do delayed attachment
	if (mModelMarkedToAttachTo) {
		attachToPointOnModel(mAttachPointMarkedToAttachTo, mModelMarkedToAttachTo);
		mModelMarkedToAttachTo = 0;
		mAttachPointMarkedToAttachTo = "";
	}
}


void EmberPhysicalEntity::loadAnimationsFromModel()
{
	if (getModel()->isAnimated()) {
		//start with the idle animation
		getModel()->startAnimation("idle");
	}
/*
 * 	Ogre::AnimationStateSet* states = mModel->getAllAnimationStates();
	if (states->size()) {
		Ogre::AnimationStateSet::iterator itr_end = states->end();
		Ogre::AnimationStateSet::iterator itr;
		itr = states->find(std::string("Walk"));
		if (itr == itr_end) {
			itr = states->find(std::string("walk"));
		}
		if (itr != itr_end) {
			std::cout << "Found animation state";
			mAnimationState_Walk = &(itr->second);
			MotionManager::getSingleton().addAnimation(mAnimationState_Walk);
		}
	}	
	*/
}


void EmberPhysicalEntity::attachToPointOnModel(const std::string& point, Model* model)
{
	//if we're not initialized, delay attachment until after init
	if (!isInitialized()) {
		mModelMarkedToAttachTo = model;
		mAttachPointMarkedToAttachTo = point;
	} else {
		if (model->hasAttachPoint(point)) {
			getScaleNode()->detachObject(getModel());
			getModel()->setVisible(true);
			model->attachObjectToAttachPoint( point, getModel(), getScaleNode()->getScale());
			mModelAttachedTo = model;
		}
	}
}

void EmberPhysicalEntity::detachFromModel()
{
	if (mModelAttachedTo) {
		mModelAttachedTo->detachObjectFromBone(getModel()->getName());
		getScaleNode()->attachObject(getModel());
		checkVisibility(isVisible());
		mModelAttachedTo = 0;
	}
}


void EmberPhysicalEntity::onAttrChanged(const std::string& str, const Atlas::Message::Element& v) {
    if (str == "right_hand_wield") {
        std::cout << "set right_hand_wield to " << v.asString() << std::endl;
        std::string id = v.asString();
        if (id.empty()) {
			detachEntity("right_hand_wield");
            //m_attached.erase("right_hand_wield");
        } else {
			//detach first
			detachEntity("right_hand_wield");
			attachEntity("right_hand_wield", id);
        }
		return;		
    }
	EmberEntity::onAttrChanged(str, v);

}



void EmberPhysicalEntity::onChildAdded(Entity *e) 
{
	Eris::Entity::onChildAdded(e);
	//see if it's in our attach map
	if (mAttachedEntities.find(e->getId()) != mAttachedEntities.end()) {
		EmberEntity* emberEntity = static_cast<EmberEntity*>(e);
		emberEntity->attachToPointOnModel(mAttachedEntities[e->getId()], getModel());
	}
	
/*	if (hasChild(entityId)) {
	}*/
	
	
}



void EmberPhysicalEntity::onChildRemoved(Entity *e)
{
	//NOTE: we don't have to do detachment here, like we do attachment in onChildAdded, since that is done by the EmberEntity::onLocationChanged(...) method
	Eris::Entity::onChildRemoved(e);
}


void EmberPhysicalEntity::scaleNode() {
	if (getModel()->getRotation()) {
		getScaleNode()->rotate(Ogre::Vector3::UNIT_Y,(Ogre::Degree)getModel()->getRotation());
	}
		
	const Ogre::AxisAlignedBox ogreBoundingBox = getModel()->getWorldBoundingBox(true);
	const Ogre::Vector3 ogreMax = ogreBoundingBox.getMaximum();
	const Ogre::Vector3 ogreMin = ogreBoundingBox.getMinimum();

	if (hasBBox()) {

		const WFMath::AxisBox<3> wfBoundingBox = getBBox();	
		const WFMath::Point<3> wfMax = wfBoundingBox.highCorner();
		const WFMath::Point<3> wfMin = wfBoundingBox.lowCorner();
		
		Ogre::Real scaleX;		
		Ogre::Real scaleY;		
		Ogre::Real scaleZ;		

		
		
		switch (getModel()->getUseScaleOf()) {
			case Model::MODEL_HEIGHT:
				scaleX = scaleY = scaleZ = fabs((wfMax.z() - wfMin.z()) / (ogreMax.y - ogreMin.y));		
				break;
			case Model::MODEL_WIDTH:
				scaleX = scaleY = scaleZ = fabs((wfMax.x() - wfMin.x()) / (ogreMax.x - ogreMin.x));		
				break;
			case Model::MODEL_DEPTH:
				scaleX = scaleY = scaleZ = fabs((wfMax.y() - wfMin.y()) / (ogreMax.z - ogreMin.z));		
				break;
			case Model::MODEL_NONE:
				scaleX = scaleY = scaleZ = 1;
				break;
				
			case Model::MODEL_ALL:
			default:				
				scaleX = fabs((wfMax.x() - wfMin.x()) / (ogreMax.x - ogreMin.x));		
				scaleY = fabs((wfMax.z() - wfMin.z()) / (ogreMax.y - ogreMin.y));		
				scaleZ = fabs((wfMax.y() - wfMin.y()) / (ogreMax.z - ogreMin.z));		
		}
			
		
		//Ogre::Real finalScale = std::max(scaleX, scaleY);
		//finalScale = std::max(finalScale, scaleZ);
		getScaleNode()->setScale(scaleX, scaleY, scaleZ);
		
	} else if (!getModel()->getScale()) {
		//set to small size
		
		Ogre::Real scaleX = (0.25 / (ogreMax.x - ogreMin.x));		
		Ogre::Real scaleY = (0.25 / (ogreMax.y - ogreMin.y));		
		Ogre::Real scaleZ = (0.25 / (ogreMax.z - ogreMin.z));		
		getScaleNode()->setScale(scaleX, scaleY, scaleZ);
	}		

	if (getModel()->getScale()) {
		if (getModel()->getScale() != 1) {
			//only scale if it's not 1
			getScaleNode()->scale(getModel()->getScale(), getModel()->getScale(), getModel()->getScale());
		}
	}

		
}


void EmberPhysicalEntity::onMoved()
{
	getSceneNode()->setPosition(Atlas2Ogre(getPosition()));
	getSceneNode()->setOrientation(Atlas2Ogre(getOrientation()));
	MotionManager* motionManager = &MotionManager::getSingleton();
	if (getVelocity() != WFMath::Vector<3>().zero()) {
		//the entity is moving
		motionManager->addEntity(this);
		if (getModel()->isAnimated()) {
			getModel()->stopAnimation("idle");
			getModel()->startAnimation("walk");
		}
	} else {
		//the entity has stopped moving
		motionManager->removeEntity(this);
		if (getModel()->isAnimated()) {
			getModel()->stopAnimation("walk");
			getModel()->startAnimation("idle");
		}
	}
	Eris::Entity::onMoved();
	//Root::getSingleton().getAutoCreatedWindow()->setDebugText(std::string("Moved: " + _id) );
}

void EmberPhysicalEntity::detachEntity(const std::string & attachPoint)
{
	std::string entityId;
	for(AttachedEntitiesStore::iterator I = mAttachedEntities.begin(); I != mAttachedEntities.end(); ++I) {
		if (I->second == attachPoint) {
			entityId = I->first;
			break;
		}
	}
	
	if (entityId != "") {
		mAttachedEntities.erase(entityId);
		if (hasChild(entityId)) {
			//we already have the entity, do the detachment
			EmberEntity* entity = EmberOgre::getSingleton().getEntity(entityId);
			if (entity) {
				entity->detachFromModel();
			}
		}
	}
}
	
void EmberPhysicalEntity::attachEntity(const std::string & attachPoint, const std::string & entityId)
{
	mAttachedEntities[entityId] = attachPoint;
	if (hasChild(entityId)) {
		//we already have the entity, do the attachment now, else we will just wait for the onChildAdded event
		EmberEntity* entity = EmberOgre::getSingleton().getEntity(entityId);
		if (entity) {
			entity->attachToPointOnModel(attachPoint, getModel());
		}
	}

}



/*
void EmberPhysicalEntity::handleTalk(const std::string &msg)
{
	
    std::string message = "<";
    message.append(getName());
    message.append("> ");
    message.append(msg);
	std::cout << "TRACE - ENTITY SAYS: [" << message << "]\n" << std::endl;
	Ember::ConsoleBackend::getMainConsole()->pushMessage(message);
}
*/



/*
void EmberPhysicalEntity::setContainer(Entity *pr)
{
		
	EmberEntity* EmberEntity = dynamic_cast<EmberEntity*>(pr);
	if (EmberEntity) {
		//detach from our current object and add to the new entity
		getSceneNode()->getParent()->removeChild(getSceneNode()->getName());
		EmberEntity->getSceneNode()->addChild(getSceneNode());
				
	} else {
		//detach from our current object and add to the world
		getSceneNode()->getParent()->removeChild(getSceneNode()->getName());
		getSceneNode()->getCreator()->getRootSceneNode()->addChild(getSceneNode());
	}		
	
}
*/






}
