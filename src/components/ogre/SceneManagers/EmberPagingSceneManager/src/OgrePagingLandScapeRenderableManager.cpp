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
	    assert(ms_Singleton);  return (*ms_Singleton);  
    }
    //-----------------------------------------------------------------------
    PagingLandScapeRenderableManager::PagingLandScapeRenderableManager() :
        mNumRenderables (0),
		mRenderableLoadInterval (0),
		mLoadInterval(0),
        mNumRenderableLoading(0)
    {
        PagingLandScapeRenderable::mOpt = PagingLandScapeOptions::getSingletonPtr();
    }
    //-----------------------------------------------------------------------
    PagingLandScapeRenderableManager::~PagingLandScapeRenderableManager()
    {
        
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
        // If Renderables change over maps (+- lit, normals, etc...)
        std::for_each (mRenderables.begin (), 
                       mRenderables.end (),  
                       delete_object());
        mRenderables.clear();
        mQueue.clear();
        mNumRenderables = 0;
    }
    //-----------------------------------------------------------------------
    void PagingLandScapeRenderableManager::load()
    {        
        PagingLandScapeOptions *opt = PagingLandScapeOptions::getSingletonPtr();  

        mNumRenderableLoading = opt->num_renderables_loading;
        mNumRenderablesIncrement = opt->num_renderables_increment;
        mRenderableLoadInterval = opt->RenderableLoadInterval;

        const uint nRend = opt->num_renderables;
        
        if (mNumRenderables < nRend)
        {
            _addBatch (nRend - mNumRenderables);
        }
		mTilesLoadRenderableQueue.clear();
    }
    //-----------------------------------------------------------------------
    PagingLandScapeRenderable *PagingLandScapeRenderableManager::getRenderable()
    {
	    if (mQueue.empty ())
        {
            // unload some Tiles/renderables no more used to free some space.
            processTileUnload ();


            //for (uint i = 0; i < mNumRenderables; ++i)
            //{
            //    assert (mRenderables[i]->isLoaded ());
            //}
            for (uint i = 0; i < mNumRenderables; ++i)
            {
                //assert (mRenderables[i]->isInUse ());
                if (!mRenderables[i]->isInUse () && !mRenderables[i]->isLoaded ())
                    mQueue.push(mRenderables[i]);
            }
            


            if (mQueue.empty ())
            {
		        // We do not have any more free Renderables
                // we need to allocate more
		        _addBatch (mNumRenderablesIncrement);
		        // Increment the next batch by a 10%
		        mNumRenderablesIncrement += static_cast<uint> (mNumRenderablesIncrement * 0.1f);
            }
	    }
	    return mQueue.pop ();
    }
    //-----------------------------------------------------------------------
    void PagingLandScapeRenderableManager::freeRenderable(PagingLandScapeRenderable *rend)
    {
	    mQueue.push (rend);
    }
    //-----------------------------------------------------------------------
    void PagingLandScapeRenderableManager::queueRenderableLoading(PagingLandScapeTile *tile)
	{
		assert (tile);  
		assert (tile->isLoaded ());  
		assert (tile->getRenderable ()); 
        assert (!tile->getRenderable ()->isLoaded ());   
        assert (tile->getRenderable ()->isInUse ());  
        mTilesLoadRenderableQueue.push (tile);
    }
    //-----------------------------------------------------------------------
    void PagingLandScapeRenderableManager::unqueueRenderable (PagingLandScapeTile *tile)
    {
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
			assert (tile->isLoaded ());       
			assert (tile->getRenderable () != 0 && tile->getSceneNode() != 0);
			assert (!tile->getRenderable ()->isLoaded ()); 

			if (!tile->getRenderable ()->isInUse ())
			{
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
            return true;	
        else
        { 
			if (mLoadInterval-- < 0)
			{
				const uint queueSize = mTilesLoadRenderableQueue.getSize () ;
				mTilesLoadRenderableQueue.sortNearest(Cameraposition);
                const uint k = mNumRenderableLoading > queueSize ? queueSize : mNumRenderableLoading;
				for (uint i = 0; i < k; i++)
				{
					
					PagingLandScapeTile * const tile = mTilesLoadRenderableQueue.pop ();
					// no more in queues.
					assert (tile != 0);
					assert (tile->isLoaded ());     
					PagingLandScapeRenderable * const rend = tile->getRenderable ();

					assert (rend != 0);
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
					tileSceneNode->needUpdate ();
				}
				mLoadInterval = mRenderableLoadInterval;
			}
        }
        return false;
    }

    //-----------------------------------------------------------------------
    uint PagingLandScapeRenderableManager::numRenderables(void) const
    {
	    return mNumRenderables;
    }

    //-----------------------------------------------------------------------
    int PagingLandScapeRenderableManager::numFree(void) const
    {
	    return mQueue.getSize();
    }

    //-----------------------------------------------------------------------
    int PagingLandScapeRenderableManager::numLoading(void) const
    {
	    return mTilesLoadRenderableQueue.getSize();
    }
    //-----------------------------------------------------------------------
    void PagingLandScapeRenderableManager::_addBatch(const uint num)
    {
	    mNumRenderables += num;
        mRenderables.reserve (mNumRenderables);
	    for (uint i = 0; i < num; i++)
	    {
		    PagingLandScapeRenderable* rend = new PagingLandScapeRenderable();
		    mRenderables.push_back(rend);
		    mQueue.push(rend);
	    }
        #ifdef _DEBUG
            std::cout << "Renderables addBatch : " << mRenderables.size() << "\n";
        #endif _DEBUG
    }
} //namespace
