/*
 *  sk_joint_euler.h - part of Motion Engine and SmartBody-lib
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
 *      Marcelo Kallmann, USC (currently at UC Merced)
 *      Andrew n marshall, USC
 */

# ifndef SK_JOINT_EULER_H
# define SK_JOINT_EULER_H

# include <sr/sr_mat.h>

# include <sk/sk_vec_limits.h>
# include <sk/sk_joint_quat.h>

//============================= SkJointEuler ============================

/*! SkJointEuler parameterizes a rotation with Euler angles. 
    Values and limits management follows the interface of SkVecLimits. 
    Note: the automatic (inverse function) update of the euler parameters when
    a rotation is set at the SkJointQuat level was not yet needed thus not implemnted. */
class SkJointEuler : public SkJointQuat, public SkVecLimits
 { private :
    char _type;
    
   public :

    /*! The currently supported euler types */
    enum Type { TypeXYZ, TypeYXZ, TypeZY };
    
    /*! Constructor sets type to YXZ and calls init(), 
        that set all values frozen in 0. */
    SkJointEuler ( SkJoint* j ) : SkJointQuat(j) { _type=(char)TypeYXZ; }
    
    /*! Change the current type (non-used rotations are frozen to zero) */
    void type ( Type t );

    /*! Get the current type */
    Type type () const { return (Type)_type; }

    /*! Set values per degree of freedom. In case joint limits are activated,
        values are bounded to the limits before inserted. The associated
        joint local rotation is set to changed if needed. */
    void value ( int d, float val );
    
    /*! Init values and set the rotation as changed */
    void init ();

    /*! Simply calls the same method in the base class */
    float value ( int d ) const { return SkVecLimits::value(d); }
    
    /*! Simply calls the same method in the base class */
    const float* value () const { return SkVecLimits::value(); }

    /*! Put in the quaternion the rotation equivalent to the
        current type. For example in type YXZ (the default), the
        rotation will be equal to the rotation matrix R=Ry*Rx*Rz */
    virtual void get_quat ( SrQuat& q ) const;

	/*! get the rotation around the X axis */
	float xRot();
	/*! set the rotation around the X axis */
	void xRot( float xRot );
	/*! get the rotation around the Y axis */
	float yRot();
	/*! set the rotation around the Y axis */
	void yRot( float yRot );
	/*! get the rotation around the Z axis */
	float zRot();
	/*! set the rotation around the Z axis */
	void zRot( float zRot );
 };

//==================================== End of File ===========================================

# endif  // SK_JOINT_EULER_H
