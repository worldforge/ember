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

#include "EmberEntity.h"
#include "EmberPhysicalEntity.h"

#include "EmberOgre.h"
#include "environment/GroundCover.h"
#include "MathConverter.h"
#include "MotionManager.h"
#include "TerrainGenerator.h"

namespace EmberOgre {


MotionManager::MotionManager() 
: mControllerManager(&Ogre::ControllerManager::getSingleton())
{}
MotionManager::~MotionManager()
{}

/*
MotionManager* MotionManager::_instance = 0;

MotionManager & MotionManager::getSingleton(void)
{
	if(_instance == 0)
		_instance = new MotionManager;
	return *_instance;
}
*/
template<> MotionManager* Ember::Singleton<MotionManager>::ms_Singleton = 0;


void MotionManager::doMotionUpdate(Ogre::Real timeSlice)
{
	std::set<EmberPhysicalEntity*>::iterator I;
	for (I = mMotionSet.begin();I != mMotionSet.end(); ++I) {
		updateMotionForEntity(*I, timeSlice);
	}
}

void MotionManager::doAnimationUpdate(Ogre::Real timeSlice)
{
	AnimationStateSet::iterator I = mAnimations.begin();
	AnimationStateSet::iterator I_end = mAnimations.end();
	for (;I != I_end; ++I) {
		if ((*I)->getEnabled()) {
			(*I)->addTime(timeSlice);
		}
	}
}

void MotionManager::updateMotionForEntity(EmberPhysicalEntity* entity, Ogre::Real timeSlice)
{
	//update the position of the entity
	Ogre::Vector3 velocity = Atlas2Ogre(entity->getVelocity());
	Ogre::SceneNode* sceneNode = entity->getSceneNode();
	//sceneNode->setOrientation(Atlas2Ogre(entity->getOrientation()));
//	sceneNode->translate(sceneNode->getOrientation() * (WF2OGRE(velocity) * timeSlice));
	sceneNode->translate(WF2OGRE(velocity) * timeSlice);
	entity->adjustHeightPosition();
	//adjustHeightPositionForNode(sceneNode);
}

void MotionManager::adjustHeightPositionForNode(Ogre::SceneNode* sceneNode) {
	Ogre::Vector3 position = sceneNode->getPosition();
	//get the height from Mercator through the TerrainGenerator
	assert(mTerrainGenerator);
	float height = mTerrainGenerator->getHeight(OGRE2WF(position.x), OGRE2WF(position.z));
	sceneNode->setPosition(position.x, WF2OGRE(height),position.z);

}

bool MotionManager::frameStarted(const Ogre::FrameEvent& event)
{
	doMotionUpdate(event.timeSinceLastFrame);
	doAnimationUpdate(event.timeSinceLastFrame);
	return true;
}

bool MotionManager::frameEnded(const Ogre::FrameEvent& event)
{
	return true;
}

void MotionManager::addAnimation(Ogre::AnimationState* animationState)
{

	animationState->setEnabled(true);
	mAnimations.insert(animationState);

/*
	//check if it's not already added
	AnimationStateSet::const_iterator I = mAnimations.find(animationState);
	if (I == mAnimations.end()) {
		animationState->setEnabled(true);
		mAnimations.insert(animationState);
		

		//how is the overhead on creating a ControllerFunction for each single AnimationState?
		//perhaps we should keep ControllerFunction bound to Animation instead?
		Ogre::AnimationControllerFunction* controllerFunction = new Ogre::AnimationControllerFunction(animationState->getLength());
		animationControllerType* animationController = Ogre::ControllerManager::getSingleton().createController(mControllerManager->getFrameTimeSource(), animationState, controllerFunction);
		
		animationState->setEnabled(true);
		
		mAnimations.insert(animationStateMap::value_type(animationState, animationController));

	}
	*/
	
}

void MotionManager::removeAnimation(Ogre::AnimationState* animationState)
{
	AnimationStateSet::const_iterator I = mAnimations.find(animationState);
	if (I != mAnimations.end()) {
		mAnimations.erase(*I);
		//animationControllerType* animationController = mAnimations[animationState];
	/* don't need to do this as SharePtr uses reference counting
		Ogre::SharedPtr< ControllerFunction<Ogre::Real> > controllerFunctionPtr = (animationController->getFunction());
		delete *controllerFunctionPtr;
	*/	

/*		mAnimations.erase(I->first);

		animationControllerType* controllerFunction = I->second;
		if (controllerFunction)
		{
		}
		Ogre::ControllerManager::getSingleton().destroyController(controllerFunction);
		std::cout << "removed controller\n";
		*/
//		Ogre::ControllerManager::getSingleton().destroyController(I->second);

	}
	
}

void MotionManager::pauseAnimation(Ogre::AnimationState* animationState)
{
	animationState->setEnabled(false);
/*
	animationControllerType* animationController = mAnimations[animationState];
	animationController->setEnabled(false);
	*/
}

void MotionManager::unpauseAnimation(Ogre::AnimationState* animationState)
{
	animationState->setEnabled(true);
/*
	animationControllerType* animationController = mAnimations[animationState];
	animationController->setEnabled(true);
	*/
}


void MotionManager::setTerrainGenerator(TerrainGenerator* generator) {
	mTerrainGenerator = generator;
}


}
