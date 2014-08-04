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

#ifndef SMARTBODYANIMATIONMANAGER_H
#define SMARTBODYANIMATIONMANAGER_H

	#include "SmartBodyAnimation.h"

	#include <random>
 	#include <vector>
 	#include <map>


namespace SmartBody
{
class SBBmlProcessor;
class SBAssetManager;
}

namespace Ember
{

class SmartBodyRepresentation;
class SmartBodyStaticAnimation;
class SmartBodyMovingAnimation;
class SmartBodyGestureAnimation;

class SmartBodyAnimationManager
{
public:

	/**
	 * @brief Ctor.
	 */
	SmartBodyAnimationManager(SmartBody::SBBmlProcessor& bmlProcessor);

	/** 
	 * @brief Dtor.
	 */
	~SmartBodyAnimationManager();


	/**
	 * @brief Initializes all the SmartBodyAnimations.
	 * It is better to initialize the behavior sets before the animations.
	 */
	void initialize(SmartBody::SBAssetManager& assetManager);

	/**
	 * @brief Launches a new animation on a character.
	 */
	void addAnimation(SmartBodyAnimation::Name name, SmartBodyRepresentation& character);

	/** 
	 * @brief Removes all animations launched on a character in order to place him into Ogre skeletal animation mode, or to delete him.
	 */
	void removeAllAnimations(SmartBodyRepresentation& character) const;

	/**
	 * If necessary, resend a request to SmartBody to match the changes operating into each kind of AnimationInstance the 
	 * character has.
	 */
	void updateAnimations(SmartBodyRepresentation& character, float timeSlice);


private:

	/**
	 * @brief The bml processor can send requests that lauches animation on characters.
	 */
	SmartBody::SBBmlProcessor& mBmlProcessor;

	/**
	 * @brief Maps a Animation::Name to the adress of the corresponding animation.
	 */
	std::map<SmartBodyAnimation::Name, SmartBodyAnimation*> mAnimations;

	/**
	 * @brief Called in addAnimation() depending on the type of animation.
	 */
	void addStaticAnimation(SmartBodyAnimation::Name name, SmartBodyRepresentation& character);
	void addMovingAnimation(SmartBodyAnimation::Name name, SmartBodyRepresentation& character);
	void addGestureAnimation(SmartBodyAnimation::Name name, SmartBodyRepresentation& character);

	/**
	 * @brief Called in updateAnimations() depending on the type of animation.
	 */
	void updateMovingAnimation(SmartBodyRepresentation& character) const;
	void updateStaticAnimation(SmartBodyRepresentation& character, float timeSlice);
	void updateGestureAnimations(SmartBodyRepresentation& character, float timeSlice) const;

	/**
	 * @brief Removes the character's posture animation and free the memory.
	 */
	void freePosture(SmartBodyRepresentation& character) const;

	/**
	 * @brief Deletes the animation instance and (TODO) clear the bml instruction sent to the bml processor.
	 */
	void freeAnimationInstance(SmartBodyAnimationInstance* animation) const;

	/**
	 * @brief A random number generating engine, used to change or not the position or gestures of a character.
	 */
	std::random_device mRandGen;

};

}

#endif