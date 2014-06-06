/*
*  me_ct_examples.cpp - part of SmartBody-lib
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

#include "controllers/me_ct_examples.h"
#include "sbm/gwiz_math.h"
using namespace gwiz;

#include <vhcl_log.h>

#include "sbm/sbm_constants.h"
#include <bml/bml.hpp>

///////////////////////////////////////////////////////////////////////////

/*
HEAD ORIENT JOINTS: { "spine4", "spine5", "skullbase" }
*/

#define USE_OLD_NOD 0

std::string MeCtHeadOrient::_type_name = "HeadOrient";

MeCtHeadOrient::MeCtHeadOrient( void )	{

	_duration = -1.0;
	_pitch_deg = 0.0;
	_heading_deg = 0.0;
	_roll_deg = 0.0;
}

MeCtHeadOrient::~MeCtHeadOrient( void )	{}

void MeCtHeadOrient::init( SbmPawn* pawn )	{
	char joint_labels[ 3 ][ MAX_JOINT_LABEL_LEN ] = {
		"spine4",
		"spine5",
		"skullbase"
	};
	int i;

	for( i = 0; i < 3; i++ )	{
		_channels.add( joint_labels[ i ], SkChannel::Quat );
	}

	MeController::init(pawn);
}

void MeCtHeadOrient::set_orient( float dur, float p, float h, float r )	{

	_duration = dur;
	_pitch_deg = p;
	_heading_deg = h;
	_roll_deg = r;
}

void MeCtHeadOrient::controller_start()	{}

bool MeCtHeadOrient::controller_evaluate( double t, MeFrameData& frame )	{

	if( _duration > 0.0 )	{
		if( t > (double)_duration )	{
			return( FALSE );
		}
	}

	SrBuffer<float>& buff = frame.buffer();
	int channels_size = _channels.size();
	float p_deg_per_joint = _pitch_deg / (float)channels_size;
	float h_deg_per_joint = _heading_deg / (float)channels_size;
	float r_deg_per_joint = _roll_deg / (float)channels_size;

	for( int i=0; i<channels_size; ++i ) {

		int index = frame.toBufferIndex( _toContextCh[ i ] );
		if (index == -1)
			continue;

		if (index == -1)
			continue;
		euler_t E_in = quat_t(
			buff[ index + 0 ],
			buff[ index + 1 ],
			buff[ index + 2 ],
			buff[ index + 3 ]
		);

		quat_t Q_out;
		if( frame.isChannelUpdated( i ) )	{
			// If channel has been touched, preserve components and add delta
			Q_out = euler_t(
				E_in.p() + p_deg_per_joint,
				E_in.h() + h_deg_per_joint,
				E_in.r() + r_deg_per_joint
				);
		}
		else	{
			// If channel has NOT been touched, set absolute values
			Q_out = euler_t(
				p_deg_per_joint,
				h_deg_per_joint,
				r_deg_per_joint
				);
		}

		buff[ index + 0 ] = (float) Q_out.w();
		buff[ index + 1 ] = (float) Q_out.x();
		buff[ index + 2 ] = (float) Q_out.y();
		buff[ index + 3 ] = (float) Q_out.z();

		// Mark channel changed
		frame.channelUpdated( i );
	}

	return( TRUE );
}

void MeCtHeadOrient::print_state( int tabs )	{
	LOG("MeCtSimpleTilt" );

	const char* str = getName().c_str();
	if( str )
		LOG(" \"%s\"", str );

	LOG(" p:%.3g h:%.3g r:%.3g degs for %.3g sec\n", 
		_pitch_deg, _heading_deg, _roll_deg, _duration );
}

///////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////

/*
TILTING JOINTS: { "spine4", "spine5", "skullbase" }
*/

std::string MeCtSimpleTilt::_type_name = "SimpleTilt";

MeCtSimpleTilt::MeCtSimpleTilt( void )	{

	_duration = -1.0;
	_angle_deg = 0.0;
}

MeCtSimpleTilt::~MeCtSimpleTilt( void )	{}

void MeCtSimpleTilt::init( SbmPawn* pawn )	{
	char joint_labels[ 3 ][ MAX_JOINT_LABEL_LEN ] = {
		"spine4",
		"spine5",
		"skullbase"
	};
	int i;

	for( i = 0; i < 3; i++ )	{
		_channels.add( joint_labels[ i ], SkChannel::Quat );
	}

	MeController::init(pawn);
}

void MeCtSimpleTilt::set_tilt( float dur, float angle_deg )	{

	_duration = dur;
	_angle_deg = angle_deg;
}

void MeCtSimpleTilt::controller_start()	{}

bool MeCtSimpleTilt::controller_evaluate( double t, MeFrameData& frame )	{

	if( _duration > 0.0 )	{
		if( t > (double)_duration )	{
			return( FALSE );
		}
	}

	SrBuffer<float>& buff = frame.buffer();
	int channels_size = _channels.size();
	float angle_deg_per_joint = _angle_deg / (float)channels_size;

	for( int i=0; i<channels_size; ++i ) {

		int index = frame.toBufferIndex( _toContextCh[ i ] );
		if (index == -1)
			continue;

		if (index == -1)
			continue;
		euler_t E_in = quat_t(
			buff[ index + 0 ],
			buff[ index + 1 ],
			buff[ index + 2 ],
			buff[ index + 3 ]
		);

		quat_t Q_out;
		if( frame.isChannelUpdated( i ) )	{
			// If channel has been touched, preserve components and add delta
			Q_out = euler_t(
				E_in.p(),
				E_in.h(),
				E_in.r() + angle_deg_per_joint
				);
		}
		else	{
			// If channel has NOT been touched, set absolute values
			Q_out = euler_t(
				0.0,
				0.0,
				angle_deg_per_joint
				);
		}

		buff[ index + 0 ] = (float) Q_out.w();
		buff[ index + 1 ] = (float) Q_out.x();
		buff[ index + 2 ] = (float) Q_out.y();
		buff[ index + 3 ] = (float) Q_out.z();

		// Mark channel changed
		frame.channelUpdated( i );
	}

	return( TRUE );
}

void MeCtSimpleTilt::print_state( int tabs )	{
	LOG("MeCtSimpleTilt" );

	const char* str = getName().c_str();
	if( str )
		LOG(" \"%s\"", str );

	LOG(" %.3g degs for %.3g sec\n", _angle_deg, _duration );
}

///////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////

/*
NODDING JOINTS: { "spine4", "spine5", "skullbase" }
*/

std::string MeCtSimpleNod::_type_name = "SimpleNod";

MeCtSimpleNod::MeCtSimpleNod( void )	{

	_duration = -1.0;
	_magnitude = 0.0;
	_repetitions = 0.0;
	_movementType = BML::HEAD_NOD;
}

MeCtSimpleNod::~MeCtSimpleNod( void )	{}

void MeCtSimpleNod::init( SbmPawn* pawn )	{
	char joint_labels[ 3 ][ MAX_JOINT_LABEL_LEN ] = {
		"spine4",
		"spine5",
		"skullbase"
	};
	int i;

	for( i = 0; i < 3; i++ )	{
		_channels.add( joint_labels[ i ], SkChannel::Quat );
	}

	_first_eval = true;

	MeController::init(pawn);
}

void MeCtSimpleNod::set_nod( float dur, float mag, float rep, int movementType, float smooth )	{

	_mode = NOD_SIMPLE;
	_duration = dur;
	_magnitude = mag;
	_repetitions = rep;
	_movementType = movementType;
	_smooth = smooth;
}

bool MeCtSimpleNod::isNod()
{
	return (_movementType == BML::HEAD_NOD && _mode == NOD_SIMPLE);
}

bool MeCtSimpleNod::isShake()
{
	return (_movementType == BML::HEAD_SHAKE && _mode == NOD_SIMPLE);
}

bool MeCtSimpleNod::isTilt()
{
	return (_movementType == BML::HEAD_TOSS && _mode == NOD_SIMPLE);
}

void MeCtSimpleNod::controller_start()	{}

float MeCtSimpleNod::calc_wiggle_curve( float t, float warp, float accel_pow )	{

	double a = warp * pow( (double)t, (double)accel_pow );	// sine acceleration curve
	double p = pow( (double)t, 2.0 * (double)accel_pow );	// decay power function
	if( p < 0.000000001 ) p = 0.000000001;

	double d = t / ( p * warp );			// decay envelope
	double v = ( ( 1.0 + cos( M_PI + a * M_PI ) ) * 0.5 ) * d;

	return( (float)v );
}

float MeCtSimpleNod::calc_waggle_curve( float t, float length, float pitch, float warp, float accel_pow, float decay_pow )	{

	double u = t / length;					// normalized traversal
	double a = warp * pow( (double)t, (double)accel_pow );	// sine acceleration curve

	double A = pow( u, (double)decay_pow ); 		// decay acceleration
	double d = sin( A * M_PI ) * 0.5;		// decay envelope

	double p = -M_PI * 0.5 * pitch; 		// pitch adjustment
	double v = ( sin( p + a * M_PI ) - sin( p ) ) * d;
	return( (float)v );
}

bool MeCtSimpleNod::controller_evaluate( double t, MeFrameData& frame )	{

	if( _duration > 0.0 )	{
		if( t > (double)_duration * 2.0) {
			return( false );
		}
	}

	float dt;
	if ( _first_eval ) {
		_first_eval = false;
		dt = 0.001f;
	}
	else	{
		dt = (float)( t - _prev_time );
	}
	_prev_time = t;

	float angle_deg = 0;
	if (t <= (double) _duration)
	{
		if( _mode == NOD_SIMPLE )	{
			float x = (float)( t / (double)_duration );
			angle_deg = (float)( -_magnitude * sin( x * 2.0 * M_PI * _repetitions ) );
		}
		else
			if( _mode == NOD_WIGGLE )	{

				float scale_time = (float)t / _period;
				angle_deg = -_magnitude * calc_wiggle_curve( scale_time, _warp, _accel_pow );
			}
			else
				if( _mode == NOD_WAGGLE )	{

					float scale_time = (float)t / _period;
					float scale_dur = _duration / _period;
					angle_deg =  -_magnitude * calc_waggle_curve( scale_time, scale_dur, _pitch, _warp, _accel_pow, _decay_pow );
				}
				else	{
					LOG( "MeCtSimpleNod::controller_evaluate ERR: mode %d not recognized" );
					return( false );
				}
	}

	SrBuffer<float>& buff = frame.buffer();

	// All of our channels are quats and recieve the same values
	int channels_size = _channels.size(); // Will be zero if init() errored
	float angle_deg_per_joint = angle_deg / (float)channels_size;

	float smooth_lerp = (float)(0.01 + ( 1.0 - powf( _smooth, dt * 30.0f /*SMOOTH_RATE_REF*/ ) ) * 0.99);

#if 0
	static double prev_dt = 0.1;
	printf( "sm:%f fps:%f dtsm:%f dif:%f\n", _smooth, 1.0/dt, smooth_lerp, dt - prev_dt );
	prev_dt = dt;
#endif

	//double deg_sum = 0.0;
	for( int local_channel_index=0;
		local_channel_index<channels_size;
		++local_channel_index )
	{

		// get buffer index
		int context_channel_index = _toContextCh[ local_channel_index ];
		int index = frame.toBufferIndex( context_channel_index );
		if (index == -1)
			continue;
		quat_t Q_in = quat_t(
			buff[ index + 0 ],
			buff[ index + 1 ],
			buff[ index + 2 ],
			buff[ index + 3 ]
		);
		euler_t E_in = Q_in;

		quat_t Q_out;
#if USE_OLD_NOD
		if (frame.isChannelUpdated( context_channel_index ) )	{
			// If channel has been touched, preserve components and add delta
			if( _affirmative )	{
				Q_out = euler_t(
					E_in.p() + angle_deg_per_joint,
					E_in.h(),
					E_in.r()
					);
			}
			else	{
				Q_out = euler_t(
					E_in.p(),
					E_in.h() + angle_deg_per_joint,
					E_in.r()
					);
			}
		}
		else	{
			// If channel has NOT been touched, set absolute values
			if( _affirmative )	{
				Q_out = euler_t(
					angle_deg_per_joint,
					0.0,
					0.0
					);
			}
			else	{
				Q_out = euler_t(
					0.0,
					angle_deg_per_joint,
					0.0
					);
			}
		}
#else
		const std::string& jointName = _channels.name(local_channel_index);
		SkJoint* channelJoint = frame.context()->channels().skeleton()->search_joint(jointName.c_str());
		SrVec rotAxis;
		if (_movementType == BML::HEAD_NOD )
		{
			 rotAxis = channelJoint->localGlobalAxis(0);
		}
		else if (_movementType == BML::HEAD_SHAKE )
		{
			rotAxis = channelJoint->localGlobalAxis(1);
		}
		else if (_movementType == BML::HEAD_TOSS )
		{
			rotAxis = channelJoint->localGlobalAxis(2);
		}
		if (frame.isChannelUpdated( context_channel_index ))
		{
			Q_out = Q_in*quat_t(angle_deg_per_joint,vector_t(rotAxis[0],rotAxis[1],rotAxis[2]));
		}
		else
		{
			Q_out = quat_t(angle_deg_per_joint,vector_t(rotAxis[0],rotAxis[1],rotAxis[2]));
		}
#endif

#if 1
		Q_out.lerp( 
			smooth_lerp, 
			Q_in
			);
#elif 0
		Q_out.lerp( 
			smooth_lerp, 
			Q_out,
			Q_in
			);
#elif 1
		Q_out.lerp( 
			smooth_lerp, 
			Q_in,
			Q_out
			);
#else
		Q_out = Q_in;
#endif

#if 0
		if( local_channel_index == 2 )	{
			static double prev_in = 0.0;
			static double prev_out = 0.0;
			double delt_dps = ( Q_out.degrees() - prev_out )/dt;
			if( abs( delt_dps ) > 3.0 )
				printf( "d-in:%f d-out:%f d-dps:%f out:%f\n", Q_in.degrees() - prev_in, Q_out.degrees() - prev_out, delt_dps, Q_out.degrees() );
			prev_in = Q_in.degrees();
			prev_out = Q_out.degrees();
		}
#endif

		buff[ index + 0 ] = (float) Q_out.w();
		buff[ index + 1 ] = (float) Q_out.x();
		buff[ index + 2 ] = (float) Q_out.y();
		buff[ index + 3 ] = (float) Q_out.z();

		// Mark channel changed
		frame.channelUpdated( context_channel_index );

		//euler_t E_out = Q_out;
		//deg_sum += E_out.p();
	}

	//printf( "mag: %f deg: %f out: %f\n", _magnitude, angle_deg, deg_sum );

	return( TRUE );
}

void MeCtSimpleNod::print_state( int tabs )	{
	LOG("MeCtSimpleNod" );

	const char* str = getName().c_str();
	if( str )
		LOG(" \"%s\"", str );

	if (_movementType == BML::HEAD_NOD)
		LOG(" affirmative" );
	else if (_movementType == BML::HEAD_SHAKE)
		LOG(" negative" );
	else if (_movementType == BML::HEAD_TOSS)
		LOG(" tilt" );

	LOG(" %.3g reps @ %.3g degs for %.3g sec\n", _repetitions, _magnitude, _duration );
}

///////////////////////////////////////////////////////////////////////////
