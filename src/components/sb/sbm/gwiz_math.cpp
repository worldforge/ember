/*
 *  gwiz_math.cpp - part of SmartBody-lib
 *  Copyright (C) 2001-2008  Marcus Thiebaux, University of Southern California
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

#include "sbm/gwiz_math.h"
//#include "sbm/lin_win.h"

//int gwiz::temp_read2( void ) { return( temp_test ); }

using namespace gwiz;
#define gw_float	gwiz::float_t // Ubuntu: float_t defined in mathdef.h

//int temp_read2( void ) { return( temp_test ); }

////////////////////////////////
// SAFE TRIGONOMETRY:

// need to use gwiz:: prefix to disambiguate definition within gwiz scope

//float_t gwiz::safe_asin( float_t s ) { // OK on Windows
gwiz::float_t gwiz::safe_asin( float_t s ) {

	if( s >= safe_arc_threshold() ) s = safe_arc_threshold();
	else
	if( s <= -safe_arc_threshold() ) s = -safe_arc_threshold();
	return( asin( s ) );
}

gwiz::float_t gwiz::safe_acos( float_t c ) {

	if( c >= safe_arc_threshold() ) c = safe_arc_threshold();
	else
	if( c <= -safe_arc_threshold() ) c = -safe_arc_threshold();
	return( acos( c ) );
}

gwiz::float_t gwiz::safe_atan( float_t t ) {

	if( t >= safe_arc_threshold() ) t = safe_arc_threshold();
	else
	if( t <= -safe_arc_threshold() ) t = -safe_arc_threshold();
	return( atan( t ) );
}

////////////////////////////////
// MATRIX UTILS

//inline float_t determinant2x2(  // OK on Windows
//	float_t a, gw_float b, 
inline gw_float determinant2x2( 
	gw_float a, gw_float b, 
	gw_float c, gw_float d 
	)	{

		return( a * d - b * c );
}

inline gw_float determinant3x3( 
	gw_float a1, gw_float a2, gw_float a3, 
	gw_float b1, gw_float b2, gw_float b3, 
	gw_float c1, gw_float c2, gw_float c3 
	)	{

		return(
			a1 * determinant2x2( b2, b3, c2, c3 ) -
			b1 * determinant2x2( a2, a3, c2, c3 ) +
			c1 * determinant2x2( a2, a3, b2, b3 )
		);
}

inline gw_float determinant4x4( const gw_float M[4][4] )	{
	gw_float a2 = M[1][0]; 
	gw_float b2 = M[1][1]; 
	gw_float c2 = M[1][2]; 
	gw_float d2 = M[1][3];
	gw_float a3 = M[2][0]; 
	gw_float b3 = M[2][1]; 
	gw_float c3 = M[2][2]; 
	gw_float d3 = M[2][3];
	gw_float a4 = M[3][0];
	gw_float b4 = M[3][1]; 
	gw_float c4 = M[3][2]; 
	gw_float d4 = M[3][3];

		return(
			M[0][0] * determinant3x3( b2, b3, b4, c2, c3, c4, d2, d3, d4 ) - 
			M[0][1] * determinant3x3( a2, a3, a4, c2, c3, c4, d2, d3, d4 ) + 
			M[0][2] * determinant3x3( a2, a3, a4, b2, b3, b4, d2, d3, d4 ) - 
			M[0][3] * determinant3x3( a2, a3, a4, b2, b3, b4, c2, c3, c4 )
		);
}

inline int inverse4x4( gw_float M_out[4][4], const gw_float M_in[4][4] )	{
	gw_float a1 = M_in[0][0]; 
	gw_float b1 = M_in[0][1]; 
	gw_float c1 = M_in[0][2]; 
	gw_float d1 = M_in[0][3];
	gw_float a2 = M_in[1][0]; 
	gw_float b2 = M_in[1][1]; 
	gw_float c2 = M_in[1][2]; 
	gw_float d2 = M_in[1][3];
	gw_float a3 = M_in[2][0]; 
	gw_float b3 = M_in[2][1];
	gw_float c3 = M_in[2][2]; 
	gw_float d3 = M_in[2][3];
	gw_float a4 = M_in[3][0]; 
	gw_float b4 = M_in[3][1]; 
	gw_float c4 = M_in[3][2]; 
	gw_float d4 = M_in[3][3];

		gw_float det0 = determinant3x3( b2, b3, b4, c2, c3, c4, d2, d3, d4 );
		gw_float det1 = determinant3x3( a2, a3, a4, c2, c3, c4, d2, d3, d4 );
		gw_float det2 = determinant3x3( a2, a3, a4, b2, b3, b4, d2, d3, d4 );
		gw_float det3 = determinant3x3( a2, a3, a4, b2, b3, b4, c2, c3, c4 );
		
		gw_float det4x4 = M_in[0][0] * det0 - M_in[0][1] * det1 + M_in[0][2] * det2 - M_in[0][3] * det3;
		if( fabs( det4x4 ) < epsilon8() ) {
			printf("inverse4x4 ERR: non-singular matrix\n");
			return( 0 );
		}
		
		gw_float inv = 1.0/det4x4;
		M_out[0][0] =   det0 * inv;
		M_out[1][0] = - det1 * inv;
		M_out[2][0] =   det2 * inv;
		M_out[3][0] = - det3 * inv;
		M_out[0][1] = - determinant3x3( b1, b3, b4, c1, c3, c4, d1, d3, d4 ) * inv;
		M_out[1][1] =   determinant3x3( a1, a3, a4, c1, c3, c4, d1, d3, d4 ) * inv;
		M_out[2][1] = - determinant3x3( a1, a3, a4, b1, b3, b4, d1, d3, d4 ) * inv;
		M_out[3][1] =   determinant3x3( a1, a3, a4, b1, b3, b4, c1, c3, c4 ) * inv;
		M_out[0][2] =   determinant3x3( b1, b2, b4, c1, c2, c4, d1, d2, d4 ) * inv;
		M_out[1][2] = - determinant3x3( a1, a2, a4, c1, c2, c4, d1, d2, d4 ) * inv;
		M_out[2][2] =   determinant3x3( a1, a2, a4, b1, b2, b4, d1, d2, d4 ) * inv;
		M_out[3][2] = - determinant3x3( a1, a2, a4, b1, b2, b4, c1, c2, c4 ) * inv;
		M_out[0][3] = - determinant3x3( b1, b2, b3, c1, c2, c3, d1, d2, d3 ) * inv;
		M_out[1][3] =   determinant3x3( a1, a2, a3, c1, c2, c3, d1, d2, d3 ) * inv;
		M_out[2][3] = - determinant3x3( a1, a2, a3, b1, b2, b3, d1, d2, d3 ) * inv;
		M_out[3][3] =   determinant3x3( a1, a2, a3, b1, b2, b3, c1, c2, c3 ) * inv;

		return( 1 );
}

inline int invert4x4( gw_float M[4][4] )	{
	gw_float a1 = M[0][0]; 
	gw_float b1 = M[0][1]; 
	gw_float c1 = M[0][2]; 
	gw_float d1 = M[0][3];
	gw_float a2 = M[1][0]; 
	gw_float b2 = M[1][1]; 
	gw_float c2 = M[1][2]; 
	gw_float d2 = M[1][3];
	gw_float a3 = M[2][0]; 
	gw_float b3 = M[2][1];
	gw_float c3 = M[2][2]; 
	gw_float d3 = M[2][3];
	gw_float a4 = M[3][0]; 
	gw_float b4 = M[3][1]; 
	gw_float c4 = M[3][2]; 
	gw_float d4 = M[3][3];
	
		gw_float det0 = determinant3x3( b2, b3, b4, c2, c3, c4, d2, d3, d4 );
		gw_float det1 = determinant3x3( a2, a3, a4, c2, c3, c4, d2, d3, d4 );
		gw_float det2 = determinant3x3( a2, a3, a4, b2, b3, b4, d2, d3, d4 );
		gw_float det3 = determinant3x3( a2, a3, a4, b2, b3, b4, c2, c3, c4 );

		gw_float det4x4 = M[0][0] * det0 - M[0][1] * det1 + M[0][2] * det2 - M[0][3] * det3;
		if( fabs( det4x4 ) < epsilon8() ) {
			printf("invert4x4 ERR: non-singular matrix\n");
			return( 0 );
		}
		
		gw_float inv = 1.0/det4x4;
		M[0][0] =   det0 * inv;
		M[1][0] = - det1 * inv;
		M[2][0] =   det2 * inv;
		M[3][0] = - det3 * inv;
		M[0][1] = - determinant3x3( b1, b3, b4, c1, c3, c4, d1, d3, d4 ) * inv;
		M[1][1] =   determinant3x3( a1, a3, a4, c1, c3, c4, d1, d3, d4 ) * inv;
		M[2][1] = - determinant3x3( a1, a3, a4, b1, b3, b4, d1, d3, d4 ) * inv;
		M[3][1] =   determinant3x3( a1, a3, a4, b1, b3, b4, c1, c3, c4 ) * inv;
		M[0][2] =   determinant3x3( b1, b2, b4, c1, c2, c4, d1, d2, d4 ) * inv;
		M[1][2] = - determinant3x3( a1, a2, a4, c1, c2, c4, d1, d2, d4 ) * inv;
		M[2][2] =   determinant3x3( a1, a2, a4, b1, b2, b4, d1, d2, d4 ) * inv;
		M[3][2] = - determinant3x3( a1, a2, a4, b1, b2, b4, c1, c2, c4 ) * inv;
		M[0][3] = - determinant3x3( b1, b2, b3, c1, c2, c3, d1, d2, d3 ) * inv;
		M[1][3] =   determinant3x3( a1, a2, a3, c1, c2, c3, d1, d2, d3 ) * inv;
		M[2][3] = - determinant3x3( a1, a2, a3, b1, b2, b3, d1, d2, d3 ) * inv;
		M[3][3] =   determinant3x3( a1, a2, a3, b1, b2, b3, c1, c2, c3 ) * inv;

		return( 1 );
}

////////////////////////////////
// ROTATION UTILS

inline void matrix4x4FromQuat( gw_float M[4][4], const quat_t& q )	{
	register gw_float xx = q.x() * q.x(); 
	register gw_float yy = q.y() * q.y(); 
	register gw_float zz = q.z() * q.z();
	register gw_float xy = q.x() * q.y(); 
	register gw_float xz = q.x() * q.z(); 
	register gw_float yz = q.y() * q.z();
	register gw_float wx = q.w() * q.x(); 
	register gw_float wy = q.w() * q.y(); 
	register gw_float wz = q.w() * q.z();

		M[ 0 ][ 0 ] = 1.0 - 2.0*( yy + zz );
		M[ 0 ][ 1 ] = 2.0*( xy + wz );
		M[ 0 ][ 2 ] = 2.0*( xz - wy );
		M[ 0 ][ 3 ] = 0.0;
		M[ 1 ][ 0 ] = 2.0*( xy - wz );
		M[ 1 ][ 1 ] = 1.0 - 2.0*( zz + xx );
		M[ 1 ][ 2 ] = 2.0*( yz + wx );
		M[ 1 ][ 3 ] = 0.0;
		M[ 2 ][ 0 ] = 2.0*( xz + wy );
		M[ 2 ][ 1 ] = 2.0*( yz - wx );
		M[ 2 ][ 2 ] = 1.0 - 2.0*( xx + yy );
		M[ 2 ][ 3 ] = 0.0;
		M[ 3 ][ 0 ] = 0.0;
		M[ 3 ][ 1 ] = 0.0;
		M[ 3 ][ 2 ] = 0.0;
		M[ 3 ][ 3 ] = 1.0;
}

////////

inline quat_t quatFrom4x4( const gw_float M[4][4] )   {
    register gw_float ww, xx, yy, rcp, qx, qy, qz, qw;
    
		ww = 0.25*( 1.0 + M[0][0] + M[1][1] + M[2][2] );
		if( ww > epsilon6() ) {
			qw = sqrt( ww );
			rcp = 1.0 / ( 4.0*qw );
			qx = ( M[1][2] - M[2][1] )*rcp;
			qy = ( M[2][0] - M[0][2] )*rcp;
			qz = ( M[0][1] - M[1][0] )*rcp;
		}
		else	{
			qw = 0.0;
			xx = -0.5*( M[1][1] + M[2][2] );
			if( xx > epsilon6() )	{
				qx = sqrt( xx );
				rcp = 1.0 / ( 2.0*qx );
				qy = M[0][1]*rcp;
				qz = M[0][2]*rcp;
			}
			else    {
				qx = 0.0;
				yy = 0.5*( 1 - M[2][2] );
				if( yy > epsilon6() ) {
					qy = sqrt( yy );
					qz = M[1][2] / ( 2.0*qy );
				}
				else	{
					qy = 0.0;
					qz = 1.0;
				}
			}
		}

		double inv_mag = 1.0/sqrt( qw*qw + qx*qx + qy*qy + qz*qz );
		return( quat_t( qw*inv_mag, qx*inv_mag, qy*inv_mag, gw_float(qz*inv_mag) ) );
}

inline quat_t quatFrom3x3( const vector_t col[3] )	{
    register gw_float ww, xx, yy, rcp, qx, qy, qz, qw;
	
		ww = 0.25*( 1.0 + col[0].x() + col[1].y() + col[2].z() );
		if( ww > epsilon6() ) {
			qw = sqrt( ww );
			rcp = 1.0 / ( 4.0*qw );
			qx = ( col[1].z() - col[2].y() )*rcp;
			qy = ( col[2].x() - col[0].z() )*rcp;
			qz = ( col[0].y() - col[1].x() )*rcp;
		}
		else	{
			qw = 0.0;
			xx = -0.5*( col[1].y() + col[2].z() );
			if( xx > epsilon6() )	{
				qx = sqrt( xx );
				rcp = 1.0 / ( 2.0*qx );
				qy = col[0].y()*rcp;
				qz = col[0].z()*rcp;
			}
			else    {
				qx = 0.0;
				yy = 0.5*( 1 - col[2].z() );
				if( yy > epsilon6() ) {
					qy = sqrt( yy );
					qz = col[1].z() / ( 2.0*qy );
				}
				else	{
					qy = 0.0;
					qz = 1.0;
				}
			}
		}

		double inv_mag = 1.0/sqrt( qw*qw + qx*qx + qy*qy + qz*qz );
		return( quat_t( qw*inv_mag, qx*inv_mag, qy*inv_mag, gw_float(qz*inv_mag) ) );
}

////////

inline euler_t eulerFrom4x4( const gw_float M[4][4] )	{
	register gw_float ex_rad, ey_rad, ez_rad;

		gw_float s = -M[2][1];
        ex_rad = safe_asin( s );
		if( ( 1.0 - fabs( s ) ) > epsilon6() )	{
			ey_rad = atan2( M[2][0], M[2][2] );
			ez_rad = atan2( M[0][1], M[1][1] );
		}
		else	{
			ey_rad = atan2( -M[0][2], M[0][0] );
			ez_rad = 0.0;
		}
		return( euler_t( DEG( ex_rad ), DEG( ey_rad ), DEG( ez_rad ) ) );
}

inline euler_t eulerFrom3x3( const vector_t col[3] )	{
	register gw_float ex_rad, ey_rad, ez_rad;

		gw_float s = -col[2].y();
        ex_rad = safe_asin( s );
		if( ( 1.0 - fabs( s ) ) > epsilon6() )	{
			ey_rad = atan2( col[2].x(), col[2].z() );
  			ez_rad = atan2( col[0].y(), col[1].y() );
		}
		else	{
			ey_rad = atan2( -col[0].z(), col[0].x() );
			ez_rad = 0.0;
		}
		return( euler_t( DEG( ex_rad ), DEG( ey_rad ), DEG( ez_rad ) ) );
}

////////

inline euler_t eulerFromQuat( const quat_t& q )	{
	register gw_float xx = q.x() * q.x(); 
	register gw_float yy = q.y() * q.y(); 
	register gw_float zz = q.z() * q.z();
	register gw_float xz = q.x() * q.z(); 
	register gw_float wy = q.w() * q.y(); 
	register gw_float ex_rad, ey_rad, ez_rad;

		gw_float s = -( 2.0 - epsilon6() ) * ( q.y() * q.z() - q.w() * q.x() );
		ex_rad = safe_asin( s );
		if( ( 1.0 - fabs( s ) ) > epsilon6() )	{
			ey_rad = atan2( 2.0*( xz + wy ), 1.0 - 2.0*( xx + yy ) );
			ez_rad = atan2( 2.0*( q.x() * q.y() + q.w() * q.z() ), 1.0 - 2.0*( zz + xx ) );
		}
		else	{
			ey_rad = atan2( -2.0*( xz - wy ), 1.0 - 2.0*( yy + zz ) );
			ez_rad = 0.0;
		}
		return( euler_t( DEG( ex_rad ), DEG( ey_rad ), DEG( ez_rad ) ) );
}

////////

inline quat_t interpolateQuat( gw_float t, const quat_t& q0, const quat_t& q_to )	{
	gw_float s0, s1;
	gw_float retx, rety, retz, retw;
		
#if 0
		quat_t q1 = q_to.shortest( q0 );
		gw_float co = q0.dot( q1 );
#else
		quat_t q1;
		gw_float co = q0.dot( q_to );
		if( co < 0.0 )	{
			co = -co;
			q1 = q_to.complement();
		}
		else	{
			q1 = q_to;
		}
#endif

		if( ( 1.0 + co ) > epsilon6() )	{
			if( ( 1.0 - co ) > epsilon6() )   {
				gw_float om = safe_acos( co );
				gw_float rs = 1.0/sin( om );
				s0 = sin( ( 1.0 - t )*om )*rs;
				s1 = sin( t*om )*rs;
			}
			else    {
				s0 = 1.0 - t;
				s1 = t;
			}
			retx = ( q0.x() * s0 + q1.x() * s1 );
			rety = ( q0.y() * s0 + q1.y() * s1 );
			retz = ( q0.z() * s0 + q1.z() * s1 );
			retw = ( q0.w() * s0 + q1.w() * s1 );
		}
		else	{
			s0 = sin( ( 0.5 - t ) * M_PI );
			s1 = sin( t * M_PI );
			retx = ( q0.x() * s0 - q0.y() * s1 );
			rety = ( q0.y() * s0 + q0.x() * s1 );
			retz = ( q0.z() * s0 - q0.w() * s1 );
			retw = ( q0.w() * s0 + q0.z() * s1 );
		}

#if 0
		gw_float inv_mag = 1.0/sqrt( retw * retw + retx * retx + rety * rety + retz * retz );
		return( quat_t( retw * inv_mag, retx * inv_mag, rety * inv_mag, retz * inv_mag ) );
#else
		return( quat_t( retw, retx, rety, retz ) ); // normalized by constructor
#endif
}

////////

inline matrix_t matrixFromZY( const vector_t& z_axis, const vector_t& y_axis_approx )	{

	vector_t z_axis_n = z_axis.normal();
	vector_t y_axis_approx_n = y_axis_approx.normal();
	if( fabs( z_axis_n.dot( y_axis_approx_n ) ) > 0.999 ) { // degenerate
		return( euler_t( z_axis_n, 0.0 ) );
	}
	vector_t x_axis_n = y_axis_approx_n.cross( z_axis_n );
	vector_t y_axis_n = z_axis_n.cross( x_axis_n );

	return( matrix_t( x_axis_n, y_axis_n, z_axis_n ) );
}

////////////////////////////////
// MATRIX DECOMPOSITION UTILS

/* 
 * From Graphics Gems II: Spencer W. Thomas
 * 
 * "unmatrix" - Decompose a non-degenerate 4x4 transformation matrix into
 *  a standard sequence of primitive transformations that produce the 
 *  equivalent:
 * 
 * [Sx][Sy][Sz][Shearx/y][Sx/z][Sz/y][Rx][Ry][Rz][Tx][Ty][Tz][P(x,y,z,w)]
 * 
 *   P(x,y,z,w) * T(x,y,z) * R(x,y,z) * Sh(x/y,x/z,z/y) * Sc(x,y,z)
 */

inline int decompose4x4General( const gw_float M[4][4], vector4_t* P, vector_t* T, quat_t* Q, euler_t* E, vector_t* Sh, vector_t* Sc, int flag )	{
	register int i, j;
	
	/* Test for degeneracy. */
		if( fabs( M[3][3] ) < epsilon8() )	{
			printf( "decompose4x4General NOTIFY: degenerate matrix: M[3][3] == 0.0\n" );
			return( 0 );
		}
	
		gw_float M_tmp[4][4];
		for( i=0;i<4;i++ )	{
			M_tmp[i][0] = M[i][0]; 
			M_tmp[i][1] = M[i][1]; 
			M_tmp[i][2] = M[i][2]; 
			M_tmp[i][3] = M[i][3];
		}

		if( flag == COMP_UNKNOWN )	{

		/* Normalize the matrix. */
			gw_float inv_m33 = 1.0/M[3][3];
			for( i=0; i<4; i++ )
				for( j=0; j<4; j++ )
					M_tmp[i][j] *= inv_m33;
		
		/* Test for singularity. */
			gw_float det = determinant4x4( M_tmp );
			if( fabs( det ) < epsilon8() )	{
				printf( "decompose4x4General ERR: singular matrix: determinant4x4( M ) == 0.0\n" );
				return( 0 );
			}
		}

	// PERSPECTIVE TRACE
		if( P )	{
			if( 
				fabs( M_tmp[0][3] ) > epsilon6() || 
				fabs( M_tmp[1][3] ) > epsilon6() ||
				fabs( M_tmp[2][3] ) > epsilon6() 
				) {

				matrix_t pmat;
				pmat.set( 0, 0, M_tmp[0][0] );
				pmat.set( 0, 1, M_tmp[0][1] );
				pmat.set( 0, 2, M_tmp[0][2] );
				pmat.set( 0, 3, M_tmp[0][3] );
				pmat.set( 1, 0, M_tmp[1][0] );
				pmat.set( 1, 1, M_tmp[1][1] );
				pmat.set( 1, 2, M_tmp[1][2] );
				pmat.set( 1, 3, M_tmp[1][3] );
				pmat.set( 2, 0, M_tmp[2][0] );
				pmat.set( 2, 1, M_tmp[2][1] );
				pmat.set( 2, 2, M_tmp[2][2] );
				pmat.set( 2, 3, M_tmp[2][3] );
				pmat.set( 3, 0, 0.0 );
				pmat.set( 3, 1, 0.0 );
				pmat.set( 3, 2, 0.0 );
				pmat.set( 3, 3, 1.0 );
				*P = pmat.invert().transpose() * vector4_t( M_tmp[0][3], M_tmp[1][3], M_tmp[2][3], M_tmp[3][3] );
			}
			else	{
				*P = vector4_t( 0.0, 0.0, 0.0, 0.0 );
			}
		}

	// TRANSLATION
		if( T )	{
			*T = vector_t( M_tmp[3][0], M_tmp[3][1], M_tmp[3][2] );
		}
		
	// SCALE & SHEAR & ROTATION
		if( Sc || Sh || Q || E )	{
			vector_t col[3];
			gw_float Sc_x = 1.0;
			gw_float Sc_y = 1.0;
			gw_float Sc_z = 1.0;
	
		/* Isolate upper 3x3 columns and unravel scale and shear. */
			for( i=0; i<3; i++ ) {
				col[i] = vector_t( M_tmp[i][0], M_tmp[i][1], M_tmp[i][2] );
			}
	
#if 0
			printf( "X-Y cos: %f\n", col[0].normal().dot( col[1].normal() ) );
			printf( "X-Z cos: %f\n", col[0].normal().dot( col[2].normal() ) );
			printf( "Y-Z cos: %f\n", col[1].normal().dot( col[2].normal() ) );

			printf( "X-Y angle: %f\n", DEG( safe_acos( col[0].normal().dot( col[1].normal() ) ) ) );
			printf( "X-Z angle: %f\n", DEG( safe_acos( col[0].normal().dot( col[2].normal() ) ) ) );
			printf( "Y-Z angle: %f\n", DEG( safe_acos( col[1].normal().dot( col[2].normal() ) ) ) );
#endif

			if( flag != COMP_M_TR )	{
				
				gw_float Sh_xy = 1.0;
				gw_float Sh_xz = 1.0;
				gw_float Sh_yz = 1.0;
				
			/* Compute X scale factor and normalize first col. */
				Sc_x = col[0].length();
				if( Sc_x != 1.0 )
					col[0] /= Sc_x;
				
			/* Compute XY shear factor and make 2nd col orthogonal to 1st. */
				if( flag != COMP_M_TRS )	{
					Sh_xy = col[0].dot( col[1] );
					if( Sh_xy != 0.0 )
						col[1] = col[1] - col[0] * Sh_xy;
				}
				
			/* Now, compute Y scale and normalize 2nd col. */
				Sc_y = col[1].length();
				if( Sc_y != 1.0 )	{
					col[1] /= Sc_y;
					if( flag != COMP_M_TRS )
						Sh_xy /= Sc_y;
				}
	
			/* Compute XZ and YZ shears, orthogonalize 3rd col. */
				if( flag != COMP_M_TRS )	{
					Sh_xz = col[0].dot( col[2] );
					if( Sh_xz != 0.0 )
						col[2] = col[2] - col[0] * Sh_xz;
					Sh_yz = col[1].dot( col[2] );
					if( Sh_yz != 0.0 )
						col[2] = col[2] - col[1] * Sh_yz;
				}
				
			/* Next, get Z scale and normalize 3rd col. */
				Sc_z = col[2].length();
				
			// SHEAR
				if( Sh )	{
					if( Sc_z == 1.0 )	{
						*Sh = vector_t( Sh_xy, Sh_xz, Sh_yz );
					}
					else	{
						gw_float inv_Sc_z = 1.0 / Sc_z;
						*Sh = vector_t( Sh_xy, Sh_xz * inv_Sc_z, Sh_yz * inv_Sc_z );
					}
				}
			}
			
		/* At this point, the matrix (in cols[]) is orthonormal.
		 * If the coordinate system is flipped, negate the matrix and the scaling factors. 
		 */
		 
#if 0
			printf( "X-Y angle: %f\n", DEG( safe_acos( col[0].dot( col[1] ) ) ) );
			printf( "X-Z angle: %f\n", DEG( safe_acos( col[0].dot( col[2] ) ) ) );
			printf( "Y-Z angle: %f\n", DEG( safe_acos( col[1].dot( col[2] ) ) ) );
#endif
		 
			if( Sc || Q || E )	{

				if( flag != COMP_M_TR )	{
				
					if( Sc_z != 1.0 )	{
						col[2] /= Sc_z;
					}
					if( col[0].box( col[1], col[2] ) < 0.0 )	{
						Sc_x *= -1.0;
						col[0] *= -1.0;
					}
				}
				
			// SCALE
				if( Sc )	{
					*Sc = vector_t( Sc_x, Sc_y, Sc_z );
				}
			
			// ROTATION
				if( Q )	{
					*Q = quatFrom3x3( col );
				}
				if( E )	{
					*E = eulerFrom3x3( col );
				}
			}
		}
		
		return( 1 );
}

inline int decompose4x4TRS( const gw_float M[4][4], vector_t* T, quat_t* Q, euler_t* E, vector_t* Sc )	{
	register int i;
	
		if( T )	{
			*T = vector_t( M[3][0], M[3][1], M[3][2] );
		}
		
		vector_t col[3];
	
		for( i=0; i<3; i++ ) {
			col[i] = vector_t( M[i][0], M[i][1], M[i][2] );
		}

		gw_float Sc_x = col[0].length();
		gw_float Sc_y = col[1].length();
		gw_float Sc_z = col[2].length();

		if( Sc_x != 1.0 )
			col[0] /= Sc_x;
		if( Sc_y != 1.0 )
			col[1] /= Sc_y;
		if( Sc_z != 1.0 )
			col[2] /= Sc_z;

		if( col[0].box( col[1], col[2] ) < 0.0 )	{
			Sc_x *= -1.0;
			col[0] *= -1.0;
		}
		
		if( Sc )
			*Sc = vector_t( Sc_x, Sc_y, Sc_z );
		if( Q )
			*Q = quatFrom3x3( col );
		if( E )
			*E = eulerFrom3x3( col );
		
		return( 1 );
}

////////////////////////////////
// CLASS MEMBERS
////////////////////////////////

vector_t gwiz::vector_t::normal( void ) const	{
	register float_t len = length();

		if( len < epsilon6() )
			return( vector_t( 1.0, 0.0, 0.0 ) );
		return( (*this) / len );
}

vector_t& gwiz::vector_t::normalize( void )	{
	register float_t len = length();

		if( len < epsilon6() )
			return( (*this) = vector_t( 1.0, 0.0, 0.0 ) );
		return( (*this) /= len );
}

////////////////////////////////

gwiz::quat_t::quat_t( const vector_t& axis_angle )	{

	float_t rad = axis_angle.length();
	if( rad < epsilon6() )	{
		*this = quat_t();
	}
	else	{
		*this = quat_t( rad, axis_angle, 1 );
	}
}

gwiz::quat_t::quat_t( float_t angle, const vector_t& v, int use_radians )	{
	float_t rad_2;

	if( use_radians )
		rad_2 = 0.5 * angle;
	else
		rad_2 = 0.5 * RAD( angle );
	float_t len = v.length();
	if( len > 0.0 )	{
		W = cos( rad_2 );
		float_t f = sin( rad_2 ) / len;
		X = v.x() * f;
		Y = v.y() * f;
		Z = v.z() * f;
		normalize();
	}
	else
		*this = quat_t();
}

gwiz::quat_t::quat_t( float_t swing_x, float_t swing_y, float_t twist, int use_radians )	{
	
	quat_t swing;
	if( use_radians )	{
		swing = quat_t( vector_t( swing_x, swing_y, 0.0 ) );
	}
	else	{
		swing = quat_t( vector_t( RAD( swing_x ), RAD( swing_y ), 0.0 ) );	
	}
	*this = swing * quat_t( twist, vector_t( 0.0, 0.0, 1.0 ), use_radians );	
}

gwiz::quat_t::quat_t( const vector_t& z_axis, const vector_t& y_axis_approx )	{

	*this = matrixFromZY( z_axis, y_axis_approx ).quat( COMP_M_TR );
}

gwiz::quat_t::quat_t( const euler_t& e ) {

	*this = 
		quat_t( e.y(), vector_t( 0.0, 1.0, 0.0 ) ) *
		quat_t( e.x(), vector_t( 1.0, 0.0, 0.0 ) ) *
		quat_t( e.z(), vector_t( 0.0, 0.0, 1.0 ) );
}

vector_t gwiz::quat_t::swingtwist( int use_radians ) const	{

	if( use_radians )	{
		if( ( W < epsilon6() )&&( Z < epsilon6() ) )	{
			return( vector_t( M_PI, 0.0, 0.0 ) );
		}
	}
	else	{
		if( ( W < epsilon4() )&&( Z < epsilon4() ) )	{
			return( vector_t( 180.0, 0.0, 0.0 ) );
		}
	}
	
	quat_t q;
	if( W < 0.0 )	{
		q = complement();
	}
	else	{
		q = *this;
	}
	
	float_t gamma = atan2( q.Z, q.W );
	float_t beta = atan2( sqrt( q.X*q.X + q.Y*q.Y ), sqrt( q.Z*q.Z + q.W*q.W ) );
	float_t sinc = 1.0;
	if( beta > epsilon6() )	{
		sinc = sin( beta )/beta;
	}
	float_t s = sin( gamma );
	float_t c = cos( gamma );
	float_t sinc2 = 2.0 / sinc;
	float_t swing_x = sinc2 * ( c * q.X - s * q.Y );
	float_t swing_y = sinc2 * ( s * q.X + c * q.Y );
	float_t twist = 2.0 * gamma;
	
	if( use_radians )	{
		return( vector_t( swing_x, swing_y, twist ) );
	}
	return( vector_t( DEG( swing_x ), DEG( swing_y ), DEG( twist ) ) );
}

quat_t& gwiz::quat_t::normalize( void )	{
	register float_t len = sqrt( W*W + X*X + Y*Y + Z*Z );

#if 0
		if( len < epsilon6() )
			return( (*this) = quat_t( 1.0, 0.0, 0.0, 0.0 ) );
		float_t inv_len = 1.0/len;
		return( (*this) = quat_t( W * inv_len, X * inv_len, Y * inv_len, Z * inv_len ) );
#else
		if( len < epsilon6() )	{
			W = 1.0; X = 0.0; Y = 0.0; Z = 0.0;
		}
		else	{
			float_t inv_len = 1.0/len;
			W *= inv_len; X *= inv_len; Y *= inv_len; Z *= inv_len;
		}
		return( *this );
#endif
}

quat_t gwiz::quat_t::lerp( float_t s, const quat_t& q ) const {
#if 1
		return( interpolateQuat( s, *this, q.shortest( *this ) ) );
#else
		return( interpolateQuat( s, *this, q ) );
#endif
}

matrix_t gwiz::quat_t::operator * ( const matrix_t& R ) const {
		return( matrix_t( *this ) * R );
}

vector_t gwiz::quat_t::operator * ( const vector_t& v ) const { 

	// optimized ?
	//    q * vector4_t( v, 1.0 ) * -q ;
	//    q * quat_t( v.x(), v.y(), v.z(), 1.0 ) * -q ;
	//    q * quat_t( 0.0, v.x(), v.y(), v.z() ) * -q ;

		return( matrix_t( *this ) * v );
}

////////////////////////////////

gwiz::euler_t::euler_t( const vector_t& z_axis, float_t roll_deg )	{
	register float_t ex_rad, ey_rad;
	vector_t norm = z_axis.normal();
		
		float_t s = -norm.y();
		ex_rad = safe_asin( s );
		X = DEG( ex_rad );

		if( ( 1.0 - fabs( s ) ) > epsilon6() )	{
			ey_rad = atan2( -norm.z(), norm.x() );
			Y = DEG( ey_rad ) + 90.0;
			Z = roll_deg;
		}
		else	{
			Y = roll_deg;
			Z = 0.0;
		}
}

gwiz::euler_t::euler_t( const vector_t& z_axis, const vector_t& y_axis_approx )	{

	*this = matrixFromZY( z_axis, y_axis_approx ).euler( COMP_M_TR );
}

gwiz::euler_t::euler_t( const quat_t& q )	{ 

		*this = eulerFromQuat( q );
}

euler_t gwiz::euler_t::operator - ( void ) const { 

	// COMPARE: return( matrix_t( *this ).transposition() )
		return ( -quat_t( *this ) ); 
}

matrix_t gwiz::euler_t::operator * ( const matrix_t& R ) const {
		return( matrix_t( *this ) * R );
}

vector_t gwiz::euler_t::operator * ( const vector_t& v ) const { 
		return( matrix_t( *this ) * v );
}

////////////////////////////////

gwiz::matrix_t::matrix_t( const quat_t& q )	{
	
		stack = 0x0;
		matrix4x4FromQuat( M, q );
}

gwiz::matrix_t::matrix_t( const euler_t& e )	{

// buggy:
//		*this = quat_t( e );
//		*this = matrix_t( quat_t( e ) );

		stack = 0x0;
		matrix4x4FromQuat( M, quat_t( e ) );
}

matrix_t gwiz::matrix_t::inverse( void )	const {
	matrix_t tmp;
		
		if( inverse4x4( tmp.M, M ) == 0 )	{
			return( tmp.identity() );
		}
		return( tmp );
}

matrix_t& gwiz::matrix_t::invert( void )	{
		
		if( invert4x4( M ) == 0 )	{
			matrix_t tmp;
			return( tmp.identity() );
		}
		return( *this );
}

////////

matrix_t& gwiz::matrix_t::frustum( float_t l, float_t r, float_t b, float_t t, float_t n, float_t f )	{
		M[0][0] = ( 2.0 * n )/( r - l );
		M[0][1] = 0.0;
		M[0][2] = 0.0;
		M[0][3] = 0.0;
		M[1][0] = 0.0;
		M[1][1] = ( 2.0 * n )/( t - b );
		M[1][2] = 0.0;
		M[1][3] = 0.0;
		M[2][0] = ( r + l )/( r - l );
		M[2][1] = ( t + b )/( t - b );
		M[2][2] = -( f + n )/( f - n );
		M[2][3] = -1.0;
		M[3][0] = 0.0;
		M[3][1] = 0.0;
		M[3][2] = -( 2.0 * f * n )/( f - n );
		M[3][3] = 0.0;
		return( *this );
}

matrix_t& gwiz::matrix_t::frustum( const vector_t& L_B_N, const vector_t& R_T_F )	{
		return( frustum( L_B_N.x(), R_T_F.x(), L_B_N.y(), R_T_F.y(), L_B_N.z(), R_T_F.z() ) );
}

matrix_t& gwiz::matrix_t::frustum( const vector_t& B_L_dir, const vector_t& R_T_dir, float_t near, float_t far )	{
		vector_t norm = B_L_dir.normal();
		vector_t BL = norm * ( -near / norm.z() );
		norm = R_T_dir.normal();
		vector_t TR = norm * ( -near / norm.z() );
		return( frustum( BL.x(), TR.x(), BL.y(), TR.y(), near, far ) );
}

matrix_t& gwiz::matrix_t::frustum( float_t fovy, float_t aspect, float_t near, float_t far )	{
		float_t T = near * tan( RAD( fovy * 0.5 ) );
		float_t R = aspect * T;
		return( frustum( -R, R, -T, T, near, far ) );
}

matrix_t& gwiz::matrix_t::translator( const vector_t& t )	{
		identity();
		M[3][0] = t.x();
		M[3][1] = t.y();
		M[3][2] = t.z();
		return( *this );
}

matrix_t& gwiz::matrix_t::rotator( const quat_t& q )	{
		return( *this = q );
}

matrix_t& gwiz::matrix_t::rotator( const euler_t& e )	{
		return( *this = e );
}

matrix_t& gwiz::matrix_t::shearer( const vector_t& s )	{
		identity();
		M[1][0] = s.x();
		M[2][0] = s.y();
		M[2][1] = s.z();
		return( *this );
}

matrix_t& gwiz::matrix_t::scaler( const vector_t& s )	{
		identity();
		M[0][0] = s.x();
		M[1][1] = s.y();
		M[2][2] = s.z();
		return( *this );
}

matrix_t& gwiz::matrix_t::lineator( const vector_t& x, const vector_t& y, const vector_t& z )	{
		identity();
		col( 0, x, 0.0 );
		col( 1, y, 0.0 );
		col( 2, z, 0.0 );
		return( *this );
}

////////

int gwiz::matrix_t::decompose( vector4_t* P, vector_t* T, quat_t* Q, euler_t * E, vector_t* Sh, vector_t* Sc, int flag ) const	{

		if( flag == COMP_M_TR )	{
			if( T ) *T = translation( COMP_M_TR );
			if( Q || E )	{
				quat_t q = quat( COMP_M_TR );
				if( Q ) *Q = q;
				if( E ) *E = q;
			}
			return( 1 );
		}
		if( flag == COMP_M_TRS )	{
			return( decompose4x4TRS( M, T, Q, E, Sc ) );
		}
		return( decompose4x4General( M, P, T, Q, E, Sh, Sc, flag ) );
}

////////

vector4_t gwiz::matrix_t::perspective( int flag ) const	{

		vector4_t P;
		decompose( &P, 0x0, 0x0, 0x0, 0x0, 0x0, flag );
		return( P );
}

vector_t gwiz::matrix_t::translation( int flag ) const	{

		if( flag == COMP_UNKNOWN )	{
			vector_t T;
			decompose( 0x0, &T, 0x0, 0x0, 0x0, 0x0, flag );
			return( T );
		}
		return( vector_t( M[3][0], M[3][1], M[3][2] ) );
}

quat_t gwiz::matrix_t::quat( int flag ) const	{

		if( flag == COMP_M_TR )	{
			return( quatFrom4x4( M ) );
		}
		quat_t Q;
		if( flag == COMP_M_TRS )
			decompose4x4TRS( M, 0x0, &Q, 0x0, 0x0 );
		else
			decompose( 0x0, 0x0, &Q, 0x0, 0x0, 0x0, flag );
		return( Q );
}

euler_t gwiz::matrix_t::euler( int flag ) const	{

		if( flag == COMP_M_TR )	{
			return( eulerFrom4x4( M ) );
		}
		euler_t E;
		if( flag == COMP_M_TRS )
			decompose4x4TRS( M, 0x0, 0x0, &E, 0x0 );
		else
			decompose( 0x0, 0x0, 0x0, &E, 0x0, 0x0, flag );
		return( E );
}

vector_t gwiz::matrix_t::shearing( int flag ) const	{

		vector_t Sh;
		decompose( 0x0, 0x0, 0x0, 0x0, &Sh, 0x0, flag );
		return( Sh );
}

vector_t gwiz::matrix_t::scaling( int flag ) const	{

		vector_t Sc;
		decompose( 0x0, 0x0, 0x0, 0x0, 0x0, &Sc, flag );
		return( Sc );
}

////////////////////////////////

///////////////////////////////////////////////////////////////////////////

#if 0
void read_gwiz_composition( vector_t T, euler_t E, vector_t S )	{
	matrix_t M;

	M.id().translate( T ).rotate( E ).scale( S );
	
	vector_t t;
	quat_t q;
	euler_t e;
	vector_t s;
	
	printf( "decompose: \n" );
	M.decompose(
		NULL, 
		& t, 
		& q, 
		& e, 
		NULL, 
		& s, 
//		COMP_UNKNOWN 
//		COMP_M_PTRSH 
//		COMP_M_TRSH 
		COMP_M_TRS 
//		COMP_M_TR 
	);
	
	printf( "\n" );
	printf( "t: %f %f %f\n", t.x(), t.y(), t.z() );
	
	printf( "\n" );
	vector_t a = q.axis();
	printf( "q: %f { %f %f %f }\n", q.degrees(), a.x(), a.y(), a.z() );

	// quat:
	// COMP_M_TR: 46.0 { 0.08 0.97 0.20 }
	
	// euler:
	// COMP_M_TR: 45 0 10
	
	printf( "\n" );
	printf( "e: %f %f %f\n", e.y(), e.x(), e.z() );
	
	printf( "\n" );
	quat_t Q = e;
	vector_t A = Q.axis();
	printf( "Q: %f { %f %f %f }\n", Q.degrees(), A.x(), A.y(), A.z() );
	
	// euler->quat:
	// COMP_M_TR: OK: 46.0 { 0.08 0.97 0.20 }
	
	printf( "\n" );
	printf( "s: %f %f %f\n", s.x(), s.y(), s.z() );
	
	printf( "\n" );
}

void test_gwiz_composition( void )	{

	vector_t T( 1.0, 2.0, 3.0 );
	euler_t E( vector_t( 1.0, 0.0, 1.0 ), 10.0 );
	vector_t S( 4.0, 5.0, 6.0 );
	
//	vector_t T( 1.0, 2.0, 3.0 );
//	euler_t E( vector_t( 0.0, 1.0, 1.0 ), 10.0 );
	
	read_gwiz_composition( T, E, S );
}

void read_axis_angle( euler_t e )	{
	
	printf( "--\n" );
	e.print();
	quat_t q1 = e;
	
	vector_t axis_angle = q1.axisangle();
	quat_t q2 = quat_t( axis_angle );
	
	euler_t e2 = q2;
	e2.print();
}

void read_swing_twist( euler_t e )	{
	
	printf( "--\n" );
	e.print();
	quat_t q1 = e;
	
	vector_t swing_twist = q1.swingtwist();
	quat_t q2 = quat_t( swing_twist.x(), swing_twist.y(), swing_twist.z() );
	
	euler_t e2 = q2;
	e2.print();
}

void test_axis_angle( void )	{

#if 0
	read_axis_angle( euler_t( 0.0, 0.0, 0.0 ) );

	read_axis_angle( euler_t( 90.0, 0.0, 0.0 ) );
	read_axis_angle( euler_t( 0.0, 90.0, 0.0 ) );
	read_axis_angle( euler_t( 0.0, 0.0, 90.0 ) );

	read_axis_angle( euler_t( 179.999995, 0.0, 0.0 ) );

	read_axis_angle( euler_t( 180.0, 0.0, 0.0 ) );
	read_axis_angle( euler_t( 0.0, 180.0, 0.0 ) );
	read_axis_angle( euler_t( 0.0, 0.0, 180.0 ) );

	read_axis_angle( euler_t( 43.0, 25.0, 13.0 ) );

#elif 0

	double rot = 179.9;
	read_axis_angle( euler_t( rot, rot, 0.0 ) );

	rot = 179.99995;
	read_axis_angle( euler_t( rot, rot, 0.0 ) );

	rot = 180.0;
	read_axis_angle( euler_t( rot, rot, 0.0 ) );
	
#elif 0
//	quat_t q1( vector_t( M_PI, M_PI, 0.0 ) );
	quat_t q1( vector_t( M_PI, 0.0, 0.0 ) );
	quat_t q2( 0.0, vector_t( 0.0, 0.0, 1.0 ) );
	
	euler_t e1 = q1;
	euler_t e2 = q2;
	
	quat_t q3 = q1 * q2;
	euler_t e3 = q3;
	
	e1.print();
	e2.print();
	e3.print();
	
#else
	read_swing_twist( euler_t( 0.0, 0.0, 0.0 ) );

	read_swing_twist( euler_t( 90.0, 0.0, 0.0 ) );
	read_swing_twist( euler_t( 0.0, 90.0, 0.0 ) );
	read_swing_twist( euler_t( 0.0, 0.0, 90.0 ) );
	
	double rot = 179.9;
	read_swing_twist( euler_t( rot, 0.0, 0.0 ) );
	read_swing_twist( euler_t( 0.0, rot, 0.0 ) );
	read_swing_twist( euler_t( 0.0, 0.0, rot ) );

	rot = 179.99995;
	read_swing_twist( euler_t( rot, 0.0, 0.0 ) );
	read_swing_twist( euler_t( 0.0, rot, 0.0 ) );
	read_swing_twist( euler_t( 0.0, 0.0, rot ) );

	read_swing_twist( euler_t( 180.0, 0.0, 0.0 ) );
	read_swing_twist( euler_t( 0.0, 180.0, 0.0 ) );
	read_swing_twist( euler_t( 0.0, 0.0, 180.0 ) );

	read_swing_twist( euler_t( 43.0, 25.0, 13.0 ) );
#endif
}

void test_quat_product( void )	{
	quat_t P = euler_t( 30.0, 0.0, 0.0 ); // pitch
	quat_t H = euler_t( 0.0, 25.5, 0.0 ); // heading
	quat_t R = euler_t( 0.0, 0.0, 10.0 ); // heading
	euler_t e;
	
	// the correct composition/decomposition:
	e = H * P * R;
	e.print();
	e = H.product( P.product( R ) );
	e.print();
	e = R.product_left( P.product_left( H ) );
	e.print();
	
	// the reversed composition/decomposition:
	e = R * P * H;
	e.print();
	e = R.product( P.product( H ) );
	e.print();
	e = H.product_left( P.product_left( R ) );
	e.print();
}

#endif

///////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////

#if 0

void print_quat_rot( char * prefix, quat_t q ) {
	float_t angle = q.degrees();
	vector_t axis = q.axis();
//	printf( "W: %.16f acos: %.16f\n", q.w(), acos( q.w() ) );
	printf( "Q(%s): %f : { %f, %f, %f }\n", prefix, angle, axis.x(), axis.y(), axis.z() );
}

void print_quat_compare( quat_t A, quat_t B )	{
	print_quat_rot( "A", A ); // from
	print_quat_rot( "B", B ); // to
	float_t dot = A.dot( B );
	printf( "dot: %f : %f deg\n", dot, DEG( 2.0 * acos( dot ) ) );
}

void test_quat_diff_print( quat_t A, quat_t B )	{
	
	printf( "-- --\n" );
	print_quat_compare( A, B );
	printf( "--\n" );

	print_quat_rot( " BpA  ", B.product( A ) );
	print_quat_rot( " BpAs ", B.product( A ).shortest() );
	print_quat_rot( " AS   ", A.shortest( B ) );
	print_quat_rot( " BS   ", B.shortest( A ) );
	print_quat_rot( " BpAS ", B.product( A.shortest( B ) ) );

#if 0
	print_quat_rot( " A*B  ", A * B );
	
	print_quat_rot( " A*Bs ", ( A * B ).shortest() );
	
	print_quat_rot( " -A*B ", ( -A ) * B );
	
	print_quat_rot( " -A*Bs", ( ( -A ) * B ).shortest() );
	
	print_quat_rot( " -ApB ", ( -A ).product( B ) );
	
	print_quat_rot( " B*-A ", B * ( -A ) );
	
	print_quat_rot( " Bp-A ", B.product( -A ) );
#endif
}

void test_quat_difference( void )	{

	printf( "-- -- -- --\n" );
	test_quat_diff_print(
		euler_t( 0.0, -89.0, 0.0 ),
		euler_t( 0.0, 89.0, 0.0 )
	);
	printf( "-- -- -- --\n" );
	test_quat_diff_print(
		euler_t( 0.0, -91.0, 0.0 ),
		euler_t( 0.0, 91.0, 0.0 )
	);

#if 0
	test_quat_diff_print(
		euler_t( 0.0, -10.0, 0.0 ),  // from
		euler_t( 0.0, 10.0, 0.0 )    // to
	);
	test_quat_diff_print(
		euler_t( 0.0, 10.0, 0.0 ),  // from
		euler_t( 0.0, -10.0, 0.0 )    // to
	);
#endif
#if 0
	test_quat_diff_print(
		euler_t( 0.0, -45.0, 0.0 ),
		euler_t( 0.0, 35.0, 0.0 )
	);
	test_quat_diff_print(
		euler_t( 0.0, 35.0, 0.0 ),
		euler_t( 0.0, -45.0, 0.0 )
	);
#endif
#if 0
	test_quat_diff_print(
		euler_t( 0.0, -89.0, 0.0 ),
		euler_t( 0.0, 89.0, 0.0 )
	);
	test_quat_diff_print(
		euler_t( 0.0, 89.0, 0.0 ),
		euler_t( 0.0, -89.0, 0.0 )
	);
#endif
#if 0
	test_quat_diff_print(
		euler_t( 0.0, -91.0, 0.0 ),
		euler_t( 0.0, 91.0, 0.0 )
	);
	test_quat_diff_print(
		euler_t( 0.0, 91.0, 0.0 ),
		euler_t( 0.0, -91.0, 0.0 )
	);
#endif
#if 0
	test_quat_diff_print(
		euler_t( 0.0, -120.0, 0.0 ),
		euler_t( 0.0, 121.0, 0.0 )
	);
	test_quat_diff_print(
		euler_t( 0.0, 120.0, 0.0 ),
		euler_t( 0.0, -120.0, 0.0 )
	);
#endif
}

int main( int ac, char** av )	{
	
	test_quat_difference();

	return( 0 );
}

#endif


