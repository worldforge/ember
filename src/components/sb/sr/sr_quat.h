/*  sr_quat.h - part of Motion Engine and SmartBody-lib
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
 *  License along with SmarBody-lib.  If not, see:
 *      http://www.gnu.org/licenses/lgpl-3.0.txt
 *
 *  CONTRIBUTORS:
 *      Marcelo Kallmann, USC (currently UC Merced)
 */

/** \file sr_quat.h 
 * Quaternion for rotations */

# ifndef SR_QUAT_H
# define SR_QUAT_H

#include <sb/SBTypes.h>
# include <cmath>
# include <sr/sr_vec.h> 
# include <sr/sr_mat.h> 

/*! \class SrQuat sr_quat.h
    \brief Quaternion for rotations. 

    A quaternion is represented as a four dimensional vector. Here we keep
    one scalar float element w, and three floats for the imaginary vector.
    The quaternion is then: w + x i + y j + z k */
class SBAPI SrQuat
 { public :
    float w; // scalar part
    float x, y, z; // imaginary vector part

   private :
    float norm2 () const { return w*w+x*x+y*y+z*z; }
    float norm () const  { return sqrtf(norm2()); } 
    SrQuat conjugate() const { return SrQuat(w,-x,-y,-z); }
    void operator *= ( float r ) { w*=r; x*=r; y*=r; z*=r; }
    SrQuat operator * ( float r ) const { return SrQuat(w*r,x*r,y*r,z*r); }
    SrQuat operator / ( float r ) const { return SrQuat(w/r,x/r,y/r,z/r); }
    SrQuat operator + ( const SrQuat &q ) const { return SrQuat(w+q.w,x+q.x,y+q.y,z+q.z); }
    float dot ( const SrQuat &q ) const { return w*q.w + x*q.x + y*q.y + z*q.z; }

   public :

    /*! A null rotation that represents a rotation around the axis (1,0,0)
        with angle 0, that generates the internal representation (1,0,0,0). */
    static const SrQuat null;

   public :

    /*! Initializes SrQuat as a null rotation. Implemented inline. */
    SrQuat () : w(1.0f), x(0), y(0), z(0) {}

    /*! Constructor from 4 floats (qw,qx,qy,qz). Implemented inline. */
    SrQuat ( float qw, float qx, float qy, float qz ) : w(qw), x(qx), y(qy), z(qz) {}

    /*! Constructor from 4 floats (w,x,y,z) from a float array. Implemented inline. */
    SrQuat ( const float* f ) : w(f[0]), x(f[1]), y(f[2]), z(f[3]) {}

    /*! Copy constructor. Implemented inline. */
    SrQuat ( const SrQuat& q ) : w(q.w), x(q.x), y(q.y), z(q.z) {}

    /*! Initializes SrQuat with the rotation around the given axis and angle in
        radians. The method set() is called inline in this constructor. */
    SrQuat ( const SrVec& axis, float radians ) { set(axis,radians); }

    /*! Initializes SrQuat with the given axis-angle rotation, where the vector
        is the rotation axis, and its norm is the angle of rotation.
        The method set() is called inline in this constructor. */
    SrQuat ( const SrVec& axisangle ) { set(axisangle); }

    /*! Initializes SrQuat with the rotation from v1 to v2.
         The method set() is called inline in this constructor. */
    SrQuat ( const SrVec& v1, const SrVec& v2 ) { set(v1,v2); }

    /*! Initializes SrQuat from a rotation matrix */
    SrQuat ( const SrMat& m ) { set(m); }









    ///*! Set a random quaternion, using a uniform distribution method */
    //void random ();
    
    /*! Set the four values */
    void set ( float qw, float qx, float qy, float qz ) { w=qw; x=qx; y=qy; z=qz; }

    /*! Set the four values from a float pointer */
    void set ( const float* f ) { w=f[0]; x=f[1]; y=f[2]; z=f[3]; }

    /*! Defines SrQuat as the rotation from v1 to v2. */
    void set ( const SrVec& v1, const SrVec& v2 );

    /*! Defines SrQuat as the rotation around axis of the given angle in radians.
        Axis is internaly normallized. */
    void set ( const SrVec& axis, float radians );

    /*! Initializes SrQuat with the given axis-angle rotation, where the given
        vector is the rotation axis, and its norm is the angle of rotation. */
    void set ( const SrVec& axisangle );

    /*! Defines SrQuat by extracting the rotation from the given rotation matrix. */
    void set ( const SrMat& m );

	void set ( float ex, float ey, float ez);

    /*! Gets the current axis and angle of rotation (in radians) that SrQuat defines. */
    void get ( SrVec& axis, float& radians ) const;

    /*! Returns the rotation axis. */
    SrVec axis () const;

    /*! Returns the angle in radians. */
    float angle () const;

	SrVec axisAngle() const;

	SrVec getEuler() const;

    /*! Normalizes the quaternion and ensures w>=0 */
    void normalize ();

    /*! Returns the inverse quaternium. */
    SrQuat inverse() const;

    /*! Invert SrQuat. */
    void invert ();

    /*! Equivalent to get_mat(), but retrieves only the 3x3 portion
        of the transformation matrix. */
    void get_rot_mat ( float& a, float& b, float& c,
                       float& d, float& e, float& f,
                       float& g, float& h, float& i ) const;

    /*! Gets the equivalent transformation matrix. */
    SrMat& get_mat ( SrMat& m ) const;

	SrVec multVec(const SrVec& v);

    /*! Given a vector v and a quaternion q, the result of applying the rotation
       in q to v is returned (in mathematical notation this is q v q^-1) . */
    SBAPI friend SrVec operator * ( const SrVec &v, const SrQuat &q );

    /*! rotation q1 followed by rotation q2 is equal to q2*q1 . */
    SBAPI friend SrQuat operator * ( const SrQuat &q1, const SrQuat &q2 );

    /*! Comparison operator makes an exact comparison of the quaternion components. */
    friend bool operator == ( const SrQuat &q1, const SrQuat &q2 );

    /*! Comparison operator makes an exact comparison, of the quaternion components. */
    friend bool operator != ( const SrQuat &q1, const SrQuat &q2 );

    /*! Swaps the contents of q1 with q2. */
    friend void swap ( SrQuat &q1, SrQuat &q2 );

    /*! Returns the interpolation between q1 and q2 with parameter t.
        Parameter q1 is not const because it is normalized. */
    friend SrQuat slerp ( SrQuat &q1, const SrQuat &q2, float t );

    /*! Outputs data in "axis x y z ang a" format. */
    friend SrOutput& operator<< ( SrOutput& out, const SrQuat& q );

    /*! Input data from "axis x y z ang a" format. */
    friend SrInput& operator>> ( SrInput& in, SrQuat& q );

	void setData(int i, float val) { if (i == 0) w = val; else if (i == 1) x = val; else if (i == 2) y = val; else if (i == 3) z = val; }

	float getData(int i) { if (i == 0) return w; else if (i == 1) return x; else if (i == 2) return y; else if (i == 3) return z; else return 0;}
	std::string toString();
 };

//============================== end of file ===============================

# endif // SR_QUAT_H
