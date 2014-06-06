/*
 *  sr_curve_builder.h - part of SmartBody-lib
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

#ifndef SR_CURVE_BUILDER_H
#define SR_CURVE_BUILDER_H


#include <stdio.h>
//#include <vhcl_log.h>

#include "sr_linear_curve.h"
#include "sr_spline_curve.h"

//////////////////////////////////////////////////////////////////

class srCurveBuilder	{

	private:
		int num_keys;
		double in_range_fr, in_range_to;
		double out_range_fr, out_range_to;
		
	public:

		void set_resolution( int num_segs ) {
			num_keys = num_segs + 1;
		}
		void set_input_range( double fr, double to )	{
			in_range_fr = fr;
			in_range_to = to;
		}
		void set_output_range( double fr, double to )	{
			out_range_fr = fr;
			out_range_to = to;
		}

		srLinearCurve *get_std_hump_curve( srLinearCurve *curve_p, int num_segs )	{
		
			set_resolution( num_segs );
			set_input_range( 0.0, 1.0 );
			set_output_range( -1.0, 1.0 );
			return( get_sin_curve( curve_p, 0.0, M_PI ) );
		}
		srLinearCurve *get_std_bell_curve( srLinearCurve *curve_p, int num_segs )	{
		
			set_resolution( num_segs );
			set_input_range( 0.0, 1.0 );
			set_output_range( 0.0, 1.0 );
			return( get_cos_curve( curve_p, -M_PI, M_PI ) );
		}

		srLinearCurve *get_sin_curve( srLinearCurve *curve_p, double alpha, double beta )	{
			if( curve_p )	{
				curve_p->clear();

				double in_span = in_range_to - in_range_fr;
				double out_span = out_range_to - out_range_fr;
				double trig_span = beta - alpha;
				double denom = 1.0 / (float)( num_keys - 1 );

				for( int i = 0; i<num_keys; i++ )	{

					double tn = (float)i * denom;
					double t = in_range_fr + tn * in_span;
					double th = alpha + tn * trig_span;
//					double s = sin( RAD( th ) );
					double s = sin( th );
					double sn = ( s + 1.0 ) * 0.5;
					double v = out_range_fr + sn * out_span;
					curve_p->insert( t, v );
/*
					double tn = (float)i * denom;
					curve_p->insert( 
						in_range_fr + tn * in_span,
						out_range_fr + ( sin( RAD( alpha + tn * trig_span ) ) + 1.0 ) * 0.5 * out_span
					);
*/
				}
			}
			return( curve_p );
		}
		srLinearCurve *get_cos_curve( srLinearCurve *curve_p, double alpha, double beta )	{
			
			if( curve_p )	{
				curve_p->clear();

				double in_span = in_range_to - in_range_fr;
				double out_span = out_range_to - out_range_fr;
				double trig_span = beta - alpha;
				double denom = 1.0 / (float)( num_keys - 1 );

				for( int i = 0; i<num_keys; i++ )	{

					double tn = (float)i * denom;
					double t = in_range_fr + tn * in_span;
					double th = alpha + tn * trig_span;
//					double s = cos( RAD( th ) );
					double s = cos( th );
					double sn = ( s + 1.0 ) * 0.5;
					double v = out_range_fr + sn * out_span;
					curve_p->insert( t, v );
/*
					double tn = (float)i * denom;
					curve_p->insert( 
						in_range_fr + tn * in_span,
						out_range_fr + ( cos( RAD( alpha + tn * trig_span ) ) + 1.0 ) * 0.5 * out_span
					);
*/
				}
			}
			return( curve_p );
		}

		srLinearCurve *new_std_hump_curve( int num_segs )	{
			return( get_std_hump_curve( new srLinearCurve, num_segs ) );
		}
		srLinearCurve *new_std_bell_curve( int num_segs )	{
			return( get_std_bell_curve( new srLinearCurve, num_segs ) );
		}
		srLinearCurve *new_sin_curve( double alpha, double beta )	{
			return( get_sin_curve( new srLinearCurve, alpha, beta ) );
		}
		srLinearCurve *new_cos_curve( double alpha, double beta )	{
			return( get_cos_curve( new srLinearCurve, alpha, beta ) );
		}

		srLinearCurve *get_resample_curve( 
			srLinearCurve *curve_p, 
			srLinearCurve& source, 
			int num_segs,
			bool auto_span = true
			)	{
			
			set_resolution( num_segs );
			if( curve_p )	{

				if( auto_span ) {
					in_range_fr = source.get_head_param();
					in_range_to = source.get_tail_param();
				}
				curve_p->clear();
				if( num_keys > 0 )	{
				
					double in_span = in_range_to - in_range_fr;
					if( in_span > 0.0 ) {
					
						double dt = in_span / (double)num_segs;
						double t = in_range_fr;
						for( int i = 0; i< num_keys; i++ ) {
						
							double v = source.evaluate( t, NULL );

							// check against out_range_fr, out_range_to:

							curve_p->insert( t, v );
							t += dt;
						}
					}
				}
			}
			return( curve_p );
		}
		
		srLinearCurve *get_spline_curve( 
			srLinearCurve *curve_p, 
			srSplineCurve& spline, 
			int num_segs,
			bool auto_span = true
			)	{
			
			set_resolution( num_segs );
			if( curve_p )	{

				if( auto_span ) {
					spline.query_span( &in_range_fr, &in_range_to );
				}
				curve_p->clear();
				if( num_keys > 0 )	{
				
					double in_span = in_range_to - in_range_fr;
					if( in_span > 0.0 ) {
					
						double dt = in_span / (double)num_segs;
						double t = in_range_fr;
						
						for( int i = 0; i< num_keys; i++ ) {
							
							double v = spline.evaluate( t, NULL );

							// check against out_range_fr, out_range_to:
							// no constructor! never set...

							curve_p->insert( t, v );
							t += dt;
						}
					}
				}
			}
			return( curve_p );
		}
		srLinearCurve *new_spline_curve( srSplineCurve& spline, int num_segs, bool auto_span = true )	{
			return( get_spline_curve( new srLinearCurve, spline, num_segs, auto_span ) );
		}
};

//////////////////////////////////////////////////////////////////
#endif

