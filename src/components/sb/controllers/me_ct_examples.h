/*
 *  me_ct_examples.h - part of SmartBody-lib
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

#ifndef ME_CT_EXAMPLES_H
#define ME_CT_EXAMPLES_H

#include <sk/sk_skeleton.h>
#include <sb/SBController.h>

#include "sbm/gwiz_math.h"

#ifndef TRUE
#define TRUE 1
#endif
#ifndef FALSE
#define FALSE 0
#endif

///////////////////////////////////////////////////////////////////////////

class MeCtHeadOrient : public SmartBody::SBController	{
	
	public:
		MeCtHeadOrient();
		virtual ~MeCtHeadOrient();
		
		void init( SbmPawn* pawn );
		void set_orient( float dur, float p, float h, float r );
		

		// Following must be public for comparisons
		static std::string _type_name;

	private:
		virtual void controller_start();
		virtual bool controller_evaluate( double t, MeFrameData& frame );
		
		virtual SkChannelArray& controller_channels() 	{ return( _channels ); }
		virtual double controller_duration()			{ return( (double)_duration ); }
		virtual const std::string& controller_type()			{ return( _type_name ); }
		virtual void print_state( int tabs );
		
		SkChannelArray _channels;
		
		float _duration;
		float _pitch_deg;
		float _heading_deg;
		float _roll_deg;
};

///////////////////////////////////////////////////////////////////////////

class MeCtSimpleTilt : public SmartBody::SBController	{
	
	public:
		MeCtSimpleTilt();
		virtual ~MeCtSimpleTilt();
		
		void init( SbmPawn* pawn );
		void set_tilt( float dur, float angle_deg );
		
		// Following must be public for comparisons
		static std::string _type_name;

	private:
		virtual void controller_start();
		virtual bool controller_evaluate( double t, MeFrameData& frame );
		
		virtual SkChannelArray& controller_channels()	{ return( _channels ); }
		virtual double controller_duration()			{ return( (double)_duration ); }
		virtual const std::string& controller_type()			{ return( _type_name ); }
		virtual void print_state( int tabs );
		
		SkChannelArray _channels;
		
		float _duration;
		float _angle_deg;
};

///////////////////////////////////////////////////////////////////////////

class MeCtSimpleNod : public SmartBody::SBController	{
	
	public:
		enum nod_axis_enum_set	{
			NOD_PITCH,
			NOD_HEADING,
			NOD_ROLL
		};
		enum nod_mode_enum_set	{
			NOD_SIMPLE,
			NOD_WIGGLE,
			NOD_WAGGLE
		};

		MeCtSimpleNod();
		virtual ~MeCtSimpleNod();
		
		void init( SbmPawn* pawn );
		void set_nod( float dur, float mag, float rep, int movementType, float smooth = 0.5 );
//		void set_nod( int axis, float dur, float mag, float rep, float smooth = 0.5 );

		bool isNod();
		bool isShake();
		bool isTilt();
		
		void set_wiggle( 
			int axis,			// sbm:axis // not supported yet
			float dur,			// not used directly by curve algo
			float mag,			// amount : -1.0 - 1.0 : 0.5 * 15 deg
			float period,		// sbm:period : 0.2 - 2.0 : 0.5
			float warp,			// sbm:warp : 0.5 - 1.0 : 1.0
			float accel_pow,	// sbm:accel :  0.5 - 1.5 : 1.5
			float smooth = 0.5	// sbm:smooth : 0.0 - 1.0 : 0.5
		)	{
			_mode = NOD_WIGGLE;
			_axis = axis;

			_duration = dur;
			_magnitude = mag;

			_period = period;
			_warp = warp;
			_accel_pow = accel_pow;

			_smooth = smooth;
		}
		void set_waggle(
			int axis,			// sbm:axis // not supported yet
			float dur,
			float mag,			// amount : -1.0 - 1.0 : 0.5 * 15 deg
			float period,		// sbm:period : 0.2 - 2.0 : 0.5
			float pitch,		// sbm:pitch : -1.0 - 1.0 : 1.0
			float warp,			// sbm:warp : 0.5 - 1.0 : 0.5
			float accel_pow,	// sbm:accel :  1.0 - 3.0 : 1.5
			float decay_pow,	// sbm:decay : 0.5 - 1.5 : 0.5
			float smooth = 0.5
		)	{
			_mode = NOD_WAGGLE;
			_axis = axis;

			_duration = dur;
			_magnitude = mag;

			_period = period;
			_pitch = pitch;
			_warp = warp;
			_accel_pow = accel_pow;
			_decay_pow = decay_pow;

			_smooth = smooth;
		}
		
		// Following must be public for comparisons
		static std::string _type_name;

	private:
		float calc_wiggle_curve( float t, float warp, float accel_pow );
		float calc_waggle_curve( float t, float length, float pitch, float warp, float accel_pow, float decay_pow );
	
		virtual void controller_start();
		virtual bool controller_evaluate( double t, MeFrameData& frame );
		
		virtual SkChannelArray& controller_channels()	{ return( _channels ); }
		virtual double controller_duration()			{ return( (double)_duration ); }
		virtual const std::string& controller_type() const		{ return( _type_name ); }
		virtual void print_state( int tabs );
		
		SkChannelArray _channels;
		
		int _mode;
		int _axis;
		
		float _duration;
		float _magnitude;
		
		float _repetitions;
		int _movementType;
		
		float _period;
		float _pitch;
		float _warp;
		float _accel_pow;
		float _decay_pow;
		
		float _smooth;
		
		double _prev_time;
		bool _first_eval;
};

///////////////////////////////////////////////////////////////////////////
#endif
