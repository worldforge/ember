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

#include "OgreVector3.h"
#include "OgreColourValue.h"

#include "OgreMovableObject.h"
#include "OgreAxisAlignedBox.h"

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

// needed to get RenderLevel for RealHeight
#include "OgrePagingLandScapeRenderable.h"
#include "OgrePagingLandScapeTile.h"
#include "OgrePagingLandScapeTileInfo.h"
#include "OgrePagingLandScapePage.h"
#include "OgrePagingLandScapePageManager.h"


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
	    assert( ms_Singleton );  return ( *ms_Singleton );  
    }

    //-----------------------------------------------------------------------
    PagingLandScapeData2DManager::PagingLandScapeData2DManager( ) :
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
        registerDataType (new PagingLandScapeData2D_Spline());
    }
    //-----------------------------------------------------------------------
    PagingLandScapeData2D *PagingLandScapeData2DManager::allocateData2D() const
    {        
       return mData2DTypeMap[mData2DType]->newPage();
    }    
    //-----------------------------------------------------------------------
    PagingLandScapeData2DManager::~PagingLandScapeData2DManager( )
    {      
        // for all in map delete.
        PagingLandScapeData2DMap::iterator i = mData2DTypeMap.begin();
        while (i != mData2DTypeMap.end())
        {
            delete (*i);
            ++i;
        }
        PagingLandScapeData2DManager::reset();
    }       
    //-----------------------------------------------------------------------
    void PagingLandScapeData2DManager::reset()
    {
       for (uint i = 0; i <  mWidth; ++i)
	    {
            PagingLandScapeData2DRow  *li = &mData2D[ i ];
            for (uint k = 0; k < mHeight; ++k)
		    {
                PagingLandScapeData2D *d = (*li)[ k ];
                d->uninit();
                delete (d);
		    }            
            li->clear();
	    }
	    mData2D.clear();
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
                OGRE_EXCEPT( Exception::ERR_INVALIDPARAMS, 
                        "PageData2D not supplied or wrong (check case) !",
		                " PagingLandScapeData2DManager::load" );
            }
        }
        WorldDimensionChange();
        mMaxHeight = mData2D[ 0 ][ 0 ]->getMaxHeight ();
    }    
    //-----------------------------------------------------------------------
    void PagingLandScapeData2DManager::clear ()
    {
//        PagingLandScapeData2DPages::iterator iend = mData2D.end();
//        for (PagingLandScapeData2DPages::iterator i = mData2D.begin(); i != iend; ++i)
//        {
//            
//            PagingLandScapeData2DRow *data2dRow = &(*i);
//            PagingLandScapeData2DRow::iterator k, 
//                                            ktbegin = data2dRow->begin (),
//                                            ktend = data2dRow->end ();
//
//            for (k = ktbegin; k != ktend; ++k)
//                (*k)->uninit();
//
////             std::for_each( i->begin (), 
////                            i->end (),  
////                             std::mem_fun(&PagingLandScapeData2D::uninit));
//           
//        }
    }   
    //-----------------------------------------------------------------------
    void PagingLandScapeData2DManager::WorldDimensionChange()
    {
        const uint newWidth = mOptions->world_width; 
        const uint newHeight = mOptions->world_height;
    
        if (newWidth != mWidth || newHeight != mHeight)
        {
            uint i,k;
            if (newWidth < mWidth )
            {
                for (i = newWidth; i <  mWidth; ++i)
	            {
                    PagingLandScapeData2DRow  *li = &mData2D[ i ];
                    for (k = 0; k < mHeight; ++k)
		            {
                        PagingLandScapeData2D *d = (*li)[ k ];
                        d->uninit();
                        delete (d);
		            }            
                    li->clear();
	            }
                mData2D.reserve (newWidth);
                mData2D.resize (newWidth);


                if (newHeight < mHeight )
                { 
                    // clear pages
                    for (i = 0; i < newWidth; i++ )
	                {
                        PagingLandScapeData2DRow  *li = &mData2D[ i ];
                        for (k = newHeight; k < mHeight; ++k)
	                    {
                            PagingLandScapeData2D *d = (*li)[ k ];
                            d->uninit();
                            delete (d);
	                    }
                        li->reserve(newHeight);
                        li->resize(newHeight);
                    }
                }
                else if (newHeight > mHeight )
                {
                    // add pages
                    for (i = 0; i < newWidth; i++ )
	                {
                        PagingLandScapeData2DRow  *li = &mData2D[ i ];
                        li->reserve(newHeight);
                        li->resize(newHeight);
	                    for (k = mHeight; k < newHeight; ++k)
		                {
			                (*li)[ k ] = allocateData2D ();
		                }
	                }
                }
            }
            else if (newWidth > mWidth )
            {
                mData2D.reserve (newWidth);
                mData2D.resize (newWidth);

                if (newHeight < mHeight )
                { 
                    // clear pages
                    for (i = 0; i < mWidth; i++ )
	                {
                        PagingLandScapeData2DRow  *li = &mData2D[ i ];
                        for (k = newHeight; k < mHeight; ++k)
	                    {
                            PagingLandScapeData2D *d = (*li)[ k ];
                            d->uninit();
                            delete (d);
	                    }
                        li->reserve(newHeight);
                        li->resize(newHeight);
                    }
                }
                else if (newHeight > mHeight )
                {
                    // add pages
                    for (i = 0; i < mWidth; i++ )
	                {
                        PagingLandScapeData2DRow  *li = &mData2D[ i ];
                        li->reserve(newHeight);
                        li->resize(newHeight);
	                    for (k = mHeight; k < newHeight; ++k)
		                {
			                (*li)[ k ] = allocateData2D ();
		                }
	                }
                }

                // add pages
	            for (i = mWidth; i < newWidth; ++i)
	            {
                    mData2D[ i ].reserve (newHeight);
                    mData2D[ i ].resize (newHeight);
	            }
                for (i = mWidth; i < newWidth; ++i)
	            {
                    PagingLandScapeData2DRow  *li = &mData2D[ i ];
                    li->reserve(newHeight);
                    li->resize(newHeight);
	                for (k = 0; k < newHeight; ++k)
		            {
                        (*li)[ k ] = allocateData2D ();
		            }
	            }
            }
            else if (newHeight < mHeight )
            { 
                for (i = 0; i < newWidth; ++i)
	            {
                    PagingLandScapeData2DRow  *li = &mData2D[ i ];
                    for (k = newHeight; k < mHeight; ++k)
	                {
                        PagingLandScapeData2D *d = (*li)[ k ];
                        d->uninit();
                        delete (d);
	                }
                    li->reserve(newHeight);
                    li->resize(newHeight);
                }
            }
            else if (newHeight > mHeight )
            {
                for (i = 0; i < newWidth; ++i)
	            {
                    PagingLandScapeData2DRow  *li = &mData2D[ i ];
                    li->reserve(newHeight);
                    li->resize(newHeight);
	                for (k = mHeight; k < newHeight; ++k)
		            {    
                        (*li)[ k ] = allocateData2D ();
		            }
	            }
            }
            mWidth = newWidth; 
            mHeight = newHeight;
            //setup the page array.      
//            for (i = 0; i < newWidth; i++ )
//            {
//                PagingLandScapeData2DRow  *li = &mData2D[ i ];
//                for (k = 0; k < newHeight; k++ )
//                {		
//                    PagingLandScapeData2D *p = (*li)[ k ];
//                }
//            }
        }
    }
    //-----------------------------------------------------------------------
    void PagingLandScapeData2DManager::setPageManager ()
    {
        mPageManager = PagingLandScapePageManager::getSingletonPtr();
    }        
    //-----------------------------------------------------------------------
    bool PagingLandScapeData2DManager::load( const uint dataX, const uint dataZ )
    {
	    PagingLandScapeData2D* data = mData2D[ dataX ][ dataZ ];
	    if ( !data->isLoaded() )
	    {
            return data->load( dataX, dataZ );
	    }
        return true;
    }
    //-----------------------------------------------------------------------
    void PagingLandScapeData2DManager::unload( const uint dataX, const uint dataZ )
    {
	    PagingLandScapeData2D* data = mData2D[ dataX ][ dataZ ];
	    if ( data->isLoaded() )
	    {
		    data->unload();
	    }
    }

    //-----------------------------------------------------------------------
    bool PagingLandScapeData2DManager::isLoaded( const uint dataX, const uint dataZ ) const
    {
	    return mData2D[ dataX ][ dataZ ]->isLoaded();
    }
    //-----------------------------------------------------------------------
    const ColourValue PagingLandScapeData2DManager::getCoverageAt( const uint dataX,
                                                                const uint dataZ, 
                                                                const Real x, 
                                                                const Real z )
    {
        PagingLandScapeData2D* data = mData2D[ dataX ][ dataZ ];
        if ( data->isLoaded() )
        {
            // TODO check it the terrain height is modified
            return data->getCoverage(x, z);
        }
        return ColourValue::White;
    }
    //-----------------------------------------------------------------------
    const ColourValue PagingLandScapeData2DManager::getBaseAt( const uint dataX, 
                                                            const uint dataZ, 
                                                            const Real x, 
                                                            const Real z )
    {
        PagingLandScapeData2D* data = mData2D[ dataX ][ dataZ ];
        if ( data->isLoaded() )
        {
            // TODO check it the terrain height is modified
            return data->getBase(x, z);
    }
        return ColourValue::White;
    }
    //-----------------------------------------------------------------------
    const Real PagingLandScapeData2DManager::getShadowAt( const uint dataX,
                                                        const uint dataZ,
                                                        const uint x,
                                                        const uint z,
                                                        const bool &positive)
    {
        PagingLandScapeData2D* data = mData2D[ dataX ][ dataZ ];
        if ( data->isLoaded() )
        {
            // TODO check it the terrain height is modified
            return data->getShadow(x, z, positive);
        }
        return 0.0f;
    }
    //-----------------------------------------------------------------------
    const Real PagingLandScapeData2DManager::getHeight( const uint dataX, 
                                                        const uint dataZ, 
                                                        const Real x, 
                                                        const Real z ) const
    {
        
	    PagingLandScapeData2D* data = mData2D[ dataX ][ dataZ ];
	    if ( data->isLoaded() )
	    {
		    // TODO check it the terrain height is modified
		    return data->getHeight(x, z);
	    }
	    return 0.0f;
    }
    //-----------------------------------------------------------------------
    const Real PagingLandScapeData2DManager::getHeight( const uint dataX, 
                                                        const uint dataZ, 
                                                        const uint x, 
                                                        const uint z ) const
    {
        
	    PagingLandScapeData2D* data = mData2D[ dataX ][ dataZ ];
	    if ( data->isLoaded() )
	    {
		    return data->getHeight(x, z);
	    }
	    return 0.0f;
    }
   
    //-----------------------------------------------------------------------
//    bool PagingLandScapeData2DManager::addNewHeight( const Sphere newSphere )
//    {
//	    uint x, z;
//
//        Vector3 s = newSphere.getCenter();
//	    // Calculate where is going to be placed the new height
//        PagingLandScapePageManager::getSingleton ().getPageIndices( s.x, s.z, x, z, true);
//	    // TODO: DeScale and add the sphere to all the necessary pages
//
//	    //place it there
//	    return mData2D[ x ][ z ]->addNewHeight(newSphere);
//    }

    //-----------------------------------------------------------------------
//    bool PagingLandScapeData2DManager::removeNewHeight( const Sphere oldSphere )
//    {
//	    uint x, z;
//        Vector3 s = oldSphere.getCenter();
//	    // Calculate where is going to be placed the new height
//	    PagingLandScapePageManager::getSingleton ().getPageIndices( s.x, s.z, x, z, true);
//	    // TODO: DeScale and add the sphere to all the necessary pages
//
//	    //remove it
//	    return mData2D[ x ][ z ]->removeNewHeight(oldSphere);
//    }
    //-----------------------------------------------------------------------
    const Real PagingLandScapeData2DManager::getMaxHeight(const uint x, 
                                                        const uint z) const
    { 
        PagingLandScapeData2D* data = mData2D[ x ][ z ];  
        assert (data);
        if ( data->isLoaded() )
        {
            return mData2D[ x ][ z ]->getMaxHeight();
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

        PagingLandScapeData2D* data = mData2D[ pX ][ pZ ];
        assert (data);
        if ( data->isLoaded() )
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
                        data = mData2D[ pX - 1 ][ pZ - 1 ];
                        if ( data->isLoaded() )
                        {
                           data->setHeight (pSize, pSize, heightResult);
                        } 
                    }
                    else if (up)
                    {
                        //  upper left corner 
                        data = mData2D[ pX - 1 ][ pZ  + 1 ];
                        if (data->isLoaded() )
                        {
                           data->setHeight (pSize, 0, heightResult);
                        } 
                    } 
                    
                    // left
                    data = mData2D[ pX - 1 ][ pZ ];
                    if ( data->isLoaded() )
                    {
                        data->setHeight (pSize, z, heightResult);
                    } 
                }
                else if (right)
                {
                    if (up)
                    {
                        //  upper right corner 
                        data = mData2D[ pX + 1 ][ pZ  + 1 ];
                        if ( data->isLoaded() )
                        {
                           data->setHeight (0, 0, heightResult);
                        } 
                    }
                    else if (down)
                    {
                        //  lower right corner 
                        data = mData2D[ pX + 1 ][ pZ  - 1 ];
                        if ( data->isLoaded() )
                        {
                           data->setHeight (0, pSize, heightResult);
                        } 
                    }  
                    // right
                    data = mData2D[ pX + 1 ][ pZ ];
                    if ( data->isLoaded() )
                    {
                        data->setHeight (0, z, heightResult);
                    }  
                }
                if (down)
                {        
                    //  lower  (down only)    
                    data = mData2D[ pX ][ pZ - 1 ];
                    if ( data->isLoaded() )
                    {
                        data->setHeight (x, pSize, heightResult);
                    } 
                }
                else if (up)
                {
                    // upper (up only)
                    data = mData2D[ pX ][ pZ  + 1 ];
                    if ( data->isLoaded() )
                    {
                        data->setHeight (x, 0, heightResult);
                    } 
                }     
                return true;
            } // if (data->DeformHeight (x, z, heightResult))
        } // if ( data->isLoaded() )
        return false;
    }

    //-----------------------------------------------------------------------
    const Real PagingLandScapeData2DManager::getRealPageHeight (const Real x, const Real z, 
                                                                const uint pageX, const uint pageZ, 
                                                                const uint Lod) const
    {
        // scale position from world to page scale
        Real localX = x / mOptions->scale.x;
        Real localZ = z / mOptions->scale.z;

        // adjust x and z to be local to page
        const Real pSize = mOptions->PageSize - 1;

        localX -= pageX*pSize - mOptions->maxUnScaledX;
        localZ -= pageZ*pSize - mOptions->maxUnScaledZ;

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
        const int verdata_spread = 1 << Lod;
        //const int verdata_spread = 1;
        const Real inv_verdata_spread =  1.0f / verdata_spread;


        // find the vertex data to the bottom left of the point
        //const int bottom_left_x = (static_cast<int>(localX * inv_verdata_spread)) * verdata_spread;
        //const int bottom_left_z = (static_cast<int>(localZ * inv_verdata_spread)) * verdata_spread;

#ifndef OLDGetheightat
        // find the vertex to the 'bottom right' of the point
        const unsigned long lod_mask = ~(verdata_spread - 1);
        const int bottom_right_x = static_cast<int>(localX) & lod_mask;
        const int bottom_right_z = static_cast<int>(localZ) & lod_mask; 

        // find the 4 heights around the point

         const Real bottom_right_y = getHeightAtPage(pageX, pageZ, bottom_right_x ,                bottom_right_z);
         const Real bottom_left_y  = getHeightAtPage(pageX, pageZ, bottom_right_x + verdata_spread, bottom_right_z);
         const Real top_right_y    = getHeightAtPage(pageX, pageZ, bottom_right_x,                 bottom_right_z + verdata_spread);
         const Real top_left_y     = getHeightAtPage(pageX, pageZ, bottom_right_x + verdata_spread, bottom_right_z + verdata_spread);
        
        const Real z_pct = (localZ - bottom_right_z) * inv_verdata_spread;
        Real x_pct = (localX - bottom_right_x) * inv_verdata_spread;
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
             const Real y1 = bottom_left_y * (1-z_pct) + top_left_y  * z_pct;
             const Real y2 = bottom_left_y * (1-z_pct) + top_right_y * z_pct;
              if (z_pct > 0)
                 x_pct = (x_pct - (1-z_pct)) / z_pct;

             return y1 * x_pct + y2 * (1-x_pct);
         } // if (x_pct > 1 - z_pct)
         else
         { 
            // This point is on the lower-right tri
             const Real y1 = bottom_left_y  * (1-z_pct) + top_right_y * z_pct;
             const Real y2 = bottom_right_y * (1-z_pct) + top_right_y * z_pct;

             if (z_pct < 1)
                 x_pct = x_pct / (1-z_pct);

             return y1 * x_pct + y2 * (1-x_pct);
         } 

#else //OLD

        //const int bottom_left_x = static_cast<int>(localX);
        //const int bottom_left_z = static_cast<int>(localZ);


        const Real bottom_left_y  = getHeightAtPage(pageX, pageZ, bottom_left_x ,                bottom_left_z);
        const Real bottom_right_y = getHeightAtPage(pageX, pageZ, bottom_left_x + verdata_spread, bottom_left_z);
        const Real top_left_y     = getHeightAtPage(pageX, pageZ, bottom_left_x,                 bottom_left_z + verdata_spread);
        const Real top_right_y    = getHeightAtPage(pageX, pageZ, bottom_left_x + verdata_spread, bottom_left_z + verdata_spread);


        const Real x_pct = (localX - bottom_left_x) * inv_verdata_spread;
        const Real z_pct = (localZ - bottom_left_z) * inv_verdata_spread;

        //       TL-------TR
        //        | \     |
        //        |  \    |
        //        |   \   |
        //        |    \  |
        //   ^    |     \ |   
        //   |    BL------BR
        //   z
        //          x->
//        const Real y1 = bottom_left_y  * (1-z_pct) + top_left_y  * z_pct;
//        const Real y2 = bottom_right_y * (1-z_pct) + top_right_y * z_pct;
//
//        return y1 * (1-x_pct) + y2 * x_pct;
#endif // OLd
    }
    //-----------------------------------------------------------------------
    //JEFF
    const Real PagingLandScapeData2DManager::getRealWorldHeight(const Real x, const Real z, 
                                                        const PagingLandScapeTileInfo* info) const
    {
        // figure out which page the point is on
        const uint dataX = info->pageX;
        const uint dataZ = info->pageZ;

        assert (mData2D[ dataX ][ dataZ ]);
        if ( ! (mData2D[ dataX ][ dataZ ]->isLoaded()))
            return 0.0f;

        PagingLandScapeTile *t = mPageManager->getPage (dataX, dataZ )->getTile (info->tileX, info->tileZ);
        uint Lod = 0;
        if (t && t->isLoaded ())
            Lod = t->getRenderable ()->mRenderLevel;

        return getRealPageHeight (x, z, dataX, dataZ, Lod);
        
    }
    //-----------------------------------------------------------------------
    const Real PagingLandScapeData2DManager::getRealWorldHeight(const Real x, const Real z) const
    {
	    // figure out which page the point is on
        const Vector3 pos (x, 0, z);
        uint dataX, dataZ;
        if (mPageManager->getPageIndices(pos.x, pos.z, dataX, dataZ, false))
        {
            assert (mData2D[ dataX ][ dataZ ]);
            if ((mData2D[ dataX ][ dataZ ]->isLoaded()))
            {
                // figure out which tile the point is on
                PagingLandScapeTile *t = 
                    mPageManager->getTile (pos.x, pos.z, 
                                dataX, dataZ, false);
                uint Lod = 0;
                if (t && t->isLoaded ())
                    Lod = t->getRenderable ()->mRenderLevel;

                return getRealPageHeight (x, z, dataX, dataZ, Lod);
            }
       
        }
        return 0.0f;
    }
    //-----------------------------------------------------------------------
    const Real PagingLandScapeData2DManager::getWorldHeight(const Real x, const Real z) const
    {
	    // figure out which page the point is on
        uint dataX, dataZ;
        if (mPageManager->getPageIndices(x, z, dataX, dataZ, false))
        {
            assert (mData2D[ dataX ][ dataZ ]);
            return getHeightAtPage (dataX, dataZ, x, z);       
        }
        return 0.0f;
    }
    //-----------------------------------------------------------------------
    const Real PagingLandScapeData2DManager::getHeightAtPage(const uint dataX, const uint dataZ, 
                                                        const Real x, const Real z) const
    {
        PagingLandScapeData2D* data = mData2D[ dataX ][ dataZ ];
        
        if ( data->isLoaded() )
        {
            Real lX = x;
            Real lZ = z;
            const Real pSize = mOptions->PageSize - 1;
            //check if we have to change current page
            if ( lX < 0.0f )
            { 
                if ( dataX == 0 )
                    lX = 0.0f;
                else
                {
                    data = mData2D[ dataX - 1 ][ dataZ ];
                    assert (data);
                    if  ( data->isLoaded() )
                    {
                        lX = static_cast<Real> (pSize);
                    }
                    else
                    {
                        lX = 0.0f;
                        data = mData2D[ dataX ][ dataZ ];
                    }
                }
              
            }
            else if (lX > pSize)
            {
                if ( dataX == mOptions->world_width - 1 )
                    lX = static_cast<Real> (pSize);
                else
                {
                    data = mData2D[ dataX + 1 ][ dataZ ];
                    assert (data);
                    if  ( data->isLoaded() )
                    {
                        lX = 0.0f;
                    }
                    else
                    {
                        lX = static_cast<Real> (pSize);
                        data = mData2D[ dataX ][ dataZ ];
                    }
                }
            }

            if ( lZ < 0.0f )
            {
                if ( dataZ == 0 )
                    lZ = 0.0f;
                else
                {
                    data = mData2D[ dataX ][ dataZ - 1 ];
                    assert (data);
                    if  ( data->isLoaded() )
                    {
                        lZ = static_cast<Real> (pSize);
                    }
                    else
                    {
                        lZ = 0.0f;
                        data = mData2D[ dataX ][ dataZ ];
                    }
                }
            }
            else if (lZ > pSize)
            {
                if ( dataZ == mOptions->world_height - 1)
                    lZ = static_cast<Real> (pSize);
                else
                {
                    data = mData2D[ dataX ][ dataZ + 1 ];
                    assert (data);
                    if  ( data->isLoaded() )
                    {
                        lZ = 0.0f;
                    }
                    else
                    {
                        lZ = static_cast<Real> (pSize);
                        data = mData2D[ dataX ][ dataZ ];
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
                                                        const int x, const int z) const
    {
        PagingLandScapeData2D* data = mData2D[ dataX ][ dataZ ];
        
        assert (data);
        if ( data->isLoaded() )
        {
            int lX = x;
            int lZ = z;
            const int pSize = mOptions->PageSize - 1;
            //check if we have to change current page
            if ( lX < 0 )
            { 
                if ( dataX == 0 )
                    lX = 0;
                else
                {
                    data = mData2D[ dataX - 1 ][ dataZ ];
                    assert (data);
                    if  ( data->isLoaded() )
                    {
                        lX = pSize;
                    }
                    else
                    {
                        lX = 0;
                        data = mData2D[ dataX ][ dataZ ];
                    }
                }
              
            }
            else if (lX > pSize)
            {
                if ( dataX == mOptions->world_width - 1)
                    lX = pSize;
                else
                {
                    data = mData2D[ dataX + 1 ][ dataZ ];
                    assert (data);
                    if  ( data->isLoaded() )
                    {
                        lX = 0;
                    }
                    else
                    {
                        lX = pSize;
                        data = mData2D[ dataX ][ dataZ ];
                    }
                }
            }

            if ( lZ < 0 )
            {
                if ( dataZ == 0 )
                    lZ = 0;
                else
                {
                    data = mData2D[ dataX ][ dataZ - 1 ];
                    assert (data);
                    if  ( data->isLoaded() )
                    {
                        lZ = pSize;
                    }
                    else
                    {
                        lZ = 0;
                        data = mData2D[ dataX ][ dataZ ];
                    }
                }
            }
            else if (lZ > pSize)
            {
                if ( dataZ == mOptions->world_height - 1)
                    lZ = pSize;
                else
                {
                    data = mData2D[ dataX ][ dataZ + 1 ];
                    assert (data);
                    if  ( data->isLoaded() )
                    {
                        lZ = 0;
                    }
                    else
                    {
                        lZ = pSize;
                        data = mData2D[ dataX ][ dataZ ];
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
    const Vector3 PagingLandScapeData2DManager::getNormalAt( const uint dataX, const uint dataZ, 
                                                            const uint x, const uint z)
    {
	    PagingLandScapeData2D* data = mData2D[ dataX ][ dataZ ];
        assert (data);
	    if ( data->isLoaded() )
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

                // the divider make sure we do respect proportion  (height and width proportional to y )
                const Real Divider = static_cast <Real> (mOptions->PageSize - 1) / PagingLandScapeOptions::getSingleton ().scale.y;

                #define  gH(a, b) (getHeightAtPage(dataX, dataZ, static_cast<int> (a), static_cast<int> (b)))

//              Vector3 result ((gH(x-1,z-1) + gH (x-1, z) + gH (x-1, z) + gH (x-1, z+1) - gH (x+1, z-1) - gH (x+1, z) - gH (x+1, z) - gH (x+1, z+1)) * Divider, 
//                              8.0f, 
//                              (gH (x-1, z-1) + gH (x, z-1) + gH (x, z-1) + gH (x+1, z-1) - gH (x-1, z+1) - gH (x, z+1) - gH (x, z+1) - gH (x+1, z+1)) * Divider);

                Vector3 result((gH (x - 1 , z)     - gH (x + 1 , z)) * Divider,
                                2.0f,
                               (gH (x,         z - 1) - gH (x        , z + 1 )) * Divider); 

                result.normalise ();

                return result;
            }
            #endif //_NOLOAD
	    }
	    return Vector3::UNIT_Y;
    }
    //----------------------------------------------------------------------------
    PagingLandScapeData2D *PagingLandScapeData2DManager::getData2d (const uint x, const uint z)
    {
        PagingLandScapeData2D *data = mData2D[ x ][ z ];
        assert (data);
        return ( data->isLoaded() )? data: 0;
    }
} //namespace
