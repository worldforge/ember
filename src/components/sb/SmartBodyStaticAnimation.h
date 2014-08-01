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

#ifndef SMARTBODYSTATICANIMATION_H
#define SMARTBODYSTATICANIMATION_H

#include "SmartBodyAnimation.h"


namespace Ember
{

class SmartBodyGestureAnimation;

/**
 * @brief This represent a set of static animations (in opposition to the MovingAnimation). 
 *
 * For example, we could create a multiple sets depending on the position of the character (a standing idling set, a sitting idling set, 
 * a swiming idling set, a set for when the character is tired, etc. - of course, right now, the motions provided into SmartBody datas 
 * only able us to provide a standing set, possibly a agressive one too, but these are ideas for future improvements).
 *
 * @author Céline NOEL <celine.noel.7294@gmail.com>
 */
class SmartBodyStaticAnimation: public SmartBodyAnimation
{
public:

	/**
	 * @brief Ctor.
	 */
	SmartBodyStaticAnimation(SmartBodyAnimation::Name name, SmartBody::SBAssetManager& assetMng, const std::vector<std::string>& postures, 
		SmartBodyGestureAnimation const *gestures = nullptr);
	
	/** 
	 * @brief Dtor.
	 */
	~SmartBodyStaticAnimation();

	/**
	 * @brief Gets the bml request that is to be sent via the BMLProcessor. 
	 * @return false if postureIndex is not a valid index into mPostures.
	 */
	bool getBmlRequest(std::string& request, int postureIndex) const;

	/**
	 * @brief Gets the bml request corresponding to the given gesture.that is to be sent via the BMLProcessor. 
	 * @return false if gestureIndex is not a valid index into mGestures.
	 */
	bool getGestureBmlRequest(std::string& request, int gestureIndex) const;

	/**
	 * @brief Returns a pointer on mGestures.
	 */
	SmartBodyGestureAnimation const* getGestures() const;

	/**
	 * @brief Gets the number of gesture motions.
	 */
	int getGestureMotionNumber() const;


private:

	/**
	 * @brief The names of the different idling postures (must contains at least one motion).
	 * While idling, the character can change his posture (still an idling posture), to give a more realistic aspect. The variation from
	 * one to another might be set randomly.
	 */
	const std::vector<std::string> mPostures;

	/**
	 * @brief The different gesture animations that can be launched during idling (can be null).
	 * When waiting long enough, the character should be gesturing (scratching his head, crossing his arms, etc.). Here are all the
	 * possible gestures for this idling set.
	 * Contrarily to the postures, which continued as long as no change has been made for them, a gesture animation will be played
	 * only once.
	 * 
	 * @see SmartBodyGestureAnimation.h.
	 */
	SmartBodyGestureAnimation const* mGestures;

};

/**
 * @brief This bound a SmartBodyStaticAnimation to a SmartBodyRepresentation. 
 *
 * For example, we could create a multiple sets depending on the position of the character (a standing idling set, a sitting idling set, 
 * a swiming idling set, a set for when the character is tired, etc. - of course, right now, the motions provided into SmartBody datas 
 * only able us to provide a standing idling set, possibly a agressive one too, but these are ideas for future improvements).
 *
 * @author Céline NOEL <celine.noel.7294@gmail.com>
 */
class SmartBodyStaticAnimationInstance: public SmartBodyAnimationInstance
{
public:

	/**
	 * @brief Ctor.
	 * @param postureIndex: the index of the idling motion wanted into SmartBodyStaticAnimation.mPostures. If it is not valid, then, the
	 * character will be animate with the first motion contained into mPostures.
	 */
	SmartBodyStaticAnimationInstance(const SmartBodyStaticAnimation& reference, int postureIndex = 0);
	
	/** 
	 * @brief Dtor.
	 */
	~SmartBodyStaticAnimationInstance();

	/**
	 * @brief Returns the bml request corresponding to mPosture.
	 */
	bool getBmlRequest(std::string& request) const;

	/**
	 * @brief Gets the time since the last posture change, in order to estimate if another change would seem natural.
	 * @return mTimeSincePostureChange.
	 */
	float getTimeSincePostureChange();

	/**
	 * @brief Changes the posture of the character.
	 * @return false if postureIndex is not valid.
	 */
	bool changePosture(int postureIndex);

	/**
	 * @brief Returns the bml request corresponding to mGesture.
	 */
	bool getGestureBmlRequest(std::string& request) const;

	/**
	 * @brief Gets the time elapsed since the last time a gesture has ended, to see if lauching a new one is reasonable.
	 * @return mTimeSinceGestureEnd.
	 */
	float getTimeSinceGestureEnd();

	/**
	 * @brief Launches a gesture on the representation bound to this instance.
	 * @return false if gestureIndex is invalid.
	 */
	bool playGesture(int gestureIndex);

	/**
	 * @brief Gets the number of gesture motions.
	 */
	int getGestureMotionNumber() const;

	/**
	 * @brief Updates mTimeSincePostureChange and mTimeSinceGestureEnd.
	 */
	void updateTimers(float timeSlice);


private:

	/** 
	 * @brief The index of the current idling posture of the character.
	 * This value is necessarily set between 0 and mReference.mPostures.size(). 
	 */
	int mPosture;

	/**
	 * @brief The index of the gesture launched on the character.
	 * It equals -1 if no gesture is active. As long as it doesn't, no other gesture can be played (one gesturing at a time).
	 */
	int mGesture;

	/**
	 * @brief The time elapsed since the last time the character changes his posture.
	 */
	float mTimeSincePostureChange;

	/**
	 * @brief The time elapsed since the character ended his last gesture motion.
	 * To limitate additionnal calculs, this value can be negative (it is set to -motion.duration() when the animation is launched).
	 */
	float mTimeSinceGestureEnd;

};

}

#endif