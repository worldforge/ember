/*
 *  sk_joint_swing_twist.h - part of Motion Engine and SmartBody-lib
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

# ifndef SK_JOINT_SWING_TWIST
# define SK_JOINT_SWING_TWIST

# include <sk/sk_joint_quat.h>

/*! Implements the swing and twist decomposition of a rotation, which is
    suitable to define meaningfull swing limits with the use of an ellipse.
    The swing is parameterized in the axis-angle format, in the XY plane.
    The twist is then defined as a rotation around Z axis.
    For further details, read:
    P. Baerlocher, R. Boulic, Parametrization and range of motion of the ball-
    and-socket joint, Deformable Avatars, Kluwer Academic Publishers Publ., 2001.
    http://vrlab.epfl.ch/Publications/pdf/Baerlocher_Boulic_Deformable_Avatars_01.pdf */
class SkJointSwingTwist : public SkJointQuat
 { private :
    float _sx, _sy;     // swing values X Y in axis-angle format
    float _ex, _ey;     // ellipse radius to limit swing
    float _twist;       // twist is a rotation around Z
    float _tmin, _tmax; // twist min/max limits
    char _limit_twist;  // if twist limits are active or not

   public :
    /*! Constructor calls init() */
    SkJointSwingTwist ( SkJoint* j ) : SkJointQuat(j) { init(); }

    /*! Init swing to (0,0), limits to (PI,PI), and twist frozen to 0. */
    void init ();

    /*! Returns true if swing (x,y) is inside the limits, and false otherwise.
        (if (x/ellipsex)^2 + (y/ellipsey)^2 - 1 <=0 then true is returned) */
    bool swing_in_limits ( float x, float y );
 
    /*! Set swing values, which are bounded to the ellipse limits before inserted.
        The local matrix of the associated joint will be set as changed only
        if a different value than the actual one is set.
        Note: (x,y) represents the rotation axis, and ||(x,y)|| is the rotation angle */
    void swing ( float x, float y );

    /*! Change the swing x value and calls swing(x,y) method to enforce limits */
    void swingx ( float x ) { swing(x,_sy); }

    /*! Change the swing y value and calls swing(x,y) method to enforce limits */
    void swingy ( float y ) { swing(_sx,y); }

    /*! Returns the x component of the current swing */
    float swingx () { _checksync(); return _sx; }

    /*! Returns the y component of the current swing */
    float swingy () { _checksync(); return _sy; }

    /*! Change the x/y radius of the limiting ellipse, ensuring swing remains valid.
        Given parameters are automatically bounded into (0,pi] */
    void ellipse ( float rx, float ry );
    
    /*! Get the x radius of the limiting ellipse, which is in (0,pi] */
    float ellipsex () const { return _ex; }

    /*! Get the y radius of the limiting ellipse, which is in (0,pi] */
    float ellipsey () const { return _ey; }

    /*! Set the twist value. In case twist limits are activated, the value is
        bounded to the limits before inserted. Changes only if a new value is set */
    void twist ( float t );
    
    /*! Returns the current twist value */
    float twist () { _checksync(); return _twist; }

    /*! Set twist limits and ensures twist value remains valid if limits are active */
    void twist_limits ( float min, float max );

    /*! Returns the min twist limit value */
    float twist_min () const { return _tmin; }

    /*! Returns the min twist limit value */
    float twist_max () const { return _tmax; }

    /*! Activates or deactivates twist limits */
    void twist_limits ( bool b ) { _limit_twist = b? 1:0; }

    /*! Returns the activation state of twist limits */
    bool twist_limits () { return _limit_twist? true:false; }

    /*! Returns true if the given value respects current twist limits and false otherwise */
    bool twist_in_limits ( float t ) const
     { return _tmin<=t && t<=_tmax? true:false; }

    /*! Activates limits and set min and max equal to the current twist value */
    void twist_freeze ();
    
    /*! Returns true if the twist value is frozen, i.e if it cannot receive new values.
        If the minimum limit is equal to the maximum limit, and limits are activated,
        then the twist is considered frozen. */
    bool twist_frozen () const;
    
    ///*! Returns random swing values, limited by the current ellipse radius */
    //void get_random_swing ( float& x, float& y ) const;

    ///*! Returns random twist values, limited if twist limits are activated */
    //float get_random_twist () const;

    /*! Put in the given quaternion the rotation equal to: R=Rtwist*Rswing */
    virtual void get_quat ( SrQuat& q ) const;
    
    /*! Converts the given quaternion q in the swing and twist parameterization.
        True is returned in case of succes and the values are put in sx, sy, and tw.
        False is returned in case the converted swing reaches the singularity, and
        in such case the returned values are sx=sy=pi, and tw=0. */
    static bool quat_to_st ( const SrQuat& q, float& sx, float& sy, float& tw );
    
    /*! This convertion function is similar but it puts the twist *before* the swing. */
    static bool quat_to_ts ( const SrQuat& q, float& tw, float& sx, float& sy );

   private:
    void _checksync();
 };

//==================================== End of File ===========================================

# endif  // SK_JOINT_SWING_TWIST
