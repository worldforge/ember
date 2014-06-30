/*
 *  me_ct_locomotion_joint_info.hpp - part of SmartBody-lib's Motion Engine
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

#ifndef ME_CT_LOCOMOTION_JOINT_INFO_HPP
#define ME_CT_LOCOMOTION_JOINT_INFO_HPP

#include "sbm/lin_win.h"
#include <controllers/me_controller.h>

//#include "controllers/me_ct_locomotion_limb.hpp"

class MeCtLocomotionLimb;

#pragma once

struct MeCtLocomotionJointInfo
{
	int joint_num;
	bool joints_indexed;
	std::vector<std::string> joint_name;
	SrArray<int> buff_index;
	SrArray<int> joint_index;
	SrArray<int> quat_valid;
	SrArray<SrQuat> quat;
	SrArray<int> mat_valid;
	SrArray<SrMat>	mat;
	//SrArray<SrMat>	gmat;

	//key frames used to interpolate
	SrArray<SrQuat> quat_key_frame1;
	SrArray<SrQuat> quat_key_frame2;

	MeCtLocomotionJointInfo()
	{
		joints_indexed = false;
	}

	int iterate(SkJoint* joint, std::vector<std::string>* limb_joint_name)
	{
		int sum = 0;
		std::string name = joint->jointName();
		if(limb_joint_name != NULL)
		{
			for(size_t i = 0; i < limb_joint_name->size(); ++i)
			{
				if ((*limb_joint_name)[i] == name)
					return 0;
			}
		}
		if(joint->quat()->active() == true)
		{
			quat_valid.push() = 1;
		}
		else 
		{
			quat_valid.push() = 0;
		}

		joint_name.push_back(name);
		joint_index.push() = joint->index();
		sum = 1;


		for(int i = 0; i < joint->num_children(); ++i)
		{
			sum += iterate(joint->child(i), limb_joint_name);
		}
		return sum;
	}

	void Init(SkSkeleton* skeleton, std::string base_name, std::vector<std::string>* limb_joint_name)
	{
		joint_name.resize(0);
		joint_index.capacity(0);
		SkJoint* tjoint = skeleton->search_joint(base_name.c_str());
		joint_num = iterate(tjoint, limb_joint_name);
		quat.capacity(joint_num);
		quat.size(joint_num);
		mat.capacity(joint_num);
		mat.size(joint_num);
		mat_valid.size(joint_num);
		mat_valid.capacity(joint_num);
		quat_key_frame1.capacity(joint_num);
		quat_key_frame1.size(joint_num);
		quat_key_frame2.capacity(joint_num);
		quat_key_frame2.size(joint_num);
		buff_index.capacity(joint_num);
		buff_index.size(joint_num);
		joints_indexed = false;
	}

	int get_index_by_name(const std::string& name)
	{
		for(size_t i = 0; i < joint_name.size(); ++i)
		{
			if (joint_name[i] == name)
				 return i;
		}
		LOG("Error: joint: '%s' not found.", name.c_str());
		return -1;
	}
};


#endif // ME_CT_LOCOMOTION_JOINT_INFO_HPP
