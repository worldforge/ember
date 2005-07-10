/***************************************************************************
	OgrePagingLandScapeTile.h  -  description
  -------------------
  begin                : Sun Jun 08 2003
  copyright            : (C) 2003-2005 by Jose A. Milan and Tuan Kuranes
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

#ifndef PAGINGLANDSCAPETILE_H
#define PAGINGLANDSCAPETILE_H

#include "OgrePagingLandScapePrerequisites.h"
#include "OgrePagingLandScapeTileInfo.h"

namespace Ogre
{

class PagingLandScapeTile 
{
public:
    PagingLandScapeTileInfo* getInfo( void ) 
    {
        return &mInfo;
    };

    /** Sets the appropriate neighbor for this TerrainRenderable.  Neighbors are necessary
    to know when to bridge between LODs.
    */
    void _setNeighbor( Neighbor n, PagingLandScapeTile* t );

    /** Returns the neighbor TerrainRenderable.
    */
    PagingLandScapeTile* _getNeighbor( Neighbor n )
    {
        return mNeighbors[ n ];
    };

    /** intersectSegment 
    @remarks
        Intersect mainly with Landscape
    @param start 
        beginning of the segment 
    @param end 
        where it ends
    @param result 
        where it intersects with terrain
    */
    bool intersectSegment( const Vector3& start, const Vector3& dir, Vector3* result );

    /** updateTerrain 
    @remarks
        Make the Tile reload its vertices and normals 
        (upon a modification of the height data)
    */
    void updateTerrain( void );
  
    PagingLandScapeRenderable* getRenderable( void )
    {
        return mRenderable;    
    }

    void _linkRenderableNeighbor( void );
    void _updateLod( void );

	PagingLandScapeTile( void );

	~PagingLandScapeTile( void );

	void init( SceneNode* PageNode, const int tableX, const int tableZ, const int tileX, const int tileZ );
    void load( void );
    void unload( void );
	void uninit( void );

	void _Notify( const Vector3& pos, PagingLandScapeCamera* Cam );

    SceneNode* getTileNode( void )
	{
		return mTileSceneNode;
	};

    bool isLoaded( void )
	{
		return mLoaded;
	};

    void setInUse( bool InUse );

    bool isVisible( void )
	{
		return mVisible;
	}

    void setRenderQueueGroup( RenderQueueGroupID qid );

protected:
	//movable object variables
    AxisAlignedBox mWorldBounds;
    AxisAlignedBox mWorldBoundsExt;
    Vector3 mWorldPosition;

	// if the tile is initialized
	bool mInit;
	// if the renderable is loaded
	bool mLoaded;	

	PagingLandScapeRenderable* mRenderable;

	SceneNode* mTileSceneNode;

    PagingLandScapeTile* mNeighbors[4];

	PagingLandScapeTileInfo mInfo;
    bool mVisible;

};

} //namespace

#endif
