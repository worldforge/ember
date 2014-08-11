/*
 Copyright (C) 2014 Céline NOËL <celine.noel.7294@gmail.com>

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

#include "SmartBodyAnimationManager.h"
#include "SmartBodyStaticAnimation.h"
#include "SmartBodyMovingAnimation.h"
#include "SmartBodyGestureAnimation.h"
#include "SmartBodyIntuitiveAnimation.h"
#include "SmartBodyRepresentation.h"

#include <string>

#define MIN_TIME_BEFORE_STATIC_POSE_CHANGE 10.0f 	//Expressed in seconds.
#define MIN_TIME_BEFORE_NEW_GESTURE_PLAYED 2.0f 	//Expressed in seconds.
#define RANDOM_STATIC_POSE_CHANGE 900 				//Every frame, we have 1/900 chance that the posture changes.
#define RANDOM_STATIC_GESTURE_PLAY 700 				//Every frame, we have 1/700 chance that a gesture is triggered.


namespace Ember
{

SmartBodyAnimationManager::SmartBodyAnimationManager(SmartBody::SBBmlProcessor& bmlProcessor)
:	mBmlProcessor(bmlProcessor)
{
}

SmartBodyAnimationManager::~SmartBodyAnimationManager()
{
	for (auto& animation : mAnimations)
	{
		delete animation.second;
	}
}

void SmartBodyAnimationManager::initialize(SmartBody::SBAssetManager& assetManager)
{	
	std::vector<std::string> motions;


	/*			*** Gestures. ***			*/
	std::vector<SmartBodyGestureAnimation::Gesturing> gesturings;

	//WAITING.
	motions.push_back("ChrBrad@Idle01_ScratchChest01");
	motions.push_back("ChrBrad@Idle01_ScratchHeadLf01");
	motions.push_back("ChrBrad@Idle01_ScratchTempleLf01");

	gesturings.push_back(SmartBodyGestureAnimation::Gesturing(SmartBodyGestureAnimation::Gesturing::Name::TOSS, 1, 2.0f));

	mAnimations[SmartBodyAnimation::Name::WAITING] = new SmartBodyGestureAnimation(SmartBodyAnimation::Name::WAITING, assetManager, motions, gesturings);

	motions.clear();
	gesturings.clear();


	/*			*** Statics. ***			*/

	//STANDING.
	motions.push_back("ChrUtah_Idle001");
	motions.push_back("ChrBrad@Idle01");

	mAnimations[SmartBodyAnimation::Name::STANDING] = new SmartBodyStaticAnimation(SmartBodyAnimation::Name::STANDING, assetManager, motions, dynamic_cast<SmartBodyGestureAnimation*>(mAnimations[SmartBodyAnimation::Name::WAITING]));

	motions.clear();


	/*			*** Movings. ***			*/

	//WALKING.
	mAnimations[SmartBodyAnimation::Name::WALKING] = new SmartBodyMovingAnimation(SmartBodyAnimation::Name::WALKING, assetManager, "ChrUtah_Walk001", "ChrUtah_Walk001", "ChrUtah_StrafeSlowLf01", "ChrUtah_StrafeSlowRt01");

	//RUNNING.
	mAnimations[SmartBodyAnimation::Name::RUNNING] = new SmartBodyMovingAnimation(SmartBodyAnimation::Name::RUNNING, assetManager, "ChrUtah_Run001", "ChrUtah_Run001", "ChrUtah_StrafeFastLf01", "ChrUtah_StrafeFastRt01");


	/*			*** Intuitives. ***			*/

	//LOCOMOTION.
	mAnimations[SmartBodyAnimation::Name::LOCOMOTION] = new SmartBodyIntuitiveAnimation(SmartBodyAnimation::Name::LOCOMOTION, assetManager, "allLocomotion", "allStartingLeft", "allStartingRight", "allIdleTurn", "allStep");
}

void SmartBodyAnimationManager::addAnimation(SmartBodyAnimation::Name name, SmartBodyRepresentation& character)
{
	//We must find what kind of animation we are looking at.
	switch (SmartBodyAnimation::getType(name))
	{
		case SmartBodyAnimation::Type::STATIC:
			addStaticAnimation(name, character);
			break;

		case SmartBodyAnimation::Type::MOVING:
			addMovingAnimation(name, character);
			break;

		case SmartBodyAnimation::Type::INTUITIVE:
			addIntuitiveAnimation(name, character);
			break;

		case SmartBodyAnimation::Type::GESTURE:
			addGestureAnimation(name, character);
			break;

		default:
			break;
	}
}

void SmartBodyAnimationManager::addStaticAnimation(SmartBodyAnimation::Name name, SmartBodyRepresentation& character)
{
	freePosture(character);

	SmartBodyStaticAnimationInstance *animation = new SmartBodyStaticAnimationInstance(dynamic_cast<SmartBodyStaticAnimation&>(*mAnimations[name]), mBmlProcessor, character.getName());
	animation->specifyStartTime(true, 0.0f);
	animation->specifyReadyTime(true, 0.2f);

	character.setPosture(animation, true);

	animation->execute(character.getName());
}

void SmartBodyAnimationManager::addMovingAnimation(SmartBodyAnimation::Name name, SmartBodyRepresentation& character)
{
	freePosture(character);

	SmartBodyMovingAnimationInstance *animation = new SmartBodyMovingAnimationInstance(dynamic_cast<SmartBodyMovingAnimation&>(*mAnimations[name]), mBmlProcessor, character.getName());
	animation->specifyStartTime(true, 0.0f);
	animation->specifyReadyTime(true, 0.2f);

	character.setPosture(animation, false);

	animation->execute(character.getName());
}

void SmartBodyAnimationManager::addIntuitiveAnimation(SmartBodyAnimation::Name name, SmartBodyRepresentation& character)
{
	SmartBodyAnimationInstance *animation = character.getPosture();

	if (!animation || animation->getReference().getName() != name)
	{
		freePosture(character);

		SmartBodyIntuitiveAnimationInstance *animation = new SmartBodyIntuitiveAnimationInstance(dynamic_cast<SmartBodyIntuitiveAnimation&>(*mAnimations[name]), mBmlProcessor, character.getName(), SmartBodyIntuitiveAnimation::Blend::STARTING_RIGHT);

		character.setIntuitivePosture(animation);
	}
}

void SmartBodyAnimationManager::addGestureAnimation(SmartBodyAnimation::Name name, SmartBodyRepresentation& character)
{
	//TODO.
	//SmartBodyGestureAnimationInstance *animation = new SmartBodyGestureAnimationInstance(dynamic_cast<SmartBodyGestureAnimation&>(*mAnimations[name]));
	//character.addGesture(animation);
}

void SmartBodyAnimationManager::updateAnimations(SmartBodyRepresentation& character, float timeSlice)
{
	switch (SmartBodyAnimation::getType(character.getPosture()->getReference().getName()))
	{
		case SmartBodyAnimation::Type::MOVING:
			updateMovingAnimation(character);
			break;

		case SmartBodyAnimation::Type::STATIC:
			updateStaticAnimation(character, timeSlice);
			break;

		case SmartBodyAnimation::Type::INTUITIVE:
			updateIntuitiveAnimation(character, timeSlice);
			break;

		default:
			break;
	}

	//Also update the gestures.
	updateGestureAnimations(character, timeSlice);
}

void SmartBodyAnimationManager::updateMovingAnimation(SmartBodyRepresentation& character) const
{
	SmartBodyMovingAnimationInstance& animation = dynamic_cast<SmartBodyMovingAnimationInstance&>(*character.getPosture());

	if (animation.hasDirectionChanged())
	{
		//Send the update to SmartBody.
		animation.execute(character.getName());
	}
}

void SmartBodyAnimationManager::updateIntuitiveAnimation(SmartBodyRepresentation& character, float timeSlice) const
{
	SmartBodyIntuitiveAnimationInstance& animation = dynamic_cast<SmartBodyIntuitiveAnimationInstance&>(*character.getPosture());

	animation.setBlend(SmartBodyIntuitiveAnimation::Blend::LOCOMOTION);
	animation.setServerPositionAndOrientation(character.getWorldPosition(), character.getWorldOrientation());
	animation.calculateBlendParameters(timeSlice);
	
	animation.execute(character.getName());
}

void SmartBodyAnimationManager::updateStaticAnimation(SmartBodyRepresentation& character, float timeSlice)
{
	SmartBodyStaticAnimationInstance& animation = dynamic_cast<SmartBodyStaticAnimationInstance&>(*character.getPosture());
	animation.updateTimers(timeSlice);

	if (animation.getTimeSincePostureChange() > MIN_TIME_BEFORE_STATIC_POSE_CHANGE)
	{
	    std::uniform_int_distribution<int> range(0, RANDOM_STATIC_POSE_CHANGE - 1);
	 	
	 	if (range(mRandGen) == 0)
	 	{			
	 		//Change the posture.
	 		std::uniform_int_distribution<int> postureRange(0, animation.getMotionNumber() - 1);
	 		animation.changePosture(postureRange(mRandGen));

	 		//Send the update to SmartBody.
			animation.execute(character.getName());
	 	}
	}

	if (animation.getTimeSinceGestureEnd() > MIN_TIME_BEFORE_NEW_GESTURE_PLAYED)
	{
		std::uniform_int_distribution<int> range(0, RANDOM_STATIC_GESTURE_PLAY - 1);

		if (range(mRandGen) == 0)
		{
			//Play a new gesture.
			std::uniform_int_distribution<int> gestRange(0, animation.getGestureMotionNumber() - 1);
			animation.playGesture(gestRange(mRandGen));

			//Send the update to SmartBody.
			animation.executeGesture(character.getName());
		}
	}
}

void SmartBodyAnimationManager::updateGestureAnimations(SmartBodyRepresentation& character, float timeSlice) const
{
	//TODO.
//	std::vector<SmartBodyGestureAnimationInstance*> gestures = character.getGestures();
//	for (auto& gesture : gestures)
//	{
//		gesture.updateTimer(timeSlice);
//	
//		if (animation.hasGestureBeenTriggered())
//		{
//			//Send the update to SmartBody.
//	 		mBmlProcessor.execBML(character.getName(), animation.getBmlRequest());
//
//		 	//Notify the animation instance that the request has been sent.
//		 	animation.notifyUpdate();
//		}
//	}
}

void SmartBodyAnimationManager::removeAllAnimations(SmartBodyRepresentation& character) const
{
	freePosture(character);

	//std::vector<SmartBodyGestureAnimationInstance*> gestures = character.getGestures();
	//for (auto& gesture : gestures)
	//{
	//	freeAnimationInstance(gesture);
	//}

	character.leaveAnimatedState();
}

void SmartBodyAnimationManager::freePosture(SmartBodyRepresentation& character) const
{
	//We get the previous animation instance to free it.
	SmartBodyAnimationInstance *previousPosture = character.getPosture();
	freeAnimationInstance(previousPosture);
}

void SmartBodyAnimationManager::freeAnimationInstance(SmartBodyAnimationInstance* animation) const
{
	//TODO: suppression of the bml request sent to the BML processor.

	delete animation;
}

}
