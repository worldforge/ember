/*
 *  sr_linear_curve.cpp - part of SmartBody-lib
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

#include "sbm/lin_win.h"

#include <math.h>
//#include <sbm/sr_linear_curve.h>
#include "sr_linear_curve.h"
#include "sr_curve_builder.h"

#if ENABLE_OBJ_KEY_CT
int srLinearCurve::objective_key_count = 0;
int srLinearCurve::objective_key_count_max = 0;
#endif

const double srLinearCurve::MAX_SLOPE = 1000000.0;
const double srLinearCurve::MAX_VALUE = 1000000000.0;

//int G_update_count = 0;

//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////

const char *srLinearCurve::mode_label( const int index ) {

	switch( index )	{
		case CROP: 			return( "crop" );
		case CLAMP: 		return( "clamp" );
		case REPEAT:		return( "repeat" );
		case EXTRAPOLATE:	return( "extrap" );
	}
	return( "UNKNOWN" ); // default err
}

int srLinearCurve::mode_index( const char *label )	{

	if( LinWin_strcmp( label, "crop" ) == 0 ) return( CROP );
	if( LinWin_strcmp( label, "clamp" ) == 0 ) return( CLAMP );
	if( LinWin_strcmp( label, "repeat" ) == 0 ) return( REPEAT );
	if( LinWin_strncmp( label, "extrapolate", 6 ) == 0 ) return( EXTRAPOLATE );
	return( -1 );
}

//////////////////////////////////////////////////////////////////

srLinearCurve::Key::Key( double p, double v ) {

	param = p;
	value = v;
	dp = 0.0; inv_dp = 0.0;
	dv = 0.0;
	next_p = NULL;
#if ENABLE_OBJ_KEY_CT
	objective_key_count++;
	if( objective_key_count > objective_key_count_max ) objective_key_count_max = objective_key_count;
#endif
}

void srLinearCurve::Key::print( int i )	{

#if 1
	printf( " key[ %d ]: ( %f, %f )\n", 
		i, param, value
	);
#elif 0
	printf( " key[ %d ]: ( %f, %f ):{ %f, %f, %f }:[ 0x%x ]\n", 
		i, param, value,
		dp, inv_dp, dv,
		next_p
	);
#else
	printf( " key[ %d ]: ( %f, %f ):{ %f }\n", 
		i, param, value,
		slope()
	);
#endif
}

//////////////////////////////////////////////////////////////////

void srLinearCurve::Key::update( void ) {

	if( next_p )	{
		dp = next_p->param - param; 
		dv = next_p->value - value;
		if( dp > 0.0 )	{
			inv_dp = 1.0 / dp;
		}
		else	{ 
		// ( dp == 0.0 )
			if( dv < 0.0 )	{
				inv_dp = -MAX_SLOPE;
			}
			else	{
				inv_dp = MAX_SLOPE;
			}
		}
	}
}

void srLinearCurve::Key::copy_delta( Key *key_p ) {

	if( key_p )	{
		dp = key_p->dp; inv_dp = key_p->inv_dp;
		dv = key_p->dv;
	}
}

double srLinearCurve::Key::slope( void )	{

	if( dp > 0.0 )	{ // parameter change
		return( dv * inv_dp );
	}
	if( dv > 0.0 )	{ // value change
		return( MAX_SLOPE );
	}
	if( dv < 0.0 )	{
		return( -MAX_SLOPE );
	}
	return( 0.0 ); // uninitialized
}

double srLinearCurve::Key::lerp( double t )	{

//	return( value + ( t - param ) * slope() ); // for illustration
	if( dp > 0.0 )	{
		return( value + dv * ( t - param ) * inv_dp );
	}
	return( value );
}

//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////

void srLinearCurve::print( void )	{

	if( dirty ) update();

#if ENABLE_OBJ_KEY_CT
	printf( "srLinearCurve: KEYS: %d of %d (%d)\n", key_count, objective_key_count, objective_key_count_max );
#else
	printf( "srLinearCurve: KEYS: %d\n", key_count );
#endif

	int c = 0;
	Key *key_p = head_p;
	while( key_p ) {
		key_p->print( c++ );
		key_p = key_p->next();
	}
}

//////////////////////////////////////////////////////////////////

void srLinearCurve::clear( void )	{

	Key *key_p = head_p;
	while( key_p ) {
		Key *tmp_p = key_p;
		key_p = key_p->next();
		delete tmp_p;
		decrement();
	}
	init();
}

void srLinearCurve::clear_after( double t )	{

	if( head_p == NULL )	{
		return;
	}
	Key *floor_p = find_floor_key( t );
	if( floor_p == NULL )	{
		clear();
		return;
	}

	Key *del_p = floor_p->next();
	floor_p->next( NULL );

	while( del_p ) {
		Key *tmp_p = del_p;
		del_p = del_p->next();
		delete tmp_p;
		decrement();
	}
}

double srLinearCurve::get_head_param( void )	{
	if( head_p )	{
		return( head_p->param );
	}
	return( 0.0 );
}

double srLinearCurve::get_head_value( void )	{
	if( head_p )	{
		return( head_p->value );
	}
	return( 0.0 );
}

double srLinearCurve::get_head_slope( void )	{
	if( dirty ) update();
	if( head_p )	{
		return( head_p->slope() );
	}
	return( 0.0 );
}

double srLinearCurve::get_tail_param( void )	{
	if( dirty ) update();
	if( tail_p )	{
		return( tail_p->param );
	}
	return( 0.0 );
}

double srLinearCurve::get_tail_value( void )	{
	if( dirty ) update();
	if( tail_p )	{
		return( tail_p->value );
	}
	return( 0.0 );
}

double srLinearCurve::get_tail_slope( void )	{
	if( dirty ) update();
	if( tail_p )	{
		return( tail_p->slope() );
	}
	return( 0.0 );
}

double srLinearCurve::get_next_nonzero_value( double after )	{

	if( head_p == NULL )	{
		return( -1.0 );
	}
	Key *key_p = NULL;
	Key *floor_p = find_floor_key( after );
	if( floor_p )	{
		key_p = floor_p->next();
	}
	else	{
		key_p = head_p;
	}
	while( key_p )	{
		if( key_p->value != 0.0 )	{
			return( key_p->param );
		}
		key_p = key_p->next();
	}
	return( -1.0 );
}

double srLinearCurve::get_next_nonzero_slope( double after )	{

	if( head_p == NULL )	{
		return( -1.0 );
	}
	Key *key_p = NULL;
	Key *floor_p = find_floor_key( after );
	if( floor_p )	{
		key_p = floor_p->next();
	}
	else	{
		key_p = head_p;
	}
	while( key_p )	{
		if( key_p->slope() != 0.0 )	{
			return( key_p->param );
		}
		key_p = key_p->next();
	}
	return( -1.0 );
}

#if 0
double srLinearCurve::get_last_nonzero_param( double after )	{

	if( head_p == NULL )	{
		return( -1.0 );
	}
	Key *key_p = NULL;
	Key *floor_p = find_floor_key( after );
	if( floor_p )	{
		key_p = floor_p->next();
	}
	else	{
		key_p = head_p;
	}
	double t = -1.0;
	while( key_p )	{
		if( key_p->value != 0.0 )	{
			t = key_p->param;
		}
		key_p = key_p->next();
	}
	return( t );
}
#endif

//////////////////////////////////////////////////////////////////

srLinearCurve::Key* srLinearCurve::find_floor_key( double t )	{

	if( dirty ) update();

	Key *key_p = curr_p;
	if( key_p )	{
		if( t < key_p->param ) {
			key_p = head_p;
		}
	}
	else	{
		key_p = head_p;
	}
	if( key_p )	{
		if( t < key_p->param )	{
			return( NULL );
		}
	}
	while( key_p )	{

		Key *next_p = key_p->next();
		if( next_p ) {

			if( t < next_p->param )  {
				return( key_p );
			}
			else	{
				key_p = next_p;
			}
		}
		else	{
			return( key_p );
		}
	}
	return( NULL );
}

double srLinearCurve::head_boundary( double t, bool *cropped_p ) {

	switch( head_bound_mode )	{
	
		case CROP:
			{
				if( cropped_p ) {
					*cropped_p = true;
				}
				return( 0.0 );
			}
		case CLAMP:
			{
				return( head_p->value );
			}
		case REPEAT:
			{
				double dur = tail_p->param - head_p->param;
				if( dur > 0.0 )	{
					double modulo = fmod( ( head_p->param - t ), dur );
					double rep_time;
					if( modulo > 0.0 )	{
						rep_time = tail_p->param - modulo;
					}
					else	{
						rep_time = head_p->param;
					}
					return( evaluate( rep_time ) );
				}
				return( head_p->value );
			}
		case EXTRAPOLATE:
			{
				if( head_p == tail_p )	{
					return( head_p->value );
				}
				double slope = head_p->slope(); // uninitialized slope returns 0.0 guaranteed
				if( slope < MAX_SLOPE )	{
					if( slope > -MAX_SLOPE )	{

						double extrap_val = head_p->value - slope * ( head_p->param - t );
						return( extrap_val );
					}
					if( cropped_p ) *cropped_p = true;
					return( min_value );
				}
				if( cropped_p ) *cropped_p = true;
				return( max_value );
			}
		default:
//			LOG( "srLinearCurve::head_boundary ERR: bound-mode %d not recognized", head_bound_mode );
			printf( "srLinearCurve::head_boundary ERR: bound-mode %d not recognized\n", head_bound_mode );
	}
	return( 0.0 );
}

double srLinearCurve::tail_boundary( double t, bool *cropped_p ) {

	switch( tail_bound_mode )	{
	
		case CROP:
			{
				if( cropped_p ) {
					*cropped_p = true;
				}
				return( 0.0 );
			}
		case CLAMP:
			{
				return( tail_p->value );
			}
		case REPEAT:
			{
				double dur = tail_p->param - head_p->param;
				if( dur > 0.0 )	{
					double rep_time = head_p->param + fmod( t - tail_p->param, dur );
					return( evaluate( rep_time ) );
				}
				return( head_p->value );
			}
		case EXTRAPOLATE:
			{
				if( head_p == tail_p )	{
					return( head_p->value );
				}
				double slope = tail_p->slope(); // uninitialized slope returns 0.0 guaranteed
				if( slope < MAX_SLOPE )	{
					if( slope > -MAX_SLOPE )	{
						
						double extrap_val = tail_p->value + slope * ( t - tail_p->param );
						return( extrap_val );
					}
					if( cropped_p ) *cropped_p = true;
					return( min_value );
				}
				if( cropped_p ) *cropped_p = true;
				return( max_value );
			}
		default:
//			LOG( "srLinearCurve::tail_boundary ERR: bound-mode %d not recognized", tail_bound_mode );
			printf( "srLinearCurve::tail_boundary ERR: bound-mode %d not recognized\n", tail_bound_mode );
	}
	return( 0.0 );
}

double srLinearCurve::evaluate( double t, bool *cropped_p )	{

	if( cropped_p ) {
		*cropped_p = false;
	}
	// find_floor_key checks dirty bit update...
	Key *floor_p = find_floor_key( t ); // if defined, ( floor-param <= t )
	if( floor_p )	{

		curr_p = floor_p; // evaluate() establishes curr = floor
		double curr_param = curr_p->param;

		if( t > curr_param )	{
			if( curr_p->next() ) {
				return( curr_p->lerp( t ) );
			}
		}
		else
		if( t == curr_param )	{
			if( tail_bound_mode == REPEAT )	{
				
				if( curr_p->next() ) {
					return( curr_p->lerp( t ) );
				}
			}
			else	{
				return( curr_p->value );
			}
		}

		// after tail, or repeat-tail_bound = head condition:
		return( tail_boundary( t, cropped_p ) );
	}
	if( head_p )	{
		// before head:
		return( head_boundary( t, cropped_p ) );
	}
	curr_p = NULL;
	if( cropped_p ) {
		*cropped_p = true;
	}
	return( 0.0 );
}

//////////////////////////////////////////////////////////////////

int srLinearCurve::insert_key( Key *key_p ) {

	if( key_p )	{

		Key *floor_p = find_floor_key( key_p->param );
		if( floor_p )	{

			insert_after( floor_p, key_p );
			curr_p = floor_p;
			return( false );
		}
		insert_head( key_p );
		curr_p = key_p;
		return( false );
	}
	curr_p = NULL;
	return( true );
}

void srLinearCurve::insert_head( Key *key_p )	{

	key_p->next( head_p );
	head_p = key_p;
	increment();
}

void srLinearCurve::insert_after( Key *prev_p, Key *key_p )	{

	Key *next_p = prev_p->next();
	prev_p->next( key_p );
	key_p->next( next_p );
	increment();
}

void srLinearCurve::decrement( void )	{

	key_count--;
	dirty = true;
}

void srLinearCurve::increment( void )	{

	key_count++;
	dirty = true;
}

void srLinearCurve::update( void )	{

//G_update_count++;

	reset();
	curr_p = NULL;
	tail_p = NULL;
	int c = 0;
	Key *prev_p = NULL;
	Key *key_p = head_p;
	while( key_p ) {

		Key *tmp_p = prev_p;
		prev_p = key_p;
		key_p = key_p->next();
		
		if( key_p ) {
			prev_p->update();
		}
		else
		if( tmp_p ) {
			prev_p->copy_delta( tmp_p );
			tail_p = prev_p;
		}
		else	{
			tail_p = head_p;
		}
		c++;
	}

	dirty = false;
	if( c != key_count )	{
//		LOG( "srLinearCurve::update ERR: corruption: counted %d of %d keys", c, key_count );
		printf( "srLinearCurve::update ERR: corruption: counted %d of %d keys\n", c, key_count );
	}
}

//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////

#if 1

void test_linear_curve( void )	{
	srLinearCurve curve;
	
	srCurveBuilder builder;
	srLinearCurve *curve_p = builder.get_std_hump_curve( &curve, 10 );
	if( curve_p )	{
		curve_p->print();
	}

	curve_p = builder.get_std_bell_curve( &curve, 10 );
	if( curve_p )	{
		curve_p->print();
	}

#if 0
	srSplineCurve spline;
	spline.insert( 0.0, 2.0 );
	spline.insert( 1.0, 1.0 );
	spline.insert( 3.0, 3.5 );
	spline.insert( 5.0, 3.0 );
	spline.insert( 6.0, 2.0 );
	
	builder.set_input_range( 0.0, 6.0 );
	curve_p = builder.get_spline_curve( &curve, spline, 10 );
	if( curve_p )	{
		curve_p->print();
	}
#endif
}
#endif

#if 0

void test_eval_linear_curve( srLinearCurve *curve_p ) {

	for( int i=0; i<=20; i++ )	{
		double t = (double)i * 0.1 - 0.5;
		bool crop = false;
		double v = curve_p->evaluate( t, &crop );
		printf( "%f: %f (%d)\n", t, v, crop );
	}
}

void test_run_linear_curve_gamut( srLinearCurve *curve_p ) {

	curve_p->clear();
	curve_p->print();

	test_eval_linear_curve( curve_p );
	
	curve_p->insert( 0.5, 1.0 );
	curve_p->print();

	test_eval_linear_curve( curve_p );

	curve_p->insert( 1.0, 2.0 );
//	curve_p->insert( 0.5, 0.5 ); 
//	curve_p->insert( 0.5, 1.5 );
//	curve_p->insert( 1.0, 1.0 );
//	curve_p->insert( 1.0, 3.0 );
	curve_p->print();

	test_eval_linear_curve( curve_p );
}

void test_linear_curve( void )	{
	srLinearCurve curve;

#if 1
	printf( "CROP:\n" );
	curve.set_boundary_mode( srLinearCurve::CROP, srLinearCurve::CROP );
	test_run_linear_curve_gamut( & curve );

	printf( "CLAMP:\n" );
	curve.set_boundary_mode( srLinearCurve::CLAMP, srLinearCurve::CLAMP );
	test_run_linear_curve_gamut( & curve );

	printf( "REPEAT:\n" );
	curve.set_boundary_mode( srLinearCurve::REPEAT, srLinearCurve::REPEAT );
	test_run_linear_curve_gamut( & curve );
	
	printf( "EXTRAPOLATE:\n" );
	curve.set_boundary_mode( srLinearCurve::EXTRAPOLATE, srLinearCurve::EXTRAPOLATE );
	test_run_linear_curve_gamut( & curve );
	
#else

	double t;
	double v;
	bool crop;

	curve.set_boundary_mode( srLinearCurve::CROP, srLinearCurve::CROP );
//	curve.set_boundary_mode( srLinearCurve::CLAMP, srLinearCurve::CLAMP );
//	curve.set_boundary_mode( srLinearCurve::REPEAT, srLinearCurve::REPEAT );
//	curve.set_boundary_mode( srLinearCurve::EXTRAPOLATE, srLinearCurve::EXTRAPOLATE );

	curve.insert( 0.5, 1.0 );
	curve.insert( 1.0, 2.0 );
	t = 1.0;
	v = curve.evaluate( t, &crop );
	printf( "%f: %f (%d)\n", t, v, crop );

	t = 1.1;
	v = curve.evaluate( t, &crop );
	printf( "%f: %f (%d)\n", t, v, crop );
#endif
}

#endif

//////////////////////////////////////////////////////////////////

#if 0

void test_linear_curve( void )	{

	srLinearCurve C;
	bool cropped;
	int reps = 1000000;
	
	for( int i=0; i<reps; i++ )	{
		int r = rand() % 32;
		switch( r )	{
			case 0:
				{
					C.insert( (float)rand() / (float)RAND_MAX, (float)rand() / (float)RAND_MAX );
					C.insert( (float)rand() / (float)RAND_MAX, (float)rand() / (float)RAND_MAX );
					C.insert( (float)rand() / (float)RAND_MAX, (float)rand() / (float)RAND_MAX );
					C.insert( (float)rand() / (float)RAND_MAX, (float)rand() / (float)RAND_MAX );
					C.insert( (float)rand() / (float)RAND_MAX, (float)rand() / (float)RAND_MAX );
				}
				break;
			case 1:
				C.clear();
//				C.clear_after( (float)rand() / (float)RAND_MAX );
				break;
			case 2:
				C.clear_after( (float)rand() / (float)RAND_MAX );
				break;
			case 3:
				C.get_next_nonzero_slope( (float)rand() / (float)RAND_MAX );
				break;
			case 4:
				C.get_next_nonzero_value( (float)rand() / (float)RAND_MAX );
				break;
			case 5:
				C.get_head_param();
				break;
			case 6:
				C.get_head_value();
				break;
			case 7:
				C.get_head_param();
				break;
			case 8:
				C.get_tail_param();
				break;
			case 9:
				C.get_tail_value();
				break;
			case 10:
				C.get_tail_slope();
				break;
			case 12:
				C.set_boundary_mode( rand() % srLinearCurve::NUM_BOUNDARY_MODES, rand() % srLinearCurve::NUM_BOUNDARY_MODES );
				break;
			default:
				C.evaluate( (float)rand() / (float)RAND_MAX, & cropped );
				break;
		}
	}
	C.print();
	printf( "updates: %d\n", G_update_count );
}
#endif

//////////////////////////////////////////////////////////////////

#if 0
void test_linear_curve( void )	{
	srLinearCurve curve;
	int i;
#if 1
	curve.insert( 2.0, 10.0 ); curve.print();
	curve.insert( 3.0, 20.0 ); curve.print();
	curve.insert( 1.0, 0.9 ); curve.print();
	curve.insert( 0.1, 0.5 ); curve.print();
	curve.insert( 2.5, 11.0 ); curve.print();
#endif
#if 0
	for( i=0; i<50; i++ )	{
		curve.insert( (double)( rand()%100 ), (double)i );
	}
#endif
	curve.print();
#if 0
	for( i=0; i<100; i++ )	{
		double s = (double)i * 0.1;
		double v = curve.evaluate( s );
		printf( "%f: %f\n", s, v );
	}
#endif
}
#endif
