/***************************************************************************
OgrePagingLandScapeRaySceneQuery.h  -  description
-------------------
begin                : Fri Sep 10 2003
copyright            : (C) 2003-2006 by Jose A Milan && Tuan Kuranes
email                : spoke2@supercable.es && tuan.kuranes@free.fr
***************************************************************************/

/***************************************************************************
*                                                                         *
*   This program is free software; you can redistribute it and/or modify  *
*   it under the terms of the GNU General Public License as        *
*   published by the Free Software Foundation; either version 2 of the    *
*   License, or (at your option) any later version.                       *
*                                                                         *
***************************************************************************/

#ifndef PAGINGLandScapeRAYSCENEQUERY_H
#define PAGINGLandScapeRAYSCENEQUERY_H

#include "OgrePagingLandScapePrerequisites.h"
#include "OgrePagingLandScapeOctreeRaySceneQuery.h"

namespace Ogre
{


	/** PagingLandScape's specialisation of RaySceneQuery.
		if RSQ_Height bit mask is set, RSQ_Terrain and RSQ_Entity bits will be ignored
		Otherwise data will be returned based on the mask
	*/
	class PagingLandScapeRaySceneQuery : public PagingLandScapeOctreeRaySceneQuery
	{

	    public:
		    PagingLandScapeRaySceneQuery(SceneManager* creator) : PagingLandScapeOctreeRaySceneQuery(creator)
		    {
			    mSupportedWorldFragments.insert(SceneQuery::WFT_SINGLE_INTERSECTION);
                //mLastResult = new RaySceneQueryResult();
		    }

//		    virtual ~PagingLandScapeRaySceneQuery(void)
//		    {
//			    clearFragmentList();
//                //delete mLastResult;
//                //mLastResult = 0;
//		    }

		    /** See RaySceneQuery. */
		    void execute(RaySceneQueryListener * listener);
            /** Executes the query, returning the results back in one list.
            @remarks
                This method executes the scene query as configured, gathers the results
                into one structure and returns a reference to that structure. These
                results will also persist in this query object until the next query is
                executed, or clearResults() is called. An more lightweight version of
                this method that returns results through a listener is also available.
            */
            //virtual RaySceneQueryResult& execute(void);

            //void clearResults(void);

	    protected:
		    //JEFF
		    //PagingLandScapeTile* getNonDividedTile(PagingLandScapeTile* tile, const Vector3& origin);
		    inline Vector3 getHeightAt(const Vector3& origin) const;

		    //std::vector< SceneQuery::WorldFragment* > fragmentList;

//			void clearFragmentList(void)
//		    {
//			    std::vector< SceneQuery::WorldFragment* >::iterator cur, end = fragmentList.end();
//			    for (cur = fragmentList.begin(); cur < end; ++cur)
//			    {
//				    SceneQuery::WorldFragment* frag = (*cur);
//				    if (frag)
//				    {
//					    delete frag;
//					    frag = 0;
//				    }
//			    }
//			    fragmentList.clear();
			//		    }

	private:
		WorldFragment mWorldFrag;

	};

} // namespace Ogre


#endif
