/*
-----------------------------------------------------------------------------
This source file is part of OGRE
(Object-oriented Graphics Rendering Engine)
For the latest info, see http://www.ogre3d.org/
 
Copyright  2000-2005 The OGRE Team
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
OgreOctreeSceneQuery.h  -  description
-------------------
begin                : Tues July 20, 2004
copyright            : (C) 2004 by Jon Anderson
email                : janders@users.sf.net
***************************************************************************/

#ifndef OCTREESCENEQUERY_H
#define OCTREESCENEQUERY_H

#include "OgreTerrainPrerequisites.h"
#include <OgreSceneManager.h>


namespace Ogre
{
/** Octree implementation of IntersectionSceneQuery. */
class _OgreTerrainExport OctreeIntersectionSceneQuery :  public DefaultIntersectionSceneQuery
{
public:
    OctreeIntersectionSceneQuery(SceneManager* creator);
    ~OctreeIntersectionSceneQuery();

    /** See IntersectionSceneQuery. */
    void execute(IntersectionSceneQueryListener* listener);
};

/** Octree implementation of RaySceneQuery. */
class _OgreTerrainExport OctreeRaySceneQuery : public DefaultRaySceneQuery
{
public:
    OctreeRaySceneQuery(SceneManager* creator);
    ~OctreeRaySceneQuery();

    /** See RayScenQuery. */
    void execute(RaySceneQueryListener* listener);
};
/** Octree implementation of SphereSceneQuery. */
class _OgreTerrainExport OctreeSphereSceneQuery : public DefaultSphereSceneQuery
{
public:
    OctreeSphereSceneQuery(SceneManager* creator);
    ~OctreeSphereSceneQuery();

    /** See SceneQuery. */
    void execute(SceneQueryListener* listener);
};
/** Octree implementation of PlaneBoundedVolumeListSceneQuery. */
class _OgreTerrainExport OctreePlaneBoundedVolumeListSceneQuery : public DefaultPlaneBoundedVolumeListSceneQuery
{
public:
    OctreePlaneBoundedVolumeListSceneQuery(SceneManager* creator);
    ~OctreePlaneBoundedVolumeListSceneQuery();

    /** See SceneQuery. */
    void execute(SceneQueryListener* listener);
};
/** Octree implementation of AxisAlignedBoxSceneQuery. */
class _OgreTerrainExport OctreeAxisAlignedBoxSceneQuery : public DefaultAxisAlignedBoxSceneQuery
{
public:
    OctreeAxisAlignedBoxSceneQuery(SceneManager* creator);
    ~OctreeAxisAlignedBoxSceneQuery();

    /** See RayScenQuery. */
    void execute(SceneQueryListener* listener);
};


}

#endif


