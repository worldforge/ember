/***************************************************************************
	OgrePagingLandScapePage.cpp  -  description
	-------------------
	begin                : Sat Mar 08 2003
	copyright            : (C) 2003-2006 by Jose A. Milan and Tuan Kuranes
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
    PagingLandScapePage::PagingLandScapePage(PagingLandScapePageManager *pageMgr) :
        mParent (pageMgr),
		mIsLoading (false),
		mIsPreLoading (false),
		mIsTextureLoading (false),
		mIsUnloading (false),
		mIsPostUnloading (false),
		mIsTextureunloading (false),
		mIsLoaded (false),
		mIsTextureLoaded (false),
		mIsPreLoaded (false), 
		mIsLoadable(true),		
		mPageNode (0),
		mRenderable(0)
    {  
		for (unsigned int i = 0; i < 4; i++)
		{
			mNeighbors[i] = 0;
		}
    }
    //-----------------------------------------------------------------------
    PagingLandScapePage::~PagingLandScapePage()
    {
    }
    //-----------------------------------------------------------------------
    void PagingLandScapePage::init (const unsigned int tableX, const unsigned int tableZ)
	{ 
		assert (!mIsLoading);
		assert (!mIsPreLoading);
		assert (!mIsTextureLoading);
		assert (!mIsUnloading);

		assert (!mIsPostUnloading);
		assert (!mIsTextureunloading);
		assert (!mIsLoaded);
		assert (!mIsTextureLoaded);
		assert (!mIsPreLoaded);
		assert (mIsLoadable);



		mTableX = tableX;
		mTableZ = tableZ;
		//create a LandScape node that handles pages and tiles
		const String NodeName =  "PagingLandScapePage." 
			+ StringConverter::toString(mTableX) + "." + 
			StringConverter::toString(mTableZ);
		
        mPageNode = mParent->getSceneManager()->createSceneNode (NodeName + ".Node");     


		const PagingLandScapeOptions * const opt = mParent->getOptions();
        mNumTiles = opt->NumTiles;

	    const unsigned int size = opt->PageSize - 1;
	    // Boundaries of this page
	    // the middle page is at world coordinates 0,0
        const Real factorX = size * opt->scale.x;
        const Real factorZ = size * opt->scale.z;

	    mIniX = static_cast<Real> (static_cast<int> (mTableX + mTableX - opt->world_width)) * 0.5f * factorX + opt->position.x;		
	    mIniZ = static_cast<Real> (static_cast<int> (mTableZ + mTableZ - opt->world_height)) * 0.5f * factorZ + opt->position.z;	

		// Set node position
		mPageNode->setPosition(static_cast<Real> (mIniX) , 
			opt->position.y, 
			static_cast<Real> (mIniZ));

		//translate page scene node to the desired position	
  		mPageNode->translate( opt->position);

	    const Real EndX = mIniX + factorX;
	    const Real EndZ = mIniZ + factorZ;
        const Real MaxHeight = mParent->getSceneManager()->getData2DManager()->getMaxHeight(mTableX, mTableZ);
        const Real chgfactor = opt->change_factor;

	    mBounds.setExtents(mIniX , 
                             0.0f, 
                             mIniZ , 
						     EndX , 
                             MaxHeight, 
                             EndZ);

	    //Change Zone of this page
	    mBoundsInt.setExtents(mIniX + chgfactor, 
                               0.0f, 
                               mIniZ + chgfactor,
						       EndX - chgfactor, 
                               MaxHeight, 
                               EndZ - chgfactor	);

         
        mBoundsExt.setExtents(mIniX - factorX * 1.5f, 
                               - MaxHeight * 1.5f, 
                               mIniZ - factorZ * 1.5f,

                               mIniX + factorX * 1.5f, 
                               MaxHeight * 1.5f , 
                               mIniZ + factorZ * 1.5f);

		mWorldPosition = mBounds.getCenter();



		if (opt->BigImage)
		{
			mRenderable = new PagingLandScapePageRenderable(mParent,
                mPageNode->getName () + "rend", 
				mTableX, mTableZ,
				mBounds);
			mPageNode->attachObject(mRenderable);
			mRenderable->load ();
		}

		PagingLandScapePageManager * const pageMgr = mParent;
		PagingLandScapePage *n;
		n = pageMgr->getPage (tableX, tableZ + 1, false);
		_setNeighbor(SOUTH, n);
		if (n)
			n->_setNeighbor(NORTH, this);

		n = pageMgr->getPage (tableX, tableZ - 1, false);
		_setNeighbor(NORTH, n);
		if (n)
			n->_setNeighbor(SOUTH, this);

		n = pageMgr->getPage (tableX + 1, tableZ, false);
		_setNeighbor(EAST, n);
		if (n)
			n->_setNeighbor(WEST, this);

		n = pageMgr->getPage (tableX - 1, tableZ, false);
		_setNeighbor(WEST, n);
		if (n)
			n->_setNeighbor(EAST, this);

		mVisible = false;

        touch ();
    }
    //-----------------------------------------------------------------------
    void PagingLandScapePage::uninit ()
    {
		postUnload ();  
		assert (mTiles.empty());

		if (mParent->getOptions()->BigImage)
        {
		    mPageNode->detachObject (mRenderable->getName());
            delete mRenderable;
        }	

		assert (mPageNode);
		mPageNode->removeAndDestroyAllChildren ();
        mParent->getSceneManager()->destroySceneNode (mPageNode->getName ());
        mPageNode = 0;
       

		PagingLandScapePage *n = mNeighbors[NORTH];
		if (n)
		{
			n->_setNeighbor(SOUTH, 0);
			mNeighbors[NORTH] = 0;
		}
		n = mNeighbors[SOUTH];
		if (n)
		{
			n->_setNeighbor(NORTH, 0);
			mNeighbors[SOUTH] = 0;
		}
		n = mNeighbors[EAST];
		if (n)
		{
			n->_setNeighbor(WEST, 0);
			mNeighbors[EAST] = 0;
		}
		n = mNeighbors[WEST];
		if (n)
		{
			n->_setNeighbor(EAST, 0);
			mNeighbors[WEST] = 0;
		}	
		// restore init state.
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
    }
	//-----------------------------------------------------------------------
	void PagingLandScapePage::_setNeighbor(const Neighbor& n, PagingLandScapePage* p)
	{
		mNeighbors[ n ] = p;
	 
		const bool thisLoaded = mIsLoaded;
		const bool neighLoaded = p && p->isLoaded() && p->isLoadable();

		if (!thisLoaded && !neighLoaded)
			return;

		assert (!thisLoaded || (thisLoaded && !mTiles.empty()));
		const unsigned int numTiles = mNumTiles;
		switch (n)
		{
		case EAST:
			{
				const unsigned int i = numTiles - 1;
				for (unsigned int j = 0; j < numTiles; j++)
				{	
					PagingLandScapeTile *t_nextpage = 0;
					PagingLandScapeTile *t_currpage = 0;
					if (thisLoaded)
						t_currpage =  mTiles[ i ][ j ];
					if (neighLoaded)
						t_nextpage =  p->getTile  (0 , j);
					if (thisLoaded)
						t_currpage->_setNeighbor(EAST, t_nextpage);
					if (neighLoaded)
						t_nextpage->_setNeighbor(WEST, t_currpage);
				}
				
			}
			break;
		case WEST:
			{
				const unsigned int i = numTiles - 1;
				for (unsigned int j = 0; j < numTiles; j++)
				{	
					PagingLandScapeTile *t_nextpage = 0;
					PagingLandScapeTile *t_currpage = 0;
					if (thisLoaded)
						t_currpage =  mTiles[ 0 ][ j ];
					if (neighLoaded)
						t_nextpage =  p->getTile  (i , j);
					if (thisLoaded)
						t_currpage->_setNeighbor(WEST, t_nextpage);
					if (neighLoaded)
						t_nextpage->_setNeighbor(EAST, t_currpage);
				}
			}
			break;		
		case NORTH:
			{
				const unsigned int j = numTiles - 1;
				for (unsigned int i = 0; i < numTiles; i++)
				{	
					PagingLandScapeTile *t_nextpage = 0;
					PagingLandScapeTile *t_currpage = 0;
					if (thisLoaded)
						t_currpage =  mTiles[ i ][ 0 ];
					if (neighLoaded)
						t_nextpage =  p->getTile  (i , j);
					if (thisLoaded)
						t_currpage->_setNeighbor(NORTH, t_nextpage);
					if (neighLoaded)
						t_nextpage->_setNeighbor(SOUTH, t_currpage);
				}
			}
			break;
		case SOUTH:
			{
				const unsigned int j = numTiles - 1;
				for (unsigned int i = 0; i < numTiles; i++)
				{	
					PagingLandScapeTile *t_nextpage = 0;
					PagingLandScapeTile *t_currpage = 0;
					if (thisLoaded)
						t_currpage =  mTiles[ i ][ j ];
					if (neighLoaded)
						t_nextpage =  p->getTile  (i , 0);
					if (thisLoaded)
						t_currpage->_setNeighbor(SOUTH, t_nextpage);
					if (neighLoaded)
						t_nextpage->_setNeighbor(NORTH, t_currpage);
				}
			}
			break;
		default:
			break;
		}
	}
    //-----------------------------------------------------------------------
    void PagingLandScapePage::setMapMaterial()
    {    
        if (mParent->getOptions()->BigImage)
        {
            mRenderable->setMaterial (mParent->getSceneManager()->getTextureManager()->getMapMaterial());
        }
    }
    //-----------------------------------------------------------------------
    void PagingLandScapePage::touch ()
    { 
        mTimeUntouched = mParent->getOptions()->PageInvisibleUnloadFrames;
    }
    //-----------------------------------------------------------------------
    const bool PagingLandScapePage::unloadUntouched ()        
    { 
        if (mTimeUntouched == 0)
            return true;
        mTimeUntouched--; 
        return false;
    }
    //-----------------------------------------------------------------------
    void PagingLandScapePage::preload()
    {
        touch ();

	    if (mIsPreLoaded)
		    return;

        mIsLoadable = mParent->getSceneManager()->getData2DManager()->load (mTableX, mTableZ);

	    mIsPreLoaded = true;

        mParent->getSceneManager()->getListenerManager()->firePagePreloaded(mTableX, mTableZ, 
            mParent->getSceneManager()->getData2DManager()->getData2D(mTableX, mTableZ)->getHeightData(),
            mBounds);
    }
    //-----------------------------------------------------------------------
    void PagingLandScapePage::loadTexture()
    {        
        touch ();
	    if (!mIsPreLoaded)
		    preload ();
        if (!mIsTextureLoaded) 
        {
            if (mIsLoadable)
                mParent->getSceneManager()->getTextureManager()->load(mTableX, mTableZ);
            mIsTextureLoaded = true;
        }
    }
    //-----------------------------------------------------------------------
    void PagingLandScapePage::load()
    {
        touch ();
        if (mIsLoaded)
            return;
	    if (!mIsPreLoaded)
		    preload ();
	    if (!mIsTextureLoaded)
		    loadTexture ();

		assert (mTiles.empty());

		//mVisibletouch = 0;
		mIsLoaded = true;
        //mPageNode->showBoundingBox (true) ;
        if (mIsLoadable)
        {
            const unsigned int numTiles = mNumTiles;
            unsigned int i, j;

            mTiles.reserve (numTiles);
            mTiles.resize (numTiles);

	        for (i = 0; i < numTiles; ++i)
	        {
                mTiles[i].reserve (numTiles);
                mTiles[i].resize (numTiles);
	        } 

			PagingLandScapeTile *tile;
            PagingLandScapeTileManager * const tileMgr = mParent->getSceneManager()->getTileManager();
	        for (i = 0; i < numTiles; ++i)
	        {
	            for (j = 0; j < numTiles; ++j)
                {
                    //char name[ 24 ];
			        //sprintf(name, "page[%d,%d][%d,%d]", mTableX, mTableZ, i, j);

			        tile = tileMgr->getTile ();
			        assert (tile);
				    mTiles[ i ][ j ] = tile;
				    tile->init (mPageNode, mTableX, mTableZ, i, j);
		        }
	        }
	        for (i = 0; i < numTiles; ++i)
	        {
	            for (j = 0; j < numTiles; ++j)
		        {		
                    if (j != numTiles - 1)
                    {
                        mTiles[ i ][ j ]-> _setNeighbor(SOUTH, mTiles[ i ][ j + 1 ]);
                        mTiles[ i ][ j + 1 ] ->_setNeighbor(NORTH, mTiles[ i ][ j ]);
                    }
                    if (i != numTiles - 1)
                    {
                        mTiles[ i ][ j ]->_setNeighbor(EAST, mTiles[ i + 1 ][ j ]);
                        mTiles[ i + 1 ][ j ]->_setNeighbor(WEST, mTiles[ i ][ j ]);    
                    }           
		        }
			}

			PagingLandScapePageManager * const pageMgr = mParent;
			PagingLandScapePage *n;

			n = pageMgr->getPage (mTableX, mTableZ + 1, false);
			_setNeighbor(SOUTH, n);
			if (n)
				n->_setNeighbor(NORTH, this);

			n = pageMgr->getPage (mTableX, mTableZ - 1, false);
			_setNeighbor(NORTH, n);
			if (n)
				n->_setNeighbor(SOUTH, this);

			n = pageMgr->getPage (mTableX + 1, mTableZ, false);
			_setNeighbor(EAST, n);
			if (n)
				n->_setNeighbor(WEST, this);

			n = pageMgr->getPage (mTableX - 1, mTableZ, false);
			_setNeighbor(WEST, n);
			if (n)
				n->_setNeighbor(EAST, this);

        }
        
        mParent->getSceneManager()->getListenerManager()->firePageLoaded(mTableX, mTableZ, 
            mParent->getSceneManager()->getData2DManager()->getData2D(mTableX, mTableZ)->getHeightData(),
            mBounds);

		_Show(true);
    }

    //-----------------------------------------------------------------------
    void PagingLandScapePage::unload()
	{
        if (mIsLoaded)
        {

			assert (!mTiles.empty());

            // must be 0 to make sure page is really set as non visible
			//mVisibletouch = 0;
            //if (mVisible)
                _Show (false);

            // Unload the Tiles
            PagingLandScapeTiles::iterator iend = mTiles.end();
            for (PagingLandScapeTiles::iterator it = mTiles.begin(); 
                it != iend; 
                ++it)
            {
                
                std::for_each(it->begin (), 
                                it->end (),  
                                std::mem_fun(&PagingLandScapeTile::uninit));

                it->clear();   
            } 
			mTiles.clear();
			assert (mTiles.empty());

            mIsLoaded = false;       
     
            mParent->getSceneManager()->getListenerManager()->firePageUnloaded(mTableX, mTableZ, 
                mParent->getSceneManager()->getData2DManager()->getData2D(mTableX, mTableZ)->getHeightData(),
                mBounds);
            assert (mPageNode);
            assert (mPageNode->getParent () == 0);

		}
    }
    //-----------------------------------------------------------------------
    void PagingLandScapePage::unloadTexture()
    {        
        unload();
        if (mIsTextureLoaded) 
        {
            if (mIsLoadable)
                mParent->getSceneManager()->getTextureManager()->unload(mTableX, mTableZ);
            mIsTextureLoaded = false;
        }
    }
    //-----------------------------------------------------------------------
    void PagingLandScapePage::postUnload()
    {
	    unloadTexture ();
	    if (mIsPreLoaded)
	    {
		    mIsPreLoaded = false;

            if (mIsLoadable)
		        mParent->getSceneManager()->getData2DManager()->unload(mTableX, mTableZ);

            mParent->getSceneManager()->getListenerManager()->firePagePostunloaded (mTableX, mTableZ);
	    }
    }

    //-----------------------------------------------------------------------
    int PagingLandScapePage::isCameraIn(const Vector3 & pos) const
    {
	    if (mBounds.intersects(pos))
	    {
		    if (mBoundsInt.intersects(pos))
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
		assert (mPageNode);
        if (do_show) 
        {
            assert (!mVisible);

            if (!mPageNode->getParent ())
                mParent->getSceneManager()->getRootSceneNode()->addChild (mPageNode);

            mParent->getSceneManager()->getListenerManager()->firePageShow (mTableX, mTableZ, 
                mParent->getSceneManager()->getData2DManager()->getData2D(mTableX, mTableZ)->getHeightData(),
			    mBounds);
                
            if (mIsLoadable)
            {
                unsigned int i,k;
                for (i = 0; i < mNumTiles; ++i)
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
        else if (mVisible)
        {
            assert (do_show == false);
			//if (mVisibletouch == 0)
			{
				if (mPageNode->getParent ())
					mParent->getSceneManager()->getRootSceneNode()->removeChild (mPageNode->getName ());

				mParent->getSceneManager()->getListenerManager()->firePageHide (mTableX, mTableZ, 
						mParent->getSceneManager()->getData2DManager()->getData2D(mTableX, mTableZ)->getHeightData(),
						mBounds);
				if (mIsLoadable)
				{
					unsigned int i,k;
					for (i = 0; i < mNumTiles; ++i)
					{
						PagingLandScapeTileRow &tr = mTiles[ i ];
						for (k = 0; k < mNumTiles; ++k)
						{
							tr[ k ]->setInUse (false);
						}
					}
				}
				mVisible = false;
            }
            //else
            //{
            //    mVisibletouch--;
            //}
        }
    }
    //-----------------------------------------------------------------------
    bool PagingLandScapePage::_Notify(const Vector3 &pos, const PagingLandScapeCamera * const Cam)
    {
	    if (mIsLoaded && mIsLoadable)
        {
            // ((pos - mWorldPosition).squaredLength() < mParent->getOptions()->page_factor ?
            if (
                1
				//Cam->isVisible (mBoundsExt) 
                //&& 
                // if we use an Horizon Visibility Map
                //( !(mParent->getOptions()->VisMap)  
                //  || (mParent->getOptions()->VisMap 
                //      && mParent->getSceneManager()->getHorizon()->IsPageVisible (Cam, mTableX, mTableZ)))
                
              )
	        {         
				touch();
				//if (!mVisible)
				//	_Show (true);
				//mVisibletouch = 30;
	            for (unsigned int i = 0; i < mNumTiles; i++)
	            {
                    PagingLandScapeTileRow &tr = mTiles[ i ];
	                for (unsigned int k = 0; k < mNumTiles; k++)
                    {                 
                       tr[ k ]->_Notify(pos, Cam);
			        }
		        }
                return true;
	        }
            else if (mVisible)
            {
                // if it was visible it needs to change its state
                //_Show (false);
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
            const unsigned int x =  static_cast<unsigned int> (pos.x / mParent->getOptions()->scale.x / (mParent->getOptions()->TileSize));
            const unsigned int z =  static_cast<unsigned int> (pos.z / mParent->getOptions()->scale.z / (mParent->getOptions()->TileSize));

            assert (mTiles[x][z] && mTiles[x][z]-> isLoaded ());
            return mTiles[x][z];
        }
        return 0;
    }
    //-------------------------------------------------------------------------
	void PagingLandScapePage::_updateLod()
    {
        if (mIsLoaded && mIsLoadable)
        {
            unsigned int i,k;
	        for (i = 0; i < mNumTiles; ++i)
            {
                PagingLandScapeTileRow &tr = mTiles[ i ];
	            for (k = 0; k < mNumTiles; ++k)
	            {
                    PagingLandScapeTile *t = tr[ k ];
                    if (t->isVisible())
                        t->_updateLod();
			    }
		    }
        }
    }
    //-------------------------------------------------------------------------
	void PagingLandScapePage::setRenderQueue(uint8 qid)
	{
        if (mVisible && 
			mIsLoadable)
        {
            unsigned int i,k;
	        for (i = 0; i < mNumTiles; ++i)
            {
                PagingLandScapeTileRow &tr = mTiles[ i ];
	            for (k = 0; k < mNumTiles; ++k)
	            {
                    PagingLandScapeTile *t = tr[ k ];
                    if (t->isVisible())
                        t->setRenderQueueGroup(qid);
			    }
		    }
        }
    }
    //-------------------------------------------------------------------------
    PagingLandScapeTile* PagingLandScapePage::getTile(const unsigned int i , const unsigned int j) const
    {
        if (mIsLoaded)
        {
            assert (!mTiles.empty());

            assert (i < mParent->getOptions()->NumTiles);
            assert (j < mParent->getOptions()->NumTiles);

            assert (i < mTiles.size());
            assert (j < mTiles[i].size());

            return mTiles[i][j];
        }
        return 0;
    }

} //namespace
