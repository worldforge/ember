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

#ifndef SMARTBODYGESTUREANIMATION_H
#define SMARTBODYGESTUREANIMATION_H

#include "SmartBodyAnimation.h"


namespace Ember
{

/**
 * @brief This represent a set of idling animations. 
 *
 * For example, we could create a multiple sets depending on the position of the character (a standing idling set, a sitting idling set, 
 * a swiming idling set, a set for when the character is tired, etc. - of course, right now, the motions provided into SmartBody datas 
 * only able us to provide a standing idling set, possibly a agressive one too, but these are ideas for future improvements).
 *
 * @author Céline NOEL <celine.noel.7294@gmail.com>
 */
class SmartBodyGestureAnimation: public SmartBodyAnimation
{
public:

	/**
	 * @brief To represent a gesture that is not generating from a motion file.
	 */
	class Gesturing
	{
	public:

		/**
		 * @brief Name of the gesture.
		 */
		enum class Name
		{
			NOD, SHAKE, WIGGLE, WAGGLE, TOSS		//The character needs the skullbase bone to use these animations. 
		};

		/**
		 * @brief Ctor.
		 */
		Gesturing(Name name, int repeat = 1, float duration = 1.0f);

		/**
		 * @brief Dtor.
		 */
		~Gesturing();

		/**
		 * @brief Returns the request corresponding to this gesture.
		 */
		const std::string& getBmlRequest() const;

		/**
		 * @brief Returns the duration of the gesture.
		 */
		float getDuration() const;

	private:

		/**
		 * @brief The name of the gesture.
		 */
		Name mName;

		/**
		 * @brief The associated bml request.
		 */
		std::string mBmlRequest;

		/**
		 * @brief The duration of the gesture.
		 */
		float mDuration;
	};


	/**
	 * @brief Ctor.
	 * @param motions: The motions that use motion files.
	 * @param gesturing: The ones that don't.
	 * One of them must contain at least one element.
	 */
	SmartBodyGestureAnimation(SmartBodyAnimation::Name name, SmartBody::SBAssetManager& assetMng, const std::vector<std::string>& motions, 
		const std::vector<Gesturing>& gesturings);

	/**
	 * @brief Copy ctor.
	 */
	SmartBodyGestureAnimation(const SmartBodyGestureAnimation& animation);
	
	/** 
	 * @brief Dtor.
	 */
	~SmartBodyGestureAnimation();

	/**
	 * @brief Gets the bml request that is to be sent via the BMLProcessor. 
	 * @param gestureIndex: An index into motions + gesturing.
	 */
	bool getBmlRequest(std::string& request, int gestureIndex) const;

	/**
	 * @brief Returns the duration of the gesture.
	 *
	 * Overrides SmartBodyAnimation::getMotionDuration().
	 */
	float getMotionDuration(int gestureIndex) const;

	/**
	 * @brief Returns the number of motions contained into this Animation.
	 * @return mMotions.size() + mGesturings.size().
	 *
	 * Overrides SmartBodyAnimation::getMotionNumber(). 
	 */
	int getMotionNumber() const;


private:

	/**
	 * @brief The names of the different gesture motions.
	 */
	const std::vector<std::string> mMotions;

	/**
	 * @brief The different gesture animations that can be launched during idling (can be null).
	 * When waiting long enough, the character should be gesturing (scratching his head, crossing his arms, etc.). Here are all the
	 * possible gestures for this idling set.
	 * Contrarily to the postures, which continued as long as no change has been made for them, a gesture animation will be played
	 * only once.
	 * 
	 * @see SmartBodyGestureAnimation.h.
	 */
	const std::vector<Gesturing> mGesturings;

};

/**
 * @brief This bound a SmartBodyGestureAnimation to a SmartBodyRepresentation. 
 *
 * For example, we could create a multiple sets depending on the position of the character (a standing idling set, a sitting idling set, 
 * a swiming idling set, a set for when the character is tired, etc. - of course, right now, the motions provided into SmartBody datas 
 * only able us to provide a standing idling set, possibly a agressive one too, but these are ideas for future improvements).
 *
 * @author Céline NOEL <celine.noel.7294@gmail.com>
 */
class SmartBodyGestureAnimationInstance: public SmartBodyAnimationInstance
{
public:

	/**
	 * @brief Ctor.
	 * @param postureIndex: the index of the idling motion wanted into SmartBodyGestureAnimation.mPostures. If it is not valid, then, the
	 * character will be animate with the first motion contained into mPostures.
	 */
	SmartBodyGestureAnimationInstance(const SmartBodyGestureAnimation& reference);
	
	/** 
	 * @brief Dtor.
	 */
	~SmartBodyGestureAnimationInstance();

	/** 
	 * @brief Gets the bml request corresponding to the active gesture.
	 * @return false if no gesture is active.
	 */
	bool getBmlRequest(std::string& request) const;

	/**
	 * @brief Gets the time elapsed since the last time a gesture has ended, to see if lauching a new one is reasonable.
	 * @return mTimeSinceGestureEnd.
	 */
	float getTimeSinceGestureEnd();

	/**
	 * @brief Updates mTimeSinceGestureEnd.
	 */
	void updateTimer(float timeSlice);

	/**
	 * @brief Launches a gesture on the representation bound to this instance.
	 * @return false if gestureIndex is invalid.
	 */
	bool playGesture(int gestureIndex);


private:

	/**
	 * @brief The index of the gesture launched on the character.
	 * It equals -1 if no gesture is active. As long as it doesn't, no other gesture can be played (one gesturing at a time).
	 */
	int mGesture;

	/**
	 * @brief The time elapsed since the character ended his last gesture motion.
	 * To limitate additionnal calculs, this value can be negative (it is set to -motion.duration() when the animation is launched).
	 */
	float mTimeSinceGestureEnd;

};

}

#endif