/*
*  sk_joint.h - part of Motion Engine and SmartBody-lib
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
*      Marcus Thiebaux, USC
*      Andrew n marshall, USC
*/

# ifndef SK_JOINT_H
# define SK_JOINT_H

#include <sb/SBTypes.h>
# include <sr/sr_mat.h>
# include <sr/sr_quat.h>
# include <sr/sr_array.h>

# include <sk/sk_joint_name.h>
# include <sk/sk_joint_pos.h>
# include <sk/sk_joint_quat.h>
# include <sk/sk_joint_euler.h>
# include <sk/sk_joint_swing_twist.h>
#include <sb/SBObject.h>

#include <string>
#include <vector>

class SrModel;
class SkSkeleton;

/*! SkJoint defines one joint and controls its translations and
rotations. The translation is always set by using the class
SkJointPos, while the rotation can be used with different
types of parameterization and joint limit control, all derived
from SkJointQuat. */
class SkJoint : public SmartBody::SBObject
{ public :
/*! RotType specifies the current joint rotation parameterization.
Current supported types are implemented in classes
SkJointEuler, SkJointQuat, and SkJointSwingTwist */
enum RotType { TypeQuat,       // Quaternion without limits, fastest convertion to matrix
TypeSwingTwist, // Axis-angle (x,y) swing, z rotation twist, ellipse limits
TypeEuler       // Euler angles with min/max limits per dof
};

enum JointType { TypeJoint,
TypeViseme,
TypeBlendShape,
TypeOther
};

protected:
	SrModel* _visgeo; // the attached geometry to visualize this joint
	SrModel* _colgeo; // the attached geometry used for collision detection
	SkJoint* _parent;
	std::vector<SkJoint*> _children;
	SrMat _gmat;           // global matrix: from the root to the children of this joint
	SrMat _gmatZero;       // global rotation matrix when all joint channels are zero
	SrMat _lmat;           // local matrix: from this joint to its children
	char  _lmat_uptodate;  // true if lmat is up to date
	std::string _name;     // the given name
	std::string _sbName;   // smartbody internal name after joint mapping
	std::string _extName;  // external name for the bone
	std::string _extID;    // external id from OpenCollada	
	std::string _extSID;   // external sid from OpenCollada
	int   _index;          // its index in SkSkeleton::_joint_table
	int   _coldetid;       // index used in collision detection
	SkSkeleton* _skeleton; // pointer to the associated skeleton
	SrVec _offset;         // offset from the joint to its children
	SrVec _endEffectorOffset;         // offset from the joint to its children
	char _rtype;           // one of the RotType enumerator
	SkJointQuat* _quat;    // generic access to the quaternion of any parameterization
	SkJointPos _pos;       // controls the translation parameterization
	float _mass;		   // mass of the bone associated with the joint
	int _jointType;

	friend class SkSkeleton;
	friend class SkColdet;

public:
	SkJoint();
	// constructor sets all dofs to non active, and rotation type as euler
	SkJoint ( SkSkeleton* sk, SkJoint* parent, RotType rtype, int i );
	virtual ~SkJoint ();

	void copyTo(SkJoint* dest);

	/*! Get a pointer for the attached geometry for visualization,
	or null if no such geometry was loaded. The geometry pointers
	can be shared ( see SrModel::ref/unref). */
	SrModel* visgeo () const { return _visgeo; }

	/*! Get a pointer for the attached geometry for collision detection,
	or null if no such geometry was loaded. The geometry pointers
	can be shared ( see SrModel::ref/unref). */
	SrModel* colgeo () const { return _colgeo; }

	/*! Replaces the visualization geometry pointer with the new one.
	The ref/unref methods are used during replacement and null can be
	passed in order to only unref the visualization geometry */
	void visgeo ( SrModel* m );

	/*! Replaces the collision geometry pointer with the new one.
	The ref/unref methods are used during replacement and null can be
	passed in order to only unref the collision geometry */
	void colgeo ( SrModel* m );

	/*! Get a pointer to the skeleton owner of this joint */
	SkSkeleton* skeleton () { return _skeleton; }

	/*! Sets the skeleton skeleton owner of this joint */
	void skeleton (SkSkeleton* skel) { _skeleton = skel; }

	/* thiebaux 6/19/2006 */
	void set_parent( SkJoint* p ) { _parent = p; }

	/*! traverse hierarchy */
	SkJoint* parent() const { return _parent; }
	SkJoint* child ( int i ) const { return _children[i]; }
	int num_children () const { return _children.size(); }

	/* Recursively get all the child joint of current joint */
	static void recursive_children(std::vector<SkJoint*>& joints, SkJoint* root);

	/*! Set the name of this joint */
	void name ( const std::string& jn ) { _name=jn; }
	SBAPI const std::string& jointName () const;

	std::string getMappedJointName();
	void updateMappedJointName();

	/*! Set the name of this joint */
	void extName ( const std::string& jn ) { _extName=jn; }
	const std::string& extName () const { return _extName; }
	
	void extID(std::string val) { _extID = val; }
	const std::string& extID() const { return _extID; }

	void extSID(std::string val) { _extSID = val; }
	const std::string& extSID() const { return _extSID; }

	/*! Returns the index of this joint in the SkSkeleton list
	of joints */
	int index () const { return _index; }
	void index (int index)  { _index = index; }

	void set_index(int id)	{_index = id;}

	/*! Returns the collision detection id of the colgeo attached
	to this joint (or -1 if no id). */
	int coldet_id () const { return _coldetid; }

	/*! returns the fixed frame translation relative to the parent */
	SrVec offset () const { return _offset; }

	/*! change the fixed frame translation relative to the parent */
	void offset ( const SrVec& o );

	/*! returns the fixed frame translation relative to the parent */
	SrVec endEffectoroffset () const { return _endEffectorOffset; }

	/*! change the fixed frame translation relative to the parent */
	void endEffectorOffset ( const SrVec& o );

	/*! Access the translation parameterization of the joint. */
	SkJointPos* pos () { return &_pos; }
	const SkJointPos* const_pos () const { return &_pos; }

	/*! Set the desired rotation model. The associated parameterization
	class (derived from SkJointParam) will then replace the current one.
	If the type is undefined, Quaternion type is selected. 
	If the type is already the existing one, nothing is done. */
	void rot_type ( RotType t );

	/*! Returns the current used rotation parameterization */
	RotType rot_type () const { return (RotType)_rtype; }

	/*! Access the Euler rotation parameterization model of the joint.
	This method can only be called if the joint is in Euler mode */
	SkJointEuler* euler () { return (SkJointEuler*)_quat; }
	const SkJointEuler* const_euler () const { return (SkJointEuler*)_quat; }

	/*! Access the Swing and Twist rotation parameterization model of the joint.
	This method can only be called if the joint is in SwingTwist mode */
	SkJointSwingTwist* st () { return (SkJointSwingTwist*)_quat; }
	const SkJointSwingTwist* const_st () const { return (SkJointSwingTwist*)_quat; }

	/*! Access the quaternion-based rotation of the joint. It can be accessed
	regardless of the parameterization type, however conflicts may arise 
	if a new quaternion is set while the joint is not in TypeQuat mode */
	SkJointQuat* quat () { return _quat; }
	const SkJointQuat* const_quat () const { return _quat; }

	/*! Set the active position channels to their "zero" values */
	void init_pos ();

	/*! Set the active rotation channels to their "zero" values */
	void init_rot ();

	/*! Set the active channels (both position and rotation) to their "zero" values */
	void init_values () { init_pos(); init_rot(); }

	/*! If the current local matrix is not up to date, it will
	recalculate the local matrix based on the current
	local translation and rotation parameterization */
	void update_lmat ();

	/*! Recursivelly updates the local matrix and the global
	matrices of the joint and all its children. It assumes
	that the global matrix of the parent is up to date.
	Note: whenever this method is called, the whole branch
	under the joint is updated */
	SBAPI void update_gmat ();

	/*! Same as update_gmat(), but it stops the recursive update
	of chidren when joint in the last_joints arrays are reached */
	SBAPI void update_gmat ( std::vector<SkJoint*>& end_joints );

	/*! Updates the local matrix and set the global matrix of this joint
	to be the local matrix multiplied by the parent global matrix */
	void update_gmat_local ();

	/*! Finds and updates the local and global matrices of all joints
	in the branch leading to the root joint, until stop_joint or
	the root is found (stop_joint is not updated). */
	void update_gmat_up ( SkJoint* stop_joint=0 );

	/*! Ensures that the local matrix is updated and returns it. */
	const SrMat& lmat () { update_lmat(); return _lmat; }

	/*! Will force the reconstruction of the local matrix from the
	rotation and position parameters. The skeleton is also notified
	with a call to invalidate_global_matrices() */
	void set_lmat_changed ();

	/*! Returns the current global matrix. Be sure that it is up to
	date by calling one of the several updated methods. It gives
	the transformation from the root to the children of this joint */
	const SrMat& gmat () const { return _gmat; }

	/*! Returns the translation encoded in the current global matrix.
	Be sure that the global matrix is up to date */
	SrVec gcenter () const { return _gmat.get_translation(); }

	/*! Get a single visualization model for this node and all the
	children (update_gmat) is called */
	void unite_visgeo ( SrModel& m );

	/*! Get a single collision model for this node and all the
	children (update_gmat) is called */
	void unite_colgeo ( SrModel& m );

	/*! Sets the mass of the bone that is associated with the joint.
	The bone is assumed to be at the midpoint between the joint and
	the average of all the child joints. */
	void mass (float m) { _mass = m; };

	/*! Gets the mass of the bone. */
	float mass () const { return _mass; };

	int getJointType() { return _jointType; };
	void setJointType(int jtype) {  _jointType = jtype; };

	SBAPI void add_child ( SkJoint* child );
	SBAPI void remove_child (SkJoint* child);

	void updateGmatZero(const SrMat& gmatZero);
	const SrMat& gmatZero() { return _gmatZero; }
	SBAPI SrVec localGlobalAxis(int i);	
};

//==================================== End of File ===========================================

# endif  // SK_JOINT_H
