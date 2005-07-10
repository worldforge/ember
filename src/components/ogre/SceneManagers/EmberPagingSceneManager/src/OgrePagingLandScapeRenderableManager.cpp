/***************************************************************************
  OgrePagingLandScapeRenderableManager.cpp  -  description
  -------------------
  begin                : Mon Jun 16 2003
  copyright            : (C) 2003-2005 by Jose A Milan && Tuan Kuranes
  email                : spoke2@supercable.es && tuan.kuranes@free.fr
***************************************************************************/

/***************************************************************************
*                                                                         *
*   This program is free software; you can redistribute it and/or modify  *
*   it under the terms of the GNU Lesser General Public License as published by  *
*   the Free Software Foundation; either version 2 of the License, or     *
*   (at your option) any later version.                                   *
*                                                                         *
***************************************************************************/

#include "OgreVector3.h"
#include "OgreColourValue.h"

#include "OgreMovableObject.h"

#include "OgreCamera.h"

#include "OgreSceneNode.h"

#include "OgreSimpleRenderable.h"

#include "OgrePagingLandScapeOptions.h"
#include "OgrePagingLandScapeCamera.h"

#include "OgrePagingLandScapeRenderable.h"
#include "OgrePagingLandScapeIndexBuffer.h"

#include "OgrePagingLandScapeRenderableManager.h"

#include "OgrePagingLandScapeTile.h"

namespace Ogre
{
    //-----------------------------------------------------------------------
    template<> PagingLandScapeRenderableManager* Singleton<PagingLandScapeRenderableManager>::ms_Singleton = 0;
    PagingLandScapeRenderableManager* PagingLandScapeRenderableManager::getSingletonPtr(void)
    {
	    return ms_Singleton;
    }
    PagingLandScapeRenderableManager& PagingLandScapeRenderableManager::getSingleton(void)
    {  
	    assert( ms_Singleton );  return ( *ms_Singleton );  
    }
    //-----------------------------------------------------------------------
    PagingLandScapeRenderableManager::PagingLandScapeRenderableManager( ) :
        mNumRenderables (0),
        mPageSize (0) ,
        mTileSize (0)
    {
        PagingLandScapeRenderable::mOpt = PagingLandScapeOptions::getSingletonPtr();
        mIndexes = new PagingLandScapeIndexBuffer();
    }
    //-----------------------------------------------------------------------
    PagingLandScapeRenderableManager::~PagingLandScapeRenderableManager( )
    {
        PagingLandScapeRenderableManager::freeTextureCoordinatesBuffers ();
        delete mIndexes;
    }
    //-----------------------------------------------------------------------
    void PagingLandScapeRenderableManager::clear()
    {

        if (!mTilesLoadQueue.empty ())
        {
            PagingLandScapeTile *t = mTilesLoadQueue.pop();
            while (t)
            {
                t->uninit ();
                t = mTilesLoadQueue.pop ();
            }
            mTilesLoadQueue.clear ();
        }


        // If renderables change over maps (+- lit, normals, etc...)
        std::for_each(mRenderables.begin (), 
                        mRenderables.end (),  
                        delete_object());
        mRenderables.clear();
        mQueue.clear();
    }
    //-----------------------------------------------------------------------
    void PagingLandScapeRenderableManager::load()
    {
        
        const uint nRend = PagingLandScapeOptions::getSingleton().num_renderables;
        if (mNumRenderables < nRend)
        {
            _addBatch (nRend - mNumRenderables);
        }
//        else if (mNumRenderables > nRend)
//        {
//            for (uint i = nRend; i < mNumRenderables; i++)
//	        {
//                PagingLandScapeRenderable *r = mRenderables[i];
//                mQueue.remove (r);
//                delete r;
//	        }
//            mNumRenderables = nRend;
//            mRenderables.resize (nRend);
//        }

        const uint pSize = PagingLandScapeOptions::getSingleton().PageSize;
        const uint tSize = PagingLandScapeOptions::getSingleton().TileSize;
        if (mPageSize != pSize || 
            mTileSize != tSize)
        {            

            mPageSize = PagingLandScapeOptions::getSingleton().PageSize;
            mTileSize = PagingLandScapeOptions::getSingleton().TileSize;

            freeTextureCoordinatesBuffers ();
            mPageSize = pSize;
            InitTextureCoordinatesBuffers();
        }
        mIndexes->load();       
    }
    //-----------------------------------------------------------------------
    void PagingLandScapeRenderableManager::InitTextureCoordinatesBuffers()
    {
        const uint NumTiles = PagingLandScapeOptions::getSingleton().NumTiles;

        mTexBuffs.reserve (NumTiles);
        mTexBuffs.resize (NumTiles);
	    for (uint  i = 0; i < NumTiles; ++i )
	    {
            mTexBuffs[i].reserve (NumTiles);
            mTexBuffs[i].resize (NumTiles);
	    }
    }
    //-----------------------------------------------------------------------
    void PagingLandScapeRenderableManager::freeTextureCoordinatesBuffers()
    {
        // Unload the Tiles
        if (!mTexBuffs.empty())
        {
            HardwareTextureBuffersCol::iterator iend = mTexBuffs.end();
            for (HardwareTextureBuffersCol::iterator i = mTexBuffs.begin(); 
                i != iend; 
                ++i)
            {
        //         std::for_each( i->begin (), 
        //                        i->end (),  
        //                        delete_object());

                i->clear();   
            } 
	        mTexBuffs.clear();    
        }
    }
    //-----------------------------------------------------------------------
    void PagingLandScapeRenderableManager::setTextureCoordinatesBuffers(const uint tilex, 
                                                                        const uint tilez,
                                                                        const HardwareVertexBufferSharedPtr &data)
    {
        assert (tilex < PagingLandScapeOptions::getSingleton().NumTiles && 
                tilez < PagingLandScapeOptions::getSingleton().NumTiles);

        mTexBuffs [tilex][tilez] = data;
    }
    //-----------------------------------------------------------------------
    HardwareVertexBufferSharedPtr PagingLandScapeRenderableManager::getTextureCoordinatesBuffers(
        const uint tilex, 
        const uint tilez)
    {
        assert (tilex < PagingLandScapeOptions::getSingleton().NumTiles && 
                tilez < PagingLandScapeOptions::getSingleton().NumTiles);

        return mTexBuffs [tilex][tilez];
    }
    //-----------------------------------------------------------------------
    PagingLandScapeRenderable *PagingLandScapeRenderableManager::getRenderable( )
    {
	    if ( mQueue.empty() )
	    {
		    // We don´t have more renderables, so we are going to add more
		    _addBatch(PagingLandScapeOptions::getSingleton().num_renderables_increment);
		    // Increment the next batch by a 10%
		    PagingLandScapeOptions::getSingleton().num_renderables_increment += static_cast<uint> (PagingLandScapeOptions::getSingleton().num_renderables_increment * 0.1f);
	    }
	    return mQueue.pop( );
    }
    //-----------------------------------------------------------------------
    void PagingLandScapeRenderableManager::freeRenderable( PagingLandScapeRenderable *rend )
    {
	    mQueue.push( rend );
    }
    //-----------------------------------------------------------------------
    void PagingLandScapeRenderableManager::queueRenderableLoading( PagingLandScapeTile *tile )
    {
        mTilesLoadQueue.push( tile );
    }
    //-----------------------------------------------------------------------
    void PagingLandScapeRenderableManager::unqueueRenderable ( PagingLandScapeTile *tile )
    {
        mTilesLoadQueue.remove ( tile );
    }
    //-----------------------------------------------------------------------
    bool PagingLandScapeRenderableManager::executeRenderableLoading(void)
    {
        if (mTilesLoadQueue.empty())
            return true;	
        else
        { 
            const uint k = PagingLandScapeOptions::getSingleton ().num_renderables_loading;
            for (uint i = 0; i < k; i++ )
            {
                PagingLandScapeTile* tile = mTilesLoadQueue.pop ();

                // no more in queues.
                if (tile == 0)
                    return true;

                assert (tile->isLoaded ());           
                           
                PagingLandScapeRenderable* rend = tile->getRenderable();

                assert (!rend->isLoaded ());      
                SceneNode * tileSceneNode = tile->getTileNode ();

                assert (rend != 0 && tileSceneNode != 0);

                tileSceneNode->attachObject( rend );
                // renderables need to be attached (BBox compute)
                if (rend->load())
                {
                    tile->_linkRenderableNeighbor ();
                }
                else
                {
                    // renderable have been unloaded since.
                    tile->unload ();
                }
                tileSceneNode->needUpdate();
            }
        }
        return false;
    }

    //-----------------------------------------------------------------------
    uint PagingLandScapeRenderableManager::numRenderables( void ) const
    {
	    return mNumRenderables;
    }

    //-----------------------------------------------------------------------
    int PagingLandScapeRenderableManager::numFree( void ) const
    {
	    return mQueue.getSize( );
    }

    //-----------------------------------------------------------------------
    int PagingLandScapeRenderableManager::numLoading( void ) const
    {
	    return mTilesLoadQueue.getSize( );
    }
    //-----------------------------------------------------------------------
    void PagingLandScapeRenderableManager::_addBatch(const uint num)
    {
	    mNumRenderables += num;
        mRenderables.reserve (mNumRenderables);
	    for (uint i = 0; i < num; i++ )
	    {
		    PagingLandScapeRenderable* rend = new PagingLandScapeRenderable();
		    mRenderables.push_back(rend);
		    mQueue.push( rend );
	    }
    }

} //namespace
