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

#ifndef SMARTBODYANIMATION_H
#define SMARTBODYANIMATION_H

#include <string>
#include <vector>


namespace SmartBody
{
class SBMotion;
class SBAssetManager;
}

namespace Ember
{

/**
 * @brief This class is used to represent an animation: each of them is identified by a name, and is composed of several motions.
 * 
 * An SmartBodyAnimation should not be created directly. It is better to instantiate SmartBodyStaticAnimation, SmartBodyMovingAnimation
 * and SmartBodyGestureAnimation. Moreover, the manipulation of this object should be handled by the SmartBodyAnimationManager. 
 * For each motion that compose an animation, there is a corresponding bml request that the SmartBodyAnimationManager retrieve when a
 * character is animated, and then send to SmartBody with the bml processor.
 *
 * @author Céline NOEL <celine.noel.7294@gmail.com>
 *
 */
class SmartBodyAnimation
{
public:

	/**
	 * @brief The different name an animation can take.
	 */
	enum class Name 
	{
		WALKING, RUNNING,			//MovingAnimations.
		STANDING,					//StaticAnimations.
		WAITING,					//Gestures.

		ANIMATIONS_COUNT			//Gives the number of supported animations.
	};

	/**
	 * @brief The different subsequent classes.
	 * This is used by the SmartBodyAnimationManager to identify what type of SmartBodyAnimationInstance it must create.
	 */
	enum class Type
	{
		STATIC, MOVING, GESTURE, 
		UNDEFINED					//Undefined is used when invalid animation name (ANIMATIONS_COUNT for example) is send to getType().
	};

	/**
	 * @brief Gets what type of class has to be created for the given animation.
	 */
	static Type getType(Name animationName);


	/**
	 * @brief Ctor.
	 */
	SmartBodyAnimation(SmartBodyAnimation::Name name, SmartBody::SBAssetManager& assetManager, const std::vector<std::string>& motions);

	/**
	 * @brief Dtor.
	 */
	virtual ~SmartBodyAnimation();

	/**
	 * @brief Returns the name for this animation.
	 */
	SmartBodyAnimation::Name getName() const;

	/**
	 * @brief Gets the request that is to be sent to the bml processor.
	 * @return false if the given index is invalid.
	 */
	virtual bool getBmlRequest(std::string& request, int motionIndex) const = 0;

	/**
	 * @brief Returns the duration in seconds of the given motion.
	 *
	 * @param motionIndex: must be set between 0 and mMotions.size().
	 * @return A negative value if something went wrong.
	 */
	virtual float getMotionDuration(int motionIndex) const;

	/**
	 * @brief Returns the total number of motions contained in this set.
	 */
	virtual int getMotionNumber() const;


protected:

	/**
	 * @brief The name for this animation.
	 */
	Name mName;

	/**
	 * @brief A reference on the asset manager. 
	 * It is needed to get the duration of a motion.
	 */
	SmartBody::SBAssetManager& mAssetManager;

	/**
	 * @brief A reference on the motions names.
	 */
	const std::vector<std::string>& mMotionNames;

};


/**
 * @brief In instance for a SmartBodyAnimation.
 *
 * Whereas a SmartBodyAnimation is used to describe an animation in general, this class is used into SmartBodyRepresentation to 
 * bound it to an animation. Everytime we want to animate a character with SmartBody, an object from this class has to be created by the 
 * SmartBodyAnimationManager and attributed to the character.
 * 
 * In the same way as SmartBodyAnimation, SmartBodyAnimationInstance should be instantiated through children classes (MovindAnimationInstance,
 * StaticAnimationInstance, GestureAnimationInstance). Each of them contains details about how the animation is being played on the 
 * representation they are linked to.
 *
 * @author Céline NOEL <celine.noel.7294@gmail.com>
 */
class SmartBodyAnimationInstance
{
public:

	/**
	 * @brief Ctor.
	 */
	SmartBodyAnimationInstance(const SmartBodyAnimation& animation);

	/**
	 * @brief Dtor.
	 */
	virtual ~SmartBodyAnimationInstance();

	/**
	 * @brief Returns the bml request to execute the current motion of this instance.
	 */
	virtual bool getBmlRequest(std::string& request) const = 0;

	/**
	 * @brief Returns the number of motions constituing mReference.
	 */ 
	int getMotionNumber() const;
	

protected:

	/**
	 * @brief A reference on the corresponding SmartBodyMovingAnimation.
	 */
	const SmartBodyAnimation& mReference;

};

}

#endif
