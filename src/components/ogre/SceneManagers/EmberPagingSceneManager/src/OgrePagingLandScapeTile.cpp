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

#include "OgrePagingLandScapeTile.h"

#include "OgrePagingLandScapePrecompiledHeaders.h"

#include "OgrePagingLandScapeCamera.h"
#include "OgrePagingLandScapeData2D.h"
#include "OgrePagingLandScapeData2DManager.h"
#include "OgrePagingLandScapeListener.h"  
#include "OgrePagingLandScapeListenerManager.h" 
#include "OgrePagingLandScapeOptions.h"
#include "OgrePagingLandScapePageManager.h"
#include "OgrePagingLandScapeRenderable.h"
#include "OgrePagingLandScapeRenderableManager.h"
#include "OgrePagingLandScapeSceneManager.h"
#include "OgrePagingLandScapeTexture.h"
#include "OgrePagingLandScapeTextureManager.h"
#include "OgrePagingLandScapeTileManager.h"
#include "OgrePagingLandScapeTileInfo.h"

#include <OgreAxisAlignedBox.h>
#include <OgreCamera.h>
#include <OgreColourValue.h>
#include <OgreMovableObject.h>
#include <OgreSceneNode.h>
#include <OgreSimpleRenderable.h>
#include <OgreStringConverter.h>
#include <OgreVector3.h>


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
		mLoading (false),
		mTimeUntouched(0)
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
void PagingLandScapeTile::uninit()
{
	if (mInit || mLoaded || mRenderable)
	{
		mInit = false;

		if (mLoaded)
		{
			if (mLoading)
				mParent->getSceneManager()->getRenderableManager()->unqueueRenderable (this);
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
void PagingLandScapeTile::init(SceneNode *ParentSceneNode, int pageX, int pageZ, int tileX, int tileZ)
{
	mInit = true;

	PagingLandScapeOptions * const opt = mParent->getOptions();
	mInfo = opt->getTileInfo (pageX, pageZ, tileX, tileZ);	

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

	const Vector3 ParentPos = ParentSceneNode->_getDerivedPosition();
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
}
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
	mVisible = false;

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
	mVisible = true;
	mLoaded = false;
	mTimeUntouched = 0;
	mParentSceneNode->removeChild (mTileSceneNode);

    assert (mInfo);
    mParent->getSceneManager()->getListenerManager()->fireTileUnloaded (mInfo->mPageX, mInfo->mPageZ,
        mInfo->mTileX, mInfo->mTileZ, getWorldBbox ());
}

//-----------------------------------------------------------------------
void PagingLandScapeTile::_Notify(const Vector3 &pos, const PagingLandScapeCamera * const Cam)
{
	//const bool wasvisible = mVisible && mLoaded && (mRenderable != 0) && mRenderable->isVisible ();
	if (1) {
		//(pos - mWorldPosition).squaredLength() < mParent->getOptions()->renderable_factor
		//&& Cam->isVisible (mWorldBoundsExt)
		touch();
		mVisible = true;
	} else {
		mVisible = false;
	}

	// if it changes
	//if (wasvisible != mVisible)
	{
		if (mVisible) {
			if (!mLoaded) {
				load(); 

				// now visible 
				mParent->getSceneManager()->getListenerManager ()->fireTileShow
					(mInfo->mPageX, mInfo->mPageZ, mInfo->mTileX,  mInfo->mTileZ, mWorldBounds);
			}
			assert (mRenderable);
			mRenderable->setVisible (true);   

		} else {
			// now hidden
			if (mRenderable)
				mRenderable->setVisible (false);

		}
	}
}
//-----------------------------------------------------------------------
const bool PagingLandScapeTile::unloadUntouched ()        
{ 
	//if (mLoaded)
	{
		if (mTimeUntouched == 0)
		{
			// not visible check if we must unload it
			// must not have been visible for some time

			if (mInfo)
			{
				mParent->getSceneManager()->getListenerManager ()->fireTileHide 
					(mInfo->mPageX, mInfo->mPageZ, mInfo->mTileX,  mInfo->mTileZ, mWorldBounds);

			}
			if  (mLoading)
				mParent->getSceneManager()->getRenderableManager()->unqueueRenderable (this);
			else if (mLoaded)
				unload();


			return true;			
		}

		mTimeUntouched--; 
	}
	return false;
}
//-----------------------------------------------------------------------
bool PagingLandScapeTile::intersectSegmentFromAbove(const Vector3 & start,
													const Vector3 & dir, 
													Vector3 * result)
{
	if (mWorldBounds.isNull())
	{
		if (result != 0)
			* result = Vector3(-1.0f, -1.0f, -1.0f);
		return false;
	}
	Vector3 ray = start; 
	const Vector3 * const corners = mWorldBounds.getAllCorners();

	//start with the next one...
	ray += dir;


	const Real leftBorder = corners[ 0 ].x;
	const Real rightBorder = corners[ 4 ].x;
	const Real topBorder = corners[ 0 ].z;
	const Real bottomBorder = corners[ 4 ].z;

	PagingLandScapeData2DManager * const dataManager = mParent->getSceneManager()->getData2DManager();
	PagingLandScapeData2D * const data = dataManager->getData2D (mInfo->mPageX, mInfo->mPageZ);
	Real worldMaxHeight(dataManager->getMaxHeight ());
	if (data == 0)
	{
		// just go until next tile.
		while((ray.x > leftBorder) &&
			(ray.x < rightBorder) &&
			(ray.z > topBorder) &&
			(ray.z < bottomBorder))
		{           
			ray += dir;
			//ember specific start
			if (ray.y > worldMaxHeight) {
				//there's no terrain above this point anyway
				return false;
			}
			//ember specific end
		}
	}
	else
	{
		const Real localMax = (mRenderable && mRenderable->isLoaded ()) ? 
								mRenderable->getMaxHeight () 
								: 
								data->getMaxHeight ();
		const Vector3 &invScale = mParent->getOptions ()->invScale;
		const unsigned int pSize = mParent->getOptions ()->PageSize;

		const Real invScaledDirX = dir.x * invScale.x;
		const Real invScaledDirZ = dir.z * invScale.z;
		Real invScaledRayX = (ray.x * invScale.x) - data->getShiftX ();
		Real invScaledRayZ = (ray.z * invScale.z) - data->getShiftZ ();
		const int pSizeMinusOne = static_cast <int> (pSize - 1);

		int i_x, i_z;


		while((ray.x > leftBorder) &&
			(ray.x < rightBorder) &&
			(ray.z > topBorder) &&
			(ray.z < bottomBorder))
		{
			//ember specific start
			if (ray.y > worldMaxHeight) {
				//there's no terrain above this point anyway
				return false;
			}
			//ember specific end
			
			if (ray.y <= localMax) // until under the max possible for this page/tile
			{
				// adjust x and z to be local to page
				i_x = static_cast<int> (invScaledRayX);
				i_z = static_cast<int> (invScaledRayZ);

				// due to Real imprecision on Reals, we have to use boundaries here
				// otherwise we'll hit asserts.
				if (i_x > pSizeMinusOne)		i_x = pSizeMinusOne;
				else if (i_x < 0)	i_x = 0;
				if (i_z > pSizeMinusOne)		i_z = pSizeMinusOne;
				else if (i_z < 0)	i_z = 0;

				if (ray.y <= data->getHeight (static_cast<unsigned int> (i_z * pSize + i_x)))// until under the max 				
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
							(ray.x > leftBorder) &&
							(ray.x < rightBorder) &&
							(ray.z > topBorder) &&
							(ray.z < bottomBorder))
						{
							ray += dir;
						}
					}
					ray.y = dataManager->getInterpolatedWorldHeight(ray.x, ray.z);

					*result = ray;
					return true;
				}
			}          
			ray += dir;
			invScaledRayX += invScaledDirX;
			invScaledRayZ += invScaledDirZ;
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
	if (mWorldBounds.isNull())
	{
		if (result != 0)
			* result = Vector3(-1.0f, -1.0f, -1.0f);

		return false;
	}
	Vector3 ray = start; 
	const Vector3 * const corners = mWorldBounds.getAllCorners();

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
		while ((ray.x > leftBorder) &&
			(ray.x < rightBorder) &&
			(ray.z > topBorder) &&
			(ray.z < bottomBorder))
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

		while ((ray.x > leftBorder) &&
			(ray.x < rightBorder) &&
			(ray.z > topBorder) &&
			(ray.z < bottomBorder))
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
						(ray.x > leftBorder) &&
						(ray.x < rightBorder) &&
						(ray.z > topBorder) &&
						(ray.z < bottomBorder))
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

void PagingLandScapeTile::setRenderQueueGroup(uint8 qid)
{
	if (mRenderable && mRenderable->isLoaded())
		mRenderable->setRenderQueueGroup(qid);
}

} //namespace
