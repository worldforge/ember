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
octreecamera.cpp  -  description
-------------------
begin                : Fri Sep 27 2002
copyright            : (C) 2002 by Jon Anderson
email                : janders@users.sf.net

Enhancements 2003 - 2004 (C) The OGRE Team

***************************************************************************/
#include <OgreMath.h>
#include <OgreAxisAlignedBox.h>
#include <OgreRoot.h>

#include <OgreOctreeCamera.h>

namespace Ogre
{
OctreeCamera::OctreeCamera( const String& name, SceneManager* sm ) : Camera( name, sm )
{
                                                                      
}

OctreeCamera::~OctreeCamera()
{
}

OctreeCamera::Visibility OctreeCamera::getVisibility( const AxisAlignedBox &bound )
{

    // Null boxes always invisible
    if ( bound.isNull() )
        return NONE;

    // Make any pending updates to the calculated frustum
    updateView();

    // Get corners of the box
    const Vector3* pCorners = bound.getAllCorners();

    // For each plane, see if all points are on the negative side
    // If so, object is not visible.
    // If one or more are, it's partial.
    // If all aren't, full

    int corners[ 8 ] = {0, 4, 3, 5, 2, 6, 1, 7};

    int planes[ 6 ] = {FRUSTUM_PLANE_TOP, FRUSTUM_PLANE_BOTTOM,
                       FRUSTUM_PLANE_LEFT, FRUSTUM_PLANE_RIGHT,
                       FRUSTUM_PLANE_FAR, FRUSTUM_PLANE_NEAR };

    bool all_inside = true;

    for ( int plane = 0; plane < 6; ++plane )
    {

        // Skip far plane if infinite view frustum
        if (mFarDist == 0 && planes[ plane ] == FRUSTUM_PLANE_FAR)
            continue;

        bool all_outside = true;

        float distance = 0;

        for ( int corner = 0; corner < 8; ++corner )
        {
            distance = mFrustumPlanes[ planes[ plane ] ].getDistance( pCorners[ corners[ corner ] ] );
            all_outside = all_outside && ( distance < 0 );
            all_inside = all_inside && ( distance >= 0 );

            if ( !all_outside && !all_inside )
                break;
        }

        if ( all_outside )
            return NONE;
    }

    if ( all_inside )
        return FULL;
    else
        return PARTIAL;

}

}




