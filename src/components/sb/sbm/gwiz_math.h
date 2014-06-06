/*
 *  gwiz_math.h - part of SmartBody-lib
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

#ifndef GWIZ_MATH_H
#define GWIZ_MATH_H

/*
#include "gwiz.h"

// "Gee Whiz!"
//    AKA: Jesu H. Chee-ryste! Why isn't this standard?
// Vector geometry wizard... a precursor to Geometric Algebra: algebraic geometry

// NOTES...
	typedef vector_t	vector3_t;
	typedef quat_t		quaternion_t;
	typedef euler_t		eulerPHR_t;
	typedef matrix_t	matrix4x4_t;

	namespace gwiz	{

	// constants
		epsilonX
		pi

	// basic types
		scalar: scalar_t: gw_float: float_t: double // 64-bit precision floating point Real
		vector: vector_t: 3 floats: a drawable 3D vertex.
		quat: quat_t: gw_quat
		euler: euler_t
		xmat: matrix_t
		xstack: matrix4_stack

	// camera/port/xform
		screen_t
		camera_t
		viewport_t

	// splines
		ctrl_key
		cardinal_key
		tempordinal_key
	}
*/

////////////////////////////////

#include <sb/SBTypes.h>

#include <math.h>
#include <stdio.h>

#define GWIZ_version	"Version 5.1 : Mar 2, 2011"

namespace gwiz { 

//#define GWIZ_32BIT
#ifdef GWIZ_32BIT
	typedef float float_t; // DEPRECATED
#else
	// 64 bit precision for sparse transformation data, ought to be enough...
	typedef double float_t;
#endif

	// forward declarations for circular references
	class vector_t;
	typedef vector_t vector3_t;

	class quat_t;
	class euler_t;
	class matrix_t;
}

#ifndef M_PI
#define M_PI	( gwiz::pi() )
#endif










































#ifndef RAD // ( degrees )
#define RAD 	( gwiz::rad )
#endif
#ifndef DEG // ( radians )
#define DEG 	( gwiz::deg )
#endif

////////////////////////////////
/*
namespace gwiz_sample_namespace_for_first_timers {

	class gwiz_cl	{
	public:
		int x() { return( X ); }
		int x_cnst( void ) const { return( 0 ); }
	private:
		int X;
	};
//	int x; No... only types in a namespace
//	int x_fcn( void ) { return( 0 ); } No!
	static int y_fcn( void ) { return( 0 ); }
//	int z_fcn( void ) const { return( 0 ); } No!
	int a_cpp_fcn( void ); // cpp def'd
}
// cpp: int gwiz_sample_namespace_for_first_timers::a_cpp_fcn( void ) { return( 0 ); }
*/

namespace gwiz {

/*
	// decl/defn examples
	static int temp_test = 0;
	static int temp_read( void ) { return( ++temp_test ); }
	int temp_read2( void );
*/
  // matrix_t composition optimization flags
	enum martrix_enum_set    {
		COMP_UNKNOWN, 
		COMP_M_TR, 
		COMP_M_TRS, 
		COMP_M_TRSH, 
		COMP_M_PTRSH
	};

  // precision constants, typically between 5 and 10:
	static float_t epsilon4( void )	{ return( 0.00001 ); } // used by vector_t quat_t::swingtwist( deg )
	static float_t epsilon5( void )	{ return( 0.000001 ); } /* one millionth */
	static float_t epsilon6( void )	{ return( 0.0000001 ); }
	static float_t epsilon7( void )	{ return( 0.00000001 ); }
	static float_t epsilon8( void )	{ return( 0.000000001 ); } /* one billionth */
	static float_t epsilon9( void )	{ return( 0.0000000001 ); }
	static float_t epsilon10( void ) { return( 0.00000000001 ); }
	static float_t epsilon11( void ) { return( 0.000000000001 ); } /* one trillionth */

  // trigonometry
	static float_t pi( void ) { return( 3.14159265358979323846 ); }
	static float_t rad( float_t deg ) { return( deg * 0.017453292519943295 ); }
	static float_t deg( float_t rad ) { return( rad * 57.295779513082323 ); }
	static float_t safe_arc_threshold( void ) { return( 1.0 - epsilon8() ); }
	float_t safe_asin( float_t s );
	float_t safe_acos( float_t c );
	float_t safe_atan( float_t t );

#if 1
  // Ubuntu warning suppression
 	static double use_warn_suppress(void)	{
		static double x = use_warn_suppress(); // use once
		x = epsilon4(); x = epsilon5(); x = epsilon6(); 
		x = epsilon7(); x = epsilon8(); x = epsilon9();
		x = epsilon10(); x = epsilon11();
		x = pi(); x = rad(0); x = deg(0);
		x = safe_arc_threshold();
		return(0);
	}
#endif

  // spline member-classes

	class ctrl_key	{
			
		private:
			float_t param, value;
		public:
			ctrl_key( void ) { set( 0.0, 0.0 ); }
			ctrl_key( float_t p_in, float_t v_in )	{ set( p_in, v_in ); }

			void set( float_t p_in, float_t v_in )	{ param = p_in; value = v_in; }
			float_t p( void ) const { return( param ); }
			float_t v( void ) const { return( value ); }

			// maybe not the correct place for this...
			// GWIZ gui utils... simple 2D bounds checking: box, circle, rect.
			bool bound_box( float_t p_comp, float_t v_comp, float_t radius ) const;
	};

	class cardinal_key: public ctrl_key	{

		private:
			float_t m_lt, m_rt;		
		public:

			cardinal_key( void ) { set( 0.0, 0.0, 0.0, 0.0 ); }
			cardinal_key( 
				float_t t_in, float_t v_in,
				float_t ml_in, float_t mr_in
			)  { set( t_in, v_in, ml_in, mr_in ); }

			void set( 
				float_t t_in, float_t v_in,
				float_t ml_in, float_t mr_in )	{ 
				ctrl_key::set( t_in, v_in ); 
				m_lt = ml_in; m_rt = mr_in; 
			}

			float_t ml( void ) const { return( m_lt ); }
			float_t mr( void ) const { return( m_rt ); }
	};

	class tempordinal_key : public cardinal_key	{

		private:
			float_t d_lt, d_rt;
		public:
			tempordinal_key( void ) { set( 0.0, 0.0, 0.0, 0.0, 1.0, 1.0 ); }
			void set( 
				float_t t_in, float_t v_in,
				float_t ml_in, float_t mr_in,
				float_t dl_in, float_t dr_in )	{ 
				cardinal_key::set( t_in, v_in, ml_in, mr_in ); 
				d_lt = dl_in; d_rt = dr_in; 
			}

			float_t dl( void ) const { return( d_lt ); }
			float_t dr( void ) const { return( d_rt ); }

			void simple( const ctrl_key& k0, const ctrl_key& k1, const ctrl_key& k2 );

			void halting( const ctrl_key& k0, const ctrl_key& k1, const ctrl_key& k2 ) 
				{ cardinal( 1.0, k0, k1, k2 ); }

			void catmullrom( const ctrl_key& k0, const ctrl_key& k1, const ctrl_key& k2 )
				{ cardinal( 0.0, k0, k1, k2 ); }

			void cardinal( float_t c, const ctrl_key& k0, const ctrl_key& k1, const ctrl_key& k2 );
			void cardinal_alt( float_t c, const ctrl_key& k0, const ctrl_key& k1, const ctrl_key& k2 );

			void kochbartels(
				float_t tension, float_t bias, float_t continuity,
				const ctrl_key& k0, const ctrl_key& k1, const ctrl_key& k2
			);
	};

  // spline utilities
	// TODO: ctrl_key lerp()
	// float_t ssvvcc_warp()
	// float_t ssvvcc_patch()
	// float_t ssvvcc_wpatch()

	float_t ssvvcc_extend( float_t f0, float_t f1, float_t f2 );
	ctrl_key ssvvcc_extend( 
		float_t t0, float_t f0, 
		float_t t1, float_t f1, 
		float_t t2, float_t f2 
	);
	ctrl_key ssvvcc_extend( 
		const ctrl_key& k0, const ctrl_key& k1, const ctrl_key& k2 
	);

	float_t bezier( float_t s, float_t f0, float_t f1, float_t f2, float_t f3 );
	float_t hermite( float_t s, float_t v1, float_t v2, float_t m1, float_t m2 );
	float_t hermite( float_t t, const cardinal_key& K1, const cardinal_key& K2 );
	float_t hermite( float_t t, const tempordinal_key& K1, const tempordinal_key& K2 );

////////////////////////////////

	// namespace gwiz{} continued:
class vector_t {

    public:
#if 1
		void print( void ) const { printf( " vector_t:\n  %f %f %f\n", X, Y, Z ); }
#else
		void print( void ) const { printf( " vector_t:\n  %.12f %.12f %.12f\n", X, Y, Z ); }
#endif

	// CONSTRUCT
        inline vector_t( void )
            { X = 0.0; Y = 0.0; Z = 0.0; }
        inline vector_t( float_t x_in, float_t y_in, float_t z_in )
            { X = x_in; Y = y_in; Z = z_in; }
        inline vector_t( const vector_t & v_in )
            { X = v_in.x(); Y = v_in.y(); Z = v_in.z(); }

	// WRITE
		inline void x( float_t x_in ) { X = x_in; }
		inline void y( float_t y_in ) { Y = y_in; }
		inline void z( float_t z_in ) { Z = z_in; }

	// READ
		inline float_t x( void ) const { return( X ); }
		inline float_t y( void ) const { return( Y ); }
		inline float_t z( void ) const { return( Z ); }

	// MISC
        inline float_t sqlen( void ) const 
			{ return( X*X + Y*Y + Z*Z ); }
        inline float_t length( void ) const 
			{ return( sqrt( sqlen() ) ); }
		inline float_t dot( const vector_t& v ) const 
			{ return( X*v.X + Y*v.Y + Z*v.Z ); }

		inline vector_t cross( const vector_t& v ) const 
			{ return( vector_t( Y*v.Z - Z*v.Y, Z*v.X - X*v.Z, X*v.Y - Y*v.X ) ); }
		inline vector_t& cross( const vector_t& v0, const vector_t& v1 ) 
			{ return( (*this) = v0.cross( v1 ) ); }

		inline float_t box( const vector_t& v1, const vector_t& v2 ) const 
			{ return( dot( v1.cross( v2 ) ) ); }
		inline float_t box( const vector_t& v1, const vector_t& v2, const vector_t& v3 ) const 
			{ return( ( v1 - *this ).dot( ( v2 - *this ).cross( v3 - *this ) ) ); }

        vector_t normal( void ) const;
        vector_t& normalize( void );

		inline vector_t lerp( float_t s, const vector_t& v ) const 
#if 0
			{ return( (*this) * ( 1.0 - s ) + v * s ); }
#else
			{ return( (*this) + ( v - (*this) ) * s ); }
#endif
		inline vector_t& lerp( float_t s, const vector_t& v0, const vector_t& v1 ) 
			{ return( (*this) = v0.lerp( s, v1 ) ); }
		
	// OPERATE
        inline vector_t operator - ( void ) const 
            { return( vector_t( -x(), -y(), -z() ) ); }

        inline vector_t operator * ( float_t s ) const 
			{ return( vector_t( X*s, Y*s, Z*s ) ); }
        inline vector_t operator / ( float_t d ) const 
			{ float_t inv = 1.0 / d; return( (*this) * inv ); }
        inline vector_t& operator *= ( float_t s ) 
			{ return( (*this) = (*this) * s ); }
        inline vector_t& operator /= ( float_t d ) 
			{ return( (*this) = (*this) / d ); }

        inline vector_t operator + ( const vector_t& v ) const 
			{ return( vector_t( X + v.X, Y + v.Y, Z + v.Z ) ); }
        inline vector_t operator - ( const vector_t& v ) const 
			{ return( vector_t( X - v.X, Y - v.Y, Z - v.Z ) ); }
        inline vector_t operator * ( const vector_t& v ) const 
			{ return( vector_t( X * v.X, Y * v.Y, Z * v.Z ) ); }
        inline vector_t operator / ( const vector_t& v ) const 
			{ return( vector_t( X / v.X, Y / v.Y, Z / v.Z ) ); }

        inline vector_t& operator += ( const vector_t& v ) 
			{ return( (*this) = (*this) + v ); }
        inline vector_t& operator -= ( const vector_t& v ) 
			{ return( (*this) = (*this) - v ); }
        inline vector_t& operator *= ( const vector_t& v ) 
			{ return( (*this) = (*this) * v ); }
        inline vector_t& operator /= ( const vector_t& v ) 
			{ return( (*this) = (*this) / v ); }

    private:
        float_t X, Y, Z;
};

class vector4_t: public vector_t {

    public:
#if 1
		void print( void ) const { printf( " vector4_t:\n  %f %f %f %f\n", x(), y(), z(), W ); }
#else
		void print( void ) const { printf( " vector4_t:\n  %.12f %.12f %.12f %.12f\n", x(), y(), z(), W ); }
#endif

	// CONSTRUCT
        inline vector4_t( void ): vector_t()
            { W = 0.0; }
        inline vector4_t( float_t x_in, float_t y_in, float_t z_in, float_t w_in )
			: vector_t( x_in, y_in, z_in )
            { W = w_in; }
        inline vector4_t( const vector_t & v_in, float_t w_in )
			: vector_t( v_in )
            { W = w_in; }

	// WRITE
		inline void w( float_t w_in ) { W = w_in; }

	// READ
		inline float_t w( void ) const { return( W ); }

	// MISC
        inline float_t sqlen( void ) const 
			{ return( vector_t::sqlen() + W*W ); }
        inline float_t length( void ) const 
			{ return( sqrt( sqlen() ) ); }

	// OPERATE: not implemented

    private:
        float_t W;
};

////////////////////////////////

class quat_t {

    public:
#if 1
		void print( void ) const { printf( " quat_t:\n  %f %f %f %f\n", W, X, Y, Z ); }
#else
		void print( void ) const { printf( " quat_t:\n  %.12f %.12f %.12f %.12f\n", W, X, Y, Z ); }
#endif

	// CONSTRUCT
        SBAPI quat_t( void )
            { W = 1.0; X = 0.0; Y = 0.0; Z = 0.0; }
        SBAPI quat_t( float w_in, float x_in, float y_in, float z_in )
            { set( w_in, x_in, y_in, z_in ); }
        SBAPI quat_t( float_t w_in, float_t x_in, float_t y_in, float_t z_in )
            { set( (float_t)w_in, (float_t)x_in, (float_t)y_in, (float_t)z_in ); }
		SBAPI quat_t( const vector_t& axis_angle );
		
		SBAPI quat_t( float_t angle, const vector_t& v, int use_radians = 0 );
		SBAPI quat_t( float_t swing_x, float_t swing_y, float_t twist, int use_radians = 0 );
        SBAPI quat_t( const vector_t& z_axis, const vector_t& y_axis_approx );

	// CONVERT CONSTRUCT
		SBAPI quat_t( const euler_t& e );

	// WRITE
		inline void set( float_t w_in, float_t x_in, float_t y_in, float_t z_in ) 
			{ W = w_in; X = x_in; Y = y_in; Z = z_in; normalize(); }
		inline void set( float w_in, float x_in, float y_in, float z_in ) 
			{ set( (float_t)w_in, (float_t)x_in, (float_t)y_in, (float_t)z_in ); }

	// READ
        inline float_t w( void ) const { return( W ); }
        inline float_t x( void ) const { return( X ); }
        inline float_t y( void ) const { return( Y ); }
        inline float_t z( void ) const { return( Z ); }
        inline float wf( void ) const { return( (float)W ); }
        inline float xf( void ) const { return( (float)X ); }
        inline float yf( void ) const { return( (float)Y ); }
        inline float zf( void ) const { return( (float)Z ); }

		inline vector_t axisangle( void ) const // Axis-Angle: Same as Exponential Map
			{ return( axis() * radians() ); }
		inline float_t radians( void ) const
			{ return( 2.0 * gwiz::safe_acos( W ) ); }
		inline float_t degrees( void ) const 
			{ return( DEG( radians() ) ); }
		inline vector_t axis( void ) const 
			{ return( vector_t( X, Y, Z ).normal() ); }
		vector_t swingtwist( int use_radians = 0 ) const;
		
	// MISC
		inline float_t dot( const quat_t& q ) const 
			{ return( W*q.W + X*q.X + Y*q.Y + Z*q.Z ); }

		inline quat_t reflection(void) const 
			{ return( quat_t( -W, X, Y, Z ) ); } // reverse
		inline quat_t conjugate(void) const 
			{ return( quat_t( W, -X, -Y, -Z ) ); } // inverse
		inline quat_t complement(void) const 
			{ return( quat_t( -W, -X, -Y, -Z ) ); }

		inline quat_t shortest( void ) const 
			{ if( W < 0.0 ) return( complement() ); return( *this ); }
		inline quat_t shortest( const quat_t& q_ref ) const 
			{ if( dot( q_ref ) < 0.0 ) return( complement() ); return( *this ); }

		SBAPI quat_t& normalize( void );

		quat_t lerp( float_t s, const quat_t& q ) const;
		inline quat_t& lerp( float_t s, const quat_t& q0, const quat_t& q1 ) 
			{ return( (*this) = q0.lerp( s, q1 ) ); }

	// OPERATE
		inline quat_t operator - ( void ) const 
			{ return( conjugate() ); }
		inline quat_t operator ~ ( void ) const 
			{ return( complement() ); }

		inline quat_t operator * ( float_t s ) const 
			{ quat_t tmp; return( tmp.lerp( s, *this ) ); }
		inline quat_t operator / ( float_t d ) const 
			{ float_t inv = 1.0 / d; return( (*this) * inv ); }

		inline quat_t& operator *= ( float_t s ) 
			{ return( (*this) = (*this) * s ); }
		inline quat_t& operator /= ( float_t d ) 
			{ return( (*this) = (*this) / d ); }

		inline quat_t& operator *= ( quat_t q ) 
			{ return( (*this) = (*this) * q ); }

	// PIPELINE
		/*
			http://www.cs.hmc.edu/courses/2006/spring/cs157/lectures/orientation2.ppt#307,17,Shorthand
				q=<s,v>
				q'=<s',v'>
				.: dot product
				x: cross product
				q*q' = <ss'-v.v', sv' + s'v + vxv'>
		*/

		inline quat_t product( const quat_t& q ) const { 
			// http://en.wikipedia.org/wiki/Quaternion
			/*
			 P: { a, b, c, d }
			 Q: { t, x, y, z }
			 P*Q == {( at - bx  - cy - dz ),           ( bt + ax + cz - dy ),            ( ct + ay + dx - bz ),            ( dt + az + by - cx )}
			 P*Q == {( W*q.W - X*q.X - Y*q.Y - Z*q.Z ),( X*q.W + W*q.X + Y*q.Z - Z*q.Y ),( Y*q.W + W*q.Y + Z*q.X - X*q.Z ),( Z*q.W + W*q.Z + X*q.Y - Y*q.X )}
			quat_t(
				W*q.W - X*q.X - Y*q.Y - Z*q.Z,
				X*q.W + W*q.X + Y*q.Z - Z*q.Y,
				Y*q.W + W*q.Y + Z*q.X - X*q.Z,
				Z*q.W + W*q.Z + X*q.Y - Y*q.X
			)
			*/
			// return( this * q );
			return(
				quat_t(
					W*q.W - X*q.X - Y*q.Y - Z*q.Z, 
					X*q.W + W*q.X - Z*q.Y + Y*q.Z, 
					Y*q.W + Z*q.X + W*q.Y - X*q.Z, 
					Z*q.W - Y*q.X + X*q.Y + W*q.Z
				)
			);
		}

		inline quat_t product_left( const quat_t& q ) const { 
			// Shoemake
			// http://answers.google.com/answers/threadview?id=596035
			// http://www.gamedev.net/reference/articles/article1997.asp
			// return( q * this );
			return(
				quat_t(
					W*q.W - X*q.X - Y*q.Y - Z*q.Z, 
					X*q.W + W*q.X + Z*q.Y - Y*q.Z, 
					Y*q.W - Z*q.X + W*q.Y + X*q.Z, 
					Z*q.W + Y*q.X - X*q.Y + W*q.Z
				)
				/*
				rearranged:
				quat_t(
					+ W*q.W - X*q.X - Y*q.Y - Z*q.Z, 
					+ W*q.X + X*q.W - Y*q.Z + Z*q.Y , 
					+ W*q.Y + X*q.Z + Y*q.W - Z*q.X , 
					+ W*q.Z - X*q.Y + Y*q.X + Z*q.W 
				)
				*/
			);
		}

		inline quat_t operator * ( const quat_t& q ) const { 
			return( product( q ) );
		}
		matrix_t operator * ( const matrix_t& R ) const;

	// TRANSFORM
		vector_t operator * ( const vector_t& v ) const;
		
    private:
        float_t W, X, Y, Z;
};

class euler_t {

	// OGL standard:
	//   axes: { X, Y, Z } == { right, up, back } == { right-hand }
	// 6DOF tracking hardware standard:
	//   packing: { pitch, heading, roll } == { X, Y, Z }
	//   concatenation: [ Heading * Pitch * Roll ] == [ Y * X * Z ]
	
    public:
#if 1
		void print( void ) const { printf( " euler_t:\n  %f %f %f\n", X, Y, Z ); }
#else
		void print( void ) const { printf( " euler_t:\n  %.12f %.12f %.12f\n", X, Y, Z ); }
#endif

	// CONSTRUCT
        SBAPI euler_t( void )
            { X = 0.0; Y = 0.0; Z = 0.0; }
        SBAPI euler_t( float_t x_in, float_t y_in, float_t z_in )
            { X = x_in; Y = y_in; Z = z_in; }
        SBAPI euler_t( const vector_t& z_axis, float_t roll_deg ); // NOTE: flipped lookat
        SBAPI euler_t( const vector_t& z_axis, const vector_t& y_axis_approx ); // NOTE: flipped lookat

	// CONVERT CONSTRUCT
		SBAPI euler_t( const quat_t& q );

	// WRITE
		inline void x( float_t x_in ) { X = x_in; }
		inline void y( float_t y_in ) { Y = y_in; }
		inline void z( float_t z_in ) { Z = z_in; }

		inline void p( float_t p_in ) { X = p_in; }
		inline void h( float_t h_in ) { Y = h_in; }
		inline void r( float_t r_in ) { Z = r_in; }
		
		inline euler_t& lookat( const vector_t& dir, float_t roll_deg )
			{ return( (*this) = euler_t( -dir, roll_deg ) ); } // NOTE: flipped z-axis
		inline euler_t& lookat( const vector_t& dir, const vector_t& up_approx )
			{ return( (*this) = euler_t( -dir, up_approx ) ); } // NOTE: flipped z-axis

	// READ
		inline float_t x( void ) const { return( X ); }
		inline float_t y( void ) const { return( Y ); }
		inline float_t z( void ) const { return( Z ); }

		inline float_t p( void ) const { return( X ); }
		inline float_t h( void ) const { return( Y ); }
		inline float_t r( void ) const { return( Z ); }

	// MISC
		inline euler_t lerp( float_t s, const euler_t& e ) const 
			{ return( quat_t(*this).lerp( s, e ) ); }
		inline euler_t& lerp( float_t s, const euler_t& e0, const euler_t& e1 ) 
			{ return( (*this) = e0.lerp( s, e1 ) ); }

	// OPERATE
		euler_t operator - ( void ) const;
		
		inline euler_t operator * ( float_t s ) const 
			{ return( quat_t(*this) * s ); }
		inline euler_t operator / ( float_t d ) const 
			{ float_t inv = 1.0 / d; return( (*this) * inv ); }

		inline euler_t& operator *= ( float_t s ) 
			{ return( (*this) = (*this) * s ); }
		inline euler_t& operator /= ( float_t d ) 
			{ return( (*this) = (*this) / d ); }

		inline euler_t& operator *= ( euler_t e ) 
			{ return( (*this) = (*this) * e ); }

	// PIPELINE
		matrix_t operator * ( const matrix_t& R ) const;
		inline euler_t operator * ( const euler_t& r ) const
			{ return( quat_t(*this) * quat_t( r ) ); }

	// TRANSFORM
		vector_t operator * ( const vector_t& v ) const;
		
    private:
        float_t X, Y, Z;
};

////////////////////////////////

class matrix_t {
	
	// OGL standard:
	//   col major packing: [ col-0 ][ col-1 ][ col-2 ][ col-3 ]
	//   access: M[ col ][ row ] == M.get( col, row )
	
    public:
		void print( char *label = 0x0 ) const {
			if( label )
				printf( " matrix_t: %s\n", label );
			else
				printf( " matrix_t:\n" );
			for( int i=0;i<4;i++ )
				printf( "  %f %f %f %f\n", M[0][i], M[1][i], M[2][i], M[3][i] );
			// what about the stack?
		}

	// CONSTRUCT
        inline matrix_t( void ) { stack = 0x0; }
        inline matrix_t( int init ) { stack = 0x0; if( init ) identity(); }
        inline matrix_t( const float_t M_array[ 4 ][ 4 ] ) { stack = 0x0; set( M_array ); }
        inline matrix_t( const vector_t& x, const vector_t& y, const vector_t& z )
			{ stack = 0x0; lineator( x, y, z ); }
	
	// COPY CONSTRUCT
		inline matrix_t( const matrix_t& M_set ) { stack = 0x0; set( M_set.M ); }

	// COPY ASSIGN
		inline matrix_t& operator = ( const matrix_t& M_set ) { 
			if( this != &M_set ) return( set( M_set.M ) ); 
			return( *this );
		}

	// CONVERT CONSTRUCT
		matrix_t( const quat_t& q );
		matrix_t( const euler_t& e );
	
	// DESTRUCT
//		inline virtual ~matrix_t( void ) { while( stack ) pop(); }
		~matrix_t( void ) { while( stack ) pop(); }

	// WRITE
		inline matrix_t& set( int c, int r, float_t f ) 
			{ M[ c ][ r ] = f; return( *this ); }

		inline matrix_t& col( int i, float_t a, float_t b, float_t c, float_t d ) 
			{ M[i][0] = a; M[i][1] = b; M[i][2] = c; M[i][3] = d; return( *this ); }
		inline matrix_t& row( int i, float_t a, float_t b, float_t c, float_t d ) 
			{ M[0][i] = a; M[1][i] = b; M[2][i] = c; M[3][i] = d; return( *this ); }

		inline matrix_t& col( int i, const vector_t& v, float_t f ) 
			{ M[i][0] = v.x(); M[i][1] = v.y(); M[i][2] = v.z(); M[i][3] = f; return( *this ); }
		inline matrix_t& row( int i, const vector_t& v, float_t f ) 
			{ M[0][i] = v.x(); M[1][i] = v.y(); M[2][i] = v.z(); M[3][i] = f; return( *this ); }

		inline matrix_t& col( int i, const vector4_t& v ) 
			{ M[i][0] = v.x(); M[i][1] = v.y(); M[i][2] = v.z(); M[i][3] = v.w(); return( *this ); }
		inline matrix_t& row( int i, const vector4_t& v ) 
			{ M[0][i] = v.x(); M[1][i] = v.y(); M[2][i] = v.z(); M[3][i] = v.w(); return( *this ); }

		inline matrix_t& set( const float_t M_array[ 4 ][ 4 ] ) { 
			for( int i=0;i<4;i++ )	{
				M[i][0] = M_array[i][0]; 
				M[i][1] = M_array[i][1]; 
				M[i][2] = M_array[i][2]; 
				M[i][3] = M_array[i][3];
			}
			return( *this );
		}
		inline matrix_t& set_col_major( const float_t M_array[ 16 ] ) { 
			for( int i=0;i<4;i++ )	{
				M[i][0] = M_array[ 4 * i ];
				M[i][1] = M_array[ 4 * i + 1 ];
				M[i][2] = M_array[ 4 * i + 2 ];
				M[i][3] = M_array[ 4 * i + 3 ];
			}
			return( *this ); 
		}
		inline matrix_t& set_row_major( const float_t M_array[ 16 ] ) { 
			for( int i=0;i<4;i++ )	{
				M[0][i] = M_array[ 4 * i ];
				M[1][i] = M_array[ 4 * i + 1 ];
				M[2][i] = M_array[ 4 * i + 2 ];
				M[3][i] = M_array[ 4 * i + 3 ];
			}
			return( *this ); 
		}

	// READ
		inline float_t get( int c, int r ) const
			{ return( M[ c ][ r ] ); }
		inline vector4_t col( int i ) const 
			{ return( vector4_t( M[i][0], M[i][1], M[i][2], M[i][3] ) ); }
		inline vector4_t row( int i ) const 
			{ return( vector4_t( M[0][i], M[1][i], M[2][i], M[3][i] ) ); }
		inline matrix_t get( float_t M_array[ 4 ][ 4 ] ) const { 
			for( int i=0;i<4;i++ )	{
				M_array[i][0] = M[i][0]; 
				M_array[i][1] = M[i][1]; 
				M_array[i][2] = M[i][2]; 
				M_array[i][3] = M[i][3]; 
			}
			return( *this );
		}
		inline matrix_t get_col_major( float_t M_array[ 16 ] ) const { 
			for( int i=0;i<4;i++ )	{
				M_array[ 4 * i ] = M[i][0];
				M_array[ 4 * i + 1 ] = M[i][1];
				M_array[ 4 * i + 2 ] = M[i][2];
				M_array[ 4 * i + 3 ] = M[i][3];
			}
			return( *this ); 
		}
		inline matrix_t get_row_major( float_t M_array[ 16 ] ) const { 
			for( int i=0;i<4;i++ )	{
				M_array[ 4 * i ] = M[0][i];
				M_array[ 4 * i + 1 ] = M[1][i];
				M_array[ 4 * i + 2 ] = M[2][i];
				M_array[ 4 * i + 3 ] = M[3][i];
			}
			return( *this ); 
		}

	// MISC
        inline matrix_t& identity( void ) { 
			for(int i=0;i<4;i++) 
				for(int j=0;j<4;j++) 
					M[ i ][ j ] = ( ( i==j ) ? 1.0 : 0.0 ); 
			return( *this );
		}
        inline matrix_t& id( void ) { return( identity() ); }
        inline matrix_t transposition( void ) const { 
			matrix_t tmp;
			for(int i=0;i<4;i++) 
				for(int j=0;j<4;j++) 
					tmp.M[ i ][ j ] = M[ j ][ i ];
			return( tmp );
		}
        inline matrix_t& transpose( void ) { 
			float_t tmp;
			tmp = M[0][1]; M[0][1] = M[1][0]; M[1][0] = tmp;
			tmp = M[0][2]; M[0][2] = M[2][0]; M[2][0] = tmp;
			tmp = M[0][3]; M[0][3] = M[3][0]; M[3][0] = tmp;
			tmp = M[1][2]; M[1][2] = M[2][1]; M[2][1] = tmp;
			tmp = M[1][3]; M[1][3] = M[3][1]; M[3][1] = tmp;
			tmp = M[2][3]; M[2][3] = M[3][2]; M[3][2] = tmp;
			return( *this );
		}
        matrix_t inverse( void ) const;
        matrix_t& invert( void );

	// ASSIGN TRANSFORMS
		matrix_t& frustum( float_t l, float_t r, float_t b, float_t t, float_t n, float_t f );
		matrix_t& frustum( const vector_t& L_B_N, const vector_t& R_T_F );
		matrix_t& frustum( const vector_t& B_L_dir, const vector_t& R_T_dir, float_t near, float_t far );
		matrix_t& frustum( float_t fovy, float_t aspect, float_t near, float_t far );

		matrix_t& translator( const vector_t& t );
		matrix_t& rotator( const quat_t& q );
		matrix_t& rotator( const euler_t& e );
		matrix_t& shearer( const vector_t& s );
		matrix_t& scaler( const vector_t& s );
		matrix_t& lineator( const vector_t& x, const vector_t& y, const vector_t& z );

		inline matrix_t& translator( float_t x, float_t y, float_t z )
			{ return( translator( vector_t( x, y, z ) ) ); }
		inline matrix_t& rotator( float_t w, float_t x, float_t y, float_t z )
			{ return( rotator( quat_t( w, x, y, z ) ) ); }
		inline matrix_t& rotator( float_t angle, const vector_t& v, int use_radians = 0 )
			{ return( rotator( quat_t( angle, v, use_radians ) ) ); }
		inline matrix_t& rotator( float_t p, float_t h, float_t r )
			{ return( rotator( euler_t( p, h, r ) ) ); }
		inline matrix_t& shearer( float_t xy, float_t xz, float_t yz )
			{ return( shearer( vector_t( xy, xz, yz ) ) ); }
		inline matrix_t& scaler( float_t x, float_t y, float_t z )
			{ return( scaler( vector_t( x, y, z ) ) ); }
		
	// OPERATE
		inline matrix_t operator - ( void ) const { return( inverse() ); }

	// PIPELINE
		inline matrix_t operator * ( const matrix_t& R ) const { 
			matrix_t tmp;
			for( int j=0; j<4; j++ )
				for( int i=0; i<4; i++ )
					tmp.M[ j ][ i ] = 
						M[ 0 ][ i ] * R.M[ j ][ 0 ] + 
						M[ 1 ][ i ] * R.M[ j ][ 1 ] + 
						M[ 2 ][ i ] * R.M[ j ][ 2 ] + 
						M[ 3 ][ i ] * R.M[ j ][ 3 ] ;
			return( tmp );
		}
		inline matrix_t& operator *= ( const matrix_t& R ) 
			{ return( (*this) = (*this) * R ); }
		
		inline matrix_t& translate( const vector_t& t )	
			{ matrix_t tmp; return( (*this) *= tmp.translator( t ) ); }
		inline matrix_t& rotate( const quat_t& q )	
			{ matrix_t tmp; return( (*this) *= tmp.rotator( q ) ); }
		inline matrix_t& rotate( const euler_t& e )	
			{ matrix_t tmp; return( (*this) *= tmp.rotator( e ) ); }
		inline matrix_t& shear( const vector_t& s )	
			{ matrix_t tmp; return( (*this) *= tmp.shearer( s ) ); }
		inline matrix_t& scale( const vector_t& s )	
			{ matrix_t tmp; return( (*this) *= tmp.scaler( s ) ); }
		inline matrix_t& lineate( const vector_t& x, const vector_t& y, const vector_t& z )	
			{ matrix_t tmp; return( (*this) *= tmp.lineator( x, y, z ) ); }

		inline matrix_t& translate( float_t x, float_t y, float_t z )
			{ return( translate( vector_t( x, y, z ) ) ); }
		inline matrix_t& rotate( float_t w, float_t x, float_t y, float_t z )
			{ return( rotate( quat_t( w, x, y, z ) ) ); }
		inline matrix_t& rotate( float_t angle, const vector_t& v, int use_radians = 0 )
			{ return( rotate( quat_t( angle, v, use_radians ) ) ); }
		inline matrix_t& rotate( float_t p, float_t h, float_t r )
			{ return( rotate( euler_t( p, h, r ) ) ); }
		inline matrix_t& shear( float_t xy, float_t xz, float_t yz )
			{ return( shear( vector_t( xy, xz, yz ) ) ); }
		inline matrix_t& scale( float_t x, float_t y, float_t z )
			{ return( scale( vector_t( x, y, z ) ) ); }
		
	// EXTRACT
		int decompose( 
			vector4_t*	Perspective, 
			vector_t*	Translation, 
			quat_t*		Quaternion, 
			euler_t*	Euler, 
			vector_t*	Shearing, 
			vector_t*	Scaling, 
			int composition_flag = gwiz::COMP_UNKNOWN
		) const;

		vector4_t	perspective	( int composition_flag = gwiz::COMP_UNKNOWN ) const;
		vector_t	translation	( int composition_flag = gwiz::COMP_UNKNOWN ) const;
		quat_t		quat		( int composition_flag = gwiz::COMP_UNKNOWN ) const;
		euler_t		euler		( int composition_flag = gwiz::COMP_UNKNOWN ) const;
		vector_t	shearing	( int composition_flag = gwiz::COMP_UNKNOWN ) const;
		vector_t	scaling		( int composition_flag = gwiz::COMP_UNKNOWN ) const;

	// VECTOR TRANSFORM
		inline vector_t operator * ( const vector_t& v ) const { 
			return( 
				vector_t(
					M[ 0 ][ 0 ] * v.x() + M[ 1 ][ 0 ] * v.y() + M[ 2 ][ 0 ] * v.z() + M[ 3 ][ 0 ], 
					M[ 0 ][ 1 ] * v.x() + M[ 1 ][ 1 ] * v.y() + M[ 2 ][ 1 ] * v.z() + M[ 3 ][ 1 ], 
					M[ 0 ][ 2 ] * v.x() + M[ 1 ][ 2 ] * v.y() + M[ 2 ][ 2 ] * v.z() + M[ 3 ][ 2 ]
				) 
			); 
		}
		inline vector4_t operator * ( const vector4_t& v ) const { 
			return( 
				vector4_t(
					M[ 0 ][ 0 ] * v.x() + M[ 1 ][ 0 ] * v.y() + M[ 2 ][ 0 ] * v.z() + M[ 3 ][ 0 ] * v.w(), 
					M[ 0 ][ 1 ] * v.x() + M[ 1 ][ 1 ] * v.y() + M[ 2 ][ 1 ] * v.z() + M[ 3 ][ 1 ] * v.w(), 
					M[ 0 ][ 2 ] * v.x() + M[ 1 ][ 2 ] * v.y() + M[ 2 ][ 2 ] * v.z() + M[ 3 ][ 2 ] * v.w(), 
					M[ 0 ][ 3 ] * v.x() + M[ 1 ][ 3 ] * v.y() + M[ 2 ][ 3 ] * v.z() + M[ 3 ][ 3 ] * v.w()
				) 
			); 
		}

	// STACK
		inline matrix_t& push(void)	{
			if( matrix_t *tmp = new matrix_t( *this ) )	{
				tmp->stack = stack;
				stack = tmp;
			}
			else 
				printf( "matrix_t::push ERR: new FAILED\n" );
			return( *this );
		}
		inline matrix_t& pop( matrix_t *M_get = 0x0 )	{
			if( stack )	{
				if( M_get )	
					*M_get = *this;
				matrix_t *tmp = stack;
				*this = *stack;
				stack = tmp->stack;
				tmp->stack = 0x0;
				delete tmp;
			}
			else 
				printf( "matrix_t::pop ERR: stack exceeded\n" );
			return( *this );
		}

    private:
        float_t M[ 4 ][ 4 ];
        matrix_t *stack;
};

} // namespace gwiz

////////////////////////////////
#endif //GWIZ_MATH_H

