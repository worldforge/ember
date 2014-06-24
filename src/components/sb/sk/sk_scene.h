/*
 *  sk_scene.h - part of Motion Engine and SmartBody-lib
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

#ifndef SK_SCENE_H
#define SK_SCENE_H

//================================ SkScene =================================================

#include "components/sb/sb/SBTypes.h"
#include "components/sb/sr/sr_array.h"
#include "components/sb/sr/sr_sn_group.h"
#include "components/sb/sr/sr_sn_shape.h"

class SrModel;
class SkJoint;
class SkSkeleton;

/*! Maintains a scene graph containing geometries to display a given SkSkeleton */
class SBAPI SkScene : public SrSnGroup
 { private :
    SrArray<SrSnGroup*> _jgroup;
    float _cradius, _sfactor, _axislen;
	float scaleFactor;
    SkSkeleton* _skeleton;
	bool _needsInit;

	void initInternal();

   public :
    /*! Constructor  */
    SkScene ();

    /*! Destructor unreferences the associated skeleton */
    virtual ~SkScene ();

    /*! Creates a scene graph according to the given skeleton.
        Only the visualization geometry is set as visible; method
        set_visibility() can be used to visualize other objects.
        Null can be passed to clear the scene.
        The skeleton ref()/unref() methods are respected. */
    void init ( SkSkeleton* s, float scale = 1.f );

	SrSnSphere* createSphere( float scaleFactor );
    /*! Update the transformations of the scene graph according
        to the joints in the skeleton sent to init(). */
    void update ();

    /*! Update the scene transformation relative to the given joint index j,
        of the skeleton sent to init. */
    void update ( int j );

    /*! Rebuild all joints of the current skeleton */
    void rebuild ();

    /*! Rebuild all data relative to the given joint index j */
    void rebuild ( int j );

    /*! Set the visibility state of the skeleton, the visualization geometry,
        the collision geometry, and joint axis. The integers mean 1:show, 0:hide, and
        -1:don't change the visibility state. */
    void set_visibility ( int skel, int visgeo, int colgeo, int axis );

    /*! Same as the other set_visibility() method, but only affecting the given joint */
    void set_visibility ( SkJoint* joint, int skel, int visgeo, int colgeo, int vaxis );

    /*! Change the radius of the cylinders used to draw the skeleton */
    void set_skeleton_radius ( float radius );

	void setJointRadius( SkJoint* joint, float radius);
	void setJointColor( SkJoint* joint, SrColor color);
    
    /*! Update the length of the axis to the new value */
    void set_axis_length ( float l );
    
    /*! Mark with red color the geometry attached to joint j */
    void mark_geometry ( SkJoint* j );

    /*! Restore original colors of all geometries */
    void unmark_all_geometries ();

    /*! Set the model m attached to j to have the given render mode, and be optionally
        marked with red color. Default mode restores the original render mode.
        If alpha!=-1 alpha is set in the diffuse color of geometries materials. */
    void set_geometry_style ( SkJoint* j, SrModel* m, srRenderMode mode, bool mark, int alpha=-1 );
    
    /*! Get the default values for the skeleton radius and axis length */
    void get_defaults ( float& sradius, float& alen );
 };


//================================ End of File =================================================

# endif  // SK_SCENE_H
