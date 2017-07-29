//
// C++ Interface: AnimationSet
//
// Description:
//
//
// Author: Erik Ogenvik <erik@ogenvik.org>, (C) 2005
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
// along with this program; if not, write to the Free Software Foundation,
// Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.//
//
#ifndef EMBEROGREANIMATIONSET_H
#define EMBEROGREANIMATIONSET_H
#include "components/ogre/EmberOgrePrerequisites.h"

#include <vector>

#include "ModelDefinition.h"

namespace Ember {
namespace OgreView {
namespace Model {

struct BoneGroupRef
{
	BoneGroupDefinition* boneGroupDefinition;
	Ogre::Real weight;
};

struct AnimationPart
{
	Ogre::AnimationState* state;
	std::vector<BoneGroupRef> boneGroupRefs;
};

class Animation;

typedef std::vector<AnimationPart> AnimationPartSet;
typedef std::vector<Animation> AnimationStore;

/**
@author Erik Ogenvik
*/
class Animation
{
public:
	Animation(int iterations, size_t boneNumber);
	void addAnimationPart(const AnimationPart& part);
	void setEnabled(bool state);

	void addTime(Ogre::Real timeSlice);
	void setTime(Ogre::Real time);

	int getIterations() const;
	Ogre::Real getLengthOfOneIteration() const;

	AnimationPartSet& getAnimationParts();
protected:
	AnimationPartSet mAnimationParts;
	Ogre::Real mIterationLength;
	int mIterations;
	size_t mBoneNumber;
};

class AnimationSet
{
public:
	AnimationSet();

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
	void addAnimation(const Animation& animation);


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

	const AnimationStore& getAnimations() const;
	AnimationStore& getAnimations();


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

}

#endif
