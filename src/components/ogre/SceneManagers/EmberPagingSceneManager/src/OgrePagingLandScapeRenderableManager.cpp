/***************************************************************************
  OgrePagingLandScapeRenderableManager.cpp  -  description
  -------------------
  begin                : Mon Jun 16 2003
  copyright            : (C) 2003-2006 by Jose A Milan && Tuan Kuranes
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

#include "OgrePagingLandScapePrecompiledHeaders.h"

#include "OgreVector3.h"
#include "OgreColourValue.h"

#include "OgreMovableObject.h"

#include "OgreCamera.h"

#include "OgreSceneNode.h"

#include "OgreSimpleRenderable.h"

#include "OgrePagingLandScapeSceneManager.h"
#include "OgrePagingLandScapeOptions.h"
#include "OgrePagingLandScapeCamera.h"

#include "OgrePagingLandScapeRenderable.h"
#include "OgrePagingLandScapeIndexBuffer.h"

#include "OgrePagingLandScapeRenderableManager.h"

#include "OgrePagingLandScapeTile.h"

namespace Ogre
{

	//-----------------------------------------------------------------------
	PagingLandScapeRenderableSet::PagingLandScapeRenderableSet():
		PoolSet<PagingLandScapeRenderable>(),
		mRenderableManager(0)
	{
	}
	//-----------------------------------------------------------------------
	PagingLandScapeRenderable* PagingLandScapeRenderableSet::allocate ()
	{
		return new PagingLandScapeRenderable(mRenderableManager);
	}
	//-----------------------------------------------------------------------
	void PagingLandScapeRenderableSet::deallocate (PagingLandScapeRenderable *r)
	{
		delete r;
	}
    //-----------------------------------------------------------------------
    PagingLandScapeRenderableManager::PagingLandScapeRenderableManager(PagingLandScapeSceneManager * scnMgr):
        mSceneManager(scnMgr),
        mOptions(scnMgr->getOptions ()), 
		mRenderableLoadInterval (0),
		mLoadInterval(0),
        mNumRenderableLoading(0)
    {
		// auto extend ourself as we rather check 
		//if we can found non Freed Renderables before
		mRenderablePool.setAutoextend (false);
		// don't Double renderables num each time... too much.
		mRenderablePool.setExtendFactor (1.2f);
    }
    //-----------------------------------------------------------------------
    PagingLandScapeRenderableManager::~PagingLandScapeRenderableManager()
    {
		mRenderablePool.deletePool ();
    }
    //-----------------------------------------------------------------------
    void PagingLandScapeRenderableManager::clear()
    {
        if (!mTilesLoadRenderableQueue.empty ())
        {
            PagingLandScapeTile *t = mTilesLoadRenderableQueue.pop();
            while (t)
            {
                t->uninit ();
                t = mTilesLoadRenderableQueue.pop ();
            }
            assert (mTilesLoadRenderableQueue.empty ());
        }
        // As Renderables change too much over maps (+- lit, normals, etc...)
        mRenderablePool.deletePool ();
    }
    //-----------------------------------------------------------------------
    void PagingLandScapeRenderableManager::load()
	{        
		mRenderablePool.setRenderableManager (this);

        PagingLandScapeOptions *opt = mOptions;  

        mNumRenderableLoading = opt->num_renderables_loading;
        mNumRenderablesIncrement = opt->num_renderables_increment;
        mRenderableLoadInterval = opt->RenderableLoadInterval;
        mRenderablePool.setPoolSize (opt->num_renderables);
		mTilesLoadRenderableQueue.clear();
    }
    //-----------------------------------------------------------------------
    PagingLandScapeRenderable *PagingLandScapeRenderableManager::getRenderable()
    {
		PagingLandScapeRenderable *r = mRenderablePool.getPoolable ();
	    if (r == 0)
		{
            // unload some Tiles/renderables no more used to free some space.
            processTileUnload ();

			r = mRenderablePool.getPoolable ();
			if (r == 0)
			{
				mRenderablePool.autoExtend();
				r = mRenderablePool.getPoolable();
			}
		}
		return r;
    }
    //-----------------------------------------------------------------------
    void PagingLandScapeRenderableManager::freeRenderable(PagingLandScapeRenderable *rend)
    {
		assert (rend->mParentTile == 0);
		mRenderablePool.removePoolable(rend);
    }
    //-----------------------------------------------------------------------
    void PagingLandScapeRenderableManager::queueRenderableLoading(PagingLandScapeTile *tile)
	{
		assert (tile);  

		assert (!tile->isLoading());
		assert (tile->getRenderable ()); 
		assert (!tile->getRenderable ()->mQueued);

		assert (tile->isLoaded ()); 
        assert (!tile->getRenderable ()->isLoaded ());   
		assert (tile->getRenderable ()->isInUse ());

		//
		tile->setLoading(true);
        tile->getRenderable ()->mQueued = true;

		//
        mTilesLoadRenderableQueue.push (tile);

    }
    //-----------------------------------------------------------------------
    void PagingLandScapeRenderableManager::unqueueRenderable (PagingLandScapeTile *tile)
	{
 		assert (tile->isLoading());
		assert (tile->getRenderable ());
		assert (tile->getRenderable ()->mQueued);

		assert (tile->isLoaded ());
		assert (!tile->getRenderable ()->isLoaded ());
		//assert (!tile->getRenderable ()->isInUse ());

		//
		tile->setLoading (false);
		tile->getRenderable ()->mQueued = false;

		//
        mTilesLoadRenderableQueue.remove (tile);
	}
	//-----------------------------------------------------------------------
	void PagingLandScapeRenderableManager::processTileUnload()
	{
		if (mTilesLoadRenderableQueue.empty())
			return;

		PagingLandScapeTile *tile;
		for (PagingLandScapeQueue<PagingLandScapeTile>::MsgQueType::iterator itq = mTilesLoadRenderableQueue.begin(); 
			itq != mTilesLoadRenderableQueue.end();)
		{
			tile = *itq;
			assert (tile != 0);

			assert (tile->isLoading());
			assert (tile->getRenderable ());
			assert (tile->getRenderable ()->mQueued);

			assert (tile->isLoaded ());       
			assert (tile->getSceneNode());
			assert (!tile->getRenderable ()->isLoaded ()); 

			if (!tile->getRenderable ()->isInUse ())
            {
				tile->setLoading (false);
				tile->getRenderable ()->mQueued = false;
                tile->unload (); 
				itq = mTilesLoadRenderableQueue.erase (itq);
			}
			else
			{
				++itq;
			}
		}
	}
    //-----------------------------------------------------------------------
    bool PagingLandScapeRenderableManager::executeRenderableLoading(const Vector3 &Cameraposition)
    {
        if (mTilesLoadRenderableQueue.empty())
        {
            return true;	
        }
        else
        { 
			if (mLoadInterval-- < 0)
			{
				const size_t queueSize = mTilesLoadRenderableQueue.getSize () ;
				mTilesLoadRenderableQueue.sortNearest(Cameraposition);
                const size_t k = mNumRenderableLoading > queueSize ? queueSize : mNumRenderableLoading;
				for (size_t i = 0; i < k; i++)
				{
					
					PagingLandScapeTile * const tile = mTilesLoadRenderableQueue.pop ();
					// no more in queues.
					assert (tile != 0);
					assert (tile->isLoaded ());
					assert (tile->isLoading());   
					PagingLandScapeRenderable * const rend = tile->getRenderable ();

					assert (rend != 0);
					assert (rend->mParentTile == tile);
					assert (rend->mQueued);  
					assert (!rend->isLoaded ());      
					SceneNode * const tileSceneNode = tile->getSceneNode ();
					assert (tileSceneNode != 0);


					// if renderable can be loaded 
					if (rend->load ())
					{
						tileSceneNode->attachObject (rend);
						tile->_linkRenderableNeighbor ();
					}
					else
					{	
						// (no data yet.) empty tile.
						tile->unload ();
					}

					tile->setLoading(false);
					rend->mQueued = false;

					tileSceneNode->needUpdate ();
				}
				mLoadInterval = mRenderableLoadInterval;
			}
        }
        return false;
    }

    //-----------------------------------------------------------------------
    size_t PagingLandScapeRenderableManager::numRenderables(void) const
    {
	    return static_cast< size_t > (mRenderablePool.getPoolSize());
    }

    //-----------------------------------------------------------------------
    size_t PagingLandScapeRenderableManager::numFree(void) const
    {
	    return mRenderablePool.getPoolSize() - mRenderablePool.getActivePoolablesSize ();
    }

    //-----------------------------------------------------------------------
    size_t PagingLandScapeRenderableManager::numLoading(void) const
    {
	    return mTilesLoadRenderableQueue.getSize();
    }
    //-----------------------------------------------------------------------
    void PagingLandScapeRenderableManager::_addBatch(const unsigned int num)
    {
		mRenderablePool.setPoolSize (mRenderablePool.getPoolSize() + num);
        #ifdef _DEBUG
            std::cout << "Renderables addBatch : " << mRenderablePool.getPoolSize() << "\n";
        #endif
    }
} //namespace
