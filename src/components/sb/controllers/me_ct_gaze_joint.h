/*
 *  me_ct_gaze_joint.h - part of SmartBody-lib
 *  Copyright (C) 2008  University of Southern California
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
 *      Marcus Thiebaux, USC
 */

#ifndef ME_CT_GAZE_JOINT_H
#define ME_CT_GAZE_JOINT_H

#include <sk/sk_skeleton.h>
#include <controllers/me_controller.h>

#include "sbm/gwiz_math.h"

#ifndef TRUE
#define TRUE 1
#endif
#ifndef FALSE
#define FALSE 0
#endif

///////////////////////////////////////////////////////////////////////////

class MeCtGazeJoint	{

	public:
		MeCtGazeJoint();
		virtual ~MeCtGazeJoint() {}
		
		int id;
		int active;
		
		float 	limit_p_up, limit_p_dn, limit_h, limit_r;
		float 	task_weight;
		float 	blend_weight;
		float 	speed_ratio;
		float 	speed;
		float 	duration;
		float 	smooth;
		
		void init( SkJoint* j_p );
		void start( void );
		
		gwiz::vector_t	forward_pos;
		gwiz::vector_t	forward_ref; // default forward direction
		gwiz::quat_t		forward_rot;
		
		gwiz::vector_t	local_pos;
		gwiz::quat_t	local_rot;
		gwiz::quat_t	prev_local_rot;

		gwiz::vector_t  world_zero_pos;
		gwiz::quat_t    world_zero_rot;

		gwiz::vector_t  parent_zero_pos;
		gwiz::quat_t    parent_zero_rot;

		gwiz::quat_t    parent_diff_rot;
		
		gwiz::vector_t	world_pos;
		gwiz::quat_t	world_rot;
		gwiz::quat_t	prev_world_rot; // to measure absolute speed of head
		float t_elapse;
	
		gwiz::vector_t	parent_loc_pos; // local coord of immediate parent joint
		gwiz::quat_t		parent_loc_rot;

		gwiz::vector_t	parent_pos; // world coord of immediate parent joint
		gwiz::quat_t		parent_rot;

		gwiz::quat_t	evaluate( float dt, gwiz::vector_t target_pos, gwiz::quat_t off_rot, float scale_factor = 1.0 );
		gwiz::quat_t	evaluate( float dt, gwiz::quat_t target_rot, gwiz::quat_t off_rot, float scale_factor = 1.0 );
		
	private:
		void	capture_joint_state( void );
		gwiz::quat_t	rotation_to_target( gwiz::vector_t target_pos );
		gwiz::quat_t	rotation_to_target( gwiz::quat_t target_rot );
		
//		gwiz::quat_t	constrain_box( gwiz::quat_t task_rot );
//		gwiz::quat_t	constrain_swing( gwiz::quat_t task_rot );
//		gwiz::quat_t	constrain_quat( gwiz::quat_t task_rot );
		gwiz::quat_t	constrain_ellipse( gwiz::quat_t task_rot );
		
		gwiz::quat_t	constrain_quat_speed( float dt, gwiz::quat_t task_rot );
		gwiz::quat_t	constrain_smooth( float dt, gwiz::quat_t task_rot );

		gwiz::quat_t	constrain( float dt, gwiz::quat_t task_rot );

		SkJoint* joint_p;
};

///////////////////////////////////////////////////////////////////////////
#endif
