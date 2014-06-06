/*
 *  sk_joint_pos.h - part of Motion Engine and SmartBody-lib
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
 */

# ifndef SK_JOINT_POS_H
# define SK_JOINT_POS_H

#include <sb/SBTypes.h>
# include <sk/sk_vec_limits.h>

class SkJoint;

/*! SkJointPos controls values and min/max limits for three (x,y,z) translations. */
class SkJointPos : public SkVecLimits
 { private :
    SkJoint* _joint; // to notifiy the joint that the current values changed
    friend class SkJoint;  // used only to fix a constructor-related warning
    
   public :
   
    /* Constructor calls init(), which set all values frozen in 0. */
    SkJointPos ( SkJoint* j ) { _joint=j; }

    /*! Set values per degree of freedom. In case joint limits are activated,
        values are bounded to the limits before inserted. The associated
        joint local matrix is set to changed if needed. */
    SBAPI void value ( int d, float val );
    
    /*! Simply calls the same method in the base class */
    float value ( int d ) const { return SkVecLimits::value(d); }
    
    /*! Simply calls the same method in the base class */
    const float* value () const { return SkVecLimits::value(); }
 };

//==================================== End of File ===========================================

# endif  // SK_JOINT_POS_H
