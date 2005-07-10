/***************************************************************************
OgrePagingLandScapeRaySceneQuery.cpp  -  description
-------------------
begin                : Fri Sep 10 2003
copyright            : (C) 2003-2005 by Jose A Milan && Tuan Kuranes
email                : spoke2@supercable.es && tuan.kuranes@free.fr
***************************************************************************/

/***************************************************************************
*                                                                         *
*   This program is free software; you can redistribute it and/or modify  *
*   it under the terms of the GNU Lesser General Public License as        *
*   published by the Free Software Foundation; either version 2 of the    *
*   License, or (at your option) any later version.                       *
*                                                                         *
***************************************************************************/

#include "OgreEntity.h"

#include "OgreVector3.h"
#include "OgreColourValue.h"

#include "OgreSceneManager.h"
#include "OgrePagingLandScapeOctreeSceneManager.h"

#include "OgrePagingLandScapeOptions.h"
#include "OgrePagingLandScapeSceneManager.h"
#include "OgrePagingLandScapeData2DManager.h"
#include "OgrePagingLandScapeRaySceneQuery.h"

namespace Ogre
{

//----------------------------------------------------------------------------
// This function return the vertex interpolated height.
// Supplied by Praetor. Thanks a lot. ]:)
void PagingLandScapeRaySceneQuery::execute(RaySceneQueryListener* listener) 
{ 
    ulong mask = getQueryMask();  
   
    
    static WorldFragment worldFrag;
    worldFrag.fragmentType = SceneQuery::WFT_SINGLE_INTERSECTION;

    const Vector3& dir = mRay.getDirection();
    const Vector3& origin = mRay.getOrigin();

    bool noSpecializedQueries = true;
    bool noTerrainQuery = true;

    // Exclusive Specialized Queries
    // Straight up / down?
    if ((mask & RSQ_Height) || (mask & RSQ_Height_no_interpolation) ||
        dir == Vector3::UNIT_Y || 
        dir == Vector3::NEGATIVE_UNIT_Y)
    {
        noTerrainQuery = false;
        if (mask & RSQ_Height || mask & RSQ_Height_no_interpolation)
            noSpecializedQueries = false;

        Real height;
        if (mask & RSQ_Height_no_interpolation)
            height = PagingLandScapeData2DManager::getSingleton().getWorldHeight(origin.x, origin.z);
        else
            height = PagingLandScapeData2DManager::getSingleton().getRealWorldHeight(origin.x, origin.z);

        worldFrag.singleIntersection.x = origin.x;
        worldFrag.singleIntersection.z = origin.z;
        worldFrag.singleIntersection.y = height;

        //if (!listener->queryResult(&worldFrag,  (worldFrag.singleIntersection - origin).length()))					
        //if (!listener->queryResult(&worldFrag,  0))
		//			return;
        listener->queryResult(&worldFrag,  0);
    } 
    else if (mask & RSQ_FirstTerrain)
    {
        noTerrainQuery = false;
        noSpecializedQueries = false;

        Real resFactor = 1.0f;
		// Only bother if the non-default mask has been set
		if((mask & RSQ_1xRes) == 0)
		{
			if(mask & RSQ_2xRes)
            {
				resFactor = 0.5f;
			} 
            else if(mask & RSQ_4xRes) 
            {
				resFactor = 0.25f;
			} 
            else if(mask & RSQ_8xRes) 
            {
				resFactor = 0.125f;
			}
		}
        if (static_cast<PagingLandScapeSceneManager*>(mParentSceneMgr)->intersectSegmentTerrain(
                        origin, dir*resFactor, 
                        &worldFrag.singleIntersection))
        {
            //if (!listener->queryResult(&worldFrag,  (worldFrag.singleIntersection - origin).length()))					
            //if (!listener->queryResult(&worldFrag,  0))
			//		    return;
            listener->queryResult(&worldFrag,  0);
            return;
        }
    }  
    else if((mask & RSQ_AllTerrain))
	{
        noTerrainQuery = false;
        noSpecializedQueries = false;

       	Real resFactor = 1.0f;
		// Only bother if the non-default mask has been set
		if((mask & RSQ_1xRes) == 0)
		{
			if(mask & RSQ_2xRes)
            {
				resFactor = 0.5f;
			} 
            else if(mask & RSQ_4xRes) 
            {
				resFactor = 0.25f;
			} 
            else if(mask & RSQ_8xRes) 
            {
				resFactor = 0.125f;
			}
		}


        const Vector3 raydir (mRay.getDirection());
        const Vector3 raymove (raydir * resFactor);
        const Real distmove = 1 * resFactor;
        const Real maxHeight = PagingLandScapeData2DManager::getSingleton().getMaxHeight ();
        const Real MaxTerrainX = PagingLandScapeOptions::getSingleton().maxScaledX;
        const Real MaxTerrainZ = PagingLandScapeOptions::getSingleton().maxScaledZ;

        Vector3 ray (mRay.getOrigin());
	    Real dist = 0.0f;

        // while ray is inside or ray is outside but raydir going inside
        while ( (ray.y < 0 && raydir.y > 0) || 
                (ray.y > maxHeight    && raydir.y < 0) || 
                (ray.x < -MaxTerrainX && raydir.x > 0) || 
                (ray.x > MaxTerrainX  && raydir.x < 0) || 
                (ray.z < -MaxTerrainZ && raydir.z > 0) || 
                (ray.z > MaxTerrainZ  && raydir.z < 0)) 
		{
			ray += raymove;
			dist += distmove;
            if (ray.y < maxHeight)// no need to do complex tests
            {
                const Vector3 land (getHeightAt(ray));
                if (ray.y < land.y)
                {
                    WorldFragment* frag = new WorldFragment();
                    //fragmentList.push_back(frag);

                    frag->fragmentType = SceneQuery::WFT_SINGLE_INTERSECTION; 
                    frag->singleIntersection = land;
                    			
                    if (!listener->queryResult(frag,  dist))
					            return;
                }
            }
        } 
    }

    // Could want entities and terrain or or just entities, or didn't say nothing...
    if(mask & RSQ_Entities)
    {
        noSpecializedQueries = false;
        // Check for entity contacts
        PagingLandScapeOctreeRaySceneQuery::execute(listener);
    }

    if (noSpecializedQueries)
    {
        if (noTerrainQuery)
        {
            mask |= RSQ_AllTerrain;
            // Check for terrain contacts
            PagingLandScapeRaySceneQuery::execute(listener);
            mask = 0;
        }
        // Check for entity contacts
        PagingLandScapeOctreeRaySceneQuery::execute(listener);
    }
} 
//----------------------------------------------------------------------------
Vector3 PagingLandScapeRaySceneQuery::getHeightAt(const Vector3& origin) const
{

	return Vector3(origin.x, PagingLandScapeData2DManager::getSingleton().getRealWorldHeight(origin.x, origin.z), origin.z);

}

} // namespace Ogre
