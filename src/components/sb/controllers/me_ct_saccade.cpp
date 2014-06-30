/*
 *  me_ct_saccade.cpp - part of Motion Engine and SmartBody-lib
 *  Copyright (C) 2011  University of Southern California
 *
 *  SmartBody-lib is free software: you can redistribute it and/or
 *  modify it under the terms of the Lesser GNU General Public License
 *  as published by the Free Software Foundation, version 3 of the
 *  license.
 *
 *  SmartBody-lib is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  Lesser GNU General Public License for more details.
 *
 *  You should have received a copy of the Lesser GNU General Public
 *  License along with SmartBody-lib.  If not, see:
 *      http://www.gnu.org/licenses/lgpl-3.0.txt
 *
 *  CONTRIBUTORS:
 *      Yuyu Xu, USC
 */

#include "controllers/me_ct_saccade.h"
#include <cstdlib>
#include <ctime> 
#include <math.h>

#include <sb/SBSimulationManager.h>
#include <sb/SBScene.h>
#include <sb/SBSkeleton.h>
#include <sb/SBCharacter.h>
#include <sb/SBAttributeManager.h>

using namespace gwiz;
std::string MeCtSaccade::CONTROLLER_TYPE = "Saccade";
std::string eyeballL = "eyeball_left";
std::string eyeballR = "eyeball_right";


MeCtSaccade::MeCtSaccade(SbmCharacter* c) : SmartBody::SBController()
{
	_character = c;

	// controllers
	_useModel = true;
	_valid = false;
	_validByPolicy = true;
	_initialized = false;
	_attributeInitialized = false;
	_idL = -1;
	_idR = -1;
	_prevTime = 0.0;
	_dur = 0.0f;
	_time = -1.0f;
	_direction = 0.0f;

	// saccade
	_intervalMode = Mutual;
	_behaviorMode = Listening;
	_minInterval = 0.001f;
	_intercept = 0.025f;				// unit: sec
	_slope = 0.0024f;					// unit: sec/degree

	srand((unsigned int)time(0));	
}

MeCtSaccade::~MeCtSaccade()
{
	_character = NULL;
}

/*
	Saccade parameters per mode (Reference: Eye Alive)
		- percentage bin for different mode
		- magnitude limit
		- percentage between gaze mutual and gaze away for different mode
		- mean and variant for different mode and different gaze type
*/

void MeCtSaccade::initAttributes()
{
	if (!_attributeInitialized)
	{
		SmartBody::SBAttributeGroup* group = _character->getAttributeManager()->getGroup("Saccade", true);
		group->setPriority(700);
		// Data from papar "eyes alive" - listening mode
		_character->createDoubleAttribute("saccade.listening.percentage0", 15.54f, true, "Saccade", 2, false, false, false, "listening mode, percentage bin for 0 degree.");
		_character->createDoubleAttribute("saccade.listening.percentage45", 6.46f, true, "Saccade", 3, false, false, false, "listening mode, percentage bin for 45 degree.");
		_character->createDoubleAttribute("saccade.listening.percentage90", 17.69f, true, "Saccade", 4, false, false, false, "listening mode, percentage bin for 90 degree.");
		_character->createDoubleAttribute("saccade.listening.percentage135", 7.44f, true, "Saccade", 5, false, false, false, "listening mode, percentage bin for 135 degree.");
		_character->createDoubleAttribute("saccade.listening.percentage180", 16.80f, true, "Saccade", 6, false, false, false, "listening mode, percentage bin for 180 degree.");
		_character->createDoubleAttribute("saccade.listening.percentage225", 7.89f, true, "Saccade", 7, false, false, false, "listening mode, percentage bin for 225 degree.");
		_character->createDoubleAttribute("saccade.listening.percentage270", 20.38f, true, "Saccade", 8, false, false, false, "listening mode, percentage bin for 270 degree.");
		_character->createDoubleAttribute("saccade.listening.percentage315", 7.79f, true, "Saccade", 9, false, false, false, "listening mode, percentage bin for 315 degree.");
		_character->createDoubleAttribute("saccade.listening.magnitudeLimit", 10.0f, true, "Saccade", 10, false, false, false, "listening mode, magnitude limit.");
		_character->createDoubleAttribute("saccade.listening.percentageMutual", 75.0f, true, "Saccade", 11, false, false, false, "listening mode, percentage for gaze mutual.");
		_character->createDoubleAttribute("saccade.listening.mutualMean", 237.5f, true, "Saccade", 12, false, false, false, "listening mode, Gaussian mean for gaze mutual.");
		_character->createDoubleAttribute("saccade.listening.mutualVariant", 47.1f, true, "Saccade", 13, false, false, false, "listening mode, Gaussian variant for gaze mutual.");
		_character->createDoubleAttribute("saccade.listening.awayMean", 13.0f, true, "Saccade", 14, false, false, false, "listening mode, Gaussian mean for gaze away.");
		_character->createDoubleAttribute("saccade.listening.awayVariant", 7.1f, true, "Saccade", 15, false, false, false, "listening mode, Gaussian variant for gaze away.");

		// Data from papar "eyes alive" - talking mode
		_character->createDoubleAttribute("saccade.talking.percentage0", 15.54f, true, "Saccade", 20, false, false, false, "talking mode, percentage bin for 0 degree.");
		_character->createDoubleAttribute("saccade.talking.percentage45", 6.46f, true, "Saccade", 21, false, false, false, "talking mode, percentage bin for 45 degree.");
		_character->createDoubleAttribute("saccade.talking.percentage90", 17.69f, true, "Saccade", 22, false, false, false, "talking mode, percentage bin for 90 degree.");
		_character->createDoubleAttribute("saccade.talking.percentage135", 7.44f, true, "Saccade", 23, false, false, false, "talking mode, percentage bin for 135 degree.");
		_character->createDoubleAttribute("saccade.talking.percentage180", 16.80f, true, "Saccade", 24, false, false, false, "talking mode, percentage bin for 180 degree.");
		_character->createDoubleAttribute("saccade.talking.percentage225", 7.89f, true, "Saccade", 25, false, false, false, "talking mode, percentage bin for 225 degree.");
		_character->createDoubleAttribute("saccade.talking.percentage270", 20.38f, true, "Saccade", 26, false, false, false, "talking mode, percentage bin for 270 degree.");
		_character->createDoubleAttribute("saccade.talking.percentage315", 7.79f, true, "Saccade", 27, false, false, false, "talking mode, percentage bin for 315 degree.");
		_character->createDoubleAttribute("saccade.talking.magnitudeLimit", 12.0f, true, "Saccade", 28, false, false, false, "talking mode, magnitude limit.");
		_character->createDoubleAttribute("saccade.talking.percentageMutual", 41.0f, true, "Saccade", 29, false, false, false, "talking mode, percentage for mutual gaze.");
		_character->createDoubleAttribute("saccade.talking.mutualMean", 93.9f, true, "Saccade", 30, false, false, false, "talking mode, Gaussian mean for gaze mutual.");
		_character->createDoubleAttribute("saccade.talking.mutualVariant", 94.9f, true, "Saccade", 31, false, false, false, "talking mode, Gaussian variant for gaze mutual.");
		_character->createDoubleAttribute("saccade.talking.awayMean", 27.8f, true, "Saccade", 32, false, false, false, "talking mode, Gaussian mean for gaze away.");
		_character->createDoubleAttribute("saccade.talking.awayVariant", 24.0f, true, "Saccade", 33, false, false, false, "talking mode, Gaussian variant for gaze away.");

		// Thinking mode data is ad-hoc - thinking mode
		_character->createDoubleAttribute("saccade.thinking.percentage0", 5.46f, true, "Saccade", 40, false, false, false, "thinking mode, percentage bin for 0 degree.");
		_character->createDoubleAttribute("saccade.thinking.percentage45", 10.54f, true, "Saccade", 41, false, false, false, "thinking mode, percentage bin for 45 degree.");
		_character->createDoubleAttribute("saccade.thinking.percentage90", 24.69f, true, "Saccade", 42, false, false, false, "thinking mode, percentage bin for 90 degree.");
		_character->createDoubleAttribute("saccade.thinking.percentage135", 6.44f, true, "Saccade", 43, false, false, false, "thinking mode, percentage bin for 135 degree.");
		_character->createDoubleAttribute("saccade.thinking.percentage180", 6.89f, true, "Saccade", 44, false, false, false, "thinking mode, percentage bin for 180 degree.");
		_character->createDoubleAttribute("saccade.thinking.percentage225", 12.80f, true, "Saccade", 45, false, false, false, "thinking mode, percentage bin for 225 degree.");
		_character->createDoubleAttribute("saccade.thinking.percentage270", 26.38f, true, "Saccade", 46, false, false, false, "thinking mode, percentage bin for 270 degree.");
		_character->createDoubleAttribute("saccade.thinking.percentage315", 6.79f, true, "Saccade", 47, false, false, false, "thinking mode, percentage bin for 315 degree.");
		_character->createDoubleAttribute("saccade.thinking.magnitudeLimit", 12.0f, true, "Saccade", 48, false, false, false, "thinking mode, magnitude limit.");
		_character->createDoubleAttribute("saccade.thinking.percentageMutual", 20.0f, true, "Saccade", 49, false, false, false, "thinking mode, percentage for mutual gaze.");
		_character->createDoubleAttribute("saccade.thinking.mutualMean", 180.0f, true, "Saccade", 50, false, false, false, "thinking mode, Gaussian mean for gaze mutual.");
		_character->createDoubleAttribute("saccade.thinking.mutualVariant", 47.0f, true, "Saccade", 51, false, false, false, "thinking mode, Gaussian variant for gaze mutual.");
		_character->createDoubleAttribute("saccade.thinking.awayMean", 180.0f, true, "Saccade", 52, false, false, false, "thinking mode, Gaussian mean for gaze away.");
		_character->createDoubleAttribute("saccade.thinking.awayVariant", 47.0f, true, "Saccade", 53, false, false, false, "thinking mode, Gaussian variant for gaze away.");

		_attributeInitialized = true;
		setBehaviorMode(Listening);
	}
}

void MeCtSaccade::spawnOnce(float dir, float amplitude, float dur)
{
	_direction = dir;
	_magnitude = amplitude;
	_dur = dur;

	//SrVec vec1 = SrVec(0, 0, 1);
	SrVec vec1 = _localAxis[2];
	float direction = (_direction - 90.0f) * (float)M_PI / 180.0f;		// 90.0f here is for adjustment
	SrVec vec2 = _localAxis[0]*sin(direction) + _localAxis[1]*cos(direction);//SrVec(sin(direction), cos(direction), 0);
	_axis = cross(vec1, vec2);
	_lastFixedRotation = _rotation;
	_fixedRotation = SrQuat(_axis, _magnitude * (float)M_PI / 180.0f);

	_time = (float) SmartBody::SBScene::getScene()->getSimulationManager()->getTime();;
}

void MeCtSaccade::reset(double t)
{
	_fixedRotation = SrQuat(_axis, 0.0f);
	_lastFixedRotation = _fixedRotation;
	_rotation = SrQuat();

	_time = (float)t + (float)_character->getDoubleAttribute("saccadeTurnOnDelay");
}

void MeCtSaccade::spawning(double t)
{
	float time = float(t);
	if (_time == -1.0f || t > (_time + _dur))
	{
		_direction = directionRandom();			// degree
		_magnitude = magnitudeRandom();			// degree

		//SrVec vec1 = SrVec(0, 0, 1);
		SrVec vec1 = _localAxis[2];
		float direction = (_direction - 90.0f) * (float)M_PI / 180.0f;		// 90.0f here is for adjustment
		//SrVec vec2 = SrVec(sin(direction), cos(direction), 0);
		SrVec vec2 = _localAxis[0]*sin(direction) + _localAxis[1]*cos(direction);
		_axis = cross(vec1, vec2);
		_lastFixedRotation = _fixedRotation;
		_fixedRotation = SrQuat(_axis, _magnitude * (float)M_PI / 180.0f);

		SrQuat actualRotation = _fixedRotation * _lastFixedRotation.inverse();
		float angle = actualRotation.angle() * 180.0f / (float)M_PI;

	//	_dur = duration(_magnitude);			// sec
		_dur = duration(angle);
		float interval = intervalRandom();		// sec
		_time = time + interval;

#if 0
		LOG("--Next Saccade happens at %f, turning angle=%f, duration=%f, interval=%f, direction=%f", _time, actualRotation.angle(), _dur, interval, _direction);
#endif
	}
	if (_time == -1.0f)
		LOG("MeCtSaccade::update ERR: this is not suppose to happen.");
}


void MeCtSaccade::processing(double t, MeFrameData& frame)
{
        if (_idL < 0 || _idR < 0)
            return;
	float time = float(t);
	float dt = (float) SmartBody::SBScene::getScene()->getSimulationManager()->getTimeDt();;
	if (_time == -1.0f)
		return;

	if ((t > (_time + _dur)) && !_useModel)
	{
		_lastFixedRotation = SrQuat();
		_fixedRotation = SrQuat();
		_rotation = SrQuat();
	}
	//_rotation = _lastFixedRotation;
	if (t >= _time && t <= (_time + _dur))
	{
		float r = (time - _time) / _dur;
	//	float y = 1 - r;
		float y = 1 - sqrt(1 - (r - 1) * (r - 1));
		_rotation = slerp(_lastFixedRotation, _fixedRotation, 1 - y);
	}
	//---
	SrQuat QL = SrQuat( frame.buffer()[_idL + 0],
						frame.buffer()[_idL + 1],
						frame.buffer()[_idL + 2],
						frame.buffer()[_idL + 3] );
	
	SrQuat QR = SrQuat( frame.buffer()[_idR + 0],
						frame.buffer()[_idR + 1],
						frame.buffer()[_idR + 2],
						frame.buffer()[_idR + 3] );

	//--- process
	SrQuat temp(_lastFixedRotation);
	SrQuat actualRot = temp * _rotation;
//	SrQuat outQL = QL * actualRot;
	SrQuat outQL = QL * _rotation;
	
	SrVec rotL = outQL.axisAngle();
	rotL = rotL*_leftRightRot;
	
	SrQuat outQR = SrQuat(rotL);

	//---
	frame.buffer()[_idL + 0] = outQL.w;
	frame.buffer()[_idL + 1] = outQL.x;
	frame.buffer()[_idL + 2] = outQL.y;
	frame.buffer()[_idL + 3] = outQL.z;

	frame.buffer()[_idR + 0] = outQR.w;
	frame.buffer()[_idR + 1] = outQR.x;
	frame.buffer()[_idR + 2] = outQR.y;
	frame.buffer()[_idR + 3] = outQR.z;
}

float MeCtSaccade::floatRandom(float min, float max)
{
	float r = (float)rand() / (float)RAND_MAX;
	return min + r * (max - min);
}

float MeCtSaccade::gaussianRandom(float mean, float variant)
{
	static double V1, V2, S;
	static int phase = 0;
	double X;
	if (phase == 0) 
	{
		do 
		{
			double U1 = (double)rand() / RAND_MAX;
			double U2 = (double)rand() / RAND_MAX;
			V1 = 2 * U1 - 1;
			V2 = 2 * U2 - 1;
			S = V1 * V1 + V2 * V2;
		} 
		while(S >= 1 || S == 0);
		X = V1 * sqrt(-2 * log(S) / S);
	} 
	else
		X = V2 * sqrt(-2 * log(S) / S);
	phase = 1 - phase;
	double Xp = X * sqrt(variant) + mean;	// X is for standard normal distribution
	return (float)Xp;	
}

float MeCtSaccade::directionRandom()
{
	float bound0, bound45, bound90, bound135, bound180, bound225, bound270, bound315;
	bound0 = _percentBin0;
	bound45 = bound0 + _percentBin45;
	bound90 = bound45 + _percentBin90;
	bound135 = bound90 + _percentBin135;
	bound180 = bound135 + _percentBin180;
	bound225 = bound180 + _percentBin225;
	bound270 = bound225 + _percentBin270;
	bound315 = bound270 + _percentBin315;

	float dir = 0.0f;
	float binIndex = floatRandom(0.0f, 100.0f);
	if (binIndex >= 0.0f && binIndex <= bound0)
		dir = 0.0f;
	if (binIndex >= bound0 && binIndex <= bound45)
		dir = 45.0f;
	if (binIndex >= bound45 && binIndex <= bound90)
		dir = 90.0f;
	if (binIndex >= bound90 && binIndex <= bound135)
		dir = 135.0f;
	if (binIndex >= bound135 && binIndex <= bound180)
		dir = 180.0f;
	if (binIndex >= bound180 && binIndex <= bound225)
		dir = 225.0f;
	if (binIndex >= bound225 && binIndex <= bound270)
		dir = 270.0f;
	if (binIndex >= bound270 && binIndex <= bound315)
		dir = 315.0f;
	return dir;
}

float MeCtSaccade::magnitudeRandom()
{
	float f = floatRandom(0.0f, 15.0f);
	float a = -6.9f * log(f / 15.7f);
	float limit = _magnitudeLimit;

	// 0.5f, 0.75f are regulated by the eye shape
	// direction 0 and 180 is moving up and down, it should have a limit
	if (_direction == 90.0f || _direction == 270.0f)
		limit *= 0.5f;
	if (_direction == 45.0f || _direction == 135.0f || _direction == 225.0f || _direction == 315.0f)
		limit *= 0.75f;

	if (a > limit)
		a = limit;
	return a;
}

float MeCtSaccade::intervalRandom()
{
	float f = floatRandom(0.0f, 100.0f);
	float mutualPercent = _percentMutual;

	if (f >= 0.0f && f <= mutualPercent)
		_intervalMode = Mutual;
	else
		_intervalMode = Away;
	float interval = -1.0f;
	while (interval < _minInterval)
	{
		if (_intervalMode == Mutual)
			interval = gaussianRandom(_mutualMean * (float)_dt, _mutualVariant * (float)_dt);
		if (_intervalMode == Away)
			interval = gaussianRandom(_awayMean * (float)_dt, _awayVariant * (float)_dt);
	}
	return interval;
}

float MeCtSaccade::duration(float amplitude)	// amplitude unit: degree
{
	float slope = _slope;
	float d0 = _intercept;
	float dur = d0 + slope * amplitude;
	return dur;
}

void MeCtSaccade::initSaccade(MeFrameData& frame)
{
	if (!_context)
		return;

	if (!_initialized)
	{

		SkJoint* lEyeJoint = _character->getSkeleton()->getJointByName(eyeballL.c_str());
		SkJoint* rEyeJoint = _character->getSkeleton()->getJointByName(eyeballR.c_str());
		if (lEyeJoint && rEyeJoint)
		{
			_leftRightRot = SrQuat(lEyeJoint->gmatZero()*rEyeJoint->gmatZero().inverse());
		for (int i=0;i<3;i++)
			_localAxis[i] = lEyeJoint->localGlobalAxis(i);
		}
		
		int idL = _context->channels().search(eyeballL, SkChannel::Quat);
		_idL = frame.toBufferIndex(idL);
		int idR = _context->channels().search(eyeballR, SkChannel::Quat);
		_idR = frame.toBufferIndex(idR);
		if (_idL < 0 || _idR < 0)
                {
			LOG("MeCtSaccade::initBufferIndex Warning: channel id not correct!");
                        _valid = false;
                }

		_initialized = true;
	}  
}

bool MeCtSaccade::controller_evaluate(double t, MeFrameData& frame)
{	
	if (_prevTime == 0)
		_dt = 0.016;
	else
	{
		_dt = t - _prevTime;
		_prevTime = t;
	}
	SmartBody::SBCharacter* sbCharacter = dynamic_cast<SmartBody::SBCharacter*> (_character);
	if (sbCharacter->getStringAttribute("saccadePolicy") == "stopinutterance")
	{
		if (sbCharacter->hasSpeechBehavior() != "")
		{
			_validByPolicy = false;
			reset(t);
		}
		else
		{
			_validByPolicy = true;
		}
	}


	SrBuffer<float>& buff = frame.buffer();
	initAttributes();
	initSaccade(frame);
	if (_valid)
	{
		if (_useModel)
			spawning(t);
		processing(t, frame);
	}
	return true;
}

void MeCtSaccade::setPercentageBins(float b0, float b45, float b90, float b135, float b180, float b225, float b270, float b315)
{
	b315 = 100 - b0 - b45 - b135 - b180 - b225 - b270;
	_percentBin0 = b0;
	_percentBin45 = b45;
	_percentBin90 = b90;
	_percentBin135 = b135;
	_percentBin180 = b180;
	_percentBin225 = b225;
	_percentBin270 = b270;
	_percentBin315 = b315;
}

void MeCtSaccade::setGaussianParameter(float mean, float variant)
{
	_mutualMean = mean;
	_mutualVariant = variant;
	_awayMean = mean;
	_awayVariant = variant;
}

void MeCtSaccade::setAngleLimit(float angle)
{
	_magnitudeLimit = angle;
}

float MeCtSaccade::getAngleLimit()
{
	return _magnitudeLimit;
}


void MeCtSaccade::setBehaviorMode(BehaviorMode m)
{
	_behaviorMode = m;
	std::string modeString = "";
	if (_behaviorMode == Talking)
	{
		modeString = "talking";
	}
	if (_behaviorMode == Listening)
	{
		modeString = "listening";
	}
	if (_behaviorMode == Thinking)
	{
		modeString = "thinking";
	}
	if (modeString == "")
		return;

	//LOG("Saccade mode set to %s", modeString.c_str());

	std::string percentage0 = "saccade." + modeString + ".percentage0";
	std::string percentage45 = "saccade." + modeString + ".percentage45";
	std::string percentage90 = "saccade." + modeString + ".percentage90";
	std::string percentage135 = "saccade." + modeString + ".percentage135";
	std::string percentage180 = "saccade." + modeString + ".percentage180";
	std::string percentage225 = "saccade." + modeString + ".percentage225";
	std::string percentage270 = "saccade." + modeString + ".percentage270";
	std::string percentage315 = "saccade." + modeString + ".percentage315";
	std::string magnitudeLimit = "saccade." + modeString + ".magnitudeLimit";
	std::string percentageMutual = "saccade." + modeString + ".percentageMutual";
	std::string mutualMean = "saccade." + modeString + ".mutualMean";
	std::string mutualVariant = "saccade." + modeString + ".mutualVariant";
	std::string awayMean = "saccade." + modeString + ".awayMean";
	std::string awayVariant = "saccade." + modeString + ".awayVariant";

	setPercentageBins(	(float)_character->getDoubleAttribute(percentage0), (float)_character->getDoubleAttribute(percentage45),
						(float)_character->getDoubleAttribute(percentage90), (float)_character->getDoubleAttribute(percentage135),
						(float)_character->getDoubleAttribute(percentage180), (float)_character->getDoubleAttribute(percentage225),
						(float)_character->getDoubleAttribute(percentage270), (float)_character->getDoubleAttribute(percentage315)
						);
	_magnitudeLimit = (float)_character->getDoubleAttribute(magnitudeLimit);
	_percentMutual = (float)_character->getDoubleAttribute(percentageMutual);
	_mutualMean = (float)_character->getDoubleAttribute(mutualMean);
	_mutualVariant = (float)_character->getDoubleAttribute(mutualVariant);
	_awayMean = (float)_character->getDoubleAttribute(awayMean);
	_awayVariant = (float)_character->getDoubleAttribute(awayVariant);
}

MeCtSaccade::BehaviorMode MeCtSaccade::getBehaviorMode()
{
	return _behaviorMode;
}
