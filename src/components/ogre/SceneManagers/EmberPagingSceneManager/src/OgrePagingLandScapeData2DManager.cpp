/***************************************************************************
	OgrePagingLandScapeData2DManager.cpp  -  description
	-------------------
	begin                : Mon Jun 16 2003
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

#include "OgreMath.h"

#include "OgreVector3.h"
#include "OgreColourValue.h"

#include "OgreMovableObject.h"
#include "OgreAxisAlignedBox.h"

#include "OgrePlane.h" 

#include "OgreCamera.h"

#include "OgreSimpleRenderable.h"
#include "OgreException.h"

#include "OgrePagingLandScapeOptions.h"
#include "OgrePagingLandScapeCamera.h"

#include "OgrePagingLandScapeData2DManager.h"
#include "OgrePagingLandScapeData2D.h"

// Data loaders implementations
#include "OgrePagingLandScapeData2D_HeightField.h"
#include "OgrePagingLandScapeData2D_HeightFieldTC.h"

#include "OgrePagingLandScapeData2D_HeightFieldN.h"
#include "OgrePagingLandScapeData2D_HeightFieldNTC.h"

#include "OgrePagingLandScapeData2D_HeightFieldRaw.h"
#include "OgrePagingLandScapeData2D_HeightFieldRawTC.h"

#include "OgrePagingLandScapeData2D_Spline.h"

// load heights from texture.
#include "OgrePagingLandScapeData2D_HeightFieldBlendNeighbor.h"

// needed to get RenderLevel for RealHeight
#include "OgrePagingLandScapeRenderable.h"
#include "OgrePagingLandScapeTile.h"
#include "OgrePagingLandScapeTileInfo.h"
#include "OgrePagingLandScapePage.h"
#include "OgrePagingLandScapePageManager.h"

#include "OgrePagingLandScapeSceneManager.h"

namespace Ogre
{

    //-----------------------------------------------------------------------
    template<> PagingLandScapeData2DManager* Singleton<PagingLandScapeData2DManager>::ms_Singleton = 0;
    //-----------------------------------------------------------------------
    PagingLandScapeData2DManager* PagingLandScapeData2DManager::getSingletonPtr(void)
    {
	    return ms_Singleton;
    }
    //-----------------------------------------------------------------------
    PagingLandScapeData2DManager& PagingLandScapeData2DManager::getSingleton(void)
    {  
	    assert(ms_Singleton);  return (*ms_Singleton);  
    }

    //-----------------------------------------------------------------------
    PagingLandScapeData2DManager::PagingLandScapeData2DManager() :
        mOptions (PagingLandScapeOptions::getSingletonPtr()),
        mData2DType (0),
        mData2DFormat (""),
        mWidth (0),
        mHeight (0),
        mMaxHeight (0),
        mPageManager (0)
    {
        // Add default texture Types.
        registerDataType (new PagingLandScapeData2D_HeightField ());
        registerDataType (new PagingLandScapeData2D_HeightFieldN());
        registerDataType (new PagingLandScapeData2D_HeightFieldRaw());
        registerDataType (new PagingLandScapeData2D_HeightFieldTC());
        registerDataType (new PagingLandScapeData2D_HeightFieldNTC());
        registerDataType (new PagingLandScapeData2D_HeightFieldRawTC());
        registerDataType (new PagingLandScapeData2D_HeightFieldBlendNeighbor());
        registerDataType (new PagingLandScapeData2D_Spline());
    }
    //-----------------------------------------------------------------------
    PagingLandScapeData2D *PagingLandScapeData2DManager::allocateData2D() const
    {        
		assert (!mData2DTypeMap.empty() && mData2DTypeMap[mData2DType]);
       return mData2DTypeMap[mData2DType]->newPage();
    }    
    //-----------------------------------------------------------------------
    PagingLandScapeData2DManager::~PagingLandScapeData2DManager()
    {      
		reset();
        // for all in map delete.
        PagingLandScapeData2DMap::iterator i = mData2DTypeMap.begin();
        while (i != mData2DTypeMap.end())
        {
            delete (*i);
            ++i;
		}
    }       
    //-----------------------------------------------------------------------
    void PagingLandScapeData2DManager::reset()
    {
		if (!mActiveData2Ds.empty())
		{
			std::for_each(mActiveData2Ds.begin(), mActiveData2Ds.end(), 
				std::mem_fun(&PagingLandScapeData2D::unload));

			// Insert actives into free list
			mFreeData2Ds.insert(mFreeData2Ds.end(), mActiveData2Ds.begin(), mActiveData2Ds.end());
			// Remove all active instances
			mActiveData2Ds.clear(); 
		}

		// could save a delete if data type is the same... ?
		if (!mData2DPool.empty())
		{
			std::for_each(mData2DPool.begin(), mData2DPool.end(), delete_object());
			mData2DPool.clear();
			mFreeData2Ds.clear();
		}

        mWidth = 0; 
        mHeight = 0;
    }
    //-----------------------------------------------------------------------
    void PagingLandScapeData2DManager::load ()
    {        
        if (mOptions->data2DFormat != mData2DFormat)
        {
            reset ();
            mData2DFormat = mOptions->data2DFormat; 
            uint i = 0;
            for (; i != mData2DTypeMap.size(); ++i)
            {
                if (StringUtil::endsWith (mData2DTypeMap[i]->getName (), mData2DFormat, false))
                {
                    mData2DType = i;
                    break;
                }
            }
            if (i == mData2DTypeMap.size())
            {
                OGRE_EXCEPT(Exception::ERR_INVALIDPARAMS, 
                        "PageData2D not supplied or wrong (check case) !",
		                " PagingLandScapeData2DManager::load");
            }
        }
        WorldDimensionChange();
		mMaxHeight = mData2DTypeMap[mData2DType]->getMaxAbsoluteHeight ();
    }    
    //-----------------------------------------------------------------------
    bool PagingLandScapeData2DManager::reload(const uint dataX, const uint dataZ)
    {
        PagingLandScapeData2D* data = getData2D (dataX, dataZ);
	
		data->unload();
		const bool ret = data->load(dataX, dataZ);
		const uint tsize =  mOptions->TileSize;
		const uint psize =  mOptions->PageSize;
		for (uint z = 0; z <= psize ; z += tsize)
		{
			for (uint x = 0; x <= psize ; x += tsize)
			{                       
				data->adjustDeformationRectangle (x, z);
			}
		}
		return ret;
	}
	//-----------------------------------------------------------------------
	PagingLandScapeData2D *PagingLandScapeData2DManager::getNewData2D(const uint x, const uint z)
	{
		PagingLandScapeData2D *newData2D;
		if (mFreeData2Ds.empty())
		{
			const size_t pool_size = mData2DPool.size ();			
			const size_t new_pool_size = (pool_size == 0) ? 9 : pool_size * 2;
			mData2DPool.reserve(new_pool_size);
			mData2DPool.resize(new_pool_size);

			// Create new pages
			for (size_t i = pool_size; i < new_pool_size; ++i)
			{
				newData2D = allocateData2D();
				mData2DPool[i] = newData2D;
				mFreeData2Ds.push_back (newData2D);
			}
		}

		newData2D = mFreeData2Ds.front ();
		mFreeData2Ds.pop_front ();
		mActiveData2Ds.push_back (newData2D); 

		newData2D->load (x, z);

		return newData2D;
	}
	//-----------------------------------------------------------------------
	void PagingLandScapeData2DManager::releaseData2D(PagingLandScapeData2D *p)
	{
		mActiveData2Ds.remove(p);
		mFreeData2Ds.push_back(p);
	}
	//-----------------------------------------------------------------------
	PagingLandScapeData2D *PagingLandScapeData2DManager::getData2D(const uint x, const uint z,
		const bool alwaysReturn)
	{
		if (x < mWidth && z < mHeight)
		{
			PagingLandScapeData2DList::iterator l, lend = mActiveData2Ds.end();
			for (l = mActiveData2Ds.begin(); l != lend; ++l)
			{
				if ((*l)->isCoord(x, z))
					return (*l);
			}
			if (alwaysReturn)
				return getNewData2D(x, z);
		}
		assert (!alwaysReturn);
		return 0;
	}
    //-----------------------------------------------------------------------
    void PagingLandScapeData2DManager::clear ()
    {
    }   
    //-----------------------------------------------------------------------
    void PagingLandScapeData2DManager::WorldDimensionChange()
    {
        const uint newWidth = mOptions->world_width; 
        const uint newHeight = mOptions->world_height;

		reset();

        mWidth = newWidth; 
        mHeight = newHeight;
    }
    //-----------------------------------------------------------------------
    void PagingLandScapeData2DManager::setPageManager ()
    {
        mPageManager = PagingLandScapePageManager::getSingletonPtr();
    }        
    //-----------------------------------------------------------------------
    bool PagingLandScapeData2DManager::load(const uint dataX, const uint dataZ)
    {
	    PagingLandScapeData2D* data = getData2D ( dataX , dataZ );
	    if (!data->isLoaded ())
	    {
            return data->load (dataX, dataZ);
	    }
        return true;
    }
    //-----------------------------------------------------------------------
    void PagingLandScapeData2DManager::unload(const uint dataX, const uint dataZ)
    {
	    PagingLandScapeData2D* data = getData2D ( dataX , dataZ, false );
	    if (data && data->isLoaded ())
	    {
			data->unload ();
			releaseData2D (data);
	    }
    }

    //-----------------------------------------------------------------------
    bool PagingLandScapeData2DManager::isLoaded(const uint dataX, const uint dataZ)
	{
		PagingLandScapeData2D * const data = getData2D (dataX , dataZ, false);
		return data && data->isLoaded();
    }
    //-----------------------------------------------------------------------
    const ColourValue PagingLandScapeData2DManager::getCoverageAt(const uint dataX,
                                                                const uint dataZ, 
                                                                const Real x, 
                                                                const Real z)
    {
        PagingLandScapeData2D* data = getData2D (dataX , dataZ, false);
        if (data && data->isLoaded())
        {
            // TODO check it the terrain height is modified
            return data->getCoverage(x, z);
        }
        return ColourValue::White;
    }
    //-----------------------------------------------------------------------
    const ColourValue PagingLandScapeData2DManager::getBaseAt(const uint dataX, 
                                                            const uint dataZ, 
                                                            const Real x, 
                                                            const Real z)
	{
		PagingLandScapeData2D* data = getData2D (dataX , dataZ, false);
		if (data && data->isLoaded())
        {
            // TODO check it the terrain height is modified
            return data->getBase(x, z);
    }
        return ColourValue::White;
    }
    //-----------------------------------------------------------------------
    const Real PagingLandScapeData2DManager::getShadowAt(const uint dataX,
                                                        const uint dataZ,
                                                        const uint x,
                                                        const uint z,
                                                        const bool &positive)
	{
		PagingLandScapeData2D* data = getData2D (dataX , dataZ, false);
		if (data && data->isLoaded())
        {
            // TODO check it the terrain height is modified
            return data->getShadow(x, z, positive);
        }
        return 0.0f;
    }
    //-----------------------------------------------------------------------
    const Real PagingLandScapeData2DManager::getHeight(const uint dataX, 
                                                        const uint dataZ, 
                                                        const Real x, 
                                                        const Real z)
    {

		PagingLandScapeData2D* data = getData2D (dataX , dataZ, false);
		if (data && data->isLoaded())
	    {
		    // TODO check it the terrain height is modified
		    return data->getHeight(x, z);
	    }
	    return 0.0f;
    }
    //-----------------------------------------------------------------------
    const Real PagingLandScapeData2DManager::getHeight(const uint dataX, 
                                                        const uint dataZ, 
                                                        const uint x, 
                                                        const uint z) 
    {

		PagingLandScapeData2D* data = getData2D (dataX , dataZ, false);
		if (data && data->isLoaded())
	    {
		    return data->getHeight(x, z);
	    }
	    return 0.0f;
    }
   
    //-----------------------------------------------------------------------
//    bool PagingLandScapeData2DManager::addNewHeight(const Sphere newSphere)
//    {
//	    uint x, z;
//
//        Vector3 s = newSphere.getCenter();
//	    // Calculate where is going to be placed the new height
//        PagingLandScapePageManager::getSingleton ().getPageIndices(s.x, s.z, x, z, true);
//	    // TODO: DeScale and add the sphere to all the necessary pages
//
//	    //place it there
//	    return mData2D[ x ][ z ]->addNewHeight(newSphere);
//    }

    //-----------------------------------------------------------------------
//    bool PagingLandScapeData2DManager::removeNewHeight(const Sphere oldSphere)
//    {
//	    uint x, z;
//        Vector3 s = oldSphere.getCenter();
//	    // Calculate where is going to be placed the new height
//	    PagingLandScapePageManager::getSingleton ().getPageIndices(s.x, s.z, x, z, true);
//	    // TODO: DeScale and add the sphere to all the necessary pages
//
//	    //remove it
//	    return mData2D[ x ][ z ]->removeNewHeight(oldSphere);
//    }
    //-----------------------------------------------------------------------
    const Real PagingLandScapeData2DManager::getMaxHeight(const uint x, 
                                                        const uint z)
	{ 
		PagingLandScapeData2D* data = getData2D (x, z, false);
		if (data && data->isLoaded())
        {
            return data->getMaxHeight();
        }
        return mMaxHeight;
    }
    //-----------------------------------------------------------------------
    const Real PagingLandScapeData2DManager::getMaxHeight() const
    { 
        return mMaxHeight;
    }   
    //-----------------------------------------------------------------------
    bool PagingLandScapeData2DManager::DeformHeight(const Vector3 &deformationPoint,
                                                    const Real modificationHeight,
                                                    const PagingLandScapeTileInfo* info)
    {
        const uint pX = info->pageX;
        const uint pZ = info->pageZ;
        const uint pSize = mOptions->PageSize - 1;
        // adjust x and z to be local to page
        const uint x = static_cast<uint> (deformationPoint.x
                                            - pX * pSize
                                            + mOptions->maxUnScaledX);
        const uint z = static_cast<uint> (deformationPoint.z
                                            - pZ * pSize
                                            + mOptions->maxUnScaledZ);

		PagingLandScapeData2D* data = getData2D (pX , pZ, false);
		if (data && data->isLoaded())
        {
            Real heightResult = modificationHeight;
            if (data->DeformHeight (x, z, heightResult))
            {
                const uint wL = mOptions->world_width;
                const uint hL = mOptions->world_height;

                // If we're on a page edge, we must duplicate the change on the 
                // neighbour page (if it has one...)
                const bool left  = (x == 0     && pX != 0);
                const bool right = (x == pSize && pX < wL - 1);
                const bool down  = (z == 0     && pZ != 0);
                const bool up    = (z == pSize && pZ < hL - 1);

                assert (z <= pSize && x <= pSize);

                if (left)
                {
                    if (down)
                    {               
                        //  lower left corner 
						data = getData2D (pX - 1, pZ - 1 , false);
						if (data && data->isLoaded())
                        {
                           data->setHeight (pSize, pSize, heightResult);
                        } 
                    }
                    else if (up)
                    {
						//  upper left corner 
						data = getData2D (pX - 1, pZ + 1 , false);
						if (data && data->isLoaded())
                        {
                           data->setHeight (pSize, 0, heightResult);
                        } 
                    } 
                    
					// left
					data = getData2D (pX - 1, pZ, false);
					if (data && data->isLoaded())
                    {
                        data->setHeight (pSize, z, heightResult);
                    } 
                }
                else if (right)
                {
                    if (up)
                    {
						//  upper right corner 
						data = getData2D (pX + 1, pZ + 1 , false);
						if (data && data->isLoaded())
                        {
                           data->setHeight (0, 0, heightResult);
                        } 
                    }
                    else if (down)
                    {
						//  lower right corner 
						data = getData2D (pX + 1, pZ - 1 , false);
						if (data && data->isLoaded())
                        {
                           data->setHeight (0, pSize, heightResult);
                        } 
                    }  
					// right
					data = getData2D (pX + 1, pZ, false);
					if (data && data->isLoaded())
                    {
                        data->setHeight (0, z, heightResult);
                    }  
                }
                if (down)
                {        
					//  lower  (down only)   
					data = getData2D (pX, pZ - 1 , false);
					if (data && data->isLoaded()) 
                    {
                        data->setHeight (x, pSize, heightResult);
                    } 
                }
                else if (up)
                {
					// upper (up only)
					data = getData2D (pX, pZ + 1 , false);
					if (data && data->isLoaded())
                    {
                        data->setHeight (x, 0, heightResult);
                    } 
                }     
                return true;
            } // if (data->DeformHeight (x, z, heightResult))
        } // if (data->isLoaded())
        return false;
    }

    //-----------------------------------------------------------------------
    const Real PagingLandScapeData2DManager::getInterpolatedWorldHeight (const Real x, const Real z, Real *slope)
    {

      Real heightValue;

		//#define _DEBUGPOS
		//#define _DEBUGPOSHEIGHTSTICH
		
        // scale position from world to page scale
        const Real unscaledX = (x / mOptions->scale.x) + mOptions->maxUnScaledX;
        const Real unscaledZ = (z / mOptions->scale.z) + mOptions->maxUnScaledZ;
        
        const Real pSize = mOptions->PageSize - 1;
        const Real inv_pSize = 1.0f / pSize;
        
        //get Page 
		int value = static_cast< int > (unscaledX * inv_pSize);
		const int maxPageX = static_cast< int > (mOptions->world_width);
		const int pageNumberX = (value < 0 ) ? 0 : (value >= maxPageX ? maxPageX - 1: value); 
		const int maxPageZ = static_cast< int > (mOptions->world_height);
		value = static_cast< int > (unscaledZ * inv_pSize);
        const int pageNumberZ = (value < 0 ) ? 0 : (value >= maxPageZ ? maxPageZ - 1: value); 
        		

		const PagingLandScapePage * const page = PagingLandScapePageManager::getSingleton().getPage (pageNumberX, pageNumberZ, false);		
        if (! (page && page->isPreLoaded()))
            return 0.0f;
            
        // adjust x and z to be local to page
        const Real localPageX = unscaledX - (pageNumberX*pSize);
        const Real localPageZ = unscaledZ - (pageNumberZ*pSize);

            
		const uint tSize = mOptions->TileSize - 1;
        const Real inv_tSize = 1.0f / tSize;
        
        //get Tile
		value = static_cast< int > (localPageX * inv_tSize);
		const int maxNumTiles = static_cast< int > (mOptions->NumTiles);
        const int tileNumberX = (value < 0 ) ? 0 : (value >=  maxNumTiles? maxNumTiles - 1: value); 
		value = static_cast< int > (localPageZ * inv_tSize);
        const int tileNumberZ = (value < 0 ) ? 0 : (value >=  maxNumTiles? maxNumTiles - 1: value); 
        
        // adjust x and z to be local to tile
        const int localTileX = Math::ICeil (localPageX - tileNumberX * tSize);
        const int localTileZ = Math::ICeil (localPageZ - tileNumberZ * tSize);
        
								
		#ifdef _DEBUGPOSHEIGHT		
		{
			RenderTarget *t = mOptions->primaryCamera->getViewport()->getTarget ();
			const String debugPos (" px: " + StringConverter::toString(pageNumberX) +
								" pz: " + StringConverter::toString(pageNumberZ) +
								" lpx: " + StringConverter::toString(localPageX) +
								" lpz: " + StringConverter::toString(localPageZ) +
								
								" tx: " + StringConverter::toString(tileNumberX) +
								" tz: " + StringConverter::toString(tileNumberZ) +
								" ltx: " + StringConverter::toString(localTileX) +
								" ltz: " + StringConverter::toString(localTileZ)
								
								);
			t->setDebugText(debugPos);
		}
		#endif // _DEBUG
		

        PagingLandScapeTile * const tile = page->getTile (tileNumberX, tileNumberZ);
        bool tileLoaded = tile && tile->isLoaded() && tile->getRenderable();
		const int currentRenderLevel = (tileLoaded) ? 
											tile->getRenderable()->getRenderLevel() 
											: 
											0;
			
			
		
			
		#ifdef _DEBUGPOSHEIGHTSTICH	
		String debugStich (		"(" + StringConverter::toString(pageNumberX) +
								", " + StringConverter::toString(pageNumberZ) +
								")(" + StringConverter::toString(tileNumberX) +
								", " + StringConverter::toString(tileNumberZ) +
								
								")(" + StringConverter::toString(localPageX) +
								", " + StringConverter::toString(localPageZ) +
								
								")(" + StringConverter::toString(localTileX) +
								", " + StringConverter::toString(localTileZ) +
								
								") rdl:" + StringConverter::toString(currentRenderLevel)
								
								);
		#endif // _DEBUGPOSHEIGHTSTICH
	
			
        // find the 4 vertices that surround the point
        // use LOD info to determine vertex data spacing - this is passed into the method
        // determine vertices on left and right of point and top and bottom
        // don't access VBO since a big performance hit when only 4 vertices are needed
        
        // Step from one vertex to another in the level detail algorithm
        const int currentLodStep = 1 << currentRenderLevel;
        //const int currentLodStep = 1;
        const Real inv_currentLodStep =  1.0f / currentLodStep;
		
		// check if can be on a stitched part of tile	
		const int tileSizeMinuscurrentLodStep = tSize + 1 - currentLodStep;	
		if (tileLoaded &&
			(localTileX <= currentLodStep ||
			localTileX >= tileSizeMinuscurrentLodStep ||
			localTileZ <= currentLodStep ||
			localTileZ >= tileSizeMinuscurrentLodStep))
		{
			
			// get stitching configuration
			PagingLandScapeRenderable * const r = tile->getRenderable();
			PagingLandScapeRenderable * neighbour;
			
			// get each tile LOD and if stitching occurs in this direction.
			neighbour = r->_getNeighbor(NORTH);
			const bool north = (neighbour && neighbour->isLoaded() 
								&& neighbour->getRenderLevel() > currentRenderLevel  
								&& localTileZ <= currentLodStep);//z-1
								
			neighbour = r->_getNeighbor(SOUTH);
			const bool south = (neighbour && neighbour->isLoaded() 
								&& neighbour->getRenderLevel() > currentRenderLevel 
								&& localTileZ >= tileSizeMinuscurrentLodStep);//z+1
			assert (north != south || (south == false && north == false));
			
			neighbour = r->_getNeighbor(EAST);
			const bool east = (neighbour && neighbour->isLoaded() 
								&& neighbour->getRenderLevel() > currentRenderLevel 
								&& localTileX >= tileSizeMinuscurrentLodStep);//x+1
								
			neighbour = r->_getNeighbor(WEST);
			const bool west = (neighbour && neighbour->isLoaded() 
								&& neighbour->getRenderLevel() > currentRenderLevel 
								&&  localTileX <= currentLodStep);//x-1
			assert (east != west || (east == false && west == false));
			
			if (north || south || east || west)
			{
			
				// Interpolate at LOD that uses stitching between tiles if needed.
				//
				//        lower LOD
				//		*-----------*
				//		|\  \ 3 /  /|
				//		|1\2 \ / 4/5|
				//		*--*--*--*--*
				//		  higher LOD
				//
				// we iterate over triangle to find which contains the points.
				// then we launch a ray to triangle intersection query.
				//
				// any simpler suggestion welcome. (gladly, this is hell long full of repetition code.)
				//
				
				const Vector2 pos (localPageX, localPageZ);
				int bottom_right_x,  bottom_right_z;
				Vector2 a2d, b2d ,c2d;
				
				if (north)
				{
					bool not_found = true;
					if (east)//northeast
					{			
						#ifdef _DEBUGPOSHEIGHTSTICH	
							debugStich += " north: " + StringConverter::toString(r->_getNeighbor(NORTH)->getRenderLevel()) +
									  " east: " + StringConverter::toString(r->_getNeighbor(EAST)->getRenderLevel());
						#endif //_DEBUGPOSHEIGHTSTICH
							
						// neighbour lod spread
						const int neighbour_currentLodStep = 1 << r->_getNeighbor(EAST)->getRenderLevel();
						// Step half way between low detail steps
						const int halfsuperstep = neighbour_currentLodStep >> 1;						
						const unsigned long neighbour_lod_mask = ~(neighbour_currentLodStep - 1);
						bottom_right_z = static_cast<int>(localPageZ) & neighbour_lod_mask;					
						const unsigned long lod_mask = ~(currentLodStep - 1);
						bottom_right_x = static_cast<int>(localPageX) & lod_mask;
						
						a2d = Vector2 (bottom_right_x + currentLodStep, bottom_right_z);
						b2d = Vector2 (bottom_right_x,					bottom_right_z + halfsuperstep);
						c2d = Vector2 (bottom_right_x + currentLodStep,	bottom_right_z + neighbour_currentLodStep);
						
						// biggest Middle tri
						if (!Math::pointInTri2D (pos.x, pos.y, a2d.x, a2d.y, b2d.x, b2d.y, c2d.x, c2d.y))
						{
							// all other smalls tri up to middle
							// but omit first triangle
							
							// omit first tri 
							a2d = Vector2 (bottom_right_x,					bottom_right_z);
							b2d = Vector2 (bottom_right_x,					bottom_right_z + currentLodStep);
							c2d = Vector2 (bottom_right_x + currentLodStep,	bottom_right_z);
							
							int step = 1;
							while (step < halfsuperstep &&
									!Math::pointInTri2D (pos.x, pos.y, a2d.x, a2d.y, b2d.x, b2d.y, c2d.x, c2d.y))
							{
								step += currentLodStep;
								
								a2d.y += currentLodStep;
								b2d.y += currentLodStep;
							}		
							// stops here, not looking after middle of higher lod.
							// middle point
							if (step < halfsuperstep)
								not_found = false;
						}
						else
						{
							not_found = false;
						}
											
					}
					else if (west)//northwest
					{										
						#ifdef _DEBUGPOSHEIGHTSTICH	
							debugStich += " north: " + StringConverter::toString(r->_getNeighbor(NORTH)->getRenderLevel()) +
									  " west: " + StringConverter::toString(r->_getNeighbor(WEST)->getRenderLevel());	
						#endif //_DEBUGPOSHEIGHTSTICH	
					
						// neighbour lod spread
						const int neighbour_currentLodStep = 1 << r->_getNeighbor(WEST)->getRenderLevel();
						// Step half way between low detail steps
						const int halfsuperstep = neighbour_currentLodStep >> 1;						
						const unsigned long neighbour_lod_mask = ~(neighbour_currentLodStep - 1);					
						bottom_right_z = static_cast<int>(localPageZ) & neighbour_lod_mask;
						const unsigned long lod_mask = ~(currentLodStep - 1);
						bottom_right_x = static_cast<int>(localPageX) & lod_mask;
						
						a2d = Vector2 (bottom_right_x,					bottom_right_z);
						b2d = Vector2 (bottom_right_x + currentLodStep,	bottom_right_z + halfsuperstep);
						c2d = Vector2 (bottom_right_x,					bottom_right_z + neighbour_currentLodStep);
						
						// biggest Middle tri
						if (!Math::pointInTri2D (pos.x, pos.y, a2d.x, a2d.y, b2d.x, b2d.y, c2d.x, c2d.y))
						{
							//all other small tri
							
							
							a2d = Vector2 (bottom_right_x + currentLodStep,	bottom_right_z);
							b2d = Vector2 (bottom_right_x + currentLodStep,	bottom_right_z + currentLodStep);
							c2d = Vector2 (bottom_right_x,					bottom_right_z);
							
							int step = halfsuperstep;
							
							// begins at middle point
							a2d.y += halfsuperstep;
							b2d.y += halfsuperstep;
							c2d.y += neighbour_currentLodStep;
							
							while (step < neighbour_currentLodStep &&
									!Math::pointInTri2D (pos.x, pos.y, a2d.x, a2d.y, b2d.x, b2d.y, c2d.x, c2d.y))
							{
								step += currentLodStep;
								
								a2d.y += currentLodStep;
								b2d.y += currentLodStep;
							}
							// stops here
							if (step < neighbour_currentLodStep)
								not_found = false;	
						}
						else
						{
							not_found = false;
						}				
					}
					if (not_found)
					{	
						// in north stitching
						#ifdef _DEBUGPOSHEIGHTSTICH
							debugStich += " north: " + StringConverter::toString(r->_getNeighbor(NORTH)->getRenderLevel());
						#endif //_DEBUGPOSHEIGHTSTICH
						
						// neighbour lod spread
						const int neighbour_currentLodStep = 1 << r->_getNeighbor(NORTH)->getRenderLevel();
						// Step half way between low detail steps
						const int halfsuperstep = neighbour_currentLodStep >> 1;							
						const unsigned long neighbour_lod_mask = ~(neighbour_currentLodStep - 1);						
						bottom_right_x = static_cast<int>(localPageX) & neighbour_lod_mask;						
						const unsigned long lod_mask = ~(currentLodStep - 1);
						bottom_right_z = static_cast<int>(localPageZ) & lod_mask;
					
						a2d = Vector2 (bottom_right_x,								bottom_right_z);
						b2d = Vector2 (bottom_right_x + halfsuperstep,				bottom_right_z + currentLodStep);
						c2d = Vector2 (bottom_right_x + neighbour_currentLodStep,	bottom_right_z);
					
						// biggest Middle tri
						if (!Math::pointInTri2D (pos.x, pos.y, a2d.x, a2d.y, b2d.x, b2d.y, c2d.x, c2d.y))
						{
							//all other small tri
							
							// could omit first and last tri ?
							int step = 0;
							
							a2d = Vector2 (bottom_right_x,					bottom_right_z + currentLodStep);
							b2d = Vector2 (bottom_right_x + currentLodStep,	bottom_right_z + currentLodStep);
							c2d = Vector2 (bottom_right_x,					bottom_right_z);
							while (step < halfsuperstep &&
									!Math::pointInTri2D (pos.x, pos.y, a2d.x, a2d.y, b2d.x, b2d.y, c2d.x, c2d.y))
							{
								step += currentLodStep;
								
								a2d.x += currentLodStep;
								b2d.x += currentLodStep;
							}			
							// middle point
							if (step >= halfsuperstep)
							{
								c2d.x += neighbour_currentLodStep;
								
								while (step < neighbour_currentLodStep &&
										!Math::pointInTri2D (pos.x, pos.y, a2d.x, a2d.y, b2d.x, b2d.y, c2d.x, c2d.y))
								{
									step += currentLodStep;
								
									a2d.x += currentLodStep;
									b2d.x += currentLodStep;
								}			 			 
							}					
						}
					}
				}
				else if (south)
				{
					bool not_found = true;
					if (east)//northeast
					{			
						#ifdef _DEBUGPOSHEIGHTSTICH	
							debugStich += " south: " + StringConverter::toString(r->_getNeighbor(SOUTH)->getRenderLevel()) +
									  " east: " + StringConverter::toString(r->_getNeighbor(EAST)->getRenderLevel());
						#endif //_DEBUGPOSHEIGHTSTICH
							
						// neighbour lod spread
						const int neighbour_currentLodStep = 1 << r->_getNeighbor(EAST)->getRenderLevel();
						// Step half way between low detail steps
						const int halfsuperstep = neighbour_currentLodStep >> 1;						
						const unsigned long neighbour_lod_mask = ~(neighbour_currentLodStep - 1);
						bottom_right_z = static_cast<int>(localPageZ) & neighbour_lod_mask;					
						const unsigned long lod_mask = ~(currentLodStep - 1);
						bottom_right_x = static_cast<int>(localPageX) & lod_mask;
						
						a2d = Vector2 (bottom_right_x + currentLodStep, bottom_right_z);
						b2d = Vector2 (bottom_right_x,					bottom_right_z + halfsuperstep);
						c2d = Vector2 (bottom_right_x + currentLodStep,	bottom_right_z + neighbour_currentLodStep);
						
						// biggest Middle tri
						if (!Math::pointInTri2D (pos.x, pos.y, a2d.x, a2d.y, b2d.x, b2d.y, c2d.x, c2d.y))
						{
							// all other smalls tri up to middle
							// but omit first triangle
							
							// omit first tri 
							a2d = Vector2 (bottom_right_x,					bottom_right_z);
							b2d = Vector2 (bottom_right_x,					bottom_right_z + currentLodStep);
							c2d = Vector2 (bottom_right_x + currentLodStep,	bottom_right_z);
							
							int step = 1;
							while (step < halfsuperstep &&
									!Math::pointInTri2D (pos.x, pos.y, a2d.x, a2d.y, b2d.x, b2d.y, c2d.x, c2d.y))
							{
								step += currentLodStep;
								
								a2d.y += currentLodStep;
								b2d.y += currentLodStep;
							}		
							// stops here, not looking after middle of higher lod.
							// middle point
							if (step < halfsuperstep)
								not_found = false;
						}
						else
						{
							not_found = false;
						}
											
					}
					else if (west)//northwest
					{										
						#ifdef _DEBUGPOSHEIGHTSTICH	
							debugStich += " south: " + StringConverter::toString(r->_getNeighbor(SOUTH)->getRenderLevel()) +
									  " west: " + StringConverter::toString(r->_getNeighbor(WEST)->getRenderLevel());	
						#endif //_DEBUGPOSHEIGHTSTICH	
					
						// neighbour lod spread
						const int neighbour_currentLodStep = 1 << r->_getNeighbor(WEST)->getRenderLevel();
						// Step half way between low detail steps
						const int halfsuperstep = neighbour_currentLodStep >> 1;						
						const unsigned long neighbour_lod_mask = ~(neighbour_currentLodStep - 1);					
						bottom_right_z = static_cast<int>(localPageZ) & neighbour_lod_mask;
						const unsigned long lod_mask = ~(currentLodStep - 1);
						bottom_right_x = static_cast<int>(localPageX) & lod_mask;
						
						a2d = Vector2 (bottom_right_x,					bottom_right_z);
						b2d = Vector2 (bottom_right_x + currentLodStep,	bottom_right_z + halfsuperstep);
						c2d = Vector2 (bottom_right_x,					bottom_right_z + neighbour_currentLodStep);
						
						// biggest Middle tri
						if (!Math::pointInTri2D (pos.x, pos.y, a2d.x, a2d.y, b2d.x, b2d.y, c2d.x, c2d.y))
						{
							//all other small tri
							
							
							a2d = Vector2 (bottom_right_x + currentLodStep,	bottom_right_z);
							b2d = Vector2 (bottom_right_x + currentLodStep,	bottom_right_z + currentLodStep);
							c2d = Vector2 (bottom_right_x,					bottom_right_z);
							
							int step = halfsuperstep;
							
							// begins at middle point
							a2d.y += halfsuperstep;
							b2d.y += halfsuperstep;
							c2d.y += neighbour_currentLodStep;
							
							while (step < neighbour_currentLodStep &&
									!Math::pointInTri2D (pos.x, pos.y, a2d.x, a2d.y, b2d.x, b2d.y, c2d.x, c2d.y))
							{
								step += currentLodStep;
								
								a2d.y += currentLodStep;
								b2d.y += currentLodStep;
							}
							// stops here
							if (step < neighbour_currentLodStep)
								not_found = false;	
						}
						else
						{
							not_found = false;
						}				
					}
					if (not_found)
					{	
						// south
						#ifdef _DEBUGPOSHEIGHTSTICH
							debugStich += " south: " + StringConverter::toString(r->_getNeighbor(SOUTH)->getRenderLevel());
						#endif //_DEBUGPOSHEIGHTSTICH
						
						// neighbour lod spread
						const int neighbour_currentLodStep = 1 << r->_getNeighbor(SOUTH)->getRenderLevel();
						// Step half way between low detail steps
						const int halfsuperstep = neighbour_currentLodStep >> 1;							
						const unsigned long neighbour_lod_mask = ~(neighbour_currentLodStep - 1);						
						bottom_right_x = static_cast<int>(localPageX) & neighbour_lod_mask;						
						const unsigned long lod_mask = ~(currentLodStep - 1);
						bottom_right_z = static_cast<int>(localPageZ) & lod_mask;
						
						a2d = Vector2 (bottom_right_x,								bottom_right_z + currentLodStep);
						b2d = Vector2 (bottom_right_x + halfsuperstep,				bottom_right_z);
						c2d = Vector2 (bottom_right_x + neighbour_currentLodStep,	bottom_right_z + currentLodStep);
					
						// biggest Middle tri
						if (!Math::pointInTri2D (pos.x, pos.y, a2d.x, a2d.y, b2d.x, b2d.y, c2d.x, c2d.y))
						{
							//all other small tri
							
							// could omit first and last tri ?
							int step = 0;
							
							a2d = Vector2 (bottom_right_x,					bottom_right_z);
							b2d = Vector2 (bottom_right_x + currentLodStep,	bottom_right_z);
							c2d = Vector2 (bottom_right_x,					bottom_right_z + currentLodStep);
							
							while (step < halfsuperstep &&
									!Math::pointInTri2D (pos.x, pos.y, a2d.x, a2d.y, b2d.x, b2d.y, c2d.x, c2d.y))
							{
								step += currentLodStep;
								
								a2d.x += currentLodStep;
								b2d.x += currentLodStep;
							}	
							// middle point
							if (step >= halfsuperstep)
							{
								c2d.x += neighbour_currentLodStep;
								
								while (step < neighbour_currentLodStep &&
										!Math::pointInTri2D (pos.x, pos.y, a2d.x, a2d.y, b2d.x, b2d.y, c2d.x, c2d.y))
								{
									step += currentLodStep;
								
									a2d.x += currentLodStep;
									b2d.x += currentLodStep;
								}			 				 			 
							}					
						}
					}
				}	
				else if (east) // just east
				{
					#ifdef _DEBUGPOSHEIGHTSTICH
						debugStich += " east: " + StringConverter::toString(r->_getNeighbor(EAST)->getRenderLevel());
					#endif //_DEBUGPOSHEIGHTSTICH
					
					// neighbour lod spread
					const int neighbour_currentLodStep = 1 << r->_getNeighbor(EAST)->getRenderLevel();
					// Step half way between low detail steps
					const int halfsuperstep = neighbour_currentLodStep >> 1;						
					const unsigned long neighbour_lod_mask = ~(neighbour_currentLodStep - 1);
					bottom_right_z = static_cast<int>(localPageZ) & neighbour_lod_mask;					
					const unsigned long lod_mask = ~(currentLodStep - 1);
					bottom_right_x = static_cast<int>(localPageX) & lod_mask;
					
					a2d = Vector2 (bottom_right_x + currentLodStep,		bottom_right_z);
					b2d = Vector2 (bottom_right_x,						bottom_right_z + halfsuperstep);
					c2d = Vector2 (bottom_right_x + currentLodStep,		bottom_right_z + neighbour_currentLodStep);
					
					// biggest Middle tri
					if (!Math::pointInTri2D (pos.x, pos.y, a2d.x, a2d.y, b2d.x, b2d.y, c2d.x, c2d.y))
					{
						//all other small tri
						a2d = Vector2 (bottom_right_x,					bottom_right_z);
						b2d = Vector2 (bottom_right_x,					bottom_right_z + currentLodStep);
						c2d = Vector2 (bottom_right_x + currentLodStep,	bottom_right_z);
						
						int step = 0;
						while (step < halfsuperstep &&
								!Math::pointInTri2D (pos.x, pos.y, a2d.x, a2d.y, b2d.x, b2d.y, c2d.x, c2d.y))
						{
							step += currentLodStep;
							
							a2d.y += currentLodStep;
							b2d.y += currentLodStep;
						}			
						// middle point
						if (step >= halfsuperstep)
						{
							c2d.y += neighbour_currentLodStep;
						
							while (step < neighbour_currentLodStep &&
									!Math::pointInTri2D (pos.x, pos.y, a2d.x, a2d.y, b2d.x, b2d.y, c2d.x, c2d.y))
							{
								step += currentLodStep;
								
								a2d.y += currentLodStep;
								b2d.y += currentLodStep;
							}			 			 
						}	
					}
				}
				else if (west) // just west
				{
					#ifdef _DEBUGPOSHEIGHTSTICH
						debugStich += " west: " + StringConverter::toString(r->_getNeighbor(WEST)->getRenderLevel());
					#endif //_DEBUGPOSHEIGHTSTICH
					
					// neighbour lod spread
					const int neighbour_currentLodStep = 1 << r->_getNeighbor(WEST)->getRenderLevel();
					// Step half way between low detail steps
					const int halfsuperstep = neighbour_currentLodStep >> 1;						
					const unsigned long neighbour_lod_mask = ~(neighbour_currentLodStep - 1);					
					bottom_right_z = static_cast<int>(localPageZ) & neighbour_lod_mask;
					const unsigned long lod_mask = ~(currentLodStep - 1);
					bottom_right_x = static_cast<int>(localPageX) & lod_mask;
					
					a2d = Vector2 (bottom_right_x,					bottom_right_z);
					b2d = Vector2 (bottom_right_x + currentLodStep,	bottom_right_z + halfsuperstep);
					c2d = Vector2 (bottom_right_x,					bottom_right_z + neighbour_currentLodStep);
					
					// biggest Middle tri
					if (!Math::pointInTri2D (pos.x, pos.y, a2d.x, a2d.y, b2d.x, b2d.y, c2d.x, c2d.y))
					{
						//all other small tri
						
						
						a2d = Vector2 (bottom_right_x + currentLodStep,	bottom_right_z);
						b2d = Vector2 (bottom_right_x + currentLodStep,	bottom_right_z + currentLodStep);
						c2d = Vector2 (bottom_right_x,					bottom_right_z);
						
						int step = 0;
						while (step < halfsuperstep &&
								!Math::pointInTri2D (pos.x, pos.y, a2d.x, a2d.y, b2d.x, b2d.y, c2d.x, c2d.y))
						{
							step += currentLodStep;
							
							a2d.y += currentLodStep;
							b2d.y += currentLodStep;
						}			
						// middle point
						if (step >= halfsuperstep)
						{
							c2d.y += neighbour_currentLodStep;
						
							while (step < neighbour_currentLodStep &&
									!Math::pointInTri2D (pos.x, pos.y, a2d.x, a2d.y, b2d.x, b2d.y, c2d.x, c2d.y))
							{
								step += currentLodStep;
								
								a2d.y += currentLodStep;
								b2d.y += currentLodStep;
							}			 			 
						}	
					}	
				}			
					
				const Vector3 a (a2d.x, getHeightAtPage (pageNumberX, pageNumberZ, a2d.x, a2d.y), a2d.y);
				const Vector3 b (b2d.x, getHeightAtPage (pageNumberX, pageNumberZ, b2d.x, b2d.y), b2d.y);
				const Vector3 c (c2d.x, getHeightAtPage (pageNumberX, pageNumberZ, c2d.x, c2d.y), c2d.y);	
					
					 
				#ifdef _DEBUGPOSHEIGHTSTICH	
					 const Real shiftx = pageNumberX*pSize - mOptions->maxUnScaledX;
					 const Real shiftz = pageNumberZ*pSize - mOptions->maxUnScaledZ;
			         
					 const Vector3 A ((a.x + shiftx) * mOptions->scale.x,						
									   a.y, 
									   (a.z  + shiftz) * mOptions->scale.z);
					 const Vector3 B ((b.x + shiftx) * mOptions->scale.x,	
									   b.y,	
									  (b.z  + shiftz) * mOptions->scale.z);
					 const Vector3 C ((c.x + shiftx) * mOptions->scale.x,						
									  c.y,	
									  (c.z + shiftz) * mOptions->scale.z);
										 
					 static bool debugged_stitch_none = true;
					 
					 static SceneNode* Anode = 0; 
					 static SceneNode* Bnode = 0;
					 static SceneNode* Cnode = 0;
			         
					 if (debugged_stitch_none)
					 {
						const String name("TriDebugSphere");
						Entity* entity = PagingLandScapeSceneManager::getSingleton().createEntity(name, "sphere.mesh"); 
						SceneNode* Rootnode = PagingLandScapeSceneManager::getSingleton().getRootSceneNode();
			            
						Entity *e;
						Anode = Rootnode->createChildSceneNode(name + "A"); 
						e = entity->clone(name + "A");
						Anode->attachObject(e); 
						Anode->setScale(0.1f, 0.1f, 0.1f);                         
						Anode->showBoundingBox(true);
						e->getMesh()->getSubMesh(0)->setMaterialName("BaseRedNoLightNoDepthCheck");
			            
						Bnode = Rootnode->createChildSceneNode(name + "B"); 
						e = entity->clone(name + "B");
						Bnode->attachObject(e); 
						Bnode->setScale(0.1f, 0.1f, 0.1f);                         
						Bnode->showBoundingBox(true);
						e->getMesh()->getSubMesh(0)->setMaterialName ("BaseRedNoLightNoDepthCheck");
			            
			            
						Cnode = Rootnode->createChildSceneNode(name + "C"); 
						e = entity->clone(name + "C");
						Cnode->attachObject(e); 
						Cnode->setScale(0.1f, 0.1f, 0.1f);                         
						Cnode->showBoundingBox(true);
						e->getMesh()->getSubMesh(0)->setMaterialName ("BaseRedNoLightNoDepthCheck");
			            
						debugged_stitch_none = false;
					}
			        
					Anode->setPosition(A); 
					Bnode->setPosition(B); 
					Cnode->setPosition(C); 
					
				#endif //_DEBUGPOSHEIGHTSTICH	
				
					
				
				const Ray triIntersect (Vector3(localPageX, getMaxHeight(), localPageZ), Vector3::NEGATIVE_UNIT_Y);
				
				std::pair<bool, Real> res = Math::intersects(triIntersect, Plane(a, b, c));
				//std::pair<bool, Real> res = Math::intersects(triIntersect, a, b, c, true, true);	
				if (res.first)
					heightValue = getMaxHeight() - res.second;
				else
					heightValue = 0.0f;

            if (slope)
               {
               Vector3 result;
               Vector3 vector1;
               Vector3 vector2;

               vector1 = a - b;
               vector2 = b - c;
               result.x = (vector1.y * vector2.z) - (vector1.z * vector2.y);
               result.y = (vector1.z * vector2.x) - (vector1.x * vector2.z);
               result.z = (vector1.x * vector2.y) - (vector1.y * vector2.x);

//               result.x = ((top_right_y - top_left_y) * (-mOptions->scale.z));
//               result.y = mOptions->scale.x * (-mOptions->scale.z);
//               result.z = -mOptions->scale.x * (bottom_left_y - top_left_y);

               result.normalise();
               *slope = 1.0 + result.y;
               if (fabs(*slope) < 0.001f)
                  *slope = 0.0f;
               }

            return heightValue;
			}
		}
		
		// no stitching, easy life... 
		//
		//   TL-----TR 1.0
		//   |     / |
		//   |    /  |  .
		//   |   /   |  .
		//   |  /    |  .   ^
		//   | /     |      |
		//   BL-----BR 0.0  z
		//  1.0 ... 0.0
		//
		//      < - x
		//
		//  Where 1.0 is currentLodStep
		//
		const unsigned long lod_mask = ~(currentLodStep - 1);
		
		const int bottom_right_x = static_cast<int>(localPageX) & lod_mask;
		const int bottom_right_z = static_cast<int>(localPageZ) & lod_mask;
		const int top_right_x = bottom_right_x + currentLodStep; 
		const int top_right_z = bottom_right_z + currentLodStep;
		
		
		#ifdef _DEBUGPOSHEIGHTSTICH	
			if (1)
			{
				RenderTarget *t = mOptions->primaryCamera->getViewport()->getTarget ();
				const String Debugtext (debugStich);
				t->setDebugText (debugStich);
			}
		#endif //_DEBUGPOSHEIGHTSTICH	
			
      
        // find the 4 heights around the point
         const Real bottom_right_y = getHeightAtPage (pageNumberX, pageNumberZ, bottom_right_x , bottom_right_z);
         const Real bottom_left_y  = getHeightAtPage (pageNumberX, pageNumberZ, top_right_x,	 bottom_right_z);
         const Real top_right_y    = getHeightAtPage (pageNumberX, pageNumberZ, bottom_right_x,  top_right_z);
         const Real top_left_y     = getHeightAtPage (pageNumberX, pageNumberZ, top_right_x,	 top_right_z);
         		
         
		#ifdef _DEBUGPOSHEIGHTSTICH	
			 const Real shiftx = pageNumberX*pSize - mOptions->maxUnScaledX;
			 const Real shiftz = pageNumberZ*pSize - mOptions->maxUnScaledZ;
	         
			 const Vector3 BR((bottom_right_x + shiftx) * mOptions->scale.x,						
								 bottom_right_y, 
								 (bottom_right_z + shiftz) * mOptions->scale.z);
			 const Vector3 BL((top_right_x + shiftx) * mOptions->scale.x,	
								 bottom_left_y,	
								 (bottom_right_z  + shiftz) * mOptions->scale.z);
			 const Vector3 TR((bottom_right_x + shiftx) * mOptions->scale.x,						
								 top_right_y,	
								 (top_right_z + shiftz) * mOptions->scale.z);
			 const Vector3 TL((top_right_x + shiftx) * mOptions->scale.x,	
								 top_left_y,		
								 (top_right_z + shiftz) * mOptions->scale.z);
	         
			 static bool debugged_none = true;
			 
			 static SceneNode* BRnode = 0; 
			 static SceneNode* BLnode = 0;
			 static SceneNode* TRnode = 0; 
			 static SceneNode* TLnode = 0; 
	         
			 if (debugged_none)
			 {
				const String name("DebugSphere");
				Entity* entity = PagingLandScapeSceneManager::getSingleton().createEntity(name, "sphere.mesh"); 
				SceneNode* Rootnode = PagingLandScapeSceneManager::getSingleton().getRootSceneNode();
	             
				BRnode = Rootnode->createChildSceneNode(name + "BR"); 
				BRnode->attachObject(entity->clone(name + "BR")); 
				BRnode->setScale(0.1f, 0.1f, 0.1f);                         
				BRnode->showBoundingBox(true);
	            
				BLnode = Rootnode->createChildSceneNode(name + "BL"); 
				BLnode->attachObject(entity->clone(name + "BL")); 
				BLnode->setScale(0.1f, 0.1f, 0.1f);                         
				BLnode->showBoundingBox(true);
	            
	            
				TRnode = Rootnode->createChildSceneNode(name + "TR"); 
				TRnode->attachObject(entity->clone(name + "TR")); 
				TRnode->setScale(0.1f, 0.1f, 0.1f);                         
				TRnode->showBoundingBox(true);
	            
				TLnode = Rootnode->createChildSceneNode(name + "TL"); 
				TLnode->attachObject(entity->clone(name + "TL")); 
				TLnode->setScale(0.1f, 0.1f, 0.1f);                         
				TLnode->showBoundingBox(true);
	            
				debugged_none = false;
			}
	        
			BRnode->setPosition(BR); 
			BLnode->setPosition(BL); 
			TRnode->setPosition(TR); 
			TLnode->setPosition(TL); 
			
			if (0)
			{
				RenderTarget *t = mOptions->primaryCamera->getViewport()->getTarget ();
				const String Debugtext ("(X= " +  StringConverter::toString(localPageX) + " Z=" +
								StringConverter::toString(localPageZ) + "), (br_x=" +
								StringConverter::toString(bottom_right_x) + ", br_z=" +
								StringConverter::toString(bottom_right_z) + ") & spread=" +
								StringConverter::toString(currentLodStep)+ ") & lod=" +
								StringConverter::toString(currentRenderLevel));
				t->setDebugText(Debugtext);
			}
	    #endif //_DEBUGPOSHEIGHTSTICH	
        
        const Real z_pct = (localPageZ - bottom_right_z) * inv_currentLodStep;
        Real x_pct = (localPageX - bottom_right_x) * inv_currentLodStep;
 
         if (x_pct > 1 - z_pct)
         { 
            // This point is on the upper-left tri
             const Real y1 = bottom_left_y * (1-z_pct) + top_left_y  * z_pct;
             const Real y2 = bottom_left_y * (1-z_pct) + top_right_y * z_pct;
              if (z_pct > 0.0f)
                 x_pct = (x_pct - (1-z_pct)) / z_pct;

              if (slope)
                 {
                 Vector3 result;
                 result.x = ((top_right_y - top_left_y) * (-mOptions->scale.z));
                 result.y = mOptions->scale.x * (-mOptions->scale.z);
                 result.z = -mOptions->scale.x * (bottom_left_y - top_left_y);
                 result.normalise();
                 *slope = 1.0 + result.y;
                 if (fabs(*slope) < 0.001f)
                    *slope = 0.0f;
                 }

             return y1 * x_pct + y2 * (1-x_pct);
         } // if (x_pct > 1 - z_pct)
         else
         { 
            // This point is on the lower-right tri
             const Real y1 = bottom_left_y  * (1-z_pct) + top_right_y * z_pct;
             const Real y2 = bottom_right_y * (1-z_pct) + top_right_y * z_pct;

             if (z_pct < 1.0f)
                 x_pct = x_pct / (1-z_pct);

             if (slope)
                {
                Vector3 result;
                result.x = (bottom_left_y - bottom_right_y) * mOptions->scale.z;
                result.y = -mOptions->scale.x * mOptions->scale.z;
                result.z = mOptions->scale.x * (top_right_y - bottom_right_y);
                result.normalise();
                *slope = 1.0 + result.y;
                if (fabs(*slope) < 0.001f)
                   *slope = 0.0f;
                }

             return y1 * x_pct + y2 * (1-x_pct);
         } 

    }
    //-----------------------------------------------------------------------
    const Real PagingLandScapeData2DManager::getWorldHeight(const Real x, const Real z)
    {
	    // figure out which page the point is on
        uint pageX, pageZ;
        if (mPageManager->getPageIndices(x, z, pageX, pageZ, false))
		{
			PagingLandScapeData2D *data = getData2D (pageX, pageZ, false);
			if (data && data->isLoaded())
            {
                // scale position from world to page scale
                Real localX = x / mOptions->scale.x;
                Real localZ = z / mOptions->scale.z;

                // adjust x and z to be local to page
                const Real pSize = mOptions->PageSize - 1;

                localX -= pageX*pSize - mOptions->maxUnScaledX;
                localZ -= pageZ*pSize - mOptions->maxUnScaledZ;

                return getHeightAtPage (pageX, pageZ, static_cast<int> (localX), static_cast<int> (localZ));   
            }
        }
        return 0.0f;
    }
    //-----------------------------------------------------------------------
    const Real PagingLandScapeData2DManager::getHeightAtPage(const uint dataX, const uint dataZ, 
                                                        const Real x, const Real z) 
    {
        PagingLandScapeData2D* data = getData2D (dataX, dataZ, false );
        if (data && data->isLoaded())
        {
            Real lX = x;
            Real lZ = z;
            const Real pSize = mOptions->PageSize - 1;
			PagingLandScapeData2D* srcdata = data;
            //check if we have to change current page
            if (lX < 0.0f)
            { 
                if (dataX == 0)
                    lX = 0.0f;
                else
                {
					data = getData2D (dataX - 1 , dataZ, false );
					if (data && data->isLoaded())
                    {
                        lX = static_cast<Real> (pSize);
                    }
                    else
                    {
                        lX = 0.0f;
                        data = srcdata;
                    }
                }
              
            }
            else if (lX > pSize)
            {
                if (dataX == mOptions->world_width - 1)
                    lX = static_cast<Real> (pSize);
                else
				{
					data = getData2D (dataX + 1 , dataZ, false );
					if (data && data->isLoaded())
                    {
                        lX = 0.0f;
                    }
                    else
                    {
                        lX = static_cast<Real> (pSize);
                        data = srcdata;
                    }
                }
            }

            if (lZ < 0.0f)
            {
                if (dataZ == 0)
                    lZ = 0.0f;
                else
				{
					data = getData2D (dataX, dataZ - 1, false );
					if (data && data->isLoaded())
                    {
                        lZ = static_cast<Real> (pSize);
                    }
                    else
                    {
                        lZ = 0.0f;
                        data = srcdata;
                    }
                }
            }
            else if (lZ > pSize)
            {
                if (dataZ == mOptions->world_height - 1)
                    lZ = static_cast<Real> (pSize);
                else
				{
					data = getData2D (dataX, dataZ + 1, false );
					if (data && data->isLoaded())
                    {
                        lZ = 0.0f;
                    }
                    else
                    {
                        lZ = static_cast<Real> (pSize);
                        data = srcdata;
                    }
                }
            }
            assert (data);
            return data->getHeight (lX, lZ);
        }
        return 0.0f;
    }
    //-----------------------------------------------------------------------
    const Real PagingLandScapeData2DManager::getHeightAtPage(const uint dataX, const uint dataZ, 
                                                        const int x, const int z) 
    {
		PagingLandScapeData2D* data = getData2D (dataX, dataZ, false );
		if (data && data->isLoaded())
        {
            int lX = x;
            int lZ = z;
			const int pSize = mOptions->PageSize - 1;
			PagingLandScapeData2D* srcdata = data;
            //check if we have to change current page
            if (lX < 0)
            { 
                if (dataX == 0)
                    lX = 0;
                else
				{
					data = getData2D (dataX - 1,  dataZ, false );
					if (data && data->isLoaded())
                    {
                        lX = pSize;
                    }
                    else
                    {
                        lX = 0;
                        data = srcdata;
                    }
                }
              
            }
            else if (lX > pSize)
            {
                if (dataX == mOptions->world_width - 1)
                    lX = pSize;
                else
				{
					data = getData2D (dataX + 1,  dataZ, false );
					if (data && data->isLoaded())
                    {
                        lX = 0;
                    }
                    else
                    {
                        lX = pSize;
                        data = srcdata;
                    }
                }
            }

            if (lZ < 0)
            {
                if (dataZ == 0)
                    lZ = 0;
                else
				{
					data = getData2D (dataX,  dataZ - 1, false );
					if (data && data->isLoaded())
                    {
                        lZ = pSize;
                    }
                    else
                    {
                        lZ = 0;
                        data = srcdata;
                    }
                }
            }
            else if (lZ > pSize)
            {
                if (dataZ == mOptions->world_height - 1)
                    lZ = pSize;
                else
				{
					data = getData2D (dataX,  dataZ + 1, false );
					if (data && data->isLoaded())
                    {
                        lZ = 0;
                    }
                    else
                    {
                        lZ = pSize;
                        data = srcdata;
                    }
                }
            }
            assert (data);
            return data->getHeight (lX, lZ);
        }
        else
        {
            // not a border demand and we do not have the data.
            return 0.0f;
        }  
    }


    //-----------------------------------------------------------------------
    const Vector3 PagingLandScapeData2DManager::getNormalAt(const uint dataX, const uint dataZ, 
                                                            const uint x, const uint z)
    {
		PagingLandScapeData2D* data = getData2D (dataX,  dataZ, false );
		if (data && data->isLoaded())
	    {
            #ifdef _LOADEDNORM  
                // not as precise (rgb normals gives 8 bits precision Real)
                // and finally nearly as fast.
                return data->getNormalAt (x, z);
            #else /*_LOADEDNORM*/
            {
                // First General method : (9 adds and 6 muls + a normalization)
                //        *---v3--*
                //        |   |   |
                //        |   |   |
                //        v1--X--v2
                //        |   |   |
                //        |   |   |
                //        *---v4--*
                //
                //        U = v2 - v1;
                //        V = v4 - v3;
                //        N = Cross(U, V);
                //        N.normalise;
                //
                // BUT IN CASE OF A HEIGHTMAP : 
                //
                //   if you do some math by hand before you code, 
                //   you can see that N is immediately given by 
                //  Approximation (2 adds and a normalization)
                // 
                //        N = Vector3(z[x-1][y] - z[x+1][y], z[x][y-1] - z[x][y+1], 2); 
                //        N.normalise();
                //
                // or even using SOBEL operator VERY accurate! 
                // (14 adds and a normalization)
                //
                //       N = Vector3 (z[x-1][y-1] + z[x-1][y] + z[x-1][y] + z[x-1][y+1] - z[x+1][y-1] - z[x+1][y] - z[x+1][y] - z[x+1][y+1], 
                //                     z[x-1][y-1] + z[x][y-1] + z[x][y-1] + z[x+1][y-1] - z[x-1][y+1] - z[x][y+1] - z[x][y+1] - z[x+1][y+1], 
                //                     8);
                //       N.normalize();

            
                // Fast SOBEL filter

                // the divider make sure we do respect proportion  (height and width proportional to y)
                const Real Divider = static_cast <Real> (mOptions->PageSize - 1) / PagingLandScapeOptions::getSingleton ().scale.y;

                #define  gH(a, b) (getHeightAtPage(dataX, dataZ, static_cast<int> (a), static_cast<int> (b)))

//              Vector3 result ((gH(x-1,z-1) + gH (x-1, z) + gH (x-1, z) + gH (x-1, z+1) - gH (x+1, z-1) - gH (x+1, z) - gH (x+1, z) - gH (x+1, z+1)) * Divider, 
//                              8.0f, 
//                              (gH (x-1, z-1) + gH (x, z-1) + gH (x, z-1) + gH (x+1, z-1) - gH (x-1, z+1) - gH (x, z+1) - gH (x, z+1) - gH (x+1, z+1)) * Divider);

                Vector3 result((gH (x - 1 , z)     - gH (x + 1 , z)) * Divider,
                                2.0f,
                               (gH (x,         z - 1) - gH (x        , z + 1)) * Divider); 

                result.normalise ();

                return result;
            }
            #endif //_NOLOAD
	    }
	    return Vector3::UNIT_Y;
    }
    //----------------------------------------------------------------------------
    const Real PagingLandScapeData2DManager::getRealWorldSlope(const Real x, const Real z)
    {
        // figure out which page the point is on
        const Vector3 pos(x, 0, z);
        uint pageX, pageZ;
        if (mPageManager->getPageIndices(pos.x, pos.z, pageX, pageZ, false))
        {

			PagingLandScapeData2D* data = getData2D (pageX,  pageZ, false);
			if (data && data->isLoaded())
            {
                // figure out which tile the point is on
                PagingLandScapeTile *t = mPageManager->getTile(pos.x, pos.z, 
                                                                pageX, pageZ, 
                                                                false);
                uint Lod = 0;
                if (t && t->isLoaded())
                    Lod = t->getRenderable()->getRenderLevel();
                const Vector3 normalVector = getNormalAt(pageX, pageZ, x, z, Lod);
                const Real slope = 1.0f + normalVector.y;
                if (slope < 0.001f)
                    return 0.0f;
                else
                    return slope;
            }
        }
        return 0.0f;
    }

    //-----------------------------------------------------------------------
    const Vector3 PagingLandScapeData2DManager::getNormalAt(const uint pageX, const uint pageZ, const uint x, const uint z, const uint Lod)
    {
        Vector3  result;

        // scale position from world to page scale
        Real localX = x / mOptions->scale.x;
        Real localZ = z / mOptions->scale.z;

        // adjust x and z to be local to page
        const Real pSize = mOptions->PageSize - 1;

        localX -= pageX * pSize - mOptions->maxUnScaledX;
        localZ -= pageZ * pSize - mOptions->maxUnScaledZ;

        // make sure x and z do not go outside the world boundaries
        if (localX < 0)
            localX = 0;
        else if (localX >= pSize) 
            localX = pSize - 1;

        if (localZ < 0)
            localZ = 0;
        else if (localZ >= pSize)
            localZ = pSize - 1;

        // find the 4 vertices that surround the point
        // use LOD info to determine vertex data spacing - this is passed into the method
        // determine vertices on left and right of point and top and bottom
        // don't access VBO since a big performance hit when only 4 vertices are needed
        const int currentLodStep = 1 << Lod;
        const Real inv_currentLodStep =  1.0f / currentLodStep;

        // find the vertex to the 'bottom right' of the point
        const unsigned long lod_mask = ~(currentLodStep - 1);
        const int bottom_right_x = static_cast<int>(localX) & lod_mask;
        const int bottom_right_z = static_cast<int>(localZ) & lod_mask; 

        // find the 4 heights around the point
        const Real bottom_right_y = getHeightAtPage(pageX, pageZ, bottom_right_x ,                bottom_right_z);
        const Real bottom_left_y  = getHeightAtPage(pageX, pageZ, bottom_right_x + currentLodStep, bottom_right_z);
        const Real top_right_y    = getHeightAtPage(pageX, pageZ, bottom_right_x,                 bottom_right_z + currentLodStep);
        const Real top_left_y     = getHeightAtPage(pageX, pageZ, bottom_right_x + currentLodStep, bottom_right_z + currentLodStep);

        const Real z_pct = (localZ - bottom_right_z) * inv_currentLodStep;
        Real x_pct = (localX - bottom_right_x) * inv_currentLodStep;
        //   TL-----TR 1.0
        //   |     / |
        //   |    /  |  .
        //   |   /   |  .
        //   |  /    |  .   ^
        //   | /     |      |
        //   BL-----BR 0.0  z
        //  1.0 ... 0.0
        //
        //      < - x

        if (x_pct > 1 - z_pct)
            {
            // This point is on the upper-left tri
            /*
            Vector3 vector1;
            Vector3 vector2;

            vector1.x = -mOptions->scale.x;
            vector1.y = top_right_y - top_left_y;
            vector1.z = 0;
            vector2.x = 0;
            vector2.y = bottom_left_y - top_left_y;
            vector2.z = -mOptions->scale.z;

            result.x = (vector1.y * vector2.z) - (vector1.z * vector2.y);
            result.y = (vector1.z * vector2.x) - (vector1.x * vector2.z);
            result.z = (vector1.x * vector2.y) - (vector1.y * vector2.x);
            */
            // The formulas for calculating result are simplified from the above
            // commented-out code for computing the cross product.
            result.x = ((top_right_y - top_left_y) * (-mOptions->scale.z));
            result.y = mOptions->scale.x * (-mOptions->scale.z);
            result.z = -mOptions->scale.x * (bottom_left_y - top_left_y);
            }
        else
            {
            // This point is on the lower-right tri
            /*
            Vector3 vector1;
            Vector3 vector2;

            vector1.x = mOptions->scale.x;
            vector1.y = bottom_left_y - bottom_right_y;
            vector1.z = 0;
            vector2.x = 0;
            vector2.y = top_right_y - bottom_right_y;
            vector2.z = mOptions->scale.z;

            result.x = (vector1.y * vector2.z) - (vector1.z * vector2.y);
            result.y = (vector1.z * vector2.x) - (vector1.x * vector2.z);
            result.z = (vector1.x * vector2.y) - (vector1.y * vector2.x);
            */
            // The formulas for calculating result are simplified from the above
            // commented-out code for computing the cross product.
            result.x = (bottom_left_y - bottom_right_y) * mOptions->scale.z;
            result.y = -mOptions->scale.x * mOptions->scale.z;
            result.z = mOptions->scale.x * (top_right_y - bottom_right_y);
            }

        result.normalise();
        return result;
      }
    //-----------------------------------------------------------------------  
    Real PagingLandScapeData2DManager::getMaxSlope(Vector3 location1, Vector3 location2, Real maxSlopeIn)
    {
        int      testCount;
        int      testIndex;
        Real     maxSlope;
        Real     nextSlope;
        Vector3  unitVector;
        Vector3  iterationOffset;
        Vector3  nextLocation;
        Real     halfWorldSizeX;
        Real     halfWorldSizeZ;
        Real     distBetweenPoints;

        //      worldSizeX = (float)mOptions->world_width * mOptions->scale.x;
        //      worldSizeZ = (float)mOptions->world_height * mOptions->scale.z;
        halfWorldSizeX = mOptions->maxScaledX;
        halfWorldSizeZ = mOptions->maxScaledZ;

        if ((location1[0] > halfWorldSizeX) || (location1[2] > halfWorldSizeZ) || 
            (location2[0] > halfWorldSizeX) || (location2[2] > halfWorldSizeZ) ||
            (location1[0] < -halfWorldSizeX) || (location1[2] < -halfWorldSizeZ) || 
            (location2[0] < -halfWorldSizeX) || (location2[2] < -halfWorldSizeZ))
        {
            // One or both endpoints is off the edge of the world.  Return max slope.
            return 1.0f;
        }

        getInterpolatedWorldHeight(location2.x, location2.z, &maxSlope);
//        maxSlope = getRealWorldSlope(location2.x, location2.z);
        if (maxSlope > maxSlopeIn)
            return maxSlope;

        unitVector.x = location2.x - location1.x;
        unitVector.y = 0.0f;
        unitVector.z = location2.z - location1.z;

        // before normalizing our unitVector, calculate the distance between the endpoints
        distBetweenPoints = (Real)sqrt((unitVector.x * unitVector.x) + (unitVector.z * unitVector.z));
//        testCount = (int)(distSquared / (mOptions->scale.x * mOptions->scale.x) / 16.0f);
        testCount = (int)((distBetweenPoints / mOptions->scale.x) * 4.0f);

        unitVector.normalise();
        iterationOffset.x = unitVector.x * (mOptions->scale.x / 4.0f);
        iterationOffset.z = unitVector.z * (mOptions->scale.z / 4.0f);

        nextLocation.x = location1.x;
        nextLocation.y = location1.y;
        nextLocation.z = location1.z;

        for (testIndex = 0; testIndex < testCount; testIndex++)
        {
            getInterpolatedWorldHeight(nextLocation.x, nextLocation.z, &nextSlope);
//            nextSlope = getRealWorldSlope(nextLocation.x, nextLocation.z);
            if (nextSlope > maxSlopeIn)
                return nextSlope;

            if (nextSlope > maxSlope)
                maxSlope = nextSlope;

            nextLocation.x += iterationOffset.x;
            nextLocation.z += iterationOffset.z;
        }

        return maxSlope;
    }

 

} //namespace
