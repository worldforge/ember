/*
 *  me_ct_basic_locomotion.cpp - part of Motion Engine and SmartBody-lib
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
 *      Yuyu Xu, ICT USC
 */

#include "controllers/me_ct_basic_locomotion.h"


std::string MeCtBasicLocomotion::_type_name = "BasicLocomotion";

MeCtBasicLocomotion::MeCtBasicLocomotion(SbmCharacter* c) :  SmartBody::SBController(), 
															 character(c)
{
	scootSpd = 0.0f;	//	unit: centermeter/sec
	movingSpd = 0.0f;	//	unit: centermeter/sec
	turningSpd = 0.0f;	//	unit: deg/sec
	_valid = false;
	_lastTime = -1.0;
}

MeCtBasicLocomotion::~MeCtBasicLocomotion()
{
}

void MeCtBasicLocomotion::init(SbmPawn* pawn)
{
	_lastTime = -1.0;
	MeController::init(pawn);
}

bool MeCtBasicLocomotion::controller_evaluate(double t, MeFrameData& frame)
{
	float dt = 1.0f / 60.0f;
	if (_lastTime > 0.0)
	{
		dt = float(t - _lastTime);
	}
	if (dt < .016f) // update no faster than 60 fps
		return true;
	_lastTime = t;
	if (character && _valid)
	{
		if (scootSpd == 0.0f && movingSpd == 0.0f && turningSpd == 0.0f)
			return true;
		float x, y, z, yaw, pitch, roll;
		character->get_world_offset(x, y, z, yaw, pitch, roll);

		//yaw += turningSpd * dt;
		yaw = getDesiredHeading();

		float movingDist = movingSpd * dt;
		x += movingDist * sin(yaw * (float)M_PI / 180.0f);
		z += movingDist * cos(yaw * (float)M_PI / 180.0f);
		//float scootDist = -scootSpd * dt;
		//x += scootDist * cos(yaw * (float)M_PI / 180.0f);
		//z += scootDist * sin(yaw * (float)M_PI / 180.0f);
		character->set_world_offset(x, y, z, yaw, pitch, roll);
	}
	return true;
}
