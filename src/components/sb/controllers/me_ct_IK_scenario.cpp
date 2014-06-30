/*
 *  me_ct_IK_scenario.cpp - part of SmartBody-lib's Motion Engine
 *  Copyright (C) 2009  University of Southern California
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
 *      Jingqiao Fu
 */

#include "me_ct_IK_scenario.hpp"

#include "sb/sbm_character.hpp"
#include "sbm/gwiz_math.h"


const char* MeCtIKScenario::TYPE = "MeCtIKScenario";


/** Constructor */
MeCtIKScenario::MeCtIKScenario() {
	ik_orientation.set(0,0,0);
	ik_compensate_factor = 1.0f;	
}

/** Destructor */
MeCtIKScenario::~MeCtIKScenario() {
	// Nothing allocated to the heap
}

MeCtIKScenarioJointInfo::MeCtIKScenarioJointInfo()
{
	type = JOINT_TYPE_UNKNOWN; // JOINT_TYPE_HINGE or JOINT_TYPE_BALL
	//index = 0;
	sk_joint = NULL;
	is_support_joint = 0;
	support_joint_height = 0.0f;
	support_joint_comp = 0.0f;
	angular_speed_limit = 120.f;
	memset(&constraint, 0, sizeof(constraint));
	axis.set(0,0,0);

}

MeCtIKScenarioJointInfo::~MeCtIKScenarioJointInfo()
{

}
