/*
 *  time_profiler.cpp - part of SmartBody-lib
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

#include <math.h>
#include "sbm/time_profiler.h"
#include "sbm/lin_win.h"
#include <vhcl.h>

////////////////////////////////////////////////////////////////////////////////////////

void TimeIntervalProfiler::null( void )	{

	group_map.expunge();
	for( int i=0; i<MAX_GROUPS; i++ ) group_p_arr[ i ] = NULL;
	full_err = false;
	id_counter = 0;
	group_arr_count = 0;
	active_group_count = 0;
	group_event_count = 0;
	sys_bypass = false;
	dis_sys_bypass = false;
	enabled = false;
	preloading = false;
	update_time = 0.0;
	update_dt = 0.0;
	req_print = false;
	req_report = false;
	req_erase = false;
	req_clobber = false;
	req_enable = false;
	req_disable = false;
	req_preload = false;
	pending_request = false;
	abs_threshold = 0.0;
	rel_threshold = 0.0;
	dyn_abs_thr = false;
	dyn_rel_thr = false;
	dyn_sniff = 1.0;
	dyn_avoid = 1.0;
	decaying_factor = 1.0;
	rolling_length = 1;
	suppression = -1;
	selection = -1;
}

void TimeIntervalProfiler::null_group( group_entry_t* group_p, const char* group_name ) {

	group_p->id = -1;
	group_p->index = -1;
	_snprintf( group_p->name, LABEL_SIZE, "%s", group_name );
	group_p->req_enable = false;
	group_p->req_disable = false;
	group_p->enabled = false;
	group_p->open = false;
	group_p->open_err = false;
	group_p->req_preload = false;
	group_p->preloading = false;
	group_p->spike = false;
	group_p->spike_count = 0;
	group_p->req_reset = false;
	group_p->interval_dt = 0.0;
	group_p->decay_dt = 0.0;
	group_p->roll_dt = 0.0;
	group_p->profile_map.expunge();
	for( int i=0; i<MAX_PROFILES; i++ ) group_p->profile_p_arr[ i ] = NULL;
	group_p->full_err = false;
	group_p->profile_arr_count = 0;
	group_p->active_profile_count = 0;
	group_p->profile_event_count = 0;
	group_p->curr_profile_p = NULL;
}

void TimeIntervalProfiler::null_profile( profile_entry_t* profile_p, const char* label ) {

	profile_p->id = -1;
	profile_p->index = -1;
	profile_p->level = -1;
	_snprintf( profile_p->label, LABEL_SIZE, "%s", label );
	profile_p->spike = false;
	profile_p->show = false;
	profile_p->req_reset = false;
	profile_p->event_time = 0.0;
	profile_p->prev_dt = 0.0;
	profile_p->interval_dt = 0.0;
	profile_p->intra_count = 0;
	profile_p->avg_intra_dt = 0.0;
	profile_p->max_intra_dt = 0.0;
	profile_p->decay_dt = 0.0;
	profile_p->accum_roll_dt = 0.0;
	profile_p->accum_count = 0;
	profile_p->roll_dt = 0.0;
	for( int i=0; i<MAX_ROLLING; i++ ) profile_p->roll_dt_arr[ i ] = 0.0;
	profile_p->roll_index = 0;
}

void TimeIntervalProfiler::reset_group( group_entry_t* group_p ) {

	group_p->spike = false;
	group_p->spike_count = 0;
	group_p->req_reset = false;
	group_p->interval_dt = 0.0;
	group_p->decay_dt = 0.0;
	group_p->roll_dt = 0.0;
	group_p->active_profile_count = 0;
	group_p->profile_event_count = 0;
}

void TimeIntervalProfiler::reset_profile( profile_entry_t* profile_p ) {

	profile_p->level = -1;
	profile_p->event_time = 0.0;
	profile_p->spike = false;
	profile_p->req_reset = false;
	profile_p->prev_dt = profile_p->interval_dt;
	profile_p->interval_dt = 0.0;
	profile_p->intra_count = 0;
	profile_p->max_intra_dt = 0.0;
}

////////////////////////////////////////////////////////////////////////////////////////

void TimeIntervalProfiler::print_legend( void )	{
	
	LOG( "TIP <> legend:" );								LOG( "\n" );
	LOG( "#	- overflow groups and profiles" );			LOG( "\n" );
	LOG( "@	- profile Active" );						LOG( "\n" );
	LOG( "*	- *spike reported in active profile" );		LOG( "\n" );
	LOG( "+	- included in +SUM" );						LOG( "\n" );
	LOG( "<>	- priority level" );						LOG( "\n" );
	LOG( "()	- (2)==(number of items, or items)" );		LOG( "\n" );
	LOG( "[]	- percent" );					LOG( "\n" );
	LOG( "e	- Event count" );				LOG( "\n" );
	LOG( "a	- Active count" );				LOG( "\n" );
	LOG( "c	- array Count" );				LOG( "\n" );
	LOG( "h	- Hash count" );				LOG( "\n" );
	LOG( "T	- event Time" );				LOG( "\n" );
	LOG( "i	- raw Interval" );				LOG( "\n" );
	LOG( "r	- Rolling average count" );		LOG( "\n" );
	LOG( "R	- Rolling average" );			LOG( "\n" );
	LOG( "D	- Decaying average" );			LOG( "\n" );
	LOG( "F	- %% of whole Frame taken up by interval or group" );	LOG( "\n" );
	LOG( "G	- %% of whole Group time taken up by interval" );		LOG( "\n" );
	LOG( "M(rep)	- %% of interval taken up by Maximum repetition" );	LOG( "\n" );
	LOG( "--tip\n" );
}

////////////////////////////////////////////////////////////////////////////////////////

void TimeIntervalProfiler::print_profile( profile_entry_t* profile_p )	{

	if( profile_p ) {

		LOG( "%s%s%s   <%d>: T:%.4f (r:%d) %s",
			( profile_p->index < 0 ) ? "#" : " ",
			( profile_p->req_reset == false ) ? "@" : " ",
			( profile_p->spike ) ? "*" : " ",
			profile_p->level,
			profile_p->event_time,
			profile_p->accum_count,
			profile_p->label
		);
		if( profile_p->intra_count > 1 )	{
			LOG( " (e:%d)", profile_p->intra_count );
		}
		LOG( "\n" );
	}
}

void TimeIntervalProfiler::print_group( group_entry_t *group_p )	{

	if( group_p )	{

		LOG( 
			"%s%s%s GRP: %s (e:%d, a:%d, c:%d, h:%d) :%s\n",
			( group_p->index < 0 ) ? "#" : " ",
			( group_p->req_reset == false ) ? "@" : " ",
			( group_p->spike ) ? "*" : " ",
			group_p->name,
			group_p->profile_event_count,
			group_p->active_profile_count,
			group_p->profile_arr_count,
			group_p->profile_map.get_num_entries(),
			group_p->enabled ? "ENABLED" : "DISABLED"
		);

		for( int i=0; i< group_p->profile_arr_count; i++ )	{
			print_profile( group_p->profile_p_arr[ i ] );
		}

		profile_entry_t *profile_p;
		group_p->profile_map.reset();
		while( ( profile_p = group_p->profile_map.next() ) != NULL ) {
			if( profile_p->index < 0 )	{
				print_profile( profile_p );
			}
		}
	}
}

void TimeIntervalProfiler::print_data( void )	{

	LOG( "TIP <>: %s\n", enabled ? "ENABLED" : "DISABLED" );
	LOG( "  suppress: %d\n", suppression );
	LOG( "    select: %d\n", selection );
	LOG( "  averages:\n" );
	LOG( "   decaying: %f\n", decaying_factor );
	LOG( "    rolling: %d\n", rolling_length );
	LOG( " threshold:\n" );
	LOG( "        abs: %f :%s\n", abs_threshold, dyn_abs_thr ? "dyn" : "fix" );
	LOG( "        rel: %f :%s\n", rel_threshold, dyn_rel_thr ? "dyn" : "fix" );
	LOG( "   dynamic:\n" );
	LOG( "      sniff: %f\n", dyn_sniff );
	LOG( "      avoid: %f\n", dyn_avoid );
	LOG( "--\n" );
	LOG( 
		"TIP groups (e:%d, a:%d, c:%d, h:%d):\n", 
		group_event_count,
		active_group_count,
		group_arr_count,
		group_map.get_num_entries()
	);

	for( int i=0; i< group_arr_count; i++ )	{
		print_group( group_p_arr[ i ] );
	}

	group_entry_t *group_p;
	group_map.reset();
	while( ( group_p = group_map.next() ) != NULL ) {
		if( group_p->index < 0 )	{
			print_group( group_p );
		}
	}
	LOG( "--tip\n" );
}

////////////////////////////////////////////////////////////////////////////////////////

void TimeIntervalProfiler::print_profile_report( profile_entry_t *profile_p, int group_id )	{

	LOG( 
		"%s%s%s%s<%d>: i:%.5f (D:%.5f, R:%.5f, r:%d)",
		( profile_p->index < 0 ) ? "#" : " ",
		( profile_p->req_reset == false ) ? "@" : " ",
		( profile_p->spike ) ? "*" : " ",
		( profile_p->id == group_id ) ? "+" : " ",
		profile_p->level,
		profile_p->interval_dt,
		profile_p->decay_dt,
		profile_p->roll_dt,
		profile_p->accum_count
	);
	if( profile_p->intra_count > 1 )	{
		LOG( 
			" [M(%d):%.1f]",
			profile_p->intra_count,
			100.0 * profile_p->max_intra_dt / profile_p->interval_dt
		);
	}
	LOG( " %s",
		profile_p->label
	);
	LOG( "\n" );
}

void TimeIntervalProfiler::print_group_report( group_entry_t* group_p )	{

	LOG( 
		"%s %s SUM: i:%.5f (D:%.5f, R:%.5f)\n",
		( group_p->index < 0 ) ? "#" : " ",
		( group_p->spike ) ? "*" : " ",
		group_p->interval_dt,
		group_p->decay_dt,
		group_p->roll_dt
	);
}

void TimeIntervalProfiler::print_profile_alert( double dt, group_entry_t* group_p, profile_entry_t *profile_p )	{
	
	LOG( 
		"TIP %s <%d>: i:%.5f [G:%.1f, F:%.3f",
		( profile_p->index < 0 ) ? "#" : "-",
		profile_p->level,
		profile_p->interval_dt,
		100.0 * profile_p->interval_dt / group_p->interval_dt,
		100.0 * profile_p->interval_dt / dt
	);
	if( profile_p->intra_count > 1 )	{
		LOG( 
			", M(%d):%.1f",
			profile_p->intra_count,
			100.0 * profile_p->max_intra_dt / profile_p->interval_dt
		);
	}
	LOG( "] (r:%d) %s:%s\n",
		profile_p->accum_count,
		group_p->name, 
		profile_p->label 
	);
}
void TimeIntervalProfiler::print_group_alert( const char *prefix, double dt, group_entry_t* group_p )	{

	LOG( 
		"TIP %s %s: i:%.5f [F:%.3f] (D:%.5f, R:%.5f) %s (a:%d)\n",
		( group_p->index < 0 ) ? "#" : "-",
		prefix,
		group_p->interval_dt,
		100.0 * group_p->interval_dt / dt,
		group_p->decay_dt,
		group_p->roll_dt,
		group_p->name,
		group_p->active_profile_count
	);
}

///////////////////////////////////////////////////

void TimeIntervalProfiler::accum_profile( profile_entry_t *profile_p )	{

	profile_p->avg_intra_dt = profile_p->interval_dt / (double) profile_p->intra_count;
	
	if( profile_p->decay_dt == 0.0 )	{
		profile_p->decay_dt = profile_p->interval_dt;
	}
	else	{
		profile_p->decay_dt =
			decaying_factor * profile_p->decay_dt +
			( 1.0 - decaying_factor ) * profile_p->interval_dt;
	}
	
	profile_p->accum_roll_dt += profile_p->interval_dt;
	if( profile_p->accum_count >= rolling_length )	{
		profile_p->accum_roll_dt -= profile_p->roll_dt_arr[ profile_p->roll_index ];
	}
	else	{
		profile_p->accum_count++;
	}
	
	profile_p->roll_dt_arr[ profile_p->roll_index ] = profile_p->interval_dt;
	profile_p->roll_index++;
	if( profile_p->roll_index >= rolling_length )	{
		profile_p->roll_index = 0;
	}
	
	profile_p->roll_dt = profile_p->accum_roll_dt / (double)profile_p->accum_count;
}

bool TimeIntervalProfiler::check_profile_spike( profile_entry_t *profile_p )	{

	// absolute 
	if( abs_threshold > 0.0 )	{
		if( profile_p->interval_dt > abs_threshold )	{
			return( true );
		}
	}
	// harsh
#if 0
	if(
		( profile_p->intra_count > 1 )&&
		( profile_p->max_intra_dt > ( profile_p->prev_dt * rel_threshold ) )
	)	{
		return( true );
	}
#endif
	// smooth harsh
	if( profile_p->interval_dt > ( profile_p->decay_dt * rel_threshold ) )	{
		return( true );
	}
	// fair
	if( profile_p->interval_dt > ( profile_p->roll_dt * rel_threshold ) )	{
		return( true );
	}
	return( false );
}

bool TimeIntervalProfiler::check_group_spike( group_entry_t* group_p )  {

	if( abs_threshold > 0.0 )	{
		if( group_p->interval_dt > abs_threshold )	{
			return( true );
		}
	}
	if( group_p->interval_dt > ( group_p->decay_dt * rel_threshold ) )	{
			return( true );
	}
	if( group_p->interval_dt > ( group_p->roll_dt * rel_threshold ) )	{
			return( true );
	}
	return( false );
}

bool TimeIntervalProfiler::check_profile_show( int level )	{

	return (
		( selection < 0 )&&
		( level > suppression )
	)||(
		( suppression < 0 )&&
		( selection > -1 )&&
		( level == selection )
	);
}

///////////////////////////////////////////////////

void TimeIntervalProfiler::report_recent_groups( group_entry_t* group_p ) {

	if( group_p->req_reset == true ) {

		LOG( "%s GRP: %s (e:%d, a:%d) :%s\n",
			( group_p->index < 0 ) ? "#" : " ",
			group_p->name,
			group_p->profile_event_count,
			group_p->active_profile_count,
			group_p->enabled ? "ENABLED" : "DISABLED"
		);

		profile_entry_t *profile_p;
		for( int j=0; j< group_p->profile_arr_count; j++ ) {

			profile_p = group_p->profile_p_arr[ j ];
			if( profile_p->req_reset == true )	{

				print_profile_report( profile_p, group_p->id );
			}
		}

		group_p->profile_map.reset();
		while( ( profile_p = group_p->profile_map.next() ) != NULL ) {

			if( profile_p->index < 0 )	{
				if( profile_p->req_reset == true )	{
				
					print_profile_report( profile_p, group_p->id );
				}
			}
		}
		print_group_report( group_p );
	}
}

void TimeIntervalProfiler::report_recent_profiles( group_entry_t* group_p ) {

	if( group_p->req_reset == false ) {
		profile_entry_t *profile_p;

		bool found_prev = false;
		for( int j=0; !found_prev &&( j< group_p->profile_arr_count ); j++ ) {
			if( group_p->profile_p_arr[ j ]->req_reset == true ) found_prev = true;
		}
		group_p->profile_map.reset();
		while( ( !found_prev )&&( ( profile_p = group_p->profile_map.next() ) != NULL ) ) {
			if( profile_p->req_reset == true ) found_prev = true;
		}
		
		if( found_prev )	{

			LOG( "%s GRP: %s (e:%d, a:%d) :%s\n",
				( group_p->index < 0 ) ? "#" : " ",
				group_p->name,
				group_p->profile_event_count,
				group_p->active_profile_count,
				group_p->enabled ? "ENABLED" : "DISABLED"
			);

			for( int j=0; j< group_p->profile_arr_count; j++ ) {

				profile_p = group_p->profile_p_arr[ j ];
				if( profile_p->req_reset == true )	{
				
					print_profile_report( profile_p, group_p->id );
				}
			}

			group_p->profile_map.reset();
			while( ( profile_p = group_p->profile_map.next() ) != NULL ) {

				if( profile_p->index < 0 )	{
					if( profile_p->req_reset == true )	{
					
						print_profile_report( profile_p, group_p->id );
					}
				}
			}
			print_group_report( group_p );
		}
	}
}

void TimeIntervalProfiler::report_current_group( group_entry_t* group_p ) {

	if( group_p->enabled )	{
		if( group_p->req_reset == false ) {
			profile_entry_t *profile_p;
 
			if( req_report )	{
				LOG( "%s GRP: %s (e:%d, a:%d)\n",
					( group_p->index < 0 ) ? "#" : " ",
					group_p->name,
					group_p->profile_event_count,
					group_p->active_profile_count
				);
			}

			for( int j=0; j< group_p->profile_arr_count; j++ ) {

				profile_p = group_p->profile_p_arr[ j ];
				if( profile_p->req_reset == false )	{

					if( req_report )	{
						print_profile_report( profile_p, group_p->id );
					}
					else
					if( profile_p->spike && profile_p->show )	{
						print_profile_alert( update_dt, group_p, profile_p );
					}
				}
			}

			group_p->profile_map.reset();
			while( ( profile_p = group_p->profile_map.next() ) != NULL ) {

				if( profile_p->index < 0 )	{
					if( profile_p->req_reset == false )	{

						if( req_report )	{
							print_profile_report( profile_p, group_p->id );
						}
						else
						if( profile_p->spike && profile_p->show )	{
							print_profile_alert( update_dt, group_p, profile_p );
						}
					}
				}
			}

			if( req_report )	{
				print_group_report( group_p );
			}
			else
			if( group_p->spike_count > 0 )	{
				print_group_alert( "SUM", update_dt, group_p );
			}
			else
			if( group_p->spike )	{
				print_group_alert( "GRP", update_dt, group_p );
			}
		}
	}
}

///////////////////////////////////////////////////

void TimeIntervalProfiler::sys_update( double time ) {

	double prev = update_time;
	update_dt = time - prev;
	if( update_dt > 0.0 )	{
		update_time += update_dt;
	}
	else	{
		LOG( "TimeIntervalProfiler::sys_update WARN: negative input dt: %f\n", update_dt );
		return;
	}

	group_entry_t *group_p = NULL;

	if( enabled )	{

	// Check arrays:
		for( int i=0; i< group_arr_count; i++ ) {

			group_p = group_p_arr[ i ];
			if( group_p == NULL ) {
				LOG( "TimeIntervalProfiler::sys_update ERR: NULL group_p in group_p_arr[ %d ]\n", i );
				return;
			}
			for( int j=0; j< group_p->profile_arr_count; j++ ) {

				profile_entry_t *profile_p = group_p->profile_p_arr[ j ];
				if( profile_p == NULL ) {
					LOG( "TimeIntervalProfiler::sys_update ERR: NULL profile_p in group_p_arr[ %d ][ %d ]\n", i, j );
					return;
				}
			}
		}
		
	// Close hanging intervals:
		group_map.reset();
		while( ( group_p = group_map.next() ) != NULL ) {
			if( group_p->enabled )	{
				if( group_p->open ) {  // close...
					if( group_p->open_err == false ) {
						LOG( "TIP WARN: dangling label: %s:%s\n", group_p->name, group_p->curr_profile_p->label );
						group_p->open_err = true;
					}
					mark_time( group_p->name, time );
				}
			}
		}
	}

	if( pending_request )	{
		if( req_print ) {
			print_data();
			req_print = false;
		}

	// Report Previous
		if( req_report )  {

			LOG( "TIP <> report:\n" );
			LOG( "RECENT GROUPS:\n" );
			for( int i=0; i< group_arr_count; i++ ) {
				report_recent_groups( group_p_arr[ i ] );
			}
			group_map.reset();
			while( ( group_p = group_map.next() ) != NULL ) {
				if( group_p->index < 0 )	{
					report_recent_groups( group_p );
				}
			}
			LOG( "RECENT PROFILES:\n" );
			for( int i=0; i< group_arr_count; i++ ) {
				report_recent_profiles( group_p_arr[ i ] );
			}
			group_map.reset();
			while( ( group_p = group_map.next() ) != NULL ) {
				if( group_p->index < 0 )	{
					report_recent_profiles( group_p );
				}
			}
		}
	}

	if( enabled )	{

		int total_spike_count = 0;

	// Accumulate:
		group_map.reset();
		while( ( group_p = group_map.next() ) != NULL ) {
			
			if( group_p->enabled )	{
				if( group_p->req_reset == false ) {

					profile_entry_t *profile_p;
					group_p->profile_map.reset();
					while( ( profile_p = group_p->profile_map.next() ) != NULL ) {

						if( profile_p->req_reset == false )	{

							if( profile_p->interval_dt < 0.0 )    {
								LOG( "TimeIntervalProfiler::sys_update WARN: negative dt: %f\n", profile_p->interval_dt );
							}
							accum_profile( profile_p );
							group_p->interval_dt += profile_p->interval_dt;
							group_p->decay_dt += profile_p->decay_dt;
							group_p->roll_dt += profile_p->roll_dt;
						}
					}
					group_p->profile_map.reset();
					while( ( profile_p = group_p->profile_map.next() ) != NULL ) {
					
						if( profile_p->req_reset == false )	{

							profile_p->spike = check_profile_spike( profile_p );
							profile_p->show = check_profile_show( profile_p->level );
							if( profile_p->spike )	{
								if( req_report || profile_p->show )	{
									group_p->spike_count++;
								}
							}
						}
					}
					group_p->spike = check_group_spike( group_p );
					if( group_p->spike_count == 0 )	{
						if( group_p->spike )	{
							total_spike_count++;
						}
					}
					total_spike_count += group_p->spike_count;
				}
			}
		}

	// Report Current:
		if( req_report )	{
			LOG( "ACTIVE: (e:%d, a:%d)\n",
				group_event_count,
				active_group_count
			);
		}
		for( int i=0; i< group_arr_count; i++ ) {
			report_current_group( group_p_arr[ i ] );
		}
		group_map.reset();
		while( ( group_p = group_map.next() ) != NULL ) {
			if( group_p->index < 0 )	{
				report_current_group( group_p );
			}
		}
		if( req_report )	{
			LOG( "--tip\n" );
		}

	// Reset:
		group_map.reset();
		while( ( group_p = group_map.next() ) != NULL ) {

			if( group_p->enabled )	{
				group_p->profile_map.reset();
				profile_entry_t *profile_p;
				while( ( profile_p = group_p->profile_map.next() ) != NULL ) {
					profile_p->req_reset = true;
				}
				group_p->req_reset = true;
			}
			if( group_p->req_enable )	{
				group_p->enabled = true;
				group_p->req_enable = false;
			}
			if( group_p->req_disable )	{
				group_p->enabled = false;
				group_p->req_disable = false;
			}
			group_p->preloading = false;
			if( group_p->req_preload )	{
				group_p->preloading = true;
			}
		}
		active_group_count = 0;
		group_event_count = 0;

	// Sniff:
		if( dyn_abs_thr || dyn_rel_thr )	{
		
			double avoid = 1.0;
			if( total_spike_count > 0 ) {
				avoid = dyn_avoid;
			}
			double clamp_dt = ( update_dt < MAX_SNIFF_DT ) ? update_dt : MAX_SNIFF_DT;
			double sniff = 1.0 - clamp_dt * ( 1.0 - dyn_sniff );
			if( dyn_abs_thr &&( abs_threshold > 0.0 ) ) {
				abs_threshold *= avoid * sniff;
			}
			if( dyn_rel_thr )	{
				rel_threshold *= avoid * sniff;
			}
		}
	}

	if( pending_request )	{
	// Wrapup:
		req_report = false;
		if( req_enable )	{
			enabled = true;
			req_enable = false;
		}
		if( req_disable )	{
			enabled = false;
			req_disable = false;
		}
		if( req_erase ) {
			group_map.reset();
			while( ( group_p = group_map.next() ) != NULL ) {
				null_group( group_p, group_p->name );
			}
			req_erase = false;
		}
		preloading = false;
		if( req_preload )	{
			preloading = true;
		}
		pending_request = false;
	}
	id_counter++;
}

///////////////////////////////////////////////////

TimeIntervalProfiler::group_entry_t* 
TimeIntervalProfiler::get_group( const char* group_name ) {

	group_entry_t *group_p = group_map.lookup( group_name );
	if( group_p == NULL ) {

		group_p = new group_entry_t;
		null_group( group_p, group_name );
		group_p->enabled = true;
		group_map.insert( group_name, group_p, true );

		if( group_arr_count < MAX_GROUPS )	{
			group_p->index = group_arr_count;
			group_p_arr[ group_arr_count ] = group_p;
			group_arr_count++;
		}
		else
		if( full_err == false )	{
			full_err = true;
			LOG( "TimeIntervalProfiler::get_group WARN: MAX_GROUPS: %d", MAX_GROUPS );
			LOG( "\n" );
		}
	}
	else
	if( group_p->req_reset ) {
		reset_group( group_p );
	}
	group_p->id = id_counter;
	return( group_p );
}

TimeIntervalProfiler::profile_entry_t* 
TimeIntervalProfiler::get_profile( group_entry_t *group_p, const char* label ) {

	profile_entry_t *profile_p = group_p->profile_map.lookup( label );
	if( profile_p == NULL ) {

		profile_p = new profile_entry_t;
		null_profile( profile_p, label );
		group_p->profile_map.insert( label, profile_p, true );

		if( group_p->profile_arr_count < MAX_PROFILES ) {
			profile_p->index = group_p->profile_arr_count;
			group_p->profile_p_arr[ group_p->profile_arr_count ] = profile_p;
			group_p->profile_arr_count++;
		}
		else
		if( group_p->full_err == false )	{
			group_p->full_err = true;
			LOG( "TimeIntervalProfiler::get_profile WARN: MAX_PROFILES: %d", MAX_PROFILES );
			LOG( "\n" );
		}
	}
	else
	if( profile_p->req_reset ) {
		reset_profile( profile_p );
	}
	profile_p->id = group_p->id;
	return( profile_p );
}

void TimeIntervalProfiler::accum_mark( group_entry_t *group_p, double curr_time )	{

	profile_entry_t *profile_p = group_p->curr_profile_p;
	if( profile_p )	{

		if( group_p->active_profile_count == 0 ) {
			active_group_count++;
		}
		if( profile_p->intra_count == 0 )   {
			group_p->active_profile_count++;
		}

		double dt = curr_time - profile_p->event_time;
		if( dt < 0.0 )    { 
			LOG( "TimeIntervalProfiler::accum_mark WARN: negative dt: %f\n", dt );
		}
		if( dt > profile_p->max_intra_dt )	{
			profile_p->max_intra_dt = dt;
		}

		profile_p->interval_dt += dt;
		profile_p->intra_count++;

		group_p->profile_event_count++;
		group_event_count++;
		return;
	}
	LOG( "TimeIntervalProfiler::accum_mark ERR: no current profile" );
}

///////////////////////////////////////////////////

double TimeIntervalProfiler::test_clock( int reps )	{

	if( reps < 1 ) reps = 1000000;

	double *time_hist_arr = new double[ reps ];
	double *time_hist_end_p = time_hist_arr + reps;
	
	int countdown = reps;
	double start = SBM_get_real_time();
	while( countdown )    {
		*( time_hist_end_p - ( countdown-- ) ) = SBM_get_real_time();
	}
	
	double *dt_hist_arr = new double[ reps ];
	double t = start;
	double prev = 0.0;
	for( int i=0; i<reps; i++ )	{
		prev = t;
		t = time_hist_arr[ i ];
		dt_hist_arr[ i ] = t - prev;
	}
	delete [] time_hist_arr;
	double total = t - start;
	double avg = total / (double)reps;

	int negs = 0;
	int hits = 0;
	int run = 0;
	int run_min = 999999999;
	int run_max = 0;
	int accum_run = 0;

	int halt = 0;
	int halt_min = 999999999;
	int halt_max = 0;
	int accum_halt = 0;

	double min = 999999999.0;
	double max = 0.0;
	double accum_dev = 0.0;
	for( int i=0; i<reps; i++ )	{

		double dt = dt_hist_arr[ i ];

#if 0
		if( i % 10 == 0 ) dt = 0.0;
#endif
#if 0
		if( ( (double)rand() / (double)RAND_MAX ) > 0.5 ) dt = 0.0;
#endif

		if( min > dt ) min = dt;
		if( max < dt ) max = dt;
		accum_dev += fabs( dt - avg );

		if( dt > 0.0 )	{
			
			if( halt )	{
				if( halt_min > halt ) halt_min = halt;
				if( halt_max < halt ) halt_max = halt;
				accum_halt += halt;
				halt = 0;
			}
			run++;
			hits++;
		}
		else	{

			if( dt < 0.0 )	{
				negs++;
			}
			if( run )	{
				if( run_min > run ) run_min = run;
				if( run_max < run ) run_max = run;
				accum_run += run;
				run = 0;
			}
			halt++;
		}
	}

	if( run )	{
		if( run_min > run ) run_min = run;
		if( run_max < run ) run_max = run;
		accum_run += run;
	}
	if( halt )	{
		if( halt_min > halt ) halt_min = halt;
		if( halt_max < halt ) halt_max = halt;
		accum_halt += halt;
	}
	double dev = accum_dev / (double)reps;
	double hit_ratio = (double)hits / (double)reps;
	double run_avg = (double)accum_run / (double)reps;
	double halt_avg = (double)accum_halt / (double)reps;

#if 0
	for( int i=0; i<reps; i++ )	{

		double dt = dt_hist_arr[ i ];

		// deviation in run/halt length
	}
#endif
	delete [] dt_hist_arr;

	LOG( "test_clock: %d reps\n", reps );
	LOG( " Tot: %.6f\n", total );
	LOG( "--\n" );

	LOG( " avg: %.12f\n", avg );
	if( min < max ) {
		if( min == 0.0 )	{
			LOG( " min: <zero>\n" );
		}
		else	{
			LOG( " min: %.12f\n", min );
		}
		LOG( " max: %.12f\n", max );
	}
	LOG( " dev: %.12f\n", dev );
	if( negs )	{
		LOG( " neg: %d\n", negs );
	}
	LOG( "--\n" );

	LOG( " fps: %.2f\n", 1.0 / avg );
	if( max > 0.0 )
		LOG( " min: %.2f\n", 1.0 / max );
	else
		LOG( " min: <inf>\n" );
	if( min > 0.0 )
		LOG( " max: %.2f\n", 1.0 / min );
	else
		LOG( " max: <inf>\n" );
	LOG( "--\n" );

	LOG( " hit: %.6f\n", hit_ratio );
	LOG( " avg: %.2f\n", run_avg );
	if( run_min < run_max ) {
		LOG( " min: %d\n", run_min );
		LOG( " max: %d\n", run_max );
	}
	LOG( "--\n" );

	LOG( " mis: %d\n", accum_halt );
	LOG( " avg: %.2f\n", halt_avg );
	if( halt_min < halt_max ) {
		LOG( " min: %d\n", halt_min );
		LOG( " max: %d\n", halt_max );
	}
	LOG( "--\n" );

	double resolution = avg / hit_ratio;
	LOG( " Res: %.12f == %.2f fps\n", resolution, 1.0 / resolution );
	LOG( "--\n" );

	return( resolution );
}
