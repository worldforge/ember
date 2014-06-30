/*
 *  sk_posture.h - part of Motion Engine and SmartBody-lib
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
 *      Andrew n marshall, USC
 */

# ifndef SK_POSTURE_H
# define SK_POSTURE_H

# include <sr/sr_vec.h>
# include <sr/sr_buffer.h>

# include <sk/sk_channel_array.h>

class SkSkeleton;

//============================== SkPostureDfJoints =================================================

/*! Keeps a shared array of joints for specifying the joints to be
    considered by the distance function in SkPosture */
class SkPostureDfJoints : public SrSharedClass
 { public :
    /*! Array containing the list of joints */
    SrArray<SkJoint*> joints;

    /*! Will read: N jname1 ... jnameN, and fill the array of joints
        with the pointers to the joints found in sk.
        Returns the final size of the joint array. */
    int input ( SrInput& in, SkSkeleton* sk );

    /*! Returns array position of given joint name if found, or -1 otherwise */
    int search ( const char* jname );
 };

//================================ SkPosture =================================================

/*! SkPosture stores joint values specifying a posture and 3D points to be
    used by the distance function in the 'values' and 'joints' buffers.
    Two shareable classes are referenced: a channel array describing 'values',
    and a SkPostureDfJoints describing how to update 'points'. */
class SkPosture
 { public :
    SrBuffer<float> values; // values defining this posture
    SrBuffer<SrPnt> points; // articulation points used by the distance function
    
   private :
    char* _name;
    bool _syncpoints;
    SkChannelArray* _channels;
    SkPostureDfJoints* _dfjoints;
    char* _filename;          // file name (optional)

   public :
    /*! Default constructor initializes an empty, not usable posture.
        An init() method must be properly called after that. */
    SkPosture ();

    /*! Copy constructor. The name is also copied */
    SkPosture ( const SkPosture& p );

    /*! Constructor from a given channel array and (optionally) a dfjoint list,
        which will be both shared */
    SkPosture ( SkChannelArray* ca, SkPostureDfJoints* dfj=0 );

    /*! Constructor shares the channel array with the given skeleton */
    SkPosture ( SkSkeleton* s );

    /*! Destructor. Be sure to access it through unref() when needed. */
    virtual ~SkPosture ();

    /*! Set the posture empty and disconnected and thus not usable (name is not changed) */
    void init ();
 
    /*! Initializes with the active channels of the given skeleton */
    void init ( SkSkeleton* s );

    /*! Initializes by referencing to the given skeleton */
    void init ( SkChannelArray* ca );

    /*! Initializes from both the channel array and distance function joints */
    void init ( SkChannelArray* ca, SkPostureDfJoints* ja );

    /*! Returns the referenced channel array (which should not be modified).
        Remember that the channels here are shared, using ref()/unref(). */
    SkChannelArray* channels () { return _channels; }
    const SkChannelArray* const_channels () const { return _channels; }

    /*! Connects the channel array to the given skeleton */
    void connect ( SkSkeleton* sk ) { _channels->connect(sk); }

    /*! Returns the skeleton associated with the channel array (can be null) */
    SkSkeleton* skeleton () const { return _channels->skeleton(); }

    /*! Get the name, can be an empty string, but not a null pointer */
    const char* name () const { return _name? _name:""; }
    
    /*! Set a name */
    void name ( const char* n );

    /*! Get the file name associated with the posture */
    const char* filename () const { return _filename? _filename:""; }

    /*! Set a file name to be associated with the posture.
        This information is not saved in the motion file and is 
        not used by SkMotion. It is here just as a convenient 
        place to store the information */
    void filename ( const char* n ) { sr_string_set(_filename,n); }

    /*! Get and store the current values of the skeleton connected to the
        referenced channel array, which must exist. */
    void get ();

    /*! Apply the posture to the referenced channel array connected skeleton
        (must exist). Skeleton global matrices remain not updated. */
    void apply () const { _channels->set_values(&values[0]); }
 
    ///*! Get a random configuration according to the joint types and limits
    //    defined in the skeleton connected to the referenced channel array (must exist)*/
    //void random ();

    /*! Set specific joints to be considered by the distance function,
        otherwise, all joints of the channel array are used.
        Given dfjoints is a shared class. */
    void dfjoints ( SkPostureDfJoints* dfjoints );

    /*! Returns the referenced array of distance function joints (should not be modified) */
    SkPostureDfJoints* dfjoints () { return _dfjoints; }


    /*! Force to recompute the distance function points next time they are needed */
    void unsyncpoints () { _syncpoints=false; }

    /*! Returns true if the distance function points are in sync with the cur posture */
    bool syncpoints () const { return _syncpoints; }

    /*! To make means to (1) apply the posture to and (2) update the global matrices
        of the associated skeleton, to then (3) get the global coordinates of each
        joint defined to be used in the distance function.
        If no distance function joints are defined, all the joints in the channel
        array are taken (duplicated joints have no effect as we use the max norm).
        Method update_gmat() is called for the given joint j, or in case it is null
        (default), all the skeleton connected to the channel array is updated.
        This function is automatically called when need, usually the user does
        not need to call it. */
    void make_points ( SkJoint* j=0 );

    /*! Copy operator copies all internal data, but does not copy the name. */
    void operator= ( const SkPosture& p );

    /*! Put in p the interpolation between p1 and p2 at the parametric position t in [0,1].
        All given postures must have compatible existing referenced channel arrays */
    friend void interp ( const SkPosture& p1, const SkPosture& p2, float t, SkPosture& p );

    /*! Returns the maximum Euclidian distance between the corresponding points in
        the 'points' buffer field. If changes were made to the postures p1 and/or p2,
        their 'points' buffer are automatically rebuilt with make_points(). */
    friend float dist ( SkPosture& p1, SkPosture& p2 );

    /*! Custom output of posture data */
    void output ( SrOutput& out, bool channels, bool values ) const;

    /*! Write all data of the posture using output() (with all options to true) */
    friend SrOutput& operator<< ( SrOutput& out, const SkPosture& p );

    /*! Read the posture, converting the angles to radians. As only angle values
        are saved/read, the points array of p are left not up to date */
    friend SrInput& operator>> ( SrInput& inp, SkPosture& p );
 };

//================================ End of File =================================================

# endif // SK_POSTURE_H
