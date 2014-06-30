 /*
 *  me_ct_IK.hpp - part of SmartBody-lib's Motion Engine
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

#ifndef ME_CT_IK_HPP
#define ME_CT_IK_HPP


#include <controllers/me_controller.h>
#include "controllers/me_ct_IK_scenario.hpp"
#include <sb/SBTypes.h>

#pragma once

#if defined (__ANDROID__) || defined(SB_IPHONE)
#define SBM_INLINE
#else
#define SBM_INLINE inline
#endif


///////////////////////////////////////////////
// This is a generic IK solution for multi-joint adjustment
// Not much to say about the use, simply fill out the MeCtIKScenario structure, 
// call update(MeCtIKScenario* scenario) and pass in the structure. The returned data
// include joint quaternions, joint positions and global matrices
 
class MeCtIK{
public:
	// Public Constants
	static const char* TYPE;

protected:

	MeCtIKScenario*				scenario;
	SkJoint*					base_joint;
	int							max_iteration;
	
	SrArray<SrMat>				joint_init_mat_list;

	SrMat						end_mat;
	
	SrArray<SrVec>				target;
	SrArray<SrQuat>             target_orientation; // feng : add orientation target
	float						threshold;

	MeCtIKScenarioJointInfo*	manipulated_joint;
	int							start_joint_index;
	int							end_joint_index;
	int							manipulated_joint_index;
	int							support_joint_num;

	//temp
	int							recrod_endmat;
	SrMat						lm;
	SrMat						pm;

	

public:
	MeCtIK();
	~MeCtIK();

public:
	virtual void update(MeCtIKScenario* scenario);
	SrVec get_target(int index);

protected:
	virtual void adjust();			

	virtual void calc_target(SrVec& orientation, SrVec& offset);

	void init();

	int check_constraint(SrQuat* quat, int index);	

	void rotate(SrVec& src, int start_index);

	void adjust_3_joints();

	void adjust_2_joints();

	void set_max_iteration(int iter);

	SrVec upright_point_to_plane(SrVec& point, SrVec& plane_normal, SrVec& plane_point);

	float distance_to_plane(SrVec& point, SrVec& plane_normal, SrVec& plane_point);

	bool cross_point_with_plane(SrVec* cross_point, SrVec& line_point, SrVec& direction, SrVec& plane_normal, SrVec& plane_point);

	SrVec cross_point_on_plane(SrVec& point, SrVec& line, SrVec& plane_normal, SrVec& plane_point);

	void update_limb_section_local_pos(int start_index);	

	void get_limb_section_local_pos(int start_index, int end_index);	

	int reach_destination();	

	void get_next_support_joint();

	int get_support_joint_num();	

	void get_init_mat_list();

	void update_manipulated_joint_pos(int index);

	void unstretch_joints();

};

#endif // ME_CT_IK_HPP
