//
// C++ Interface: AnimationSet
//
// Description: 
//
//
// Author: Erik Hjortsberg <erik.hjortsberg@gmail.com>, (C) 2005
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
// 
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.//
//
#ifndef EMBEROGREANIMATIONSET_H
#define EMBEROGREANIMATIONSET_H
#include "components/ogre/EmberOgrePrerequisites.h"

// #include "ModelDefinition.h"

namespace EmberOgre {
namespace Model {

struct AnimationPart
{
	Ogre::AnimationState* state;
	Ogre::Real weight;
};

class Animation;

typedef std::vector<AnimationPart> AnimationPartSet;
typedef std::vector<Animation> AnimationStore;

/**
@author Erik Hjortsberg
*/
class Animation
{
public:
	Animation(int iterations);
	void addAnimationPart(AnimationPart part);
	void setEnabled(bool state);
	
	void addTime(Ogre::Real timeSlice);
	void setTime(Ogre::Real time);
	
	int getIterations() const;
	Ogre::Real getLengthOfOneIteration() const;
protected:
	AnimationPartSet mAnimationParts;
	Ogre::Real mIterationLength;
	int mIterations;
};

class AnimationSet
{
public:
	AnimationSet();
	~AnimationSet();
	
	/**
	 * Adds time to the animation, thus advancing it. 
	 * @param timeSlice The time to add in seconds. 
	 * @param continueAnimation Upon completion, this is either true if the animation hasn't been completed yet, or false if has completed. 
	 */
	void addTime(Ogre::Real timeSlice, bool& continueAnimation);
	
	/**
	 * Adds time to the animation, thus advancing it. 
	 * @param timeSlice The time to add in seconds. 
	 */
	void addTime(Ogre::Real timeSlice);
	
	/**
	 * Adds a single animation to this set. 
	 * @param animation 
	 */
	void addAnimation(Animation animation);
	
	
	/**
	 *    Resets the animations to it's initial status, also disabling it.
	 */
	void reset();
	
	/**
	 *    returns true if all animation parts have been played 
	 * @return 
	 */
// 	bool hasCompleted() const;

	void setSpeed(Ogre::Real speed);
	Ogre::Real getSpeed() const;

protected:
	Ogre::Real mAccumulatedTime;
	size_t mCurrentAnimationSetIndex;
	AnimationStore mAnimations;
	Ogre::Real mSpeed;
};

inline void AnimationSet::setSpeed(Ogre::Real speed) { mSpeed = speed;}
inline Ogre::Real AnimationSet::getSpeed() const { return mSpeed; }


inline Ogre::Real Animation::getLengthOfOneIteration() const
{
	return mIterationLength;
}

inline int Animation::getIterations() const
{
	return mIterations;
}

}
}

#endif
