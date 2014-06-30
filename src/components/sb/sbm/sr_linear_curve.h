/*
 *  sr_linear_curve.h - part of SmartBody-lib
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

#ifndef SR_LINEAR_CURVE_H
#define SR_LINEAR_CURVE_H

#include <stdio.h>
//#include <vhcl_log.h>
#include "sbm/gwiz_math.h"
#include "sr_spline_curve.h"

#define ENABLE_OBJ_KEY_CT	0

//////////////////////////////////////////////////////////////////

class srLinearCurve	{

	public:
		enum boundary_mode_enum_set	{
			CROP,			// do not write
			CLAMP, 			// write boundary value
			REPEAT, 		// loop curve, skip tail
			EXTRAPOLATE,	// extrapolate boundary slope, min/max_value if undefined
			NUM_BOUNDARY_MODES
		};

		static const char *mode_label( const int index );
		static int mode_index( const char *label );

		static const double MAX_SLOPE;
		static const double MAX_VALUE;

	private:

#if ENABLE_OBJ_KEY_CT
		static int objective_key_count;
		static int objective_key_count_max;
#endif

		int 	head_bound_mode;
		int 	tail_bound_mode;
		
		double	min_value;
		double	max_value;

		int 	key_count;
		bool	dirty;

		class Key	{

			public:

				Key( double p, double v );
				~Key(void) {
#if ENABLE_OBJ_KEY_CT
					objective_key_count--;
#endif
				}

				void print( int i );

				void next( Key *set_p ) { next_p = set_p; }
				Key *next( void ) { return( next_p ); }

				void update( void );
				void copy_delta( Key *key_p );

				double slope( void );
				double lerp( double t );

				double	param; // ANIMATION: time
				double	value; // template<> ?

			private:

				double	dp, inv_dp;
				double	dv;

				Key	*next_p;
		};

		Key		*head_p;
		Key		*curr_p;
		Key 	*curr_query_p;
		Key		*tail_p;

		void null( void )	{
			head_bound_mode = 0;
			tail_bound_mode = 0;
			min_value = -MAX_VALUE;
			max_value = MAX_VALUE;
			init();
		}
		void init( void )	{
			key_count = 0;
			dirty = false;
			head_p = NULL;
			tail_p = NULL;
			reset();
		}
		void reset( void )	{
			curr_p = NULL;
			curr_query_p = NULL;
		}
		
	public:

		srLinearCurve( int bound_mode = CLAMP )	{
			null();
			set_boundary_mode( bound_mode, bound_mode );
		}
		srLinearCurve( int head_mode, int tail_mode )	{
			null();
			set_boundary_mode( head_mode, tail_mode );
		}
		~srLinearCurve( void )	{
			clear();
		}

		void print( void );

		void set_boundary_mode( int head_mode, int tail_mode )	{
			head_bound_mode = head_mode;
			tail_bound_mode = tail_mode;
		}
		void set_output_clamp( double min, double max )	{
			min_value = min;
			max_value = max;
		}

		int get_num_keys( void ) { return( key_count ); }
		
		int insert( double p, double v )	{ /* sort by key.time, add after same time */
			return( insert_key( new Key( p, v ) ) );
		}
		
		void clear( void );
		void clear_after( double t );

	// Preferably these should be subsumed by 
	//	boundary conditions, crop { PRE, POST }, 
	//	and simple param-range queries.

		double get_head_param( void );
		double get_head_value( void );
		double get_head_slope( void );
		
		double get_tail_param( void );
		double get_tail_value( void );
		double get_tail_slope( void );

		double get_next_nonzero_value( double after );
		double get_next_nonzero_slope( double after );
//		double get_last_nonzero_param( double after );

		double evaluate( double t, bool *cropped_p = NULL );

	protected:

		int insert_key( Key *key_p );
		void insert_head( Key *key_p );
		void insert_after( Key *prev_p, Key *key_p );
		void decrement( void );
		void increment( void );
		
		void update( void );
		
		Key* find_floor_key( double t );
		double head_boundary( double t, bool *cropped_p );
		double tail_boundary( double t, bool *cropped_p );

	public:

		void query_reset( void )	{ 
			if( dirty ) update();
			curr_query_p = head_p; 
		}
		bool query_next( 
			double *t_p, double *v_p, 
			bool increment 
			)	{

			if( curr_query_p )	{
//				if( t_p ) { *t_p = curr_query_p->p(); } // from gwiz::ctrl_key
//				if( v_p ) { *v_p = curr_query_p->v(); }
				if( t_p ) { *t_p = curr_query_p->param; }
				if( v_p ) { *v_p = curr_query_p->value; }
				if( increment ) {
					curr_query_p = curr_query_p->next();
				}
				return( true );
			}
			return( false );
		}
};

//////////////////////////////////////////////////////////////////
#endif

#if 1
void test_linear_curve( void );
#endif
