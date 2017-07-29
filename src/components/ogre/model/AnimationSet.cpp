//
// C++ Implementation: AnimationSet
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
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "AnimationSet.h"
#include <OgreAnimationState.h>

namespace Ember
{
namespace OgreView
{
namespace Model
{

AnimationSet::AnimationSet() :
		mAccumulatedTime(0), mCurrentAnimationSetIndex(0), mSpeed(1.0)
{
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
			//Ogre will throw an assert exception if an animation with zero length is activated
		} else {
			//see if we've done enough iterations to either advance to the next animation, or the mark this animation as done
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

			animation->setEnabled(true);
			animation->addTime(calculatedTime);
		}
		mAccumulatedTime += calculatedTime;
	}

}

void AnimationSet::reset()
{
	for (AnimationStore::iterator I = mAnimations.begin(); I != mAnimations.end(); ++I) {
		I->setEnabled(false);
		I->setTime(0.0f);
	}
	mCurrentAnimationSetIndex = 0;
	mAccumulatedTime = 0;
}

const AnimationStore& AnimationSet::getAnimations() const
{
	return mAnimations;
}

AnimationStore& AnimationSet::getAnimations()
{
	return mAnimations;
}

void AnimationSet::addAnimation(const Animation& animation)
{
	mAnimations.push_back(animation);
}

Animation::Animation(int iterations, size_t boneNumber) :
		mIterationLength(0), mIterations(iterations), mBoneNumber(boneNumber)
{
}

AnimationPartSet& Animation::getAnimationParts()
{
	return mAnimationParts;
}

void Animation::addAnimationPart(const AnimationPart& part)
{
	mAnimationParts.push_back(part);
	mIterationLength = std::max<Ogre::Real>(part.state->getLength(), mIterationLength);
}

void Animation::addTime(Ogre::Real timeSlice)
{
	AnimationPartSet::iterator I = mAnimationParts.begin();
	for (; I != mAnimationParts.end(); ++I) {
		//we'll get an assert error if we try to add time to an animation with zero length
		if (I->state->getLength() != 0) {
			I->state->addTime(timeSlice);
		}
	}
}

void Animation::setTime(Ogre::Real time)
{

	for (AnimationPartSet::iterator I = mAnimationParts.begin(); I != mAnimationParts.end(); ++I) {
		//we'll get an assert error if we try to add time to an animation with zero length
		if (I->state->getLength() != 0) {
			I->state->setTimePosition(time);
		}
	}
}

void Animation::setEnabled(bool enabled)
{
	for (AnimationPartSet::const_iterator I = mAnimationParts.begin(); I != mAnimationParts.end(); ++I) {
		//we'll get an assert error if we try to enable an animation with zero length
		Ogre::AnimationState* state = I->state;
		if (state->getLength() != 0) {
			if (state->getEnabled() != enabled) {
				state->setEnabled(enabled);
				state->destroyBlendMask();
				if (enabled) {
					const std::vector<BoneGroupRef>& boneGroupRefs = I->boneGroupRefs;
					for (std::vector<BoneGroupRef>::const_iterator J = boneGroupRefs.begin(); J != boneGroupRefs.end(); ++J) {
						const BoneGroupRef& boneGroupRef = *J;
						const BoneGroupDefinition& boneGroupDef = *boneGroupRef.boneGroupDefinition;
						if (!state->hasBlendMask()) {
							state->createBlendMask(mBoneNumber, 0.0f);
						}
						const std::vector<size_t>& boneIndices = boneGroupDef.Bones;
						for (std::vector<size_t>::const_iterator bones_I = boneIndices.begin(); bones_I != boneIndices.end(); ++bones_I) {
							state->setBlendMaskEntry(*bones_I, boneGroupRef.weight);
						}
					}
				}
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
}
