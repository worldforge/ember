/*
-----------------------------------------------------------------------------
This source file is part of OGRE
(Object-oriented Graphics Rendering Engine)
For the latest info, see http://www.ogre3d.org/

Copyright © 2000-2004 The OGRE Team
Also see acknowledgements in Readme.html

This program is free software; you can redistribute it and/or modify it under
the terms of the GNU Lesser General Public License as published by the Free Software
Foundation; either version 2 of the License, or (at your option) any later
version.

This program is distributed in the hope that it will be useful, but WITHOUT
ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License along with
this program; if not, write to the Free Software Foundation, Inc., 59 Temple
Place - Suite 330, Boston, MA 02111-1307, USA, or go to
http://www.gnu.org/copyleft/lesser.txt.
-----------------------------------------------------------------------------
*/
/***************************************************************************
PagingLandScapeCamera.cpp  -  description
-------------------
begin                : Fri Sep 27 2002
copyright            : (C) 2002 by Jon Anderson
email                : janders@users.sf.net

Enhancements 2003 - 2004 (C) The OGRE Team

***************************************************************************/


#include "OgreCamera.h"

#include "OgrePagingLandScapeCamera.h"
#include "OgrePagingLandScapeOptions.h"
#include "OgrePagingLandScapeSceneManager.h"

namespace Ogre
{
    //-----------------------------------------------------------------------
    PagingLandScapeCamera::PagingLandScapeCamera( const String& name, SceneManager* sm ) : PagingLandScapeOctreeCamera( name, sm ),
        

        mCurrentCameraPageX(0), 
        mCurrentCameraPageZ(0),
        mCurrentCameraTileX(0), 
        mCurrentCameraTileZ(0),
        mIniX(0),
        mFinX(0),
		mIniZ(0),
        mFinZ(0),
        mPreIniX(0),
        mPreFinX(0),
        mPreIniZ(0),
        mPreFinZ(0),
	    mLastCameraPos (Vector3(std::numeric_limits<Real>::min (), 
                                0.0f, 
                                std::numeric_limits<Real>::min ()))
    {
        mLastViewport = 0;
    }
    //-----------------------------------------------------------------------
    PagingLandScapeCamera::~PagingLandScapeCamera()
    {  
       
    }
    //-----------------------------------------------------------------------
    void PagingLandScapeCamera::resetPaging()
    {
        mCurrentCameraPageX = 0; 
        mCurrentCameraPageZ = 0;
        mCurrentCameraTileX = 0; 
        mCurrentCameraTileZ = 0;
        mIniX = 0;
        mFinX = 0;
		mIniZ = 0;
        mFinZ = 0;
        mPreIniX = 0;
        mPreFinX = 0;
        mPreIniZ = 0;
        mPreFinZ = 0;
        mLastCameraPos = Vector3 (std::numeric_limits<Real>::min (), 
                                  0.0f, 
                                  std::numeric_limits<Real>::min ());
        updatePaging (0, 0);
//        Real dist = Camera::getDerivedPosition().squaredLength()
//            + PagingLandScapeOptions::getSingleton().cameraThreshold
//        mLastCameraPos.x = dist;
//        mLastCameraPos.y = dist;
//        mLastCameraPos.z = dist;
    }
    //-----------------------------------------------------------------------
    void PagingLandScapeCamera::updatePaging (const uint x, const uint z)
    {
        // We must load the next visible landscape pages, 
        // check the landscape boundaries	

        mCurrentCameraPageX = x;
        mCurrentCameraPageZ = z;
        const PagingLandScapeOptions * const opt = PagingLandScapeOptions::getSingletonPtr();
        const uint w = opt->world_width;
        const uint h = opt->world_height;
        const uint adjpages = opt->max_adjacent_pages;
        const uint prepages = opt->max_preload_pages;

        // Load Windowing
        uint lx = x;
        if (static_cast<int> (x - adjpages) >= 0)
        {
            mIniX = x - adjpages;
        }
        else
        {
            mIniX = 0;
            lx -= x - adjpages;
        }
        if (lx + adjpages < w )
        {
            mFinX = lx + adjpages;
        }
        else
        {
            mFinX = w - 1;
            mIniX = (static_cast <int> (mIniX + (w - (lx + adjpages))) > 0)? mIniX + (w - (lx + adjpages)):0;
        }

        uint lz = z;
        if (static_cast<int> (z - adjpages) >= 0)
        {
            mIniZ = z - adjpages;
        }
        else
        {
            mIniZ = 0;
            lz -= z - adjpages;
        }
        if (lz + adjpages < h )
        {
            mFinZ = lz + adjpages;
        }
        else
        {
            mFinZ = h - 1;
            mIniZ = (static_cast <int> (mIniZ + (h - (lz + adjpages))) > 0)? mIniZ + (h - (lz + adjpages)):0;
        }

        // Pre-load Windowing
        lx = x;
        if (static_cast<int> (x - prepages) > 0)
        {
            mPreIniX = x - prepages;
        }
        else
        {
            mPreIniX = 0;
            lx -= x - prepages;
        }
        if (x + prepages < w )
        {
            mPreFinX = x + prepages;
        }
        else
        {
            mPreFinX = w - 1;
            mPreIniX = (static_cast <int> (mPreIniX + (w - (lx + prepages))) > 0)? mPreIniX + (w - (lx + prepages)):0;
        }

        lz = z;
        if (static_cast<int> (z - prepages) > 0)
        {
            mPreIniZ = z - prepages;
        }
        else
        {
            mPreIniZ = 0;
            lz -= z - prepages;
        }
        if (lz + prepages < h )
        {
            mPreFinZ = lz + prepages;
        }
        else
        {
            mPreFinZ = h - 1;
            mPreIniZ = (static_cast <int> (mPreIniZ + (h - (lz + prepages))) > 0)? mPreIniZ  + (h - (lz + prepages)):0;
        }
        //(static_cast <PagingLandScapeSceneManager*> (getSceneManager())->resize());
    }
    //-----------------------------------------------------------------------
    bool PagingLandScapeCamera::getVisibility( const AxisAlignedBox &bound ) const
    {
        // Null boxes always invisible
        if ( bound.isNull() )
            return false;

        // Make any pending updates to the calculated frustum
        Camera::updateView();

        // Get corners of the box
        const Vector3 * const pCorners = bound.getAllCorners();

        // For each plane, see if all points are on the negative side
        // If so, object is not visible.
        // If one or more are, it's partial.
        // If all aren't, full

        static const unsigned int corners[ 8 ] = {0, 4, 3, 5, 2, 6, 1, 7};

        static const unsigned int planes[ 6 ] = {FRUSTUM_PLANE_TOP, FRUSTUM_PLANE_BOTTOM,
                                FRUSTUM_PLANE_LEFT, FRUSTUM_PLANE_RIGHT,
                                FRUSTUM_PLANE_FAR, FRUSTUM_PLANE_NEAR };

        bool all_inside = true;
        const bool infinite_far_clip = (mFarDist == 0);
        for ( unsigned int plane = 0; plane < 6; ++plane )
        {

            const unsigned int currPlane = planes[ plane ];
            // Skip far plane if infinite view frustum
            if (infinite_far_clip && currPlane == FRUSTUM_PLANE_FAR)
                continue;

            bool all_outside = true;
            const Plane &frustumPlane = mFrustumPlanes[ currPlane ];
            for ( unsigned int corner = 0; corner < 8; ++corner )
            {
                const Real distance = frustumPlane.getDistance( pCorners[ corners[ corner ] ] );

                all_outside = all_outside && ( distance < 0 );
                all_inside = all_inside && ( distance >= 0 );

                if ( !all_outside && !all_inside )
                    break;
            }

            if ( all_outside )
                return false;
        }
        return true;
    }
}




