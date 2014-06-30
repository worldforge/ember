/*
 *  me_ct_gaze_target.cpp - part of SmartBody-lib
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
 *      Ed Fast, USC
 */

#include "controllers/me_ct_gaze.h"
#include "sbm/lin_win.h"
using namespace gwiz;

#if 0
/////////////////////////////////////////////////////////////////

// target coordinate
void MeCtGaze::set_target_point( float x, float y, float z )	{

	point_target_pos = vector_t( x, y, z );
	target_mode = TARGET_POINT;	
}

void MeCtGaze::set_target_euler( float p, float h, float r )	{

	orient_target_rot = euler_t( p, h, r );
	target_mode = TARGET_ORIENT;
}

void MeCtGaze::set_target_swing( float sw_p, float sw_h, float tw = 0.0 )	{

	orient_target_rot = quat_t( sw_p, sw_h, tw );
	target_mode = TARGET_ORIENT;
}

void MeCtGaze::set_target_polar( float d, float a, float r = 0.0 )	{
	
#if 0
	vector_t swing_axis = euler_t( 0.0, 0.0, d ) * vector_t( -1.0, 0.0, 0.0 );
	orient_target_rot = quat_t( a, swing_axis ) * quat_t( r, vector_t( 0.0, 0.0, 1.0 ) );	
#else
	vector_t direction = 
		euler_t( 0.0, 0.0, d ) * 
		euler_t( -a, 0.0, 0.0 ) * 
		vector_t( 0.0, 0.0, 1.0 ); // NOTE: flip pitch
	orient_target_rot = euler_t( direction, r ); // lookat constructor
#endif
	target_mode = TARGET_ORIENT;
}

// target coordinate system
void MeCtGaze::set_target_coord_world( void )	{

	if( target_ref_joint_str ) free( target_ref_joint_str );
	target_ref_joint_str = NULL;
	target_ref_joint_p = NULL;
}

void MeCtGaze::set_target_coord_joint( char *joint_name )	{
	
	if( target_ref_joint_str ) free( target_ref_joint_str );
	target_ref_joint_str = _strdup( joint_name );
	target_ref_joint_p = NULL;
}

void MeCtGaze::set_target_coord_joint( SkJoint* joint_p )	{

	if( target_ref_joint_str ) free( target_ref_joint_str );
	target_ref_joint_str = NULL;
	target_ref_joint_p = joint_p;
}

// offset coordinate
void MeCtGaze::set_offset_point( float x, float y, float z )	{
	
	point_offset_pos = vector_t( x, y, z );
	offset_mode = OFFSET_POINT;
}

void MeCtGaze::set_offset_euler( float p, float h, float r = 0.0 )	{

	orient_offset_rot = euler_t( p, h, r );
	offset_mode = OFFSET_ORIENT;
}

void MeCtGaze::set_offset_swing( float sw_p, float sw_h, float tw = 0.0 )	{

	orient_offset_rot = quat_t( sw_p, sw_h, tw );
	offset_mode = OFFSET_ORIENT;
}

void MeCtGaze::set_offset_polar( float d, float a, float r = 0.0 )	{
	
	vector_t direction = 
		euler_t( 0.0, 0.0, d ) * 
		euler_t( -a, 0.0, 0.0 ) * 
		vector_t( 0.0, 0.0, 1.0 ); // NOTE: flip pitch
	orient_offset_rot = euler_t( direction, r ); // lookat constructor
	offset_mode = OFFSET_ORIENT;
}

// offset coordinate system
void MeCtGaze::set_offset_coord_world( void )	{

	if( offset_ref_joint_str ) free( offset_ref_joint_str );
	offset_ref_joint_str = NULL;
	offset_ref_joint_p = NULL;
	offset_coord = OFFSET_WORLD;
}

void MeCtGaze::set_offset_coord_joint( char *joint_name )	{

	if( offset_ref_joint_str ) free( offset_ref_joint_str );
	offset_ref_joint_str = _strdup( joint_name );
	offset_ref_joint_p = NULL;
	offset_coord = OFFSET_JOINT_LOCAL;
}

void MeCtGaze::set_offset_coord_joint( SkJoint* joint_p )	{

	if( offset_ref_joint_str ) free( offset_ref_joint_str );
	offset_ref_joint_str = NULL;
	offset_ref_joint_p = joint_p;
	offset_coord = OFFSET_JOINT_LOCAL;
}

void MeCtGaze::set_offset_coord_parent( void )	{

	if( offset_ref_joint_str ) free( offset_ref_joint_str );
	offset_ref_joint_str = NULL;
	offset_ref_joint_p = NULL;
	offset_coord = OFFSET_PARENT_LOCAL;
}

// BACKWARDS COMPATIBILITY:

void MeCtGaze::set_target_joint( float x, float y, float z, SkJoint* joint_p = NULL ) {

#if 1
static int once = 1;
if( once )	{
	once = 0; LOG( "MeCtGaze::set_target_joint( f, f, f, * ) NOTICE: call deprecated\n" );
}
#endif

	set_target_point( x, y, z );
	target_mode = TARGET_POINT;	
	if( joint_p ) set_target_coord_joint( joint_p );
	else set_target_coord_world();
}

void MeCtGaze::set_target( float x, float y, float z, char *joint_name = NULL ) {

#if 1
static int once = 1;
if( once )	{
	once = 0; LOG( "MeCtGaze::set_target( f, f, f, * ) NOTICE: call deprecated\n" );
}
#endif

	set_target_point( x, y, z );
	target_mode = TARGET_POINT;	
	if( joint_name ) set_target_coord_joint( joint_name );
	else set_target_coord_world();
}

void MeCtGaze::set_orient_joint( float p, float h, float r, SkJoint* joint_p = NULL ) {

#if 1
static int once = 1;
if( once )	{
	once = 0; LOG( "MeCtGaze::set_orient_joint( f, f, f, * ) NOTICE: call deprecated\n" );
}
#endif

	set_target_euler( p, h, r );
	target_mode = TARGET_POINT;	
	if( joint_p ) set_target_coord_joint( joint_p );
	else set_target_coord_world();
}

void MeCtGaze::set_orient( float p, float h, float r, char *joint_name = NULL ) {

#if 1
static int once = 1;
if( once )	{
	once = 0; LOG( "MeCtGaze::set_orient( f, f, f, * ) NOTICE: call deprecated\n" );
}
#endif

	set_target_euler( p, h, r );
	target_mode = TARGET_POINT;	
	if( joint_name ) set_target_coord_joint( joint_name );
	else set_target_coord_world();
}

/////////////////////////////////////////////////////////////////
#endif

void MeCtGaze::set_target_joint( float x, float y, float z, SkJoint* joint_p )	{

	target_mode = TARGET_POINT;	
	point_target_pos = vector_t( x, y, z );
	if( ref_joint_str ) {
		free( ref_joint_str );
		ref_joint_str = NULL;
	}
	ref_joint_p = joint_p;
}

void MeCtGaze::set_target( float x, float y, float z, char *ref_joint_name )	{

	target_mode = TARGET_POINT;	
	point_target_pos = vector_t( x, y, z );
	if( ref_joint_str ) {
		free( ref_joint_str );
	}
	if (ref_joint_name)
	{
		ref_joint_str = _strdup( ref_joint_name );
		ref_joint_p = NULL;
	}
}

void MeCtGaze::set_orient_joint( float p, float h, float r, SkJoint* joint_p )	{

	target_mode = TARGET_ORIENT;
	orient_target_rot = euler_t( p, h, r );
	if( ref_joint_str ) {
		free( ref_joint_str );
		ref_joint_str = NULL;
	}
	ref_joint_p = joint_p;
}

void MeCtGaze::set_orient( float p, float h, float r, char *ref_joint_name )	{

	target_mode = TARGET_ORIENT;
	orient_target_rot = euler_t( p, h, r );
	if( ref_joint_str ) {
		free( ref_joint_str );
	}
	ref_joint_str = _strdup( ref_joint_name );
	ref_joint_p = NULL;
}


void MeCtGaze::set_offset_euler( float off_p, float off_h, float off_r )	{

	offset_rot = euler_t( off_p, off_h, off_r );
}

void MeCtGaze::set_offset_swing( float off_p, float off_h, float off_r )	{
	
	offset_rot = quat_t( off_p, off_h, off_r );
}

void MeCtGaze::set_offset_polar( float off_d, float off_a, float off_r )	{
	
	vector_t swing_axis = euler_t( 0.0, 0.0, off_d ) * vector_t( -1.0, 0.0, 0.0 );
	offset_rot = quat_t( off_a, swing_axis ) * quat_t( off_r, vector_t( 0.0, 0.0, 1.0 ) );	
}

SkJoint* MeCtGaze::get_target_joint( float& x, float& y, float& z)
{ 
	x = float(point_target_pos.x());
	y = float(point_target_pos.y());
	z = float(point_target_pos.z());

	return ref_joint_p;
}
