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

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "EmberEntity.h"
#include "EmberPhysicalEntity.h"

#include "EmberOgre.h"
#include "MathConverter.h"
#include "MotionManager.h"
#include <OgreAnimationState.h>
#include <OgreControllerManager.h>
// #include "terrain/TerrainGenerator.h"

template<> EmberOgre::MotionManager* Ember::Singleton<EmberOgre::MotionManager>::ms_Singleton = 0;
namespace EmberOgre {


MotionManager::MotionManager()
: mControllerManager(&Ogre::ControllerManager::getSingleton())
{
	mInfo.MovingEntities = mMotionSet.size();
	mInfo.AnimatedEntities = mAnimatedEntities.size();
	mInfo.Animations = mAnimations.size();
}


MotionManager::~MotionManager()
{}


void MotionManager::doMotionUpdate(Ogre::Real timeSlice)
{
	std::set<EmberEntity*>::iterator I;
	for (I = mMotionSet.begin(); I != mMotionSet.end(); ++I) {
		updateMotionForEntity(*I, timeSlice);
	}
}

void MotionManager::doAnimationUpdate(Ogre::Real timeSlice)
{

	EntityStore::iterator I = mAnimatedEntities.begin();
	for (;I != mAnimatedEntities.end(); ++I) {
		I->second->updateAnimation(timeSlice);
	}
/*	AnimationStateSet::iterator I = mAnimations.begin();
	AnimationStateSet::iterator I_end = mAnimations.end();
	for (;I != I_end; ++I) {
		if ((*I)->getEnabled()) {
			(*I)->addTime(timeSlice);
		}
	}*/
}



void MotionManager::updateMotionForEntity(EmberEntity* entity, Ogre::Real timeSlice)
{
	entity->updateMotion(timeSlice);
}

// void MotionManager::adjustHeightPositionForNode(Ogre::SceneNode* sceneNode) {
// 	Ogre::Vector3 position = sceneNode->getPosition();
// 	//get the height from Mercator through the TerrainGenerator
// 	assert(mTerrainGenerator);
// 	TerrainPosition pos = Ogre2Atlas_TerrainPosition(position);
// 	float height = mTerrainGenerator->getHeight(pos);
// 	sceneNode->setPosition(position.x, height,position.z);
//
// }

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

void MotionManager::addEntity(EmberEntity* entity)
{
	mMotionSet.insert(entity);
	mInfo.MovingEntities = mMotionSet.size();
	entity->updateMotion(0);
}

void MotionManager::removeEntity(EmberEntity* entity)
{
	mMotionSet.erase(entity);
	mInfo.MovingEntities = mMotionSet.size();
//	entity->updateMotion(0);
}



void MotionManager::addAnimatedEntity(EmberPhysicalEntity* entity)
{
	mAnimatedEntities[entity->getId()] = entity;
	mInfo.AnimatedEntities = mAnimatedEntities.size();
}

void MotionManager::removeAnimatedEntity(EmberPhysicalEntity* entity)
{
	mAnimatedEntities.erase(entity->getId());
	mInfo.AnimatedEntities = mAnimatedEntities.size();
}


void MotionManager::addAnimation(Ogre::AnimationState* animationState)
{

	animationState->setEnabled(true);
	mAnimations.insert(animationState);
	mInfo.Animations = mAnimations.size();

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
	mInfo.Animations = mAnimations.size();

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


// void MotionManager::setTerrainGenerator(TerrainGenerator* generator) {
// 	mTerrainGenerator = generator;
// }


}
