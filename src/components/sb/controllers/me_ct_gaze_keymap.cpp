/*
 *  me_ct_gaze_keymap.cpp - part of SmartBody-lib
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

#include "controllers/me_ct_gaze.h"

/*
	Original key mapping intended to bracket spine between
	BACK and HEAD keys:

	LUMBAR: 	{ SPINE1, SPINE2 }
	THORAX: 	{ SPINE3 }
	CERVICAL:	{ SPINE4, SPINE5 }
	HEAD:		{ SKULL }
	EYES:		{ EYE_L, EYE_R }
	
	SPINE1: 	LUMBAR
	SPINE2: 	LUMBAR + THORAX
	SPINE3: 	THORAX
	SPINE4: 	CERVICAL
	SPINE5: 	CERVICAL + HEAD
	SKULL:		HEAD
	EYE_L:		EYES
	EYE_R:		EYES

	back:  lumbar:   { spine1, spine2 }
	chest: thorax:   { spine3 }
	neck:  cervical: { spine4, spine5 }
	head:  cranial:  { skullbase }
	eyes:  optical:  { eyeball_left, eyeball_right }
*/

/*

	GAZE_KEY_COMBINE_HEAD_AND_NECK:
	
	LUMBAR: 	{ SPINE1, SPINE2 }
	THORAX: 	{ SPINE3 }
	CERVICAL:	{ SPINE4, SPINE5, SKULL }
	EYES:		{ EYE_L, EYE_R }
	
	SPINE1: 	LUMBAR
	SPINE2: 	LUMBAR + THORAX
	SPINE3: 	THORAX
	SPINE4: 	CERVICAL
	SPINE5: 	CERVICAL
	SKULL:		CERVICAL
	EYE_L:		EYES
	EYE_R:		EYES

	back:  lumbar:   { spine1, spine2 }
	chest: thorax:   { spine3 }
	neck:  cervical: { spine4, spine5, skullbase }
	eyes:  optical:  { eyeball_left, eyeball_right }
*/

///////////////////////////////////////////////////////////////////////////

void MeCtGaze::set_smooth( float smooth_basis, float eye_smooth_basis ) {

	if( smooth_basis < 0.0 ) smooth_basis = 0.0;
	if( smooth_basis > 1.0 ) smooth_basis = 1.0;
	if( eye_smooth_basis < 0.0 ) eye_smooth_basis = 0.0;
	if( eye_smooth_basis > 1.0 ) eye_smooth_basis = 1.0;

	joint_key_arr[ GAZE_KEY_BACK ].smooth = smooth_basis;
	joint_key_arr[ GAZE_KEY_CHEST ].smooth = smooth_basis;
	joint_key_arr[ GAZE_KEY_NECK ].smooth = smooth_basis;
#if GAZE_KEY_COMBINE_HEAD_AND_NECK
#else
	joint_key_arr[ GAZE_KEY_HEAD ].smooth = smooth_basis;
#endif
	joint_key_arr[ GAZE_KEY_EYES ].smooth = eye_smooth_basis;
	
	key_smooth_dirty = 1;
}

void MeCtGaze::set_smooth( float back_sm, float neck_sm, float eyes_sm )	{
	
	if( back_sm < 0.0 ) back_sm = 0.0;
	if( back_sm > 1.0 ) back_sm = 1.0;
	if( neck_sm < 0.0 ) neck_sm = 0.0;
	if( neck_sm > 1.0 ) neck_sm = 1.0;
	if( eyes_sm < 0.0 ) eyes_sm = 0.0;
	if( eyes_sm > 1.0 ) eyes_sm = 1.0;
	
	joint_key_arr[ GAZE_KEY_BACK ].smooth = back_sm;
	joint_key_arr[ GAZE_KEY_CHEST ].smooth = back_sm;
	joint_key_arr[ GAZE_KEY_NECK ].smooth = neck_sm;
#if GAZE_KEY_COMBINE_HEAD_AND_NECK
#else
	joint_key_arr[ GAZE_KEY_HEAD ].smooth = neck_sm;
#endif
	joint_key_arr[ GAZE_KEY_EYES ].smooth = eyes_sm;

	key_smooth_dirty = 1;
}

void MeCtGaze::apply_smooth_keys( void )	{
	
	if( key_smooth_dirty )	{

		joint_arr[ GAZE_JOINT_SPINE1 ].smooth = 
			joint_key_arr[ GAZE_KEY_BACK ].smooth;

		joint_arr[ GAZE_JOINT_SPINE2 ].smooth = 
			0.5f * ( joint_key_arr[ GAZE_KEY_BACK ].smooth + joint_key_arr[ GAZE_KEY_CHEST ].smooth );

		joint_arr[ GAZE_JOINT_SPINE3 ].smooth = 
			joint_key_arr[ GAZE_KEY_CHEST ].smooth;

#if GAZE_KEY_COMBINE_HEAD_AND_NECK
		joint_arr[ GAZE_JOINT_SPINE4 ].smooth = 
		joint_arr[ GAZE_JOINT_SPINE5 ].smooth = 
		joint_arr[ GAZE_JOINT_SKULL ].smooth = 
			joint_key_arr[ GAZE_KEY_NECK ].smooth;
#else
		joint_arr[ GAZE_JOINT_SPINE4 ].smooth = 
			joint_key_arr[ GAZE_KEY_NECK ].smooth;

		joint_arr[ GAZE_JOINT_SPINE5 ].smooth = 
			0.5f * ( joint_key_arr[ GAZE_KEY_NECK ].smooth + joint_key_arr[ GAZE_KEY_HEAD ].smooth );

		joint_arr[ GAZE_JOINT_SKULL ].smooth = 
			joint_key_arr[ GAZE_KEY_HEAD ].smooth;
#endif

		joint_arr[ GAZE_JOINT_EYE_L ].smooth = 
		joint_arr[ GAZE_JOINT_EYE_R ].smooth = 
			joint_key_arr[ GAZE_KEY_EYES ].smooth;

		key_smooth_dirty = 0;
	}
}

///////////////////////////////////////////////////////////////////////////

void MeCtGaze::set_bias( int key, float p, float h, float r )	{
	
	if( ! valid_key( key ) ) return;
	joint_key_arr[ key ].bias_rot = gwiz::euler_t( p, h, r );
	key_bias_dirty = 1;
}

void MeCtGaze::set_bias_pitch( int key, float p )	{

	if( ! valid_key( key ) ) return;
	joint_key_arr[ key ].bias_rot.p( p );
	key_bias_dirty = 1;
}

void MeCtGaze::set_bias_heading( int key, float h )	{

	if( ! valid_key( key ) ) return;
	joint_key_arr[ key ].bias_rot.h( h );
	key_bias_dirty = 1;
}

void MeCtGaze::set_bias_roll( int key, float r )	{

	if( ! valid_key( key ) ) return;
	joint_key_arr[ key ].bias_rot.r( r );
	key_bias_dirty = 1;
}

void MeCtGaze::set_bias_pitch( int key1, int key2, float p1, float p2 )	{
	int i;

	if( ! valid_key( key1 ) ) return;
	if( ! valid_key( key2 ) ) return;
	if( key1 == key2 )	{
		set_bias_pitch( key1, 0.5f * ( p1 + p2 ) );
		return;
	}
	if( key1 > key2 )	{
		int tmp = key1; key1 = key2; key2 = tmp;
		float tmp_p = p1; p1 = p2; p2 = tmp_p;
	}
	for( i = key1; i <= key2; i++ ) {
		float f = (float)( i - key1 )/(float)( key2 - key1 );
		float p = ( 1.0f - f ) * p1 + f * p2;
		set_bias_pitch( i, p );
	}
}

void MeCtGaze::set_bias_heading( int key1, int key2, float h1, float h2 ) {
	int i;

	if( ! valid_key( key1 ) ) return;
	if( ! valid_key( key2 ) ) return;
	if( key1 == key2 )	{
		set_bias_heading( key1, 0.5f * ( h1 + h2 ) );
		return;
	}
	if( key1 > key2 )	{
		int tmp = key1; key1 = key2; key2 = tmp;
		float tmp_h = h1; h1 = h2; h2 = tmp_h;
	}
	for( i = key1; i <= key2; i++ ) {
		float f = (float)( i - key1 )/(float)( key2 - key1 );
		float h = ( 1.0f - f ) * h1 + f * h2;
		set_bias_heading( i, h );
	}
}

void MeCtGaze::set_bias_roll( int key1, int key2, float r1, float r2 )	{
	int i;

	if( ! valid_key( key1 ) ) return;
	if( ! valid_key( key2 ) ) return;
	if( key1 == key2 )	{
		set_bias_roll( key1, 0.5f * ( r1 + r2 ) );
		return;
	}
	if( key1 > key2 )	{
		int tmp = key1; key1 = key2; key2 = tmp;
		float tmp_r = r1; r1 = r2; r2 = tmp_r;
	}
	for( i = key1; i <= key2; i++ ) {
		float f = (float)( i - key1 )/(float)( key2 - key1 );
		float r = ( 1.0f - f ) * r1 + f * r2;
		set_bias_roll( i, r );
	}
}

#if GAZE_KEY_COMBINE_HEAD_AND_NECK
void MeCtGaze::set_bias_pitch( float l_p, float t_p, float c_p, float e_p ) {

	joint_key_arr[ GAZE_KEY_BACK ].bias_rot.p( l_p );
	joint_key_arr[ GAZE_KEY_CHEST ].bias_rot.p( t_p );
	joint_key_arr[ GAZE_KEY_NECK ].bias_rot.p( c_p );
	joint_key_arr[ GAZE_KEY_EYES ].bias_rot.p( e_p );
	key_bias_dirty = 1;
}

void MeCtGaze::set_bias_heading( float l_h, float t_h, float c_h, float e_h )	{

	joint_key_arr[ GAZE_KEY_BACK ].bias_rot.h( l_h );
	joint_key_arr[ GAZE_KEY_CHEST ].bias_rot.h( t_h );
	joint_key_arr[ GAZE_KEY_NECK ].bias_rot.h( c_h );
	joint_key_arr[ GAZE_KEY_EYES ].bias_rot.h( e_h );
	key_bias_dirty = 1;
}

void MeCtGaze::set_bias_roll( float l_r, float t_r, float c_r, float e_r ) {

	joint_key_arr[ GAZE_KEY_BACK ].bias_rot.r( l_r );
	joint_key_arr[ GAZE_KEY_CHEST ].bias_rot.r( t_r );
	joint_key_arr[ GAZE_KEY_NECK ].bias_rot.r( c_r );
	joint_key_arr[ GAZE_KEY_EYES ].bias_rot.r( e_r );
	key_bias_dirty = 1;
}
#else
void MeCtGaze::set_bias_pitch( float e_p, float h_p, float c_p, float t_p, float l_p ) {

	joint_key_arr[ GAZE_KEY_EYES ].bias_rot.p( e_p );
	joint_key_arr[ GAZE_KEY_HEAD ].bias_rot.p( h_p );
	joint_key_arr[ GAZE_KEY_NECK ].bias_rot.p( c_p );
	joint_key_arr[ GAZE_KEY_CHEST ].bias_rot.p( t_p );
	joint_key_arr[ GAZE_KEY_BACK ].bias_rot.p( l_p );
	key_bias_dirty = 1;
}

void MeCtGaze::set_bias_heading( float e_h, float h_h, float c_h, float t_h, float l_h )	{

	joint_key_arr[ GAZE_KEY_EYES ].bias_rot.h( e_h );
	joint_key_arr[ GAZE_KEY_HEAD ].bias_rot.h( h_h );
	joint_key_arr[ GAZE_KEY_NECK ].bias_rot.h( c_h );
	joint_key_arr[ GAZE_KEY_CHEST ].bias_rot.h( t_h );
	joint_key_arr[ GAZE_KEY_BACK ].bias_rot.h( l_h );
	key_bias_dirty = 1;
}

void MeCtGaze::set_bias_roll( float e_r, float h_r, float c_r, float t_r, float l_r ) {

	joint_key_arr[ GAZE_KEY_EYES ].bias_rot.r( e_r );
	joint_key_arr[ GAZE_KEY_HEAD ].bias_rot.r( h_r );
	joint_key_arr[ GAZE_KEY_NECK ].bias_rot.r( c_r );
	joint_key_arr[ GAZE_KEY_CHEST ].bias_rot.r( t_r );
	joint_key_arr[ GAZE_KEY_BACK ].bias_rot.r( l_r );
	key_bias_dirty = 1;
}
#endif

void MeCtGaze::apply_bias_keys( void ) {

	if( key_bias_dirty )   {

		joint_arr[ GAZE_JOINT_SPINE1 ].forward_rot = 
			joint_key_arr[ GAZE_KEY_BACK ].bias_rot;

		joint_arr[ GAZE_JOINT_SPINE2 ].forward_rot = 
			joint_key_arr[ GAZE_KEY_BACK ].bias_rot.lerp( 0.5, joint_key_arr[ GAZE_KEY_CHEST ].bias_rot );

		joint_arr[ GAZE_JOINT_SPINE3 ].forward_rot = 
			joint_key_arr[ GAZE_KEY_CHEST ].bias_rot;

#if GAZE_KEY_COMBINE_HEAD_AND_NECK
		joint_arr[ GAZE_JOINT_SPINE4 ].forward_rot = 
			joint_key_arr[ GAZE_KEY_NECK ].bias_rot * 0.333333f;

		joint_arr[ GAZE_JOINT_SPINE5 ].forward_rot = 
			joint_key_arr[ GAZE_KEY_NECK ].bias_rot * 0.333333f;

		joint_arr[ GAZE_JOINT_SKULL ].forward_rot = 
			joint_key_arr[ GAZE_KEY_NECK ].bias_rot * 0.333333f;

#else
		joint_arr[ GAZE_JOINT_SPINE4 ].forward_rot = 
			joint_key_arr[ GAZE_KEY_NECK ].bias_rot;

		joint_arr[ GAZE_JOINT_SPINE5 ].forward_rot = 
			joint_key_arr[ GAZE_KEY_NECK ].bias_rot.lerp( 0.5, joint_key_arr[ GAZE_KEY_HEAD ].bias_rot );

		joint_arr[ GAZE_JOINT_SKULL ].forward_rot = 
			joint_key_arr[ GAZE_KEY_HEAD ].bias_rot;
#endif

		joint_arr[ GAZE_JOINT_EYE_L ].forward_rot = 
			joint_arr[ GAZE_JOINT_EYE_R ].forward_rot = 
				joint_key_arr[ GAZE_KEY_EYES ].bias_rot;

		key_bias_dirty = 0;
	}
}

/////////////////////////////////////////////////////////////////////////////

void MeCtGaze::set_limit( int key, float p, float h, float r )	{

	set_limit( key, p, p, h, r );
}

void MeCtGaze::set_limit( int key, float p_up, float p_dn, float h, float r )	{

	if( ! valid_key( key ) ) return;
	joint_key_arr[ key ].limit_p_up = p_up;
	joint_key_arr[ key ].limit_p_dn = p_dn;
	joint_key_arr[ key ].limit_h = h;
	joint_key_arr[ key ].limit_r = r;
	key_limit_dirty = 1;
}

void MeCtGaze::set_all_limits( int key, float pitchUp, float pitchDown, float heading, float roll)
{
	MeCtGaze::DEFAULT_LIMIT_PITCH_UP[key] = pitchUp;
	MeCtGaze::DEFAULT_LIMIT_PITCH_DOWN[key] = pitchDown;
	MeCtGaze::DEFAULT_LIMIT_HEADING[key] = heading;
	MeCtGaze::DEFAULT_LIMIT_ROLL[key] = roll;
}

void MeCtGaze::apply_limit_key( int J, int K, float weight )	{
	
	joint_arr[ J ].limit_p_up = 
		joint_key_arr[ K ].limit_p_up * weight;

	joint_arr[ J ].limit_p_dn = 
		joint_key_arr[ K ].limit_p_dn * weight;

	joint_arr[ J ].limit_h = 
		joint_key_arr[ K ].limit_h * weight;

	joint_arr[ J ].limit_r = 
		joint_key_arr[ K ].limit_r * weight;
}

void MeCtGaze::apply_limit_keys( void )	{
	
	if( key_limit_dirty )	{
		apply_limit_key( GAZE_JOINT_SPINE1, GAZE_KEY_BACK, 0.5f );
		apply_limit_key( GAZE_JOINT_SPINE2, GAZE_KEY_BACK, 0.5f );
		apply_limit_key( GAZE_JOINT_SPINE3, GAZE_KEY_CHEST, 1.0f );
#if GAZE_KEY_COMBINE_HEAD_AND_NECK
		apply_limit_key( GAZE_JOINT_SPINE4, GAZE_KEY_NECK, 0.333333f );
		apply_limit_key( GAZE_JOINT_SPINE5, GAZE_KEY_NECK, 0.333333f );
		apply_limit_key( GAZE_JOINT_SKULL, GAZE_KEY_NECK, 0.333333f );
#else
		apply_limit_key( GAZE_JOINT_SPINE4, GAZE_KEY_NECK, 0.5f );
		apply_limit_key( GAZE_JOINT_SPINE5, GAZE_KEY_NECK, 0.5f );
		apply_limit_key( GAZE_JOINT_SKULL, GAZE_KEY_HEAD, 1.0f );
#endif
		apply_limit_key( GAZE_JOINT_EYE_L, GAZE_KEY_EYES, 1.0f );
		apply_limit_key( GAZE_JOINT_EYE_R, GAZE_KEY_EYES, 1.0f );
		key_limit_dirty = 0;
	}
}

/////////////////////////////////////////////////////////////////////////////

void MeCtGaze::set_blend( int key, float w )	{

	if( ! valid_key( key ) ) return;
	joint_key_arr[ key ].blend_weight = w;
	key_blend_dirty = 1;
}

#if GAZE_KEY_COMBINE_HEAD_AND_NECK
void MeCtGaze::set_blend( float l_w, float t_w, float c_w, float e_w ) {

	joint_key_arr[ GAZE_KEY_BACK ].blend_weight = l_w;
	joint_key_arr[ GAZE_KEY_CHEST ].blend_weight = t_w;
	joint_key_arr[ GAZE_KEY_NECK ].blend_weight = c_w;
	joint_key_arr[ GAZE_KEY_EYES ].blend_weight = e_w;
	key_blend_dirty = 1;
}
#else
void MeCtGaze::set_blend( float l_w, float t_w, float c_w, float h_w, float e_w ) {

	joint_key_arr[ GAZE_KEY_BACK ].blend_weight = l_w;
	joint_key_arr[ GAZE_KEY_CHEST ].blend_weight = t_w;
	joint_key_arr[ GAZE_KEY_NECK ].blend_weight = c_w;
	joint_key_arr[ GAZE_KEY_HEAD ].blend_weight = h_w;
	joint_key_arr[ GAZE_KEY_EYES ].blend_weight = e_w;
	key_blend_dirty = 1;
}
#endif

void MeCtGaze::set_blend( int key1, int key2, float w1, float w2 )	{
	int i;
	
	if( ! valid_key( key1 ) ) return;
	if( ! valid_key( key2 ) ) return;
	if( key1 == key2 )	{
		set_blend( key1, 0.5f * ( w1 + w2 ) );
		return;
	}
	if( key1 > key2 )	{
		int tmp = key1; key1 = key2; key2 = tmp;
		float tmp_w = w1; w1 = w2; w2 = tmp_w;
	}
	for( i = key1; i <= key2; i++ ) {
		float f = (float)( i - key1 )/(float)( key2 - key1 );
		float w = ( 1.0f - f ) * w1 + f * w2;
		set_blend( i, w );
	}
}

void MeCtGaze::apply_blend_keys( void )	{
	
	if( key_blend_dirty )	{

		joint_arr[ GAZE_JOINT_SPINE1 ].blend_weight = 
			joint_key_arr[ GAZE_KEY_BACK ].blend_weight;

		joint_arr[ GAZE_JOINT_SPINE2 ].blend_weight = 
			0.5f * ( joint_key_arr[ GAZE_KEY_BACK ].blend_weight + joint_key_arr[ GAZE_KEY_CHEST ].blend_weight );

		joint_arr[ GAZE_JOINT_SPINE3 ].blend_weight = 
			joint_key_arr[ GAZE_KEY_CHEST ].blend_weight;

#if GAZE_KEY_COMBINE_HEAD_AND_NECK
		joint_arr[ GAZE_JOINT_SPINE4 ].blend_weight = 
		joint_arr[ GAZE_JOINT_SPINE5 ].blend_weight = 
		joint_arr[ GAZE_JOINT_SKULL ].blend_weight = 
			joint_key_arr[ GAZE_KEY_NECK ].blend_weight;

#else
		joint_arr[ GAZE_JOINT_SPINE4 ].blend_weight = 
			joint_key_arr[ GAZE_KEY_NECK ].blend_weight;

		joint_arr[ GAZE_JOINT_SPINE5 ].blend_weight = 
			0.5f * ( joint_key_arr[ GAZE_KEY_NECK ].blend_weight + joint_key_arr[ GAZE_KEY_HEAD ].blend_weight );

		joint_arr[ GAZE_JOINT_SKULL ].blend_weight = 
			joint_key_arr[ GAZE_KEY_HEAD ].blend_weight;
#endif

		joint_arr[ GAZE_JOINT_EYE_L ].blend_weight = 
		joint_arr[ GAZE_JOINT_EYE_R ].blend_weight = 
			joint_key_arr[ GAZE_KEY_EYES ].blend_weight;

		key_blend_dirty = 0;
	}
}

/////////////////////////////////////////////////////////////////////////////
