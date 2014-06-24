/*
 *  me_ct_locomotion_func.cpp - part of SmartBody-lib's Motion Engine
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

#include "me_ct_locomotion_func.hpp"

//#include "components/sb/sb/sbm_character.hpp"
#include "components/sb/sbm/gwiz_math.h"
#include "components/sb/vhcl/vhcl_log.h"

SrMat get_gmat(SkJoint* from_joint, int depth, SrArray<SrQuat>& quat)
{
	SrMat lmat, gmat;
	SrQuat q;
	for(int i = 0; ; ++i)
	{
		q = quat.get(i);
		lmat = get_lmat(from_joint, &q);
		gmat = lmat*gmat;

		if(i == depth-1) break;
		from_joint = from_joint->child(0);
	}
	return gmat;
}

SrVec get_offset(SkJoint* from_joint, int depth, SrArray<SrQuat>& quat)
{
	SrVec offset;
	if(depth <= 0) return offset;
	SrMat lmat, gmat;
	SrQuat q;
	for(int i = 0; ; ++i)
	{
		if(i == depth-1) 
		{
			lmat.identity();
			lmat.set(12, from_joint->offset().x);
			lmat.set(13, from_joint->offset().y);
			lmat.set(14, from_joint->offset().z);
			gmat = lmat*gmat;
			break;
		}
		q = quat.get(i);
		lmat = get_lmat(from_joint, &q);
		gmat = lmat*gmat;

		from_joint = from_joint->child(0);
	}
	offset.set(gmat.get(12), gmat.get(13), gmat.get(14));
	return offset;
}

SrMat get_lmat(SkJoint* joint, SrQuat* quat)
{
	SrMat _lmat;
    SrQuat q = *quat;

      float x2  = q.x+q.x;
      float x2x = x2*q.x;
      float x2y = x2*q.y;
      float x2z = x2*q.z;
      float x2w = x2*q.w;
      float y2  = q.y+q.y;
      float y2y = y2*q.y;
      float y2z = y2*q.z;
      float y2w = y2*q.w;
      float z2  = q.z+q.z;
      float z2z = z2*q.z;
      float z2w = z2*q.w;

      _lmat[0] = 1.0f - y2y - z2z; _lmat[1] = x2y + z2w;        _lmat[2]  = x2z - y2w;
      _lmat[4] = x2y - z2w;        _lmat[5] = 1.0f - x2x - z2z; _lmat[6]  = y2z + x2w;
      _lmat[8] = x2z + y2w;        _lmat[9] = y2z - x2w;        _lmat[10] = 1.0f - x2x - y2y;

      if (_lmat[0]==0 && _lmat[1]==0 && _lmat[2]==0) _lmat.identity(); // to avoid a null matrix
   // now update offset + translation:
	//_lmat[12] = joint->offset().x;
	//_lmat[13] = joint->offset().y;
	//_lmat[14] = joint->offset().z;
	_lmat[12] = joint->offset().x + joint->pos()->value(0);
	_lmat[13] = joint->offset().y + joint->pos()->value(1);
	_lmat[14] = joint->offset().z + joint->pos()->value(2);
	//sr_out << "joint_name : " << joint->name() << "   joint_offset = " << joint->offset() << srnl;

   return _lmat;
 }


SrArray<SrQuat>* get_blended_quat_buffer(MeCtLocomotionJointInfo* joint_info, float weight)
{
	get_blended_quat_buffer(&(joint_info->quat), &(joint_info->quat_key_frame1), &(joint_info->quat_key_frame2), weight);
	return &(joint_info->quat);
}

SrArray<SrQuat>* get_blended_quat_buffer(SrArray<SrQuat>* dest, SrArray<SrQuat>* quat_buffer1, SrArray<SrQuat>* quat_buffer2, float weight)
{
	if(quat_buffer1->size() != quat_buffer2->size()) return NULL;
	if(dest == NULL)
	{
		LOG("Error. buffer size = 0");
	}
	SrQuat q;
	for(int i = 0; i < dest->size(); ++i)
	{
		q = slerp(quat_buffer1->get(i), quat_buffer2->get(i), 1.0f-weight);
		dest->set(i, q);
	}
	return dest;
}

int iterate_set(SkJoint* base, int index, int depth, SrArray<SrQuat>* buff, SrArray<int>* index_buff)
{
	if(index >= depth && depth > 0) return index;
	if(base->quat()->active())
	{
		if( base->index() == index_buff->get(index))
		{
			SrQuat q = base->quat()->value();
			buff->set(index, q);
		}
		else
			--index;
	}

	for(int i = 0; i < base->num_children(); ++i)
	{
		index = iterate_set(base->child(i), index+1, depth, buff, index_buff);
	}
	return index;
}

void get_frame(SkMotion* walking, SkSkeleton* walking_skeleton, float frame, std::string limb_base, MeCtLocomotionJointInfo* joint_info)
{
	get_frame(walking, walking_skeleton, frame, limb_base, &(joint_info->quat), &(joint_info->quat_key_frame1), &(joint_info->quat_key_frame2), &(joint_info->joint_index));
}

void get_frame(SkMotion* walking, SkSkeleton* walking_skeleton, float frame, std::string limb_base, SrArray<SrQuat>* quat_buffer, SrArray<SrQuat>* quat_buffer1, SrArray<SrQuat>* quat_buffer2, SrArray<int>* index_buff)
{
	SkJoint* base = walking_skeleton->search_joint(limb_base.c_str());

	int frame1 = (int)frame;
	int frame2 = ((int)frame+1);
	frame2 = (frame2 >= walking->frames()? 0: frame2);

	walking->apply_frame(frame1);
	iterate_set(base, 0, quat_buffer->size(), quat_buffer1, index_buff);
	walking->apply_frame(frame2);
	iterate_set(base, 0, quat_buffer->size(), quat_buffer2, index_buff);
	SrQuat q;
	for(int i = 0; i < quat_buffer->size(); ++i)
	{
		q = slerp(quat_buffer1->get(i), quat_buffer2->get(i), frame-(int)frame);
		quat_buffer->set(i,q);
	}
}