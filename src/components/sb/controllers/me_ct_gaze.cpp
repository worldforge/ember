/*
 *  me_ct_gaze.cpp - part of SmartBody-lib
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
 */


#include "controllers/me_ct_gaze.h"
#include "sbm/lin_win.h"
using namespace gwiz;
#include <vhcl_log.h>
#include <sb/SBSimulationManager.h>
#include <sb/SBScene.h>
#include <sb/SBPawn.h>
#include <sb/SBSkeleton.h>
#include <sb/sbm_pawn.hpp>

//#define DFL_GAZE_HEAD_SPEED 180.0
//#define DFL_GAZE_EYE_SPEED  1000.0

#if GAZE_KEY_COMBINE_HEAD_AND_NECK

// Default Values per Gaze Key:                LUMBAR,   THORAX, CERVICAL, OPTICAL
float MeCtGaze::DEFAULT_LIMIT_PITCH_UP[]   = { -15.0,    -6.0,   -45.0,    -35.0    };
float MeCtGaze::DEFAULT_LIMIT_PITCH_DOWN[] = { 15.0,     6.0,    45.0,     35.0   };
float MeCtGaze::DEFAULT_LIMIT_HEADING[]    = { 30.0,     15.0,   90.0,     40.0    };
float MeCtGaze::DEFAULT_LIMIT_ROLL[]       = { 10.0,     5.0,    35.0,     0.0     };
#else

// Default Values per Gaze Key:                LUMBAR,   THORAX, CERVICAL, CRANIAL,  OPTICAL
float MeCtGaze::DEFAULT_LIMIT_PITCH_UP[]   = { -15.0,    -6.0,   -25.0,    -20.0,    -35.0    };
float MeCtGaze::DEFAULT_LIMIT_PITCH_DOWN[] = { 15.0,     6.0,    25.0,     20.0,     35.0   };
float MeCtGaze::DEFAULT_LIMIT_HEADING[]    = { 30.0,     15.0,   60.0,     45.0,     40.0    };
float MeCtGaze::DEFAULT_LIMIT_ROLL[]       = { 10.0,     5.0,    20.0,     15.0,     0.0     };
#endif

// Defaults for the Old APIs
// History of Values:
//   Original Marcus Implementation:
//     Speed     (back, neck, eyes): 1000, 1500, 2000
//     Smoothing (back, neck, eyes): 0.8, 0.8, 0.1
//   Original BML Defaults:
//     Speed     (back, neck, eyes): 1000, 1500, 2000
//     Smoothing (back, neck, eyes): 0.8, 0.8, 0.1
//   Gaze Key Revision (June2007):
//     Speed     (head, eyes, eye arg default): 180, 1000, 10000
//     Smoothing (back, neck, eyes): 0.3, 0.1, 0.0
//   Brent's Edits / Gaze Paper Update (Oct2008):
//     Speed     (head, eyes, default eyes arg): 180, 1000, 10000
//     Smoothing (back, neck, eyes): 0.3, 0.1, 0.0
//   Andrew's Updates, equiv to old BML defaults (Feb2009):
//     Speed     (head, eyes, default eyes arg): 360, 10000, 10000
//     Smoothing (back, neck, eyes): 0.8, 0.8, 0.1

const float MeCtGaze::DEFAULT_SPEED_HEAD         = 5000.0f;
const float MeCtGaze::DEFAULT_SPEED_EYES         = 5000.0f;

const float MeCtGaze::DEFAULT_SMOOTHING_LUMBAR   = 0.6f;
const float MeCtGaze::DEFAULT_SMOOTHING_CERVICAL = 0.5f;
const float MeCtGaze::DEFAULT_SMOOTHING_EYEBALL  = 0.0f;

#if TEST_SENSOR
#include "controllers/me_ct_gaze_sensor.h"
void test_sensor_callback( int id, int status )	{
	LOG( "test_sensor_callback: id:%d status:%d\n", id, status );
}
#endif

#if ENABLE_HACK_TARGET_CIRCLE
int G_hack_target_circle = 0;
float G_hack_target_heading = 0.0;
#endif

///////////////////////////////////////////////////////////////////////////

/*
	GAZING HIERARCHY:    { "spine1", "spine2", "spine3", "spine4", "spine5", "skullbase", "face_top_parent", "eyeball_left", "eyeball_right" }
	FULL GAZING JOINTS:  { "spine1", "spine2", "spine3", "spine4", "spine5", "skullbase", "eyeball_left", "eyeball_right" }

	BACK: { "spine1", "spine2", "spine3" }
	NECK: { "spine4", "spine5", "skullbase" }
	EYES: { "eyeball_left", "eyeball_right" }

---

	LUMBAR: 	{ "spine1", "spine2" }
	THORAX: 	{ "spine3" }
		CERVICAL:	{ "spine4", "spine5", "skullbase" }
	CERVICAL:	{ "spine4", "spine5" }
	HEAD:		{ "skullbase" }
	EYES:		{ "eyeball_left", "eyeball_right" }
*/

/*
	height: segment length
		spine1: 7.8 		7.8
		spine2: 17.7		9.9
		spine3: 28.9		11.2
		spine4: 56.0		27.1
		spine5: 59.7		3.7
		skullbase: 64.7 	5.0
		eyeball: 70.6		5.9
*/

std::string MeCtGaze::CONTROLLER_TYPE = "Gaze";

int MeCtGaze::joint_index( const char *label )	{
	if( label )	{
		if( _stricmp( label, "spine1" ) == 0 )        return( GAZE_JOINT_SPINE1 );
		if( _stricmp( label, "spine2" ) == 0 )        return( GAZE_JOINT_SPINE2 );
		if( _stricmp( label, "spine3" ) == 0 )        return( GAZE_JOINT_SPINE3 );
		if( _stricmp( label, "spine4" ) == 0 )        return( GAZE_JOINT_SPINE4 );
		if( _stricmp( label, "spine5" ) == 0 )        return( GAZE_JOINT_SPINE5 );
		if( _stricmp( label, "skullbase" ) == 0 )     return( GAZE_JOINT_SKULL );
		if( _stricmp( label, "eyeball_left" ) == 0 )  return( GAZE_JOINT_EYE_L );
		if( _stricmp( label, "eyeball_right" ) == 0 ) return( GAZE_JOINT_EYE_R );
	}
	return( -1 ); // default err
}

char * MeCtGaze::joint_label( const int index ) {
	switch( index )	{
		case GAZE_JOINT_SPINE1: return( (char*)"spine1" );
		case GAZE_JOINT_SPINE2: return( (char*)"spine2" );
		case GAZE_JOINT_SPINE3: return( (char*)"spine3" );
		case GAZE_JOINT_SPINE4: return( (char*)"spine4" );
		case GAZE_JOINT_SPINE5: return( (char*)"spine5" );
		case GAZE_JOINT_SKULL:  return( (char*)"skullbase" );
		case GAZE_JOINT_EYE_L:  return( (char*)"eyeball_left" );
		case GAZE_JOINT_EYE_R:  return( (char*)"eyeball_right" );
	}
	return( (char*)"UNKNOWN" ); // default err
}

int MeCtGaze::key_index( const char *label )	{
	if( label )	{
		if( _stricmp( label, "lumbar" ) == 0 )   return( GAZE_KEY_BACK );
		if( _stricmp( label, "thorax" ) == 0 )   return( GAZE_KEY_CHEST );
		if( _stricmp( label, "cervical" ) == 0 ) return( GAZE_KEY_NECK );
		if( _stricmp( label, "cranial" ) == 0 )  return( GAZE_KEY_HEAD );
		if( _stricmp( label, "optical" ) == 0 )  return( GAZE_KEY_EYES );
		if( _stricmp( label, "back" ) == 0 )     return( GAZE_KEY_BACK );
		if( _stricmp( label, "chest" ) == 0 )    return( GAZE_KEY_CHEST );
		if( _stricmp( label, "neck" ) == 0 )     return( GAZE_KEY_NECK );
		if( _stricmp( label, "head" ) == 0 )     return( GAZE_KEY_HEAD );
		if( _stricmp( label, "eyes" ) == 0 )     return( GAZE_KEY_EYES );
	}
	return( -1 ); // default err
}

char * MeCtGaze::key_label( const int key )	{
	switch( key )	{
		case GAZE_KEY_BACK: 	return( (char*)"BACK" );
		case GAZE_KEY_CHEST:	return( (char*)"CHEST" );
		case GAZE_KEY_NECK: 	return( (char*)"NECK" );
#if GAZE_KEY_COMBINE_HEAD_AND_NECK
#else
		case GAZE_KEY_HEAD:		return((char*) "HEAD" );
#endif
		case GAZE_KEY_EYES: 	return( (char*)"EYES" );
	}
	return( (char*)"UNKNOWN" ); // default err
}

///////////////////////////////////////////////////////////////////////////

MeCtGaze::MeCtGaze( void )	: SmartBody::SBController() {
	
	started = 0;
	setStart(0);
	prev_time = 0.0;

	priority_joint = GAZE_JOINT_EYE_L;
	target_mode = TARGET_POINT;	
	offset_mode = OFFSET_POINT;
	offset_coord = OFFSET_WORLD;
	
	_duration = -1.0f;

	skeleton_ref_p = NULL;

	target_ref_joint_str = NULL;
	target_ref_joint_p = NULL;
	offset_ref_joint_str = NULL;
	offset_ref_joint_p = NULL;
	ref_joint_str = NULL;
	ref_joint_p = NULL;
	
	timing_mode = TASK_SPEED;
	head_speed = 1.0;
	head_time = 1.0;
	fade_interval = 0.0;
	fade_remaining = 0.0;
	fading_normal = 1.0;
	fading_complete = false;
	fading_mode = FADING_MODE_OFF;
	
	scheduled_time = 0.0f;
	scheduled_fade_mode = FADING_MODE_OFF;
	scheduled_fade_interval = 0.0f;

	joint_key_count = 0;
	joint_key_map = NULL;
	joint_key_top_map = NULL;
	joint_key_arr = NULL;
	
	key_smooth_dirty = 0;
	key_bias_dirty = 0;
	key_limit_dirty = 0;
	key_blend_dirty = 0;
	
	joint_count = 0;
	joint_arr = NULL;

	// gaze settings
	setDefaultAttributeGroupPriority("Gaze", 400);
	addDefaultAttributeDouble("gaze.speedEyes", 5000, "Gaze");
	addDefaultAttributeDouble("gaze.speedNeck", 5000, "Gaze");
	addDefaultAttributeDouble("gaze.limitPitchUpEyes", -35.0, "Gaze");
	addDefaultAttributeDouble("gaze.limitPitchDownEyes",35.0, "Gaze");
	addDefaultAttributeDouble("gaze.limitHeadingEyes", 40.0, "Gaze");
	addDefaultAttributeDouble("gaze.limitRollEyes",  0.0, "Gaze");
	addDefaultAttributeDouble("gaze.limitPitchUpNeck", -45.0, "Gaze");
	addDefaultAttributeDouble("gaze.limitPitchDownNeck", 45.0, "Gaze");
	addDefaultAttributeDouble("gaze.limitHeadingNeck",  90.0, "Gaze");
	addDefaultAttributeDouble("gaze.limitRollNeck", 35.0, "Gaze");
	addDefaultAttributeDouble("gaze.limitPitchUpChest", -6.0, "Gaze");
	addDefaultAttributeDouble("gaze.limitPitchDownChest",  6.0, "Gaze");
	addDefaultAttributeDouble("gaze.limitHeadingChest", 15.0, "Gaze");
	addDefaultAttributeDouble("gaze.limitRollChest", 5.0, "Gaze");
	addDefaultAttributeDouble("gaze.limitPitchUpBack", -15.0, "Gaze");
	addDefaultAttributeDouble("gaze.limitPitchDownBack", 15.0, "Gaze");
	addDefaultAttributeDouble("gaze.limitHeadingBack", 30.0, "Gaze");
	addDefaultAttributeDouble("gaze.limitRollBack", 10.0, "Gaze");
}

MeCtGaze::~MeCtGaze( void )	{
	
	if( joint_key_map )	{
		delete [] joint_key_map;
		joint_key_map = NULL;
	}
	if( joint_key_top_map )	{
		delete [] joint_key_top_map;
		joint_key_top_map = NULL;
	}
	if( joint_key_arr )	{
		delete [] joint_key_arr;
		joint_key_arr = NULL;
	}
	if( joint_arr )	{
		delete [] joint_arr;
		joint_arr = NULL;
	}
	if( target_ref_joint_str ) {
		free( target_ref_joint_str );
		target_ref_joint_str = NULL;
	}
	if( offset_ref_joint_str ) {
		free( offset_ref_joint_str );
		offset_ref_joint_str = NULL;
	}
	if( ref_joint_str ) {
		free( ref_joint_str );
		ref_joint_str = NULL;
	}
}

void MeCtGaze::init( SbmPawn* pawn, int key_fr, int key_to )	{
	int i;
	
	joint_key_count = NUM_GAZE_KEYS;

	joint_key_map = new int[ joint_key_count ];
	joint_key_map[ GAZE_KEY_BACK ] = GAZE_JOINT_SPINE1;
	joint_key_map[ GAZE_KEY_CHEST ] = GAZE_JOINT_SPINE3;
	joint_key_map[ GAZE_KEY_NECK ] = GAZE_JOINT_SPINE4;
#if GAZE_KEY_COMBINE_HEAD_AND_NECK
#else
	joint_key_map[ GAZE_KEY_HEAD ] = GAZE_JOINT_SKULL;
#endif
	joint_key_map[ GAZE_KEY_EYES ] = GAZE_JOINT_EYE_L;

	joint_key_top_map = new int[ joint_key_count ];
	joint_key_top_map[ GAZE_KEY_BACK ] = GAZE_JOINT_SPINE2;
	joint_key_top_map[ GAZE_KEY_CHEST ] = GAZE_JOINT_SPINE3;
#if GAZE_KEY_COMBINE_HEAD_AND_NECK
	joint_key_top_map[ GAZE_KEY_NECK ] = GAZE_JOINT_SKULL;
#else
	joint_key_top_map[ GAZE_KEY_NECK ] = GAZE_JOINT_SPINE5;
	joint_key_top_map[ GAZE_KEY_HEAD ] = GAZE_JOINT_SKULL;
#endif
	joint_key_top_map[ GAZE_KEY_EYES ] = GAZE_JOINT_EYE_L;

	joint_key_arr = new MeCtGazeKey[ joint_key_count ];
	for( i = 0; i < NUM_GAZE_KEYS; i++ )	{
		joint_key_arr[ i ].id = i;
	}

	joint_count = NUM_GAZE_JOINTS;
	joint_arr = new MeCtGazeJoint[ joint_count ];
	for( i = 0; i < NUM_GAZE_JOINTS; i++ )	{
		joint_arr[ i ].id = i;
	}
	
	// Sort key range:
	if( key_fr < 0 )	{
		key_fr = GAZE_KEY_BACK;
	}
	if( key_fr >= NUM_GAZE_KEYS )	{
		key_fr = GAZE_KEY_EYES;
	}
	if( key_to < 0 )	{
		key_to = GAZE_KEY_EYES;
	}
	if( key_to >= NUM_GAZE_KEYS )	{
		key_to = GAZE_KEY_EYES;
	}
	if( key_fr > key_to )	{
		int tmp = key_fr; key_fr = key_to; key_to = tmp;
	}

	key_min = key_fr;
	key_max = key_to;

	// Activate joints based on key range
	int joint_fr = joint_key_map[ key_fr ];
	int joint_to = joint_key_map[ key_to ];

	if (key_to == GAZE_KEY_EYES)
	{
		// no eye ball joints, select neck instead
		if (!pawn->get_joint(joint_label(GAZE_JOINT_EYE_R)) && !pawn->get_joint(joint_label(GAZE_JOINT_EYE_L)))
			joint_to = GAZE_JOINT_SKULL;
	}

	for( i = joint_fr; i <= joint_to; i++ )	{
		joint_arr[ i ].active = 1;
	}
	joint_arr[ GAZE_JOINT_EYE_R ].active = joint_arr[ GAZE_JOINT_EYE_L ].active;
	
	// Add names to channel array
	for( i = 0; i < joint_count; i++ )	{
		_channels.add( joint_label( i ), SkChannel::Quat );
	}
	
	set_task_priority( key_max );
	//set_speed( DFL_GAZE_HEAD_SPEED, DFL_GAZE_EYE_SPEED ); // initializes timing_mode = TASK_SPEED;
	
	initialEyeSpeed = DEFAULT_SPEED_EYES;
	initialNeckSpeed = DEFAULT_SPEED_HEAD;

	if (pawn->getAttribute("gaze.speedEyes") != NULL)
		initialEyeSpeed = (float) pawn->getDoubleAttribute("gaze.speedEyes");
	if (pawn->getAttribute("gaze.speedNeck") != NULL)
		initialNeckSpeed = (float) pawn->getDoubleAttribute("gaze.speedNeck");

	set_speed( initialNeckSpeed, initialEyeSpeed );

	//set_smooth( 0.3f, 0.1f, 0.0f );
	set_smooth( DEFAULT_SMOOTHING_LUMBAR, DEFAULT_SMOOTHING_CERVICAL, DEFAULT_SMOOTHING_EYEBALL );
	/*
	set_limit( GAZE_KEY_LUMBAR,   15.0, 30.0, 10.0 );
	set_limit( GAZE_KEY_THORAX,   6.0,  15.0, 5.0 );
	set_limit( GAZE_KEY_CERVICAL, 25.0, 60.0, 20.0 );
	set_limit( GAZE_KEY_HEAD,     20.0, 45.0, 15.0 );
	set_limit( GAZE_KEY_EYES,     50.0, 75.0, 0.0 );
	*/
	float limitPitchUp, limitPitchDown, limitHeading, limitRoll;

	if (pawn->getAttribute("gaze.limitPitchUpBack") != NULL)
		limitPitchUp = (float) pawn->getDoubleAttribute("gaze.limitPitchUpBack");
	else
		limitPitchUp = DEFAULT_LIMIT_PITCH_UP[GAZE_KEY_BACK];
	if (pawn->getAttribute("gaze.limitPitchDownBack") != NULL)
		limitPitchDown = (float) pawn->getDoubleAttribute("gaze.limitPitchDownBack");
	else
		limitPitchDown = DEFAULT_LIMIT_PITCH_DOWN[GAZE_KEY_BACK];
	if (pawn->getAttribute("gaze.limitHeadingBack") != NULL)
		limitHeading = (float) pawn->getDoubleAttribute("gaze.limitHeadingBack");
	else
		limitHeading = DEFAULT_LIMIT_HEADING[GAZE_KEY_BACK];
	if (pawn->getAttribute("gaze.limitRollBack") != NULL)
		limitRoll = (float) pawn->getDoubleAttribute("gaze.limitRollBack");
	else
		limitRoll = DEFAULT_LIMIT_ROLL[GAZE_KEY_BACK];

	set_limit( GAZE_KEY_BACK,   limitPitchUp,
								limitPitchDown,
								limitHeading,
								limitRoll);

	
	if (pawn->getAttribute("gaze.limitPitchUpChest") != NULL)
		limitPitchUp = (float) pawn->getDoubleAttribute("gaze.limitPitchUpChest");
	else
		limitPitchUp = DEFAULT_LIMIT_PITCH_UP[GAZE_KEY_CHEST];
	if (pawn->getAttribute("gaze.limitPitchDownChest") != NULL)
		limitPitchDown = (float) pawn->getDoubleAttribute("gaze.limitPitchDownChest");
	else
		limitPitchDown = DEFAULT_LIMIT_PITCH_DOWN[GAZE_KEY_CHEST];
	if (pawn->getAttribute("gaze.limitHeadingChest") != NULL)
		limitHeading = (float) pawn->getDoubleAttribute("gaze.limitHeadingChest");
	else
		limitHeading = DEFAULT_LIMIT_HEADING[GAZE_KEY_CHEST];
	if (pawn->getAttribute("gaze.limitRollChest") != NULL)
		limitRoll = (float) pawn->getDoubleAttribute("gaze.limitRollChest");
	else
		limitRoll = DEFAULT_LIMIT_ROLL[GAZE_KEY_CHEST];

	set_limit( GAZE_KEY_CHEST,  limitPitchUp,
								limitPitchDown,
								limitHeading,
								limitRoll);

	

	if (pawn->getAttribute("gaze.limitPitchUpNeck") != NULL)
		limitPitchUp = (float) pawn->getDoubleAttribute("gaze.limitPitchUpNeck");
	else
		limitPitchUp = DEFAULT_LIMIT_PITCH_UP[GAZE_KEY_NECK];
	if (pawn->getAttribute("gaze.limitPitchDownNeck") != NULL)
		limitPitchDown = (float) pawn->getDoubleAttribute("gaze.limitPitchDownNeck");
	else
		limitPitchDown = DEFAULT_LIMIT_PITCH_DOWN[GAZE_KEY_NECK];
	if (pawn->getAttribute("gaze.limitHeadingNeck") != NULL)
		limitHeading = (float) pawn->getDoubleAttribute("gaze.limitHeadingNeck");
	else
		limitHeading = DEFAULT_LIMIT_HEADING[GAZE_KEY_NECK];
	if (pawn->getAttribute("gaze.limitRollNeck") != NULL)
		limitRoll = (float) pawn->getDoubleAttribute("gaze.limitRollNeck");
	else
		limitRoll = DEFAULT_LIMIT_ROLL[GAZE_KEY_NECK];

	set_limit( GAZE_KEY_NECK,   limitPitchUp,
								limitPitchDown,
								limitHeading,
								limitRoll);

#if GAZE_KEY_COMBINE_HEAD_AND_NECK
#else
	set_limit( GAZE_KEY_HEAD,  DEFAULT_LIMIT_PITCH_UP[GAZE_KEY_HEAD],
								  DEFAULT_LIMIT_PITCH_DOWN[GAZE_KEY_HEAD],
								  DEFAULT_LIMIT_HEADING[GAZE_KEY_HEAD],
								  DEFAULT_LIMIT_ROLL[GAZE_KEY_HEAD]);
#endif

	
	if (pawn->getAttribute("gaze.limitPitchUpEyes") != NULL)
		limitPitchUp = (float) pawn->getDoubleAttribute("gaze.limitPitchUpEyes");
	else
		limitPitchUp = DEFAULT_LIMIT_PITCH_UP[GAZE_KEY_EYES];
	if (pawn->getAttribute("gaze.limitPitchDownEyes") != NULL)
		limitPitchDown = (float) pawn->getDoubleAttribute("gaze.limitPitchDownEyes");
	else
		limitPitchDown = DEFAULT_LIMIT_PITCH_DOWN[GAZE_KEY_EYES];
	if (pawn->getAttribute("gaze.limitHeadingEyes") != NULL)
		limitHeading = (float) pawn->getDoubleAttribute("gaze.limitHeadingEyes");
	else
		limitHeading = DEFAULT_LIMIT_HEADING[GAZE_KEY_EYES];
	if (pawn->getAttribute("gaze.limitRollEyes") != NULL)
		limitRoll = (float) pawn->getDoubleAttribute("gaze.limitRollEyes");
	else
		limitRoll = DEFAULT_LIMIT_ROLL[GAZE_KEY_EYES];

	set_limit( GAZE_KEY_EYES,   limitPitchUp,
								limitPitchDown,
								limitHeading,
								limitRoll);

	for( i = 0; i < NUM_GAZE_KEYS; i++ )	{
		set_bias( i, 0.0f, 0.0f, 0.0f );
		set_blend( i, 1.0f );
	}

	MeController::init(pawn);

#if TEST_SENSOR
	sensor_p = new MeCtGazeSensor;
	sensor_p->init(
		1.0,
		10.0,
		99,
		this,
		test_sensor_callback
	);
#endif
}

void MeCtGaze::set_task_priority( int key )	{

	priority_joint = joint_key_map[ key ];
//	priority_joint = joint_key_top_map[ key ];
}

///////////////////////////////////////////////////////////////////////////

bool MeCtGaze::is_covered( MeBlendCoverMap & map, float lim )	{
	bool covered = true;
	int i;
	
	for( i=0; i<NUM_GAZE_KEYS; i++ )	{
		if( 
			( get_blend( i ) >= ( 1.0 - lim ) )&&
			( map.read( key_label( i ) ) < lim ) 
		)	{
			covered = false;
		}
	}
	return( covered );
}

void MeCtGaze::apply_coverage( MeBlendCoverMap & map ) {
	int i;
	
	for( i=0; i<NUM_GAZE_KEYS; i++ )	{
		map.update( key_label( i ), get_blend( i ) );
	}
}

bool MeCtGaze::check_and_apply_coverage( MeBlendCoverMap & map, float lim )	{
	bool covered = true;
	int i;
	
	for( i=0; i<NUM_GAZE_KEYS; i++ )	{
		const char *label = key_label( i );
		float this_blend = get_blend( i );
		if( 
			( this_blend >= ( 1.0 - lim ) )&&
			( map.read( label ) < lim ) 
		)	{
			covered = false;
			map.update( label, this_blend );
		}
	}
	return( covered );
}

bool MeCtGaze::calc_real_angle_to_target( float& get_deg )	{

#if 0
static int once = 1;
if( once )	{
once = 0;
vector_t a( 0.0, 0.0, 1.0 ), b( 1.0, 0.0, 1.0 );
float deg = DEG( acos( a.normal().dot( b.normal() ) ) );
LOG( "test dot: %f deg\n", deg );
LOG( "CALC: joint: %s\n", joint_label( priority_joint ) );
}
#endif

	if( !skeleton_ref_p ) return( false );
	
	char *joint_str = joint_label( priority_joint );
	SkJoint* joint_p = skeleton_ref_p->search_joint( joint_str );
	if( !joint_p ) {
		LOG("MeCtGaze::calc_real_angle_to_target ERR: joint '%s' NOT FOUND in skeleton\n", joint_str );
		return( false );
	}
	joint_p->update_gmat_up();

	SrMat sr_M;
	gwiz::matrix_t M;
	int i, j;
	
	sr_M = joint_p->gmat();
	for( i=0; i<4; i++ )	{
		for( j=0; j<4; j++ )	{
			M.set( i, j, sr_M.get( i, j ) );
		}
	}
	vector_t w_joint_pos = M.translation( gwiz::COMP_M_TR );
	quat_t w_joint_rot = M.quat( gwiz::COMP_M_TR );

	if( target_mode == TARGET_POINT )	{
		vector_t w_tgt_pos = world_target_point();

#if 1
	// Assume head/eye gaze, no bias
	
		vector_t tgt_dir = ( w_tgt_pos - w_joint_pos ).normalize();
		vector_t fwd_dir = w_joint_rot * (-offset_rot) * vector_t( 0.0, 0.0, 1.0 );
		get_deg = (float)DEG( acos( fwd_dir.dot( tgt_dir ) ) );
#else

// quat_t l_task_q = joint_arr[ priority_joint ].rotation_to_target( w_point ) * offset_rot; // see: MeCtGazeJoint::evaluate()

// NEED TO INTEGRATE offset_rot, forward-ray, bias (see TASK_TIME_HINT)
/*
		// construct an euler from ( z-axis, roll ):
		w_target_orient = euler_t( 
			w_point - (
				joint_arr[ GAZE_JOINT_SKULL ].world_pos +
				joint_arr[ GAZE_JOINT_SKULL ].local_rot *
				joint_arr[ GAZE_JOINT_SKULL ].forward_pos
			), 
			0.0 
		);
*/
		
		vector_t biased_offset_joint_fwd = 
//		vector_t tgt_dir = ( w_tgt_pos - world_pos ).normalize();
		joint_arr[ priority_joint ].forward_pos;
		
		vector_t tgt_dir = ( w_tgt_pos - w_joint_pos ).normalize();

//		vector_t fwd_dir = w_joint_rot * vector_t( 0.0, 0.0, 1.0 );
		vector_t fwd_dir = w_joint_rot * (-offset_rot) * vector_t( 0.0, 0.0, 1.0 );

		get_deg = (float)DEG( acos( fwd_dir.dot( tgt_dir ) ) );
#endif

//LOG( "deg: %f\n", get_deg );
	}
	else	{
		quat_t w_tgt_rot = world_target_orient();
// quat_t l_task_q = joint_arr[ priority_joint ].rotation_to_target( w_orient ) * offset_rot; // see: MeCtGazeJoint::evaluate()

// DON"T FORGET offset_rot, bias
		
		vector_t tgt_dir = w_tgt_rot * vector_t( 0.0, 0.0, 1.0 );
		vector_t fwd_dir = w_joint_rot * (-offset_rot) * vector_t( 0.0, 0.0, 1.0 );
		get_deg = (float)DEG( acos( fwd_dir.dot( tgt_dir ) ) );
	}
	
	return( true );
}

///////////////////////////////////////////////////////////////////////////

void MeCtGaze::set_speed( float head_dps, float eyes_dps )	{
	timing_mode = TASK_SPEED;
	head_speed = head_dps;

	joint_arr[ GAZE_JOINT_EYE_L ].speed = eyes_dps;
	joint_arr[ GAZE_JOINT_EYE_R ].speed = eyes_dps;
}

void MeCtGaze::set_time_hint( float head_sec )	{
	
	timing_mode = TASK_TIME_HINT;
	head_time = head_sec;
}

/////////////////////////////////////////////////////////////////////////////

void MeCtGaze::inspect_skeleton_down( SkJoint* joint_p, int depth )	{
	
	if( joint_p )	{
		const char *name = joint_p->jointName().c_str();
		char indent[ 256 ];
		int c, i, n;
		for( c=0; c<depth; c++ ) { indent[ c ] = ' '; }
		LOG( "%s%s\n", indent, name );
		n = joint_p->num_children();
		for( i=0; i<n; i++ )	{
			inspect_skeleton_down( joint_p->child( i ), depth + 1 );
		}
	}
}

void MeCtGaze::inspect_skeleton_local_transform_down( SkJoint* joint_p, int depth )	{
	
	if( joint_p )	{
		const char *name = joint_p->jointName().c_str();
		gwiz::matrix_t M;
		int i, j, c;

		SrMat sr_M = joint_p->lmat();
		for( i=0; i<4; i++ )	{
			for( j=0; j<4; j++ )	{
				M.set( i, j, sr_M.get( i, j ) );
			}
		}
		vector_t pos = M.translation( gwiz::COMP_M_TR );
		euler_t rot = M.euler( gwiz::COMP_M_TR );

		char indent[ 256 ];
		for( c=0; c<depth; c++ ) { indent[ c ] = ' '; }
		indent[ c ] = 0;
		LOG( "%s : pos{ %.3f %.3f %.3f } : phr{ %.2f %.2f %.2f }", 
			indent, name,
			pos.x(), pos.y(), pos.z(),
			rot.p(), rot.h(), rot.r()
		);

		int n = joint_p->num_children();
		for( i=0; i<n; i++ )	{
			inspect_skeleton_local_transform_down( joint_p->child( i ), depth + 1 );
		}
	}
}

void MeCtGaze::inspect_skeleton_world_transform_down( SkJoint* joint_p, int depth )	{
	
	if( joint_p )	{
		const char *name = joint_p->jointName().c_str();
		gwiz::matrix_t M;
		int i, j, c;

		joint_p->update_gmat_up();
		SrMat sr_M = joint_p->gmat();
		for( i=0; i<4; i++ )	{
			for( j=0; j<4; j++ )	{
				M.set( i, j, sr_M.get( i, j ) );
			}
		}
		vector_t pos = M.translation( gwiz::COMP_M_TR );
		euler_t rot = M.euler( gwiz::COMP_M_TR );

		char indent[ 256 ];
		for( c=0; c<depth; c++ ) { indent[ c ] = ' '; }
		indent[ c ] = 0;
		LOG( "%s : pos{ %.3f %.3f %.3f } : phr{ %.2f %.2f %.2f }", 
			indent, name,
			pos.x(), pos.y(), pos.z(),
			rot.p(), rot.h(), rot.r()
		);
		
		int n = joint_p->num_children();
		for( i=0; i<n; i++ )	{
			inspect_skeleton_world_transform_down( joint_p->child( i ), depth + 1 );
		}
	}
}

void MeCtGaze::inspect_skeleton_local_transform_up( SkJoint* joint_p, int depth )	{
	
	if( joint_p )	{
		const char *name = joint_p->jointName().c_str();
		gwiz::matrix_t M;
		int i, j, c;

		SrMat sr_M = joint_p->lmat();
		for( i=0; i<4; i++ )	{
			for( j=0; j<4; j++ )	{
				M.set( i, j, sr_M.get( i, j ) );
			}
		}
		vector_t pos = M.translation( gwiz::COMP_M_TR );
		euler_t rot = M.euler( gwiz::COMP_M_TR );

		char indent[ 256 ];
		for( c=0; c<depth; c++ ) { indent[ c ] = ' '; }
		indent[ c ] = 0;
		LOG( "%s%s : pos{ %.3f %.3f %.3f } : phr{ %.2f %.2f %.2f }", 
			indent, name,
			pos.x(), pos.y(), pos.z(),
			rot.p(), rot.h(), rot.r()
		);

		inspect_skeleton_local_transform_up( joint_p->parent(), depth + 1 );
	}
}

void MeCtGaze::update_skeleton_gmat( void )	{

	if( skeleton_ref_p )	{
		SkJoint* joint_p = skeleton_ref_p->search_joint( "skullbase" );
		if( joint_p ) {
			joint_p->update_gmat_up();
		}
		else	{
//			LOG("MeCtGaze::update_skeleton_gmat ERR: joint 'skullbase' NOT FOUND in skeleton" );
		}

		joint_p = skeleton_ref_p->search_joint( "eyeball_left" );
		if( joint_p ) {
			joint_p->update_gmat_up();
		}
		else	{
//			LOG("MeCtGaze::update_skeleton_gmat ERR: joint 'eyeball_left' NOT FOUND in skeleton" );
		}

		joint_p = skeleton_ref_p->search_joint( "eyeball_right" );
		if( joint_p ) {
			joint_p->update_gmat_up();
		}
		else	{
//			LOG("MeCtGaze::update_skeleton_gmat ERR: joint 'eyeball_right' NOT FOUND in skeleton" );
		}
	}
	else	{
		//LOG("MeCtGaze::update_skeleton_gmat ERR: skeleton NOT FOUND" );
	}
}

#if 0
void MeCtGaze::load_forward_pos( void ) {

	vector_t world_mid_eye_pos = 
		joint_arr[ GAZE_JOINT_EYE_L ].world_pos.lerp( 
			0.5, joint_arr[ GAZE_JOINT_EYE_R ].world_pos 
		) +
		vector_t( 0.0, 0.0, 5.0 ); // NOTE: PRESUMES 5CM SCALE...

	printf( "eyes:\n" );
	world_mid_eye_pos.print();

	joint_arr[ GAZE_JOINT_SPINE1 ].forward_pos =
		world_mid_eye_pos - joint_arr[ GAZE_JOINT_SPINE1 ].world_pos;
	
	printf( "spine1:\n" );
	joint_arr[ GAZE_JOINT_SPINE1 ].forward_pos.print();

	joint_arr[ GAZE_JOINT_SPINE2 ].forward_pos =
		world_mid_eye_pos - joint_arr[ GAZE_JOINT_SPINE2 ].world_pos;
		
	joint_arr[ GAZE_JOINT_SPINE3 ].forward_pos =
		world_mid_eye_pos - joint_arr[ GAZE_JOINT_SPINE3 ].world_pos;
		
	joint_arr[ GAZE_JOINT_SPINE4 ].forward_pos =
		world_mid_eye_pos - joint_arr[ GAZE_JOINT_SPINE4 ].world_pos;
		
	joint_arr[ GAZE_JOINT_SPINE5 ].forward_pos =
		world_mid_eye_pos - joint_arr[ GAZE_JOINT_SPINE5 ].world_pos;

	joint_arr[ GAZE_JOINT_SKULL ].forward_pos =
		world_mid_eye_pos - joint_arr[ GAZE_JOINT_SKULL ].world_pos;
}
#else

void MeCtGaze::load_forward_pos( void ) {

#if 0
printf( "eye: %f\n", joint_arr[ GAZE_JOINT_EYE_L ].local_pos.y() );
printf( "eye-p: %f\n", joint_arr[ GAZE_JOINT_EYE_L ].parent_loc_pos.y() );
printf( "skull: %f\n", joint_arr[ GAZE_JOINT_SKULL ].local_pos.y() );
printf( "s5: %f\n", joint_arr[ GAZE_JOINT_SPINE5 ].local_pos.y() );
printf( "s4: %f\n", joint_arr[ GAZE_JOINT_SPINE4 ].local_pos.y() );
printf( "s3: %f\n", joint_arr[ GAZE_JOINT_SPINE3 ].local_pos.y() );
printf( "s2: %f\n", joint_arr[ GAZE_JOINT_SPINE2 ].local_pos.y() );
printf( "s1: %f\n", joint_arr[ GAZE_JOINT_SPINE1 ].local_pos.y() );
#endif

#define USE_OLD_GAZE 0
#if USE_OLD_GAZE
	gwiz::float_t interocular = 
		joint_arr[ GAZE_JOINT_EYE_L ].local_pos.x() - 
		joint_arr[ GAZE_JOINT_EYE_R ].local_pos.x();
		
	gwiz::float_t height;
	
	height = 
		joint_arr[ GAZE_JOINT_EYE_L ].local_pos.y() +
		joint_arr[ GAZE_JOINT_EYE_L ].parent_loc_pos.y();
	joint_arr[ GAZE_JOINT_SKULL ].forward_pos = vector_t( 0.0, height, interocular );
	
	height += joint_arr[ GAZE_JOINT_SKULL ].local_pos.y();
	joint_arr[ GAZE_JOINT_SPINE5 ].forward_pos = vector_t( 0.0, height, interocular );

	height += joint_arr[ GAZE_JOINT_SPINE5 ].local_pos.y();
	joint_arr[ GAZE_JOINT_SPINE4 ].forward_pos = vector_t( 0.0, height, interocular );
	
	height += joint_arr[ GAZE_JOINT_SPINE4 ].local_pos.y();
	joint_arr[ GAZE_JOINT_SPINE3 ].forward_pos = vector_t( 0.0, height, interocular );
	
	height += joint_arr[ GAZE_JOINT_SPINE3 ].local_pos.y();
	joint_arr[ GAZE_JOINT_SPINE2 ].forward_pos = vector_t( 0.0, height, interocular );
	
	height += joint_arr[ GAZE_JOINT_SPINE2 ].local_pos.y();
	joint_arr[ GAZE_JOINT_SPINE1 ].forward_pos = vector_t( 0.0, height, interocular );
#else

	vector_t interocular = joint_arr[ GAZE_JOINT_EYE_L ].world_zero_pos - joint_arr[ GAZE_JOINT_EYE_R ].world_zero_pos;

	vector_t world_mid_eye_pos = 
		joint_arr[ GAZE_JOINT_EYE_L ].world_zero_pos.lerp( 
			0.5, joint_arr[ GAZE_JOINT_EYE_R ].world_zero_pos 
		) +
		vector_t( 0.0, 0.0, interocular.length()); // NOTE: PRESUMES 5CM SCALE...
	
	//LOG("before ,  interocular length = %f, skull pos = %f %f %f, mideye pos = %f %f %f", interocular.length(), joint_arr[ GAZE_JOINT_SKULL ].world_zero_pos.x(), joint_arr[ GAZE_JOINT_SKULL ].world_zero_pos.y(), joint_arr[ GAZE_JOINT_SKULL ].world_zero_pos.z(), world_mid_eye_pos.x(), world_mid_eye_pos.y(), world_mid_eye_pos.z() );


	if (!joint_arr[GAZE_JOINT_EYE_L].active && !joint_arr[GAZE_JOINT_EYE_R].active && skeleton_ref_p) // no eye joints
	{		
		double forwardLen = skeleton_ref_p->getCurrentHeight()*0.03;		
		world_mid_eye_pos = joint_arr[ GAZE_JOINT_SKULL ].world_zero_pos + vector_t(0.0,forwardLen*2.0, forwardLen*3.0);
		//LOG("has skeleton ref, forward len = %f, skull pos = %f %f %f, mideye pos = %f %f %f",forwardLen, joint_arr[ GAZE_JOINT_SKULL ].world_zero_pos.x(), joint_arr[ GAZE_JOINT_SKULL ].world_zero_pos.y(), joint_arr[ GAZE_JOINT_SKULL ].world_zero_pos.z(), world_mid_eye_pos.x(), world_mid_eye_pos.y(), world_mid_eye_pos.z() );
	}	
	else
	{
		//LOG("no skeleton ref");
	}

	//printf( "eyes:\n" );
	//world_mid_eye_pos.print();

	joint_arr[ GAZE_JOINT_SPINE1 ].forward_pos =
		world_mid_eye_pos - joint_arr[ GAZE_JOINT_SPINE1 ].world_zero_pos;
	
	//printf( "spine1:\n" );
	//joint_arr[ GAZE_JOINT_SPINE1 ].forward_pos.print();

	joint_arr[ GAZE_JOINT_SPINE2 ].forward_pos =
		world_mid_eye_pos - joint_arr[ GAZE_JOINT_SPINE2 ].world_zero_pos;
		
	joint_arr[ GAZE_JOINT_SPINE3 ].forward_pos =
		world_mid_eye_pos - joint_arr[ GAZE_JOINT_SPINE3 ].world_zero_pos;
		
	joint_arr[ GAZE_JOINT_SPINE4 ].forward_pos =
		world_mid_eye_pos - joint_arr[ GAZE_JOINT_SPINE4 ].world_zero_pos;
		
	joint_arr[ GAZE_JOINT_SPINE5 ].forward_pos =
		world_mid_eye_pos - joint_arr[ GAZE_JOINT_SPINE5 ].world_zero_pos;

	joint_arr[ GAZE_JOINT_SKULL ].forward_pos =
		world_mid_eye_pos - joint_arr[ GAZE_JOINT_SKULL ].world_zero_pos;
#endif
}

#endif

#if 0
SkJoint* MeCtGaze::get_joint( char *joint_str, SkJoint *joint_p )	{

	if( joint_str )	{
		if( joint_p == NULL )	{
			if( skeleton_ref_p )	{
				joint_p = skeleton_ref_p->search_joint( joint_str );
				if( joint_p == NULL )	{
					fprintf( stderr, "MeCtGaze::get_joint ERR: joint '%s' NOT FOUND in skeleton\n", joint_str );
					free( joint_str );
					joint_str = NULL;
				}
			}
			else	{
				fprintf( stderr, "MeCtGaze::get_joint ERR: skeleton NOT FOUND\n" );
			}
		}
	}
	return( joint_p );
}

SkJoint* MeCtGaze::target_ref_joint( void ) {
//	return( target_ref_joint_p = get_joint( target_ref_joint_str, target_ref_joint_p ) );
	return( get_joint( target_ref_joint_str, &target_ref_joint_p ) );
}

SkJoint* MeCtGaze::offset_ref_joint( void ) {
//	return( offset_ref_joint_p = get_joint( offset_ref_joint_str, offset_ref_joint_p ) );
	return( get_joint( offset_ref_joint_str, &offset_ref_joint_p ) );
}
#endif

SkJoint* MeCtGaze::reference_joint( void )	{

	if( ref_joint_str )	{
		if( ref_joint_p == NULL )	{
			if( skeleton_ref_p )	{
				ref_joint_p = skeleton_ref_p->search_joint( ref_joint_str );
				if( ref_joint_p == NULL )	{
					LOG("MeCtGaze::reference_joint ERR: joint '%s' NOT FOUND in skeleton", ref_joint_str );
					free( ref_joint_str );
					ref_joint_str = NULL;
				}
			}
			else	{
				LOG("MeCtGaze::reference_joint ERR: skeleton NOT FOUND" );
			}
		}
	}
	return( ref_joint_p );
}

vector_t MeCtGaze::world_target_point( void )	{
	
/*
	add point offset here:

*/

	SkJoint* joint_p = reference_joint();
	if( joint_p )	{
		SrMat sr_M;
		gwiz::matrix_t M;
		int i, j;
		
		joint_p->update_gmat_up();
		sr_M = joint_p->gmat();
		for( i=0; i<4; i++ )	{
			for( j=0; j<4; j++ )	{
				M.set( i, j, sr_M.get( i, j ) );
			}
		}
		vector_t pos = M.translation( gwiz::COMP_M_TR );
		quat_t rot = M.quat( gwiz::COMP_M_TR );
		return( pos + rot * point_target_pos );
	}
	return( point_target_pos );
}

quat_t MeCtGaze::world_target_orient( void )	{
	
/*
	add point offset here:

*/

	SkJoint* joint_p = reference_joint();
	if( joint_p )	{
		SrMat sr_M;
		gwiz::matrix_t M;
		int i, j;
		
		joint_p->update_gmat_up();
		sr_M = joint_p->gmat();
		for( i=0; i<4; i++ )	{
			for( j=0; j<4; j++ )	{
				M.set( i, j, sr_M.get( i, j ) );
			}
		}
		quat_t rot = M.quat( gwiz::COMP_M_TR );
		return( rot * orient_target_rot );
	}
	return( orient_target_rot );
}

void MeCtGaze::controller_start( void )	{
	
//	LOG("START ON %s", this->name());
#if ENABLE_HACK_TARGET_CIRCLE
//G_hack_target_heading = 0.0;  // don't update this for every gaze controller
#endif

	started = 1;
	setStart(1);


#if TEST_SENSOR
	sensor_p->controller_start();
#endif
}

void MeCtGaze::controller_start_evaluate( void )	{
//	LOG("START_EVALUTE ON %s", this->name());
	int i;

	if( _context->channels().size() > 0 )	{
		skeleton_ref_p = _context->channels().skeleton();
	}
	else
	{
		LOG("No gazing channels found.");
	}
	
	// ensure skeleton global tansforms are up to date
	update_skeleton_gmat();
	
	for( i=0; i<joint_count; i++ )	{

		float local_contrib;
		if( i < priority_joint )	{
			local_contrib = 1.0f / ( (float)( priority_joint + 1 - i ) );
		}
		else	{
			local_contrib = 1.0f;
		}
		joint_arr[ i ].task_weight = local_contrib;
		
		int context_index = _toContextCh[ i ];
		if( context_index < 0 ) {
			joint_arr[ i ].active = 0;
			LOG("MeCtGaze:: ERR: '%s' NOT FOUND in skeleton", joint_label( i ) );
		} 
		else {
			SkJoint* joint_p = _context->channels().joint( context_index );
			if( !joint_p )	{
				joint_arr[ i ].active = 0;
				LOG("MeCtGaze:: ERR: joint( %d ): '%s' NOT FOUND in skeleton", context_index, joint_label( i ) );
			} 
			else {
				joint_arr[ i ].init( joint_p );
				joint_arr[ i ].start();
			}
		}
#if 0
		LOG( "start: [%d]", i );
		LOG( " wgt: %.4f",
			joint_arr[ i ].task_weight 
		);
#endif
	}
	
	// set forward position to eyeball center in local coords, 
	//  after joints have start()ed, to init world transform state
	load_forward_pos();

	// set head_speed based on head-rot-to-target-rot
	if( timing_mode == TASK_TIME_HINT )	{

		quat_t w_target_orient;
		if( target_mode == TARGET_POINT )	{
			vector_t w_point = world_target_point();
			// construct an euler from ( z-axis, roll )
			w_target_orient = euler_t( 
				w_point - (
					joint_arr[ GAZE_JOINT_SKULL ].world_pos +
					joint_arr[ GAZE_JOINT_SKULL ].local_rot *
					joint_arr[ GAZE_JOINT_SKULL ].forward_pos
				), 
				0.0 
			);
		}
		else	{
			w_target_orient = world_target_orient();
		}
		
		quat_t body_head_rot = 
			-joint_arr[ GAZE_JOINT_SPINE1 ].parent_rot *
			joint_arr[ GAZE_JOINT_SKULL ].world_rot * 
			joint_arr[ GAZE_JOINT_SKULL ].forward_rot;
		
		quat_t body_task_rot = 
			-joint_arr[ GAZE_JOINT_SPINE1 ].parent_rot *
			w_target_orient * 
			joint_arr[ GAZE_JOINT_SKULL ].forward_rot;
		
		quat_t head_task_rot = ( -body_head_rot ).product( body_task_rot );
		gwiz::float_t head_task_deg = head_task_rot.degrees();
		head_speed = (float)( head_task_deg / head_time );
//LOG( "MeCtGaze::controller_start TASK: %f deg  %f dps\n", head_task_deg, head_speed );
	}

	float total_w = 0.0;
	for( i=0; i<=GAZE_JOINT_SKULL; i++ )	{
		total_w += joint_arr[ i ].task_weight;
	}

	// move to -update-joint-speed() if dirty bit is set.
	for( i=0; i<=GAZE_JOINT_SKULL; i++ )	{
		joint_arr[ i ].speed_ratio = joint_arr[ i ].task_weight / total_w;
		joint_arr[ i ].speed = joint_arr[ i ].speed_ratio * head_speed;
//		joint_arr[ i ].speed = head_speed / ( priority_joint + 1 );
	}
}

void MeCtGaze::set_fade_in( float interval )	{
	
	if( fading_normal < 1.0 )	{
		fade_interval = interval * ( 1.0f - fading_normal );
		fade_remaining = fade_interval;
		fading_mode = FADING_MODE_IN;
		fading_complete = false;
	}
}

void MeCtGaze::set_fade_out( float interval )	{

	fading_mode = FADING_MODE_OUT;
	if( fading_normal > 0.0 )	{
		fade_interval = interval * fading_normal;
		fade_remaining = fade_interval;
	}
}

void MeCtGaze::setGazeSchedule(double time, GazeScheduleInfo g)
{
	GazeScheduleInfo s;
	s.hasTargetJoint = g.hasTargetJoint;
	s.targetJoint = g.targetJoint;
	s.hasTargetPosition = g.hasTargetPosition;
	s.targetPosition = g.targetPosition;
	s.direction = g.direction;
	s.sweepAngle = g.sweepAngle;
	s.roll = g.roll;

	gazeSchedules.insert(std::make_pair(time, s));
}

void MeCtGaze::set_fade_in_scheduled(float interval, double time)
{
	FadingInfo info;
	info.fadingMode = FADING_MODE_IN;
	info.fadingInterval = interval;
	double fadingScheduleTime = SmartBody::SBScene::getScene()->getSimulationManager()->getTime() + time;
	fadingSchedules.insert(std::make_pair((float)fadingScheduleTime, info));
	//LOG("set_fade_in_scheduled(%s): Current time %f, scheduled fading time is at %f.", this->_handle.c_str(), mcuCBHandle::singleton().time, fadingScheduleTime);
}

void MeCtGaze::set_fade_out_scheduled(float interval, double time)
{
	FadingInfo info;
	info.fadingMode = FADING_MODE_OUT;
	info.fadingInterval = interval;
	double fadingScheduleTime = SmartBody::SBScene::getScene()->getSimulationManager()->getTime() + time;
	fadingSchedules.insert(std::make_pair((float)fadingScheduleTime, info));
	//LOG("set_fade_out_scheduled(%s): Current time %f, scheduled fading time is at %f.", this->_handle.c_str(), mcuCBHandle::singleton().time, fadingScheduleTime);
}

#define SMOOTH_RATE_REF (30.0f)
#define FADE_EPSILON	(0.001f)

bool MeCtGaze::update_fading( float dt )	{

	std::map<double, FadingInfo>::iterator iter = fadingSchedules.begin();
	for (; iter != fadingSchedules.end(); ++iter)
	{
		if (SmartBody::SBScene::getScene()->getSimulationManager()->getTime() >= iter->first)
		{
			//LOG("update_fading(%s): Current time %f, scheduled fading time is at %f.", this->_handle.c_str(), mcuCBHandle::singleton().time, scheduled_time);	
			if (iter->second.fadingMode == FADING_MODE_IN)
				if (!isFadingIn())
				{
					//LOG("fade in!");
					set_fade_in(iter->second.fadingInterval);
				}
			if (iter->second.fadingMode == FADING_MODE_OUT)
				if (!isFadingOut())
				{
					//LOG("fade out!");
					set_fade_out(iter->second.fadingInterval);				
				}
			fadingSchedules.erase(iter);
			return false;
		}
	}

	// returns true if fully faded.
	
	if( fading_mode )	{

		fade_remaining -= dt;
		if( fade_remaining <= 0.0 ) {
			fade_remaining = 0.0;
		}

		if( fading_mode == FADING_MODE_IN )	{
			if( fade_interval > 0.0 )	{
				fading_normal = 1.0f - fade_remaining / fade_interval;
			}
			else	{
				fading_normal = 1.0;
			}
			float min_val = 1.0f;
			for( int i = 0; i < joint_key_count; i++ )	{
				MeCtGazeKey* key_p = joint_key_arr + i;

				float s = key_p->smooth;
				float blend = s * key_p->blend_weight + ( 1.0f - s ) * fading_normal;
				set_blend( i, blend );
				if( blend < min_val )	{
					min_val = blend;
				}
			}
			if( min_val > ( 1.0 - FADE_EPSILON ) )	{
				for( int i = 0; i < joint_key_count; i++ )	{
					set_blend( i, 1.0 );
				}
				fading_normal = 1.0f;
				fading_mode = FADING_MODE_OFF;
				set_speed( initialNeckSpeed, initialEyeSpeed );


//				LOG( "MeCtGaze::update_fading FULL WEIGHT" );
			}

		}
		else	{ // if( fading_mode == FADING_MODE_OUT )	{

			if( fading_complete == false )	{
				
				if( fade_interval > 0.0 )	{
					fading_normal = fade_remaining / fade_interval;
				}
				else	{
					fading_normal = 0.0;
				}
				float max_val = 0.0f;
				for( int i = 0; i < joint_key_count; i++ )	{
					MeCtGazeKey* key_p = joint_key_arr + i;

					float s = key_p->smooth;
					float blend = s * key_p->blend_weight + ( 1.0f - s ) * fading_normal;
					set_blend( i, blend );
					if( blend > max_val )	{
						max_val = blend;
					}
				}
				if( max_val < FADE_EPSILON )	{
					for( int i = 0; i < joint_key_count; i++ )	{
						set_blend( i, 0.0 );
					}
					fading_normal = 0.0f;
					fading_complete = true;
					fading_mode = FADING_MODE_OFF;
					set_speed( initialNeckSpeed, initialEyeSpeed );

//					LOG( "MeCtGaze::update_fading ZERO WEIGHT" );
				}
			}
		}
	}
	return( fading_complete );
}

void MeCtGaze::updateGazeSchedules( float dt )
{
	int numFound = 1;
	while (numFound > 0)
	{
		numFound--;
		std::map<double, GazeScheduleInfo>::iterator iter = gazeSchedules.begin();
		for (; iter != gazeSchedules.end(); ++iter)
		{
			GazeScheduleInfo& s = (*iter).second;
			if (SmartBody::SBScene::getScene()->getSimulationManager()->getTime() >= iter->first)
			{
				this->set_offset_polar( s.direction, s.sweepAngle, s.roll );
				if (s.hasTargetJoint)
				{
					size_t pos = s.targetJoint.find_first_of(":");
					if (pos != std::string::npos)
					{
						std::string t = s.targetJoint.substr(0, pos);
						std::string joint = s.targetJoint.substr(pos + 1);
						SmartBody::SBPawn* pawn = SmartBody::SBScene::getScene()->getPawn(t);
						if (!pawn)
						{
							LOG("Gaze cannot be switched to %s, does not exist.", s.targetJoint.c_str());
						}
						else
						{
							SmartBody::SBJoint* sbjoint = dynamic_cast<SmartBody::SBJoint*>(pawn->getSkeleton()->getJointByMappedName(joint));
							if (!sbjoint)
							{
								LOG("Gaze cannot be switched to %s/%s - joint does not exist.", s.targetJoint.c_str(), joint.c_str());
							}
							else
							{
								if (s.hasTargetPosition)
								{
									this->set_target_joint( s.targetPosition.x, s.targetPosition.y, s.targetPosition.z, sbjoint );
								}
								else
								{
									this->set_target_joint( 0.0f, 0.0f, 0.0f, sbjoint );
								}
							}

						}
					}
					else
					{
						SmartBody::SBPawn* pawn = SmartBody::SBScene::getScene()->getPawn(s.targetJoint.c_str());
						if (!pawn)
						{
							LOG("Gaze cannot be switched to %s, does not exist.", s.targetJoint.c_str());
						}
						else
						{
							SmartBody::SBJoint* sbjoint = dynamic_cast<SmartBody::SBJoint*>(pawn->getSkeleton()->getJointByMappedName("eyeball_left"));
							if (!sbjoint)
							{
								sbjoint = dynamic_cast<SmartBody::SBJoint*>(pawn->getSkeleton()->getJointByMappedName(SbmPawn::WORLD_OFFSET_JOINT_NAME));
								if (!sbjoint)
								{
									LOG("Target character does not have joint named left_eyeball or %s, cannot switch gaze.", SbmPawn::WORLD_OFFSET_JOINT_NAME);
								}
								else
								{
									if (s.hasTargetPosition)
									{
										this->set_target_joint( s.targetPosition.x, s.targetPosition.y, s.targetPosition.z, sbjoint );
									}
								}
							}
						
						}

					}
				}
				else
				{
					if (s.hasTargetPosition)
					{
						this->set_target(s.targetPosition.x, s.targetPosition.y, s.targetPosition.z);
					}
				}
				gazeSchedules.erase(iter);
				numFound++;
				break;
			}
		}
	}
}

bool MeCtGaze::controller_evaluate( double t, MeFrameData& frame )	{
	

	float dt;
	if( getStart() ) {
		setStart(0);
		dt = 0.001f;
		controller_start_evaluate();
	}
	else	{
		dt = (float)(t - prev_time);
	}
	prev_time = t;
	
#if 0
	static int once = 1;
	if( once )	{
		once = 0;

//		test_forward_ray();
#if 0
		LOG( "-- skeleton:" );
		if( skeleton_ref_p )	{

//			SkJoint* joint_p = skeleton_ref_p->search_joint( SbmPawn::WORLD_OFFSET_JOINT_NAME );
//			inspect_skeleton_down( joint_p );
//			inspect_skeleton_local_transform_down( joint_p );
//			inspect_skeleton_world_transform_down( joint_p );

			SkJoint* joint_p = skeleton_ref_p->search_joint( "eyeball_left" );
			inspect_skeleton_local_transform_up( joint_p );
		}
		LOG( "--" );
#endif
	}
#endif
	
	if( _duration > 0.0 )	{
		if( t > (double)_duration )	{
			return( FALSE );
		}
	}

	if( update_fading( dt ) )	{
		return( TRUE );
	}

	updateGazeSchedules(dt);

	update_skeleton_gmat();
	
	// map key values to joints if set
	apply_smooth_keys();
	apply_bias_keys();
	apply_limit_keys();
	apply_blend_keys();
	
	vector_t w_point;
	quat_t w_orient;
	if( target_mode == TARGET_POINT )	{
#if ENABLE_HACK_TARGET_CIRCLE
		if( G_hack_target_circle )	{
			G_hack_target_heading += 60.0f * dt;  // 60 degrees per second
			w_point = euler_t( 0.0, G_hack_target_heading, 0.0 ) * vector_t( 0.0, 150.0, 10000.0 );  // 150 up and 10,000 radius
		}
		else
#endif
		w_point = world_target_point();
	}
	else	{
		w_orient = world_target_orient();
	}

#if 0
	if( offset_mode == OFFSET_POINT )	{
		
	}
	else	{
		
	}
#endif

#if 0
printf( "eyelim: %f %f %f %f\n", 
	joint_key_arr[ GAZE_KEY_EYES ].limit_p_up,
	joint_key_arr[ GAZE_KEY_EYES ].limit_p_dn,
	joint_key_arr[ GAZE_KEY_EYES ].limit_h,
	joint_key_arr[ GAZE_KEY_EYES ].limit_r
);
#endif

	SrBuffer<float>& buff = frame.buffer();
	int channels_size = _channels.size();

	for( int i=0; i<channels_size; ++i ) {

		if( joint_arr[ i ].active && ( joint_arr[ i ].blend_weight > 0.0 ) ) {
		
			int index = frame.toBufferIndex( _toContextCh[ i ] );

			quat_t Q_in = quat_t(
				buff[ index + 0 ],
				buff[ index + 1 ],
				buff[ index + 2 ],
				buff[ index + 3 ]
			);

			quat_t Q_eval;
			if( target_mode == TARGET_POINT )	{
				Q_eval = joint_arr[ i ].evaluate( dt, w_point, offset_rot, 1.0 );
			}
			else	{
				Q_eval = joint_arr[ i ].evaluate( dt, w_orient, offset_rot, 1.0 );
			}

			quat_t Q_out = Q_in.lerp( joint_arr[ i ].blend_weight, Q_eval );
				
#define SPINE_LOCK_HEAD_TEST 0
#if SPINE_LOCK_HEAD_TEST
			if( i == GAZE_JOINT_SPINE1 )	{
	//			euler_t e( 0.0, 0.0, 0.0 );
	//			euler_t e( -90.0, 0.0, 0.0 );
	//			euler_t e( 90.0, 0.0, 0.0 );
	//			euler_t e( 0.0, 90.0, 0.0 );
	//			euler_t e( 0.0, 180.0, 0.0 );
	//			euler_t e( 0.0, 0.0, 90.0 );
	//			Q_out = e;
			}
			if( ( i == GAZE_JOINT_SPINE1 )||( i == GAZE_JOINT_SKULL ) )	{
				buff[ index + 0 ] = (float) Q_out.w();
				buff[ index + 1 ] = (float) Q_out.x();
				buff[ index + 2 ] = (float) Q_out.y();
				buff[ index + 3 ] = (float) Q_out.z();
			}
#else

			buff[ index + 0 ] = (float) Q_out.w();
			buff[ index + 1 ] = (float) Q_out.x();
			buff[ index + 2 ] = (float) Q_out.y();
			buff[ index + 3 ] = (float) Q_out.z();
				
#endif

			// Mark channel changed
			frame.channelUpdated( i );
		}
	}
	
#if TEST_SENSOR
	sensor_p->controller_evaluate( t, frame );
#endif
	return( TRUE );
}

void MeCtGaze::print_state( int tabs )	{
	LOG("MeCtGaze" );
	const char* str = getName().c_str();
	if( str )
		LOG(" \"%s\"\n", str );
}

///////////////////////////////////////////////////////////////////////////

int MeCtGaze::getStart()
{
	return foostart;
}

void MeCtGaze::setStart(int value)
{
//	LOG("START SET TO %d ON %s", value, this->name());
	foostart = value;
}

bool MeCtGaze::isFadingIn()
{
	return fading_mode ==FADING_MODE_IN;
}

bool MeCtGaze::isFadingOut()
{
	return fading_mode == FADING_MODE_OUT;
}

bool MeCtGaze::isFadedOut()
{
	return fading_normal == 0.0f;
}
