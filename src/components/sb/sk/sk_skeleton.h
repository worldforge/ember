/*
 *  sk_skeleton.h - part of Motion Engine and SmartBody-lib
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
 *      Marcus Thiebaux, USC
 */

# ifndef SK_SKELETON_H
# define SK_SKELETON_H

#include <sb/SBTypes.h>
# include <sr/sr_string.h>
# include <sr/sr_hash_table.h>
# include <sr/sr_shared_class.h>
# include <sr/sr_box.h>
# include <sk/sk_joint.h>
# include <sk/sk_channel_array.h>
# include <sk/sk_posture.h>
#include <sb/SBAsset.h>

class SrStringArray;
class SrVarTable;
class SkColdet;
namespace SmartBody {
	class SBSkeleton;
}
//================================ SkSkeleton =================================================

/*! A Skeleton defined as a hierarchy of joints */
class SkSkeleton : public SmartBody::SBAsset, public SrSharedClass
 { protected :
    // basic data:
	std::string _skfilename;
    SkJoint* _root;
    std::vector<SkJoint*> _joints;
    std::map<std::string, SkJoint*> _jointMap;
	std::map<std::string, SkJoint*> _extJointMap;
	std::map<std::string, SkJoint*> _extIDJointMap;	
	std::map<std::string, SkJoint*> _extSIDJointMap;	
    bool _gmat_uptodate;

    // collision detection:
    std::vector<SkJoint*> _coldet_free_pairs;
    int _coldetid;
    friend class SkColdet;

    // posture management:
    SkChannelArray* _channels;
    SrArray<SkPosture*> _postures;
	// physical properties
	SrVec _com;
	std::string jointMap;

	public :
    /*! Constructor */
    SBAPI SkSkeleton ();

	SBAPI SkSkeleton(SkSkeleton* origSkel);

    /*! Destructor is public but pay attention to the use of ref()/unref() */
    SBAPI virtual ~SkSkeleton ();

	/*! Sets the filename used to load the skeleton, if any */
    SBAPI void skfilename ( const char* s ) { _skfilename = s; }

    /*! Access to the .sk skeleton definiton, if any*/
	SBAPI const std::string& skfilename () { return _skfilename; }

    /*! Deletes all data and set the skeleton to be an empty hierarchy */
    SBAPI void init ();
	
	SBAPI std::vector<SkJoint*>& get_joint_array( void ) { return( _joints ); }

    /*! Adds a joint to the hierarchy. If parentid<0 (the default) the last
        joint in the joint list is used as parent (the joint becomes root
        if the list is empty) */
    SBAPI SkJoint* add_joint ( SkJoint::RotType rtype, int parentid=-1 );

    /*! Inserts a new joint above the previous root */
    SBAPI SkJoint* insert_new_root_joint ( SkJoint::RotType rtype );

    /*! Returns the collision detection id (or -1 if no id). */
    SBAPI int coldet_id () const { return _coldetid; }

	/*! Refresh the joints */
	SBAPI void refresh_joints();

    /*! Returns a flat list with all joints. Note that method
        SkJoint::index() returns the index of the joint in this list */
    SBAPI const std::vector<SkJoint*>& joints () const { return _joints; }

    /*! Array with the pairs that should be deactivated for collision
        detection at connection time with SrColdet. The pairs can be
        declared in the skeleton .sk file */
    SBAPI std::vector<SkJoint*>& coldet_free_pairs () { return _coldet_free_pairs; }

    /*! Returns the channel array of all the active channels */
    SBAPI SkChannelArray& channels () { return *_channels; }

    /*! Returns the array of pre-defined postures loaded from the .sk file */
    SBAPI SrArray<SkPosture*>& postures () { return _postures; }

    /*! Rebuilds the active channels according to the free channels in the joint hierarchy */
    SBAPI void make_active_channels () { _channels->get_active_channels(this); }
    
    /*! Returns a pointer to the root joint of the skeleton */
    SBAPI SkJoint* root () const { return _root; }

	/*! Set the root joint pointer */
	SBAPI void root(SkJoint* r)	{_root = r;}

	/*! Returns the joint with name n. A hash table is used to perform the search.
        0 is returned in case the joint is not found.
        If the hash table is not up to date, it is automatically re-created. */
    SBAPI SkJoint* search_joint ( const char* n );

	SBAPI void updateJointMap();
    /*! Returns the joint with name n performing a linear search.
        0 is returned in case the joint is not found.
        This method should be used if the joint list is being constructed,
        as it does not rely on the hash table. */
    SBAPI SkJoint* linear_search_joint ( const char* n ) const;

    /*! Calls root()->update_gmat() only if it is required due to any
        changes to the local matrices in joints */
    SBAPI void update_global_matrices ();

	SBAPI void updateGlobalMatricesZero();

    /*! Returns true if all global matrices are up to date */
    SBAPI bool global_matrices_uptodate () { return _gmat_uptodate; }

    /*! Set the internal flag that controls global matrices update to false.
        This method is automatically called each time a joint value is changed */
    SBAPI void invalidate_global_matrices () { _gmat_uptodate=false; }

    /*! Compress all internal arrays */
    SBAPI void compress ();

    /*! To fix geometries that were created in global coordinates */
    SBAPI void set_geo_local ();

    /* Loads a skeleton hierarchy in .sk or .bvh format.
       Returns false if some error is encountered, otherwise true is returned.
       Parameter basedir can specify the base directory used for searching for geometry
       (srm) files, which are searched in the paths declared in the sk file, in basedir,
       and in the current directory. Also, relative paths in sk become relative to basedir.
       If basedir is not given (null), it is extracted from in.filename(), if available.
       Method compress() is called after the file is loaded. */
    SBAPI bool loadSk ( SrInput& in, double scale = 1.0, const char* basedir=0 );

    /*! Save in .sk format the current skeleton.
        Geometries are also exported if parameter geopath is provided, or if
        a valid path can be extracted from SrOutput::filename(). */
    SBAPI bool save ( SrOutput& out, const char* geopath=0 );

    /*! Save joints definitions that can be merged into a skeleton */
    SBAPI bool export_joints ( SrOutput& out );

	SBAPI void copy(SkSkeleton* origSkel);

	/*! Computes the center of mass of the skeleton */
	SBAPI void compute_com () {}; 

	/*! retrieves the center of mass of the skeleton */
	SBAPI SrVec& com () { return _com; };	
	
	SBAPI float getBaseHeight(const std::string& baseName);
	SBAPI float getCurrentHeight();	
	SBAPI SrBox getBoundingBox();
	
	SBAPI void copy_joint(SkJoint* dest, SkJoint* src);
	SBAPI void create_joints(SkJoint* origParent, SkJoint* parent);	
	SBAPI void clearJointValues(); // reset all joint quat/pos values to zero


	// get the global direction of a bone pair based on the current skeleton configuration
	SBAPI SrVec boneGlobalDirection(const std::string& srcJoint, const std::string& dstJoints); 
	SBAPI SrVec getFacingDirection();
	SBAPI void resetSearchJoint();

	void setJointMapName(const std::string& jointMapName);
	std::string getJointMapName();

   protected :
    int _loadjdata ( SrInput& in, SkJoint* j, SrStringArray& paths );
    SkJoint* _loadj ( SrInput& in, SkJoint* p, SrStringArray& paths, bool merge );
    SkJoint* _loadjlist ( SrInput& in, float scale, SrStringArray& paths );
 };

//================================ End of File =================================================

# endif  // SK_SKELETON_H
