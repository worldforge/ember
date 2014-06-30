/*
 *  me_ct_eyelid.h - part of SmartBody-lib
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

#ifndef ME_CT_EYELID_H
#define ME_CT_EYELID_H

#include <sk/sk_skeleton.h>
#include <sb/SBController.h>
#include "sbm/sr_linear_curve.h"

//////////////////////////////////////////////////////////////////////////////////

class MeCtEyeLidRegulator : public SmartBody::SBController	{ 

	private:

	class LidSet	{
	
		public:
		
			LidSet( void ) {
				dirty_bit = true;
				base_angle = 0.0f;
				full_angle = 0.0f;
				close_angle = 0.0f;
				diff = 0.0f;
				inv_diff = 0.0f;
				lid_tight = 0.0f;
				open_angle = 0.0f;
				tight_sweep = 0.0f;
				close_sweep = 0.0f;
				eye_pitch = 0.0f;
			}
			~LidSet( void ) {}

			void print( void );

			// specify normalized range in degrees
			void set_range( float fr, float to );
			void get_range(float& from, float& to);
			
			// specify closed/blink angle in degrees
			void set_close( float angle );
			float get_close();
			
			// modifier: 0.0: wide; 0.5: neutral; 0.9: squinting
			void set_tighten( float tighten ); 
			float get_tighten();

			// specify eyeball pitch in degrees
			void set_pitch( float pitch );
			float get_pitch();

			// convert normalized weight (0..1) to output weight with tigthener/eye-pitch
			float get_mapped_weight( float in_weight );

		private:
			void update( void );
			bool  dirty_bit;
			float base_angle;
			float full_angle;
			float close_angle;
			float diff;
			float inv_diff;
			float lid_tight;
			float open_angle;
			float tight_sweep;
			float close_sweep;
			float eye_pitch;
	};

	public:
	
		void test( void );

		static std::string type_name;

		MeCtEyeLidRegulator( void );
		~MeCtEyeLidRegulator( void );
		
		void init(SbmPawn* pawn,  bool tracking_pitch = false);

		void print( void );

		bool get_eyeball_tracking( void ) { return( pitch_tracking ); }
		void set_eyeball_tracking( bool enable )	{
			pitch_tracking = enable;
			if (!pitch_tracking) {
				UL_set.set_pitch( 0.0f );
				UR_set.set_pitch( 0.0f );
#if 0
				LL_set.set_pitch( 0.0f );
				LR_set.set_pitch( 0.0f );
#endif
			}
		}

		void set_upper_range( float fr, float to )	{
			float mn = minval( fr, to );
			float mx = maxval( fr, to );
			UL_set.set_range( mn, mx );
			UR_set.set_range( mn, mx );
		}
	
		void set_lower_range( float fr, float to )	{
			float mn = minval( fr, to );
			float mx = maxval( fr, to );
			LL_set.set_range( mx, mn );
			LR_set.set_range( mx, mn );
		}
		
		void set_close_angle( float angle )	{
			LL_set.set_close( angle );
			LR_set.set_close( angle );
			UL_set.set_close( angle );
			UR_set.set_close( angle );
		}

		void set_upper_tighten( float tight )	{
			hard_upper_tighten = tight;
		}

		void set_lower_tighten( float tight )	{
			hard_lower_tighten = tight;
		}

		void set_upper_delay( float upper_delay)
		{
			upper_lid_delay = upper_delay;
		}

		void set_lower_delay( float lower_delay)
		{
			lower_lid_delay = lower_delay;
		}

		void set_smooth( float upper, float lower )	{
			upper_lid_smooth = upper;
			lower_lid_smooth = lower;
		}

		void blink_now( void ) { new_blink = true; }

		float get_upper_left( bool *changed_p = NULL ) { 
			if( changed_p ) {
				*changed_p = ( UL_value != prev_UL_value );
			}
			return( UL_value ); 
		}
		
		float get_upper_right( bool *changed_p = NULL ) { 
			if( changed_p ) {
				*changed_p = ( UR_value != prev_UR_value );
			}
			return( UR_value ); 
		}

		// uses the "blink" viseme instead of au_45
		void set_use_blink_viseme(bool val);
		bool get_use_blink_viseme();

		void notify(SBSubject* subject);

	private:
		float minval( float a, float b )	{
			if( a < b ) return( a );
			return( b );
		}
		float maxval( float a, float b )	{
			if( a > b ) return( a );
			return( b );
		}

		float smooth( float sm, double dt, float soft, float hard );
		float granular( float in, float min, float max, int grains );
		
		LidSet	UL_set;
		LidSet	LL_set;
		LidSet	UR_set;
		LidSet	LR_set;
		SrQuat          _leftPreRot;	// left eye ball pre-rot
		SrQuat          _rightPreRot;	// right eye ball pre-rot

		srLinearCurve	curve;
		
		bool	pitch_tracking;
		double	prev_time;

		float	upper_lid_smooth;
		float	lower_lid_smooth;

		float	upper_lid_delay;
		float	lower_lid_delay;

		float	hard_upper_tighten;
		float	hard_lower_tighten;
		float	soft_upper_tighten;
		float	soft_lower_tighten;
		float	gran_upper_tighten;
		float	gran_lower_tighten;

		bool	new_blink;
		double	blink_period_min;
		double	blink_period_max;
		double	blink_period;
		double	prev_blink; // time at last blink

		float	prev_UL_value;
		float	prev_LL_value;
		float	prev_UR_value;
		float	prev_LR_value;
		
		float	UL_value;
		float	LL_value;
		float	UR_value;
		float	LR_value;
		
		bool  use_blink_viseme;
		
		SkChannelArray		_channels;

		virtual void context_updated( void );
		virtual void controller_map_updated();
		virtual void controller_start();
		virtual bool controller_evaluate ( double t, MeFrameData& frame );
		virtual SkChannelArray& controller_channels ();
		virtual double controller_duration ();
		virtual const std::string& controller_type () const;
		virtual void print_state( int tabCount );
};

//////////////////////////////////////////////////////////////////////////////////

/*
	char <> softeyes [on|off]
	char <> softeyes weight <upper> <lower>
	char <> softeyes eyepitch|upperlid|lowerlid <upper> <lower>
*/

//////////////////////////////////////////////////////////////////////////////////

class MeCtEyeLid : public SmartBody::SBController	{ 

	private:

		SkChannelArray		_channels; // override motion channels, to include world_offset
		
		float	precision;
		float	lid_weight[ 2 ];
		float	upper_lid_range[ 2 ];
		float	lower_lid_range[ 2 ];
		float	eye_pitch_range[ 2 ];
	
		float minval( float a, float b )	{
			if( a < b ) return( a );
			return( b );
		}
		float maxval( float a, float b )	{
			if( a > b ) return( a );
			return( b );
		}

	public:
		static std::string type_name;

		/*! Constructor */
		MeCtEyeLid( void );

		/*! Destructor is public but pay attention to the use of ref()/unref() */
		virtual ~MeCtEyeLid( void );
		void init( SbmPawn* pawn );
		
		void set_weight( float lo, float up )	{
			lid_weight[ 0 ] = lo;
			lid_weight[ 1 ] = up;
		}
		void get_weight( float &lo, float &up )	{
			lo = lid_weight[ 0 ];
			up = lid_weight[ 1 ];
		}
		
		void set_lower_lid_range( float a, float b ) {
			lower_lid_range[ 0 ] = minval( a, b );
			lower_lid_range[ 1 ] = maxval( a, b );
		}
		void get_lower_lid_range( float &lo, float &up ) {
			lo = lower_lid_range[ 0 ];
			up = lower_lid_range[ 1 ];
		}
		
		void set_upper_lid_range( float a, float b ) {
			upper_lid_range[ 0 ] = minval( a, b );
			upper_lid_range[ 1 ] = maxval( a, b );
		}
		void get_upper_lid_range( float &lo, float &up ) {
			lo = upper_lid_range[ 0 ];
			up = upper_lid_range[ 1 ];
		}
		
		void set_eye_pitch_range( float a, float b )	{
			eye_pitch_range[ 0 ] = maxval( a, b ); // down is positive...
			eye_pitch_range[ 1 ] = minval( a, b );
		}
		void get_eye_pitch_range( float &lo, float &up )	{
			lo = eye_pitch_range[ 0 ];
			up = eye_pitch_range[ 1 ];
		}
		
		
	private:
		float	calc_lid_correction( 
			float in_eye_p, 
			float eye_range[ 2 ], 
			float in_lid_y,
			float lid_range[ 2 ]
		);
			
		virtual void context_updated( void );
		virtual void controller_map_updated();
		virtual void controller_start();
		virtual bool controller_evaluate ( double t, MeFrameData& frame );
		virtual SkChannelArray& controller_channels ();
		virtual double controller_duration ();
		virtual const std::string& controller_type () const;
		virtual void print_state( int tabCount );
};

//////////////////////////////////////////////////////////////////////////////////
#endif // ME_CT_EYELID_H
