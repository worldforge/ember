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

#include "DimeEntity.h"

#include "MathConverter.h"
#include "MotionManager.h"
#include "TerrainGenerator.h"

MotionManager::MotionManager() 
: mControllerManager(&Ogre::ControllerManager::getSingleton())
{}
MotionManager::~MotionManager()
{}

MotionManager* MotionManager::_instance = 0;

MotionManager & MotionManager::getSingleton(void)
{
	if(_instance == 0)
		_instance = new MotionManager;
	return *_instance;
}

void MotionManager::doMotionUpdate(Ogre::Real timeSlice)
{
	std::set<DimeEntity*>::iterator I;
	for (I = mMotionSet.begin();I != mMotionSet.end(); ++I) {
		updateMotionForEntity(*I, timeSlice);
	}
}

void MotionManager::updateMotionForEntity(DimeEntity* entity, Ogre::Real timeSlice)
{
	//update the position of the entity
	Ogre::Vector3 velocity = Atlas2Ogre(entity->getVelocity());
	Ogre::SceneNode* sceneNode = entity->getSceneNode();
	//sceneNode->setOrientation(Atlas2Ogre(entity->getOrientation()));
//	sceneNode->translate(sceneNode->getOrientation() * (WF2OGRE(velocity) * timeSlice));
	sceneNode->translate(WF2OGRE(velocity) * timeSlice);
	adjustHeightPositionForNode(sceneNode);
}

void MotionManager::adjustHeightPositionForNode(Ogre::SceneNode* sceneNode) {
	Ogre::Vector3 position = sceneNode->getPosition();
	//get the height from Mercator through the TerrainGenerator
	float height = TerrainGenerator::getSingleton().getHeight(OGRE2WF(position.x), OGRE2WF(position.z));
	sceneNode->setPosition(position.x, WF2OGRE(height),position.z);

}

bool MotionManager::frameStarted(const Ogre::FrameEvent& event)
{
	doMotionUpdate(event.timeSinceLastFrame);
	return true;
}

bool MotionManager::frameEnded(const Ogre::FrameEvent& event)
{
}

void MotionManager::addAnimation(AnimationState* animationState)
{
	//check if it's not already added
	animationStateMap::const_iterator I = mAnimations.find(animationState);
	if (I == mAnimations.end()) {
		//how is the overhead on creating a ControllerFunction for each single AnimationState?
		//perhaps we should keep ControllerFunction bound to Animation instead?
		Ogre::AnimationControllerFunction* controllerFunction = new Ogre::AnimationControllerFunction(animationState->getLength());
		animationControllerType* animationController = Ogre::ControllerManager::getSingleton().createController(mControllerManager->getFrameTimeSource(), animationState, controllerFunction);
		
		
		mAnimations.insert(animationStateMap::value_type(animationState, animationController));
	}
	
}

void MotionManager::removeAnimation(AnimationState* animationState)
{
	animationStateMap::const_iterator I = mAnimations.find(animationState);
	if (I != mAnimations.end()) {
		//animationControllerType* animationController = mAnimations[animationState];
	/* don't need to do this as SharePtr uses reference counting
		Ogre::SharedPtr< ControllerFunction<Ogre::Real> > controllerFunctionPtr = (animationController->getFunction());
		delete *controllerFunctionPtr;
	*/	
		mAnimations.erase(I->first);

//		Ogre::ControllerManager::getSingleton().destroyController(I->second);

	}
	
}

void MotionManager::pauseAnimation(AnimationState* animationState)
{
	animationControllerType* animationController = mAnimations[animationState];
	animationController->setEnabled(false);
}

void MotionManager::unpauseAnimation(AnimationState* animationState)
{
	animationControllerType* animationController = mAnimations[animationState];
	animationController->setEnabled(true);
}
