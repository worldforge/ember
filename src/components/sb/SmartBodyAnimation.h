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

namespace Ember
{

/**
 * @brief This class is used as a mapping between the name of an animation and the corresponding bml request.
 *			(not finished).
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
		IDLE, WALK, RUN,			//Body postures.
		NOD, 						//Head animations.

		ANIMATIONS_COUNT			//Gives the number of supported animations.
	};

	/**
	 * @brief Ctor.
	 */
	SmartBodyAnimation(SmartBodyAnimation::Name name);

	/**
	 * @brief Dtor.
	 */
	~SmartBodyAnimation();


	/**
	 * @brief Accessor for the request that is to be sent to the BMLProcessor to execute an animation on a character.
	 * @return mBmlRequest.
	 */
	const std::string& getBmlRequest() const;

	/**
	 * @brief If this animation uses a motion file (posture, gestures), it gets the name of the motion.
	 * @return mMotionName.
	 */
	const std::string& getMotionName() const;


private:

	/**
	 * @brief The corresponding request to send to the BMLProcessor to execute an animation on a character.
	 */
	std::string mBmlRequest;

	/**
	 * @brief The name of the corresponding SBMotion (if it exists).
	 */
	std::string mMotionName;
	
};

}

#endif
