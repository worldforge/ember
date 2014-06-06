/*
 *  sk_joint_quat.h - part of Motion Engine and SmartBody-lib
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

# ifndef SK_JOINT_QUAT_H
# define SK_JOINT_QUAT_H

#include <sb/SBTypes.h>
# include <sr/sr_quat.h>

class SkJoint;

/*! Controls the joint rotation with a quaternion.
    This is the default parameterization as no trigonometric
    functions are needed to construct the rotation matrix needed for rendering.
    Pararameterizations based on Euler angles or Swing and Twist decomposition
    are implemented in classes deriving this one */
class SkJointQuat
 { protected://private :
    SkJoint* _joint; // to notify the joint that the current rotation has changed
    SrQuat _quat;    // the quaternion
	SrQuat _rawQuat; // the raw quaternion value without applying joint orientation and pre-post rotation
    bool _active;    // to block access to setting new rotations
    bool _jntsync;   // 0 whenever a new quat is set, and only SkJoint will make it 1 (SkJoint is a friend)
    bool _dersync;   // 0 whenever a new quat is set, and only the derived class will make it 1 with ask_new
    bool _asknew;    // to notify that a new rotation is available from the derived class
    struct PrePost { SrQuat pre, post; };
    PrePost* _prepost; // frame adjustment rotations, important: only for the derived classes.
    friend class SkJoint;
	SrQuat _jorientation; // now 

   public :

    /*! Constructor initializes the joint as frozen, ie, non active */
    SkJointQuat ( SkJoint* j );

    /*! Destructor ensures prepost structure is deleted, in case it was used */
    virtual ~SkJointQuat();

    /* Set the quaternion to a null rotation and invalidate the sync flags. */
    void init () { value(SrQuat::null); }

    /*! Set a new rotation and notifies the associated joint that its
        local matrix requires to be changed. The new rotation is 
        only set if this object is active, ie, not frozen.
        Note: the given quaternion will become the final joint rotation, 
        even if pre/post rotations exist (they are only used by derived classes)*/
    SBAPI void value ( const SrQuat& q );

    /*! Same value(q) method, but receiving a float pointer as input */
    SBAPI void value ( const float* f );

    /*! Access the current quaternion. If a new rotation is available
        by derived classes, get_quat will be called. */
    SBAPI const SrQuat& value ();

	const SrQuat& rawValue();
    
    /*! Freeze rotations, ie, make new calls to value(q) to have no effect */
    void deactivate () { _active=0; }
    
    /*! Set the rotation active, i.e accepting new values. */
    void activate () { _active=1; }

    /*! Returns true if the joint is active, and false otherwise  */
    bool active () const { return _active? true:false; }

    /*! Returns true if there are pre/post rotations assigned for the derived class */
    bool has_prepost () const { return _prepost? true:false; }
    
    /*! Specifies a pre-multiplied frame rotation adjustment to help placing the
        derived class rotation parameterization frame of reference as needed */
    SBAPI void prerot ( const SrQuat& q );

    /*! Specifies a post-multiplied frame rotation adjustment for the derived class */
   SBAPI void postrot ( const SrQuat& q );

    /*! Options for method adjust_prepost() */
    enum AlignType { AlignPre, AlignPost, AlignPrePost, AlignPreInv, AlignPostInv };

    /*! Adjust pre and post rotations for alignment in v direction. Options:
        AlignPre: align local frame so that v follows incoming parent link;
        AlignPost: align local frame so that v follows outgoing child(0) link;
        AlignPrePost: performs both Pre and Post alignments;
        AlignPreInv: performs Pre alignment and accumulates its inverse in posrot;
        AlignPostInv: performs Post alignment and accumulates its inverse in prerot; */
    void align ( AlignType t, const SrVec& v );
    
    /*! returns the current pre-multiplied rotation adjustment for the derived class
        note: quat.w is 1 if no rotation */
    SBAPI const SrQuat& prerot () const { return _prepost? _prepost->pre:SrQuat::null; }

    /*! returns the current pos-multiplied rotation adjustment for the derived class
        note: quat.w is 1 if no rotation */
    SBAPI const SrQuat& postrot () const { return _prepost? _prepost->post:SrQuat::null; }

	SBAPI void orientation(const SrQuat& q);

	SBAPI const SrQuat& orientation();

	/*! convert local rot into world aligned rot (added by David Huang 2012-06)
	// for applying directly on world-aligned (MotionBuilder standard) T-pose skel
	// use after doing SBSkeleton::orientJointsLocalAxesToWorld() */
	SrQuat getWorldAlignedLocalRot();

   protected :

    /*! This method is to be called by derived classes whenever a new rotation
        is available to be retrieved with method get_quat(). It also notifies 
        the associated joint that its local matrix requires to be changed. */
    void ask_new ();

    /*! Returns true if the quat class is up to date in relation to the derived class,
        otherwise (false returned) it means the quat was changed not trought the derived class. */
    bool dersync () { return _dersync? true:false; }
    
    /*! Ask to the derived class a new quaternion (the implementation here does nothing). */
    virtual void get_quat ( SrQuat& q ) const;
 };

//==================================== End of File ===========================================

# endif  // SK_JOINT_QUAT_H
