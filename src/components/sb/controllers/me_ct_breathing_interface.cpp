/**
* \file me_ct_breathing_interface.cpp
* \brief The breathing controller interface and breath cycle classes
*
* Part of Motion Engine and SmartBody-lib.
* Copyright (C) 2008  University of Southern California
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
*   - Celso M. de Melo, demelo@usc.edu, USC
*
* \author University of Southern California
* \author Institute for Creative Technologies, USC
*/


# include <controllers/me_ct_breathing_interface.h>

NumericalInterpolationKey::NumericalInterpolationKey(float startValue, float endValue)
{
	_startValue = startValue;
	_endValue = endValue;
	_startTime = -1;
	_endTime = -1;
	_value = startValue;
	_finished = false;
}
void NumericalInterpolationKey::update(float time)
{
	float t = (time - _startTime) / (_endTime - _startTime);

	if(t > 1.0f)
	{
		_value = _endValue;
		_finished = true;
	}
	else
		_value = _startValue + t * (_endValue - _startValue);
}


BreathCycle::BreathCycle()
{
	_type = "Breath";
	_is_inspiring = true;
}
MinMaxBreathCycle::MinMaxBreathCycle()
{
	_type = "MinMaxBreathCycle";

	_min = 0.0;
	_max = 1.0;
	_min_key = NULL;
	_max_key = NULL;
}
MinMaxBreathCycle::MinMaxBreathCycle(float min, float max)
{
	_type = "MinMaxBreathCycle";

	_min = min;
	_max = max;
	_min_key = NULL;
	_max_key = NULL;
}
void MinMaxBreathCycle::min(float value, float transitionDuration)
{
	_min_key = new NumericalInterpolationKey(_min, value);
	_min_key->setEndTime(transitionDuration);
}
void MinMaxBreathCycle::max(float value, float transitionDuration)
{
	_max_key = new NumericalInterpolationKey(_max, value);
	_max_key->setEndTime(transitionDuration);
}
void MinMaxBreathCycle::updateInterpolationKeys(double realTime)
{
	if(_max_key != NULL)
	{
		if(_max_key->getStartTime() == -1)
		{
			_max_key->setStartTime(float(realTime));
			_max_key->setEndTime(_max_key->getEndTime() + float(realTime));
		}
		_max_key->update(float(realTime));
		max(_max_key->getValue());
		if(_max_key->isFinished())
		{
			delete _max_key;
			_max_key = NULL;
		}
	}
	if(_min_key != NULL)
	{
		if(_min_key->getStartTime() == -1)
		{
			_min_key->setStartTime(float(realTime));
			_min_key->setEndTime(_min_key->getEndTime() + float(realTime));
		}
		_min_key->update(float(realTime));
		min(_min_key->getValue());
		if(_min_key->isFinished())
		{
			delete _min_key;
			_min_key = NULL;
		}
	}
}
LinearBreathCycle::LinearBreathCycle()
{
	_type = "LinearBreathCycle";
}
LinearBreathCycle::LinearBreathCycle(float min, float max)
:MinMaxBreathCycle(min, max)
{
	_type = "LinearBreathCycle";
}
float LinearBreathCycle::value(double realTime, double cycleTime)
{
	updateInterpolationKeys(realTime);

	if(cycleTime < 1.0f)
	{
		_is_inspiring = true; 
		return _min + float(cycleTime)*(_max-_min);
	}
	else 
	{
		_is_inspiring = false;
		return _max - (float(cycleTime)-1.0f)*(_max-_min);
	}
}
SineBreathCycle::SineBreathCycle()
{
	_type = "SineBreathCycle";
}
SineBreathCycle::SineBreathCycle(float min, float max)
:MinMaxBreathCycle(min, max)
{
	_type = "SineBreathCycle";
}
float SineBreathCycle::value(double realTime, double cycleTime)
{
	updateInterpolationKeys(realTime);

	_is_inspiring = cycleTime < (M_PI/2);

	return _min + float(sin(cycleTime)) * (_max-_min);
}
KeyframeBreathCycle* KeyframeBreathCycle::custom(float breathMin, float breathMax, 
		float breathStartTime, float inspirationStartTime, float inspirationEndTime, 
		float expirationStartTime, float expirationEndTime, float breathEndTime)
{
	KeyframeBreathCycle* breath_cycle = new KeyframeBreathCycle();

	breath_cycle->keyframes.push_back(new Keyframe(breathMin, breathStartTime));
	if(inspirationStartTime != breathStartTime)
		breath_cycle->keyframes.push_back(new Keyframe(breathMin, inspirationStartTime));
	breath_cycle->keyframes.push_back(new Keyframe(breathMax, inspirationEndTime));
	if(expirationStartTime != inspirationEndTime)
		breath_cycle->keyframes.push_back(new Keyframe(breathMax, expirationStartTime));
	breath_cycle->keyframes.push_back(new Keyframe(breathMin, expirationEndTime));
	if(breathEndTime != expirationEndTime)
		breath_cycle->keyframes.push_back(new Keyframe(breathMin, breathEndTime));
	breath_cycle->update();

	return breath_cycle;
}
KeyframeBreathCycle::KeyframeBreathCycle()
{
	_type = "KeyframeBreathCycle";
	_max_time = -1;
	_last_start_keyframe = 0;
}
KeyframeBreathCycle::~KeyframeBreathCycle()
{
	vector<Keyframe*>::iterator it;
	for(it = keyframes.begin(); it != keyframes.end(); it++)
		delete *it;
}
bool compare_keyframes(KeyframeBreathCycle::Keyframe* k1, KeyframeBreathCycle::Keyframe* k2)
{
	return (k1->time < k2->time);
}
void KeyframeBreathCycle::update()
{
	std::sort(keyframes.begin(), keyframes.end(), compare_keyframes);

	_max_time = -1;
	float max = -1;
	vector<Keyframe*>::iterator it;
	for(it = keyframes.begin(); it!=keyframes.end(); it++)
	{
		if((*it)->value > max)
		{
			_max_time = (*it)->time;
			max = (*it)->value;
		}
	}
}
float KeyframeBreathCycle::duration()
{
	return keyframes[keyframes.size()-1]->time;
}
float KeyframeBreathCycle::value(double realTime, double cycleTime)
{
	_is_inspiring = cycleTime < _max_time;

	int start_keyframe = 0;
	int end_keyframe;
	if(cycleTime > keyframes[_last_start_keyframe]->time)
		start_keyframe = _last_start_keyframe;
	for(unsigned int i=start_keyframe; i<keyframes.size(); i++)
	{
		if(cycleTime == keyframes[i]->time)
			return keyframes[i]->value;
		if(cycleTime > keyframes[i]->time)
			start_keyframe = i;
		else
		{
			end_keyframe = i;
			break;
		}
	}
	_last_start_keyframe = start_keyframe;

	double t = (cycleTime - keyframes[start_keyframe]->time) / (keyframes[end_keyframe]->time - keyframes[start_keyframe]->time);
	return float(keyframes[start_keyframe]->value + t * (keyframes[end_keyframe]->value - keyframes[start_keyframe]->value));
}
SplineBreathCycle::SplineBreathCycle(MeSpline1D* spline)
{
	_type = "SplineBreathCycle";
	_max_time = -1;
	_spline = spline;

	update();
}
SplineBreathCycle::~SplineBreathCycle()
{
	delete _spline;
}
void SplineBreathCycle::update()
{
#ifndef __ANDROID__
	MeSpline1D::Knot* knot = _spline->knot_first();
	float max = -1;
	do
	{
		if(knot->get_y() > max)
		{
			max = float(knot->get_y());
			_max_time = float(knot->get_x());
		}
		knot = knot->get_next();
	}
	while(knot != NULL);
#endif
}
float SplineBreathCycle::duration()
{
#ifdef __ANDROID__
	return 1.f;
#else
	return float(_spline->knot_last()->get_x()-_spline->knot_first()->get_x());
#endif
}
float SplineBreathCycle::value(double realTime, double cycleTime)
{
#ifdef __ANDROID__
	return 1.f;
#else
	_is_inspiring = cycleTime < _max_time;
	return float(_spline->eval(cycleTime));
#endif
}