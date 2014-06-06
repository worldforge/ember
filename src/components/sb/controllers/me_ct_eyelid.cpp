/*
 *  me_ct_eyelid.cpp - part of SmartBody-lib
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

#include "sbm/gwiz_math.h"
using namespace gwiz;

#include "controllers/me_ct_eyelid.h"
#include <sb/sbm_pawn.hpp>
#include <sb/SBCharacter.h>
#include <sb/SBSkeleton.h>
#include <sb/SBJoint.h>
#include <vhcl_log.h>

//////////////////////////////////////////////////////////////////////////////////

std::string MeCtEyeLidRegulator::type_name = "EyeLidRegulator";

//////////////////////////////////////////////////////////////////////////////////

void MeCtEyeLidRegulator::LidSet::print( void )	{

	if( dirty_bit ) {
		update();
	}
	LOG( "base_angle : %f", base_angle );
	LOG( "full_angle : %f", full_angle );
	LOG( "close_angle: %f", close_angle );
	LOG( "diff       : %f: %f", diff, inv_diff );
	LOG( "lid_tight  : %f", lid_tight );
	LOG( "open_angle : %f", open_angle );
	LOG( "tight_sweep: %f", tight_sweep );
	LOG( "full_sweep : %f", close_sweep );
	LOG( "eye_pitch  : %f", eye_pitch );
}

void MeCtEyeLidRegulator::LidSet::set_range( float fr, float to )	{
		
	if( ( base_angle != fr )||( full_angle != to ) )	{
		base_angle = fr;
		full_angle = to;
		diff = full_angle - base_angle;
		if( fabs( diff ) > 0.0f )	{
			inv_diff = 1.0f / diff;
		}
		else	{
			inv_diff = 0.0f;
		}
		dirty_bit = true;
	}
}

void MeCtEyeLidRegulator::LidSet::get_range( float& fr, float& to )
{
	fr = base_angle;
	to = full_angle;
}

void MeCtEyeLidRegulator::LidSet::set_close( float angle )	{
	
	if( close_angle != angle )	{
		close_angle = angle;
		dirty_bit = true;
	}
}

float MeCtEyeLidRegulator::LidSet::get_close()
{
	return close_angle;
}

void MeCtEyeLidRegulator::LidSet::set_tighten( float tighten )	{
	
	if( lid_tight != tighten )	{
		lid_tight = tighten;
		if( lid_tight > 1.0f ) lid_tight = 1.0f;
		if( lid_tight < 0.0f ) lid_tight = 0.0f;
		dirty_bit = true;
	}
}


float MeCtEyeLidRegulator::LidSet::get_tighten( )
{
	return lid_tight;
}

void MeCtEyeLidRegulator::LidSet::set_pitch( float pitch )	{

	if( eye_pitch != pitch )	{
		eye_pitch = pitch;
		dirty_bit = true;
	}
}

float MeCtEyeLidRegulator::LidSet::get_pitch()
{
	return eye_pitch;
}

void MeCtEyeLidRegulator::LidSet::update( void )	{

	open_angle = base_angle * ( 1.0f - lid_tight ) + eye_pitch;
	tight_sweep = open_angle - base_angle;
	close_sweep = close_angle - open_angle;
	dirty_bit = false;
}

float MeCtEyeLidRegulator::LidSet::get_mapped_weight( float in_weight )	{

	if( dirty_bit ) {
		update();
	}
	float weight = ( tight_sweep + in_weight * close_sweep ) * inv_diff;
	if( weight < 0.0f ) return( 0.0f );
	if( weight > 1.0f ) return( 1.0f );
	return( weight );
}

void MeCtEyeLidRegulator::test( void )	{
	int i;

	LidSet lid;

	printf( "LidSet TEST:\n" );

	lid.set_range( -30.0f, 30.0f );
	lid.set_close( 30.0f );
	lid.set_tighten( 0.0f );
	lid.set_pitch( 0.0f );

	for( i=0; i<=10; i++ )	{
		float f = (float)i/10.0f;
		float w = lid.get_mapped_weight( f );
		printf( "[%d] f:%f w:%f\n", i, f, w );
	}

	lid.set_tighten( 0.5f );
	for( i=0; i<=10; i++ )	{
		float f = (float)i/10.0f;
		
		lid.set_range( -30.0f + f * 30.0f, 30.0f );
		
		float w = lid.get_mapped_weight( 0.5 );
		printf( "[%d] f:%f w:%f\n", i, f, w );
	}


#if 0
	lid.set_range( -30.0f, 30.0f );
//	lid.set_close( 30.0f );
//	lid.set_range( 30.0f, 0.0f );
	lid.set_close( 30.0f );
//	lid.set_pitch( -15.0f );
	lid.print();

	for( i=0; i<=10; i++ )	{
		float f = (float)i/10.0f;
		float w = lid.get_mapped_weight( f );
		printf( "[%d] f:%f w:%f\n", i, f, w );
	}

	lid.set_close( 0.0f );
//	lid.set_tighten( 0.5 );
	for( i=0; i<=10; i++ )	{
		float f = (float)i/10.0f;
		float w = lid.get_mapped_weight( f );
		printf( "[%d] f:%f w:%f\n", i, f, w );
	}

	lid.set_close( -30.0f );
//	lid.set_tighten( 1.0 );
	for( i=0; i<=10; i++ )	{
		float f = (float)i/10.0f;
		float w = lid.get_mapped_weight( f );
		printf( "[%d] f:%f w:%f\n", i, f, w );
	}
#endif

#if 0
//	lid.set_range( -50.0f, 0.0f );
	lid.set_close( 0.0f );
	lid.set_tighten( 0.0f );
	lid.print();

	for( i=0; i<=10; i++ )	{
		float f = (float)i/10.0f;
		float w = lid.get_mapped_weight( f );
		printf( "[%d] f:%f w:%f\n", i, f, w );
	}

	lid.set_tighten( 0.5 );
	for( i=0; i<=10; i++ )	{
		float f = (float)i/10.0f;
		float w = lid.get_mapped_weight( f );
		printf( "[%d] f:%f w:%f\n", i, f, w );
	}

	lid.set_tighten( 1.0 );
	for( i=0; i<=10; i++ )	{
		float f = (float)i/10.0f;
		float w = lid.get_mapped_weight( f );
		printf( "[%d] f:%f w:%f\n", i, f, w );
	}
#endif
}

//////////////////////////////////////////////////////////////////////////////////

MeCtEyeLidRegulator::MeCtEyeLidRegulator( void )	{
	set_use_blink_viseme(false);
	upper_lid_smooth = 0.9f;
	lower_lid_smooth = 0.9f;


	setDefaultAttributeGroupPriority("Eyelids", 410);
	// blinking settings
	addDefaultAttributeFloat("eyelid.blinkPeriodMin", 4.0f, "Eyelids");
	addDefaultAttributeFloat("eyelid.blinkPeriodMax", 8.0f, "Eyelids");

	// softeyes settings
	addDefaultAttributeBool("eyelid.softeyes", true, "Eyelids");
	addDefaultAttributeFloat("eyelid.rangeUpperMin", -30.0f, "Eyelids");
	addDefaultAttributeFloat("eyelid.rangeUpperMax", 30.0f, "Eyelids");
	addDefaultAttributeFloat("eyelid.tightWeightUpper", 0.f, "Eyelids");
	addDefaultAttributeFloat("eyelid.delayUpper", .3f, "Eyelids");
	addDefaultAttributeFloat("eyelid.closeAngle", 30.0f, "Eyelids");

}

MeCtEyeLidRegulator::~MeCtEyeLidRegulator( void )	{

}

void MeCtEyeLidRegulator::init(SbmPawn* pawn,  bool tracking_pitch)	{

	_channels.add( "eyeball_left", SkChannel::Quat );
	_channels.add( "eyeball_right", SkChannel::Quat );
	
	_channels.add( "au_45_left", SkChannel::XPos );
	_channels.add( "au_45_right", SkChannel::XPos );
	//_channels.add( "au_45", SkChannel::XPos );
	if (get_use_blink_viseme())
	{
		_channels.add( "blink", SkChannel::XPos );
	}

	MeController::init(pawn);


	SmartBody::SBCharacter* c = dynamic_cast<SmartBody::SBCharacter*> (pawn);
	if (c)
	{
		SmartBody::SBSkeleton* skel = c->getSkeleton();
		if (skel)
		{
			SmartBody::SBJoint* lEyeJoint = skel->getJointByMappedName("eyeball_left");
			SmartBody::SBJoint* rEyeJoint = skel->getJointByMappedName("eyeball_right");
			if (lEyeJoint && rEyeJoint)
			{
				_leftPreRot = lEyeJoint->quat()->prerot();
				_rightPreRot = rEyeJoint->quat()->prerot();
			}
		}
	}


	float upperMin = (float) pawn->getDoubleAttribute("eyelid.rangeUpperMin");
	float upperMax = (float) pawn->getDoubleAttribute("eyelid.rangeUpperMax");
	set_upper_range( upperMin, upperMax );
	set_lower_range( 20.0f, 20.0f ); // non existent...
	float closeAngle = (float) pawn->getDoubleAttribute("eyelid.closeAngle");
	set_close_angle( closeAngle );
	
	curve.insert( 0.0, 0.0 );
	curve.insert( 0.05, 1.0 );
	curve.insert( 0.2, 0.33 );
	curve.insert( 0.25, 0.0 );
	
	bool usePitchTracking = pawn->getBoolAttribute("eyelid.softeyes");
	pitch_tracking = usePitchTracking;
	
	prev_time = -1.0f;
	
	hard_upper_tighten = (float) pawn->getDoubleAttribute("eyelid.tightWeightUpper");
	hard_lower_tighten = 0.0f;
	soft_upper_tighten = 0.0f;
	soft_lower_tighten = 0.0f;
	
	new_blink = false;
	
	blink_period_min = (float) pawn->getDoubleAttribute("eyelid.blinkPeriodMin");
	blink_period_max = (float) pawn->getDoubleAttribute("eyelid.blinkPeriodMax");
	if (blink_period_min == 0.0)
		blink_period_min = 4.0f;
	if (blink_period_max == 0.0)
		blink_period_max = 8.0f;
	blink_period = blink_period_min;
	prev_blink = 0.0f;
	
	prev_UL_value = 0.0f;
	prev_LL_value = 0.0f;
	prev_UR_value = 0.0f;
	prev_LR_value = 0.0f;
	
	UL_value = 0.0f;
	LL_value = 0.0f;
	UR_value = 0.0f;
	LR_value = 0.0f;

	float upperDelay = (float) pawn->getDoubleAttribute("eyelid.delayUpper");
	upper_lid_delay = upperDelay;
	lower_lid_delay = .3f;

	
//	test();
}

void MeCtEyeLidRegulator::print( void )	{

	UL_set.print();
//	UR_set.print();
//	LL_set.print();
//	LR_set.print();
}

#define SMOOTH_RATE_REF (30.0f)

float MeCtEyeLidRegulator::smooth( float sm, double dt, float soft, float hard )	{

	if( sm > 0.0 )	{
		float s = 0.01f + ( 1.0f - powf( sm, (float)dt * SMOOTH_RATE_REF ) ) * 0.99f;
		return( ( 1.0f - s ) * soft + s * hard );
	}
	return( hard );
}

float MeCtEyeLidRegulator::granular( float in, float min, float max, int grains )	{
	
	float diff = ( max - min );
	float norm = ( in - min )/diff;
	int i = (int)( (float)grains * norm );
	return( (float)i * diff / (float)grains );
}

void MeCtEyeLidRegulator::context_updated( void ) {
}

void MeCtEyeLidRegulator::controller_map_updated( void ) {
}

void MeCtEyeLidRegulator::controller_start( void )	{
}

bool MeCtEyeLidRegulator::controller_evaluate( double t, MeFrameData& frame ) {
	double dt = 0.01f;
        
	if( t < 0.0 ) {
		return( true );
	}

	if( prev_time < 0.0 ) {
#if 0
		printf( "0.3333: %f\n", granular( 0.3333, 0.0, 10.0, 100 ) );
		printf( "0.1999: %f\n", granular( 0.1999, 0.0, 10.0, 100 ) );
		printf( "0.2500: %f\n", granular( 0.2500, 0.0, 10.0, 100 ) );
#endif
#if 0
		float s = 0.0;
		float h = 1.0;
		for( int i=0; i<30; i++ ) {
		s = smooth( 0.8, 0.1, s, h );
		printf( "%f\n", s );
	}
#endif
		prev_time = t;
	} else {
		dt = t - prev_time;
		prev_time = t;
	}

	if( new_blink ) {
		prev_blink = 0.0;
		new_blink = false;
	}

	double blink_elapsed = t - prev_blink;
	if( blink_elapsed >= blink_period ) {
		//LOG( "blink @ %f", blink_elapsed );
		blink_elapsed = 0.0;
		prev_blink = t;
		float r = (float)rand() / (float)RAND_MAX;
		blink_period = blink_period_min + r * ( blink_period_max - blink_period_min );
	}

	float *fbuffer = &( frame.buffer()[0] );
	int n_chan = _channels.size();

	if( pitch_tracking ) {
		int L_eye_quat_idx = _context->channels().search( "eyeball_left", SkChannel::Quat );
		int R_eye_quat_idx =  _context->channels().search( "eyeball_right", SkChannel::Quat );

		int buff_idx = _context->toBufferIndex( L_eye_quat_idx );
		euler_t L_eye_e;
		if (buff_idx >= 0)
		{
			quat_t L_eye_q = quat_t(
									fbuffer[ buff_idx ],
									fbuffer[ buff_idx + 1 ],
									fbuffer[ buff_idx + 2 ],
									fbuffer[ buff_idx + 3 ]
			);
			quat_t L_prerot = quat_t(_leftPreRot.w, _leftPreRot.x, _leftPreRot.y, _leftPreRot.z);
			L_eye_e = L_prerot * L_eye_q;
		}

		buff_idx = _context->toBufferIndex( R_eye_quat_idx );
		euler_t R_eye_e;
		if (buff_idx >= 0)
		{
			quat_t R_eye_q = quat_t(
									fbuffer[ buff_idx ],
									fbuffer[ buff_idx + 1 ],
									fbuffer[ buff_idx + 2 ],
									fbuffer[ buff_idx + 3 ]
			);
			quat_t R_prerot = quat_t(_rightPreRot.w, _rightPreRot.x, _rightPreRot.y, _rightPreRot.z);
			R_eye_e =  R_prerot * R_eye_q;
		}

		UL_set.set_pitch( (float)( L_eye_e.p() ) );
		UR_set.set_pitch( (float)( R_eye_e.p() ) );
	#if 0
		LL_set.set_pitch( (float)( L_eye_e.p() ) );
		LR_set.set_pitch( (float)( R_eye_e.p() ) );
	#endif
	}

	prev_UL_value = UL_value; // for change detection
	prev_LL_value = LL_value;
	prev_UR_value = UR_value;
	prev_LR_value = LR_value;

	float raw_lid_val = (float)( curve.evaluate( blink_elapsed ) );

	soft_upper_tighten = smooth( upper_lid_smooth, dt, soft_upper_tighten, hard_upper_tighten );
	gran_upper_tighten = granular( soft_upper_tighten, 0.0f, 1.0f, 100 );
#if 0
	soft_lower_tighten = smooth( lower_lid_smooth, dt, soft_lower_tighten, hard_lower_tighten );
	gran_lower_tighten = granular( soft_lower_tighten, 0.0f, 1.0f, 100 );
#endif

	UL_set.set_tighten( gran_upper_tighten );
	UR_set.set_tighten( gran_upper_tighten );
#if 0
	LL_set.set_tighten( gran_lower_tighten );
	LR_set.set_tighten( gran_lower_tighten );
#endif

//	UL_value = UL_set.get_mapped_weight( raw_lid_val );
//	UR_value = UR_set.get_mapped_weight( raw_lid_val );
	float hard_UL_val = UL_set.get_mapped_weight( raw_lid_val );
	float hard_UR_val = UR_set.get_mapped_weight( raw_lid_val );
	UL_value = smooth( upper_lid_delay, dt, UL_value, hard_UL_val );
	UR_value = smooth( upper_lid_delay, dt, UR_value, hard_UR_val );

#if 0
	float hard_LL_value = LL_set.get_mapped_weight( raw_lid_val );
	float hard_LR_value = LR_set.get_mapped_weight( raw_lid_val );
	LL_value = smooth( lower_lid_delay, dt, LL_value, hard_LL_val );
	LR_value = smooth( lower_lid_delay, dt, LR_value, hard_LR_val );
#endif

	bool applied = false;

	if (get_use_blink_viseme()) {
		int U_au_blink_idx = _context->channels().search("blink", SkChannel::XPos );
		int U_au_blink_buff_idx = _context->toBufferIndex( U_au_blink_idx );
		if( U_au_blink_idx >= 0 )	{
			fbuffer[ U_au_blink_buff_idx ] = UL_value;
		}
		applied = true;
	}

	/*
	// TODO: use safe threshold: abs( diff( a, b ) )
	if( ( UL_value != 0.0f ) && ( UL_value == UR_value ) ) {
		int U_au_blink_idx = _context->channels().search( SkJointName( "au_45" ), SkChannel::XPos );
		int U_au_blink_buff_idx = _context->toBufferIndex( U_au_blink_idx );
		if( U_au_blink_idx >= 0 )	{
			fbuffer[ U_au_blink_buff_idx ] = UL_value;
		} 
		applied = true;
	}
	*/

	if( applied == false ) {
		if( UL_value != 0.0f ) {
			int UL_au_blink_idx =  _context->channels().search("au_45_left", SkChannel::XPos );
			int UL_au_blink_buff_idx = _context->toBufferIndex( UL_au_blink_idx );
			if( UL_au_blink_buff_idx >= 0 ) {
				fbuffer[ UL_au_blink_buff_idx ] = UL_value;
			}
		}
		if( UR_value != 0.0f ) {
			int UR_au_blink_idx =  _context->channels().search( "au_45_right", SkChannel::XPos );
			int UR_au_blink_buff_idx = _context->toBufferIndex( UR_au_blink_idx );
			if( UR_au_blink_buff_idx >= 0 ) {
				fbuffer[ UR_au_blink_buff_idx ] = UR_value;
			}
		}
	}

#if 0
	int LL_au_blink_idx =  _context->channels().search( SkJointName( "xxxx" ), SkChannel::XPos );
	int LR_au_blink_idx =  _context->channels().search( SkJointName( "xxxx" ), SkChannel::XPos );

	int LL_au_blink_buff_idx = _context->toBufferIndex( LL_au_blink_idx );
	int LR_au_blink_buff_idx = _context->toBufferIndex( LR_au_blink_idx );

	if( LL_au_blink_buff_idx >= 0 ) {
	if( LL_value != 0.0f ) {
	fbuffer[ LL_au_blink_buff_idx ] = LL_value;
	}
	}
	if( LR_au_blink_buff_idx >= 0 ) {
	if( LR_value != 0.0f ) {
	fbuffer[ LR_au_blink_buff_idx ] = LR_value;
	}
	}
#endif
	return( true );
}

SkChannelArray& MeCtEyeLidRegulator::controller_channels( void )	{
	return( _channels );
}

double MeCtEyeLidRegulator::controller_duration( void ) {
	return( -1.0 );
}

const std::string& MeCtEyeLidRegulator::controller_type( void )	const {
	return( type_name );
}

void MeCtEyeLidRegulator::print_state( int tabCount ) {

	LOG("MeCtEyeLidRegulator" );

	const char* str = getName().c_str();
	if( str )
		LOG(" \"%s\"", str );

	LOG("\n" );
}

void MeCtEyeLidRegulator::set_use_blink_viseme(bool val)
{
	use_blink_viseme = val;
}

bool MeCtEyeLidRegulator::get_use_blink_viseme()
{
	return use_blink_viseme;
}

void MeCtEyeLidRegulator::notify(SBSubject* subject)
{
	SmartBody::SBAttribute* attribute = dynamic_cast<SmartBody::SBAttribute*>(subject);
	if (attribute)
	{
		const std::string& name = attribute->getName();
		if (name == "eyelid.blinkPeriodMin")
		{
			SmartBody::DoubleAttribute* minAttr = dynamic_cast<SmartBody::DoubleAttribute*>(attribute);
			blink_period_min = (float) minAttr->getValue();
			blink_period = blink_period_min;
		}
		else if (name == "eyelid.blinkPeriodMax")
		{
			SmartBody::DoubleAttribute* maxAttr = dynamic_cast<SmartBody::DoubleAttribute*>(attribute);
			blink_period_max = (float)  maxAttr->getValue();
		}
		else if (name == "eyelid.softeyes")
		{
			SmartBody::BoolAttribute* pitchAttr = dynamic_cast<SmartBody::BoolAttribute*>(attribute);
			set_eyeball_tracking(pitchAttr->getValue());
		}
		else if (name == "eyelid.rangeUpperMin")
		{
			SmartBody::DoubleAttribute* attr = dynamic_cast<SmartBody::DoubleAttribute*>(attribute);
			float min, max;
			UL_set.get_range(min, max);
			set_upper_range((float) attr->getValue(), max);
		}
		else if (name == "eyelid.rangeUpperMax")
		{
			SmartBody::DoubleAttribute* attr = dynamic_cast<SmartBody::DoubleAttribute*>(attribute);
			float min, max;
			UL_set.get_range(min, max);
			set_upper_range(min, (float) attr->getValue());
		}
		else if (name == "eyelid.tightWeightUpper")
		{
			SmartBody::DoubleAttribute* attr = dynamic_cast<SmartBody::DoubleAttribute*>(attribute);
			set_upper_tighten((float) attr->getValue());
		}
		else if (name == "eyelid.delayUpper")
		{
			SmartBody::DoubleAttribute* attr = dynamic_cast<SmartBody::DoubleAttribute*>(attribute);
			set_upper_delay((float) attr->getValue());
		}
		else if (name == "eyelid.closeAngle")
		{
			SmartBody::DoubleAttribute* attr = dynamic_cast<SmartBody::DoubleAttribute*>(attribute);
			set_close_angle((float) attr->getValue());
		}
	}
}

//////////////////////////////////////////////////////////////////////////////////

/*
	JOINTS:
	
		eyeball_left
		eyeball_right

		lower_eyelid_left
		upper_eyelid_left
		
		lower_eyelid_right
		upper_eyelid_right
*/

/*
	upper_eyelid_left
		offx = 3.15
		offy = 3.038
		offz = 11.75
	
	lookup
		posy = 0.372
		pitch = 0.0
	
	neutral
		posy = 0.0
		pitch = 0.0
	
	flat
		posy = -0.538
		pitch = 0.0

	lookdown
		posy = -0.788
		pitch = 0.0
	
	blink
		posy = -0.87
		pitch = 12.8
*/


#define DFL_EYEBALL_ROT_LIMIT_UP	-30.0f
#define DFL_EYEBALL_ROT_LIMIT_DN	35.0f

#define DFL_EYELID_UPPER_Y_LIMIT_UP	0.372f
#define DFL_EYELID_UPPER_Y_LIMIT_DN	-0.788f

#define DFL_EYELID_LOWER_Y_LIMIT_UP	0.2f
#define DFL_EYELID_LOWER_Y_LIMIT_DN	-0.2f

#define DFL_EYELID_UPPER_WEIGHT		1.0f
#define DFL_EYELID_LOWER_WEIGHT		0.2f


//////////////////////////////////////////////////////////////////////////////////

static bool G_debug = false;

std::string MeCtEyeLid::type_name = "EyeLid";

MeCtEyeLid::MeCtEyeLid( void )	{

	precision = 0.0001f; // ought to be enough, as long as you are not modeling in miles...
	
	set_weight( DFL_EYELID_LOWER_WEIGHT, DFL_EYELID_UPPER_WEIGHT );
	set_lower_lid_range( DFL_EYELID_LOWER_Y_LIMIT_DN, DFL_EYELID_LOWER_Y_LIMIT_UP );
	set_upper_lid_range( DFL_EYELID_UPPER_Y_LIMIT_DN, DFL_EYELID_UPPER_Y_LIMIT_UP );
	set_eye_pitch_range( DFL_EYEBALL_ROT_LIMIT_DN, DFL_EYEBALL_ROT_LIMIT_UP );
}

MeCtEyeLid::~MeCtEyeLid( void )	{
}

float MeCtEyeLid::calc_lid_correction( 
	float in_eye_p, float eye_range[ 2 ],
	float in_lid_y, float lid_range[ 2 ]
	)	{
	float adj_lid_y = 0.0;

	// first clamp eye-pitch to range...
	if( in_eye_p > eye_range[ 0 ] ) in_eye_p = eye_range[ 0 ]; // down is positive...
	if( in_eye_p < eye_range[ 1 ] ) in_eye_p = eye_range[ 1 ];

	// adjust for eye pitch
	if( in_eye_p > precision )	{ // looking down
		float eye_norm = in_eye_p / eye_range[ 0 ];
		adj_lid_y = eye_norm * lid_range[ 0 ];
	}
	else
	if( in_eye_p < -precision ) { // looking up
		float eye_norm = in_eye_p / eye_range[ 1 ];
		adj_lid_y = eye_norm * lid_range[ 1 ];
	}

	// then clamp lid to range...
	//  prevents normalization against 0.0
	if( in_lid_y < lid_range[ 0 ] ) in_lid_y = lid_range[ 0 ];
	if( in_lid_y > lid_range[ 1 ] ) in_lid_y = lid_range[ 1 ];

	// adjust for blink/lift
	float out_lid_y = adj_lid_y;
	if( in_lid_y < -precision )	{ // lid is lowering
		float blink_norm = in_lid_y / lid_range[ 0 ];
		out_lid_y = adj_lid_y - blink_norm * ( adj_lid_y - lid_range[ 0 ] );
	}
	else
	if( in_lid_y > precision )	{ // lid is lifting
		float blink_norm = in_lid_y / lid_range[ 1 ];
		out_lid_y = adj_lid_y + blink_norm * ( lid_range[ 1 ] - adj_lid_y );
	}

	if( G_debug ) LOG( "eye: %f  lid: %f  --> %f", in_eye_p, in_lid_y, out_lid_y );

	return( out_lid_y );
}

void MeCtEyeLid::init( SbmPawn* pawn ) {
	
	_channels.add( "eyeball_left", SkChannel::Quat );
	_channels.add( "eyeball_right", SkChannel::Quat );
	
	_channels.add( "upper_eyelid_left", SkChannel::YPos );
	_channels.add( "upper_eyelid_right", SkChannel::YPos );

	_channels.add( "lower_eyelid_left", SkChannel::YPos );
	_channels.add( "lower_eyelid_right", SkChannel::YPos );

	MeController::init(pawn);
}

///////////////////////////////////////////////////////////////////////////////////

void MeCtEyeLid::context_updated( void ) {
}

void MeCtEyeLid::controller_map_updated( void ) {

}

void MeCtEyeLid::controller_start( void )	{

//	if( _context->channels().size() > 0 )	{
//		_skeleton_ref_p = _context->channels().skeleton();
//	}
}

bool MeCtEyeLid::controller_evaluate( double t, MeFrameData& frame ) {

#if 0
	static int once = 1;
	if( once )	{
		once = 0;
		G_debug = 1;
		
		LOG( "eye: %f %f", eye_pitch_range[ 0 ], eye_pitch_range[ 1 ] );
		int i;
#if 1

		LOG( "UPPER:" );
		LOG( "lid: %f %f", upper_lid_range[ 0 ], upper_lid_range[ 1 ] );

		float up_lid_samps[ 8 ] = { -1.0f, -0.5f, -0.2f, -0.1f, 0.0f, 0.001f, 0.1f, 0.4f };

		LOG( "look up:" );
		for( i=0; i<8; i++ )	{
			calc_lid_correction( -30.0f, eye_pitch_range, up_lid_samps[ i ], upper_lid_range );
		}
		for( i=0; i<8; i++ )	{
			calc_lid_correction( -10.0f, eye_pitch_range, up_lid_samps[ i ], upper_lid_range );
		}
		LOG( "look fwd:" ); 
		for( i=0; i<8; i++ )	{
			calc_lid_correction( 0.0f, eye_pitch_range, up_lid_samps[ i ], upper_lid_range );
		}
		LOG( "look down:" );
		for( i=0; i<8; i++ )	{
			calc_lid_correction( 10.0f, eye_pitch_range, up_lid_samps[ i ], upper_lid_range );
		}
		for( i=0; i<8; i++ )	{
			calc_lid_correction( 30.0f, eye_pitch_range, up_lid_samps[ i ], upper_lid_range );
		}
#endif
#if 1
		LOG( "LOWER:" );
		LOG( "lid: %f %f", lower_lid_range[ 0 ], lower_lid_range[ 1 ] );

		float lo_lid_samps[ 8 ] = { -0.3f, -0.2f, -0.1f, 0.0f, 0.001f, 0.1f, 0.2f, 0.3f };

		LOG( "look up:" );
		for( i=0; i<8; i++ )	{
			calc_lid_correction( -30.0f, eye_pitch_range, lo_lid_samps[ i ], lower_lid_range );
		}
		for( i=0; i<8; i++ )	{
			calc_lid_correction( -10.0f, eye_pitch_range, lo_lid_samps[ i ], lower_lid_range );
		}
		LOG( "look fwd:" ); 
		for( i=0; i<8; i++ )	{
			calc_lid_correction( 0.0f, eye_pitch_range, lo_lid_samps[ i ], lower_lid_range );
		}
		LOG( "look down:" );
		for( i=0; i<8; i++ )	{
			calc_lid_correction( 10.0f, eye_pitch_range, lo_lid_samps[ i ], lower_lid_range );
		}
		for( i=0; i<8; i++ )	{
			calc_lid_correction( 30.0f, eye_pitch_range, lo_lid_samps[ i ], lower_lid_range );
		}
#endif
		G_debug = 0;
	}
#endif

	if( t < 0.0 )	{
		return( true );
	}

	float *fbuffer = &( frame.buffer()[0] );
	int n_chan = _channels.size();

	int L_eye_quat_chan_index = _context->channels().search( "eyeball_left", SkChannel::Quat );
//	int R_eye_quat_chan_index =  _context->channels().search( SkJointName( "eyeball_right" ), SkChannel::Quat );

	int UL_lid_posy_chan_index =  _context->channels().search("upper_eyelid_left", SkChannel::YPos );
	int UR_lid_posy_chan_index =  _context->channels().search("upper_eyelid_right", SkChannel::YPos );

	int LL_lid_posy_chan_index =  _context->channels().search( "lower_eyelid_left", SkChannel::YPos );
	int LR_lid_posy_chan_index =  _context->channels().search( "lower_eyelid_right" , SkChannel::YPos );

	int i_map;
	
	i_map = _context->toBufferIndex( L_eye_quat_chan_index );
	euler_t L_eye_e;
	if (i_map >= 0)
	{
		L_eye_e = quat_t(
			fbuffer[ i_map ],
			fbuffer[ i_map + 1 ],
			fbuffer[ i_map + 2 ],
			fbuffer[ i_map + 3 ]
		);
	}

	int UL_lid_y_map = _context->toBufferIndex( UL_lid_posy_chan_index );
	int UR_lid_y_map = _context->toBufferIndex( UR_lid_posy_chan_index );
#if 1
	
	
	if (UL_lid_y_map >= 0)
	{
		float UL_lid_y = fbuffer[ UL_lid_y_map ];

		float UL_correct_posy = lid_weight[ 1 ] * calc_lid_correction( 
			(float)( L_eye_e.p() ), 
			eye_pitch_range,
			UL_lid_y, 
			upper_lid_range
		);
		fbuffer[ UL_lid_y_map ] = UL_correct_posy;
	}
	
	if (UR_lid_y_map >= 0)
	{
		float UR_lid_y = fbuffer[ UR_lid_y_map ];
		float UR_correct_posy = lid_weight[ 1 ] * calc_lid_correction( 
			(float)( L_eye_e.p() ), 
			eye_pitch_range,
			UR_lid_y, 
			upper_lid_range
		);
		fbuffer[ UR_lid_y_map ] = UR_correct_posy;
	}
	
#else
	if (UL_lid_y_map >= 0)
		fbuffer[ UL_lid_y_map ] = 0.0;
	if (UR_lid_y_map >= 0)
		fbuffer[ UR_lid_y_map ] = 0.0;
#endif

	int LL_lid_y_map = _context->toBufferIndex( LL_lid_posy_chan_index );
	int LR_lid_y_map = _context->toBufferIndex( LR_lid_posy_chan_index );
#if 1

	if (LL_lid_y_map >= 0)
	{
		float LL_lid_y = fbuffer[ LL_lid_y_map ];
		float LL_correct_posy = lid_weight[ 0 ] * calc_lid_correction( 
			(float)( L_eye_e.p() ), 
			eye_pitch_range,
			LL_lid_y, 
			lower_lid_range
		);
		fbuffer[ LL_lid_y_map ] = LL_correct_posy;
	}

	if (LR_lid_y_map >= 0)
	{
		float LR_lid_y = fbuffer[ LR_lid_y_map ];
		float LR_correct_posy = lid_weight[ 0 ] * calc_lid_correction( 
			(float)( L_eye_e.p() ), 
			eye_pitch_range,
			LR_lid_y, 
			lower_lid_range
		);
		fbuffer[ LR_lid_y_map ] = LR_correct_posy;
	}
	
#else
	if (LL_lid_y_map >= 0)
		fbuffer[ LL_lid_y_map ] = 0.0;
	if (LR_lid_y_map >= 0)
		fbuffer[ LR_lid_y_map ] = 0.0;
#endif

	return true;
}

SkChannelArray& MeCtEyeLid::controller_channels( void )	{
	return( _channels );
}

double MeCtEyeLid::controller_duration( void ) {
	return( -1.0 );
}

const std::string& MeCtEyeLid::controller_type( void )	const {
	return( type_name );
}

void MeCtEyeLid::print_state( int tabCount ) {

	LOG("MeCtEyeLid" );

	const char* str = getName().c_str();
	if( str )
		LOG(" \"%s\"", str );

	LOG("\n" );
}


