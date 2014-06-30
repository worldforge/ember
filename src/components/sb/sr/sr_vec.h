/*
 *  sr_vec.h - part of Motion Engine and SmartBody-lib
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
 *      Marcelo Kallmann, USC (currently UC Merced)
 */

/** \file sr_vec.h 
 * Three dimensional vector */

# ifndef SR_VEC_H
# define SR_VEC_H

#include <sb/SBTypes.h>
# include <sr/sr_input.h> 
# include <sr/sr_output.h> 
#include <string>

class SrVec;
class SrVec2;

SBAPI float dist2 ( const SrVec& v1, const SrVec& v2 );
SBAPI float angle ( const SrVec& v1, const SrVec& v2 ); 

/*! This "point typedef" is used mainly to coherently describe variables,
    making possible to distiguish points from vectors parameters. */
typedef SrVec SrPnt;

/*! \class SrVec sr_vec.h
    \brief Three dimensional vector. 

    Its three coordinates are of type float, and can be accessed with the 
    provided methods or as a float array, to be compatible with other libraries.
    Access as a float array is done with an overloaded typecast, that should
    correctly work with most compilers. */
class SBAPI SrVec
 { public :
    float x, y, z;
   public :
    static const SrVec null;     //!< (0,0,0) null vector
    static const SrVec one;      //!< (1,1,1) vector
    static const SrVec minusone; //!< (-1,-1,-1) vector
    static const SrVec i;        //!< (1,0,0) vector
    static const SrVec j;        //!< (0,1,0) vector
    static const SrVec k;        //!< (0,0,1) vector
   public :

    /*! Initializes SrVec as a null vector. Implemented inline. */
    SrVec () : x(0), y(0), z(0) {}

    /*! Initializes SrVec from an int pointer. Implemented inline. */
    SrVec ( const int *v ) : x((float)v[0]), y((float)v[1]), z((float)v[2]) {}

    /*! Initializes from a float pointer. Implemented inline. */
    SrVec ( const float *v ) : x((float)v[0]), y((float)v[1]), z((float)v[2]) {}

    /*! Initializes from a double pointer. Implemented inline. */
    SrVec ( const double *v ) : x((float)v[0]), y((float)v[1]), z((float)v[2]) {}

    /*! Copy constructor. Implemented inline. */
    SrVec ( const SrVec& v ) : x(v.x), y(v.y), z(v.z) {}

    /*! Initializes from a 2d vector: the z coordinate is set to zero. */
    SrVec ( const SrVec2& v );

    /*! Initializes with the three given float coordinates. Implemented inline. */
    SrVec ( float a, float b, float c ) : x(a), y(b), z(c) {}

    /*! Sets coordinates from an int pointer. Implemented inline. */
    void set ( const int *v ) { x=(float)v[0]; y=(float)v[1]; z=(float)v[2]; }

    /*! Sets coordinates from a float pointer. Implemented inline. */
    void set ( const float *v ) { x=(float)v[0]; y=(float)v[1]; z=(float)v[2]; }
    
    /*! Sets coordinates from a double pointer. Implemented inline. */
    void set ( const double *v ) { x=(float)v[0]; y=(float)v[1]; z=(float)v[2]; }

    /*! Sets coordinates from the three given float values. Implemented inline. */
    void set ( float a, float b, float c ) { x=a; y=b; z=c; }

    /*! Sets coordinates from another SrVec. Implemented inline. */
    void set ( const SrVec& v ) { x=v.x; y=v.y; z=v.z; }

    /*! Sets coordinates from a 2d vector: the z coordinate is set to zero. */
    void set ( const SrVec2& v );

    /*! Put in f the coordinate values. */
    void get ( float f[3] ) const { f[0]=x; f[1]=y; f[2]=z; }

    /*! Apply a rotation to SrVec around the X axis with
        the angle 'a' where sa=sin(a) and ca=cos(a). */
    void rotx ( float sa, float ca );

    /*! Apply a rotation to SrVec around the Y axis with 
        the angle 'a' where sa=sin(a) and ca=cos(a). */
    void roty ( float sa, float ca );

    /*! Apply a rotation to SrVec around the Z axis with
        the angle 'a' where sa=sin(a) and ca=cos(a). */
    void rotz ( float sa, float ca );

    /*! Same as the other rotx() method, but with one parameter
       angle in radians. */
    void rotx ( float radians );

    /*! Same as the other roty() method, but with one parameter
       angle in radians. */
    void roty ( float radians );
	void rotY ( float radians ) { roty(radians); } // for python interface

    /*! Same as the other rotz() method, but with one parameter
       angle in radians. */
    void rotz ( float radians );

    /*! Makes each coordinate to have its absolute value. */
    void abs ();

    /*! Normalizes the vector. If its norm is 0, the vector is not changed. */
    void normalize ();
    
    /*! Sets the length to be n and returns the previous length */
    float len ( float n );

    /*! Returns the length of the vector. Equivalent to norm() */
    float len () { return norm(); }

    /*! Returns the norm (distance to zero) of the vector. If the vector is
        null, zero is returned. */
    float norm () const;

    /*! Returns the square of the norm (x*x + y*y + z*z). */
    float norm2 () const;
    
    /*! Returns the norm, but using the max function of distance, which does not require sqrt() */
    float norm_max () const;

    /*! Returns a 2d orthogonal vector to the 2d vector(x,y) that is (-y,x)
        The z coordinate of SrVec is not considered. Implemented inline. */
    SrVec ortho2d () const { return SrVec(-y,x,0); }

    /*! Returns true if all coordinates are zero; false otherwise. */
    bool iszero () const { return x==0.0 && y==0.0 && z==0.0? true:false; }

    /*! Returns true if the vector is inside the closed neighboorhood of point 0,0,0 and radius ds. */
    bool nextzero ( float ds ) const { return norm2()<=ds*ds? true:false; }

    /*! Allows member access like a vector. */
    float& operator[] ( int i ) { return *((&x)+i); }

    /*! Convertion to a float pointer. */
    operator float* () const { return (float*)&x; }

    /*! Copy operator from another SrVec. Implemented inline. */
    void operator = ( const SrVec& v ) { set(v); }

    /*! Copy operator from a 2d vector: the z coordinate is set to zero. */
    void operator = ( const SrVec2& v ) { set(v); } 

    /*! Copy operator from an int pointer. Implemented inline. */
    void operator = ( const int *v ) { set(v); }
    
    /*! Copy operator from a float pointer. Implemented inline. */
    void operator = ( const float *v ) { set(v); }
    
    /*! Copy operator from a double pointer. Implemented inline. */
    void operator = ( const double *v ) { set(v); }

    /*! Operator-=() with another SrVec. Implemented inline. */
    void operator -= ( const SrVec& v ) { x-=v.x; y-=v.y; z-=v.z; }

    /*! Operator+=() with another SrVec. Implemented inline. */
    void operator += ( const SrVec& v ) { x+=v.x; y+=v.y; z+=v.z; }

    /*! Operator*=() with a float. Implemented inline. */
    void operator *= ( float r ) { x*=r; y*=r; z*=r; }

    /*! Operator/=() with a float. Implemented inline. */
    void operator /= ( float r ) { x/=r; y/=r; z/=r; }

    /*! Unary minus. Implemented inline. */
    SBAPI friend SrVec operator - ( const SrVec& v )
           { return SrVec ( -v.x, -v.y, -v.z ); }

    /*! Adds two vectors. Implemented inline. */
    SBAPI friend SrVec operator + ( const SrVec& v1, const SrVec& v2 ) 
           { return SrVec (v1.x+v2.x, v1.y+v2.y, v1.z+v2.z); }

    /*! Subtracts two vectors. Implemented inline. */
    SBAPI friend SrVec operator - ( const SrVec& v1, const SrVec& v2 ) 
           { return SrVec (v1.x-v2.x, v1.y-v2.y, v1.z-v2.z); }

    /*! Comparison operator. Implemented inline. */
    SBAPI friend bool operator == ( const SrVec& v1, const SrVec& v2 )
           { return v1.x==v2.x && v1.y==v2.y && v1.z==v2.z? true:false; }

    /*! Comparison operator. Implemented inline. */
    SBAPI friend bool operator != ( const SrVec& v1, const SrVec& v2 )
           { return v1.x!=v2.x || v1.y!=v2.y || v1.z!=v2.z? true:false; }

    /*! Pos multiplication by scalar. Implemented inline. */
    SBAPI friend SrVec operator * ( const SrVec& v, float r )
           { return SrVec (v.x*r, v.y*r, v.z*r); }

    /*! Pre multiplication by scalar. Implemented inline. */
    SBAPI friend SrVec operator * ( float r, const SrVec& v )
           { return SrVec (v.x*r, v.y*r, v.z*r); }

    /*! Division by scalar. Implemented inline. */
    SBAPI friend SrVec operator / ( const SrVec& v, float r )
           { return SrVec (v.x/r, v.y/r, v.z/r); }

    /*! Swaps the contents of v1 with v2. */
    SBAPI friend void swap ( SrVec& v1, SrVec& v2 );

    /*! Returns true if dist(v1,v2)<=ds, otherwise returns false. Implemented inline. */
    SBAPI friend bool next ( const SrVec& v1, const SrVec& v2, float ds )
           { return dist2(v1,v2)<=ds*ds? true:false; }

    /*! Returns the distance between v1 and v2 using norm_max(). */
    SBAPI friend float dist_max ( const SrVec& v1, const SrVec& v2 );

    /*! Returns the distance between v1 and v2. */
    SBAPI friend float dist ( const SrVec& v1, const SrVec& v2 );

    /*! Returns the square of the distance between v1 and v2. */
    SBAPI friend float dist2 ( const SrVec& v1, const SrVec& v2 );

    /*! Returns the angle between v1 and v2, that is inside the interval [0,pi],
        using acosf. The returned angle is in radians. */
    SBAPI friend float angle ( const SrVec& v1, const SrVec& v2 ); 
	float vecAngle(const SrVec& v1, const SrVec& v2);

    /*! Returns the angle between v1 and v2, but considering that
        v1 and v2 are already normalized, ie, computes acosf(dot(v1,v2)). */
    SBAPI friend float angle_fornormvecs ( const SrVec& v1, const SrVec& v2 ); 

    /*! Returns the cross product of v1 and v2. */
    SBAPI friend SrVec cross ( const SrVec& v1, const SrVec& v2 );

    /*! Inline version of cross(), but as a member. 
        Attention: v1 or v2 cannot be the same as SrVec. */
    void cross ( const SrVec& v1, const SrVec& v2 );

    /*! Returns the dot product of v1 and v2 (v1.x*v2.x + v1.y*v2.y + v1.z*v2.z).
        Its interpretation is the length of the projection of v1 onto v2
        ( v1.v2 = |v1||v2|cos(ang) ). */
    SBAPI friend float dot ( const SrVec& v1, const SrVec& v2 );

    /* Returns the normalized normal of triangle (a,b,c) */
    SBAPI friend SrVec triangle_normal ( const SrPnt& a, const SrPnt& b, const SrPnt& c );

    /*! Returns -1,0,1 depending if v1 is less, equal or greater than v2, 
        using a comparison by coordinates. Parameters are passed by reference. */
    SBAPI friend int compare ( const SrVec& v1, const SrVec& v2 );

    /*! Returns -1,0,1 depending if v1 is less, equal or greater than v2, 
        using a comparison by coordinates. Parameters are passed by pointers. */
    SBAPI friend int compare ( const SrVec* v1, const SrVec* v2 );

    /*! Returns the linear interpolation of v1 v2 in parameter t ( v1(1-t)+v2(t) ). */
    SBAPI friend SrVec lerp ( const SrVec& v1, const SrVec& v2, float t );

    /*! Outputs in format: "x y z". */
    SBAPI friend SrOutput& operator<< ( SrOutput& o, const SrVec& v );

    /*! Inputs from format: "x y z". */
    SBAPI friend SrInput& operator>> ( SrInput& in, SrVec& v );

	/*! Set the value specified by the index */
    void setData ( int index, float val ) { (*this)[index] = val;}

	/*! Returns the value specified by the index */
    float getData ( int index ) { return (*this)[index];}

	/*! Returns the Yaw angle value of given vector on X-Z plane, in radian, not normalized. - David Huang */
	float vecYaw(const SrVec& v);
	std::string toString();

 };

 SBAPI SrVec rotatePoint(SrVec point, SrVec origin, SrVec direction, float angle);


 class SBAPI SrVec4i 
 {
 public:
	 int data[4];
	 SrVec4i() {}
	 SrVec4i(int a, int b, int c, int d) { data[0] = a; data[1] = b; data[2] = c; data[3] = d; }
	 int& operator[] ( int i ) { return data[i]; }
	 void operator = ( const SrVec4i& v )
	 {
		 for (int i=0;i<4;i++)
			 data[i] = v.data[i];
	 }
 };

 class SrVec3i 
 {
 public:
	 int data[3];
	 SrVec3i() {}
	 SrVec3i(int a, int b, int c) { data[0] = a; data[1] = b; data[2] = c; }
	 int& operator[] ( int i ) { return data[i]; }
	 void operator = ( const SrVec4i& v )
	 {
		 for (int i=0;i<3;i++)
			 data[i] = v.data[i];
	 }
 };

 class SrVec4
 {
 public:
	 float data[4];
	 SrVec4() {}
	 SrVec4(float a, float b, float c, float d) { data[0] = a; data[1] = b; data[2] = c; data[3] = d; }
	 float& operator[] ( int i ) { return data[i]; }
	 void operator = ( const SrVec4& v )
	 {
		 for (int i=0;i<4;i++)
			 data[i] = v.data[i];
	 }
 };
//============================== end of file ===============================

# endif // SR_VEC_H
