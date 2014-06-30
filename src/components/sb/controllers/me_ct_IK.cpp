/*
 *  me_ct_IK.cpp - part of SmartBody-lib's Motion Engine
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

#include "controllers/me_ct_IK.hpp"

#include "sb/sbm_character.hpp"
#include "sbm/gwiz_math.h"


const char* MeCtIK::TYPE = "MeCtIK";

/** Constructor */
MeCtIK::MeCtIK() {

	max_iteration = 20;
	threshold = 0.01f;
	recrod_endmat = 0;
}

/** Destructor */
MeCtIK::~MeCtIK() {
	// Nothing allocated to the heap
}

void MeCtIK::set_max_iteration(int iter)
{
	max_iteration = iter;
}

void MeCtIK::update(MeCtIKScenario* scenario)
{
	this->scenario = scenario;
	int modified = 0;
	SrMat inv_end;

	SrQuat quat;
	SrMat mat;

	/*for(int i = 0; i < scenario->joint_info_list.size(); ++i)
	{
		if(!scenario->joint_info_list.get(i).is_support_joint && scenario->joint_info_list.get(i).type == JOINT_TYPE_HINGE)
		{
			quat = scenario->joint_quat_list.get(i);
			if(quat.angle() <= 0.03f)
			{
				mat.rot(scenario->joint_quat_list.get(i).axis(), 0.03f-quat.angle());
				quat = mat * quat;
				scenario->joint_quat_list.set(i, quat);
			}
		}
	}*/

	/*SrQuat before;
	for(int i = 0; i < scenario->joint_info_list.size(); ++i)
	{
		if(scenario->joint_info_list.get(i).is_support_joint)
		{
			before = scenario->joint_quat_list.get(i);
			break;
		}
	}*/

	init();

	unstretch_joints();

	
	for(int k = 0; k < support_joint_num; ++k)
	{
		if(manipulated_joint_index - start_joint_index == 1) adjust_2_joints();
		else adjust();

		//handles the support joints
		SrQuat before = scenario->joint_quat_list.get(manipulated_joint_index);
		//temp

		pm = scenario->joint_global_mat_list.get(manipulated_joint_index);
		pm.set(12, 0.0f);//??
		pm.set(13, 0.0f);
		pm.set(14, 0.0f);

		lm = joint_init_mat_list.get(manipulated_joint_index);
		lm.set(12, 0.0f);//??
		lm.set(13, 0.0f);
		lm.set(14, 0.0f);

		inv_end = lm*pm.inverse();
		before = inv_end * before;
		SrQuat after = before;
		//temp

		modified = check_constraint(&after, manipulated_joint_index);
		scenario->joint_quat_list.set(manipulated_joint_index, before);
		if(modified == 0) 
		{
			start_joint_index = manipulated_joint_index;
			get_next_support_joint();
			get_limb_section_local_pos(start_joint_index, -1);
			calc_target(scenario->ik_orientation, scenario->ik_offset);
		}
		else 
		{
			recrod_endmat = 0;
			get_next_support_joint();
			get_limb_section_local_pos(0, -1);
			scenario->joint_quat_list.set(manipulated_joint_index-1, after);
			calc_target(scenario->ik_orientation, scenario->ik_offset);
		}
	}
	
}

SBM_INLINE void MeCtIK::unstretch_joints()
{
	SrVec& pivot = scenario->joint_pos_list.get(0);
	SrVec& i_target = target.get(manipulated_joint_index);
	SrVec& i_src = scenario->joint_pos_list.get(manipulated_joint_index);

	SrVec v1, v2;
	SrQuat quat;
	SrMat mat;
	v1 = pivot - i_src;
	v2 = i_target - i_src;
	if(dot(v1, v2) < 0.0f) return;
	int i = 0;
	for(i = 0; i < scenario->joint_info_list.size(); ++i)
	{
		quat = scenario->joint_quat_list.get(i);
		if(!scenario->joint_info_list.get(i).is_support_joint && scenario->joint_info_list.get(i).type == JOINT_TYPE_HINGE )
		{
			mat.rot(scenario->joint_quat_list.get(i).axis(), 3.14159265f * v2.len() / v1.len());
			quat = mat * quat;
			scenario->joint_quat_list.set(i, quat);
			break;
		}
	}
	if(i < scenario->joint_info_list.size()) get_limb_section_local_pos(i, -1);
}

// fast solution for 2 joints
SBM_INLINE void MeCtIK::adjust_2_joints()
{
	rotate(scenario->joint_pos_list.get(manipulated_joint_index), start_joint_index);
}

SBM_INLINE void MeCtIK::adjust()
{
	int reach = 0;
	int i,j;
	for(i = 0; i < max_iteration; ++i)
	{
		for(j = start_joint_index; j != manipulated_joint_index; ++j)
		{
			/*if(reach_destination()) 
			{
				reach = 1;
				break;
			}*/
			rotate(scenario->joint_pos_list.get(manipulated_joint_index), j);
		}
		if(reach) break;
	}
}

int MeCtIK::get_support_joint_num()
{
	int num = 0;
	for(int i = 0; i < scenario->joint_info_list.size(); ++i)
	{
		if(scenario->joint_info_list.get(i).is_support_joint) ++num;
	}
	return num;
}

SBM_INLINE int MeCtIK::check_constraint(SrQuat* quat, int index)
{
	return 0;
	float angle = quat->angle();
	int modified = 0;
	MeCtIKScenarioJointInfo* info = &(scenario->joint_info_list.get(index));
	if(info->type == JOINT_TYPE_HINGE)
	{
		if(info->constraint.hinge.max > 0.0f && angle > info->constraint.hinge.max) 
		{
			angle = info->constraint.hinge.max;
			modified = 1;
		}
		else if(info->constraint.hinge.min < 0.0f && angle < info->constraint.hinge.min) 
		{
			angle = info->constraint.hinge.min;
			modified = 1;
		}
	}
	else if(info->type == JOINT_TYPE_BALL && info->constraint.ball.max > 0.0f)
	{
		if(angle > info->constraint.ball.max) 
		{
			angle = info->constraint.ball.max;
			modified = 1;
		}
		else if(angle < -info->constraint.ball.max) 
		{
			angle = -info->constraint.ball.max;
			modified = 1;
		}
	}
	if(modified) quat->set(quat->axis(), angle);
	return modified;
}

SBM_INLINE int MeCtIK::reach_destination()
{
	SrVec v = scenario->joint_pos_list.get(manipulated_joint_index) - target.get(manipulated_joint_index);
	if(v.len() < threshold) return 1;
	return 0;
}

SBM_INLINE float MeCtIK::distance_to_plane(SrVec& point, SrVec& plane_normal, SrVec& plane_point)
{
	SrVec v;
	plane_normal.normalize();
	v = point - plane_point;
	float dis = dot(plane_normal, v);
	return dis;
}

SrVec MeCtIK::cross_point_on_plane(SrVec& point, SrVec& line, SrVec& plane_normal, SrVec& plane_point)
{
	float dis = distance_to_plane(point, plane_normal, plane_point);
	float cos = dot(line, plane_normal)/plane_normal.len();
	return (point - line*dis/cos);
}

SBM_INLINE SrVec MeCtIK::upright_point_to_plane(SrVec& point, SrVec& plane_normal, SrVec& plane_point)
{
	float dis = distance_to_plane(point, plane_normal, plane_point);
	return (point - plane_normal*dis);
}

SBM_INLINE bool MeCtIK::cross_point_with_plane(SrVec* cross_point, SrVec& line_point, SrVec& direction, SrVec& plane_normal, SrVec& plane_point)
{
	float angle = dot(direction, plane_normal);
	if(angle == 0.0f) return false;
	SrVec v;
	v = plane_point - line_point;
	v = direction * dot(v, plane_normal)/angle;
	*cross_point = v + line_point;
	return true;
}

SBM_INLINE void MeCtIK::update_limb_section_local_pos(int start_index)
{
	//temp solution, for efficiency, this must be replaced with optimized methods.
	get_limb_section_local_pos(start_index, -1);
	//temp solution, for efficiency, this must be replaced with optimized methods.
}

//The rotate function basicaly puts the joint back to its local coordinate
SBM_INLINE void MeCtIK::rotate(SrVec& src, int start_index)
{
	SrVec v1, v2, v3, v4;
	SrVec v, i_target, i_src;
	//SrVec axis, r_axis;
	SrVec r_axis;
	SrQuat q;
	SrMat mat, mat_inv;
	if(start_index == 0) mat_inv = scenario->gmat.inverse();
	else mat_inv = scenario->joint_global_mat_list.get(start_index-1).inverse();
	SrVec& pivot = scenario->joint_pos_list.get(start_index);
	pivot = pivot * mat_inv; // tranverse joint back to its local coordinate
	i_target = target.get(manipulated_joint_index) * mat_inv;
	i_src = src * mat_inv;

	//
	v1 = i_src - pivot;
	v1.normalize();


	if(scenario->joint_info_list.get(start_index).type == JOINT_TYPE_BALL)
	{
		v2 = i_target - pivot;
		v2.normalize();

		double dot_v = dot(v1, v2);
		if(dot_v >= 0.9999995000000f) 
		{
			if(start_index >= scenario->joint_pos_list.size()-1) return;
			dot_v = 1.0f;
			v3 = scenario->joint_pos_list.get(start_index+1);
			v3 = v3 * mat_inv; // tranverse joint back to its local coordinate
			v2 = v3 - pivot;
		}
		else if(dot_v < -1.0f) dot_v = -1.0f;
		double angle = acos(dot_v);

		r_axis = cross(v2, v1);
		r_axis.normalize();

		mat.rot(r_axis, (float)-angle);
	}
	else if(scenario->joint_info_list.get(start_index).type == JOINT_TYPE_HINGE)
	{
		r_axis = scenario->joint_info_list.get(start_index).axis;
		v = upright_point_to_plane(i_target, r_axis, i_src);
		v2 = v - pivot;
		v2.normalize();
		
		double dot_v = dot(v1, v2);
		if(dot_v > 1.0f) dot_v = 1.0f;
		else if(dot_v < -1.0f) dot_v = -1.0f;
		double angle = acos(dot_v);

		v3 = cross(v1, v2);
		v3.normalize();
		if(dot(v3, r_axis) > 0.0f) mat.rot(r_axis, (float)angle);
		else mat.rot(r_axis, (float)-angle);
	}
	
	
	q = scenario->joint_quat_list.get(start_index);
	q = mat * q;

	check_constraint(&q, start_index);

	scenario->joint_quat_list.set(start_index, q);
	get_limb_section_local_pos(start_index, -1);
}

/*inline void MeCtIK::rotate(SrVec& src, int start_index)
{
	SrVec v1, v2, v3, v4;
	SrVec v, i_target, i_src;
	//SrVec axis, r_axis;
	SrVec r_axis;
	SrQuat q;
	SrMat mat, mat_inv;
	if(start_index == 0) mat_inv = scenario->gmat.inverse();
	else mat_inv = scenario->joint_global_mat_list.get(start_index-1).inverse();
	SrVec& pivot = scenario->joint_pos_list.get(start_index);
	pivot = pivot * mat_inv; // tranverse joint back to its local coordinate
	i_target = target.get(manipulated_joint_index) * mat_inv;
	i_src = src * mat_inv;

	//
	v1 = i_src - pivot;
	v1.normalize();
	if(scenario->joint_info_list.get(start_index).type == JOINT_TYPE_BALL)
	{
		//compute the axis
		//v4 = i_target - i_src;
		//v4.normalize();
		
		//
		v2 = i_target - pivot;
		v2.normalize();

		//v3 = v4*v1.len()/2.0f+i_src;
		
		r_axis = cross(v2, v1);
		r_axis.normalize();
		//r_axis = axis;
	}
	else if(scenario->joint_info_list.get(start_index).type == JOINT_TYPE_HINGE)
	{
		r_axis = scenario->joint_info_list.get(start_index).axis;
		v = upright_point_to_plane(i_target, r_axis, i_src);
		v2 = v - pivot;
		v2.normalize();
	}
	
	double dot_v = dot(v1, v2);
	if(dot_v > 1.0f) dot_v = 1.0f;
	double angle = acos(dot_v);

	//if(dot_v > 0.995f) angle /= 2.0f;

	v3 = cross(v1, v2);
	v3.normalize();
	if(dot(v3, r_axis) > 0.0f) mat.rot(r_axis, (float)angle);
	else mat.rot(r_axis, (float)-angle);
	
	q = scenario->joint_quat_list.get(start_index);
	q = mat * q;

	check_constraint(&q, start_index);

	scenario->joint_quat_list.set(start_index, q);
	get_limb_section_local_pos(start_index, -1);
}*/

SBM_INLINE void MeCtIK::calc_target(SrVec& orientation, SrVec& offset)
{	
	orientation.normalize();
	SrVec pos = scenario->joint_pos_list.get(manipulated_joint_index);
	pos += offset;
	SrVec t = cross_point_on_plane(pos, orientation, scenario->plane_normal, scenario->plane_point);
	target.set(manipulated_joint_index, -(manipulated_joint->support_joint_comp + manipulated_joint->support_joint_height)*orientation + t);
}

SBM_INLINE void MeCtIK::get_next_support_joint()
{
	int i;
	for(i = manipulated_joint_index+1; i < scenario->joint_info_list.size(); ++i)
	{
		if(scenario->joint_info_list.get(i).is_support_joint)
		{
			manipulated_joint_index = i;
			manipulated_joint = &(scenario->joint_info_list.get(manipulated_joint_index));
			break;
		}
	}

	if(manipulated_joint == NULL)// in case the user sets no support joint. set the manipulated joint the last joint
	{
		manipulated_joint_index = scenario->joint_info_list.size()-1;
		manipulated_joint = &(scenario->joint_info_list.get(manipulated_joint_index));
	}
}

void MeCtIK::init()
{
	manipulated_joint = NULL;
	recrod_endmat = 0;
	end_mat.identity();

	int size = scenario->joint_quat_list.size();

	target.capacity(size);
	target.size(size);

	// feng : add orientation target
	target_orientation.capacity(size);
	target_orientation.size(size);

	scenario->joint_pos_list.capacity(size);
	scenario->joint_pos_list.size(size);
	scenario->joint_global_mat_list.capacity(size);
	scenario->joint_global_mat_list.size(size);
	joint_init_mat_list.capacity(size);
	joint_init_mat_list.size(size);

	get_limb_section_local_pos(0, -1);

	get_init_mat_list();

	scenario->plane_normal.normalize();
	manipulated_joint_index = -1;
	support_joint_num = get_support_joint_num();
	get_next_support_joint();

	start_joint_index = 0;
	end_joint_index = support_joint_num;

	calc_target(scenario->ik_orientation, scenario->ik_offset);
}

SBM_INLINE void MeCtIK::get_init_mat_list()
{
	joint_init_mat_list = scenario->joint_global_mat_list;
}

SBM_INLINE void MeCtIK::update_manipulated_joint_pos(int index)
{
	scenario->joint_pos_list.set(manipulated_joint_index, scenario->joint_global_mat_list.get(index) * scenario->joint_pos_list.get(manipulated_joint_index));
}

void MeCtIK::get_limb_section_local_pos(int start_index, int end_index)
{
	SrMat gmat;
	SrMat pmat;
	SrMat lmat;
	SrVec axis;
	SrVec ppos;
	SkJoint* tjoint = scenario->start_joint->sk_joint;
	
	
	for(int i = 0; i < start_index; ++i)
	{
		tjoint = tjoint->child(0);
	}
	
	//tjoint = scenario->joint_info_list[start_index].sk_joint;

	if(end_index < 0 || end_index > scenario->joint_info_list.size()-1) end_index = scenario->joint_info_list.size()-1;

	for(int j  = start_index; j <= end_index; ++j)
	{
		//scenario->joint_info_list.get(j).index = j;
		if(j == 0) pmat = scenario->gmat;
		else pmat = scenario->joint_global_mat_list.get(j-1);

		lmat = get_lmat(tjoint, &(scenario->joint_quat_list.get(j)));
		//joint_local_mat_list.set(j, lmat);

		gmat = lmat * pmat;
		scenario->joint_global_mat_list.set(j, gmat);

		ppos.set(*gmat.pt(12), *gmat.pt(13), *gmat.pt(14));
		scenario->joint_pos_list.set(j, ppos);

		if(scenario->joint_info_list.get(j).type == JOINT_TYPE_BALL)
		{
			//do nothing
		}
		else if(scenario->joint_info_list.get(j).type == JOINT_TYPE_HINGE)
		{
			// do nothing
		}

		if(tjoint->num_children()>0 && tjoint != scenario->end_joint->sk_joint)
		{
			tjoint = tjoint->child(0);
			//tjoint = scenario->joint_info_list[j].sk_joint;
		}
		else break;
	}
}

SrVec MeCtIK::get_target(int index)
{
	return target.get(index);
}
