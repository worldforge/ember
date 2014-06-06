/*
 *  time_profiler.h - part of SmartBody-lib
 *  Copyright (C) 2010  University of Southern California
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

#ifndef TIME_INTERVAL_PROFILER_H
#define TIME_INTERVAL_PROFILER_H

#include <stdio.h>
#include <iostream>
#include "sbm/sr_hash_map.h"
#include "sbm/time_regulator.h"

#define DEFAULT_BYPASS			true
#define DEFAULT_ENABLED			true
#define DEFAULT_SNIFF			0.9
#define DEFAULT_AVOID			1.5
#define DEFAULT_THRESHOLD		10.0
#define DEFAULT_DECAYING		0.95
#define DEFAULT_ROLLING 		64
#define MAX_SNIFF_DT			0.2

////////////////////////////////////////////////////////////////////////////////////////

class TimeIntervalProfiler { // T.I.P.
	
	private:
		enum time_profiler_enum_set	{
		
#if 0
			LABEL_SIZE =	8192,
			MAX_GROUPS =	256,
			MAX_PROFILES =	256,
			MAX_ROLLING = 	256
#elif 1
			LABEL_SIZE =	512,
			MAX_GROUPS =	32,
			MAX_PROFILES =	32,
			MAX_ROLLING = 	64
#else
			LABEL_SIZE =	512,
			MAX_GROUPS =	1,
			MAX_PROFILES =	1,
			MAX_ROLLING = 	64
#endif
	};

		typedef struct profile_entry_s {

			int 	id;
			int 	index;
			int 	level;
			char	label[ LABEL_SIZE ];
			bool	spike;
			bool	show;
			bool	req_reset;

			double	event_time;
			double	prev_dt;
			double  interval_dt;

			int 	intra_count;
			double	avg_intra_dt;
			double	max_intra_dt;

			double	decay_dt;

			double	accum_roll_dt;
			int 	accum_count;
			double	roll_dt;
			double	roll_dt_arr[ MAX_ROLLING ];
			int 	roll_index;
			
		} profile_entry_t;

		typedef struct group_entry_s {

			int 	id;
			int 	index;
			char	name[ LABEL_SIZE ];
			bool	req_enable;
			bool	req_disable;
			bool	enabled;
			bool 	open;
			bool	open_err;
			
			bool	req_preload;
			bool	preloading;
			bool	spike;
			int		spike_count;
			bool	req_reset;

			double  interval_dt;
			double	decay_dt;
			double	roll_dt;

			srHashMap <profile_entry_t> profile_map;
			profile_entry_t* profile_p_arr[ MAX_PROFILES ];
			bool	full_err;
			
			int 	profile_arr_count;
			int 	active_profile_count;
			int 	profile_event_count;
			profile_entry_t* curr_profile_p;
			
		} group_entry_t;


		srHashMap <group_entry_t> group_map;
		group_entry_t* group_p_arr[ MAX_GROUPS ];
		bool	full_err;
		
		int 	id_counter;
		int 	group_arr_count;
		int 	active_group_count;
		int 	group_event_count;

		bool	sys_bypass;
		bool	dis_sys_bypass;
		bool	enabled;
		bool	preloading;

		double	update_time;
		double	update_dt;

		bool	req_print;
		bool	req_report;
		bool	req_erase;
		bool	req_clobber;
		bool	req_enable;
		bool	req_disable;
		bool	req_preload;
		bool	pending_request;
		
		double	abs_threshold;
		double	rel_threshold;
		bool	dyn_abs_thr;
		bool	dyn_rel_thr;
		double	dyn_sniff; // decaying: positive, less than 1, per second.
		double	dyn_avoid; // bumping: greater than 1: small hiccup, x2... large hiccup, x1.1

		double	decaying_factor;
		int 	rolling_length;
		int 	suppression;
		int 	selection;

		void null( void );
		void null_group( group_entry_t* group_p, const char* group_name = "" );
		void null_profile( profile_entry_t* profile_p, const char* label = "" );
		
		void reset_group( group_entry_t* group_p );
		void reset_profile( profile_entry_t* profile_p );
		
///////////////////////////////////////////////////

	public:

		TimeIntervalProfiler( void ) { reset(); }
		~TimeIntervalProfiler( void ) {}

		void bypass( bool bp ) {
			if( bp )	{
				sys_bypass = true;
			}
			else	{
				bool has_disable = req_disable;
				reset();
				dis_sys_bypass = true;
				req_enable = !has_disable;
				pending_request = true;
			}
		}
		void reset( void )	{
			null();
			sys_bypass = DEFAULT_BYPASS;
			req_enable = DEFAULT_ENABLED;
			rel_threshold = DEFAULT_THRESHOLD;
			dyn_sniff = DEFAULT_SNIFF;
			dyn_avoid = DEFAULT_AVOID;
			decaying_factor = DEFAULT_DECAYING;
			rolling_length = DEFAULT_ROLLING;
		}

		void print_legend( void );

		void print( void )	{
			if( sys_bypass )	{
				printf( "TIP BYPASS: print request ignored\n" );
				return;
			}
			req_print = true;
			pending_request = true;
		}
		void report( void )	{
			if( sys_bypass )	{
				printf( "TIP BYPASS: report request ignored\n" );
				return;
			}
			req_report = true;
			pending_request = true;
		}

		void erase( void )	{
			if( sys_bypass )	{
				printf( "TIP BYPASS: erase request ignored\n" );
				return;
			}
			req_erase = true;
			pending_request = true;
		}
		void enable( bool en )	{
			if( sys_bypass )
				printf( "TIP BYPASS: %s request noted\n", en ? "enable" : "disable" );
			if( en )
				req_enable = true;
			else
				req_disable = true;
			pending_request = true;
		}
		void preload( void )	{
			if( sys_bypass )	{
				printf( "TIP BYPASS: preload request ignored\n" );
				return;
			}
			req_preload = true;
			pending_request = true;
		}

		bool enable( const char* group_name, bool en )	{
			if( sys_bypass )	{
				printf( "TIP BYPASS: group %s request ignored\n", en ? "enable" : "disable" );
				return( false );
			}
			group_entry_t* group_p = get_group( group_name );
			if( group_p )	{
				if( en )
					group_p->req_enable = true;
				else
					group_p->req_disable = true;
				return( true );
			}
			return( false );
		}
		bool preload( const char* group_name )	{
			if( sys_bypass )	{
				printf( "TIP BYPASS: group preload request ignored\n" );
				return( false );
			}
			group_entry_t* group_p = get_group( group_name );
			if( group_p )	{
				group_p->req_preload = true;
				return( true );
			}
			return( false );
		}

		void set_suppression( int sup )	{
			suppression = sup;
			selection = -1;
		}
		void set_selection( int sel )	{
			selection = sel;
			suppression = -1;
		}

		void set_abs_threshold( double delta )	{
			abs_threshold = delta;
			if( abs_threshold <= 0.0 )    {
				abs_threshold = 0.0;
			}
		}
		void set_rel_threshold( double factor ) {
			rel_threshold = factor;
			if( rel_threshold <= 0.0 )    {
				rel_threshold = 100.0;
			}
		}
		void set_dynamic_abs( bool dyn )	{
			dyn_abs_thr = dyn;
		}
		void set_dynamic_rel( bool dyn )	{
			dyn_rel_thr = dyn;
		}

		void set_sniff( double sniff )	{
			if( sniff > 0.0 ) dyn_sniff = sniff;
			if( dyn_sniff >= 1.0 ) dyn_sniff = 0.999;
		}
		void set_avoid( double avoid )	{
			if( avoid > 1.0 ) dyn_avoid = avoid;
			if( dyn_avoid <= 1.0 ) dyn_avoid = 1.01;
		}
		void set_decaying( double s )	{
			decaying_factor = s;
			if( decaying_factor < 0.0 ) decaying_factor = 0.0;
			if( decaying_factor > 0.999 ) decaying_factor = 0.999;
		}
		void set_rolling( int len )	{

			rolling_length = len;
			if( rolling_length < 1 ) rolling_length = 1;
			if( rolling_length > MAX_ROLLING ) rolling_length = MAX_ROLLING;
			
			// for now, just set counts to 0:
			group_entry_t *group_p;
			group_map.reset();
			while( ( group_p = group_map.next() ) != NULL ) {
				profile_entry_t *profile_p;
				group_p->profile_map.reset();
				while( ( profile_p = group_p->profile_map.next() ) != NULL ) {
					profile_p->accum_roll_dt = 0.0;
					profile_p->accum_count = 0;
					profile_p->roll_index = 0;
				}
			}
		}

///////////////////////////////////////////////////

	private:

		void print_data( void );
		void print_profile( profile_entry_t* profile_p );
		void print_group( group_entry_t *group_p );
		void print_profile_report( profile_entry_t *profile_p, int group_id );
		void print_group_report( group_entry_t* group_p );

		void print_profile_alert( double dt, group_entry_t* group_p, profile_entry_t *profile_p );
		void print_group_alert( const char *prefix, double dt, group_entry_t* group_p );
		
		void accum_profile( profile_entry_t *profile_p );
		bool check_profile_spike( profile_entry_t *profile_p );
		bool check_group_spike( group_entry_t* group_p );
		bool check_profile_show( int level );

		void report_recent_groups( group_entry_t* group_p );
		void report_recent_profiles( group_entry_t* group_p );
		void report_current_group( group_entry_t* group_p );

		void sys_update( double time );

		group_entry_t* get_group( const char* group_name );
		profile_entry_t* get_profile( group_entry_t *group_p, const char* label );

		void accum_mark( group_entry_t *group_p, double time );
		void touch_profile( profile_entry_t *profile_p, int level )	{
			if( level > profile_p->level )	{
				profile_p->level = level;
			}
		}
		void touch_group( const char* group_name, int level, const char* label )	{
			group_entry_t *group_p = get_group( group_name );
			profile_entry_t *profile_p = get_profile( group_p, label );
			touch_profile( profile_p, level );
		}
		
		double convert_time( double time )	{
			if( time < 0.0 )	{
				return( SBM_get_real_time() );
			}
			return( time );
		}
		
	public:

		void update( double time ) {
			if( dis_sys_bypass ) { sys_bypass = false; dis_sys_bypass = false; }
			if( sys_bypass ) return;
			sys_update( convert_time( time ) );
		}
		void update( void ) {
			update( -1.0 );
		}

		void mark_time( const char* group_name, int level, const char* label, double time )	{

			if( sys_bypass ) return;
			double curr_time = convert_time( time );
			if( enabled )	{

				group_entry_t *group_p = get_group( group_name );
				if( group_p ) {

					profile_entry_t *profile_p = get_profile( group_p, label );
					if( group_p->enabled )	{

						if( group_p->open ) {  // continuation
							accum_mark( group_p, curr_time );
						}
						else	{  // new segment
							group_p->open = true;
						}
						touch_profile( profile_p, level );
						profile_p->event_time = curr_time;
						group_p->curr_profile_p = profile_p;
					}
					else
					if( group_p->preloading )	{
						touch_profile( profile_p, level );
					}
				}
			}
			else
			if( preloading )	{
				touch_group( group_name, level, label );
			}
		}

		int mark_time( const char* group_name, double time )	{

			if( sys_bypass ) return( 0 );
			double curr_time = convert_time( time );
			if( enabled )	{
				group_entry_t *group_p = group_map.lookup( group_name );
				if( group_p ) {
					if( group_p->enabled )	{
						if( group_p->open ) {  // close...
							accum_mark( group_p, curr_time );
							group_p->open = false;
							group_p->curr_profile_p = NULL;
							return( group_p->profile_event_count );
						}
					}
				}
			}
			return( 0 );
		}

		void mark( const char* group_name, int level, const char* label )	{
			mark_time( group_name, level, label, -1.0 );
		}
		int mark( const char* group_name )	{
			return( mark_time( group_name, -1.0 ) );
		}

		static double test_clock( int reps = 0 );
};

////////////////////////////////////////////////////////////////////////////////////////
#endif
