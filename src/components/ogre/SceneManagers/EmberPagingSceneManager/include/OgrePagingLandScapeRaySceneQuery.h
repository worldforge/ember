/***************************************************************************
OgrePagingLandScapeRaySceneQuery.h  -  description
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

#ifndef PAGINGLANDSCAPERAYSCENEQUERY_H
#define PAGINGLANDSCAPERAYSCENEQUERY_H

#include "OgrePagingLandScapePrerequisites.h"
#include "OgrePagingLandScapeOctreeRaySceneQuery.h"

namespace Ogre
{

	enum
	{
		// Height will return the height at the origin
		// Distance will always be 0
		RSQ_Height       = 1<<0,

		// AllTerrain will return all terrain contacts
		// along the ray
		RSQ_AllTerrain   = 1<<1,

		// FirstTerrain will return only the first
		// contact along the ray
		RSQ_FirstTerrain = 1<<2,

		// Entities will return all entity contacts along the ray
		RSQ_Entities     = 1<<3,

		// Different resolution scales.  It defaults to 1 unit
		// resolution.  2x resolution tests every 0.5 units
		// 4x tests every 0.25 and 8x every 0.125
		RSQ_8xRes        = 1<<4,
		RSQ_4xRes        = 1<<5,
		RSQ_2xRes        = 1<<6,
		RSQ_1xRes        = 1<<7,

		// Height will return the height at the origin
		// Distance will always be 0
        // without interpolation = Much faster but
        // wrong between points.
        RSQ_Height_no_interpolation =  1<<8,
	};

	/** PagingLandScape's specialisation of RaySceneQuery.
		if RSQ_Height bit mask is set, RSQ_Terrain and RSQ_Entity bits will be ignored
		Otherwise data will be returned based on the mask
	*/
	class PagingLandScapeRaySceneQuery : public PagingLandScapeOctreeRaySceneQuery
	{
   
	    public:
		    PagingLandScapeRaySceneQuery( SceneManager* creator ) : PagingLandScapeOctreeRaySceneQuery( creator )
		    { 
			    mSupportedWorldFragments.insert( SceneQuery::WFT_SINGLE_INTERSECTION );
                //mLastResult = new RaySceneQueryResult();
		    }

//		    virtual ~PagingLandScapeRaySceneQuery( void )
//		    {
//			    clearFragmentList( );
//                //delete mLastResult;
//                //mLastResult = 0;
//		    }

		    /** See RaySceneQuery. */
		    void execute( RaySceneQueryListener * listener );
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
		    inline Vector3 getHeightAt( const Vector3& origin ) const;

		    //std::vector< SceneQuery::WorldFragment* > fragmentList;
		    
//			void clearFragmentList( void )
//		    {
//			    std::vector< SceneQuery::WorldFragment* >::iterator cur, end = fragmentList.end( );
//			    for ( cur = fragmentList.begin( ); cur < end; ++cur )
//			    {
//				    SceneQuery::WorldFragment* frag = ( *cur );
//				    if ( frag )
//				    {
//					    delete frag;
//					    frag = 0;
//				    }
//			    }
//			    fragmentList.clear( ); 
//		    }

	    private:

	};

} // namespace Ogre


#endif
