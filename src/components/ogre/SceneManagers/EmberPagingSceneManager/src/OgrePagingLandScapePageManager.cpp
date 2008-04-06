/***************************************************************************
	OgrePagingLandScapePageManager.cpp  -  description
	-------------------
	begin                : Sat May 01 2004
	copyright            : (C) 2003-2006 by Jose A Milan && Tuan Kuranes
	email                : spoke2@supercable.es && tuan.kuranes@free.fr
***************************************************************************/

/***************************************************************************
*                                                                         *
*   This program is free software; you can redistribute it and/or modify  *
*   it under the terms of the GNU General Public License as        *
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


#include "OgrePagingLandScapeSceneManager.h"
#include "OgrePagingLandScapeOptions.h"
#include "OgrePagingLandScapeCamera.h"
#include "OgrePagingLandScapePage.h"
#include "OgrePagingLandScapePageManager.h"
#include "OgrePagingLandScapeTileManager.h"
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
    PagingLandScapePageManager::PagingLandScapePageManager(PagingLandScapeSceneManager * scnMgr) :
        mSceneManager(scnMgr),
        mOptions(scnMgr->getOptions()),
        mData2d(scnMgr->getData2DManager()),
        mTexture(scnMgr->getTextureManager()),
        mRenderablesMgr(scnMgr->getRenderableManager()),
        mWidth(0),
        mHeight(0),
        mNextQueueFrameCount(0),
	    mPause(99),
        mCurrentcam(0),
        mTerrainReady(false),
        mTimePreLoaded(0),
        mRenderQueueGroupID (static_cast<Ogre::RenderQueueGroupID>(scnMgr->getWorldGeometryRenderQueue())),
        //mRenderQueueGroupID (RENDER_QUEUE_WORLD_GEOMETRY_2),
		mPageLoadInterval (mOptions->PageLoadInterval),
		mOnFrame(false),
        mEnabled(false)
    {
		mPagePool.clear();
		mActivePages.clear();
		mFreePages.clear();
    }
    //-----------------------------------------------------------------------
    PagingLandScapePageManager::~PagingLandScapePageManager(void)
    {
		reset();
		// could save a delete if texture type is the same... ?
		if (!mPagePool.empty())
		{
			std::for_each (mPagePool.begin (), mPagePool.end (), 
							delete_object ());
			mPagePool.clear();
			mFreePages.clear();
		}
    }
    //----------------------------------------------------------------------- 
    void PagingLandScapePageManager::reset(void)
    {	
		std::for_each(mActivePages.begin(), mActivePages.end(), 
			std::mem_fun(&PagingLandScapePage::uninit));

		// Insert actives into free list
		mFreePages.insert(mFreePages.end(), mActivePages.begin(), mActivePages.end());
		// Remove all active instances
		mActivePages.clear(); 

		mPageLoadQueue.clear();
		mPagePreloadQueue.clear();
		mPageTextureloadQueue.clear();

		mLoadedPages.clear();
		mTextureLoadedPages.clear();
		mPreLoadedPages.clear();

        mWidth = 0;
        mHeight = 0;
		mOnFrame = false;
        mEnabled = false;
    }
    //-----------------------------------------------------------------------
    void PagingLandScapePageManager::load(void)
    {
        WorldDimensionChange ();
        mEnabled = true;
    }
    //-----------------------------------------------------------------------
    void PagingLandScapePageManager::clear(void)
    {
        // before calling the scene manager node clearing
		reset();
    }
    //-----------------------------------------------------------------------
    void PagingLandScapePageManager::WorldDimensionChange(void)
    {
        const unsigned int newWidth = mOptions->world_width; 
        const unsigned int newHeight = mOptions->world_height;

		reset();

		mPageLoadInterval = mOptions->PageLoadInterval;
        mWidth = newWidth; 
        mHeight = newHeight;
    }
    //-----------------------------------------------------------------------
    void PagingLandScapePageManager::setMapMaterial(void)
    {
        std::for_each(mActivePages.begin(), mActivePages.end(), 
					std::mem_fun(&PagingLandScapePage::setMapMaterial));
    }
    //-----------------------------------------------------------------------
    void PagingLandScapePageManager::_updateLod(void)
    {
        PagingLandScapePageList::iterator lend = mLoadedPages.end();
        for (PagingLandScapePageList::iterator l = mLoadedPages.begin(); l != lend; ++l)
        { 
            (*l)->_updateLod();
        }
    }
    //-----------------------------------------------------------------------
    bool PagingLandScapePageManager::frameStarted(const FrameEvent& evt)
    {
        if (mEnabled)//if not queued to be removed from framelistener or Paused
        {
            --mTimePreLoaded;
            if (mOptions->VisMap)
            {
                mSceneManager->getHorizon ()->prepare(static_cast< PagingLandScapeCamera* >(mOptions->primaryCamera));
            }
			mOnFrame = false;
		}
        return true;
    }
    //-----------------------------------------------------------------------
    bool PagingLandScapePageManager::frameEnded(const FrameEvent& evt)
    {
        // mOnFrame If This Frame has seen any Camera,
        // We won't unload anything.
        // since un-focusing rendering may make this method unload all renderables.
        // mEnabled sm paused or frame listener queued for deletion
        if (!mOnFrame || !mEnabled)
            return true;

		// unload some pages if no more in use
		processUnloadQueues();
		// load some pages that are still queued
		processLoadQueues();

        if (!mTerrainReady && 
            mPagePreloadQueue.empty() && 
            mPageLoadQueue.empty() && 
            mPageTextureloadQueue.empty())
        {
            mSceneManager->getListenerManager()->fireTerrainReady();// no more to load
			mTerrainReady = true;
        }

        if (mOptions->VisMap)
            mSceneManager->getHorizon()->update();

		mSceneManager->getTileManager()->unloadUntouched();
		mRenderablesMgr->resetVisibles();
        return true;
    }
	//-----------------------------------------------------------------------
	PagingLandScapePage *PagingLandScapePageManager::getNewPage(const unsigned int x, const unsigned int z)
	{
		PagingLandScapePage *p;

		// should we resize page pool
		if (mFreePages.empty())
		{
			const size_t pool_size = mPagePool.size ();			
			const size_t new_pool_size = (pool_size == 0) ? 9 : pool_size * 2;

			mPagePool.reserve(new_pool_size);
			mPagePool.resize(new_pool_size);

			// Create new pages
			for (size_t i = pool_size; i < new_pool_size; ++i)
			{
				p = new PagingLandScapePage (this);
				mPagePool[i] = p;
				mFreePages.push_back (p);
			}
		}

		// Get a pre-allocated new page.
		p = mFreePages.front ();
		mFreePages.pop_front ();
		mActivePages.push_back (p); 

		p->init (x, z);

		return p;
	}
	//-----------------------------------------------------------------------
	void PagingLandScapePageManager::releasePage(PagingLandScapePage *p)
	{
		removeFromQueues (p);
		p->uninit ();
		mActivePages.remove (p);
		mFreePages.push_back (p);
	}
	//-----------------------------------------------------------------------
	PagingLandScapePage *PagingLandScapePageManager::getPage(const unsigned int x, const unsigned int z,
															 const bool alwaysReturn)
	{
		if (x < mWidth && z < mHeight)
		{
			if (!mActivePages.empty())
			{
				PagingLandScapePageList::iterator l, lend = mActivePages.end();
				for (l = mActivePages.begin(); l != lend; ++l)
				{
					if ((*l)->isCoord(x, z))
						return (*l);
				}
			}
			if (alwaysReturn)
				return getNewPage(x, z);
		}
		assert (!alwaysReturn);
		return 0;
	}
    //-----------------------------------------------------------------------
    void PagingLandScapePageManager::LoadFirstPage(PagingLandScapeCamera* cam)
    {           

		const Vector3 CamPos =  cam->getDerivedPosition();
		//gets page indices (if outside Terrain gets nearest page)
		unsigned int i, j;
	    getPageIndices  (CamPos.x, CamPos.z, i, j, true);        
		// update the camera page position
        // does modify mIniX, mFinX, mIniZ, mFinZ

		PagingLandScapePage *p = getPage (i, j, false);
		if (p) {
			makePageLoadedNow (p);
		}
	}
	//-----------------------------------------------------------------------
	void PagingLandScapePageManager::makePageLoadedNow(PagingLandScapePage * p)
	{
		// Have the current page be loaded now !
		if (!p->isLoaded())
		{
		    // remove from lists it does belongs to
		    if (p->isTextureLoaded())
			    mTextureLoadedPages.remove (p);
		    else if (p->isPreLoaded())
			    mPreLoadedPages.remove (p);
		    // remove from queue it does belongs to
			removeFromQueues (p);      
			
			p->load();
			assert ( std::find(mLoadedPages.begin(), mLoadedPages.end(), p) == mLoadedPages.end());
			mLoadedPages.push_back (p);
			// make sure this brutal loading doesn't impact on fps
			mNextQueueFrameCount = mPageLoadInterval;   
		}
		else
		{
			p->touch ();   
		}
	}

	//-----------------------------------------------------------------------
	void PagingLandScapePageManager::updateLoadedPages()
	{
		PagingLandScapeCamera * const cam = mCurrentcam;

        // Make any pending updates to the calculated frustum
        cam->updateView();

		const Vector3 pos (cam->getDerivedPosition().x, 127.0f,cam->getDerivedPosition().z);
		// hide page not visible by this Camera
		// Notify those Page (update tile vis/ rend load on cam distance)
		// update Page Texture if needed

		const unsigned int iniX = cam->mIniX;
		const unsigned int finX = cam->mFinX;

		const unsigned int iniZ = cam->mIniZ;
		const unsigned int finZ = cam->mFinZ;

		PagingLandScapePage *p;
		const bool lightchange = mOptions->lightmoved;
		PagingLandScapePageList::iterator l, lend = mLoadedPages.end ();
		for (l = mLoadedPages.begin (); l != lend; ++l)
		{
			p = (*l);
			unsigned int x, z;
			p->getCoordinates (x, z);
			if ((z >= iniZ) && (z <= finZ) && (x >= iniX) && (x <= finX))
			{
				// inform pages we are near Camera on next render.
				if (p->_Notify (pos, cam))
				{
					// get pages that needs modification and Are visible..                 
					PagingLandScapeTexture * const tex = mTexture->getTexture(x, z);
					assert(tex);
					if (lightchange)
						tex->lightUpdate();
					if (tex->needUpdate())
						tex->update();
				}
			}
			else
			{
				p->_Show (false);
			}
		} 



		if (lightchange)
			mOptions->lightmoved = false;
	}
	//-----------------------------------------------------------------------
	void PagingLandScapePageManager::loadNow(PagingLandScapeCamera *cam)
	{
	    updatePaging (cam);
	    const Vector3 pos (cam->getDerivedPosition().x, 127.0f, cam->getDerivedPosition().z);
	    while (!mTerrainReady || 
                !mPagePreloadQueue.empty() || 
                !mPageLoadQueue.empty() || 
                !mPageTextureloadQueue.empty())
        {
		    processLoadQueues();// fill pages queues
		    updateLoadedPages();// fill tiles queues
			mTerrainReady = mRenderablesMgr->executeRenderableLoading(pos); // load renderables
			mNextQueueFrameCount = -1;
        }
        //assert ()
	}
	//-----------------------------------------------------------------------
	void PagingLandScapePageManager::queuePageNeighbors()
	{
		const PagingLandScapeCamera * const cam = mCurrentcam;
		// Queue the rest				
		// Loading  must be done one by one to avoid FPS drop, so they are queued.
		// We must load the next visible LandScape pages, 
		// check the LandScape boundaries	

		const unsigned int preIniX = cam->mPreIniX;
		const unsigned int preFinX = cam->mPreFinX;

		const unsigned int preIniZ = cam->mPreIniZ;
		const unsigned int preFinZ = cam->mPreFinZ;

		const unsigned int iniX = cam->mIniX;
		const unsigned int finX = cam->mFinX;

		const unsigned int iniZ = cam->mIniZ;
		const unsigned int finZ = cam->mFinZ;

		PagingLandScapePage *p;
		for (unsigned int i = preIniX; i <= preFinX; i++)
		{
			for (unsigned int j = preIniZ; j <= preFinZ; j++)
			{
				// pages here, in this zone around camera,
				// must be at least preloading. 
				// that means they can be loaded too.
				p = getPage (i, j, true);

				if (!(p->mIsLoading || p->isLoaded()))
				{                        
					if((j >= iniZ) && (j <= finZ) && (i >= iniX) && (i <= finX))
					{
						// pages here, in this tighter zone around camera,
						// must be Loading or Loaded as they may 
						// be below camera very soon.
						removeFromQueues (p);
						mPageLoadQueue.push (p);                            
						p->mIsLoading = true;
					}
					else if (!p->mIsTextureLoading && 
					         !p->isTextureLoaded() && 
					         !p->mIsPreLoading &&
					         !p->isPreLoaded())
					{
					    //  must be at least preloading. 
						removeFromQueues (p);
						mPagePreloadQueue.push (p);                    
						p->mIsPreLoading = true;	
					}
				}
				p->touch ();
			}
		}
		mTimePreLoaded = mPageLoadInterval;
	}
    //-----------------------------------------------------------------------
    void PagingLandScapePageManager::updatePaging(PagingLandScapeCamera *cam)
    {
        mCurrentcam = cam;
	    // Here we have to look if we have to load, unload any of the LandScape Pages
	    // Fix from Praetor, so the camera used gives you "world-relative" coordinates
        // make sure in the bounding box of LandScape	
	    const Vector3 pos (cam->getDerivedPosition().x, 
	                        127.0f, 
	                        cam->getDerivedPosition().z);

	    //updateStats(pos);

		bool need_touch = false;//(mTimePreLoaded < 0);
		
		if (mWidth == 0 && mHeight == 0) {
			//just return if we haven't got any world yet
			return;
		}
	
	    if (cam->mLastCameraPos != pos
			&& (mOptions->cameraThreshold < fabs (cam->mLastCameraPos.x - pos.x) ||
				mOptions->cameraThreshold < fabs (cam->mLastCameraPos.z - pos.z)))
		{	
			// Update only if the camera was moved
			PagingLandScapePage * const oldPage = getPage (cam->mCurrentCameraPageX, cam->mCurrentCameraPageZ, false);
			PagingLandScapeTile * const oldTile = (oldPage && oldPage->isLoaded() ?
													oldPage->getTile (cam->mCurrentCameraTileX, cam->mCurrentCameraTileZ) : 0);

			unsigned int i, j;

			//gets page indices (if outside Terrain gets nearest page)
  			getPageIndices (pos.x, pos.z, i, j, true); 
			PagingLandScapePage *p = getPage (i, j);
			
			if (!p) {
				return;
			}
			makePageLoadedNow (p);

			// update current Cam Page info 
			if (oldPage != p)
			{
				// update the camera info :
				cam->updatePaging(i, j); 
				// need to inform neighbors pages 
				need_touch = true;
			}

			// Update current Cam Tile info.
			if (p->isLoadable())
			{	
				PagingLandScapeTile * const t = getTile (pos.x, pos.z, i, j, true);
				if (t && t != oldTile)
				{
					if (mOptions->MaxLodUnderCam)
					{
						// reset previous tile at normal LOD mechanism.
						if (oldTile && oldTile->isLoaded())
						{
							assert (oldTile->getRenderable());
							oldTile->getRenderable()->setMaxLod (false);
						}    
						// set current tile at max LOD whatever complexity it is.
						if (t->isLoaded())
						{
							assert (t->getRenderable());
							t->getRenderable()->setMaxLod (true);
						}   
					}

					PagingLandScapeTileInfo * const CurrentTileInfo  = t->getInfo();
					cam->mCurrentCameraTileX = CurrentTileInfo->mTileX;
					cam->mCurrentCameraTileZ = CurrentTileInfo->mTileZ;					
				}
			}
			// Update the last camera position
			if (mOptions->cameraThreshold < fabs (cam->mLastCameraPos.x - pos.x))
				cam->mLastCameraPos.x = pos.x;   
			if (mOptions->cameraThreshold < fabs (cam->mLastCameraPos.z - pos.z))
				cam->mLastCameraPos.z = pos.z;   
        }
		
        if (need_touch) 
			queuePageNeighbors();
        updateLoadedPages();
		//if (mNextQueueFrameCount < 0)
			mRenderablesMgr->executeRenderableLoading(pos);

        // This Frame has seen a Camera.
        mOnFrame = true;
	}
	//-----------------------------------------------------------------------
	void PagingLandScapePageManager::processUnloadQueues()
	{
		// Check for pages that need to be unloaded.
		// if touched, that means they didn't have been touch by any cameras
		// for several frames and thus need to be unloaded.
		PagingLandScapePage *p;
		// LIST CHECKS
		PagingLandScapePageList::iterator itl;
		for (itl = mPreLoadedPages.begin (); itl != mPreLoadedPages.end ();)
		{
			if ((*itl)->unloadUntouched ())
			{
				p = *itl;
				releasePage (p);
				itl = mPreLoadedPages.erase (itl); 
			}
			else
			{
				++itl;
			}
		}
		for (itl = mTextureLoadedPages.begin (); itl != mTextureLoadedPages.end ();)
		{
			if ((*itl)->unloadUntouched ())
			{
				p = *itl;
				releasePage (p);
				itl = mTextureLoadedPages.erase (itl); 
			}
			else
			{
				++itl;
			}
		}
		for (itl = mLoadedPages.begin (); itl != mLoadedPages.end ();)
		{             
			if ((*itl)->unloadUntouched ())
			{
				p = *itl;
				releasePage (p);
				itl = mLoadedPages.erase (itl); 
			}
			else
			{
				++itl;
			}  
		}

		// QUEUES CHECKS
		// check queues for page that need to be excluded from queues
		PagingLandScapeQueue<PagingLandScapePage>::MsgQueType::iterator itq;
		for (itq = mPagePreloadQueue.begin (); itq != mPagePreloadQueue.end ();)
		{
			assert (!(*itq)->isLoaded() && !(*itq)->isPreLoaded() && !(*itq)->isTextureLoaded());
		    assert (!(*itq)->mIsLoading && !(*itq)->mIsTextureLoading);
			if ((*itq)->unloadUntouched ())
			{
				p = *itq;
				// remove from queue
				p->mIsPreLoading = false;
				itq = mPagePreloadQueue.erase (itq);
				// remove from active pages 
				//(must be removed from queue first)
				releasePage (p);  
			}
			else
			{
				++itq;
			}
		}
		for (itq = mPageTextureloadQueue.begin(); itq != mPageTextureloadQueue.end();)
		{
			assert (!(*itq)->isLoaded() && (*itq)->isPreLoaded() && !(*itq)->isTextureLoaded());
		    assert (!(*itq)->mIsLoading && (*itq)->mIsTextureLoading && !(*itq)->mIsPreLoading);
			if ((*itq)->unloadUntouched ())
			{
				p = *itq;
				// remove from queue
				p->mIsTextureLoading = false;
				itq = mPageTextureloadQueue.erase (itq); 
				// remove from active pages 
				//(must be removed from queue first)
				releasePage (p);  
			}
			else
			{
				++itq;
			}
		}
		for (itq = mPageLoadQueue.begin (); itq != mPageLoadQueue.end ();)
		{
			assert (!(*itq)->isLoaded());
		    assert ((*itq)->mIsLoading && !(*itq)->mIsTextureLoading && !(*itq)->mIsPreLoading);
			if ((*itq)->unloadUntouched ())
			{
				p = *itq;
				// remove from queue
				p->mIsLoading = false; 
				itq = mPageLoadQueue.erase (itq); 
				// remove from active pages 
				//(must be removed from queue first)				 
				releasePage (p);  
			}
			else
			{
				++itq;
			}
		}
	}
	//-----------------------------------------------------------------------
	void PagingLandScapePageManager::processLoadQueues()
	{
		// Should be called every count frame only 
		// to minimize fps impact
		if (mNextQueueFrameCount-- < 0)
		{
			SceneManager::CameraIterator camIt = mSceneManager->getCameraIterator();
			while (camIt.hasMoreElements())
			{
				Camera const * const currentCamera = camIt.getNext();
				const Vector3 pos (currentCamera->getDerivedPosition().x, 
									127.0f, 
									currentCamera->getDerivedPosition().z);

				//	We to Load nearest page in non-empty queue
				if (!mPageLoadQueue.empty ())
				{
					//	We to Load nearest page in non-empty queue
					PagingLandScapePage *p = mPageLoadQueue.find_nearest (pos);
					assert (p && !p->isLoaded ());
					assert (!p->mIsTextureLoading && !p->mIsPreLoading);
					p->load ();

					p->mIsLoading = false;   
					mLoadedPages.push_back (p);
					mTextureLoadedPages.remove (p);
					mNextQueueFrameCount = mPageLoadInterval;
				}
				else if (!mPageTextureloadQueue.empty ())
				{
					//	We TextureLoad nearest page in non-empty queue
					PagingLandScapePage *p = mPageTextureloadQueue.find_nearest (pos);
					assert (p && !p->isTextureLoaded());
					assert (!p->mIsLoading && !p->mIsPreLoading);
					p->loadTexture ();

					p->mIsTextureLoading = false;   
					mTextureLoadedPages.push_back (p);
					mPreLoadedPages.remove (p);
					// do not automatically push to level up.
					//mPageLoadQueue.push (p);
					mNextQueueFrameCount = mPageLoadInterval;
				}
				else if (!mPagePreloadQueue.empty ())
				{
					//	We PreLoad nearest page in non-empty queue
					PagingLandScapePage *p = mPagePreloadQueue.find_nearest (pos);
					assert (p && !p->isPreLoaded());
					assert (!p->mIsLoading && !p->mIsTextureLoading);
					p->preload ();

					p->mIsPreLoading = false;
					mPreLoadedPages.push_back (p);
					mPageTextureloadQueue.push (p);
					p->mIsTextureLoading = true;
					mNextQueueFrameCount = mPageLoadInterval;
				}
			}
		} // if (mNextQueueFrameCount-- < 0)
	}
    //-----------------------------------------------------------------------
    void PagingLandScapePageManager::removeFromQueues(PagingLandScapePage* p)
    {
		assert (p);
        if (p->mIsLoading)
        {
            p->mIsLoading = false;
            mPageLoadQueue.remove (p);	
        }
        else if (p->mIsTextureLoading)
        {
            p->mIsTextureLoading = false;
			mPageTextureloadQueue.remove (p);
        }
        else if (p->mIsPreLoading)
        {
            p->mIsPreLoading = false;
			mPagePreloadQueue.remove (p);
        }
		assert (!p->mIsLoading && !p->mIsTextureLoading && !p->mIsPreLoading);
    }
    //-----------------------------------------------------------------------
    unsigned int PagingLandScapePageManager::getCurrentCameraPageX(void) const
    {
        if (mCurrentcam)
		{

            return mCurrentcam->mCurrentCameraPageX;
		}
        return 0;
    }

    //-----------------------------------------------------------------------
    unsigned int PagingLandScapePageManager::getCurrentCameraPageZ(void) const
    {
        if (mCurrentcam)
		{
	        return mCurrentcam->mCurrentCameraPageZ;
		}
        return 0;
    }

    //-----------------------------------------------------------------------
    unsigned int PagingLandScapePageManager::getCurrentCameraTileX(void) const
    {
        if (mCurrentcam)
		{
            return mCurrentcam->mCurrentCameraTileX;
		}
        return 0;
    }

    //-----------------------------------------------------------------------
    unsigned int PagingLandScapePageManager::getCurrentCameraTileZ(void) const
    {
        if (mCurrentcam)
		{
			return mCurrentcam->mCurrentCameraTileZ;
		}
       return 0;
    }
    //-----------------------------------------------------------------------
    void PagingLandScapePageManager::addLoadedPage(PagingLandScapePage *p)
    {
        mLoadedPages.push_back (p);
    }
    //-----------------------------------------------------------------------
    int PagingLandScapePageManager::getLoadedPageSize(void) const
    {
	    return static_cast< int >(mLoadedPages.size());
    }
    //-----------------------------------------------------------------------
    int PagingLandScapePageManager::getUnloadedPagesSize(void) const
    {
	    return static_cast< int >(mWidth*mHeight - mLoadedPages.size());
    }
    //-----------------------------------------------------------------------
    int PagingLandScapePageManager::getTextureLoadedPageSize(void) const
    {
	    return static_cast< int >(mTextureLoadedPages.size());
    }

    //-----------------------------------------------------------------------
    int PagingLandScapePageManager::getPreLoadedPageSize(void) const
    {
	    return static_cast< int >(mPreLoadedPages.size());
    }
    //-----------------------------------------------------------------------
    int PagingLandScapePageManager::getPagePreloadQueueSize(void) const
    {
	    return static_cast< int >(mPagePreloadQueue.getSize());
    }

    //-----------------------------------------------------------------------
    int PagingLandScapePageManager::getPageTextureloadQueueSize(void) const
    {
	    return static_cast< int >(mPageTextureloadQueue.getSize());
    }

    //-----------------------------------------------------------------------
    int PagingLandScapePageManager::getPageLoadQueueSize(void) const
    {
	    return static_cast< int >(mPageLoadQueue.getSize());
    }

    //-----------------------------------------------------------------------
    void PagingLandScapePageManager::getGlobalToPage(Real& x, Real& z) const
    {
        const Real inv_pSize = 1.0f / (mOptions->PageSize - 1);
      
	    x = static_cast< int >(((x / mOptions->scale.x) + mOptions->maxUnScaledX) * inv_pSize);
	    z = static_cast< int >(((z / mOptions->scale.z) + mOptions->maxUnScaledZ) * inv_pSize);
    }

    //-----------------------------------------------------------------------
    void PagingLandScapePageManager::getNearestPageIndicesUnscaled(const Real posx, const Real posz, unsigned int& x, unsigned int& z) const
    {
        // adjust x and z to be local to page
        const Real inv_pSize = 1.0f / (mOptions->PageSize - 1);
      
	    const int lx = static_cast< int >((posx + mOptions->maxUnScaledX) * inv_pSize);
	    const int lz = static_cast< int >((posz + mOptions->maxUnScaledZ) * inv_pSize);

        const int w = static_cast< int >(mOptions->world_width);
        const int h = static_cast< int >(mOptions->world_height);

	    // make sure indices are not negative or outside range of number of pages
	    if (lx >= w)
	    {
		    x = static_cast< unsigned int >(w - 1);
	    }
        else if (lx < 0)
        {
            x = 0;
        }
        else
		{
            x = static_cast< unsigned int >(lx);
		}

	    if (lz >= h) 
	    {
		    z = static_cast< unsigned int >(h - 1);
	    }
        else if (lz < 0)
        {
            z = 0;
        }
        else
		{
            z = static_cast< unsigned int >(lz);
		}
    }

  
    
    //-----------------------------------------------------------------------
    void PagingLandScapePageManager::getNearestTileIndicesUnscaled(const Real posx, const Real posz, 
                                                                    const unsigned int pagex, const unsigned int pagez, 
                                                                    unsigned int& x, unsigned int& z) const
    {
        // adjust x and z to be local to page
        const Real inv_tSize = 1.0f / (mOptions->TileSize - 1);
        const int pSize = mOptions->PageSize - 1;

        const int tilex = static_cast< int >((posx - ((pagex * pSize) - mOptions->maxUnScaledX)) * inv_tSize); 
        //- mOptions->maxUnScaledX

        const int tilez = static_cast< int >((posz - ((pagez * pSize) - mOptions->maxUnScaledZ)) * inv_tSize);
        //- mOptions->maxUnScaledZ

        const int tilesPerPage = static_cast< int >((pSize * inv_tSize) - 1);

        if (tilex > tilesPerPage)
		{
            x = static_cast< unsigned int >(tilesPerPage);
		}
        else if (tilex < 0)
		{
            x = 0;
		}
        else
		{
            x = static_cast< unsigned int >(tilex);
		}

        if (tilez > tilesPerPage)
		{
            z = static_cast< unsigned int >(tilesPerPage);
		}
        else if(tilez < 0)
		{
            z = 0;
		}
        else
		{
            z = static_cast< unsigned int >(tilez);
		}
    }
    //-----------------------------------------------------------------------
    bool PagingLandScapePageManager::getTileIndices(const Real posx, const Real posz, 
                                                    const unsigned int pagex, const unsigned int pagez, 
                                                    unsigned int& x, unsigned int& z, bool alwaysAnswer) const
    {
        if (alwaysAnswer)
        {
            getNearestTileIndicesUnscaled(posx / mOptions->scale.x, posz / mOptions->scale.z, pagex, pagez, x, z);
            return true;
        }
        else
        {
            return getRealTileIndicesUnscaled(posx / mOptions->scale.x, posz / mOptions->scale.z, pagex, pagez, x, z);
        }
    }
    //-----------------------------------------------------------------------
    PagingLandScapeTile* PagingLandScapePageManager::getTileUnscaled(const Real posx, const Real posz, const unsigned int pagex, const unsigned int pagez, bool alwaysAnswer)
    {
        unsigned int tilex, tilez;
        if (alwaysAnswer)
        {
            getNearestTileIndicesUnscaled(posx, posz, pagex, pagez, tilex, tilez);
			PagingLandScapePage * const p = getPage (pagex , pagez, false);
			if (p)
				return p->getTile(tilex, tilez);
        }
        else
        {
            if (getRealTileIndicesUnscaled(posx, posz, pagex, pagez, tilex, tilez))
			{
				PagingLandScapePage * const p = getPage (pagex , pagez, false);
				if (p)
					return p->getTile(tilex, tilez);
			}
		}
		return 0;
    }
    //-----------------------------------------------------------------------
    PagingLandScapeTile* PagingLandScapePageManager::getTile(const Real posx, const Real posz, const unsigned int pagex, const unsigned int pagez, bool alwaysAnswer)
    {
       return getTileUnscaled(posx / mOptions->scale.x, posz / mOptions->scale.z, pagex, pagez, alwaysAnswer);
    }
    //-----------------------------------------------------------------------
    PagingLandScapeTile* PagingLandScapePageManager::getTile(const Real posx, const Real posz, bool alwaysAnswer)
    {
        return getTileUnscaled(posx / mOptions->scale.x, posz / mOptions->scale.z, alwaysAnswer);
    }
    //-----------------------------------------------------------------------
    PagingLandScapeTile* PagingLandScapePageManager::getTileUnscaled(const Real posx, const Real posz, bool alwaysAnswer)
    {
        unsigned int pagex, pagez;
        if (alwaysAnswer)
        {
            unsigned int tilex, tilez;
            getNearestPageIndicesUnscaled(posx, posz, pagex, pagez);
            getNearestTileIndicesUnscaled(posx, posz, pagex, pagez, tilex, tilez);
			return getPage (pagex , pagez)->getTile(tilex, tilez);
        }
        else
        {
            if (getRealPageIndicesUnscaled(posx, posz, pagex, pagez))
            {
                unsigned int tilex, tilez;
                if (getRealTileIndicesUnscaled(posx, posz, pagex, pagez, tilex, tilez))
				{
					PagingLandScapePage * const p = getPage (pagex , pagez, false);
					if (p)
						return p->getTile(tilex, tilez);
				}
            }        
		}      
		return 0;
    }
    //-------------------------------------------------------------------------
    PagingLandScapeTile* PagingLandScapePageManager::getTilePage (unsigned int &posx, unsigned int &posz, 
                                                                  const unsigned int pagex, const unsigned int pagez)
    {        
        const Real tSize = mOptions->TileSize - 1;
        const Real inv_tSize = 1.0f / tSize;
        const int tilex = static_cast< int >(posx * inv_tSize); 
        const int tilez = static_cast< int >(posz * inv_tSize);

        const int pSize = mOptions->PageSize - 1;
        const int tilesPerPage = static_cast< int > (mOptions->NumTiles - 1);

        unsigned int x;
        if (tilex > tilesPerPage)
		{
            x = static_cast< unsigned int >(tilesPerPage);
		}
        else if (tilex < 0)
		{
            x = 0;
		}
        else
		{
            x = static_cast< unsigned int >(tilex);
		}

        unsigned int z;
        if (tilez > tilesPerPage)
		{
            z = static_cast< unsigned int >(tilesPerPage);
		}
        else if(tilez < 0)
		{
            z = 0;
		}
        else
		{
            z = static_cast< unsigned int >(tilez);
		}
        posx = posx - static_cast< unsigned int > (x * tSize);
		posz = posz - static_cast< unsigned int > (z * tSize);
		PagingLandScapePage *p = getPage (pagex , pagez);
		if (p)
			return p->getTile(x, z);
		return 0;
    }
    //-------------------------------------------------------------------------
	void PagingLandScapePageManager::setWorldGeometryRenderQueue(uint8 qid)
    {
        PagingLandScapePageList::iterator l, lend = mLoadedPages.end();
        for (l = mLoadedPages.begin(); l != lend; ++l)
        {
            PagingLandScapePage *p = (*l);
            {
                p->setRenderQueue(qid);
            }
        }
	}

}
