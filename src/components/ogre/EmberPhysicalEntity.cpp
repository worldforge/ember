/*
    Copyright (C) 2004  Erik Hjortsberg
    some parts Copyright (C) 2004 bad_camel at Ogre3d forums

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
#include "Model.h"

#include "DimeEntity.h"
#include "DimePhysicalEntity.h"

namespace DimeOgre {


DimePhysicalEntity::DimePhysicalEntity(const Atlas::Objects::Entity::GameEntity &ge, Eris::World* vw, Ogre::SceneManager* sceneManager, Ogre::SceneNode* nodeWithModel) : 
DimeEntity(ge, vw, sceneManager),
mAnimationState_Walk(NULL),
mScaleNode(nodeWithModel)
{
//	mOgreEntity = static_cast<Ogre::Entity*>(nodeWithEntity->getAttachedObject(0));
	mModel = static_cast<Model*>(nodeWithModel->getAttachedObject(0));
	mModel->setQueryFlags(DimeEntity::CM_UNDEFINED);

	assert(mOgreNode);
	assert(mScaleNode);
	scaleNode();
	mOgreNode->addChild(mScaleNode);
	loadAnimationsFromModel();

	mModel->setUserObject(this);
	
	
}

DimePhysicalEntity::~DimePhysicalEntity()
{
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


void DimePhysicalEntity::loadAnimationsFromModel()
{
	if (mModel->isAnimated()) {
		//start with the idle animation
		mModel->startAnimation("idle");
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

Model* DimePhysicalEntity::getModel() const
{
	return mModel;	
}


void DimePhysicalEntity::scaleNode() {
	if (mModel->getRotation()) {
		mScaleNode->rotate(Ogre::Vector3::UNIT_Y,(Ogre::Degree)mModel->getRotation());
	}
	if (mModel->getScale()) {
		if (mModel->getScale() != 1) {
			//only scale if it's not 1
			mScaleNode->setScale(mModel->getScale(), mModel->getScale(), mModel->getScale());
		}
	} else {
		const Ogre::AxisAlignedBox ogreBoundingBox = mModel->getWorldBoundingBox(true);
		const Ogre::Vector3 ogreMax = ogreBoundingBox.getMaximum();
		const Ogre::Vector3 ogreMin = ogreBoundingBox.getMinimum();
	
		if (hasBBox()) {

			const WFMath::AxisBox<3> wfBoundingBox = getBBox();	
			const WFMath::Point<3> wfMax = wfBoundingBox.highCorner();
			const WFMath::Point<3> wfMin = wfBoundingBox.lowCorner();
			
			Ogre::Real scaleX;		
			Ogre::Real scaleY;		
			Ogre::Real scaleZ;		

			switch (mModel->getUseScaleOf()) {
				case Model::MODEL_HEIGHT:
					scaleX = scaleY = scaleZ = fabs((wfMax.z() - wfMin.z()) / (ogreMax.y - ogreMin.y));		
					break;
				case Model::MODEL_WIDTH:
					scaleX = scaleY = scaleZ = fabs((wfMax.x() - wfMin.x()) / (ogreMax.x - ogreMin.x));		
					break;
				case Model::MODEL_DEPTH:
					scaleX = scaleY = scaleZ = fabs((wfMax.y() - wfMin.y()) / (ogreMax.z - ogreMin.z));		
					break;
					
				default:				
					scaleX = fabs((wfMax.x() - wfMin.x()) / (ogreMax.x - ogreMin.x));		
					scaleY = fabs((wfMax.z() - wfMin.z()) / (ogreMax.y - ogreMin.y));		
					scaleZ = fabs((wfMax.y() - wfMin.y()) / (ogreMax.z - ogreMin.z));		
			}
				
			
			//Ogre::Real finalScale = std::max(scaleX, scaleY);
			//finalScale = std::max(finalScale, scaleZ);
			mScaleNode->setScale(scaleX, scaleY, scaleZ);
			
		} else {
			//set to small size
			
			Ogre::Real scaleX = (0.25 / (ogreMax.x - ogreMin.x));		
			Ogre::Real scaleY = (0.25 / (ogreMax.y - ogreMin.y));		
			Ogre::Real scaleZ = (0.25 / (ogreMax.z - ogreMin.z));		
			mScaleNode->setScale(scaleX, scaleY, scaleZ);
		}		

	}	
	
}


void DimePhysicalEntity::handleMove()
{
	getSceneNode()->setPosition(Atlas2Ogre(getPosition()));
	getSceneNode()->setOrientation(Atlas2Ogre(getOrientation()));
	MotionManager* motionManager = &MotionManager::getSingleton();
	if (getVelocity() != WFMath::Vector<3>().zero()) {
		//the entity is moving
		motionManager->addEntity(this);
		if (mModel->isAnimated()) {
			mModel->stopAnimation("idle");
			mModel->startAnimation("walk");
		}
	} else {
		//the entity has stopped moving
		motionManager->removeEntity(this);
		if (mModel->isAnimated()) {
			mModel->stopAnimation("walk");
			mModel->startAnimation("idle");
		}
	}
	//Root::getSingleton().getAutoCreatedWindow()->setDebugText(std::string("Moved: " + _id) );
}

void DimePhysicalEntity::handleTalk(const std::string &msg)
{
	
    std::string message = "<";
    message.append(getName());
    message.append("> ");
    message.append(msg);
	std::cout << "TRACE - ENTITY SAYS: [" << message << "]\n" << std::endl;
	dime::ConsoleBackend::getMainConsole()->pushMessage(message);
}

void DimePhysicalEntity::setVisible(bool vis)
{
	DimeEntity* container = dynamic_cast<DimeEntity*>(getContainer());
	if (container) {
		//check with the parent first if we should show ourselves
		if (vis && container->allowVisibilityOfMember(this)) {
			mModel->setVisible(true);	
		} else {
			mModel->setVisible(false);	
		}
		
	} else {
		mModel->setVisible(vis);
	}
}

/*
void DimePhysicalEntity::setContainer(Entity *pr)
{
		
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
	
}
*/




void DimePhysicalEntity::adjustHeightPosition()
{
	DimePhysicalEntity* container = static_cast<DimePhysicalEntity*>(getContainer());
	if (container) {
		container->adjustHeightPositionForContainedNode(this);
	}
	
}


}
