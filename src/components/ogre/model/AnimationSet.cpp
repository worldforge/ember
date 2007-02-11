//
// C++ Implementation: AnimationSet
//
// Description: 
//
//
// Author: Erik Hjortsberg <erik@katastrof.nu>, (C) 2005
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
#include "AnimationSet.h"

namespace EmberOgre {
namespace Model {


AnimationSet::AnimationSet() : mAccumulatedTime(0), mCurrentAnimationSetIndex(0), mSpeed(1.0)
{
}

AnimationSet::~AnimationSet()
{
/*	for (AnimationStore::iterator I = mAnimations.begin(); I != mAnimations.end(); ++I) {
		delete *I;
	}*/
}

void AnimationSet::addTime(Ogre::Real timeSlice)
{
	static bool discardThis;
	addTime(timeSlice, discardThis);
}


void AnimationSet::addTime(Ogre::Real timeSlice, bool& continueAnimation)
{
	if (mAnimations.size() > 0) {
		continueAnimation = true;
		Animation* animation = &mAnimations[mCurrentAnimationSetIndex];
		float animationLength = animation->getLengthOfOneIteration();
		Ogre::Real calculatedTime = timeSlice * mSpeed;
		if (animationLength == 0) {
			///Ogre will throw an assert exception if an animtion with zero length is activated
		} else {
			///see if we've done enough iterations to either advance to the next animation, or the mark this animation as done
			if (fabs(mAccumulatedTime) >= animation->getLengthOfOneIteration() * animation->getIterations()) {
				animation->setEnabled(false);
				mAccumulatedTime = 0;
				if (mAnimations.size() > (mCurrentAnimationSetIndex + 1)) {
					++mCurrentAnimationSetIndex;
					continueAnimation = true;
				} else {
					mCurrentAnimationSetIndex = 0;
					continueAnimation = false;
				}
				animation = &mAnimations[mCurrentAnimationSetIndex];
				animation->setTime(0);
			}
			
			Ogre::Real calculatedTime = timeSlice * mSpeed;
			animation->setEnabled(true);
			animation->addTime(calculatedTime);
		}
		mAccumulatedTime += calculatedTime;
	}
	
}

void AnimationSet::reset()
{
	if (mAnimations.size() > 0) {
		Animation& animation = mAnimations[mCurrentAnimationSetIndex];
		animation.setEnabled(false);
	}
	mCurrentAnimationSetIndex = 0;
	mAccumulatedTime = 0;
}
	
void AnimationSet::addAnimation(Animation animation)
{
	mAnimations.push_back(animation);
}


Animation::Animation(int iterations) : mIterationLength(0), mIterations(iterations) {}

void Animation::addAnimationPart(AnimationPart part)
{
	mAnimationParts.push_back(part);
	mIterationLength = std::max<Ogre::Real>(part.state->getLength(), mIterationLength);
}

void Animation::addTime(Ogre::Real timeSlice)
{
	AnimationPartSet::iterator I = mAnimationParts.begin();
	for (; I != mAnimationParts.end(); ++I) {
		///we'll get an assert error if we try to add time to an animation with zero length
		if (I->state->getLength() != 0) {
			I->state->addTime(timeSlice);
		}
	}
}

void Animation::setTime(Ogre::Real time)
{
	AnimationPartSet::iterator I = mAnimationParts.begin();
	for (; I != mAnimationParts.end(); ++I) {
		///we'll get an assert error if we try to add time to an animation with zero length
		if (I->state->getLength() != 0) {
			I->state->setTimePosition(time);
		}
	}
}

void Animation::setEnabled(bool state)
{
	AnimationPartSet::iterator I = mAnimationParts.begin();
	for (; I != mAnimationParts.end(); ++I) {
		///we'll get an assert error if we try to enable an animation with zero length
		if (I->state->getLength() != 0) {
			if (I->state->getEnabled() != state) {
				I->state->setEnabled(state);
			}
		}
	}
}

// bool Animation::hasCompleted() const
// {
// 	AnimationPartSet::const_iterator I = mAnimations.begin();
// 	for (; I != mAnimations.end(); ++I) {
// 		if (I->state->getTimePosition() < I->state->getLength()) {
// 			return false;
// 		}
// 	}
// 	return true;
// }

}
}
