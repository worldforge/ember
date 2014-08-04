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

#ifndef SMARTBODYMOVINGANIMATION_H
#define SMARTBODYMOVINGANIMATION_H

#include "SmartBodyAnimation.h"

#include <string>


namespace Ogre
{
class Vector3;
}

namespace Ember
{

/**
 * @brief This class is used to handle all animations describing a locomotion type (Walk, Run, Jog, Swim if it had existed).
 *
 * Like any SmartBodyAnimation, into the SmartBodyAnimationManager.
 * Contrarily to the other animations, it is composed by four motion files (one for each direction).
 *
 * @author Céline NOEL <celine.noel.7294@gmail.com>
 *
 */
class SmartBodyMovingAnimation: public SmartBodyAnimation
{
public:

	/**
	 * @brief The direction toward which the character can go.
	 */
	enum class Direction
	{
		FORWARD, BACKWARD, 	//SmartBody only provides stepping back animation. We thus need to make our own going back motion.
		LEFT, RIGHT, 		//For these two ones, we use strafing motions.

		DIRECTION_COUNT, 	//The total number of directions (used to set the size of mBmlRequests).
	};

	/**
	 * @brief Ctor.
	 */
	SmartBodyMovingAnimation(SmartBodyAnimation::Name name, SmartBody::SBAssetManager& assetMng, 
		const std::string& forwardMotion, const std::string& backwardMotion, const std::string& leftMotion, const std::string& rightMotion);

	/**
	 * @brief Copy ctor.
	 */
	SmartBodyMovingAnimation(const SmartBodyMovingAnimation& animation);

	/** 
	 * @brief Dtor.
	 */
	~SmartBodyMovingAnimation();

	/**
	 * @brief Gets the bml request that is to be sent via the BMLProcessor. The default direction is forward.
	 */
	void getBmlRequest(std::string& request, Direction direction = Direction::FORWARD, const std::string& readyTime = "") const;

	/**
	 * @brief Gets the bml request that is to be sent via the BMLProcessor. The default direction is forward.
	 * @return false if direction does not correspond to a direction.
	 */
	bool getBmlRequest(std::string& request, int direction = (int)Direction::FORWARD, const std::string& readyTime = "") const;


private:

	/**
	 * @brief The names of the different motions, each corresponding to its direction.
	 * Use mMotions[(int)direction] to access the motion corresponding to the given direction.
	 */
	std::vector<std::string> mMotions;

};

/**
 * @brief This is an instance for SmartBodyMovingAnimation. 
 *
 * Every SmartBodyRepresentation contain either a SmartBodyIdlingAnimation, either a SmartBodyMovingAnimationInstance. If it has one
 * SmartBodyMovingAnimationInstance, then, during the bone position updating loop, the shifting of the base joint must be passed on the 
 * scene node connecting the representation to the world.
 *
 * @author Céline NOEL <celine.noel.7294@gmail.com>
 */
class SmartBodyMovingAnimationInstance: public SmartBodyAnimationInstance
{
public:

	/**
	 * @brief Ctor.
	 */
	SmartBodyMovingAnimationInstance(const SmartBodyMovingAnimation& animation, SmartBodyMovingAnimation::Direction direction = SmartBodyMovingAnimation::Direction::FORWARD);

	/**
	 * @brief Dtor.
	 */
	~SmartBodyMovingAnimationInstance();

	/**
	 * @brief Gets the request corresponding to mDirection.
	 */
	bool getBmlRequest(std::string& request) const;

	/**
	 * @brief Changes the direction.
	 */
	void setDirection(SmartBodyMovingAnimation::Direction direction);

	/**
	 * @brief Gets the moving direction of the character.
	 */
	SmartBodyMovingAnimation::Direction getDirection() const;

	/**
	 * @brief Returns true if the direction has changed. The SmartBodyAnimationManager must then send a new request to SmartBody.
	 */
	bool hasDirectionChanged();

	/**
	 * @brief Notify that a bml request has been sent to follow the change of direction.
	 * This set mHasDirectionChange to false.
	 */
	void notifyUpdate();

	/**
	 * @brief Returns true if the motion has just ended one loop turn.
	 *
	 * This must be used into SmartBodyRepresentation::calculateTransformations: actually, in order for the character not to be returning
	 * to his starting position everytime the motion ends, we need to identify when it happens. For the moment, there is no other way to 
	 * know it but to compare the difference of his position at the end of two consecutive frames. Though the method is not extremely 
	 * accurate, it should work in all the cases we'll have to handle here, and it is quick enough.
	 */
	bool isMotionLoopOver(const Ogre::Vector3& translation);
	

private:

	/**
	 * @brief The current direction the character is heading toward.
	 */
	SmartBodyMovingAnimation::Direction mDirection;

	/**
	 * @brief States that mDirection has changed. We then need to send the corresponding request to the bml processor.
	 */
	bool mHasDirectionChanged;
};

}

#endif

