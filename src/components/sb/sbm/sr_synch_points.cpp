/*
 *  sr_synch_points.cpp - part of SmartBody-lib
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

#include <sbm/sr_synch_points.h>
#include <math.h>
#include <sbm/lin_win.h>


//////////////////////////////////////////////////////////////////

int srSynchPoints::tag_index( const char *label )	{

	if( label )	{
		if( _stricmp( label, "start" ) == 0 )			return( START );
		if( _stricmp( label, "ready" ) == 0 )			return( READY );
		if( _stricmp( label, "stroke_start" ) == 0 )	return( STROKE_START );
		if( _stricmp( label, "stroke" ) == 0 )			return( STROKE );
		if( _stricmp( label, "stroke_stop" ) == 0 ) 	return( STROKE_STOP );
		if( _stricmp( label, "relax" ) == 0 )			return( RELAX );
		if( _stricmp( label, "stop" ) == 0 )			return( STOP );
		if( _stricmp( label, "end" ) == 0 )				return( STOP );
	}
	return( -1 ); // default err
}

const char * srSynchPoints::tag_label( const int index ) {

	switch( index )	{
		case START: 		return( "start" );
		case READY: 		return( "ready" );
		case STROKE_START:	return( "stroke_start" );
		case STROKE:		return( "stroke" );
		case STROKE_STOP:	return( "stroke_stop" );
		case RELAX: 		return( "relax" );
		case STOP:			return( "end" );
	}
	return( "UNKNOWN" ); // default err
}

const char * srSynchPoints::error_label( const int index ) {

	switch( err_code )	{
		case NO_ERROR_DETECTED: 	return( "NONE" );
		case BAD_TAG: 				return( "BAD_TAG" );
		case BUMPED_BACK_TAG:		return( "BUMPED_BACK_TAG" );
		case BUMPED_FWD_TAG:		return( "BUMPED_FWD_TAG" );
		case NEGATIVE_RAMP:			return( "NEGATIVE_RAMP" );
		case UNDEFINED_INTERVAL:	return( "UNDEFINED_INTERVAL" );
		case OVERLAPPED_RAMPS:		return( "OVERLAPPED_RAMPS" );
	}
	return( "UNKNOWN" ); // default err
}

//////////////////////////////////////////////////////////////////

void srSynchPoints::init( void )	{

	for( int i=0; i<NUM_SYNCH_TAGS; i++ )	{
		synch_time_arr[ i ] = -1.0;
	}
	global_offset = 0.0;
	implied_ramp_out = 0.0;
	default_ramp_out = -1.0;
	err_code = NO_ERROR_DETECTED;
}

bool srSynchPoints::valid_tag( int tag )	{

	if( tag < 0 ) return( false );
	if( tag < NUM_SYNCH_TAGS ) return( true );
	err_code = BAD_TAG;
	return( false );
}

//////////////////////////////////////////////////////////////////

bool srSynchPoints::set_time( int tag, double t )	{

#if 1
	if( valid_tag( tag ) )	{
		synch_time_arr[ tag ] = t;
		if( t < 0.0 )	{
			return( true ); // assigning negative is acceptable.
		}
	}
	else	{
		return( false );
	}
	return false;
// feng : If there are errors in the input sync points, the order adjustment usually makes it worse instead of fix it. So disable for now.
/*
// check and bump back order of preceding:
	int tmp_tag = tag;
	double tmp_time = get_prev( & tmp_tag );
	if( tmp_time >= 0.0 )	{
		if( tmp_time > t )	{
			set_time( tmp_tag, t );
			err_code = BUMPED_BACK_TAG;
		}
	}

// check and bump forward order of succeeding:
	tmp_tag = tag;
	tmp_time = get_next( & tmp_tag );
	if( tmp_time >= 0.0 )	{
		if( tmp_time < t )	{
			set_time( tmp_tag, t );
			err_code = BUMPED_FWD_TAG;
		}
	}
	return( true );
	*/
#else
	if( valid_tag( tag ) )	{

		synch_time_arr[ tag ] = t;
		if( t < 0.0 )	{
			return( true );
		}
	// check and bump back order of preceding:
		for( int i = 0; i < tag; i++ )	{

			if( valid_time( i ) )	{
				if( get_time( i ) > t ) {
					set_time( i, t );		// should exploit recursion... NO LOOP!
					err_code = BUMPED_BACK_TAG;
				}
			}
		}
	// check and bump forward order of succeeding:
		for( int i = tag + 1; i < NUM_SYNCH_TAGS; i++ )	{

			if( valid_time( i ) )	{
				if( get_time( i ) < t ) {
					set_time( i, t );
					err_code = BUMPED_FWD_TAG;
				}
			}
		}
		return( true );
	}
	return( false );
#endif	
}

bool srSynchPoints::set_interval( int tag, double t )	{

#if 1
	int prev_tag = tag;
	if( get_prev( & prev_tag ) >= 0.0 )	{
		return( set_time( tag, synch_time_arr[ prev_tag ] + t ) );
	}
	err_code = UNDEFINED_INTERVAL;
	return( false );
#else
	if( valid_tag( tag ) )	{
		for( int i = tag-1; i>=0; i-- )	{

			if( valid_time( i ) )	{
				return( set_time( tag, synch_time_arr[ i ] + t ) ); // force bump
			}
		}
		err_code = UNDEFINED_INTERVAL;
		return( false );
	}
	return( false );
#endif
}

bool srSynchPoints::copy_points( srSynchPoints & source, double alt_offset, double alt_scale )	{

	init();
	int tag = -1;
	bool done = false;
	while( !done  )	{
		double t = source.get_next( & tag );
		if( tag < 0 )	{
			done = true;
		}
		else	{
			set_time( tag, alt_offset + alt_scale * t );
		}
	}
	double ramp = get_interval_from( RELAX );
	if( ramp >= 0.0 )	{
		implied_ramp_out = ramp;
	}
	// skipping global_offset and default_ramp_out.
	return( true );
}

//////////////////////////////////////////////////////////////////

void srSynchPoints::set_time( double start, double stop ) {

	init();
	set_time( START, start );
	set_time( STOP, stop );
}

void srSynchPoints::set_time( double start, double stop, double ramp ) {

	init();
	set_time( START, start );
	set_time( READY, start + ramp );
	if( stop >= 0.0 )	{
		set_time( RELAX, stop - ramp );
		set_time( STOP, stop );
	}
//	implied_ramp_out = ramp;
	ramp = get_interval_from( RELAX );
	if( ramp >= 0.0 )	{
		implied_ramp_out = ramp;
	}
}

void srSynchPoints::set_time( double start, double ready, double relax, double stop ) {

	init();
	set_time( START, start );
	set_time( READY, ready );
	set_time( RELAX, relax );
	set_time( STOP, stop );
//	if( ( relax >= 0.0 )&&( stop >= 0.0 ) ) {
//		implied_ramp_out = stop - relax;
//	}
	double ramp = get_interval_from( RELAX );
	if( ramp >= 0.0 )	{
		implied_ramp_out = ramp;
	}
}

void srSynchPoints::set_time( 
	double start, 
	double ready, 
	double stroke, 
	double relax, 
	double stop
) {

	init();
	set_time( START, start );
	set_time( READY, ready );
	set_time( STROKE, stroke );
	set_time( RELAX, relax );
	set_time( STOP, stop );
//	if( ( relax >= 0.0 )&&( stop >= 0.0 ) ) {
//		implied_ramp_out = stop - relax;
//	}
	double ramp = get_interval_from( RELAX );
	if( ramp >= 0.0 )	{
		implied_ramp_out = ramp;
	}
}

void srSynchPoints::set_time( 
	double start, double ready, 
	double st_start, double stroke, double st_stop, 
	double relax, double stop 
) {

	init();
	set_time( START, start );
	set_time( READY, ready );
	set_time( STROKE_START, st_start );
	set_time( STROKE, stroke );
	set_time( STROKE_STOP, st_stop );
	set_time( RELAX, relax );
	set_time( STOP, stop );
//	if( ( relax >= 0.0 )&&( stop >= 0.0 ) ) {
//		implied_ramp_out = stop - relax;
//	}
	double ramp = get_interval_from( RELAX );
	if( ramp >= 0.0 )	{
		implied_ramp_out = ramp;
	}
	
//	print();
}

//////////////////////////////////////////////////////////////////

void srSynchPoints::set_interval( double start_at, double start_to_stop )	{

	set_time( START, start_at );
	set_interval( STOP, start_to_stop );
}

void srSynchPoints::set_interval( double start_at, double start_to_stop, double ramp )	{
	if( ramp < 0.0 )	{
		ramp = 0.0;
		err_code = NEGATIVE_RAMP;
	}

	set_time( START, start_at );
	set_interval( READY, ramp );
	set_interval( RELAX, start_to_stop - 2 * ramp );
	set_interval( STOP, ramp );
	implied_ramp_out = ramp;
}

void srSynchPoints::set_interval( 
	double start_at, 
	double start_to_ready, 
	double ready_to_relax, 
	double relax_to_stop 
)	{

	set_time( START, start_at );
	set_interval( READY, start_to_ready );
	set_interval( RELAX, ready_to_relax );
	set_interval( STOP, relax_to_stop );
	if( relax_to_stop > 0.0 )	{
		implied_ramp_out = relax_to_stop;
	}
}

void srSynchPoints::set_interval( 
	double start_at, 
	double start_to_ready, 
	double ready_to_stroke, 
	double stroke_to_relax, 
	double relax_to_stop
)	{

	set_time( START, start_at );
	set_interval( READY, start_to_ready );
	set_interval( STROKE, ready_to_stroke );
	set_interval( RELAX, stroke_to_relax );
	set_interval( STOP, relax_to_stop );
	if( relax_to_stop > 0.0 )	{
		implied_ramp_out = relax_to_stop;
	}
}

void srSynchPoints::set_interval( 
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
)	{

	set_time( START, start_at );
	set_interval( READY, start_to_ready );
	set_interval( STROKE_START, ready_to_S_start );
	set_interval( STROKE, S_start_to_S );
#if 0
	if( repetitions > 1 ) set_interval( STROKE_STOP, S_stop_return_to_start ); // what a mess
	double rep_offset = repetitions * ( S_start_to_S + S_to_S_stop );
	double rep_offset = ( 0.0 ); // definition of rep?
	set_interval( RELAX, rep_offset + S_stop_to_relax ); // what if reps == 0?
#else
	set_interval( STROKE_STOP, S_to_S_stop );
	set_interval( RELAX, S_stop_to_relax );
#endif
	set_interval( STOP, relax_to_stop );
	if( relax_to_stop > 0.0 )	{
		implied_ramp_out = relax_to_stop;
	}
}

//////////////////////////////////////////////////////////////////

void srSynchPoints::set_stop( double at_time, double ramp_out ) {

	set_time( RELAX, at_time );
	set_time( STOP, at_time + ramp_out );
}

void srSynchPoints::set_stop( double at_time ) {

	double ramp = default_ramp_out;
	if( ramp < 0.0 )	{
		ramp = implied_ramp_out;
	}
	set_stop( at_time, ramp );
}

//////////////////////////////////////////////////////////////////

double srSynchPoints::get_time( int tag, bool global )	{

	if( valid_tag( tag ) )	{
		double t = synch_time_arr[ tag ];
		if( t < 0.0 )	{
			return( -1.0 );
		}
		if( global )	{
			return( global_offset + t );
		}
		return( t );
	}
	return( -1.0 );
}

double srSynchPoints::get_interval( int fr_tag, int to_tag )   {

	double f = get_time( fr_tag );
	if( f < 0.0 )	{
		return( -1.0 );
	}
	double t = get_time( to_tag );
	if( t < 0.0 )	{
		return( -1.0 );
	}
	return( fabs( t - f ) );
}

double srSynchPoints::get_interval_to( int to_tag )	{

	int fr_tag = to_tag;
	double t = get_prev( & fr_tag );
	return( get_interval( fr_tag, to_tag ) );
}

double srSynchPoints::get_interval_from( int fr_tag )	{

	int to_tag = fr_tag;
	double t = get_next( & fr_tag );
	return( get_interval( fr_tag, to_tag ) );
}

double srSynchPoints::get_prev( int *tag_p, bool global )	{

	if( tag_p == NULL )	{
		return( -1.0 );
	}
	int tag = *tag_p;
	if( tag < 0 )	{
		tag = STOP;
	}
	else	{
		tag--;
	}
	double t = -1.0;
	bool done = false;
	while( !done )	{
		if( tag < 0 )	{
			done = true;
			tag = -1;
		}
		else	{
			t = get_time( tag, global );
			if( t >= 0.0 )	{
				done = true;
			}
			else	{
				tag--;
			}
		}
	}
	*tag_p = tag;
	return( t );
}

double srSynchPoints::get_next( int *tag_p, bool global )	{

	if( tag_p == NULL )	{
		return( -1.0 );
	}
	int tag = *tag_p;
	if( tag < 0 )	{
		tag = START;
	}
	else	{
		tag++;
	}
	double t = -1.0;
	bool done = false;
	while( !done )	{
		if( tag >= NUM_SYNCH_TAGS )	{
			done = true;
			tag = -1;
		}
		else	{
			t = get_time( tag, global );
			if( t >= 0.0 )	{
				done = true;
			}
			else	{
				tag++;
			}
		}
	}
	*tag_p = tag;
	return( t );
}

//////////////////////////////////////////////////////////////////

srLinearCurve *srSynchPoints::get_trapezoid( srLinearCurve *curve_p, double dfl_dur, double dfl_in, double dfl_out ) {

	// extract ordered data, check and apply defaults:

	double t0 = get_time( START );
	if( t0 < 0.0 )	{
		t0 = 0.0;
		err_code = UNDEFINED_INTERVAL;
	}
	double t3 = get_time( STOP );
	bool duration_defined = true;
	if( t3 < 0.0 )	{
		if( dfl_dur < 0.0 ) {
			duration_defined = false;
		}
		else	{
			t3 = t0 + dfl_dur;
		}
	}

	double t1 = get_time( READY );
	if( t1 < 0.0 )	{
		if( dfl_in < 0.0 )	{
			dfl_in = 0.0;
		}
		t1 = t0 + dfl_in;
	}
	double t2 = get_time( RELAX );
	if( t2 < 0.0 )	{
		if( duration_defined )	{
			if( dfl_out < 0.0 )	{
				dfl_out = 0.0;
			}
			t2 = t3 - dfl_out;
		}
	}

	// adjust ramps as needed:

	if( duration_defined )	{
		double len = t3 - t0;
		double ramp_in = t1 - t0;
		double ramp_out = t3 - t2;
		if( ( ramp_in + ramp_out ) > len ) {
			double ratio = ramp_in / ( ramp_in + ramp_out );
			ramp_in = ratio * len;
			ramp_out = ( 1.0 - ratio ) * len;
			t1 = t0 + ramp_in;
			t2 = t3 - ramp_out;
			err_code = OVERLAPPED_RAMPS;
		}
	}

	// build curve:

	curve_p->clear();
	curve_p->insert( t0, 0.0 );
	curve_p->insert( t1, 1.0 );
	if( duration_defined )	{
		curve_p->insert( t2, 1.0 );
		curve_p->insert( t3, 0.0 );
	}
	return( curve_p );
}

//////////////////////////////////////////////////////////////////

void srSynchPoints::print( void )	{

	for( int i=0; i<NUM_SYNCH_TAGS; i++ )	{
		if( synch_time_arr[ i ] >= 0.0 )	{
			LOG( "srSynchPoints: %s: %f", tag_label( i ), synch_time_arr[ i ] );
		}
	}
}

void srSynchPoints::print_error( void )	{

	if( err_code )	{
		LOG( "srSynchPoints ERR: %s", error_label( err_code ) );
	}
	else	{
		LOG( "srSynchPoints NOTE: NO_ERROR_DETECTED" );
	}
}

int srSynchPoints::get_error( bool print_out )	{

	if( err_code > 0 )	{
		if( print_out )	{
			print_error();
		}
	}
	return( err_code );
}

//////////////////////////////////////////////////////////////////

#if 0
void test_synch_points( void )	{
	srSynchPoints sp;
	
	srLinearCurve *c_p = sp.new_trapezoid();
	c_p->print();

	c_p = sp.get_trapezoid( c_p, -1.0 );
	c_p->print();

	sp.set_time( 1.5, 2.5, 0.1 );
	c_p = sp.get_trapezoid( c_p );
	c_p->print();

	sp.set_interval( 1.5, 0.1, 1.0, 0.1 );
	c_p = sp.get_trapezoid( c_p );
	c_p->print();

	int tag = -1;
	do	{
		double t = sp.get_next( & tag );
		if( t >= 0.0 ) printf( "tag: %s: %f\n", sp.tag_label( tag ), t );
	} while( tag > -1 );
}
#endif
