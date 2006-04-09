/*
-----------------------------------------------------------------------------
This source file is part of OGRE
(Object-oriented Graphics Rendering Engine)
For the latest info, see http://www.ogre3d.org/

Copyright (c) 2000-2005 The OGRE Team
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
PagingLandScapeOctreecamera.cpp  -  description
-------------------
begin                : Fri Sep 27 2002
copyright            : (C) 2002 by Jon Anderson
email                : janders@users.sf.net

Enhancements 2003 - 2004 (C) The OGRE Team

***************************************************************************/
#include "OgreMath.h"
#include "OgreAxisAlignedBox.h"
#include "OgreRoot.h"
#include "OgreViewport.h"
#include "OgreRenderSystem.h"

#include "OgrePagingLandScapeOctreeCamera.h"
#include "OgrePagingLandScapeOctreeSceneManager.h"

namespace Ogre
{
    unsigned int PagingLandScapeOctreeCamera::s_camId = 0;
    //-----------------------------------------------------------------------
    PagingLandScapeOctreeCamera::PagingLandScapeOctreeCamera(const String& name, SceneManager* sm) : 
            Camera(name, sm),
            mVisFacesLastCHCRender(0),
            isOcclusionSystemRegistered (false),
            mOcclusionMode (VIEW_FRUSTUM_DIRECT),
            mFrameId(0),
            mFrameSceneId(0)
    {
       mUniqueIdentification = s_camId++;      
       updateRegistrationInOcclusionSystem ();
    }
    //-----------------------------------------------------------------------
    PagingLandScapeOctreeCamera::~PagingLandScapeOctreeCamera()
    {
    } 
    //-----------------------------------------------------------------------
    bool PagingLandScapeOctreeCamera::nextFrame(const unsigned int framesSceneId)
    {
        if  (framesSceneId != mFrameSceneId)
        {  
            // change frame Id counter 
            //that identify current frame.
            mFrameSceneId = framesSceneId;
            mFrameId += 1;
            return true;
        }
        return false;
    }
    //----------------------------------------------------------------------- 
    bool PagingLandScapeOctreeCamera::isRegisteredInOcclusionSystem() const
    {
        return isOcclusionSystemRegistered;
    }
    //-----------------------------------------------------------------------
    void PagingLandScapeOctreeCamera::setRegisteredInOcclusionSystem(const bool registered)
    {
        isOcclusionSystemRegistered = registered;
	}
	//-----------------------------------------------------------------------
	void PagingLandScapeOctreeCamera::changeOcclusionMode(culling_modes nextOcclusionMode)
	{
		if (nextOcclusionMode != mOcclusionMode) 
		{
			if (nextOcclusionMode == VIEW_FRUSTUM_DIRECT)
			{
				if (mLastViewport)
					mLastViewport->setClearEveryFrame (true);                
			}
			else
			{            
				if (!getSceneManager ()->getDestinationRenderSystem ()->getCapabilities()->hasCapability(RSC_HWOCCLUSION))
				{
					OGRE_EXCEPT(1, 
						"Your card does not support HWOCCLUSION ", 
						"PagingLandScapeOctreeCamera::setNextOcclusionMode");
				}
				if (mLastViewport)
					mLastViewport->setClearEveryFrame (false); 
			}

            mOcclusionMode = nextOcclusionMode;
			updateRegistrationInOcclusionSystem();
		}
    }
    //-----------------------------------------------------------------------
    void PagingLandScapeOctreeCamera::updateRegistrationInOcclusionSystem()
    {
        if (needRegistrationInOcclusionSystem() && !isRegisteredInOcclusionSystem()) 
        {
            static_cast <PagingLandScapeOctreeSceneManager *> (getSceneManager ())->registerCamera(this);
            setRegisteredInOcclusionSystem (true);				
        }
        else if (!needRegistrationInOcclusionSystem() && isRegisteredInOcclusionSystem())
        {	
            static_cast <PagingLandScapeOctreeSceneManager *> (getSceneManager ())->unregisterCamera(this);
            setRegisteredInOcclusionSystem (false);
        }
    }
    //-----------------------------------------------------------------------
    void PagingLandScapeOctreeCamera::setNextOcclusionMode()
    {
        culling_modes nextOcclusionMode = static_cast <culling_modes> ((mOcclusionMode + 1) % NUM_CULLING_MODE);
        
        changeOcclusionMode (nextOcclusionMode);
	}
	//-----------------------------------------------------------------------
	bool PagingLandScapeOctreeCamera::needRegistrationInOcclusionSystem() const
	{
		#ifdef _VISIBILITYDEBUG	
			return true;
		#endif //_VISIBILITYDEBUG   
		
		if (mOcclusionMode == CHC || 
			mOcclusionMode == CHC_CONSERVATIVE)
			return true;
		else
			return false;
	};
	//-----------------------------------------------------------------------
	void PagingLandScapeOctreeCamera::setOcclusionMode(culling_modes occlusionMode) 
	{
		changeOcclusionMode (occlusionMode);
	};
	//-----------------------------------------------------------------------
	void PagingLandScapeOctreeCamera::setOcclusionModeAsString(const String &cullingModeAsString) 
	{
		culling_modes cullingmode (VIEW_FRUSTUM_DIRECT);
		if (cullingModeAsString == "VIEW_FRUSTUM_DIRECT")
			cullingmode = VIEW_FRUSTUM_DIRECT;
		else if (cullingModeAsString == "CHC")
			cullingmode = CHC;
		else if (cullingModeAsString == "CHC_CONSERVATIVE")
			cullingmode = CHC_CONSERVATIVE;
		setOcclusionMode(cullingmode);	
	};
	//-----------------------------------------------------------------------
	String PagingLandScapeOctreeCamera::getOcclusionModeAsString() const
	{
		switch (getOcclusionMode())
		{
		case CHC: return String("CHC");
		case CHC_CONSERVATIVE: return String("CHC_CONSERVATIVE");
		case VIEW_FRUSTUM_DIRECT:
		default:
			return String("VIEW_FRUSTUM_DIRECT");
		} 
	}
    //-----------------------------------------------------------------------
    PagingLandScapeOctreeCamera::Visibility PagingLandScapeOctreeCamera::getVisibility(const AxisAlignedBox &bound) const 
    {
        // Null boxes always invisible
        if (bound.isNull())
            return NONE;

        // Make any pending updates to the calculated frustum
        Camera::updateView();

        // Get corners of the box
        const Vector3 * const pCorners = bound.getAllCorners();

        // For each plane, see if all points are on the negative side
        // If so, object is not visible.
        // If one or more are, it's partial.
        // If all aren't, full

        static unsigned int corners[ 8 ] = {0, 4, 3, 5, 2, 6, 1, 7};

        static unsigned int planes[ 6 ] = 
                        {FRUSTUM_PLANE_TOP, FRUSTUM_PLANE_BOTTOM,
                         FRUSTUM_PLANE_LEFT, FRUSTUM_PLANE_RIGHT,
                         FRUSTUM_PLANE_FAR, FRUSTUM_PLANE_NEAR };

        bool all_inside = true;

        const bool infinite_far_clip = (mFarDist == 0);
        for (unsigned int plane = 0; plane < 6; ++plane)
        {
            const unsigned int currPlane = planes[ plane ];

            // Skip far plane if infinite view frustum
            if (infinite_far_clip && currPlane == FRUSTUM_PLANE_FAR)
                continue;

            bool all_outside = true;
            const Plane &frustumPlane = mFrustumPlanes[ currPlane ];
            for (unsigned int corner = 0; corner < 8; ++corner)
            {
                const Real distance = frustumPlane.getDistance(pCorners[ corners[ corner ] ]);
               
                all_outside = all_outside && (distance < 0);
                all_inside = all_inside && (distance >= 0);

                if (!all_outside && !all_inside)
                    break;
            }

            if (all_outside)
                return NONE;
        }

        if (all_inside)
            return FULL;
        else
            return PARTIAL;

    }

}




