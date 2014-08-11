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

#include "SmartBodyIntuitiveAnimation.h"


namespace Ember
{


int SmartBodyIntuitiveAnimation::getBlendDimension(Blend blend)
{
	switch (blend)
	{
		case Blend::LOCOMOTION:
			return 3;

		case Blend::STEP:
			return 2;

		case Blend::STARTING_LEFT:
		case Blend::STARTING_RIGHT:
		case Blend::IDLE_TURN:
			return 1;

		default:
			return -1;
	}
}

bool SmartBodyIntuitiveAnimation::areParametersValid(Blend blend, const std::vector<std::string>& parameters)
{
	return getBlendDimension(blend) == parameters.size();
}


SmartBodyIntuitiveAnimation::SmartBodyIntuitiveAnimation(SmartBodyAnimation::Name name, SmartBody::SBAssetManager& assetMng, 
		const std::string& locomotionBlend, const std::string& startingLfBlend, const std::string& startingRtBlend, const std::string& idleTurnBlend, const std::string& stepBlend)
: 	SmartBodyAnimation(name, assetMng, mBlends), mBlends((int)Blend::BLENDS_COUNT)
{
	mBlends[(int)Blend::LOCOMOTION] = locomotionBlend; 
	mBlends[(int)Blend::STARTING_LEFT] = startingLfBlend;
	mBlends[(int)Blend::STARTING_RIGHT] = startingRtBlend;
	mBlends[(int)Blend::IDLE_TURN] = idleTurnBlend;
	mBlends[(int)Blend::STEP] = stepBlend;
}

SmartBodyIntuitiveAnimation::SmartBodyIntuitiveAnimation(const SmartBodyIntuitiveAnimation& animation)
:	SmartBodyAnimation(animation.mName, animation.mAssetManager, mBlends), mBlends(animation.mBlends)
{
}

SmartBodyIntuitiveAnimation::~SmartBodyIntuitiveAnimation()
{
}

bool SmartBodyIntuitiveAnimation::getBmlRequest(std::string& request, Blend blend, const std::vector<std::string>& parameters) const
{
	if (!areParametersValid(blend, parameters))
	{
		return false;
	}

	request = "<blend name=\"" + mBlends[(int)blend] + "\"";

	for (auto& parameter : parameters)
	{
		request += parameter;
	}
	
	request += "/>";
	
	return true;
}

bool SmartBodyIntuitiveAnimation::getBmlRequest(std::string& request, int blendNumber, const std::vector<std::string>& parameters) const
{
	if (blendNumber < 0 || !(blendNumber < (int)Blend::BLENDS_COUNT))
	{
		return false;
	}

	return getBmlRequest(request, (Blend)blendNumber, parameters);
}


SmartBodyIntuitiveAnimationInstance::SmartBodyIntuitiveAnimationInstance(const SmartBodyIntuitiveAnimation& animation, SmartBody::SBBmlProcessor& bmlProcessor, const std::string& character, SmartBodyIntuitiveAnimation::Blend blend)
:	SmartBodyAnimationInstance(animation, bmlProcessor, character), mBlend(blend), mNbParameters(SmartBodyIntuitiveAnimation::getBlendDimension(blend)), mX(0.0f), mY(0.0f), mZ(0.0f)
{
}

SmartBodyIntuitiveAnimationInstance::~SmartBodyIntuitiveAnimationInstance()
{
}

bool SmartBodyIntuitiveAnimationInstance::getBmlRequest(std::string& request) const
{
	std::vector<std::string> parameters;
	
	if (mNbParameters > 0)
	{
		parameters.push_back(" x=\"" + std::to_string(mX) + "\" ");

		if (mNbParameters > 1)
		{
			parameters.push_back(" y=\"" + std::to_string(mY) + "\" ");

			if (mNbParameters > 2)
			{
				parameters.push_back(" z=\"" + std::to_string(mZ) + "\" ");
			}
		}
	}

	return dynamic_cast<const SmartBodyIntuitiveAnimation&>(mReference).getBmlRequest(request, mBlend, parameters);
}

void SmartBodyIntuitiveAnimationInstance::setBlend(SmartBodyIntuitiveAnimation::Blend blend)
{
	mBlend = blend;
}

void SmartBodyIntuitiveAnimationInstance::setBlendParameters(int nbParameters, float x /*= 0.0f*/, float y /*= 0.0f*/, float z /*= 0.0f */)
{
	mNbParameters = nbParameters;

	mX = x;
	mY = y;
	mZ = z;
}

SmartBodyIntuitiveAnimation::Blend SmartBodyIntuitiveAnimationInstance::getBlend() const
{
	return mBlend;
}

void SmartBodyIntuitiveAnimationInstance::notifyUpdate()
{
	mPrvPosition = mNewPosition;
	mPrvOrientation = mNewOrientation;
}

void SmartBodyIntuitiveAnimationInstance::initializePositionAndOrientation(const Ogre::Vector3& position, const Ogre::Quaternion& orientation)
{
	mPrvPosition = position;
	mPrvOrientation = orientation;
}

void SmartBodyIntuitiveAnimationInstance::setServerPositionAndOrientation(const Ogre::Vector3& position, const Ogre::Quaternion& orientation)
{
	mNewPosition = position;
	mNewOrientation = orientation;
}

void SmartBodyIntuitiveAnimationInstance::calculateBlendParameters(float timeSlice)
{
	switch (mBlend)
	{
		case SmartBodyIntuitiveAnimation::Blend::LOCOMOTION:
			calculateSpeedAngularSpeedAndDirectionParameters(timeSlice);
			break;

		case SmartBodyIntuitiveAnimation::Blend::STARTING_LEFT:
		case SmartBodyIntuitiveAnimation::Blend::STARTING_RIGHT:
		case SmartBodyIntuitiveAnimation::Blend::IDLE_TURN:
			calculateAngleParameter(timeSlice);
			break;

		case SmartBodyIntuitiveAnimation::Blend::STEP:
			calculateTransitionParameter(timeSlice);
			break;

		default:
			break;
	}
}

void SmartBodyIntuitiveAnimationInstance::calculateSpeedAngularSpeedAndDirectionParameters(float timeSlice)
{
	Ogre::Vector3 prvDirection(mPrvOrientation.x, mPrvOrientation.y, mPrvOrientation.z);
	Ogre::Vector3 newDirection(mNewOrientation.x, mNewOrientation.y, mNewOrientation.z);
	Ogre::Vector3 translation(mNewPosition - mPrvPosition);

	//Calculate x parameter: speed.
	float dist = translation.length();
	float speed = dist / timeSlice;

	//Calculate y parameter: angular speed.
	float angle = prvDirection.getRotationTo(newDirection).getYaw().valueDegrees();
	float angularSpeed = angle / timeSlice;

	//Calculate z parameter: direction.
	//This one is represented by the angle between the previous orientation and the direction of the velocity vector.
	float direction = prvDirection.getRotationTo(translation).getYaw().valueDegrees();

	setBlendParameters(3, speed, angle, direction);
}

void SmartBodyIntuitiveAnimationInstance::calculateAngleParameter(float timeSlice)
{
	//The yaw between the previous orientation and the new one.
	float angle = Ogre::Vector3(mPrvOrientation.x, mPrvOrientation.y, mPrvOrientation.z).getRotationTo(Ogre::Vector3(mNewOrientation.x, mNewOrientation.y, mNewOrientation.z)).getYaw().valueDegrees();
	
	setBlendParameters(1, angle);
}

void SmartBodyIntuitiveAnimationInstance::calculateTransitionParameter(float timeSlice)
{
	//TODO.
}


}