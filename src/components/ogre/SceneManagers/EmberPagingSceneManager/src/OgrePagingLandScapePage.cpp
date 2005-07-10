/***************************************************************************
	OgrePagingLandScapePage.cpp  -  description
	-------------------
	begin                : Sat Mar 08 2003
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

#include "OgreStringConverter.h"
#include "OgreSceneNode.h"
#include "OgreException.h"

#include "OgrePagingLandScapeOptions.h"
#include "OgrePagingLandScapeCamera.h"
#include "OgrePagingLandScapePageManager.h"
#include "OgrePagingLandScapeRenderableManager.h"
#include "OgrePagingLandScapeData2DManager.h"
#include "OgrePagingLandScapeData2D.h"
#include "OgrePagingLandScapeTileInfo.h"
#include "OgrePagingLandScapeTile.h"
#include "OgrePagingLandScapeTileManager.h"
#include "OgrePagingLandScapeTextureManager.h"

#include "OgrePagingLandScapePage.h"
#include "OgrePagingLandScapePageRenderable.h"
#include "OgrePagingLandScapeListenerManager.h"

#include "OgrePagingLandScapeSceneManager.h"

#include "OgrePagingLandScapeHorizon.h"

namespace Ogre
{
    PagingLandScapePage::PagingLandScapePage( const uint tableX, const uint tableZ ) :
        mPageNode (0),
	    mTableX (tableX),
	    mTableZ (tableZ)
    {  
        for ( uint i = 0; i < 4; i++ )
        {
            mNeighbors[ i ] = 0;
        }   
    }
    //-----------------------------------------------------------------------
    PagingLandScapePage::~PagingLandScapePage()
    {
        PagingLandScapePage::uninit();
    }
    //-----------------------------------------------------------------------
    void PagingLandScapePage::init ()
    {  
        mNumTiles = PagingLandScapeOptions::getSingleton().NumTiles;

	    const uint size = PagingLandScapeOptions::getSingleton().PageSize - 1;
	    // Boundaries of this page
	    // the middle page is at world coordinates 0,0
        const Real factorX = size * PagingLandScapeOptions::getSingleton().scale.x;
        const Real factorZ = size * PagingLandScapeOptions::getSingleton().scale.z;

	    mIniX = static_cast<Real> (static_cast<int> (mTableX + mTableX - PagingLandScapeOptions::getSingleton().world_width)) * 0.5f * factorX ;		
	    mIniZ = static_cast<Real> (static_cast<int> (mTableZ + mTableZ - PagingLandScapeOptions::getSingleton().world_height)) * 0.5f * factorZ ;		

	    const Real EndX = mIniX + factorX;
	    const Real EndZ = mIniZ + factorZ;
	    const Real MaxHeight = PagingLandScapeData2DManager::getSingleton().getMaxHeight(mTableX, mTableZ);
        const Real chgfactor = PagingLandScapeOptions::getSingleton().change_factor;

	    mBounds.setExtents(  mIniX , 
                             0.0f, 
                             mIniZ , 
						     EndX , 
                             MaxHeight, 
                             EndZ );

	    //Change Zone of this page
	    mBoundsInt.setExtents( mIniX + chgfactor, 
                               0.0f, 
                               mIniZ + chgfactor,
						       EndX - chgfactor, 
                               MaxHeight, 
                               EndZ - chgfactor	);

         
        mBoundsExt.setExtents( mIniX - factorX * 1.5f, 
                               - MaxHeight * 0.5f, 
                               mIniZ - factorZ * 1.5f,
                               mIniX + factorX * 1.5f, 
                               MaxHeight * 1.5f , 
                               mIniZ + factorZ * 1.5f);


        //create a root landscape node.
        const String NodeName =  "PagingLandScapePage." 
            + StringConverter::toString( mTableX ) + "." + 
            StringConverter::toString( mTableZ );


        assert (mPageNode == 0);

	    mPageNode = PagingLandScapeSceneManager::getSingleton().getRootSceneNode()
            ->createChildSceneNode (NodeName + ".Node");

        assert (mPageNode);

	    // Set node position
	    mPageNode->setPosition( static_cast<Real> (mIniX) , 
                                0.0f, 
                                static_cast<Real> (mIniZ) );


        if (PagingLandScapeOptions::getSingleton().BigImage)
        {
            mRenderable = new PagingLandScapePageRenderable(mPageNode->getName () + "rend", 
                                                            mTableX, mTableZ,
                                                            mBounds);
            mPageNode->attachObject(mRenderable);
            mRenderable->load ();
        }
        mPageNode->needUpdate();

        mIsLoading = false;
	    mIsPreLoading = false;
        mIsTextureLoading = false;
       
	    mIsUnloading = false;
	    mIsPostUnloading = false;
        mIsTextureunloading = false;

	    mIsLoaded = false;
	    mIsTextureLoaded = false;
	    mIsPreLoaded = false;      

        mIsLoadable = true;
        touch ();
    }
    //-----------------------------------------------------------------------
    void PagingLandScapePage::uninit ()
    {
        postUnload ();  

		if (PagingLandScapeOptions::getSingleton().BigImage)
        {
            if (mPageNode)
		        mPageNode->detachObject (mRenderable->getName());
            delete mRenderable;
        }	 
        if (mPageNode)
        {
		    static_cast<SceneNode*>( mPageNode->getParent() )->removeAndDestroyChild( mPageNode->getName() );
            mPageNode = 0;
        }
        
        for ( uint i = 0; i < 4; i++ )
        {
            mNeighbors[ i ] = 0;
        }
    }
    //-----------------------------------------------------------------------
    void PagingLandScapePage::setMapMaterial()
    {    
        if (PagingLandScapeOptions::getSingleton().BigImage)
        {
            mRenderable->setMaterial ( PagingLandScapeTextureManager::getSingleton().getMapMaterial() );
        }
    }
    //-----------------------------------------------------------------------
    void PagingLandScapePage::touch ()
    { 
        mTimePreLoaded = 400;
    }
    //-----------------------------------------------------------------------
    const bool PagingLandScapePage::touched ()        
    { 
        if ( mTimePreLoaded == 0)
            return true;
        mTimePreLoaded--; 
        return false;
    }
    //-----------------------------------------------------------------------
    void PagingLandScapePage::preload( )
    {
        touch ();

	    if ( mIsPreLoaded )
		    return;

	    mIsLoadable = PagingLandScapeData2DManager::getSingleton().load (mTableX, mTableZ);

	    mIsPreLoaded = true;

        PagingLandscapeListenerManager::getSingleton().firePagePreloaded(mTableX, mTableZ, 
            PagingLandScapeData2DManager::getSingleton().getData2d(mTableX, mTableZ)->getHeightData(),
            mBounds);
    }
    //-----------------------------------------------------------------------
    void PagingLandScapePage::loadTexture( )
    {        
        touch ();
	    if (!mIsPreLoaded)
		    preload ();
        if (!mIsTextureLoaded) 
        {
            if (mIsLoadable)
                PagingLandScapeTextureManager::getSingleton().load(mTableX, mTableZ);
            mIsTextureLoaded = true;
        }
    }
    //-----------------------------------------------------------------------
    void PagingLandScapePage::load( )
    {
        touch ();
        if (mIsLoaded)
            return;
	    if ( !mIsPreLoaded)
		    preload ();
	    if ( !mIsTextureLoaded)
		    loadTexture ();

        //mPageNode->showBoundingBox (true) ;
        if (mIsLoadable)
        {
            const uint numTiles = mNumTiles;
            uint i, j;

            mTiles.reserve (numTiles);
            mTiles.resize (numTiles);

	        for (i = 0; i < numTiles; ++i )
	        {
                mTiles[i].reserve (numTiles);
                mTiles[i].resize (numTiles);
	        }

	        for (i = 0; i < numTiles; ++i )
	        {
	            for (j = 0; j < numTiles; ++j )
                {
                    //char name[ 24 ];
			        //sprintf( name, "page[%d,%d][%d,%d]", mTableX, mTableZ, i, j );

			        PagingLandScapeTile *tile = PagingLandScapeTileManager::getSingleton().getTile();
			        if ( tile != 0 )
			        {
				        mTiles[ i ][ j ] = tile;
				        tile->init( mPageNode, mTableX, mTableZ, i, j );
			        }
			        else
			        {
				        String err = "Error: Invalid Tile: Make sure the default TileManager size is set to WorldWidth * WorldHeight * 4. Try increasing MaxNumTiles in the configuration file.";
				        OGRE_EXCEPT( Exception::ERR_INVALIDPARAMS, err, "PagingLandScapePage::load" );
			        }
		        }
	        }
	        for (i = 0; i < numTiles; ++i )
	        {
	            for (j = 0; j < numTiles; ++j )
		        {		
                    if ( j != numTiles - 1 )
                    {
                        mTiles[ i ][ j ]-> _setNeighbor( SOUTH, mTiles[ i ][ j + 1 ] );
                        mTiles[ i ][ j + 1 ]  -> _setNeighbor( NORTH, mTiles[ i ][ j ] );
                    }
                    if ( i != numTiles - 1 )
                    {
                        mTiles[ i ][ j ] -> _setNeighbor( EAST, mTiles[ i + 1 ][ j ] );
                        mTiles[ i + 1 ][ j ] -> _setNeighbor( WEST, mTiles[ i ][ j ] );    
                    }           
		        }
	        }
            if (mNeighbors[EAST] && 
                mNeighbors[EAST]->isLoaded() && mNeighbors[EAST]->isLoadable())
            {
                i = numTiles - 1;
                for (j = 0; j < numTiles; j++ )
                {	
                    PagingLandScapeTile *t_nextpage =  mNeighbors[EAST]->getTile  ( 0 , j );
                    PagingLandScapeTile *t_currpage =  mTiles[ i ][ j ];
                    t_currpage -> _setNeighbor( EAST, t_nextpage );
                    t_nextpage -> _setNeighbor( WEST, t_currpage );
                }
            }
            if (mNeighbors[WEST] && 
                mNeighbors[WEST]->isLoaded() && mNeighbors[WEST]->isLoadable())
            {
                i = numTiles - 1;
                for (j = 0; j < numTiles; j++ )
                {	
                    PagingLandScapeTile *t_nextpage =  mNeighbors[WEST]->getTile  ( i , j );
                    PagingLandScapeTile *t_currpage =  mTiles[ 0 ][ j ];
                    t_currpage -> _setNeighbor( WEST, t_nextpage );
                    t_nextpage -> _setNeighbor( EAST, t_currpage );
                }
            }

            if (mNeighbors[SOUTH] && 
                mNeighbors[SOUTH]->isLoaded() && mNeighbors[SOUTH]->isLoadable())
            {       
                j = numTiles - 1;
                for (i = 0; i < numTiles; i++ )
                {	
                    PagingLandScapeTile *t_nextpage =  mNeighbors[SOUTH]->getTile  ( i , 0 );
                    PagingLandScapeTile *t_currpage =  mTiles[ i ][ j ];
                    t_currpage -> _setNeighbor( SOUTH, t_nextpage);
                    t_nextpage -> _setNeighbor( NORTH, t_currpage );
                }
            }
            if (mNeighbors[NORTH] && 
                mNeighbors[NORTH]->isLoaded() && mNeighbors[NORTH]->isLoadable())
            {       
                j = numTiles - 1;
                for (i = 0; i < numTiles; i++ )
                {	
                    PagingLandScapeTile *t_nextpage =  mNeighbors[NORTH]->getTile  ( i , j );
                    PagingLandScapeTile *t_currpage =  mTiles[ i ][ 0 ];
                    t_currpage -> _setNeighbor(NORTH, t_nextpage );
                    t_nextpage -> _setNeighbor(SOUTH, t_currpage );
                }
            }
        }
	    mIsLoaded = true;
        
        PagingLandscapeListenerManager::getSingleton().firePageLoaded(mTableX, mTableZ, 
            PagingLandScapeData2DManager::getSingleton().getData2d(mTableX, mTableZ)->getHeightData(),
            mBounds);
    }
    //-----------------------------------------------------------------------
    void PagingLandScapePage::unload( )
    {
        if (mIsLoaded)
        {
            // Unload the Tiles
            PagingLandScapeTiles::iterator iend = mTiles.end();
            for (PagingLandScapeTiles::iterator it = mTiles.begin(); 
                it != iend; 
                ++it)
            {
                
                std::for_each( it->begin (), 
                                it->end (),  
                                std::mem_fun( &PagingLandScapeTile::uninit ));

                it->clear();   
            } 
	        mTiles.clear();
            mIsLoaded = false;            
            PagingLandscapeListenerManager::getSingleton().firePageUnloaded(mTableX, mTableZ, 
                PagingLandScapeData2DManager::getSingleton().getData2d(mTableX, mTableZ)->getHeightData(),
            mBounds);

        }
    }
    //-----------------------------------------------------------------------
    void PagingLandScapePage::unloadTexture( )
    {        
        unload();
        if (mIsTextureLoaded) 
        {
            if (mIsLoadable)
                PagingLandScapeTextureManager::getSingleton().unload(mTableX, mTableZ);
            mIsTextureLoaded = false;
        }
    }
    //-----------------------------------------------------------------------
    void PagingLandScapePage::postUnload( )
    {
	    unloadTexture ();
	    if ( mIsPreLoaded )
	    {
		    mIsPreLoaded = false;

            if (mIsLoadable)
		        PagingLandScapeData2DManager::getSingleton().unload( mTableX, mTableZ );

            PagingLandscapeListenerManager::getSingleton().firePagePostunloaded (mTableX, mTableZ);
	    }
    }

    //-----------------------------------------------------------------------
    int PagingLandScapePage::isCameraIn( const Vector3 & pos ) const
    {
	    if ( mBounds.intersects( pos ) )
	    {
		    if ( mBoundsInt.intersects( pos )  )
		    {
			    // Full into this page
			    return PAGE_INSIDE;
		    }
		    else
		    {
			    // Over the change zone
			    return PAGE_CHANGE;
		    }
	    }
	    else
	    {
		    // Not in this page
		    return PAGE_OUTSIDE;
	    }
    }
    //-----------------------------------------------------------------------
    void PagingLandScapePage::_Show(const bool do_show)
    {
        if (do_show) 
        {
            if (!mVisible)
            {
                PagingLandscapeListenerManager::getSingleton().firePageShow (mTableX, mTableZ, 
                    PagingLandScapeData2DManager::getSingleton().getData2d(mTableX, mTableZ)->getHeightData(),
            mBounds);
                
                if (mIsLoadable)
                {
                    uint i,k;
	                for (i = 0; i < mNumTiles; ++i )
                    {
                        PagingLandScapeTileRow &tr = mTiles[ i ];
	                    for (k = 0; k < mNumTiles; ++k)
	                    {
                            tr[ k ]->setInUse(true);
			            }
		            }
                }
                mVisible = true;
            }
        }
        else if (mVisible)
        {
            PagingLandscapeListenerManager::getSingleton().firePageHide (mTableX, mTableZ, 
                    PagingLandScapeData2DManager::getSingleton().getData2d(mTableX, mTableZ)->getHeightData(),
            mBounds);
             if (mIsLoadable)
            {
                uint i,k;
	            for (i = 0; i < mNumTiles; ++i)
                {
                    PagingLandScapeTileRow &tr = mTiles[ i ];
	                for (k = 0; k < mNumTiles; ++k )
	                {
                        tr[ k ]->setInUse(false);
			        }
		        }
            }
            mVisible = false;
        }
    }
    //-----------------------------------------------------------------------
    bool PagingLandScapePage::_Notify(const Vector3 &pos, PagingLandScapeCamera* Cam)
    {
	    if (mIsLoaded && mIsLoadable)
        {
            if (
                Cam->getVisibility (mBoundsExt) 
                //&& PagingLandScapeHorizon::getSingleton ().IsPageVisible (Cam, mTableX, mTableZ)
                )
	        {  
                mVisible = true;
                uint i,k;
	            for (i = 0; i < mNumTiles; i++ )
	            {
                    PagingLandScapeTileRow &tr = mTiles[ i ];
	                for (k = 0; k < mNumTiles; k++ )
                    {
                        tr[ k ]->_Notify( pos, Cam);
			        }
		        }
                return true;
	        }
            else if (mVisible)
            {
                _Show (false);
                return false;
            }
        }
        return false;
    }

    //-----------------------------------------------------------------------
    PagingLandScapeTile *PagingLandScapePage::getTile(const Vector3& pos)
    {
        if (mIsLoaded && mIsLoadable)
        {
            const uint x =  static_cast<uint> (pos.x / PagingLandScapeOptions::getSingleton().scale.x / (PagingLandScapeOptions::getSingleton().TileSize));
            const uint z =  static_cast<uint> (pos.z / PagingLandScapeOptions::getSingleton().scale.z / (PagingLandScapeOptions::getSingleton().TileSize));

            if (mTiles[x][z] && mTiles[x][z]-> isLoaded ())
                return mTiles[x][z];
        }
        return 0;
    }
    //-------------------------------------------------------------------------
	void PagingLandScapePage::_updateLod()
    {
        if (mIsLoaded && mIsLoadable)
        {
            uint i,k;
	        for (i = 0; i < mNumTiles; ++i)
            {
                PagingLandScapeTileRow &tr = mTiles[ i ];
	            for (k = 0; k < mNumTiles; ++k )
	            {
                    PagingLandScapeTile *t = tr[ k ];
                    if (t->isVisible())
                        t->_updateLod();
			    }
		    }
        }
    }
    //-------------------------------------------------------------------------
	void PagingLandScapePage::setRenderQueue(RenderQueueGroupID qid)
	{
        if (mVisible && mIsLoadable)
        {
            uint i,k;
	        for (i = 0; i < mNumTiles; ++i)
            {
                PagingLandScapeTileRow &tr = mTiles[ i ];
	            for (k = 0; k < mNumTiles; ++k )
	            {
                    PagingLandScapeTile *t = tr[ k ];
                    if (t->isVisible())
                        t->setRenderQueueGroup(qid);
			    }
		    }
        }
	}
} //namespace
