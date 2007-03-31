/***************************************************************************
OgrePagingLandScapeRaySceneQuery.cpp  -  description
-------------------
begin                : Fri Sep 10 2003
copyright            : (C) 2003-2006 by Jose A Milan && Tuan Kuranes
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

#include "OgrePagingLandScapePrecompiledHeaders.h"

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
	///Ember start
	///Make sure that there really is some world geometry first
    if ((static_cast<PagingLandScapeSceneManager*>(mParentSceneMgr)->mWorldGeomIsSetup) && (getQueryTypeMask() & SceneManager::WORLD_GEOMETRY_TYPE_MASK))
	///Ember end
	{
		mWorldFrag.fragmentType = SceneQuery::WFT_SINGLE_INTERSECTION;

		const Vector3& dir = mRay.getDirection();
		const Vector3& origin = mRay.getOrigin();

		PagingLandScapeSceneManager* mSceneMgr = static_cast<PagingLandScapeSceneManager*>(mParentSceneMgr);
		if (mWorldFragmentType & WFT_SINGLE_INTERSECTION)
		{
			if (dir == Vector3::UNIT_Y || 
				dir == Vector3::NEGATIVE_UNIT_Y)
			{
				Real height;
				if (mSceneMgr->getOptions()->queryNoInterpolation)
					height = mSceneMgr->getData2DManager()->getWorldHeight(origin.x, origin.z);
				else
					height = mSceneMgr->getData2DManager()->getInterpolatedWorldHeight(origin.x, origin.z);

				mWorldFrag.singleIntersection.x = origin.x;
				mWorldFrag.singleIntersection.z = origin.z;
				mWorldFrag.singleIntersection.y = height;

				mWorldFrag.singleIntersection += mSceneMgr->getOptions()->position; //consider terrain offset

				listener->queryResult(&mWorldFrag, (Math::Abs(mWorldFrag.singleIntersection.y - origin.y)));
				return;
			}
			else if (mSceneMgr->intersectSegmentTerrain(
					origin, 
					dir * mSceneMgr->getOptions()->queryResolutionFactor, 
					&mWorldFrag.singleIntersection))
			{
                listener->queryResult(&mWorldFrag, (mWorldFrag.singleIntersection - origin).length());
                ///Ember start
                PagingLandScapeOctreeRaySceneQuery::execute(listener);
                ///Ember end
                return;
			}
		}
		else
		{
			// multiple terrain intersection
			const Vector3 raydir (mRay.getDirection());
			const Vector3 raymove (raydir * mSceneMgr->getOptions()->queryResolutionFactor);
			const Real distmove = mSceneMgr->getOptions()->queryResolutionFactor;
			const Real maxHeight = mSceneMgr->getData2DManager()->getMaxHeight ();
			const Real MaxTerrainX = mSceneMgr->getOptions()->maxScaledX;
			const Real MaxTerrainZ = mSceneMgr->getOptions()->maxScaledZ;

			Vector3 ray (mRay.getOrigin());
			Real dist = 0.0f;

			// while ray is inside or ray is outside but raydir going inside
			while ((ray.y < 0 && raydir.y > 0) || 
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

	}
    // if anything else is queried, ask underlying Octree Scene Manager.
    PagingLandScapeOctreeRaySceneQuery::execute(listener);
} 
//----------------------------------------------------------------------------
Vector3 PagingLandScapeRaySceneQuery::getHeightAt(const Vector3& origin) const
{

    PagingLandScapeSceneManager * mSceneMgr = static_cast<PagingLandScapeSceneManager*>(mParentSceneMgr);
	return Vector3(origin.x, mSceneMgr->getData2DManager()->getInterpolatedWorldHeight(origin.x, origin.z), origin.z);

}

} // namespace Ogre
