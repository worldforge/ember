/*
 *  me_ct_IK_scenario.hpp - part of SmartBody-lib's Motion Engine
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
 *      Jingqiao Fu, USC
 */

#ifndef ME_CT_IK_SCENARIO_HPP
#define ME_CT_IK_SCENARIO_HPP


#include <controllers/me_controller.h>
#include "me_ct_locomotion_func.hpp"

#include <sr/sr_vec2.h>

#pragma once

#define JOINT_TYPE_UNKNOWN 0
#define JOINT_TYPE_HINGE 1
#define JOINT_TYPE_BALL 2
#define JOINT_TYPE_BALLSOCKET 3

/*#define ORIENTATION_RES_UNCHANGED_LOCAL 0
#define ORIENTATION_RES_PLANE 1
#define ORIENTATION_RES_UNCHANGED_WORLD 2*/

///////////////////////////////////////////////
// This is a generic IK solution for multi-joint adjustment
// Not much to say about the use, simply fill out the MeCtIKScenario structure, 
// call update(MeCtIKScenario* scenario) and pass in the structure. The returned data
// include joint quaternions, joint positions and global matrices

// more thorough definition of joint angle limits for IK solver
// joint rotation will be decomposed into swin-twist parameterization.
// joint angle limits are defined swing ( rotation about x-axis and y-axis ), and twist separately.
// Each SrVec2(limit_positive, limit_negative) define the joint rotation limits at counter-clockwise and clockwise directions.
struct MeCtIKJointLimit
{
	SrVec2 x_limit;
	SrVec2 y_limit;
	SrVec2 twist_limit;	
};


class MeCtIKScenarioJointInfo
{
public:
	int			type; // JOINT_TYPE_HINGE or JOINT_TYPE_BALL
	SkJoint*	sk_joint;

	int			is_support_joint; // 0: no; 1: yes
	float		support_joint_height; // How much distance off the ground can be considered touching the ground.
	float		support_joint_comp; // In case a compensation is needed to maintain a certain distance beyond support_joint_height.
	union
	{
		struct{float max; float twist_max; }ball;
		struct{float min; float max;}hinge;			
	}constraint; // the default set is ball joint

	MeCtIKJointLimit joint_limit;
	float            angular_speed_limit; // limit the bending speed of a joint, in degree/sec

	SrVec		axis; // for hinge joint, the rotation axis in its local coordinate.

public:
	MeCtIKScenarioJointInfo();
	~MeCtIKScenarioJointInfo();
};


class MeCtIKScenario{
public:
	// Public Constants
	static const char* TYPE;

public: // User provide data
	
	SrMat								gmat;				// global translation matrix of the start joint.
	MeCtIKScenarioJointInfo*			start_joint;		// The start joint that will be manipulated by IK, usually the root of a limb.
	MeCtIKScenarioJointInfo*			end_joint;			// The end joint that will be manipulated by IK, usually the tip of a limb.
	SrArray<MeCtIKScenarioJointInfo>	joint_info_list;	// Joint info, see above
	
	//definition for plane the limb should be adapted to.
	SrVec								plane_normal;		// The normal of plane on which the limb should be adapted to.
	SrVec								plane_point;		// A point of plane on which the limb should be adapted to.
	//definition for plane the limb should be adapted to.

	SrVec								ik_offset;			// offset from the current position to the position that is 
															// used to calculate the target position.
	SrVec								ik_orientation;		// Orientation of the limb, may not be in the direction of plane normal
	SrQuat                              ik_quat_orientation; // feng : world orientation ( a 3D rotation ) for the manipulated joint
	float								ik_compensate_factor; // Compensate for the off-ground height, usually cos(plane_normal, ik_orientation)

public:// User provide data & Returned data
	SrArray<SrQuat>						joint_quat_list;	// quaternions of joints

public:// Returned data
	SrArray<SrVec>						joint_pos_list;		// global position of joints
	SrArray<SrMat>						joint_global_mat_list; // global matrices of joints

public:
	MeCtIKScenario();
	~MeCtIKScenario();

public:
};

#endif // ME_CT_IK_SCENARIO_HPP
