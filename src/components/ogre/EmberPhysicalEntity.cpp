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
#include "model/SubModel.h"


#include "EmberEntity.h"
#include "EmberPhysicalEntity.h"

#include "EmberOgre.h"

#include "EmberEntityUserObject.h"
#include "ogreopcode/include/OgreCollisionManager.h"
#include "ogreopcode/include/OgreCollisionShape.h"
#include "ogreopcode/include/OgreCollisionObject.h"

namespace EmberOgre {

const char * const EmberPhysicalEntity::ACTION_STAND = "__movement_idle";
const char * const EmberPhysicalEntity::ACTION_RUN = "__movement_run";
const char * const EmberPhysicalEntity::ACTION_WALK = "__movement_walk";
const char * const EmberPhysicalEntity::ACTION_SWIM = "__movement_swim";
const char * const EmberPhysicalEntity::ACTION_FLOAT = "__movement_float";



EmberPhysicalEntity::EmberPhysicalEntity(const std::string& id, Eris::TypeInfo* ty, Eris::View* vw, Ogre::SceneManager* sceneManager, Ogre::SceneNode* nodeWithModel) : 
mScaleNode(nodeWithModel),
mModelAttachedTo(0), 
mModelMarkedToAttachTo(0),
EmberEntity(id, ty, vw, sceneManager),
mCurrentMovementAction(0)
{
	mModel = static_cast<Model*>(getScaleNode()->getAttachedObject(0));
//	loadAnimationsFromModel();
}

EmberPhysicalEntity::~EmberPhysicalEntity()
{
	delete mModel->getUserObject();
	delete mModel;
	Ogre::SceneNode *parent = static_cast<Ogre::SceneNode*>(getScaleNode()->getParent());
	if (parent) {
		parent->removeAndDestroyChild(getScaleNode()->getName());
	}
	
	//make sure it's not in the MotionManager
	//TODO: keep a marker in the entity so we don't need to call this for all entities
	MotionManager::getSingleton().removeAnimatedEntity(this);

/*

	mSceneManager->removeEntity(mOgreEntity);
	mSceneManager->removeEntity(mOgreEntity);
	
	delete mOgreEntity;
	delete mSceneNode;
	*/
}

EmberEntity* EmberPhysicalEntity::getEntityAttachedToPoint(const std::string attachPoint)
{
	//first check with the attach points
	std::string entityId;
	for(AttachedEntitiesStore::iterator I = mAttachedEntities.begin(); I != mAttachedEntities.end(); ++I) {
		if (I->second == attachPoint) {
			entityId = I->first;
			break;
		}
	}	

	if (entityId != "") {
		//then get the entity from the world
		EmberEntity* entity = EmberOgre::getSingleton().getEmberEntity(entityId);
		return entity;
	}
	return 0;
}


void EmberPhysicalEntity::setVisible(bool visible)
{
	EmberEntity::setVisible(visible);
	getScaleNode()->setVisible(visible && getLocation(), false);	
	//getModel()->setVisible(visible);
}

void EmberPhysicalEntity::init(const Atlas::Objects::Entity::GameEntity &ge)
{
	EmberEntity::init(ge);
	getModel()->setQueryFlags(EmberEntity::CM_ENTITY);

/*	assert(mOgreNode);
	assert(mScaleNode);*/
	scaleNode();
	getSceneNode()->addChild(getScaleNode());

	
/*	EmberEntityUserObject* userObject = new EmberEntityUserObject(this, getModel(), 0, 0);
	getModel()->setUserObject(userObject);*/
	connectEntities();
	
	//check if we should do delayed attachment
	if (mModelMarkedToAttachTo) {
		attachToPointOnModel(mAttachPointMarkedToAttachTo, mModelMarkedToAttachTo);
		mModelMarkedToAttachTo = 0;
		mAttachPointMarkedToAttachTo = "";
	}

	//NOTE: for now, add all particle systems. we will want to add some visibility flag or something in the future
	for (ParticleSystemSet::iterator I = mModel->getParticleSystems().begin(); I != mModel->getParticleSystems().end(); ++I) 
	{
		getScaleNode()->attachObject((*I)->getOgreParticleSystem());
	}

	onModeChanged(EmberEntity::MM_DEFAULT);
}

void EmberPhysicalEntity::connectEntities()
{
	Ogre::CollisionContext* collideContext = Ogre::CollisionManager::getSingletonPtr()->GetDefaultContext();
	const Model::SubModelSet submodels = getModel()->getSubmodels();
	EmberEntityUserObject::CollisionObjectStore collisionObjects;
	for (Model::SubModelSet::const_iterator I = submodels.begin(); I != submodels.end(); ++I)
	{
		std::string collideShapeName = std::string("entity_") + (*I)->getEntity()->getName();
		Ogre::CollisionShape *collideShape = Ogre::CollisionManager::getSingletonPtr()->NewShape(collideShapeName.c_str());
		collideShape->Load((*I)->getEntity());
		Ogre::CollisionObject* collideObject = collideContext->NewObject();
		collideObject->SetShape(collideShape);
		
		collideContext->AddObject(collideObject);
		
		collisionObjects.push_back(collideObject);
		//collideShape->setDebug(true);
		//collideShape->Visualize();
/*	
		EmberEntityUserObject* userObject = new EmberEntityUserObject(this, getModel(), (*I)->getEntity(), 0);
		(*I)->getEntity()->setUserObject(userObject);*/
	
	}
	EmberEntityUserObject* userObject = new EmberEntityUserObject(this, getModel(),  collisionObjects);
	getModel()->setUserObject(userObject);


}


void EmberPhysicalEntity::attachToPointOnModel(const std::string& point, Model* model)
{
	//if we're not initialized, delay attachment until after init
	if (!isInitialized()) {
		mModelMarkedToAttachTo = model;
		mAttachPointMarkedToAttachTo = point;
	} else {
		if (model->hasAttachPoint(point) && model->getSkeleton()) {
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

void EmberPhysicalEntity::showOgreBoundingBox(bool show)
{
	getScaleNode()->showBoundingBox(show);
}

// void EmberPhysicalEntity::showErisBoundingBox(bool show)
// {
// }

bool EmberPhysicalEntity::getShowOgreBoundingBox()
{
	return getScaleNode()->getShowBoundingBox();
}

// bool EmberPhysicalEntity::getShowErisBoundingBox()
// {
// }


void EmberPhysicalEntity::onAttrChanged(const std::string& str, const Atlas::Message::Element& v) {
	EmberEntity::onAttrChanged(str, v);
	
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
// 	if (str == "bbox") {
// 		scaleNode();
// 	}

	//check if the changed attribute should affect any particle systems
	if (mModel->hasParticles()) {
		ParticleSystemBindingsPtrSet bindings = mModel->getAllParticleSystemBindings();
		for (ParticleSystemBindingsPtrSet::iterator I = bindings.begin(); I != bindings.end(); ++I) {
			if ((*I)->getVariableName() == str && v.isNum()) {
				(*I)->scaleValue(v.asNum());
			}
		}
	}


}

void EmberPhysicalEntity::onModeChanged(MovementMode newMode)
{
/*	if (newMode != mMovementMode) 
	{*/
		const char * actionName;
		if (newMode == EmberEntity::MM_WALKING) {
			actionName = ACTION_WALK;
		} else if (newMode == EmberEntity::MM_RUNNING) {
			actionName = ACTION_WALK;
		} else if (newMode == EmberEntity::MM_SWIMMING) {
			actionName = ACTION_SWIM;
		} else {
			actionName = ACTION_STAND;
		}
		if (!mCurrentMovementAction || mCurrentMovementAction->getName() != actionName) {
			Action* newAction = mModel->getAction(actionName);
			mCurrentMovementAction = newAction;
			if (newAction) {
				MotionManager::getSingleton().addAnimatedEntity(this);
				mCurrentMovementAction->getAnimations()->setEnabled(true);
				
			} else {
				MotionManager::getSingleton().removeAnimatedEntity(this);
			}
		}
		//might set mCurrentMovementAction to 0
// 	}

	EmberEntity::onModeChanged(newMode);
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
			case ModelDefinition::MODEL_HEIGHT:
				scaleX = scaleY = scaleZ = fabs((wfMax.z() - wfMin.z()) / (ogreMax.y - ogreMin.y));		
				break;
			case ModelDefinition::MODEL_WIDTH:
				scaleX = scaleY = scaleZ = fabs((wfMax.x() - wfMin.x()) / (ogreMax.x - ogreMin.x));		
				break;
			case ModelDefinition::MODEL_DEPTH:
				scaleX = scaleY = scaleZ = fabs((wfMax.y() - wfMin.y()) / (ogreMax.z - ogreMin.z));		
				break;
			case ModelDefinition::MODEL_NONE:
				scaleX = scaleY = scaleZ = 1;
				break;
				
			case ModelDefinition::MODEL_ALL:
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


void EmberPhysicalEntity::updateMotion(Ogre::Real timeSlice)
{
	EmberEntity::updateMotion(timeSlice);
}


void EmberPhysicalEntity::updateAnimation(Ogre::Real timeSlice)
{
	if (mCurrentMovementAction) {
		//check if we're walking backward
		if (static_cast<int>((WFMath::Vector<3>(getVelocity()).rotate((getOrientation().inverse()))).x()) < 0) {
			mCurrentMovementAction->getAnimations()->addTime(-timeSlice);
		} else {
			mCurrentMovementAction->getAnimations()->addTime(timeSlice);
		}
	}
}

void EmberPhysicalEntity::setMoving(bool moving)
{
// 	MotionManager* motionManager = &MotionManager::getSingleton();
// 	if (moving) {
// 		if (getModel()->isAnimated()) {
// 			getModel()->stopAnimation("idle");
// 			getModel()->startAnimation("walk");
// 		}
// 	} else {
// 		if (getModel()->isAnimated()) {
// 			getModel()->stopAnimation("walk");
// 			getModel()->startAnimation("idle");
// 		}
// 	}
	EmberEntity::setMoving(moving);
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
