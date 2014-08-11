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

#include "SmartBodyGestureAnimation.h"
#include "sb/SBBmlProcessor.h"

namespace Ember
{


SmartBodyGestureAnimation::Gesturing::Gesturing(Name name, int repeat /*= 1*/, float duration /*= 1.0f*/)
:	mName(name), mDuration(duration)
{
	std::string type;

	switch (name)
	{
		case Name::NOD:
			type = "NOD";
			break;
		case Name::SHAKE:
			type = "SHAKE";
			break;
		case Name::WIGGLE:
			type = "WIGGLE";
			break;
		case Name::WAGGLE:
			type = "WAGGLE";
			break;
		case Name::TOSS:
			type = "TOSS";
			break;			
	}

	//Check that the duration is a positive value.
	if (duration < 0)
	{
		duration = -duration;
	}

	mBmlRequest = "<head type=\"" + type + "\" repeat=\"" + std::to_string(repeat) + "\" start=\"0\" end=\"" + std::to_string(duration) + "\"/>";
}

SmartBodyGestureAnimation::Gesturing::~Gesturing()
{
}

const std::string& SmartBodyGestureAnimation::Gesturing::getBmlRequest() const
{
	return mBmlRequest;
}

float SmartBodyGestureAnimation::Gesturing::getDuration() const
{
	return mDuration;
}



SmartBodyGestureAnimation::SmartBodyGestureAnimation(SmartBodyAnimation::Name name, SmartBody::SBAssetManager& assetMng, 
	const std::vector<std::string>& motions, const std::vector<SmartBodyGestureAnimation::Gesturing>& gesturings)
:	SmartBodyAnimation::SmartBodyAnimation(name, assetMng, mMotions), mMotions(motions), mGesturings(gesturings)
{
}

SmartBodyGestureAnimation::SmartBodyGestureAnimation(const SmartBodyGestureAnimation& animation)
:	SmartBodyAnimation::SmartBodyAnimation(animation.mName, animation.mAssetManager, mMotions), mMotions(animation.mMotions), mGesturings(animation.mGesturings)
{
}
	
SmartBodyGestureAnimation::~SmartBodyGestureAnimation()
{
}

bool SmartBodyGestureAnimation::getBmlRequest(std::string& request, int gestureIndex, const std::vector<std::string>& attributes) const
{
	if (gestureIndex < 0 || getMotionNumber() <= gestureIndex)
	{
		return false;
	}

	if (gestureIndex < mMotions.size())
	{
		request = "<gesture name=\"" + mMotions[gestureIndex] + "\"";

		for (auto& attribute : attributes)
		{
			request += attribute;
		}

		request += "/>";
	}

	else
	{
		request = mGesturings[gestureIndex - mMotions.size()].getBmlRequest();
	}

	return true;
}

float SmartBodyGestureAnimation::getMotionDuration(int gestureIndex) const
{
	if (gestureIndex < 0 || !(gestureIndex < getMotionNumber()))
	{
		return -1;
	}

	if (gestureIndex < mMotions.size())
	{
		return SmartBodyAnimation::getMotionDuration(gestureIndex);
	}

	else
	{
		return mGesturings[gestureIndex - mMotions.size()].getDuration();
	}
}

int SmartBodyGestureAnimation::getMotionNumber() const
{
	return mMotions.size() + mGesturings.size();
}



SmartBodyGestureAnimationInstance::SmartBodyGestureAnimationInstance(const SmartBodyGestureAnimation& reference, SmartBody::SBBmlProcessor& bmlProcessor, const std::string& character)
:	SmartBodyAnimationInstance::SmartBodyAnimationInstance(reference, bmlProcessor, character)
{
}
	
SmartBodyGestureAnimationInstance::~SmartBodyGestureAnimationInstance()
{
}

bool SmartBodyGestureAnimationInstance::getBmlRequest(std::string& request) const
{
	std::vector<std::string> times;
	convertTimesToBmlStrings(times);

	return mReference.getBmlRequest(request, mGesture, times);
}

float SmartBodyGestureAnimationInstance::getTimeSinceGestureEnd()
{
	return mTimeSinceGestureEnd;
}

void SmartBodyGestureAnimationInstance::updateTimer(float timeSlice)
{
	mTimeSinceGestureEnd += timeSlice;
}

bool SmartBodyGestureAnimationInstance::playGesture(int gestureIndex)
{
	float duration = dynamic_cast<const SmartBodyGestureAnimation&>(mReference).getMotionDuration(gestureIndex);

	if (duration < 0)
	{
		//If something went wrong, we return false.
		return false;
	}

	mGesture = gestureIndex;
	mTimeSinceGestureEnd = -duration;

	return true;
}




}