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

#include "DimeOgrePrerequisites.h"


#include "framework/ConsoleBackend.h"
#include "MotionManager.h"
#include "GUIManager.h"
#include "DimeEntityFactory.h"
#include "Model.h"


#include "DimeEntity.h"
#include "DimePhysicalEntity.h"

#include "DimeOgre.h"

namespace DimeOgre {


DimePhysicalEntity::DimePhysicalEntity(const std::string& id, Eris::TypeInfo* ty, Eris::View* vw, Ogre::SceneManager* sceneManager, Ogre::SceneNode* nodeWithModel) : 
mAnimationState_Walk(NULL),
mScaleNode(nodeWithModel),
DimeEntity(id, ty, vw, sceneManager)
{
	mModel = static_cast<Model*>(getScaleNode()->getAttachedObject(0));
	loadAnimationsFromModel();
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

void DimePhysicalEntity::init(const Atlas::Objects::Entity::GameEntity &ge)
{
	DimeEntity::init(ge);
	getModel()->setQueryFlags(DimeEntity::CM_UNDEFINED);

/*	assert(mOgreNode);
	assert(mScaleNode);*/
	scaleNode();
	getSceneNode()->addChild(getScaleNode());

	getModel()->setUserObject(this);
}


void DimePhysicalEntity::loadAnimationsFromModel()
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




void DimePhysicalEntity::scaleNode() {
	if (getModel()->getRotation()) {
		getScaleNode()->rotate(Ogre::Vector3::UNIT_Y,(Ogre::Degree)getModel()->getRotation());
	}
	if (getModel()->getScale()) {
		if (getModel()->getScale() != 1) {
			//only scale if it's not 1
			getScaleNode()->setScale(getModel()->getScale(), getModel()->getScale(), getModel()->getScale());
		}
	} else {
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

			
// 			if ((wfMax.x() - wfMin.x()) > 100 || (wfMax.y() - wfMin.y()) > 100 || (wfMax.z() - wfMin.z()) > 100) 
// 			{
// 				//big!!!
// 				int i = 0;
// 			}
			
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
					
				default:				
					scaleX = fabs((wfMax.x() - wfMin.x()) / (ogreMax.x - ogreMin.x));		
					scaleY = fabs((wfMax.z() - wfMin.z()) / (ogreMax.y - ogreMin.y));		
					scaleZ = fabs((wfMax.y() - wfMin.y()) / (ogreMax.z - ogreMin.z));		
			}
				
			
			//Ogre::Real finalScale = std::max(scaleX, scaleY);
			//finalScale = std::max(finalScale, scaleZ);
			getScaleNode()->setScale(scaleX, scaleY, scaleZ);
			
		} else {
			//set to small size
			
			Ogre::Real scaleX = (0.25 / (ogreMax.x - ogreMin.x));		
			Ogre::Real scaleY = (0.25 / (ogreMax.y - ogreMin.y));		
			Ogre::Real scaleZ = (0.25 / (ogreMax.z - ogreMin.z));		
			getScaleNode()->setScale(scaleX, scaleY, scaleZ);
		}		

	}	
	
}


void DimePhysicalEntity::onMoved()
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

/*
void DimePhysicalEntity::handleTalk(const std::string &msg)
{
	
    std::string message = "<";
    message.append(getName());
    message.append("> ");
    message.append(msg);
	std::cout << "TRACE - ENTITY SAYS: [" << message << "]\n" << std::endl;
	dime::ConsoleBackend::getMainConsole()->pushMessage(message);
}
*/



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
	DimeEntity* container = static_cast<DimeEntity*>(getLocation());
	if (container) {
		container->adjustHeightPositionForContainedNode(this);
	}
	
}


}
