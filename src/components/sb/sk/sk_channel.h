/*
 *  sk_channel.h - part of Motion Engine and SmartBody-lib
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

# ifndef SK_CHANNEL_H
# define SK_CHANNEL_H

#include <sb/SBTypes.h>
# include <sk/sk_joint.h>

/*! SkChannel manipulates a joint according to the specified
    channel. A channel can be seen as an extension of a DOF
    to more complex parameterizations of a group of DOF. 
    For example, a channel can be a (euler) rotation around one axis,
    or a swing rotation or a full quaternion. Possible channels
    are enumerated in the enum Type */
class SkChannel
 { public :
    /*! Possible types for a channel. Quat requires 4 floats, Swing 2 floats,
        and all the others require 1 float to be fully represented.
        (enum order must not be modified) */
    enum Type { XPos, YPos, ZPos, XRot, YRot, ZRot, Quat, Swing, Twist };

    SkJoint* joint; //<! The joint to access the channel
    Type type;      //<! The channel type

   public :

    /*! Constructor */
    SkChannel ( SkJoint* j=0, Type t=XPos ) { joint=j; type=t; }

    /*! set both values at once */
    void set ( SkJoint* j, Type t ) { joint=j; type=t; }
    
    /*! Put values in the channel of the associated joint. The number of
        actual floats inserted in the joint are returned (can be 1, 2 or 4).
        It is the user responsibility to ensure that the used channel is
        compatible with the current rotation mode of the associated joint */
    int set ( const float* v );

    /*! Get the channel values. The number of actual floats inserted in v
        are returned (can be 1, 2 or 4). It is the user responsibility to
        ensure that the used channel is compatible with the rotation type
        of the associated joint */
    SBAPI int get ( float* v );

//    /*! Get random values for the specified channel, according to the 
//        associated joint limits and parameterization type. The number of
//        actual floats inserted in v are returned (can be 1, 2 or 4).
//        It is the user responsibility to ensure that the used channel
//        is compatible with the rotation type being used */
//    int get_random ( float* v ) const;

    /*! Returns in v the interpolation between values v1 and v2,
        according to the channel type. Static Method.
        Returns the size of the channel being interpolated */
    static int interp ( SkChannel::Type type, float* v, const float* v1, const float* v2, float t );

    /*! Member function version of interp() */
    int interp ( float* v, const float* v1, const float* v2, float t ) const
        { return interp(type,v,v1,v2,t); }

    /*! The given values are multiplied by mfactor and then 'added' with offset.
        If mfactor is 0, offset becomes the new value. For the Swing and Quat 
        types of channels, mfactor acts as a multiplication of the current 
        rotational angle encoded in the rotation. Offset and values need to
        contain up to 4 valid positions, according to the channel type */
    static void change_values ( Type type, float* values, float mfactor, const float* offset );

    /*! Member function version of change_values() */
    void change_values ( float* values, float mfactor, const float* offset )
         { change_values ( type, values, mfactor, offset ); }

    /*! The size of a channel can be 1, 2(Swing), or 4 (Quat) */
    SBAPI static int size ( Type type );

    /*! Member function version of size() */
    int size () const { return size(type); }
    
    /*! Returns a static string with the name of the channel type, 
        eg, "XPos", etc. If c is 'out of range', string "Undef" is returned */
    SBAPI static const char* type_name ( Type type );

    /*! Member function version of type_name() */
    const char* type_name () const { return type_name(type); }

    /*! Returns true if channel type is a valid channel of a joint in
        the rotation parameterization type r. Note that Quat channel 
        is always valid as it is the base class of all rotation types. */
    static bool valid ( Type type, SkJoint::RotType r );

    /*! Member function version of valid() */
    bool valid () const { return valid(type,joint->rot_type()); }

    /*! Returns the type of the channel according to the name in string s */
    static Type get_type ( const char* s );

    /*! Outputs the needed number of floats from v, according to type.
        Returns the processed channel type size, which can be different
        than the number of floats written (Quat is written in axis-angle) */
    static int save ( Type type, SrOutput& o, const float* v );
    
    /*! Member function version of save() */
    int save ( SrOutput& o, const float* v ) const { return save(type,o,v); }
    
    /*! Inputs the channel values to v according to type and returns
        the number of floats put in v */
    static int load ( Type type, SrInput& in, float* v, double scale = 1.0 ); // static

    /*! Member function version of load() */
    int load ( SrInput& in, float* v, double scale = 1.0 ) { return load(type,in,v,scale); }
 };

//==================================== End of File ===========================================

# endif  // SK_CHANNEL_H
