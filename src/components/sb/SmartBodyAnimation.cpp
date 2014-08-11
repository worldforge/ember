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

#include "SmartBodyAnimation.h"
#include "sb/SBAssetManager.h"
#include "sb/SBMotion.h"
#include "sb/SBBmlProcessor.h"

namespace Ember
{

SmartBodyAnimation::Type SmartBodyAnimation::getType(Name animationName)
{
	switch (animationName)
	{
		case Name::WALKING:
		case Name::RUNNING:
			return Type::MOVING;

		case Name::LOCOMOTION:
			return Type::INTUITIVE;

		case Name::STANDING:
			return Type::STATIC;

		case Name::WAITING:
			return Type::GESTURE;

		default:
			return Type::UNDEFINED;
	}
}


SmartBodyAnimation::SmartBodyAnimation(SmartBodyAnimation::Name name, SmartBody::SBAssetManager& assetManager, const std::vector<std::string>& motions)
: mName(name), mAssetManager(assetManager), mMotionNames(motions)
{
}

SmartBodyAnimation::~SmartBodyAnimation()
{
}

SmartBodyAnimation::Name SmartBodyAnimation::getName() const
{
	return mName;
}

float SmartBodyAnimation::getMotionDuration(int motionIndex) const
{
	if (motionIndex < 0 || !(motionIndex < getMotionNumber()))
	{
		return -1;
	}

	SmartBody::SBMotion *motion = mAssetManager.getMotion(mMotionNames[motionIndex]);

	if (!motion)
	{
		return -1;
	}

	return motion->getDuration();
}

int SmartBodyAnimation::getMotionNumber() const
{
	return mMotionNames.size();
}



SmartBodyAnimationInstance::SmartBodyAnimationInstance(const SmartBodyAnimation& animation, SmartBody::SBBmlProcessor& bmlProcessor, const std::string& character)
:	mReference(animation), mBmlProcessor(bmlProcessor), mCharacter(character), mHasStartTime(false), mHasReadyTime(false), mLastRequestId("")
{
}

SmartBodyAnimationInstance::~SmartBodyAnimationInstance()
{
	if (mLastRequestId != "")
	{
		mBmlProcessor.interruptBML(mCharacter, mLastRequestId, 0);
	}
}

int SmartBodyAnimationInstance::getMotionNumber() const
{
	return mReference.getMotionNumber();
}

void SmartBodyAnimationInstance::notifyUpdate()
{
	specifyReadyTime(false);
	specifyStartTime(false);
}

void SmartBodyAnimationInstance::specifyStartTime(bool specify, float startTime /*= 0.0f*/)
{
	if (specify)
	{
		mStartTime = startTime;
	}

	mHasStartTime = specify;
}

void SmartBodyAnimationInstance::specifyReadyTime(bool specify, float readyTime /*= 0.0f*/)
{
	if (specify)
	{
		mReadyTime = readyTime;
	}

	mHasReadyTime = specify;
}

void SmartBodyAnimationInstance::convertTimesToBmlStrings(std::vector<std::string>& times) const
{
	times.clear();

	if (mHasStartTime)
	{
		times.push_back(" start=\"" + std::to_string(mStartTime) + "\" ");

		if (mHasReadyTime)
		{
			times.push_back(" ready=\"" + std::to_string(mStartTime + mReadyTime) + "\" ");
		}
	}
}

const SmartBodyAnimation& SmartBodyAnimationInstance::getReference() const
{
	return mReference;
}

void SmartBodyAnimationInstance::execute(const std::string& characterName)
{
	std::string request;
	getBmlRequest(request);
	mLastRequestId = mBmlProcessor.execBML(characterName, request);

 	//Notify the animation instance that the request has been sent.
 	notifyUpdate();
}

}