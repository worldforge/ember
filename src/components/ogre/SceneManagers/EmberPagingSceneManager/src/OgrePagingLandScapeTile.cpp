/***************************************************************************
	OgrePagingLandScapeTile.cpp  -  description
	-------------------
	begin                : Sun Jun 08 2003
	copyright            : (C) 2003 by Jose A. Milan
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

#include "OgreVector3.h"
#include "OgreColourValue.h"

#include "OgreMovableObject.h"
#include "OgreAxisAlignedBox.h"

#include "OgreCamera.h"

#include "OgreStringConverter.h"

#include "OgreSceneNode.h"

#include "OgreSimpleRenderable.h"

#include "OgrePagingLandScapeOptions.h"
#include "OgrePagingLandScapeCamera.h"

#include "OgrePagingLandScapeTileManager.h"
#include "OgrePagingLandScapeTile.h"
#include "OgrePagingLandScapeTileInfo.h"

#include "OgrePagingLandScapeData2DManager.h"
#include "OgrePagingLandScapeData2D.h"

#include "OgrePagingLandScapePageManager.h"

#include "OgrePagingLandScapeRenderableManager.h"
#include "OgrePagingLandScapeRenderable.h"

#include "OgrePagingLandScapeTextureManager.h"

#include "OgrePagingLandScapeListener.h"   

namespace Ogre
{

//-----------------------------------------------------------------------
PagingLandScapeTile::PagingLandScapeTile()
{
	mTileSceneNode = 0;
	mRenderable = 0;
	mInit = false;
	mLoaded = false;
    for ( uint i = 0; i < 4; i++ )
    {
        mNeighbors[ i ] = 0;
    }
}

//-----------------------------------------------------------------------
PagingLandScapeTile::~PagingLandScapeTile( )
{
}
//-----------------------------------------------------------------------
void PagingLandScapeTile::uninit( void )
{
    if (mInit || mLoaded || mRenderable)
    {
        mInit = false;

        if (mLoaded)
            unload();

        if (mNeighbors[SOUTH])
            mNeighbors[SOUTH]->_setNeighbor (NORTH, 0);
        if (mNeighbors[NORTH])
            mNeighbors[NORTH]->_setNeighbor (SOUTH, 0);
        if (mNeighbors[EAST])
            mNeighbors[EAST]->_setNeighbor (WEST, 0);
        if (mNeighbors[WEST])
            mNeighbors[WEST]->_setNeighbor (EAST, 0);
        for ( uint i = 0; i < 4; i++ )
        {
            mNeighbors[ i ] = 0;
        }
		mTileSceneNode = 0;  
        PagingLandScapeTileManager::getSingleton().freeTile( this );
    }
	
   
}
//-----------------------------------------------------------------------
void PagingLandScapeTile::init( SceneNode *ParentSceneNode,
                               const int pageX, const int pageZ, 
                               const int tileX, const int tileZ )
{
	mInit = true;


	mInfo.pageX = pageX;
	mInfo.pageZ = pageZ;
	mInfo.tileX = tileX;
	mInfo.tileZ = tileZ;
	// Calculate the offset from the parent for this tile

    const Vector3 scale = PagingLandScapeOptions::getSingleton().scale;
    const Real endx = PagingLandScapeOptions::getSingleton().TileSize * scale.x;
    const Real endz = PagingLandScapeOptions::getSingleton().TileSize * scale.z;
	mInfo.posX = mInfo.tileX * endx;
	mInfo.posZ = mInfo.tileZ * endz;

    assert (ParentSceneNode);
    mTileSceneNode = ParentSceneNode ;

    const Real MaxHeight = PagingLandScapeData2DManager::getSingleton().getMaxHeight(mInfo.pageX, mInfo.pageZ);

    //Change Zone of this page

    const Vector3 ParentPos = mTileSceneNode->getWorldPosition();

    //tile center position
    mWorldPosition = ParentPos + Vector3 (mInfo.posX + endx * 0.5f, 
                                MaxHeight * 0.5f, 
                                mInfo.posZ + endz * 0.5f);


    mWorldBounds.setExtents(mInfo.posX + ParentPos.x , 
                            0, 
                            mInfo.posZ + ParentPos.z, 
                            mInfo.posX + ParentPos.x + endx, 
                            MaxHeight, 
                            mInfo.posZ + ParentPos.z + endz);



    mWorldBoundsExt.setExtents(mInfo.posX + ParentPos.x  - endx * 0.5f, 
                                                         - MaxHeight * 0.5f, 
                                mInfo.posZ + ParentPos.z - endz * 0.5f, 
                                mInfo.posX + ParentPos.x + endx * 1.5f, 
                                MaxHeight * 1.5f, 
                                mInfo.posZ + ParentPos.z + endz * 1.5f);

   

    for ( uint i = 0; i < 4; i++ )
    {
        mNeighbors[ i ] = 0;
    }
	//force update in scene node
    mLoaded = false;
}
//-----------------------------------------------------------------------
void PagingLandScapeTile::_setNeighbor( Neighbor n, PagingLandScapeTile *t )
{
    mNeighbors[ n ] = t;
    if (t && 
        mLoaded &&
        mRenderable &&
        t->mLoaded)
    {
        PagingLandScapeRenderable *r = t->getRenderable();

        mRenderable->_setNeighbor (n, r);
        if (r)
        {
            switch (n)
            {
                case NORTH:
                    r->_setNeighbor (SOUTH, mRenderable); 
                    return;
                case SOUTH:
                    r->_setNeighbor (NORTH, mRenderable); 
                    return;
                case EAST:
                    r->_setNeighbor (WEST, mRenderable); 
                    return;
                case WEST:
                    r->_setNeighbor (EAST, mRenderable); 
                    return;
            }
        }
    }
};
//-----------------------------------------------------------------------
void PagingLandScapeTile::_linkRenderableNeighbor()
{
    if (mNeighbors[SOUTH] && mNeighbors[SOUTH]->mLoaded)
    {
        PagingLandScapeRenderable *n = mNeighbors[SOUTH]->getRenderable();
        mRenderable->_setNeighbor (SOUTH, n);
        n->_setNeighbor (NORTH, mRenderable); 
    }
    if (mNeighbors[NORTH] && mNeighbors[NORTH]->mLoaded)
    {
        PagingLandScapeRenderable *n = mNeighbors[NORTH]->getRenderable();
        mRenderable->_setNeighbor (NORTH, n);
        n->_setNeighbor (SOUTH, mRenderable);
    }
    if (mNeighbors[EAST] && mNeighbors[EAST]->mLoaded)
    {
        PagingLandScapeRenderable *n = mNeighbors[EAST]->getRenderable();
        mRenderable->_setNeighbor (EAST, n);
        n->_setNeighbor (WEST, mRenderable);
    }
    if (mNeighbors[WEST] && mNeighbors[WEST]->mLoaded)
    {
        PagingLandScapeRenderable *n = mNeighbors[WEST]->getRenderable();
        mRenderable->_setNeighbor (WEST, n); 
        n->_setNeighbor (EAST, mRenderable);
    }
}
//-----------------------------------------------------------------------
void PagingLandScapeTile::_updateLod()
{
    if ( mRenderable )
	{        
      _linkRenderableNeighbor();  
	  //mRenderable->update ();
    }
}
//-----------------------------------------------------------------------
void PagingLandScapeTile::load()
{
    assert ( !mLoaded && mRenderable == 0);
   
    // Request a renderable
	mRenderable = PagingLandScapeRenderableManager::getSingleton().getRenderable();
	assert ( mRenderable );

	mRenderable->init( &mInfo );
	// Set the material
    const MaterialPtr mmat =  PagingLandScapeTextureManager::getSingleton().getMaterial( mInfo.pageX, mInfo.pageZ );
    if (!mmat.isNull()) 
		mRenderable->setMaterial( mmat );
    				    
	//Queue it for loading
	PagingLandScapeRenderableManager::getSingleton().queueRenderableLoading( this );
    PagingLandScapePageManager::getSingleton().setTerrainReady (false);
    mLoaded = true;
    mVisible = true;
}
//-----------------------------------------------------------------------
void PagingLandScapeTile::unload()
{
    assert ( mLoaded );
	mRenderable->uninit ();
    mTileSceneNode->needUpdate();
    PagingLandScapeRenderableManager::getSingleton().unqueueRenderable(this);
	mRenderable = 0;
    mVisible = false;
    mLoaded = false;
}
//-----------------------------------------------------------------------
void PagingLandScapeTile::_Notify(const Vector3 &pos, PagingLandScapeCamera* Cam)
{
	Ogre::Real dist = ( pos - mWorldPosition ).squaredLength();
	Ogre::Real factor = PagingLandScapeOptions::getSingleton().renderable_factor;
	if (( dist < factor))
//	if (true)
	{    
        if ( !mLoaded )
        {
//			load(); 
             
            if (
                Cam->getVisibility (mWorldBoundsExt)
                )
            {
                load(); 
            }
        }
        else
        {
            mRenderable->setInUse (true);
        }
	}
	else
	{
		if ( mLoaded )
		{
            unload();
		}
	}
}
//-----------------------------------------------------------------------
 bool PagingLandScapeTile::intersectSegment( const Vector3 & start,
                                             const Vector3 & dir, 
                                             Vector3 * result )
{
    Vector3 ray = start; 
    if (mWorldBounds.isNull())
    {
        if ( result != 0 )
            * result = Vector3( -1.0f, -1.0f, -1.0f );

        return false;
    }
    const Vector3 * corners = mWorldBounds.getAllCorners();

    //start with the next one...
    ray += dir;
    PagingLandScapeData2D *data = PagingLandScapeData2DManager::getSingleton().getData2d (mInfo.pageX, mInfo.pageZ);
    
    if (data == 0)
    {
        // just go until next tile.
        while ( ! ( ( ray.x < corners[ 0 ].x ) ||
                    ( ray.x > corners[ 4 ].x ) ||
                    ( ray.z < corners[ 0 ].z ) ||
                    ( ray.z > corners[ 4 ].z ) ) )
        {           
            ray += dir;
        }
    }
    else
    {
        Real pageMax;
        if (mRenderable && mRenderable->isLoaded ())
            pageMax = mRenderable->getMaxHeight ();
        else
            pageMax = data->getMaxHeight ();

        while ( ! ( ( ray.x < corners[ 0 ].x ) ||
                    ( ray.x > corners[ 4 ].x ) ||
                    ( ray.z < corners[ 0 ].z ) ||
                    ( ray.z > corners[ 4 ].z ) ) )
        {
           
            if ( ray.y <= pageMax &&  // until under the max possible for this page/tile
                 ray.y <= data->getHeightAbsolute( ray.x, ray.z)// until under the max 
                 )
            {

                // Found intersection range 
                // zone (ray -  dir < intersection < ray.y +  dir )
                // now do a precise check using a interpolated height getter (height between vertices)
                ray -= dir;
                // until under the interpolated upon current LOD max 
                while( ray.y > PagingLandScapeData2DManager::getSingleton().getRealPageHeight( ray.x, ray.z, 
                                                                                                mInfo.pageX, mInfo.pageZ, 
                                                                                                0))
                {
                    ray += dir;
                }

                ray.y = PagingLandScapeData2DManager::getSingleton().getRealPageHeight (ray.x, ray.z, 
                                                                                        mInfo.pageX, mInfo.pageZ, 
                                                                                        1);
                   
                *result = ray;
                return true;
            }          
            ray += dir;
        }
    }
        

    if ( ray.x < corners[ 0 ].x && mNeighbors[ WEST ] != 0 && mNeighbors[ WEST ]->isLoaded())
        return mNeighbors[ WEST ] ->intersectSegment( ray, dir, result );
    else if ( ray.z < corners[ 0 ].z && mNeighbors[ NORTH ] != 0  && mNeighbors[ NORTH ]->isLoaded())
        return mNeighbors[ NORTH ] ->intersectSegment( ray, dir, result );
    else if ( ray.x > corners[ 4 ].x && mNeighbors[ EAST ] != 0  && mNeighbors[ EAST ]->isLoaded())
        return mNeighbors[ EAST ] ->intersectSegment( ray, dir, result );
    else if ( ray.z > corners[ 4 ].z && mNeighbors[ SOUTH ] != 0  && mNeighbors[ SOUTH ]->isLoaded())
        return mNeighbors[ SOUTH ] ->intersectSegment( ray, dir, result );
    else
    {
        if ( result != 0 )
            * result = Vector3( -1.0f, -1.0f, -1.0f );

        return false;
    }
} 
//-----------------------------------------------------------------------
void PagingLandScapeTile::updateTerrain ()
{
     mRenderable->setNeedUpdate ();

}
//-----------------------------------------------------------------------
void PagingLandScapeTile::setInUse (bool InUse)
{
    if (mRenderable)
        mRenderable->setInUse (InUse);
}

void PagingLandScapeTile::setRenderQueueGroup(RenderQueueGroupID qid)
{
    if (mRenderable && mRenderable->isLoaded())
        mRenderable->setRenderQueueGroup(qid);
};
} //namespace
