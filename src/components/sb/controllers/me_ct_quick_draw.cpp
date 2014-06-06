/*
 *  me_ct_quick_draw.cpp - part of SmartBody-lib
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
 *      Andrew n marshall, USC
 */

#include "controllers/me_ct_quick_draw.h"
#include <vhcl_log.h>

using namespace gwiz;

#include "sbm/sbm_constants.h"

#define DFL_CTRL_TIME_INDT		( 0.3f )
#define DFL_CTRL_TIME_OUTDT		( 0.3f )

//////////////////////////////////////////////////////////////////////////////////

std::string MeCtQuickDraw::type_name = "QuickDraw";

MeCtQuickDraw::MeCtQuickDraw( void )	{

	_gundraw_motion = NULL;
	_holster_motion = NULL;
	_curr_motion_p = NULL;
	
	_play_mode = SkMotion::Linear;
	_duration = -1.0;
	_last_apply_frame = 0;

	skeleton_ref_p = NULL;
	target_ref_joint_str = NULL;
	target_ref_joint_p = NULL;

	interim_pose_buff_p = NULL;
	
	smooth = 0.75;
	start = 0;
	prev_time = 0.0;

	blend_in = 0.0;
	blend_out = 0.0;
	raw_gundraw_dur = 0.0;
	raw_gundraw_scale = 1.0;
	play_gundraw_dur = 0.0;
	hard_gundraw_dur = 0.0;
	has_gundraw_leadup = false;
	raw_holster_dur = 0.0;
	raw_holster_scale = 1.0;
	play_holster_dur = 0.0;
	hard_holster_dur = 0.0;
	has_holster_leadaway = false;

	track_dur = 0.0;
	draw_mode = DRAW_DISABLED;
}

MeCtQuickDraw::~MeCtQuickDraw( void )	{

	if( interim_pose_buff_p )	{
		delete [] interim_pose_buff_p;
		interim_pose_buff_p = NULL;
	}
}

void print_motion( SkMotion* mot_p )	{

	if( mot_p ) {
		LOG( "motion:'%s'\n", mot_p->getName().c_str() );
		LOG( "file:'%s'\n", mot_p->filename().c_str() );
		LOG( " ready:     %f\n", mot_p->time_ready() );
		LOG( " str-start: %f \n", mot_p->time_stroke_start() );
		LOG( " str-emph:  %f \n", mot_p->time_stroke_emphasis() );
		LOG( " str-end:   %f \n", mot_p->time_stroke_end() );
		LOG( " relax:     %f \n", mot_p->time_relax() );
	}
}

void MeCtQuickDraw::init( SbmPawn* pawn, SkMotion* mot_p, SkMotion* mot2_p ) {
#if 0
	static char l_arm_labels[ NUM_ARM_JOINTS ][ MAX_JOINT_LABEL_LEN ] = {
		"l_shoulder", "l_elbow", "l_forearm", "l_wrist"
	};
#else
	static char r_arm_labels[ NUM_ARM_JOINTS ][ MAX_JOINT_LABEL_LEN ] = {
		"r_shoulder", "r_elbow", "r_forearm", "r_wrist"
	};
#endif

	if( _gundraw_motion ) {
		if( mot_p == _gundraw_motion ) {
			// Minimal init()
			_last_apply_frame = 0;
			MeController::init (pawn);
			return;
		}
	}
	
#if 0
	print_motion( mot_p );
	print_motion( mot2_p );
#endif

	_last_apply_frame = 0;
	
	_gundraw_motion = mot_p;
	_gundraw_motion->move_keytimes( 0 ); 
	raw_gundraw_dur = _gundraw_motion->duration();
	set_gundraw_duration( raw_gundraw_dur );

	_holster_motion = NULL;
	if( mot2_p ) {
		if( 
			( _gundraw_motion->channels().size() == mot2_p->channels().size() )&&
			( _gundraw_motion->channels().count_floats() == mot2_p->channels().count_floats() )
		)	{
			_holster_motion = mot2_p;
			_holster_motion->move_keytimes( 0 ); 
			raw_holster_dur = _holster_motion->duration();
			set_holster_duration( raw_holster_dur );
		}
		else	{
			LOG( "MeCtQuickDraw::init ERR: unmatched channels in reholster motion '%s', IGNORED\n", mot2_p->filename().c_str() );
		}
	}
	else	{
		raw_holster_dur = raw_gundraw_dur;
		set_holster_duration( raw_holster_dur );
	}

	start = 0;
	prev_time = 0.0;
	draw_mode = DRAW_DISABLED;
	
//	const int n_chan = _gundraw_motion->channels().size();
	const int n_float = _gundraw_motion->channels().count_floats();
	
	l_final_hand_in_body_rot = quat_t();
	l_wrist_offset_rot = quat_t();
	
	_arm_chan_indices.size( NUM_ARM_JOINTS );
	for( int i = 0; i < NUM_ARM_JOINTS; i++ )	{

		// ASSUME right handed draw
		_arm_chan_indices[ i ] = 
			_gundraw_motion->channels().search( r_arm_labels[ i ] , SkChannel::Quat );
		
		quat_t joint_rot = get_final_joint_rot( _gundraw_motion, r_arm_labels[ i ] );
		l_final_hand_in_body_rot = l_final_hand_in_body_rot * joint_rot;
	}

	if( interim_pose_buff_p )	{
		delete [] interim_pose_buff_p;
	}
	interim_pose_buff_p = new float[ n_float ];
	
	reset_blend();
	
	MeController::init(pawn);

	if( _context ) {
		// Notify _context of channel change.
		_context->child_channels_updated( this );
	}
}

///////////////////////////////////////////////////////////////////////////////////

void MeCtQuickDraw::reset_blend( void ) {

/*
	NOTE:
		-assumes critical motion start time is at time_ready()
		-assumes critical motion end time is at time_relax()
		-could also be set at time_stroke_start() and time_stroke_end()
*/
	if( _gundraw_motion->time_ready() > 0.0 )	{
		has_gundraw_leadup = true;
		blend_in = (float)_gundraw_motion->time_ready() / raw_gundraw_scale;
	}
	else	{
		has_gundraw_leadup = false;
		blend_in = DFL_CTRL_TIME_INDT;
	}

	if( _holster_motion )	{
		float relax_interval = (float)_holster_motion->duration() - (float)_holster_motion->time_relax();
		if( relax_interval > 0.001 )   {
			has_holster_leadaway = true;
			blend_out = relax_interval / raw_holster_scale;
		}
		else	{
			has_holster_leadaway = false;
			blend_out = DFL_CTRL_TIME_OUTDT;
		}
	}
	else	{
		has_holster_leadaway = has_gundraw_leadup;
		blend_out = blend_in;
	}

	inoutdt( blend_in, blend_out );
}

void MeCtQuickDraw::set_gundraw_duration( float sec )	{
	
	play_gundraw_dur = sec;
	raw_gundraw_scale = raw_gundraw_dur / play_gundraw_dur;
	reset_blend();
}

void MeCtQuickDraw::set_holster_duration( float sec )	{
	
	play_holster_dur = sec;
	raw_holster_scale = raw_holster_dur / play_holster_dur;
	reset_blend();
}

void MeCtQuickDraw::set_motion_duration( float gundraw_sec, float holster_sec )	{
	
	set_gundraw_duration( gundraw_sec );
	set_holster_duration( holster_sec );
}

void MeCtQuickDraw::set_motion_duration( float sec )	{
	
	set_gundraw_duration( sec );
	set_holster_duration( sec );
}

void MeCtQuickDraw::set_aim_offset( float p, float h, float r ) {
	
	l_wrist_offset_rot = euler_t( p, h, r );
}

void MeCtQuickDraw::set_target_coord_joint( SkJoint* joint_p )	{

	if( target_ref_joint_str ) free( target_ref_joint_str );
	target_ref_joint_str = NULL;
	target_ref_joint_p = joint_p;
}

void MeCtQuickDraw::set_target_point( float x, float y, float z )	{

	point_target_pos = vector_t( x, y, z );
}

void MeCtQuickDraw::set_target_joint( float x, float y, float z, SkJoint* ref_joint_p ) {

	set_target_point( x, y, z );
	set_target_coord_joint( ref_joint_p );
}

///////////////////////////////////////////////////////////////////////////////////

quat_t MeCtQuickDraw::get_final_joint_rot( SkMotion* mot_p, char *joint_name )	{
	quat_t q;
	
	if( mot_p && joint_name ) {
	
		float * final_p = mot_p->posture( mot_p->frames() - 1 );
		SkChannelArray& mchan_arr = mot_p->channels();

		int i = mchan_arr.float_position( mchan_arr.search(joint_name, SkChannel::Quat ) );
		q = quat_t( final_p[ i ], final_p[ i + 1 ], final_p[ i + 2 ], final_p[ i + 3 ] );
	}
	return( q );
}

MeCtQuickDraw::joint_state_t MeCtQuickDraw::capture_joint_state( SkJoint *joint_p ) {
	SrMat sr_M;
	matrix_t M;
	int i, j;
	joint_state_t state;

	if( joint_p )	{

		sr_M = joint_p->lmat();
		for( i=0; i<4; i++ )	{
			for( j=0; j<4; j++ )	{
				M.set( i, j, sr_M.get( i, j ) );
			}
		}
		state.local_pos = M.translation( COMP_M_TR );
		state.local_rot = M.quat( COMP_M_TR );

		sr_M = joint_p->gmat();
		for( i=0; i<4; i++ )	{
			for( j=0; j<4; j++ )	{
				M.set( i, j, sr_M.get( i, j ) );
			}
		}
		state.world_pos = M.translation( COMP_M_TR );
		state.world_rot = M.quat( COMP_M_TR );

		SkJoint* parent_p = joint_p->parent();
		if( parent_p )	{

			sr_M = parent_p->gmat();
			for( i=0; i<4; i++ )	{
				for( j=0; j<4; j++ )	{
					M.set( i, j, sr_M.get( i, j ) );
				}
			}
			state.parent_pos = M.translation( COMP_M_TR );
			state.parent_rot = M.quat( COMP_M_TR );
		}
		else	{
			const char *name = joint_p->jointName().c_str();
			LOG( "MeCtQuickDraw::capture_joint_state ERR: parent of joint '%s' not found\n", name );
		}
	}
	return( state );
}

quat_t MeCtQuickDraw::rotation_to_target( vector_t l_forward_dir, vector_t w_target_pos, joint_state_t* state_p )	{

	vector_t l_target_dir_n = ( -state_p->parent_rot ) * ( w_target_pos - state_p->world_pos ).normal();

	gwiz::float_t rad = safe_acos( l_forward_dir.dot( l_target_dir_n ) );
	gwiz::float_t angle = deg( rad );
	vector_t axis = l_forward_dir.cross( l_target_dir_n ).normal();

	return( quat_t( angle, axis ) );
}

SkJoint* MeCtQuickDraw::find_joint( char *joint_str, SkJoint **joint_pp )	{

	if( joint_str )	{
		if( *joint_pp == NULL )	{
			if( skeleton_ref_p )	{
				*joint_pp = skeleton_ref_p->search_joint( joint_str );
				if( *joint_pp == NULL )	{
					fprintf( stderr, "MeCtQuickDraw::find_joint ERR: joint '%s' NOT FOUND in skeleton\n", joint_str );
					free( joint_str );
					joint_str = NULL;
				}
			}
			else	{
				fprintf( stderr, "MeCtQuickDraw::find_joint ERR: skeleton NOT FOUND\n" );
			}
		}
	}
	return( *joint_pp );
}

SkJoint* MeCtQuickDraw::target_ref_joint( void ) {
//	return( target_ref_joint_p = find_joint( target_ref_joint_str, target_ref_joint_p ) );
	return( find_joint( target_ref_joint_str, &target_ref_joint_p ) );
}


vector_t MeCtQuickDraw::world_target_point( void )	{
	
	SkJoint* joint_p = target_ref_joint();
	if( joint_p )	{
		SrMat sr_M;
		matrix_t M;
		int i, j;

		joint_p->update_gmat_up();
		sr_M = joint_p->gmat();
		for( i=0; i<4; i++ )	{
			for( j=0; j<4; j++ )	{
				M.set( i, j, sr_M.get( i, j ) );
			}
		}
		vector_t pos = M.translation( COMP_M_TR );
		quat_t rot = M.quat( COMP_M_TR );
		return( pos + rot * point_target_pos );
	}
	return( point_target_pos );
}

///////////////////////////////////////////////////////////////////////////////////

void MeCtQuickDraw::context_updated( void ) {

#if 0
	if( _context ) {
		skeleton_ref_p = _context->channels().skeleton(); // WHY HERE?
		if( skeleton_ref_p == NULL )	{
			LOG( "MeCtQuickDraw::context_updated ERR: skeleton_ref_p is NULL\n" );
		}
	}
	else	{
		LOG( "MeCtQuickDraw::context_updated ERR: context is NULL\n" );
	}
#endif
}

void MeCtQuickDraw::controller_map_updated( void ) {

	SkChannelArray& motion_channels = _gundraw_motion->channels();
	const int n_chan = motion_channels.size();
	_motion_chan_to_buff.size( n_chan );


	if( _context ) {

		SkChannelArray& context_channels = _context->channels();
		for( int i=0; i<n_chan; ++i ) {
			int chan_index = context_channels.search( motion_channels.name( i ), motion_channels.type( i ) );
			_motion_chan_to_buff[ i ] = _context->toBufferIndex( chan_index );
		}

	} 
	else {
		LOG( "MeCtQuickDraw::controller_map_updated: context NOT found\n" );
		_motion_chan_to_buff.setall( -1 );
	}
}

void MeCtQuickDraw::controller_start( void )	{

	if( _context->channels().size() > 0 )	{
		skeleton_ref_p = _context->channels().skeleton();
	}
	
#if 0
	if( has_gundraw_leadup )
		LOG( "animated indt: %f\n", indt() );
	else
		LOG( "static indt: %f\n", indt() );
	if( has_holster_leadaway )
		LOG( "animated outdt: %f\n", outdt() );
	else
		LOG( "static outdt: %f\n", outdt() );
#endif

	hard_gundraw_dur = play_gundraw_dur - ( has_gundraw_leadup ? indt() : 0.0f );
	hard_holster_dur = play_holster_dur - ( has_holster_leadaway ? outdt() : 0.0f );

//LOG( "drawgun: play:%f hard:%f\n", play_gundraw_dur, hard_gundraw_dur );
//LOG( "holster: play:%f hard:%f\n", play_holster_dur, hard_holster_dur );

	start = 1;
	draw_mode = DRAW_READY;
}

///////////////////////////////////////////////////////////////////////////////////

/*
	Redefine H-P-R, without moving the axes:
		frame: orientation of H-P-R def relative to standard X-Y-Z
*/
quat_t GWIZ_to_frame( quat_t q, quat_t frame_q )	{
	return( -frame_q * q * frame_q );
}
quat_t GWIZ_from_frame( quat_t q, quat_t frame_q )	{
	return( frame_q * q * -frame_q );
}

#define SMOOTH_RATE_REF (30.0f)

bool MeCtQuickDraw::controller_evaluate( double t, MeFrameData& frame ) {

#if 0
static float tmp_prev_t = 0.0;
static float tmp_accum = 0.0;
static int tmp_count = 0;
int tmp_event = 0;
tmp_accum += ( (float)t - tmp_prev_t );
tmp_prev_t = (float)t;
if( ( tmp_accum > 0.2f )&&( tmp_count < 30 ) )	{
	tmp_accum = 0.0f;
	tmp_event = 1;
	tmp_count++;
}
else	{
	tmp_event = 0;
}
#endif

	bool continuing = true;
	continuing = t < _duration; // ERR: -1.0
	if( t < 0.0 )	{
		return( continuing );
	}
	
	float dt;
	if( start ) {
		start = 0;
		dt = 0.001f;
	}
	else	{
		dt = (float)(t - prev_time);
	}
	prev_time = t;

	// increment draw_mode, select motion, and set motion_time:
	float motion_time = 0.0;
	gwiz::float_t raw_lerp = 0.0;
	

	if( draw_mode == DRAW_READY )	{
		_curr_motion_p = _gundraw_motion;
		curr_motion_dur = raw_gundraw_dur;
		curr_motion_scale = raw_gundraw_scale;
		curr_play_dur = play_gundraw_dur;
		curr_hard_dur = hard_gundraw_dur;
		if( has_gundraw_leadup )	{
			motion_time = (float)t;
		}
		if( t > indt() )	{
			draw_mode = DRAW_AIMING;
		}
//LOG( "READY: play:%.2f hard:%.2f motT:%.2f lerp:%.3f\n", play_gundraw_dur, hard_gundraw_dur, motion_time, raw_lerp );
	}
	if( draw_mode == DRAW_AIMING )	{
		float mode_time = (float)t - indt();
		if( mode_time < hard_gundraw_dur )	{
			if( has_gundraw_leadup )	{
				motion_time = (float)t;
				raw_lerp = mode_time / hard_gundraw_dur;
			}
			else	{
				motion_time = mode_time;
				raw_lerp = motion_time / hard_gundraw_dur;
			}
		}
		else	{
			draw_mode = DRAW_TRACKING; 
		}
//LOG( "AIM: play:%.2f hard:%.2f motT:%.2f lerp:%.3f\n", play_gundraw_dur, hard_gundraw_dur, motion_time, raw_lerp );
	}
	if( draw_mode == DRAW_TRACKING )	{
		motion_time = play_gundraw_dur;
		raw_lerp = 1.0;
		float mode_time;
		if( track_dur >= 0.0 )	{
			mode_time = (float)t - curr_hard_dur - indt();
			if( mode_time > track_dur ) {
				reholster_time = (float)t;
				draw_mode = DRAW_RETURN; 
			}
		}
//LOG( "TRACK: play:%.2f hard:%.2f motT:%.2f lerp:%.3f\n", play_gundraw_dur, hard_gundraw_dur, motion_time, raw_lerp );
//LOG( "TRACK:%.2f time:%.2f modeT:%.2f motT:%.2f lerp:%.3f\n", track_dur, t, mode_time, motion_time, raw_lerp );
	}
	if( draw_mode == DRAW_RETURN )	{
		if( _holster_motion )	{
			_curr_motion_p = _holster_motion;
			curr_motion_dur = raw_holster_dur;
			curr_motion_scale = raw_holster_scale;
			curr_play_dur = play_holster_dur;
			curr_hard_dur = hard_holster_dur;
		}
		float mode_time = (float)t - reholster_time;
		if( mode_time < curr_hard_dur )	{
			if( _holster_motion )	{
				motion_time = mode_time; // increasing
			}
			else	{
				motion_time = play_gundraw_dur - mode_time; // decreasing
			}
			if( has_holster_leadaway )	{
				raw_lerp = 1.0 - mode_time / curr_hard_dur;
			}
			else	{
				raw_lerp = 1.0 - mode_time / curr_hard_dur;
			}
//LOG( "RET: play:%.2f hard:%.2f motT:%.2f lerp:%.3f\n", curr_play_dur, curr_hard_dur, motion_time, raw_lerp );
		}
		else	{
			complete_time = (float)t;
			draw_mode = DRAW_COMPLETE; 
		}
	}
	if( draw_mode == DRAW_COMPLETE )	{
		float mode_time = (float)t - complete_time;
		if( mode_time < outdt() )	{
			if( _holster_motion )	{
				if( has_holster_leadaway )	{
					motion_time = curr_hard_dur + mode_time;
				}
				else	{
					motion_time = curr_hard_dur;
				}
			}
			else	{
				if( has_holster_leadaway )	{
					motion_time = outdt() - mode_time;
				}
				else	{
					motion_time = 0.0;
				}
			}
			raw_lerp = 0.0;
//LOG( "COMPL: play:%.2f hard:%.2f motT:%.2f lerp:%.3f\n", curr_play_dur, curr_hard_dur, motion_time, raw_lerp );
		}
		else	{
			draw_mode = DRAW_DISABLED; 
		}
	}
	if( draw_mode == DRAW_DISABLED )	{
		return( 0 );
	}

	// calc lerp blend between raw motion and modified aim
	if( raw_lerp > 1.0 ) raw_lerp = 1.0;
#if 1
	if( raw_lerp < 0.25 ) raw_lerp = 0.0;
	else raw_lerp = ( raw_lerp - 0.25 ) * 1.3333;
#elif 1
	if( raw_lerp < 0.5 ) raw_lerp = 0.0;
	else raw_lerp = ( raw_lerp - 0.5 ) * 2.0;
#endif
	gwiz::float_t lerp_power = 2.0;
	gwiz::float_t lerp_value = pow( raw_lerp, lerp_power );
	
//	LOG( "mot: %f dur: %f lerp: %f\n", motion_time, curr_play_dur, raw_lerp );

#if 0 // RAW QUICKDRAW
	_curr_motion_p->apply( 
		float( motion_time * curr_motion_scale ),
		&( frame.buffer()[0] ),  // pointer to buffer's float array
		&_motion_chan_to_buff,
		_play_mode, 
		&_last_apply_frame 
	);

#else // ADAPTED QUICKDRAW

	_curr_motion_p->apply( 
		float( motion_time * curr_motion_scale ),
		interim_pose_buff_p,
		NULL, // same order in interim_buffer
		_play_mode, 
		&_last_apply_frame 
	);

	float *fbuffer = &( frame.buffer()[0] );
	SkChannelArray& motion_channels = _curr_motion_p->channels();
	int n_chan = motion_channels.size();
	
	int interim_arm_chan_indices[ NUM_ARM_JOINTS ];
	int i_interim = 0;

	for( int i_chan=0; i_chan<n_chan; i_chan++ )	{

		int ch_size = motion_channels[ i_chan ].size();
		int fbuffer_offset = _motion_chan_to_buff[ i_chan ];
		if( fbuffer_offset >= 0 )	{
		
			bool is_arm_chan = false;
			int which_arm_chan = -1;
			for( int j = 0; j < NUM_ARM_JOINTS; j++ )	{
				if( i_chan == _arm_chan_indices[ j ] )	{
					is_arm_chan = true;
					which_arm_chan = j;
				}
			}

			if( is_arm_chan == true )	{
				interim_arm_chan_indices[ which_arm_chan ] = i_interim;
			}
			else	{
#define ENABLE_NON_ARM_MOTION 1
#if ENABLE_NON_ARM_MOTION
				for( int k=0; k<ch_size; k++ ) {
					fbuffer[ fbuffer_offset + k ] = interim_pose_buff_p[ i_interim + k ];
				}
#endif
			}
		}
		i_interim += ch_size;
	}


	if( skeleton_ref_p )	{
		SkJoint* r_wrist_joint_p = skeleton_ref_p->search_joint( "r_wrist" );
		r_wrist_joint_p->update_gmat_up();
	}
	else	{
		fprintf( stderr, "MeCtQuickDraw::controller_evaluate ERR: skeleton NOT FOUND\n" );
	}

	// apply smoothing to world target
	vector_t hard_w_point = world_target_point();
	float s = (float)(0.01 + ( 1.0 - powf( smooth, dt * SMOOTH_RATE_REF ) ) * 0.99 );
	vector_t w_point = prev_w_point.lerp( s, hard_w_point );
	prev_w_point = w_point;
	
	vector_t joint_forward_v( -1.0, 0.0, 0.0 );
	vector_t joint_upward_v( 0.0, 1.0, 0.0 );
	euler_t joint_frame_e;
	joint_frame_e.lookat( joint_forward_v, joint_upward_v );
	quat_t joint_frame_q = joint_frame_e;

	vector_t l_wrist_aim_v = l_wrist_offset_rot * joint_forward_v;

	// COMPUTE TARGET CONFIG
	for( int j = 0; j < NUM_ARM_JOINTS; j++ )	{

		i_interim = interim_arm_chan_indices[ j ];
		quat_t in_q(
			interim_pose_buff_p[ i_interim + 0 ],
			interim_pose_buff_p[ i_interim + 1 ],
			interim_pose_buff_p[ i_interim + 2 ],
			interim_pose_buff_p[ i_interim + 3 ]
		);
		euler_t in_e = in_q;

		SkJoint* joint_p = _context->channels().joint( _toContextCh[ _arm_chan_indices[ j ] ] );
		if( joint_p == NULL ) 
			LOG( "MeCtQuickDraw::controller_evaluate: joint is NULL\n" );
		joint_state_t joint_state = capture_joint_state( joint_p );

		quat_t out_q;
		if( j == ARM_JOINT_SHOULDER )	{

//			vector_t w_gesture_fwd_v = joint_state.parent_rot * l_final_hand_in_body_rot * joint_forward_v;
//			vector_t w_gesture_fwd_v = joint_state.parent_rot * l_final_hand_in_body_rot * l_wrist_aim_v;
//			quat_t adjust_q = rotation_to_target( w_gesture_fwd_v, w_point, & joint_state );
			vector_t l_gesture_fwd_v = l_final_hand_in_body_rot * l_wrist_aim_v;
			quat_t adjust_q = rotation_to_target( l_gesture_fwd_v, w_point, & joint_state );
			out_q = adjust_q * in_q;

//w_point.print();
//euler_t( joint_state.parent_rot ).print();
//l_wrist_aim_v.print();
//l_final_hand_in_body_rot.print();
//w_gesture_fwd_v.print();

//			euler_t e = GWIZ_to_frame( in_q, joint_frame_q );
//			quat_t q = euler_t( e.p(), e.h(), e.r() );
//			out_q = GWIZ_from_frame( q, joint_frame_q );
		}
		else
		if( j == ARM_JOINT_WRIST )	{
			
//			quat_t point_q = rotation_to_target( joint_forward_v, w_point, & joint_state );
			quat_t point_q = rotation_to_target( l_wrist_aim_v, w_point, & joint_state );
			euler_t alt_point_e = GWIZ_to_frame( point_q, joint_frame_q );
			euler_t alt_in_e = GWIZ_to_frame( in_q, joint_frame_q );
			quat_t alt_out_q = euler_t( alt_point_e.p(), alt_point_e.h(), alt_in_e.r() );
			out_q = GWIZ_from_frame( alt_out_q, joint_frame_q );
		}
		else	{
			out_q = in_q;
//			out_q = quat_t();
		}

		gwiz::float_t w, x, y, z;
#if 0
		w = in_q.w();
		x = in_q.x();
		y = in_q.y();
		z = in_q.z();
#elif 1
		quat_t blend_q = in_q.lerp( lerp_value, out_q );
		w = blend_q.w();
		x = blend_q.x();
		y = blend_q.y();
		z = blend_q.z();
#else
		w = out_q.w();
		x = out_q.x();
		y = out_q.y();
		z = out_q.z();
#endif
		
		int i_map = _motion_chan_to_buff[ _arm_chan_indices[ j ] ];
#if 0
		quat_t src_q( fbuffer[ i_map + 0 ], fbuffer[ i_map + 1 ], fbuffer[ i_map + 2 ], fbuffer[ i_map + 3 ] );
		quat_t hard_q( w, x, y, z );
		float s = (float)(0.01 + ( 1.0 - powf( smooth, dt * SMOOTH_RATE_REF ) ) * 0.99 );
		quat_t smooth_q = src_q.lerp( s, hard_q );
		
		fbuffer[ i_map + 0 ] = (float)smooth_q.w();
		fbuffer[ i_map + 1 ] = (float)smooth_q.x();
		fbuffer[ i_map + 2 ] = (float)smooth_q.y();
		fbuffer[ i_map + 3 ] = (float)smooth_q.z();
#else
		fbuffer[ i_map + 0 ] = (float)w;
		fbuffer[ i_map + 1 ] = (float)x;
		fbuffer[ i_map + 2 ] = (float)y;
		fbuffer[ i_map + 3 ] = (float)z;
#endif
	}

#endif // ADAPTED QUICKDRAW

	return continuing;
}

SkChannelArray& MeCtQuickDraw::controller_channels( void )	{

	return( _gundraw_motion->channels() );
}

double MeCtQuickDraw::controller_duration( void ) {

// THIS GETS CALLED PRIOR TO controller_start().
#if 0
	// screws up inoutdt() call, which calls this function
	if( ( play_gundraw_dur > 0.0 ) && ( track_dur >= 0.0 ) )	{
		return( indt() + play_gundraw_dur + play_holster_dur + track_dur + outdt() );
	}
#endif
	return( -1.0 );
}

const std::string& MeCtQuickDraw::controller_type( void ) const	{

	return type_name;
}

void MeCtQuickDraw::print_state( int tabCount ) {

	LOG("MeCtQuickDraw" );

	const char* str = getName().c_str();
	if( str )
		LOG(" \"%s\"", str );

	LOG(", motion" );
	if( _gundraw_motion ) {

		// motion name
		str = _gundraw_motion->getName().c_str();
		if( str )
			LOG(" \"%s\"", str );

		// motion filename
		str = _gundraw_motion->filename().c_str();
		if( str )
			LOG(" file \"%s\"", str );
	} 
	if( _holster_motion ) {

		// motion name
		str = _holster_motion->getName().c_str();
		if( str )
			LOG(" \"%s\"", str );

		// motion filename
		str = _holster_motion->filename().c_str();
		if( str )
			LOG(" file \"%s\"", str );
	} 
	else {
		LOG("=NULL" );
	}
	LOG("\n" );
}

//////////////////////////////////////////////////////////////////////////////////

