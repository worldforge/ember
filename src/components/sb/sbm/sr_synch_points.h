/*
 *  sr_synch_points.h - part of SmartBody-lib
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

#ifndef SR_SYNCH_POINTS_H
#define SR_SYNCH_POINTS_H

#include <sb/SBTypes.h>
#include <stdio.h>
#include <vhcl_log.h>
#include <sbm/sr_linear_curve.h>

/*
	srSynchPoints:

	ASSUMPTIONS:
		Time is a 'double' precision float.
		Any negative time values are interpreted as undefined.
		This code uses -1.0, but all negative inputs are equivalent.
		A negative tag value, or -1, is interpreted as uninitialized.
		Undefined and uninitialied values are not consdiered errors.
		If tag >= NUM_SYNCH_TAGS, this is considered an error.
*/

//////////////////////////////////////////////////////////////////

class srSynchPoints	{

	public:

		enum synch_point_enum_set	{
			START,
			READY,
			STROKE_START,
			STROKE,
			STROKE_STOP,
			RELAX,
			STOP,
			NUM_SYNCH_TAGS
		};
		enum error_enum_set	{
			NO_ERROR_DETECTED,
			BAD_TAG,
			BUMPED_BACK_TAG,
			BUMPED_FWD_TAG,
			NEGATIVE_RAMP,
			UNDEFINED_INTERVAL,
			OVERLAPPED_RAMPS,
			NUM_ERROR_CODES
		};

		int tag_index( const char *label );
		const char * tag_label( const int index );
		const char * error_label( const int index );

	// CPP cruft:
		~srSynchPoints( void ) {}
		srSynchPoints( void )	{
			init();
		}
		srSynchPoints( double start, double stop )	{
			set_time( start, stop );
		}
		srSynchPoints( double start, double stop, double ramp )	{
			set_time( start, stop, ramp );
		}
		srSynchPoints( double start, double ready, double relax, double stop )	{
			set_time( start, ready, relax, stop );
		}
		srSynchPoints(
			double start, double ready, double stroke, double relax, double stop )	{
			set_time( start, ready, stroke, relax, stop );
		}
		srSynchPoints(
			double start, double ready, 
			double st_start, double stroke, double st_stop, 
			double relax, double stop 
		)	{
			set_time( start, ready, st_start, stroke, st_stop, relax, stop );
		}
		srSynchPoints( srSynchPoints & source, double alt_offset = 0.0, double alt_scale = 1.0 )	{
			copy_points( source, alt_offset, alt_scale );
		}

	protected:

	// internal maintenance:

		void init( void );
		bool valid_tag( int tag );		
		bool set_interval( int tag, double t );

	public:

		void set_global_offset( double offset )	{
			global_offset = offset;
		}
		double get_global_offset( void )	{
			return( global_offset );
		}
		
	// set by copy:

		bool copy_points( srSynchPoints & source, double alt_offset = 0.0, double alt_scale = 1.0 );

	// set by explicit time:

		void set_time( double start, double stop = -1.0 );
		void set_time( double start, double stop, double ramp );
		void set_time( double start, double ready, double relax, double stop );
		void set_time( double start, double ready, double stroke, double relax, double stop );
		void set_time( 
			double start, double ready, 
			double st_start, double stroke, double st_stop, 
			double relax, double stop 
		);

	// set by START time and intervals:

		void set_interval( double start_at, double start_to_stop );
		void set_interval( double start_at, double start_to_stop, double ramp );
		void set_interval( double start_at, double start_to_ready, double ready_to_relax, double relax_to_stop );
		void set_interval( 
			double start_at, 
			double start_to_ready, 
			double ready_to_stroke, 
			double stroke_to_relax, 
			double relax_to_stop
		);
		void set_interval( 
			double start_at, 
			double start_to_ready, 
			double ready_to_S_start, // S: stroke_
			double S_start_to_S, 
			double S_to_S_stop, 
#if 0
			int repetitions,
			double S_stop_return_to_S_start,
#endif
			double S_stop_to_relax, 
			double relax_to_stop
		);

	// termination options:
		void set_stop_interval( double ramp )	{
			default_ramp_out = ramp;
		}
		void set_stop( double at_time, double ramp_out );
		void set_stop( double at_time );

	// queries:
		SBAPI bool set_time( int tag, double t );
		SBAPI double get_time( int tag, bool global = false );
		double get_interval( int fr_tag, int to_tag );
		double get_interval_to( int to_tag );
		double get_interval_from( int fr_tag );
		
		double get_length( void ) {
			return( get_interval( START, STOP ) );
		}
		double get_duration( void ) {
			return( get_time( STOP ) );
		}
		double get_prev( int *tag_p, bool global = false );
		double get_next( int *tag_p, bool global = false );
		
		srLinearCurve *get_trapezoid( srLinearCurve *curve_p, double dfl_dur, double dfl_in, double dfl_out );
		srLinearCurve *get_trapezoid( srLinearCurve *curve_p, double dfl_dur = 1.0, double dfl_ramp = 0.0 ) {
			return( get_trapezoid( curve_p, dfl_dur, dfl_ramp, dfl_ramp ) );
		}
		srLinearCurve *new_trapezoid( double dfl_dur, double dfl_in, double dfl_out ) {
			return( get_trapezoid( new srLinearCurve(), dfl_dur, dfl_in, dfl_out ) );
		}
		srLinearCurve *new_trapezoid( double dfl_dur = 1.0, double dfl_ramp = 0.0 ) {
			return( new_trapezoid( dfl_dur, dfl_ramp, dfl_ramp ) );
		}
			
		void print( void );
		void print_error( void );
		int get_error( bool print_out = false );
		
	private:
		
		int err_code;

		double default_ramp_out;
		double implied_ramp_out;
		double global_offset;

		double synch_time_arr[ NUM_SYNCH_TAGS ];
};
#endif
