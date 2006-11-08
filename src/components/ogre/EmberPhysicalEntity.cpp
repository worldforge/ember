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

#include "EmberPhysicalEntity.h"


#include "framework/ConsoleBackend.h"
#include "MotionManager.h"
#include "model/Model.h"
#include "model/SubModel.h"
#include "model/ParticleSystemBinding.h"
#include "model/Action.h"


#include <OgreException.h>

#include "EmberOgre.h"

#include "EmberEntityUserObject.h"
#include "OpcodeCollisionDetector.h"

#include <Eris/Entity.h>
#include <Eris/View.h>
#include <Eris/TypeInfo.h>


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
	mModel = static_cast<Model::Model*>(getScaleNode()->getAttachedObject(0));

	///make a copy of the original bbox
	mDefaultOgreBoundingBox = mModel->getWorldBoundingBox(true);

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
	
	///make sure it's not in the MotionManager
	///TODO: keep a marker in the entity so we don't need to call this for all entities
	MotionManager::getSingleton().removeAnimatedEntity(this);

/*

	mSceneManager->removeEntity(mOgreEntity);
	mSceneManager->removeEntity(mOgreEntity);
	
	delete mOgreEntity;
	delete mSceneNode;
	*/
}

EmberEntity* EmberPhysicalEntity::getEntityAttachedToPoint(const std::string& attachPoint)
{
	///first check with the attach points
	std::string entityId;
	for(AttachedEntitiesStore::iterator I = mAttachedEntities.begin(); I != mAttachedEntities.end(); ++I) {
		if (I->second == attachPoint) {
			entityId = I->first;
			break;
		}
	}	

	if (entityId != "") {
		///then get the entity from the world
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

void EmberPhysicalEntity::init(const Atlas::Objects::Entity::RootEntity &ge, bool fromCreateOp)
{
	onModeChanged(EmberEntity::MM_DEFAULT);
	EmberEntity::init(ge, fromCreateOp);
	getModel()->setQueryFlags(EmberEntity::CM_ENTITY);

/*	assert(mOgreNode);
	assert(mScaleNode);*/
	
	//if there is no bounding box, scaleNode hasn't been called, so do it here
/*	if (!hasBBox()) {
		scaleNode();
	}*/
	getSceneNode()->addChild(getScaleNode());
	
	//translate the scale node according to the translate defined in the model
// 	getScaleNode()->translate(getModel()->getDefinition()->getTranslate());
	initFromModel();
	
/*	EmberEntityUserObject* userObject = new EmberEntityUserObject(this, getModel(), 0, 0);
	getModel()->setUserObject(userObject);*/
	
	//check if we should do delayed attachment
	if (mModelMarkedToAttachTo) {
		attachToPointOnModel(mAttachPointMarkedToAttachTo, mModelMarkedToAttachTo);
		mModelMarkedToAttachTo = 0;
		mAttachPointMarkedToAttachTo = "";
	}

	//NOTE: for now, add all particle systems. we will want to add some visibility flag or something in the future
	for (Model::ParticleSystemSet::iterator I = mModel->getParticleSystems().begin(); I != mModel->getParticleSystems().end(); ++I) 
	{
		getScaleNode()->attachObject((*I)->getOgreParticleSystem());
	}
	
	getModel()->Reloaded.connect(sigc::mem_fun(*this, &EmberPhysicalEntity::Model_Reloaded));

}

void EmberPhysicalEntity::initFromModel()
{
	getScaleNode()->setOrientation(Ogre::Quaternion::IDENTITY);
	///rotate node to fit with WF space
	///perhaps this is something to put in the model spec instead?
	getScaleNode()->rotate(Ogre::Vector3::UNIT_Y,(Ogre::Degree)90);
	getScaleNode()->rotate(getModel()->getRotation());
	
	scaleNode();
	
	getScaleNode()->setPosition(Ogre::Vector3::ZERO);
	///translate the scale node according to the translate defined in the model
	getScaleNode()->translate(getModel()->getDefinition()->getTranslate());

	connectEntities();

}

void EmberPhysicalEntity::connectEntities()
{
	if (getModel()->getUserObject()) {
		delete getModel()->getUserObject();
	}
	ICollisionDetector* collisionDetector = new OpcodeCollisionDetector(getModel());
	EmberEntityUserObject* userObject = new EmberEntityUserObject(this, getModel(),  collisionDetector);
	getModel()->setUserObject(userObject);
		
/*		OgreOpcode::CollisionContext* collideContext = OgreOpcode::CollisionManager::getSingletonPtr()->getDefaultContext();
		const Model::Model::SubModelSet& submodels = getModel()->getSubmodels();
		EmberEntityUserObject::CollisionObjectStore collisionObjects;
		for (Model::Model::SubModelSet::const_iterator I = submodels.begin(); I != submodels.end(); ++I)
		{
			std::string collideShapeName(std::string("entity_") + (*I)->getEntity()->getName());
			OgreOpcode::MeshCollisionShape *collideShape = OgreOpcode::CollisionManager::getSingletonPtr()->createMeshCollisionShape(collideShapeName.c_str());
			collideShape->load((*I)->getEntity());
			OgreOpcode::CollisionObject* collideObject = collideContext->newObject(collideShapeName);
			collideObject->setShape(collideShape);
			
			collideContext->addObject(collideObject);
			
			collisionObjects.push_back(collideObject);
// 			collideObject->setDebug(true, false, false, false);
		
		}*/
		
// 	}

}


void EmberPhysicalEntity::attachToPointOnModel(const std::string& point, Model::Model* model)
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


bool EmberPhysicalEntity::getShowOgreBoundingBox()
{
	return getScaleNode()->getShowBoundingBox();
}

Model::Model* EmberPhysicalEntity::getModel() const
{
	return mModel;	
}

void EmberPhysicalEntity::Model_Reloaded()
{
	initFromModel();
}


void EmberPhysicalEntity::processWield(const std::string& wieldName, const Atlas::Message::Element& idElement)
{
	S_LOG_VERBOSE("Set " << wieldName << " to " << idElement.asString());
	const std::string& id = idElement.asString();
	if (id.empty()) {
		detachEntity(wieldName);
	} else {
		//detach first
		detachEntity(wieldName);
		attachEntity(wieldName, id);
	}

}

void EmberPhysicalEntity::onAttrChanged(const std::string& str, const Atlas::Message::Element& v) {
	EmberEntity::onAttrChanged(str, v);
	
	///this is kind of a hack, but it allows characters to wield other entities in their hands
	if (str == "right_hand_wield" || str == "left_hand_wield") {
		processWield(str, v);
		return;		
	}
// 	if (str == "bbox") {
// 		scaleNode();
// 	}

	//check if the changed attribute should affect any particle systems
	if (mModel->hasParticles()) {
		const Model::ParticleSystemBindingsPtrSet& bindings = mModel->getAllParticleSystemBindings();
		for (Model::ParticleSystemBindingsPtrSet::const_iterator I = bindings.begin(); I != bindings.end(); ++I) {
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
			actionName = ACTION_RUN;
		} else if (newMode == EmberEntity::MM_SWIMMING) {
			actionName = ACTION_SWIM;
		} else {
			actionName = ACTION_STAND;
		}
		if (!mCurrentMovementAction || mCurrentMovementAction->getName() != actionName) {
			///first disable the current action
			if (mCurrentMovementAction) {
				mCurrentMovementAction->getAnimations()->setEnabled(false);
			}
			
			Model::Action* newAction = mModel->getAction(actionName);
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
		
	getScaleNode()->setScale(1, 1, 1);
	
	const Ogre::Vector3& ogreMax = mDefaultOgreBoundingBox.getMaximum();
	const Ogre::Vector3& ogreMin = mDefaultOgreBoundingBox.getMinimum();
	
	if (hasBBox()) {

		const WFMath::AxisBox<3>& wfBoundingBox = getBBox();	
		const WFMath::Point<3>& wfMax = wfBoundingBox.highCorner();
		const WFMath::Point<3>& wfMin = wfBoundingBox.lowCorner();
		
		Ogre::Real scaleX;		
		Ogre::Real scaleY;		
		Ogre::Real scaleZ;		

		
		
		switch (getModel()->getUseScaleOf()) {
			case Model::ModelDefinition::MODEL_HEIGHT:
				scaleX = scaleY = scaleZ = fabs((wfMax.z() - wfMin.z()) / (ogreMax.y - ogreMin.y));		
				break;
			case Model::ModelDefinition::MODEL_WIDTH:
				scaleX = scaleY = scaleZ = fabs((wfMax.x() - wfMin.x()) / (ogreMax.x - ogreMin.x));		
				break;
			case Model::ModelDefinition::MODEL_DEPTH:
				scaleX = scaleY = scaleZ = fabs((wfMax.y() - wfMin.y()) / (ogreMax.z - ogreMin.z));		
				break;
			case Model::ModelDefinition::MODEL_NONE:
				scaleX = scaleY = scaleZ = 1;
				break;
				
			case Model::ModelDefinition::MODEL_ALL:
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

const Ogre::Vector3& EmberPhysicalEntity::getOffsetForContainedNode(const Ogre::Vector3& position, EmberEntity* const entity)
{
	///if the model has an offset specified, use that, else just send to the base class
	const Ogre::Vector3& offset = getModel()->getDefinition()->getContentOffset();
	if (offset != Ogre::Vector3::ZERO) {
		return offset;
	} else {
		return EmberEntity::getOffsetForContainedNode(position, entity);
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
	
	for (ActionStore::iterator I = mActiveActions.begin() ; I != mActiveActions.end(); ++I) {
		if ((*I)->getAnimations()->hasCompleted()) {
			(*I)->getAnimations()->setEnabled(false);
			mActiveActions.erase(I);
		} else {
			(*I)->getAnimations()->addTime(timeSlice);
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

void EmberPhysicalEntity::onBboxChanged()
{
	EmberEntity::onBboxChanged();
	scaleNode();
}

const Ogre::AxisAlignedBox& EmberPhysicalEntity::getWorldBoundingBox(bool derive) const
{
	return getModel()->getWorldBoundingBox(true);
}

void EmberPhysicalEntity::onAction(const Atlas::Objects::Operation::RootOperation& act)
{
	
/*	std::string allattribs;
	
	//Atlas::Objects::BaseObjectData::const_iterator I = act->begin();
	std::list< std::string >::const_iterator I = act->getParents().begin();

	for (; I != act->getParents().end(); ++I) 
	{
		//const Atlas::Message::Element e = (const Atlas::Message::Element)(*I).second;
		allattribs.append((*I) + " : ");
	
	}*/
	
	const std::list<std::string> &p = act->getParents();
	std::list<std::string>::const_iterator I = p.begin();
	
	if (I == p.end()) return;
	
	const std::string& name = *I;
	
	S_LOG_VERBOSE( "Entity: " << this->getId() << " (" << this->getName() << ") action: " << name);
/*	if (debug) {
	printf("Entity %s (%s) received action: %s\n", getName().c_str(), getId().c_str(), a.c_str());
	}*/
	
	
//	const std::string& name = act->getName();
	
	Model::Action* newAction = mModel->getAction(name);
	if (newAction) {
		MotionManager::getSingleton().addAnimatedEntity(this);
		mCurrentMovementAction->getAnimations()->setEnabled(true);
	}

}

bool EmberPhysicalEntity::allowVisibilityOfMember(EmberEntity* entity) {
	return mModel->getDefinition()->getShowContained();
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
