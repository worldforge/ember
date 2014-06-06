/*
 *  me_ct_gaze_joint.cpp - part of SmartBody-lib
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

#include "controllers/me_ct_gaze_joint.h"
#include "controllers/me_ct_gaze_alg.h"
using namespace gwiz;
#include <vhcl_log.h>

#include <sr/sr_alg.h>

#define ENABLE_FORWARD_RAY  		1
#define USE_OLD_GAZE 0

///////////////////////////////////////////////////////////////////////////

MeCtGazeJoint::MeCtGazeJoint( void )	{
	
	id = -1;
	active = 0;
//	limit_rot = euler_t( 90.0, 180.0, 180.0 );
	limit_p_up = 90.0;
	limit_p_dn = 90.0;
	limit_h = 180.0;
	limit_r = 180.0;
	task_weight = 1.0;
	blend_weight = 1.0;
	speed = 1.0;
	duration = 1.0;
	smooth = 0.0;
	
	forward_pos = vector_t();
	forward_ref = vector_t( 0.0, 0.0, 1.0 );
	forward_rot = quat_t();
//	forward_rot = euler_t( 45.0, 0.0, 0.0 );
//	forward_rot = euler_t( 0.0, 45.0, 0.0 );
//	forward_rot = euler_t( 0.0, 0.0, 45.0 );
	
	joint_p = NULL;
}

void MeCtGazeJoint::init( SkJoint* j_p )	{
	
	joint_p = j_p;
}

void MeCtGazeJoint::start( void )	{

	capture_joint_state();
	prev_local_rot = local_rot;
	prev_world_rot = world_rot;
	t_elapse = 0.0;
}

#define ENABLE_CONSTRAIN	1

quat_t MeCtGazeJoint::evaluate( float dt, vector_t target_pos, quat_t off_rot, float scale_factor )	{
	
	prev_world_rot = world_rot;
	capture_joint_state();
	quat_t Q_task = ( rotation_to_target( target_pos ) * off_rot ) * scale_factor;	
	//Q_task.print();
#if ENABLE_CONSTRAIN
	quat_t Q = constrain( dt, Q_task );	
#else
	quat_t Q = Q_task;
#endif
	prev_local_rot = Q;
	// this part of the code should not affect the old skeleton, since their (-world_zero_rot) should all be zero rotations. 
	vector_t newAxis = (-world_zero_rot)*Q.axis();
	quat_t Qout = quat_t(Q.degrees(),newAxis);
	return( Qout );
}

quat_t MeCtGazeJoint::evaluate( float dt, quat_t target_rot, quat_t off_rot, float scale_factor )	{
	
	prev_world_rot = world_rot;
	capture_joint_state();
	quat_t Q_task = ( rotation_to_target( target_rot ) * off_rot ) * scale_factor;

#if ENABLE_CONSTRAIN
	quat_t Q = constrain( dt, Q_task );
#else
	quat_t Q = Q_task;
#endif
	prev_local_rot = Q;
	vector_t newAxis = (-world_zero_rot)*Q.axis();
	quat_t Qout = quat_t(Q.degrees(),newAxis);
	return( Q );
}

void MeCtGazeJoint::capture_joint_state( void ) {
	SrMat sr_M;
	gwiz::matrix_t M, PM,WM, PZ;
	int i, j;
#if USE_OLD_GAZE
	if( joint_p )	{

		sr_M = joint_p->lmat();
		for( i=0; i<4; i++ )	{
			for( j=0; j<4; j++ )	{
				M.set( i, j, sr_M.get( i, j ) );
			}
		}		
		local_pos = M.translation( gwiz::COMP_M_TR );
		local_rot = M.quat( gwiz::COMP_M_TR );

		sr_M = joint_p->gmat();
		for( i=0; i<4; i++ )	{
			for( j=0; j<4; j++ )	{
				M.set( i, j, sr_M.get( i, j ) );
			}
		}
		world_pos = M.translation( gwiz::COMP_M_TR );
		world_rot = M.quat( gwiz::COMP_M_TR );

		SkJoint* parent_p = joint_p->parent();
		if( parent_p )	{

			sr_M = parent_p->lmat();
			for( i=0; i<4; i++ )	{
				for( j=0; j<4; j++ )	{
					M.set( i, j, sr_M.get( i, j ) );
				}
			}
			parent_loc_pos = M.translation( gwiz::COMP_M_TR );
			parent_loc_rot = M.quat( gwiz::COMP_M_TR );

			sr_M = parent_p->gmat();
			for( i=0; i<4; i++ )	{
				for( j=0; j<4; j++ )	{
					M.set( i, j, sr_M.get( i, j ) );
				}
			}
			parent_pos = M.translation( gwiz::COMP_M_TR );
			parent_rot = M.quat( gwiz::COMP_M_TR );
		}
		else	{
			const char *name = joint_p->name().c_str();
			LOG( "MeCtGazeJoint::capture_joint_state ERR: parent of joint '%s' not found\n", name );
		}
	}
#else // consider the pre-rotation
	if( joint_p )	{
		sr_M = joint_p->gmat();
		for( i=0; i<4; i++ )	{
			for( j=0; j<4; j++ )	{
				M.set( i, j, sr_M.get( i, j ) );
			}
		}

		SrMat srZeroM = joint_p->gmatZero();
		for( i=0; i<4; i++ )	{
			for( j=0; j<4; j++ )	{
				WM.set( i, j, srZeroM.get( i, j ) );
			}
		}
		world_zero_pos = WM.translation( gwiz::COMP_M_TR );
		world_zero_rot = WM.quat( gwiz::COMP_M_TR );

		world_pos = M.translation( gwiz::COMP_M_TR );
		world_rot = M.quat( gwiz::COMP_M_TR );		

		SkJoint* parent_p = joint_p->parent();		
		if( parent_p )	{
			sr_M = parent_p->gmat();
			for( i=0; i<4; i++ )	{
				for( j=0; j<4; j++ )	{
					PM.set( i, j, sr_M.get( i, j ) );
				}
			}
			parent_pos = PM.translation( gwiz::COMP_M_TR );
			parent_rot = PM.quat( gwiz::COMP_M_TR );	


			SrMat sr_PZ = parent_p->gmatZero();
			for( i=0; i<4; i++ )	{
				for( j=0; j<4; j++ )	{
					PZ.set( i, j, sr_PZ.get( i, j ) );
				}
			}
			parent_zero_pos = PZ.translation( gwiz::COMP_M_TR );
			parent_zero_rot = PZ.quat( gwiz::COMP_M_TR );	

			parent_diff_rot = world_zero_rot*(-parent_zero_rot);

			gwiz::matrix_t LM = WM*(-PM)*M;
			local_pos = (parent_zero_rot)*(-parent_rot)*(world_pos-parent_pos);//LM.translation( gwiz::COMP_M_TR );

			local_rot = (parent_zero_rot)*(-parent_rot)*(world_rot)*(-world_zero_rot);//LM.quat( gwiz::COMP_M_TR );
			//gwiz::vector_t newAxis = world_zero_rot*local_rot.axis();
			//local_rot = quat_t(local_rot.degrees(),newAxis);
			}	   

		}
		else	{
			const char *name = joint_p->jointName().c_str();
			LOG( "MeCtGazeJoint::capture_joint_state ERR: parent of joint '%s' not found\n", name );
		}
#endif
}

////////////////////////////////////////

#define DFL_FORWARD_RAY_BUFFER_ZONE_RATIO 0.2

#define ENABLE_ZERO_ROLL	1

#if ENABLE_ZERO_ROLL

quat_t MeCtGazeJoint::rotation_to_target( vector_t target_pos )	{
	
	// no roll

#if ENABLE_FORWARD_RAY

	// parent coord with forward-ray
	/*
	quat_t rotation_ray_to_target_point(
		vector_t X,   // target
		vector_t R,   // center of rotation
		vector_t Fo,  // forward ray origin
		vector_t Fd,  // forward ray direction
		gwiz::float_t buffer_ratio = 0.1,	// proportion of buffer zone for pathological case
	)
	*/
#if USE_OLD_GAZE
	euler_t E_forward = forward_rot;
	quat_t forward_ph = euler_t( E_forward.p(), E_forward.h(), 0.0 );
	quat_t forward_roll = euler_t( 0.0, 0.0, E_forward.r() );
	
	vector_t l_target_pos = ( -parent_rot ) * ( target_pos - parent_pos );
	vector_t forward_dir = ( -forward_ph ) * forward_ref.normal();

	quat_t Q = rotation_ray_to_target_point(
		l_target_pos,
		local_pos,
		forward_pos + local_pos,
		forward_dir,
		DFL_FORWARD_RAY_BUFFER_ZONE_RATIO
	);
#else
	euler_t E_forward = forward_rot;
	quat_t forward_ph = euler_t( E_forward.p(), E_forward.h(), 0.0 );
	quat_t forward_roll = euler_t( 0.0, 0.0, E_forward.r() );
	
	vector_t l_target_pos = ( -parent_rot ) * ( target_pos - parent_pos );
	//vector_t l_target_pos = (world_zero_rot)*(-world_rot)*( target_pos - world_pos );
	vector_t forward_dir = ( -forward_ph ) * forward_ref.normal();
	vector_t l_rot_pos = (-parent_rot)* (world_pos-parent_pos);

	quat_t Q_p = rotation_ray_to_target_point(
		(parent_zero_rot)*l_target_pos,
		(parent_zero_rot)*l_rot_pos,
		(parent_zero_rot)*((-parent_zero_rot)*forward_pos + l_rot_pos),
		(parent_zero_rot)*(-parent_zero_rot)*forward_dir,
		DFL_FORWARD_RAY_BUFFER_ZONE_RATIO
	);
	quat_t Q = Q_p;
	//quat_t Q = (matrix_t(-world_zero_rot)*matrix_t(Q_p)).quat();
#endif

/*
	INCORRECT:
		- can't just apply roll here. 
		- must first inverse-roll the forward ray position above.
*/

//	return( Q * forward_roll );
	return( Q );

#else

#if 0
	// world coord
	return( ( -parent_rot ) * quat_t( euler_t( target_pos - world_pos, 0.0 ) ) );

#elif 1
	// parent coord
	vector_t l_target_pos = ( -parent_rot ) * ( target_pos - parent_pos );
	return( quat_t( euler_t( l_target_pos - local_pos, 0.0 ) ) );

#elif 0
	// parent coord with forward-rot
	vector_t l_target_pos = ( -parent_rot ) * ( target_pos - parent_pos );
	return( quat_t( euler_t( l_target_pos - local_pos, 0.0 ) ) * forward_rot );

#else
	// parent coord with forward-rot

	euler_t E_forward = forward_rot;
	quat_t forward_ph = euler_t( E_forward.p(), E_forward.h(), 0.0 );
	quat_t forward_roll = euler_t( 0.0, 0.0, E_forward.r() );
	
	vector_t l_target_pos = ( -parent_rot ) * ( target_pos - parent_pos );
	vector_t forward_dir = forward_ph * ( l_target_pos - local_pos );
	
	return( quat_t( euler_t( forward_dir, 0.0 ) ) * forward_roll );

#endif
#endif
}

#elif 1

quat_t MeCtGazeJoint::rotation_to_target( vector_t target_pos )	{
	
	// free roll:
#if 0
	// world coord
	vector_t w_forward_dir_n = parent_rot * forward_rot * forward_ref.normal();
	vector_t target_dir_n = ( target_pos - world_pos ).normal();

	gwiz::float_t angle = DEG( gwiz::safe_acos( w_forward_dir_n.dot( target_dir_n ) ) );
	vector_t axis = w_forward_dir_n.cross( target_dir_n ).normal();

	return( ( -parent_rot ) * quat_t( angle, axis ) * ( parent_rot ) );

#else
	// parent coord
	vector_t forward_dir_n = forward_rot * forward_ref.normal();
	vector_t l_target_dir_n = ( -parent_rot ) * ( target_pos - world_pos ).normal();

	gwiz::float_t angle = DEG( gwiz::safe_acos( forward_dir_n.dot( l_target_dir_n ) ) );
	vector_t axis = forward_dir_n.cross( l_target_dir_n ).normal();

	return( quat_t( angle, axis ) );
#endif
}

#elif 0

// GARBAGE ?

quat_t MeCtGazeJoint::rotation_to_target( vector_t target_pos )	{
	
//	vector_t forward_dir_n = parent_rot * forward_rot * forward_ref.normal();
	vector_t forward_dir_n = forward_rot * forward_ref.normal();
	vector_t target_dir_n = ( target_pos - world_pos ).normal();

	float angle = (float)DEG( gwiz::safe_acos( forward_dir_n.dot( target_dir_n ) ) );
	vector_t axis = forward_dir_n.cross( target_dir_n ).normal();
	
//	quat_t Q = quat_t( -45.0, vector_t( 0.0, 1.0, 0.0 ) );
//	quat_t Q = quat_t( -45.0, vector_t( 1.0, 0.0, 0.0 ) );
//	quat_t Q = quat_t( -45.0, vector_t( 0.0, 1.0, 0.0 ) ) * quat_t( -45.0, vector_t( 1.0, 0.0, 0.0 ) );

//	quat_t Q = ( -parent_rot ) * quat_t( -45.0, vector_t( 0.0, 1.0, 0.0 ) );
//	quat_t Q = ( -parent_rot ) * quat_t( -45.0, vector_t( 1.0, 0.0, 0.0 ) );
//	quat_t Q = ( -parent_rot ) * quat_t( -45.0, vector_t( 0.0, 1.0, 0.0 ) ) * quat_t( -45.0, vector_t( 1.0, 0.0, 0.0 ) );

//	quat_t Q = quat_t( angle, axis );
	quat_t Q = ( -parent_rot ) * quat_t( angle, axis );

	return( Q );
}

#else

quat_t MeCtGazeJoint::rotation_to_target( vector_t target_pos )	{
	
	euler_t E_forward = forward_rot;
	quat_t forward_ph = euler_t( E_forward.p(), E_forward.h(), 0.0 );
	quat_t forward_roll = euler_t( 0.0, 0.0, E_forward.r() );
	
//	vector_t forward_dir_n = parent_rot * ( -forward_ph ) * forward_ref.normal();
	vector_t forward_dir_n = ( -forward_ph ) * forward_ref.normal();
	vector_t target_dir_n = ( target_pos - world_pos ).normal();

	float angle = (float)DEG( gwiz::safe_acos( forward_dir_n.dot( target_dir_n ) ) );
	vector_t axis = forward_dir_n.cross( target_dir_n ).normal();
	
	return( ( -parent_rot ) * quat_t( angle, axis ) * forward_roll );
}
#endif

////////////////////////////

quat_t MeCtGazeJoint::rotation_to_target( quat_t target_rot )	{
	
#if USE_OLD_GAZE
	
/*
	quat_t rotation_ray_to_target_orient(
		quat_t Q,     // target orientation
		vector_t Fd,  // forward ray direction
		vector_t Fr = vector_t( 0.0, 0.0, 1.0 ) // null reference ray direction
	)
*/

	return( ( -parent_rot ) * target_rot * forward_rot );
	
#else
	return( (parent_zero_rot)*( -parent_rot ) * target_rot * forward_rot );
#endif
}

#if 0
quat_t MeCtGazeJoint::constrain_box( quat_t task_rot )	{

	float lim_x = (float)limit_rot.x();
	float lim_y = (float)limit_rot.y();
	float lim_z = (float)limit_rot.z();

	euler_t e = task_rot;
	float e_x = (float)e.x();
	float e_y = (float)e.y();
	float e_z = (float)e.z();

	if( e_x > lim_x ) e_x = lim_x;
	if( e_x < -lim_x ) e_x = -lim_x;
	if( e_y > lim_y ) e_y = lim_y;
	if( e_y < -lim_y ) e_y = -lim_y;
	if( e_z > lim_z ) e_z = lim_z;
	if( e_z < -lim_z ) e_z = -lim_z;

	return( quat_t( euler_t( e_x, e_y, e_z ) ) );
}

quat_t MeCtGazeJoint::constrain_swing( quat_t task_rot )	{

	float lim_x = (float)limit_rot.x();
	float lim_y = (float)limit_rot.y();
	float lim_z = (float)limit_rot.z();

	vector_t st = task_rot.swingtwist();
	float sw_x = (float)st.x();
	float sw_y = (float)st.y();
	float tw = (float)st.z();

	if( sw_x > lim_x ) sw_x = lim_x;
	if( sw_x < -lim_x ) sw_x = -lim_x;
	if( sw_y > lim_y ) sw_y = lim_y;
	if( sw_y < -lim_y ) sw_y = -lim_y;
	if( tw > lim_z ) tw = lim_z;
	if( tw < -lim_z ) tw = -lim_z;

	return( quat_t( sw_x, sw_y, tw ) );
}

quat_t MeCtGazeJoint::constrain_quat( quat_t task_rot )	{

	float mag = (float)quat_t( limit_rot ).degrees();
	if( task_rot.degrees() > mag )    {
		task_rot = quat_t( mag, task_rot.axis() );
	}
	return( task_rot );
}
#endif

#if 0
quat_t MeCtGazeJoint::constrain_ellipse( quat_t task_rot )	{
	
	float lim_x = (float)limit_rot.x();
	float lim_y = (float)limit_rot.y();
	float lim_z = (float)limit_rot.z();
	
	vector_t st = task_rot.swingtwist();
	float sw_x = (float)st.x();
	float sw_y = (float)st.y();

	// Marcelo's exact ellipse solution:
	if( sr_in_ellipse( sw_x, sw_y, lim_x, lim_y ) > 0.0 )	{
		sr_get_closest_on_ellipse( lim_x, lim_y, sw_x, sw_y );
	}

	float tw = (float)st.z();
	if( tw > lim_z ) tw = lim_z;
	if( tw < -lim_z ) tw = -lim_z;
	
	return( quat_t( sw_x, sw_y, tw ) );
}
#endif

quat_t MeCtGazeJoint::constrain_ellipse( quat_t task_rot )	{
	
	vector_t st = task_rot.swingtwist();
	float sw_x = (float)st.x();
	float sw_y = (float)st.y();

//if( id == 6 ) LOG( "%f %f %f %f\n", limit_p_up, limit_p_dn, limit_h, limit_r );
//euler_t e = task_rot; if( id == 6 ) e.print();

	// Marcelo's exact ellipse solution:
	if( sw_x > 0.0 )	{ // positive pitch is down...
		if( sr_in_ellipse( sw_x, sw_y, limit_p_dn, limit_h ) > 0.0 )	{
			sr_get_closest_on_ellipse( limit_p_dn, limit_h, sw_x, sw_y );
		}
	}
	else	{
		if( sr_in_ellipse( sw_x, sw_y, limit_p_up, limit_h ) > 0.0 )	{
			sr_get_closest_on_ellipse( limit_p_up, limit_h, sw_x, sw_y );
		}
	}

	float tw = (float)st.z();
	if( tw > limit_r ) tw = limit_r;
	if( tw < -limit_r ) tw = -limit_r;
	
	return( quat_t( sw_x, sw_y, tw ) );
}

quat_t MeCtGazeJoint::constrain_quat_speed( float dt, quat_t task_rot )    {
	quat_t Q_delta;
	quat_t Q_ret;
	
	quat_t Q_dif = ( task_rot * ( -prev_local_rot ) ).shortest();
	float deg = (float)( Q_dif.degrees() );
	vector_t axis = Q_dif.axis();

	float dps = deg / dt;
	if( dps < speed )	{
		Q_ret = task_rot;
	}
	else	{
		Q_delta = quat_t( speed * dt, axis );
		Q_ret = prev_local_rot * Q_delta;
	}

#if 0
if( id == 5 )	{ // MeCtGaze::GAZE_JOINT_SKULL
 t_elapse += dt;
 Q_dif = ( world_rot * ( -prev_world_rot ) ).shortest();
 dps = (float)( Q_dif.degrees() ) / dt;
 if( dps > 0.1 ) LOG( "MeCtGazeJoint world-speed: %f  at %f\n", dps, t_elapse );
}
#endif

	return( Q_ret );
}

#define SMOOTH_RATE_REF (30.0f)

quat_t MeCtGazeJoint::constrain_smooth( float dt, quat_t task_rot )	{

	if( smooth > 0.0 )	{
		float s = (float)(0.01 + ( 1.0 - powf( smooth, dt * SMOOTH_RATE_REF ) ) * 0.99);
		return(
			prev_local_rot.lerp( 
				s, 
				task_rot
			)
		);
	}
	return( task_rot );
}
	
quat_t MeCtGazeJoint::constrain( float dt, quat_t task_rot )	{
	
	quat_t Q_w = task_rot * task_weight;
	
	quat_t Q_c = 
#if 0
		Q_w;
#elif 0
		constrain_box( Q_w );
#elif 0
		constrain_swing( Q_w );
#elif 0
		constrain_quat( Q_w );
#else
		constrain_ellipse( Q_w );
#endif

#if 0
	quat_t Q_sm = Q_c;
#elif 0
	quat_t Q_sm = 
		constrain_smooth(
			dt,
			Q_c
		);
#elif 0
	quat_t Q_sm = 
		constrain_quat_speed(
			dt,
			Q_c
		);
#else
	quat_t Q_sp = 
		constrain_quat_speed(
			dt,
			Q_c
		);
	
	quat_t Q_sm = 
		constrain_smooth(
			dt,
			Q_sp
		);
#endif

//euler_t e = Q_sm; if( id == 6 ) e.print();
	return( Q_sm );
}

///////////////////////////////////////////////////////////////////////////
