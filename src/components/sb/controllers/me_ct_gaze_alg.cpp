/*
 *  me_ct_gaze_alg.cpp - part of SmartBody-lib
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

#include "controllers/me_ct_gaze_alg.h"
#include <vhcl_log.h>

using namespace gwiz;

///////////////////////////////////////////////////////////////////////////

vector_t find_nearest_point_on_line(
	vector_t P,  // point near line
	vector_t L1, // point 1 on line
	vector_t L2  // point 2 on line
)	{
	
	vector_t N = ( L2 - L1 ).normal();
	
	// tangent plane equation:
	gwiz::float_t d = -( N.dot( P ) );
	
	// intersect line and plane
	gwiz::float_t A = N.dot( L1 ) + d;
	gwiz::float_t B = -( N.sqlen() );
	gwiz::float_t u = A / B;
	
	return( L1 + N * u );
}

//#define FORWARD_RAY_EPSILON ( 0.00001 )
#define FORWARD_RAY_EPSILON ( 0.01 )

quat_t rotation_ray_to_target_orient(
	quat_t Q,     // target orientation
	vector_t Fd,  // forward ray direction
	vector_t Fr   // null reference ray direction
)	{
	
	if( Fd.length() < FORWARD_RAY_EPSILON ) {
		LOG( "rotation_ray_to_target_orient ERR: forward direction has no length\n" );
		return( quat_t() );
	}
	
	vector_t Td = Q * Fr; // target direction vector
	
	vector_t axis = Fd.cross( Td );
	gwiz::float_t angle = DEG( acos( Fd.normal().dot( Td.normal() ) ) );
	return( quat_t( angle, axis ) );
}

//////////////////////////////////////////////////////////////////////////

quat_t rotation_ray_to_target_point(
	vector_t X,   			// target
	vector_t R,   			// center of rotation
	vector_t Fo,  			// forward ray origin
	vector_t Fd,  			// forward ray direction
	gwiz::float_t buffer_ratio, // proportion of buffer zone for pathological case
	int heading_only
)	{
	
#if 0
if( G_debug )	{
LOG( "-- --\n" );

X.print();
R.print();
Fo.print();
Fd.print();

LOG( "-- --\n" );
}
#endif

	if( Fd.length() < FORWARD_RAY_EPSILON ) {
		if( heading_only == false ) {
			LOG( "rotation_ray_to_target_point ERR: forward direction has no length\n" );
		}
		return( quat_t() );
	}
	
	vector_t RX = X - R; // vector from rotation to target
	gwiz::float_t d = RX.length(); // distance from rotation to target

	vector_t RF = Fo - R; // vector from rotation to forward origin
	gwiz::float_t f = RF.length(); // distance from rotation to forward origin

	// pathological case:
	// if target is inside of forward origin radius
	if( d < f )	{
//	if( !heading_only || ( d < f ) )	{

		if( heading_only )	{
			// give up:
			return( quat_t() );
		}
		
		// treat preferred 2D heading case:
#if 0
// pathological flat case:
		vector_t X2 = vector_t( X.x(), 0.0, X.z() );
		vector_t R2 = vector_t( R.x(), 0.0, R.z() );
		vector_t Fo2 = vector_t( Fo.x(), 0.0, Fo.z() );
#else
// patch:
		vector_t X2 = vector_t( X.x(), 0.0, X.z() );
		vector_t R2 = vector_t( R.x(), 0.0, R.z() );
		vector_t Fo2 = vector_t( Fo.x(), 1.0, Fo.z() );
#endif
		vector_t Fd2 = vector_t( Fd.x(), 0.0, Fd.z() );

		return( rotation_ray_to_target_point( X2, R2, Fo2, Fd2, buffer_ratio, true ) );
//		return( quat_t() );
	}

// Merge this buffer section with above:

	// blend pathological case if near forward origin radius:
	quat_t Qbuffer; // solution for pathological case
	gwiz::float_t buffer_lerp = 0.0; // amount of Qbuffer to apply
	gwiz::float_t buffer_len = f * buffer_ratio; // length of buffer zone
	
#if 1
	// if target is near forward origin radius: 
	if( d < ( f + buffer_len ) )	{
		buffer_lerp = 1.0 - ( d - f ) / buffer_len;

		if( !heading_only )	{

			// treat preferred 2D heading case:
			vector_t X2 = vector_t( X.x(), 0.0, X.z() );
			vector_t R2 = vector_t( R.x(), 0.0, R.z() );
//			vector_t Fo2 = vector_t( Fo.x(), 0.0, Fo.z() );
			vector_t Fo2 = vector_t( Fo.x(), 1.0, Fo.z() );
			vector_t Fd2 = vector_t( Fd.x(), 0.0, Fd.z() );

			Qbuffer = rotation_ray_to_target_point( X2, R2, Fo2, Fd2, buffer_ratio, true );
		}
	}
#endif

	// T: forward ray tangent to rotation sphere
	vector_t T = find_nearest_point_on_line( R, Fo, Fo + Fd );
	
	vector_t RT = T - R; // vector from rotation to forward tangent
	gwiz::float_t r = RT.length(); // radius of sphere

	// if radius is tiny
	if( r < FORWARD_RAY_EPSILON )	{

		// if target is close
		if( d < FORWARD_RAY_EPSILON )	{
			return( quat_t() );
		}

		vector_t axis = Fd.cross( RX );
		gwiz::float_t angle = deg( safe_acos( Fd.normal().dot( RX.normal() ) ) );
		return( quat_t( angle, axis ) );
	}

	// rho: angle from RX to X-tangent-plane
	gwiz::float_t rho = deg( safe_acos( r / d ) );

	// gamma: angle from RX to RT
	gwiz::float_t gamma = deg( safe_acos( RX.normal().dot( RT.normal() ) ) );

	// Aaxis: axis perpendicular to RX and RT
	vector_t Aaxis = RT.cross( RX );

	if( gamma > 180.0 )	{
//LOG( "flip gamma\n" );
		gamma = 360.0 - gamma;
		Aaxis = -Aaxis; // axis of shortest rotation
	}

	// alpha: rotation about Aaxis to move T to X-tangent-plane
	
// patches for flat case (from above pathology)
#if 0
	gwiz::float_t alpha;
	if( ( heading_only )&&( Aaxis.y() > 0.0 ) )	{
		alpha = gamma + rho;
	}
	else	{
		alpha = gamma - rho;
	}
#elif 0
	gwiz::float_t kappa = deg( safe_acos( Fd.normal().dot( RX.normal() ) ) );
	gwiz::float_t alpha;
	if( heading_only && ( kappa > 90.0 ) )	{
		alpha = gamma + rho;
	}
	else	{
		alpha = gamma - rho;
	}
#else
	gwiz::float_t alpha = gamma - rho;
#endif

	quat_t Qalpha( alpha, Aaxis ); // to apply alpha

	vector_t newT = R + Qalpha * RT; // rotated tangent vector
	vector_t newF = Qalpha * Fd; // rotated forward vector

	// beta: rotation to align new forward with new tangent

	gwiz::float_t beta = deg( safe_acos( newF.normal().dot( ( X - newT ).normal() ) ) );
	if( beta > 0.0 )	{

		// Baxis: axis to which beta is implicitly applied
		vector_t Baxis = newF.cross( X - newT );

		// phi: compare axes to determine sign of beta
		//  because implicit axis (Baxis) is not used
		gwiz::float_t phi = Baxis.normal().dot( ( newT - R ).normal() );
		if( phi < 0.0 ) {
			beta = -beta;
		}
	}

	quat_t Qbeta( beta, RT ); // to apply beta to original tangent-point axis

#if 0
if( G_debug )	{
LOG( "--\n" );

LOG( "alpha: %f\n", alpha );
euler_t( Qalpha ).print();
LOG( "beta:  %f\n", beta );
euler_t( RT ).print();

LOG( "--\n" );
}
#endif

	// combine ( alpha * beta ) with buffer-zone solution
	quat_t Qresult = ( Qalpha * Qbeta ).lerp( buffer_lerp, Qbuffer );
//	quat_t Qresult = ( Qalpha * Qbeta );

	return( Qresult );
}

//////////////////////////////////////////////////////////////////////////

void test_forward_ray( void )	{

#if 0
	vector_t X( 0.0, 4.0, 2.0 ); // heading-only case
	vector_t R( 0.0, 0.0, 0.0 );
	vector_t Fo( 1.0, 5.0, 0.0 );
	vector_t Fd( 0.0, 0.0, 1.0 );
#elif 0
	vector_t X( 4.0, 10.0, 0.0 );
	vector_t R( 4.0, 5.0, 0.0 );
	vector_t Fo( 4.0, 1.0, 0.0 );
	vector_t Fd( 0.0, 1.0, 1.0 );
#elif 0
	vector_t X( 4.0, 10.0, 0.0 );
	vector_t R( 4.0, 0.0, 0.0 );
	vector_t Fo( 0.0, 0.0, 0.0 );
//	vector_t Fd( 0.0, 1.0, 0.0 );
	vector_t Fd( 0.0, 1.0, 1.0 );
#elif 1
	vector_t X( 4.0, 10.0, 10.0 );
	vector_t R( 4.0, 0.0, 0.0 );
	vector_t Fo( 0.0, 0.0, 0.0 );
//	vector_t Fd( 0.0, 1.0, 0.0 );
//	vector_t Fd( 0.0, 0.0, 1.0 );
	vector_t Fd( 0.0, 1.0, 1.0 );
#else
	vector_t X( 4.0, -10.0, 30.0 );
	vector_t R( -3.0, 4.0, 5.0 );
	vector_t Fo( -10.0, 3.0, -2.0 );
	vector_t Fd( 5.0, -10.0, 1.0 );
#endif

	LOG( "--- FORWARD RAY TEST:\n" );
	quat_t Q = rotation_ray_to_target_point( X, R, Fo, Fd, 0.0 );

	LOG( "ROTATION:\n" );
	euler_t E = Q;
	E.print();

	LOG( "NEW FORWARD DIR:\n" );
	vector_t newFd = Q * Fd;
	newFd.normal().print();

	LOG( "FORWARD ORIGIN TO TARGET:\n" );
	vector_t Td = X - ( R + Q * ( Fo - R ) );
	Td.normal().print();

	LOG( "DIFF:\n" );
	( Td.normal() - newFd.normal() ).print();
	
	LOG( "DIFF HEADING-ONLY:\n" );
	( vector_t( Td.x(), 0.0, Td.z() ).normal() - vector_t( newFd.x(), 0.0, newFd.z() ).normal() ).print();
	LOG( "---\n" );
}
