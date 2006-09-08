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

#include "OgrePagingLandScapePrecompiledHeaders.h"

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

#include "OgrePagingLandScapeSceneManager.h"
#include "OgrePagingLandScapeRenderableManager.h"
#include "OgrePagingLandScapeRenderable.h"

#include "OgrePagingLandScapeTextureManager.h"
#include "OgrePagingLandScapeTexture.h"

#include "OgrePagingLandScapeListener.h"  
#include "OgrePagingLandScapeListenerManager.h" 

namespace Ogre
{

	//-----------------------------------------------------------------------
	PagingLandScapeTile::PagingLandScapeTile(PagingLandScapeTileManager *tileMgr) :
		mParent(tileMgr),
		mInfo(0),
		mTileSceneNode (0),
		mRenderable (0),
		mInit (false),
		mLoaded (false),
		mLoading (false)
{
	for (unsigned int i = 0; i < 4; i++)
	{
		mNeighbors[ i ] = 0;
	}
}

//-----------------------------------------------------------------------
PagingLandScapeTile::~PagingLandScapeTile()
{
}
//-----------------------------------------------------------------------
void PagingLandScapeTile::uninit(void)
{
	if (mInit || mLoaded || mRenderable)
	{
		mInit = false;

		if (mLoaded)
		{
			if (mLoading)
			{
				mParent->getSceneManager()->getRenderableManager()->unqueueRenderable (this);
				mLoading = false;
			}
			unload ();
		}

		if (mNeighbors[SOUTH])
			mNeighbors[SOUTH]->_setNeighbor (NORTH, 0);
		if (mNeighbors[NORTH])
			mNeighbors[NORTH]->_setNeighbor (SOUTH, 0);
		if (mNeighbors[EAST])
			mNeighbors[EAST]->_setNeighbor (WEST, 0);
		if (mNeighbors[WEST])
			mNeighbors[WEST]->_setNeighbor (EAST, 0);

		for (unsigned int i = 0; i < 4; i++)
		{
			mNeighbors[ i ] = 0;
		}

		mParent->getSceneManager()->destroySceneNode (mTileSceneNode->getName ());
		mTileSceneNode = 0;
		mParentSceneNode = 0;
		mInfo = 0;
		mParent->freeTile(this);
	}   
}
//-----------------------------------------------------------------------
void PagingLandScapeTile::init (SceneNode *ParentSceneNode,
								const int pageX, const int pageZ, 
								const int tileX, const int tileZ)
{
	mInit = true;

	PagingLandScapeOptions * const opt = mParent->getOptions();
	mInfo = opt->getTileInfo (pageX, pageZ, tileX, tileZ);
	if (mInfo == 0)
	{
		mInfo = new PagingLandScapeTileInfo(pageX, pageZ, tileX, tileZ);
		opt->setTileInfo(mInfo);
	}

	// Calculate the offset from the parent for this tile
	const Vector3 scale = opt->scale;
	const Real endx = opt->TileSize * scale.x;
	const Real endz = opt->TileSize * scale.z;

	const Real posX = mInfo->mTileX * endx;
	const Real posZ = mInfo->mTileZ * endz;


	assert (ParentSceneNode);
	mParentSceneNode = ParentSceneNode;
	const String NodeName =  "PagingLandScapeTile." 
		+ StringConverter::toString(pageX) + "." + 
		StringConverter::toString(pageZ)
		+ StringConverter::toString(tileX) + "." + 
		StringConverter::toString(tileZ);
	mTileSceneNode = mParent->getSceneManager()->createSceneNode (NodeName + ".Node"); 

	const Vector3 ParentPos = ParentSceneNode->getWorldPosition();
	const Real MaxHeight = mParent->getSceneManager()->getData2DManager()->getMaxHeight(mInfo->mPageX, mInfo->mPageZ);
	assert (MaxHeight >= (Real)0.0f);
	//Change Zone of this page

	//tile center position
	mWorldPosition = ParentPos + Vector3 (posX + endx * 0.5f, 
											MaxHeight * 0.5f, 
											posZ + endz * 0.5f);


	mWorldBounds.setExtents(posX + ParentPos.x , 
							(Real)0.0, 
							posZ + ParentPos.z, 
							posX + ParentPos.x + endx, 
							MaxHeight, 
							posZ + ParentPos.z + endz);



	mWorldBoundsExt.setExtents(mWorldPosition.x - endx * 1.5f, 
		- MaxHeight * 1.5f, 
		mWorldPosition.z - endz * 1.5f, 

		mWorldPosition.x + endx * 1.5f, 
		MaxHeight * 1.5f, 
		mWorldPosition.z + endz * 1.5f);



	for (unsigned int i = 0; i < 4; i++)
	{
		mNeighbors[ i ] = 0;
	}
	//force update in scene node
	mLoaded = false;
	mLoading = false;
}
//-----------------------------------------------------------------------
void PagingLandScapeTile::_setNeighbor(Neighbor n, PagingLandScapeTile *t)
{
	mNeighbors[ n ] = t;
	if (t && mLoaded && mRenderable && t->mLoaded)
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
	assert (mLoaded && mRenderable);
	PagingLandScapeRenderable *n;
	PagingLandScapeTile *t = mNeighbors[SOUTH];
	if (t && t->mLoaded)
	{
		n = t->getRenderable();
		assert (n);
		mRenderable->_setNeighbor (SOUTH, n);
		n->_setNeighbor (NORTH, mRenderable); 
	}
	t = mNeighbors[NORTH];
	if (t && t->mLoaded)
	{
		n = t->getRenderable();
		assert (n);
		mRenderable->_setNeighbor (NORTH, n);
		n->_setNeighbor (SOUTH, mRenderable);
	}
	t = mNeighbors[EAST];
	if (t && t->mLoaded)
	{
		n = t->getRenderable();
		assert (n);
		mRenderable->_setNeighbor (EAST, n);
		n->_setNeighbor (WEST, mRenderable);
	}
	t = mNeighbors[WEST];
	if (t && t->mLoaded)
	{
		n = t->getRenderable();
		assert (n);
		mRenderable->_setNeighbor (WEST, n); 
		n->_setNeighbor (EAST, mRenderable);
	}
}
//-----------------------------------------------------------------------
void PagingLandScapeTile::_updateLod()
{
	if (mRenderable)
	{        
		_linkRenderableNeighbor();  
		//mRenderable->update ();
	}
}
//-----------------------------------------------------------------------
void PagingLandScapeTile::load()
{
	assert (mInit && !mLoaded && mRenderable == 0);

	PagingLandScapeRenderableManager * const rendMgr = mParent->getSceneManager()->getRenderableManager();

	// Request a renderable
	mRenderable = rendMgr->getRenderable();

	assert (mRenderable);

	mRenderable->init (mInfo);
	mRenderable->mParentTile = this;

	// Set the material
	PagingLandScapeTexture * const tex =  mParent->getSceneManager()->getTextureManager()->getTexture (mInfo->mPageX, mInfo->mPageZ);
	assert (tex);
	assert (!tex->getMaterial().isNull()) ;
	mRenderable->setMaterial(tex->getMaterial());

	mParentSceneNode->addChild (mTileSceneNode);
	mLoaded = true;	    
	mVisible = true;

	//Queue its renderable for loading
	rendMgr->queueRenderableLoading (this);
	mParent->getSceneManager()->getPageManager()->setTerrainReady (false);
}
//-----------------------------------------------------------------------
void PagingLandScapeTile::unload()
{
	assert (mLoaded);
	mRenderable->uninit ();

	mRenderable = 0;
	mVisible = false;
	mLoaded = false;
	mTimePreLoaded = 0;
	mParentSceneNode->removeChild (mTileSceneNode);
}
//-----------------------------------------------------------------------
void PagingLandScapeTile::_Notify(const Vector3 &pos, PagingLandScapeCamera* Cam)
{
	bool wasvisible = mVisible;
	if ((pos - mWorldPosition).squaredLength() < mParent->getOptions()->renderable_factor)
	{    
		if (Cam->isVisible (mWorldBoundsExt))
		{
			if (!mLoaded)
				load(); 
			touch();
			mVisible = true;
			mRenderable->setVisible (true);
		}
		else
		{
			mVisible = false;
		}
	}
	else
	{
		mVisible = false;
	}
	if (wasvisible != mVisible)
	{
		if (wasvisible)
		{
			if (mRenderable)
				mRenderable->setVisible (false);
			mParent->getSceneManager()->getListenerManager ()->fireTileHide 
				(mInfo->mPageX, mInfo->mPageZ, mInfo->mTileX,  mInfo->mTileZ, mWorldBounds);
		}
		else 
		{
			mParent->getSceneManager()->getListenerManager ()->fireTileShow
				(mInfo->mPageX, mInfo->mPageZ, mInfo->mTileX,  mInfo->mTileZ, mWorldBounds);
		}
	}

}
//-----------------------------------------------------------------------
const bool PagingLandScapeTile::touched ()        
{ 
	if (mLoaded)
	{
		if (mTimePreLoaded == 0)
		{
			// not visible check if we must unload it
			// must not have been visible for some time
			
			if  (mLoading)
				mParent->getSceneManager()->getRenderableManager()->unqueueRenderable (this);
			unload();
			return true;			
		}

		mTimePreLoaded--; 
	}
	return false;
}
//-----------------------------------------------------------------------
bool PagingLandScapeTile::intersectSegmentFromAbove(const Vector3 & start,
													const Vector3 & dir, 
													Vector3 * result)
{
	Vector3 ray = start; 
	if (mWorldBounds.isNull())
	{
		if (result != 0)
			* result = Vector3(-1.0f, -1.0f, -1.0f);

		return false;
	}
	const Vector3 * corners = mWorldBounds.getAllCorners();

	//start with the next one...
	ray += dir;

	PagingLandScapeData2DManager * const dataManager = mParent->getSceneManager()->getData2DManager();

	PagingLandScapeData2D *data = dataManager->getData2D (mInfo->mPageX, mInfo->mPageZ);

	const Real leftBorder = corners[ 0 ].x;
	const Real rightBorder = corners[ 4 ].x;
	const Real topBorder = corners[ 0 ].z;
	const Real bottomBorder = corners[ 4 ].z;



	if (data == 0)
	{
		// just go until next tile.
		while (! ((ray.x < leftBorder) ||
			(ray.x > rightBorder) ||
			(ray.z < topBorder) ||
			(ray.z > bottomBorder)))
		{           
			ray += dir;
		}
	}
	else
	{
		const Real localMax = (mRenderable && mRenderable->isLoaded ()) ? 
			mRenderable->getMaxHeight () 
			: 
		data->getMaxHeight ();

		while (! ((ray.x < leftBorder) ||
			(ray.x > rightBorder) ||
			(ray.z < topBorder) ||
			(ray.z > bottomBorder)))
		{

			if (ray.y <= localMax &&  // until under the max possible for this page/tile
				ray.y <= data->getHeightAbsolute(ray.x, ray.z)// until under the max 
				)
			{

				// Found intersection range 
				// zone (ray -  dir < intersection < ray.y +  dir)
				// now do a precise check using a interpolated height getter (height between vertices)
				ray -= dir;
				// until under the interpolated upon current LOD max      
				{
					while (ray.y > dataManager->getInterpolatedWorldHeight(ray.x, ray.z)
						&&
						ray.y < localMax 
						&&
						ray.y > 0 
						&&
						! ( (ray.x < leftBorder) ||
						(ray.x > rightBorder) ||
						(ray.z < topBorder) ||
						(ray.z > bottomBorder)))
					{
						ray += dir;
					}
				}
				ray.y = dataManager->getInterpolatedWorldHeight(ray.x, ray.z);

				*result = ray;
				return true;
			}          
			ray += dir;
		}
	}


	if (ray.x < leftBorder && mNeighbors[ WEST ] != 0 && mNeighbors[ WEST ]->isLoaded())
		return mNeighbors[ WEST ] ->intersectSegmentFromAbove(ray, dir, result);
	else if (ray.z < topBorder && mNeighbors[ NORTH ] != 0  && mNeighbors[ NORTH ]->isLoaded())
		return mNeighbors[ NORTH ] ->intersectSegmentFromAbove(ray, dir, result);
	else if (ray.x > rightBorder && mNeighbors[ EAST ] != 0  && mNeighbors[ EAST ]->isLoaded())
		return mNeighbors[ EAST ] ->intersectSegmentFromAbove(ray, dir, result);
	else if (ray.z > bottomBorder && mNeighbors[ SOUTH ] != 0  && mNeighbors[ SOUTH ]->isLoaded())
		return mNeighbors[ SOUTH ] ->intersectSegmentFromAbove(ray, dir, result);
	else
	{
		if (result != 0)
			* result = Vector3(-1.0f, -1.0f, -1.0f);

		return false;
	}
} 
//-----------------------------------------------------------------------
bool PagingLandScapeTile::intersectSegmentFromBelow(const Vector3 & start,
													const Vector3 & dir, 
													Vector3 * result)
{
	Vector3 ray = start; 
	if (mWorldBounds.isNull())
	{
		if (result != 0)
			* result = Vector3(-1.0f, -1.0f, -1.0f);

		return false;
	}
	const Vector3 * corners = mWorldBounds.getAllCorners();

	//start with the next one...
	ray += dir;

	PagingLandScapeData2DManager * const dataManager = mParent->getSceneManager()->getData2DManager();

	PagingLandScapeData2D *data = dataManager->getData2D (mInfo->mPageX, mInfo->mPageZ);

	const Real leftBorder = corners[ 0 ].x;
	const Real rightBorder = corners[ 4 ].x;
	const Real topBorder = corners[ 0 ].z;
	const Real bottomBorder = corners[ 4 ].z;



	if (data == 0)
	{
		// just go until next tile.
		while (! ((ray.x < leftBorder) ||
			(ray.x > rightBorder) ||
			(ray.z < topBorder) ||
			(ray.z > bottomBorder)))
		{           
			ray += dir;
		}
	}
	else
	{
		const Real localMax = (mRenderable && mRenderable->isLoaded ()) ? 
			mRenderable->getMaxHeight () 
			: 
		data->getMaxHeight ();

		while (! ((ray.x < leftBorder) ||
			(ray.x > rightBorder) ||
			(ray.z < topBorder) ||
			(ray.z > bottomBorder)))
		{

			if (ray.y >= 0 &&
				ray.y >= data->getHeightAbsolute(ray.x, ray.z))
			{

				// Found intersection range 
				// zone (ray -  dir < intersection < ray.y +  dir)
				// now do a precise check using a interpolated height getter (height between vertices)
				ray -= dir;
				// until under the interpolated upon current LOD max   
				{
					while (ray.y < dataManager->getInterpolatedWorldHeight(ray.x, ray.z)
						&&
						ray.y < localMax 
						&&
						ray.y >= 0 
						&&
						! ( (ray.x < leftBorder) ||
						(ray.x > rightBorder) ||
						(ray.z < topBorder) ||
						(ray.z > bottomBorder)))
					{
						ray += dir;
					}
				}
				ray.y = dataManager->getInterpolatedWorldHeight (ray.x, ray.z);

				*result = ray;
				return true;
			}          
			ray += dir;
		}
	}


	if (ray.x < leftBorder && mNeighbors[ WEST ] != 0 && mNeighbors[ WEST ]->isLoaded())
		return mNeighbors[ WEST ] ->intersectSegmentFromBelow(ray, dir, result);
	else if (ray.z < topBorder && mNeighbors[ NORTH ] != 0  && mNeighbors[ NORTH ]->isLoaded())
		return mNeighbors[ NORTH ] ->intersectSegmentFromBelow(ray, dir, result);
	else if (ray.x > rightBorder && mNeighbors[ EAST ] != 0  && mNeighbors[ EAST ]->isLoaded())
		return mNeighbors[ EAST ] ->intersectSegmentFromBelow(ray, dir, result);
	else if (ray.z > bottomBorder && mNeighbors[ SOUTH ] != 0  && mNeighbors[ SOUTH ]->isLoaded())
		return mNeighbors[ SOUTH ] ->intersectSegmentFromBelow(ray, dir, result);
	else
	{
		if (result != 0)
			* result = Vector3(-1.0f, -1.0f, -1.0f);

		return false;
	}
} 
//-----------------------------------------------------------------------
void PagingLandScapeTile::updateTerrain ()
{
	assert (mInit && mLoaded && mRenderable);
	mRenderable->setNeedUpdate ();
}

void PagingLandScapeTile::setRenderQueueGroup(RenderQueueGroupID qid)
{
	if (mRenderable && mRenderable->isLoaded())
		mRenderable->setRenderQueueGroup(qid);
};
} //namespace
