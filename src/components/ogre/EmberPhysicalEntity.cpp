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

#include "DimeEntity.h"
#include "DimePhysicalEntity.h"

namespace DimeOgre {


DimePhysicalEntity::DimePhysicalEntity(const Atlas::Objects::Entity::GameEntity &ge, Eris::World* vw, Ogre::SceneManager* sceneManager, Ogre::SceneNode* nodeWithEntity, Ogre::Vector3 scaler) : 
DimeEntity(ge, vw, sceneManager),
mAnimationState_Walk(NULL),
mScaleNode(nodeWithEntity)
{
	mOgreEntity = static_cast<Ogre::Entity*>(nodeWithEntity->getAttachedObject(0));
	
	assert(mOgreNode);
	assert(mScaleNode);
	scaleNode(scaler);
	mOgreNode->addChild(mScaleNode);
	
	if (mAnimationState_Walk) {
		MotionManager::getSingleton().addAnimation(mAnimationState_Walk);
	}
	mOgreEntity->setUserObject(this);
	
	
}

DimePhysicalEntity::~DimePhysicalEntity()
{
	if (mAnimationState_Walk) {
		MotionManager::getSingleton().removeAnimation(mAnimationState_Walk);
	}
/*	mSceneManager->removeEntity(mOgreEntity);
	mSceneManager->removeEntity(mOgreEntity);
	
	delete mOgreEntity;
	delete mSceneNode;
	*/
}



Ogre::Entity* DimePhysicalEntity::getOgreEntity() {
	return mOgreEntity;	
}

void DimePhysicalEntity::scaleNode(Ogre::Vector3 scaler) {
	const Ogre::AxisAlignedBox ogreBoundingBox = mOgreEntity->getBoundingBox();
	const Ogre::Vector3 ogreMax = ogreBoundingBox.getMaximum();
	const Ogre::Vector3 ogreMin = ogreBoundingBox.getMinimum();

	if (hasBBox()) {
		const WFMath::AxisBox<3> wfBoundingBox = getBBox();	
		const WFMath::Point<3> wfMax = wfBoundingBox.highCorner();
		const WFMath::Point<3> wfMin = wfBoundingBox.lowCorner();
		
		Ogre::Real scaleX = ((wfMax.x() - wfMin.x()) / (ogreMax.x - ogreMin.x)) * scaler.x;		
		Ogre::Real scaleY = ((wfMax.z() - wfMin.z()) / (ogreMax.y - ogreMin.y)) * scaler.y;		
		Ogre::Real scaleZ = ((wfMax.y() - wfMin.y()) / (ogreMax.z - ogreMin.z)) * scaler.z;		
		
		Ogre::Real finalScale = std::max(scaleX, scaleY);
		finalScale = std::max(finalScale, scaleZ);
		mScaleNode->setScale(finalScale, finalScale, finalScale);
		
	} else {
		//set to small size
		
		Ogre::Real scaleX = (0.25 / (ogreMax.x - ogreMin.x)) * scaler.x;		
		Ogre::Real scaleY = (0.25 / (ogreMax.y - ogreMin.y)) * scaler.y;		
		Ogre::Real scaleZ = (0.25 / (ogreMax.z - ogreMin.z)) * scaler.z;		
		mScaleNode->setScale(scaleX, scaleY, scaleZ);
	}		
	
	
}


void DimePhysicalEntity::handleMove()
{
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
	mOgreEntity->setVisible(vis);	
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
	DimePhysicalEntity* container = (DimePhysicalEntity*)getContainer();
	if (container) {
		container->adjustHeightPositionForContainedNode(this);
	}
	
}


}
