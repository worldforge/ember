/***************************************************************************
	OgrePagingLandScapePageManager.cpp  -  description
	-------------------
	begin                : Sat May 01 2004
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
#include "OgreVector3.h"
#include "OgreColourValue.h"

#include "OgreMovableObject.h"
#include "OgreAxisAlignedBox.h"

#include "OgreCamera.h"


#include "OgrePagingLandScapeOptions.h"
#include "OgrePagingLandScapeCamera.h"
#include "OgrePagingLandScapePage.h"
#include "OgrePagingLandScapePageManager.h"
#include "OgrePagingLandScapeRenderableManager.h"
#include "OgrePagingLandScapeData2DManager.h"
#include "OgrePagingLandScapeTextureManager.h"
#include "OgrePagingLandScapeTexture.h"
#include "OgrePagingLandScapeTileInfo.h"
#include "OgrePagingLandScapeTile.h"
#include "OgrePagingLandScapeRenderable.h"
#include "OgrePagingLandScapeListenerManager.h"
#include "OgrePagingLandScapePageRenderable.h"   
#include "OgrePagingLandScapeHorizon.h"

namespace Ogre
{

    //-----------------------------------------------------------------------
    template<> PagingLandScapePageManager* Singleton<PagingLandScapePageManager>::ms_Singleton = 0;
    PagingLandScapePageManager* PagingLandScapePageManager::getSingletonPtr(void)
    {
	    return ms_Singleton;
    }
    PagingLandScapePageManager& PagingLandScapePageManager::getSingleton(void)
    {  
	    assert( ms_Singleton );  return ( *ms_Singleton );  
    }

    //-----------------------------------------------------------------------
    PagingLandScapePageManager::PagingLandScapePageManager( void ) :
    
        mOptions(PagingLandScapeOptions::getSingletonPtr( ) ),
        mData2d(PagingLandScapeData2DManager::getSingletonPtr( ) ),
        mTexture(PagingLandScapeTextureManager::getSingletonPtr( ) ),
        mRenderable(PagingLandScapeRenderableManager::getSingletonPtr( ) ),
        mWidth( 0 ),
        mHeight( 0 ),
        mNextQueueFrameCount( 0 ),
	    mPause( 99 ),
        mCurrentcam( 0 ),
        mTerrainReady( false ),
        mTimePreLoaded( 0 ),
        mPageSize( mOptions->PageSize ),  
        mTileSize( mOptions->TileSize )
    {
        PagingLandScapePageRenderable::mOpt = mOptions;
    }

    //-----------------------------------------------------------------------
    PagingLandScapePageManager::~PagingLandScapePageManager( void )
    {
        reset( );
        PagingLandScapePages::iterator iend = mPages.end( );
        for ( PagingLandScapePages::iterator i = mPages.begin( ); i != iend; ++i )
        {
            std::for_each( i->begin( ), i->end( ), delete_object( ) );
            //i->clear( );
        }    
	    mPages.clear( );
    }

    //----------------------------------------------------------------------- 
    void PagingLandScapePageManager::reset( void )
    {
        PagingLandScapePages::iterator iend = mPages.end( );
        for ( PagingLandScapePages::iterator i = mPages.begin( ); i != iend; ++i )
        {
            std::for_each( i->begin( ), i->end( ), delete_object( ) );
            //i->clear( );
        }    
	    mPages.clear( );

        // should have a width, height conservative usage ?
        // need work in tilemanager, renderablemanaer, textures, etc...

        //        uint i,k;
        //        for ( i = 0; i < mWidth; ++i )
        //        {
        //            PagingLandScapePageRow* li = &mPages[ i ];
        //            for ( k = 0; k < mHeight; ++k )
        //            {
        //                PagingLandScapePage* p = ( *li )[ k ];
        //                p->uninit( );
        //            }
        //        }
        //      
         mWidth = 0;
         mHeight = 0;
    }

    //-----------------------------------------------------------------------
    void PagingLandScapePageManager::load( void )
    {
  

//        if ( mPageSize != mOptions->PageSize || mTileSize != mOptions->TileSize )
//        {
             // problem is PageNode being Cleared from PagingLandScapeOctree, or scene manager and thus no more valid ??

            // so reset is called before calling the scene manager node clearing
            //reset( );

//            mPageSize = mOptions->PageSize;
//            mTileSize = mOptions->TileSize;
//
//            uint i,k;
//            for ( i = 0; i < mWidth; ++i )
//            {
//                PagingLandScapePageRow  *li = &mPages[ i ];
//                for ( k = 0; k < mHeight; ++k )
//                {
//                    PagingLandScapePage* p = ( *li )[ k ];
//                    p->init( );
//                }
//            }
        //}
        WorldDimensionChange( );
    }

    //-----------------------------------------------------------------------
    void PagingLandScapePageManager::clear( void )
    {
        mPageLoadQueue.clear( );
        mPagePreloadQueue.clear( );
        mPageTextureloadQueue.clear( );

        mLoadedPages.clear( );  
        mTextureLoadedPages.clear( );
        mPreLoadedPages.clear( );	
        mUnloadedPages.clear( );

        uint i,k;
        for ( i = 0; i < mWidth; ++i )
        {
            PagingLandScapePageRow* li = &mPages[ i ];
            for ( k = 0; k < mHeight; ++k )
            {		
                removeFromQueues( ( *li )[ k ] );
            }
        }

        // before calling the scene manager node clearing
        reset( );
    }

    //-----------------------------------------------------------------------
    void PagingLandScapePageManager::WorldDimensionChange( void )
    {
        const uint newWidth = mOptions->world_width; 
        const uint newHeight = mOptions->world_height;
    
        uint i, k;
        if ( newWidth != mWidth || newHeight != mHeight )
        {
            if ( newWidth < mWidth )
            {         
                //clear pages
                for ( i = newWidth; i < mWidth; ++i )
	            {
                    PagingLandScapePageRow* li = &mPages[ i ];
                    for ( k = 0; k < mHeight; k++ )
		            {
                        removeFromQueues( ( *li )[ k ] );
                        mUnloadedPages.remove( ( *li )[ k ] );
                        delete ( ( *li )[ k ] );
		            }            
                    li->clear( );
	            }
                mPages.reserve( newWidth );
                mPages.resize( newWidth );

                if ( newHeight < mHeight )
                { 
                    // clear pages
                    for ( i = 0; i < newWidth; i++ )
	                {
                        PagingLandScapePageRow* li = &mPages[ i ];
                        for ( k = newHeight; k < mHeight; ++k )
	                    {
                            removeFromQueues( ( *li )[ k ] );
                            mUnloadedPages.remove( ( *li )[ k ] );
                            delete ( *li )[ k ];
	                    }
                        li->reserve( newHeight );
                        li->resize( newHeight );
                    }
                }
                else if ( newHeight > mHeight )
                {
                    // add pages
                    for ( i = 0; i < newWidth; i++ )
	                {
                        PagingLandScapePageRow* li = &mPages[ i ];
                        li->reserve( newHeight );
                        li->resize( newHeight );
	                    for ( k = mHeight; k < newHeight; k++ )
		                {
			                ( *li )[ k ] = new PagingLandScapePage( i, k );
                            //( *li )[ k ]->init( );
                            mUnloadedPages.push_back( ( *li )[ k ] );
		                }
	                }
                }
            }
            else if ( newWidth > mWidth )
            {
                mPages.reserve( newWidth );
                mPages.resize( newWidth );

                if ( newHeight < mHeight )
                { 
                    // clear pages
                    for ( i = 0; i < mWidth; i++ )
	                {
                        PagingLandScapePageRow* li = &mPages[ i ];
                        for ( k = newHeight; k < mHeight; ++k )
	                    {
                            removeFromQueues( ( *li )[ k ] );
                            mUnloadedPages.remove( ( *li )[ k ] );
                            delete ( *li )[ k ];
	                    }
                        li->reserve( newHeight );
                        li->resize( newHeight );
                    }
                }
                else if ( newHeight > mHeight )
                {
                    // add pages
                    for ( i = 0; i < mWidth; i++ )
	                {
                        PagingLandScapePageRow *li = &mPages[ i ];
                        li->reserve( newHeight );
                        li->resize( newHeight );
	                    for ( k = mHeight; k < newHeight; k++ )
		                {
			                ( *li )[ k ] = new PagingLandScapePage( i, k );
                            //( *li )[ k ]->init( );
                            mUnloadedPages.push_back( ( *li )[ k ] );
		                }
	                }
                }
                
                // add pages
	            for ( i = mWidth; i < newWidth; i++ )
	            {
                    mPages[ i ].reserve( newHeight );
                    mPages[ i ].resize( newHeight );
	            }
                for ( i = mWidth; i < newWidth; i++ )
	            {
                    PagingLandScapePageRow* li = &mPages[ i ];
                    li->reserve( newHeight );
                    li->resize( newHeight );
	                for ( k = 0; k < newHeight; k++ )
		            {
			            ( *li )[ k ] = new PagingLandScapePage( i, k );
                        //( *li )[ k ]->init( );
                        mUnloadedPages.push_back( ( *li )[ k ] );
		            }
	            }
            }
            else if ( newHeight < mHeight )
            { 
                for ( i = 0; i < newWidth; i++ )
	            {
                    PagingLandScapePageRow* li = &mPages[ i ];
                    for ( k = newHeight; k < mHeight; ++k )
	                {
                        delete ( *li )[ k ];
                        removeFromQueues( ( *li )[ k ]);
                        mUnloadedPages.remove( ( *li )[ k ] );
	                }
                    li->reserve( newHeight );
                    li->resize( newHeight );
                }
            }
            else if ( newHeight > mHeight )
            {
                for ( i = 0; i < newWidth; i++ )
	            {
                    PagingLandScapePageRow* li = &mPages[ i ];
                    li->reserve( newHeight );
                    li->resize( newHeight );
	                for ( k = mHeight; k < newHeight; ++k )
		            {
			            ( *li )[ k ] = new PagingLandScapePage( i, k );
                        //( *li )[ k ]->init( );
                        mUnloadedPages.push_back( ( *li )[ k ] );
		            }
	            }
            }
	        //setup the page array.
            for ( i = 0; i < newWidth; i++ )
            {
                PagingLandScapePageRow* li = &mPages[ i ];
                for ( k = 0; k < newHeight; ++k )
                {		
                    PagingLandScapePage* p = ( *li )[ k ];
                    assert ( p );
                    p->init( );   
                    if ( k != newHeight - 1 )
                    {
                        PagingLandScapePage* pSouth = ( *li )[ k + 1 ];
                        p->_setNeighbor( SOUTH, pSouth );
                        pSouth->_setNeighbor( NORTH, p );
                    }

                    if ( i != newWidth - 1 )
                    {
                        PagingLandScapePage *pEast = mPages[ i + 1 ][ k ];
                        p->_setNeighbor( EAST, pEast );
                        pEast->_setNeighbor( WEST, p );
                    }
                }
            }
            mWidth = newWidth; 
            mHeight = newHeight;
        }
    }

    //-----------------------------------------------------------------------
    void PagingLandScapePageManager::setMapMaterial( void )
    {
        PagingLandScapePages::iterator iend = mPages.end( );
        for ( PagingLandScapePages::iterator i = mPages.begin( ); i != iend; ++i )
        {
                    
          std::for_each( i->begin( ), i->end( ), std::mem_fun( &PagingLandScapePage::setMapMaterial ) );
        }
    }

    //-----------------------------------------------------------------------
    void PagingLandScapePageManager::_updateLod( void )
    {
        PagingLandScapePagelist::iterator lend = mLoadedPages.end( );
        for ( PagingLandScapePagelist::iterator l = mLoadedPages.begin( ); l != lend; ++l )
        { 
            ( *l )->_updateLod( );
        }
    }

    //-----------------------------------------------------------------------
    bool PagingLandScapePageManager::frameStarted( const FrameEvent& evt )
    {
        --mTimePreLoaded;
        if ( mOptions->VisMap )
		{
			PagingLandScapeHorizon::getSingleton( ).prepare( static_cast< PagingLandScapeCamera* >( mOptions->primaryCamera ) );
		}
        mRenderable->resetVisibles( );
        mOnFrame = false;
        return true;
    }

    //-----------------------------------------------------------------------
    bool PagingLandScapePageManager::frameEnded( const FrameEvent& evt )
    {
        // If This Frame has seen any Camera,
        // We won't unload anything.
        // since un-focusing rendering may make this method unload all renderables.
        if ( !mOnFrame )
		{
            return true;
		}

        // load some renderables if needed
        if ( mRenderable->executeRenderableLoading( )  
            && !mTerrainReady && 
            mPagePreloadQueue.empty( ) && 
                mPageLoadQueue.empty( ) && 
            mTextureLoadedPages.empty( ) )
        {
            PagingLandscapeListenerManager::getSingleton( ).fireTerrainReady( );// no more to load
            mTerrainReady = true;
        }
        else
        {
            // Check for pages that need to be unloaded.
            // if touched, that means they didn't have been touch by any cameras
            // for several frames and thus need to be unloaded.
            PagingLandScapePagelist::iterator l;
            for ( l = mPreLoadedPages.begin( ); l != mPreLoadedPages.end( ); )
            {
                if ( ( *l )->touched( ) )
                {
                    ( *l )->postUnload( );
                    removeFromQueues( *l );
                    l = mPreLoadedPages.erase( l ); 
                }
                else
                {
                    ++l;
                }
            }
            for ( l = mTextureLoadedPages.begin( ); l != mTextureLoadedPages.end( ); )
            {
                if ( ( *l )->touched( ))
                {
                    ( *l )->postUnload( );
                    removeFromQueues( *l );
                    l = mTextureLoadedPages.erase( l ); 
                }
                else
                {
                    ++l;
                }
            }
            for ( l = mLoadedPages.begin( ); l != mLoadedPages.end( ); )
            {             
                if ( ( *l )->touched( ) )
                {
                    ( *l )->postUnload( );
                    removeFromQueues( *l );
                    l = mLoadedPages.erase( l ); 
                }
                else
                {
                    ++l;
                }  
            }
            // check queues for page that need to be excluded from queues
            for ( l = mPagePreloadQueue.begin( ); l != mPagePreloadQueue.end( ); )
            {
                if ( ( *l )->touched( ) )
                {
                    ( *l )->mIsPreLoading = false;
                    l = mPagePreloadQueue.erase( l );  
                }
                else
                {
                    ++l;
                }
            }
            for ( l = mPageTextureloadQueue.begin( ); l != mPageTextureloadQueue.end( ); )
            {
                if ( ( *l )->touched( ) )
                {
                    ( *l )->mIsTextureLoading = false;
                    l = mPageTextureloadQueue.erase( l );   
                }
                else
                {
                    ++l;
                }
            }
            for ( l = mPageLoadQueue.begin( ); l != mPageLoadQueue.end( ); )
        {
            if ( ( *l )->touched( ) )
            {
                ( *l )->mIsLoading = false;
                 l = mPageLoadQueue.erase( l );     
            }
            else
            {
                ++l;
            }
        }
		
        }

        if ( mOptions->VisMap )
		{
            PagingLandScapeHorizon::getSingleton( ).update( );
		}
        return true;
    }

            void LoadFirstPage( PagingLandScapeCamera* cam );
            void LoadFirstPage( const Vector3 &pos );

    //-----------------------------------------------------------------------
    void PagingLandScapePageManager::LoadFirstPage( const Vector3 &pos )
    {           
        uint i, j;

        //gets page indices (if outside Terrain gets nearest page)
	    getPageIndices  (pos.x, pos.z, i, j, true); 
        // Have the current page be loaded now !
        PagingLandScapePage *p = mPages[ i ][ j ];
		if ( !p->isLoaded()  )
		{                    
            removeFromQueues (p);
			p->load();
            mLoadedPages.push_back (p);
            // make sure this brutal loading doesn't impact on fps
            mNextQueueFrameCount = 20;   
		}
        else
            p->touch ();   

    }
    //-----------------------------------------------------------------------
    void PagingLandScapePageManager::LoadFirstPage( PagingLandScapeCamera* cam )
    {           
        uint i, j;

        const Vector3 CamPos =  cam->getDerivedPosition();

        LoadFirstPage (CamPos);

        //gets page indices (if outside Terrain gets nearest page)
	    getPageIndices  (CamPos.x, CamPos.z, i, j, true); 
       
		// update the camera page position
        // does modify mIniX, mFinX, mIniZ, mFinZ
        cam->updatePaging (i, j);
    }

    //-----------------------------------------------------------------------
    void PagingLandScapePageManager::updatePaging( PagingLandScapeCamera* cam )
    {
        mCurrentcam = cam;
	    // Here we have to look if we have to load, unload any of the LandScape Pages
	    //Vector3 pos = cam->getPosition( );
	    // Fix from Praetor, so the camera used gives you "world-relative" coordinates
	    Vector3 pos = cam->getDerivedPosition( );

	    //updateStats( pos );

        uint i = cam->mCurrentCameraPageX; 
        uint j = cam->mCurrentCameraPageZ;

        PagingLandScapePage *p = mPages[ i ][ j ];
//        if ( p->isLoadable( ) )
//        {
//            PagingLandScapeTile *t = p->getTile(cam->mCurrentCameraTileX, cam->mCurrentCameraTileZ);
//            if ( t  && t->isLoaded())
//            {
//                t->getRenderable()->setMaxLod (false);
//            }
//        } // if ( p->isLoadable( ) )

	    // Update only if the camera was moved
        // make sure in the bounding box of landscape	
	    pos.y = 127.0f;
        bool need_touch = ( mTimePreLoaded < 0 );
	    if (//mLoadedPages.size() <  mOptions->NumPages &&
            mOptions->cameraThreshold < ( cam->mLastCameraPos - pos ).squaredLength( ) )
	    {	
            // Check for the camera position in the LandScape Pages list, 
            // we check if we are in the inside the security zone
		    //  if not, launch the change routine and 
            // update the camera position mCurrentCameraX, mCurrentCameraY.
            const int page_pos = p->isCameraIn( pos );  
      
            if ( page_pos == PAGE_OUTSIDE )
            {

                //gets page indices (if outside Terrain gets nearest page)
			    getPageIndices( pos.x, pos.z, i, j, true ); 
                // if outside all pages, just don't do any loading until pages does really change
                //if ( i != cam->mCurrentCameraPageX || j != cam->mCurrentCameraPageZ )
                {  
                    // Have the current page be loaded now !
                    p = mPages[ i ][ j ];
		            if ( !p->isLoaded( ) )
		            {                    
                        removeFromQueues( p );
			            p->load( );
                        mLoadedPages.push_back( p );
                        // make sure this brutal loading doesn't impact on fps
                        mNextQueueFrameCount = 20;   
		            }
                    else
                        p->touch( );    

				    // update the camera page position
                    // does modify mIniX, mFinX, mIniZ, mFinZ
                    cam->updatePaging( i, j );

                    need_touch = true;
                    mTerrainReady = false;               
                }
            }
            // update the camera info :
            // Update current Tile
            if ( p->isLoadable( ) )
            {
                PagingLandScapeTile *t = getTile( pos.x, pos.z, i, j, true );
                if ( t )
                {
                    PagingLandScapeTileInfo *CurrentTileInfo  = t->getInfo( );
                    if ( CurrentTileInfo )
                    {          
                        cam->mCurrentCameraTileX = CurrentTileInfo->tileX;
                        cam->mCurrentCameraTileZ = CurrentTileInfo->tileZ;
                    }
                }
            }
            // Update the last camera position
            cam->mLastCameraPos = pos;
        }

        const uint iniX = cam->mIniX;
        const uint finX = cam->mFinX;

		const uint iniZ = cam->mIniZ;
		const uint finZ = cam->mFinZ;
        if ( need_touch )
        {
            // Queue the rest				
            // Loading  must be done one by one to avoid FPS drop, so they are queued.
            // We must load the next visible landscape pages, 
            // check the landscape boundaries	

			const uint preIniX = cam->mPreIniX;
            const uint preFinX = cam->mPreFinX;

            const uint preIniZ = cam->mPreIniZ;
			const uint preFinZ = cam->mPreFinZ;

            for ( i = preIniX; i <= preFinX; i++ )
            {
                
                PagingLandScapePageRow* li = &mPages[ i ];
                for ( j = preIniZ; j <= preFinZ; j++ )
	            {
                    p = ( *li )[j];
                    if ( !( p->mIsLoading || p->isLoaded( ) ) )
                    {                        
                        if( ( j >= iniZ ) && ( j <= finZ ) && ( i >= iniX ) && ( i <= finX ) )
                        {
                            removeFromQueues( p );
    				        mPageLoadQueue.push_back( p );                            
                            p->mIsLoading = true;
                        }
                        else if ( !( p->mIsPreLoading || p->isPreLoaded( ) ) )
                        {
                            removeFromQueues( p );
				            mPagePreloadQueue.push_back( p );                    
                            p->mIsPreLoading = true;	
                        }
                    }
                    p->touch( );
                }
            }
            mTimePreLoaded = 20;
        }    

        const bool lightchange = mOptions->lightmoved;
        //pos = cam->getDerivedPosition( );
        // hide page not visible by this Camera
        // Notify those Page (update tile vis/ rend load on cam distance)
        // update Page Texture if needed
        PagingLandScapePagelist::iterator l, lend = mLoadedPages.end( );
        for ( l = mLoadedPages.begin( ); l != lend; ++l )
        {
            p = ( *l );
            uint x,z;
            p->getCoordinates( x, z );
            if ( ( z >= iniZ ) && ( z <= finZ ) && ( x >= iniX ) && ( x <= finX ) )
            {
                if ( p->_Notify( pos, cam ) )
                {
//                  mInfo->pageX, mInfo->pageZ, 
                 
                    PagingLandScapeTexture *tex = mTexture->getTexture( x, z );
                    assert( tex );
                    if ( lightchange || tex->needUpdate( ) )
					{
                        tex->update( );
					}
                }
            }
            else
            {
                 p->_Show( false );
            }
        } 

        if ( lightchange )
		{
            mOptions->lightmoved = false;
		}

        // Should be called every count frame only 
        // to minimize fps impact
        if ( mNextQueueFrameCount-- < 0 )
        {
            mNextQueueFrameCount = 5;
            
            //pos = currentpage->getCenter( );

            i = cam->mCurrentCameraPageX;
            j = cam->mCurrentCameraPageZ;
             //	We try to Load
            p = find_nearest( pos, i, j, mPageLoadQueue );
		    if ( p && !p->isLoaded( ) )
            {
			    p->load( );
                p->mIsLoading = false;   

                mLoadedPages.push_back( p );
                mTextureLoadedPages.remove( p );
            }
            else
            {
                //	We try to TextureLoad
                p = find_nearest( pos, i, j, mPageTextureloadQueue );
		        if ( p && !p->isTextureLoaded( ) )
                {
			        p->loadTexture( );
                    p->mIsTextureLoading = false;   

                    mTextureLoadedPages.push_back( p );
                    mPreLoadedPages.remove( p );
                }
                else
                {
                     //	We try to PreLoad
	                p = find_nearest( pos, i, j, mPagePreloadQueue );
	                if ( p && !p->isPreLoaded( ) )
	                {
		                p->preload( );
                        p->mIsPreLoading = false;        

                        mPreLoadedPages.push_back( p );
                        mUnloadedPages.remove( p );

                        mPageTextureloadQueue.push_back( p );
	                }
                }
            }
        } // if ( mNextQueueFrameCount-- < 0 )
        
        // set current tile at max lod.
//        p = mPages[ cam->mCurrentCameraPageX ][ cam->mCurrentCameraPageZ ];
//        if ( p->isLoadable( ) )
//        {
//            PagingLandScapeTile *t = p->getTile(cam->mCurrentCameraTileX, cam->mCurrentCameraTileZ);
//            if ( t && t->isLoaded())
//            {
//                t->getRenderable()->setMaxLod (true);
//            }
//        } // if ( p->isLoadable( ) )

        // This Frame has seen a Camera.
        mOnFrame = true;
    }
   
    //-----------------------------------------------------------------------
    void PagingLandScapePageManager::removeFromQueues( PagingLandScapePage* p )
    {
        if ( p->mIsLoading )
        {
            p->mIsLoading = false;
            mPageLoadQueue.remove( p );
        }
        if ( p->mIsPreLoading )
        {
            p->mIsPreLoading = false;
            mPagePreloadQueue.remove( p );
        }
        if ( p->mIsTextureLoading )
        {
            p->mIsTextureLoading = false;
            mPageTextureloadQueue.remove( p );
        }
    }

    //-----------------------------------------------------------------------
    PagingLandScapePage *PagingLandScapePageManager::find_nearest( const Vector3 &pos, const uint x, const uint z, PagingLandScapePagelist &mQueue ) const
    {
        if (mQueue.empty())
            return 0;
    
        Real mindist = std::numeric_limits< Real >::max( );
        PagingLandScapePage *p = 0;
        PagingLandScapePagelist::iterator qend = mQueue.end( );
        for (PagingLandScapePagelist::iterator q = mQueue.begin( ); q != qend; ++q )
        {
            Real res = ( pos - ( *q )->getCenter( ) ).squaredLength( );
            if ( res < mindist )
            {
                mindist = res;
                p = ( *q );
            }
        }
        if ( p )
		{
            mQueue.remove( p );
		}
        return p;
       
    };

    //-----------------------------------------------------------------------
    uint PagingLandScapePageManager::getCurrentCameraPageX( void ) const
    {
        if ( mCurrentcam )
		{
	        return mCurrentcam->mCurrentCameraPageX;
		}
        return 0;
    }

    //-----------------------------------------------------------------------
    uint PagingLandScapePageManager::getCurrentCameraPageZ( void ) const
    {
        if ( mCurrentcam )
		{
	        return mCurrentcam->mCurrentCameraPageZ;
		}
        return 0;
    }

    //-----------------------------------------------------------------------
    uint PagingLandScapePageManager::getCurrentCameraTileX( void ) const
    {
        if ( mCurrentcam )
		{
            return mCurrentcam->mCurrentCameraTileX;
		}
        return 0;
    }

    //-----------------------------------------------------------------------
    uint PagingLandScapePageManager::getCurrentCameraTileZ( void ) const
    {
        if ( mCurrentcam )
		{
			return mCurrentcam->mCurrentCameraTileZ;
		}
       return 0;
    }

    //-----------------------------------------------------------------------
    int PagingLandScapePageManager::getLoadedPageSize( void ) const
    {
	    return static_cast< int >( mLoadedPages.size( ) );
    }

    //-----------------------------------------------------------------------
    int PagingLandScapePageManager::getTextureLoadedPageSize( void ) const
    {
	    return static_cast< int >( mTextureLoadedPages.size( ) );
    }

    //-----------------------------------------------------------------------
    int PagingLandScapePageManager::getPreLoadedPageSize( void ) const
    {
	    return static_cast< int >( mPreLoadedPages.size( ) );
    }

    //-----------------------------------------------------------------------
    int PagingLandScapePageManager::getUnloadedPageSize( void ) const
    {
	    return static_cast< int >( mUnloadedPages.size( ) );
    }

    //-----------------------------------------------------------------------
    int PagingLandScapePageManager::getPagePreloadQueueSize( void ) const
    {
	    return static_cast< int >( mPagePreloadQueue.size( ) );
    }

    //-----------------------------------------------------------------------
    int PagingLandScapePageManager::getPageTextureloadQueueSize( void ) const
    {
	    return static_cast< int >( mPageTextureloadQueue.size( ) );
    }

    //-----------------------------------------------------------------------
    int PagingLandScapePageManager::getPageLoadQueueSize( void ) const
    {
	    return static_cast< int >( mPageLoadQueue.size( ) );
    }

    //-----------------------------------------------------------------------
    void PagingLandScapePageManager::getGlobalToPage( Real& x, Real& z ) const
    {
        const Real inv_pSize = 1.0f / ( mOptions->PageSize - 1 );
      
	    x = static_cast< int >( ( (x / mOptions->scale.x) + mOptions->maxUnScaledX ) * inv_pSize );
	    z = static_cast< int >( ( (z / mOptions->scale.z) + mOptions->maxUnScaledZ ) * inv_pSize );
    }
    //-----------------------------------------------------------------------
    bool PagingLandScapePageManager::getRealPageIndicesUnscaled( const Real posx, 
        const Real posz, uint& x, uint& z ) const
    {
        const Real inv_pSize = 1.0f / ( mOptions->PageSize - 1 );
      
	    const int lx = static_cast< int >( ( posx + mOptions->maxUnScaledX ) * inv_pSize );
	    const int lz = static_cast< int >( ( posz + mOptions->maxUnScaledZ ) * inv_pSize );

        const int w = static_cast< int >( mOptions->world_width );
        const int h = static_cast< int >( mOptions->world_height );

	    // make sure indices are not negative or outside range of number of pages
	    if ( lx >= w || lx < 0 || lz >= h || lz < 0 )
	    {
		   return false;
	    }
        else 
        {
            x = static_cast< uint >( lx );
            z = static_cast< uint >( lz );
            return true;
        }
    }

    //-----------------------------------------------------------------------
    void PagingLandScapePageManager::getNearestPageIndicesUnscaled( const Real posx, const Real posz, uint& x, uint& z ) const
    {
        // adjust x and z to be local to page
        const Real inv_pSize = 1.0f / ( mOptions->PageSize - 1 );
      
	    const int lx = static_cast< int >( ( posx + mOptions->maxUnScaledX ) * inv_pSize );
	    const int lz = static_cast< int >( ( posz + mOptions->maxUnScaledZ ) * inv_pSize );

        const int w = static_cast< int >( mOptions->world_width );
        const int h = static_cast< int >( mOptions->world_height );

	    // make sure indices are not negative or outside range of number of pages
	    if ( lx >= w )
	    {
		    x = static_cast< uint >( w - 1 );
	    }
        else if ( lx < 0 )
        {
            x = 0;
        }
        else
		{
            x = static_cast< uint >( lx );
		}

	    if ( lz >= h ) 
	    {
		    z = static_cast< uint >( h - 1 );
	    }
        else if ( lz < 0 )
        {
            z = 0;
        }
        else
		{
            z = static_cast< uint >( lz );
		}
    }

    //-----------------------------------------------------------------------
    bool PagingLandScapePageManager::getPageIndices( const Real posx, const Real posz, uint& x, uint& z, bool alwaysAnswer ) const
    {
        if ( alwaysAnswer )
        {
            getNearestPageIndicesUnscaled( posx / mOptions->scale.x, posz / mOptions->scale.z, x, z );
            return true;
        }
        else
		{
            return getRealPageIndicesUnscaled( posx / mOptions->scale.x, posz / mOptions->scale.z, x, z );
		}
    }
    //-----------------------------------------------------------------------
    bool PagingLandScapePageManager::getRealTileIndicesUnscaled( const Real posx, const Real posz, 
                                                                const uint pagex, const uint pagez, 
                                                                uint& x, uint& z ) const
    {
		// adjust x and z to be local to page
        const Real inv_tSize = 1.0f / ( mOptions->TileSize - 1 );
        const int pSize = mOptions->PageSize - 1;

        int tilex = static_cast< int >( ( posx - ( ( pagex * pSize ) - mOptions->maxUnScaledX ) ) * inv_tSize ); 
        //- mOptions->maxUnScaledX

        int tilez = static_cast< int >( ( posz - ( ( pagez * pSize ) - mOptions->maxUnScaledZ ) ) * inv_tSize );
        //- mOptions->maxUnScaledZ

        const int tilesPerPage = static_cast< int >( ( pSize * inv_tSize ) - 1 );

        if ( tilex > tilesPerPage || tilex < 0 || tilez > tilesPerPage || tilez < 0 )
		{
            return false;
		}
        else
        {
            x = static_cast< uint >( tilex );
            z = static_cast< uint >( tilez );
            return true;
        }
    }

    //-----------------------------------------------------------------------
    void PagingLandScapePageManager::getNearestTileIndicesUnscaled( const Real posx, const Real posz, 
                                                                    const uint pagex, const uint pagez, 
                                                                    uint& x, uint& z ) const
    {
        // adjust x and z to be local to page
        const Real inv_tSize = 1.0f / ( mOptions->TileSize - 1 );
        const int pSize = mOptions->PageSize - 1;

        const int tilex = static_cast< int >( ( posx - ( ( pagex * pSize ) - mOptions->maxUnScaledX ) ) * inv_tSize ); 
        //- mOptions->maxUnScaledX

        const int tilez = static_cast< int >( ( posz - ( ( pagez * pSize ) - mOptions->maxUnScaledZ ) ) * inv_tSize );
        //- mOptions->maxUnScaledZ

        const int tilesPerPage = static_cast< int >( ( pSize * inv_tSize ) - 1 );

        if ( tilex > tilesPerPage )
		{
            x = static_cast< uint >( tilesPerPage );
		}
        else if ( tilex < 0 )
		{
            x = 0;
		}
        else
		{
            x = static_cast< uint >( tilex );
		}

        if ( tilez > tilesPerPage )
		{
            z = static_cast< uint >( tilesPerPage );
		}
        else if( tilez < 0 )
		{
            z = 0;
		}
        else
		{
            z = static_cast< uint >( tilez );
		}
    }
    //-----------------------------------------------------------------------
    bool PagingLandScapePageManager::getTileIndices( const Real posx, const Real posz, 
                                                    const uint pagex, const uint pagez, 
                                                    uint& x, uint& z, bool alwaysAnswer ) const
    {
        if ( alwaysAnswer )
        {
            getNearestTileIndicesUnscaled( posx / mOptions->scale.x, posz / mOptions->scale.z, pagex, pagez, x, z );
            return true;
        }
        else
        {
            return getRealTileIndicesUnscaled( posx / mOptions->scale.x, posz / mOptions->scale.z, pagex, pagez, x, z );
        }
    }
    //-----------------------------------------------------------------------
    PagingLandScapeTile* PagingLandScapePageManager::getTileUnscaled( const Real posx, const Real posz, const uint pagex, const uint pagez, bool alwaysAnswer )
    {
        uint tilex, tilez;
        if ( alwaysAnswer )
        {
            getNearestTileIndicesUnscaled( posx, posz, pagex, pagez, tilex, tilez );
            return mPages[ pagex ][ pagez ]->getTile( tilex, tilez );
        }
        else
        {
            if ( getRealTileIndicesUnscaled( posx, posz, pagex, pagez, tilex, tilez ) )
			{
                return mPages[ pagex ][ pagez ]->getTile( tilex, tilez );
			}
            return 0;
        }
    }
    //-----------------------------------------------------------------------
    PagingLandScapeTile* PagingLandScapePageManager::getTile( const Real posx, const Real posz, const uint pagex, const uint pagez, bool alwaysAnswer )
    {
       return getTileUnscaled( posx / mOptions->scale.x, posz / mOptions->scale.z, pagex, pagez, alwaysAnswer );
    }
    //-----------------------------------------------------------------------
    PagingLandScapeTile* PagingLandScapePageManager::getTile( const Real posx, const Real posz, bool alwaysAnswer )
    {
        return getTileUnscaled( posx / mOptions->scale.x, posz / mOptions->scale.z, alwaysAnswer );
    }
    //-----------------------------------------------------------------------
    PagingLandScapeTile* PagingLandScapePageManager::getTileUnscaled( const Real posx, const Real posz, bool alwaysAnswer )
    {
        uint pagex, pagez;
        if ( alwaysAnswer )
        {
            uint tilex, tilez;
            getNearestPageIndicesUnscaled( posx, posz, pagex, pagez );
            getNearestTileIndicesUnscaled( posx, posz, pagex, pagez, tilex, tilez );
            return mPages[ pagex ][ pagez ]->getTile( tilex, tilez );
        }
        else
        {
            if ( getRealPageIndicesUnscaled( posx, posz, pagex, pagez ) )
            {
                uint tilex, tilez;
                if ( getRealTileIndicesUnscaled( posx, posz, pagex, pagez, tilex, tilez ) )
				{
                    return mPages[ pagex ][ pagez ]->getTile( tilex, tilez );
				}
            }                
            return 0;
        }
    }
    //-------------------------------------------------------------------------
    PagingLandScapeTile* PagingLandScapePageManager::getTilePage (uint &posx, uint &posz, 
                                                                  const uint pagex, const uint pagez)
    {        
        const Real tSize = mOptions->TileSize - 1;
        const Real inv_tSize = 1.0f / tSize;
        const int tilex = static_cast< int >(posx * inv_tSize ); 
        const int tilez = static_cast< int >(posz * inv_tSize );

        const int pSize = mOptions->PageSize - 1;
        const int tilesPerPage = static_cast< int > (mOptions->NumTiles - 1);

        uint x;
        if ( tilex > tilesPerPage )
		{
            x = static_cast< uint >( tilesPerPage  );
		}
        else if ( tilex < 0 )
		{
            x = 0;
		}
        else
		{
            x = static_cast< uint >( tilex );
		}

        uint z;
        if ( tilez > tilesPerPage )
		{
            z = static_cast< uint >( tilesPerPage );
		}
        else if( tilez < 0 )
		{
            z = 0;
		}
        else
		{
            z = static_cast< uint >( tilez );
		}
        posx = posx - static_cast< uint > (x * tSize);
        posz = posz - static_cast< uint > (z * tSize);
       return mPages[ pagex ][ pagez ]->getTile( x, z );
    }
    //-------------------------------------------------------------------------
	void PagingLandScapePageManager::setWorldGeometryRenderQueue( RenderQueueGroupID qid )
	{
        mRenderQueueGroupID = qid;
        PagingLandScapePagelist::iterator l, lend = mLoadedPages.end( );
        for ( l = mLoadedPages.begin( ); l != lend; ++l )
        {
            PagingLandScapePage *p = ( *l );
            //if ( p->isVisible( ) )
            {
                p->setRenderQueue( qid );
            }
        }
	}

}
