/***************************************************************************
	OgrePagingLandScapeTile.h  -  description
  -------------------
  begin                : Sun Jun 08 2003
  copyright            : (C) 2003-2006 by Jose A. Milan and Tuan Kuranes
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

#ifndef PAGINGLandScapeTILE_H
#define PAGINGLandScapeTILE_H

#include "OgrePagingLandScapePrerequisites.h"
#include "OgrePagingLandScapeTileInfo.h"

#include "OgrePagingLandScapeOptions.h"
#include "OgrePagingLandScapeTileManager.h"
#include "OgrePagingLandScapeRenderable.h"

namespace Ogre
{

class _OgrePagingLandScapeExport PagingLandScapeTile 
{
public:
    inline PagingLandScapeTileInfo* getInfo(void) 
    {
        return mInfo;
    };

    /** Sets the appropriate neighbor for this TerrainRenderable.  Neighbors are necessary
    to know when to bridge between LODs.
    */
    void _setNeighbor(Neighbor n, PagingLandScapeTile* t);

    /** Returns the neighbor TerrainRenderable.
    */
    inline PagingLandScapeTile* _getNeighbor(Neighbor n)
    {
        return mNeighbors[ n ];
    };

    /** intersectSegment 
    @remarks
        Intersect mainly with LandScape
    @param start 
        beginning of the segment 
    @param end 
        where it ends
    @param result 
        where it intersects with terrain
    */
    bool intersectSegmentFromAbove(const Vector3& start, const Vector3& dir, Vector3* result);
	bool intersectSegmentFromBelow(const Vector3& start, const Vector3& dir, Vector3* result);

    /** updateTerrain 
    @remarks
        Make the Tile reload its vertices and normals 
        (upon a modification of the height data)
    */
    void updateTerrain(void);
  
    inline PagingLandScapeRenderable* getRenderable(void)
    {
        return mRenderable;    
    }

    void _linkRenderableNeighbor(void);
    void _updateLod(void);

	PagingLandScapeTile(PagingLandScapeTileManager *pageMgr);

	~PagingLandScapeTile(void);

	void init(SceneNode* PageNode, const int tableX, const int tableZ, const int tileX, const int tileZ);
    void load(void);
    void unload(void);
	void uninit(void);

	void _Notify(const Vector3& pos, PagingLandScapeCamera* Cam);

    inline bool isLoaded(void)
	{
		return mLoaded;
	};
	inline bool isLoading(void)
	{
		return mLoading;
	};
	inline void setLoading(bool value)
	{
		mLoading = value;
	};

	void PagingLandScapeTile::setInUse (bool InUse)
	{
		assert (mInit);
		if (mRenderable)
			mRenderable->setInUse (InUse);
	}

    inline bool isVisible(void)
	{
		return mVisible;
	}

	/// make tile visible not being unload until a certain time.
	inline void touch ()
	{ 
		mTimePreLoaded = mParent->getOptions()->TileInvisibleUnloadFrames;
	}

    void setRenderQueueGroup(RenderQueueGroupID qid);

	SceneNode *getSceneNode()
	{
		return mTileSceneNode;
	};
	const AxisAlignedBox &getWorldBbox() const 
	{
		return mWorldBounds;
	};
	const Vector3 &getCenter(void) const 
	{
		return mWorldPosition;
	};

	const bool touched ();

protected:
	//movable object variables
    AxisAlignedBox mWorldBounds;
    AxisAlignedBox mWorldBoundsExt;
    Vector3 mWorldPosition;

	// if the tile is initialized
	bool mInit;
	// if the renderable is loaded
	bool mLoaded;	
	// if the renderable is loading
	bool mLoading;	

	PagingLandScapeRenderable* mRenderable;

	SceneNode* mTileSceneNode;
    SceneNode* mParentSceneNode;

    PagingLandScapeTile* mNeighbors[4];

	PagingLandScapeTileInfo *mInfo;
	bool mVisible;
	unsigned int mTimePreLoaded;

    PagingLandScapeTileManager *mParent;
};

} //namespace

#endif
