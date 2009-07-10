/***************************************************************************
	OgrePagingLandScapePageManager.h  -  description
	-------------------
	begin                : Sat May 01 2004
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

#ifndef PAGINGLandScapePAGEMANAGER_H
#define PAGINGLandScapePAGEMANAGER_H

#include "OgrePagingLandScapePrerequisites.h"
#include "OgrePagingLandScapeOptions.h"
#include "OgrePagingLandScapePage.h"
#include "OgrePagingLandScapeQueue.h"

#include "OgreFrameListener.h"


namespace Ogre
{
	class _OgrePagingLandScapeExport PagingLandScapePageManager : public FrameListener
	{
        public:

		///ctor
		PagingLandScapePageManager(PagingLandScapeSceneManager * scnMgr);
		///dtor
	        ~PagingLandScapePageManager();

		/// upon new landscape
		void load();
		/// empty before loading a landscape or final deletion
		void clear();
		/// reset paging but keep page pool in memory
		void reset();

		/// used to load first page earlier than first updatePaging() call
		/// say like just after scene manager setWorldGeom()
		void LoadFirstPage(PagingLandScapeCamera* cam);
		/// Make sure page under camera is loaded,
		/// that neighbor pages are preLoaded, preLoading
		/// and process queues accordingly
	        void updatePaging(PagingLandScapeCamera* cam);

		// recursively call LOD update on all page and tiles
		void _updateLod();
			
		// load everything around camera just now.
		void loadNow(PagingLandScapeCamera *cam);
		// Make sure page gets into loaded page list when loaded from outside 
		// ie "LoadNow" getOption
		void addLoadedPage(PagingLandScapePage *p);

		/// if page is already instantiated get page at this pos,
		/// otherwise allocate one, if alwaysReturn is set true.
		PagingLandScapePage* getPage(const unsigned int i , const unsigned int j,
					     const bool alwaysReturn = true);
		/// Instantiate a new page from pool.
		PagingLandScapePage* getNewPage(const unsigned int i , const unsigned int j);
		/// Return a page to the pool.
		void releasePage (PagingLandScapePage*p );

		/// get Tile at absolute position in space, return answer even when position is outside landscape
		/// when alwaysAnswer is true
		PagingLandScapeTile* getTile(const Real posx, const Real posz, bool alwaysAnswer);
		/// get Tile at absolute position but unscaled by landscape scale in space, 
		/// return answer even when position is outside landscape
		/// when alwaysAnswer is true
		PagingLandScapeTile* getTileUnscaled(const Real posx, const Real posz, bool alwaysAnswer);
		// get Tile at relative position (in the page containing the tile page coordinate system) in space, 
		/// return answer even when position is outside landscape
		/// when alwaysAnswer is true
		PagingLandScapeTile* getTile(const Real posx, const Real posz, 
					     const unsigned int pagex, const unsigned int pagez, 
					     bool alwaysAnswer);
		// get Tile at relative position but unscaled by landscape scale (in the page containing the tile page coordinate system) in space, 
		/// return answer even when position is outside landscape
		/// when alwaysAnswer is true
		PagingLandScapeTile* getTileUnscaled(const Real posx, const Real posz, 
						     const unsigned int pagex, const unsigned int pagez, 
						     bool alwaysAnswer);
		// get Tile at relative position but unscaled by landscape scale (in the page containing the tile page coordinate system) in space, 
		/// return answer even when position is outside landscape
		/// when alwaysAnswer is true
		PagingLandScapeTile* getTilePage (unsigned int &posx, unsigned int &posz, const unsigned int pagex, const unsigned int pagez);

		void getGlobalToPage(Real& x, Real& z) const;
		/** Get the Page indices from a position
		    @param posx the world position vector.
		    @param posz the world position vector. 
		    @param x	result placed in reference to the x index of the page
		    @param z	result placed in reference to the z index of the page
		*/
		inline bool getPageIndices(const Real posx, const Real posz, unsigned int& x, unsigned int& z, bool alwaysAnswer) const
		{
			if (alwaysAnswer)
			{
				getNearestPageIndicesUnscaled(posx * mOptions->invScale.x, posz* mOptions->invScale.z, x, z);
				return true;
			}
			else
			{
				return getRealPageIndicesUnscaled(posx * mOptions->invScale.x, posz* mOptions->invScale.z, x, z);
			}
		}
		/** Get the Page indices from a position, returning page only if position is in.
		    @param posx the world position vector but unscaled. 
		    @param posz the world position vector but unscaled.  
		    @param x	result placed in reference to the x index of the page
		    @param z	result placed in reference to the z index of the page
		*/
		inline bool getRealPageIndicesUnscaled(const Real posx, const Real posz, 
						       unsigned int& x, unsigned int& z) 
			const
		{      
			const Real lx = ((posx + mOptions->maxUnScaledX) * mOptions->invPageSizeMinusOne);
			const Real lz = ((posz + mOptions->maxUnScaledZ) * mOptions->invPageSizeMinusOne);

			// make sure indices are not negative or outside range of number of pages
			if (lx >= mOptions->world_width || lx < static_cast <Real> (0.0) || 
			    lz >= mOptions->world_height || lz < static_cast <Real> (0.0) )
			{
				return false;
			}
			else 
			{
				x = static_cast< unsigned int > (lx);
				z = static_cast< unsigned int > (lz);
				return true;
			}
		}
		/** Get the Page indices from a position, always returning a page.
		    @param posx the world position vector but unscaled. 
		    @param posz the world position vector but unscaled.  
		    @param x	result placed in reference to the x index of the page
		    @param z	result placed in reference to the z index of the page
		*/
		void getNearestPageIndicesUnscaled(const Real posx, const Real posz, unsigned int& x, unsigned int& z) const; 

		/** Get the Tile indices from a position
		    @param posx the world position vector. 
		    @param posz the world position vector.
		    @param pagex the world position page number. 
		    @param pagez the world position page number. 
		    @param x	result placed in reference to the x index of the page
		    @param z	result placed in reference to the z index of the page
		*/
		bool getTileIndices(const Real posx, const Real posz, const unsigned int pagex, const unsigned int pagez, unsigned int& x, unsigned int& z, bool alwaysAnswer) const;

		/** Get the Tile indices from a position, returning tile only if position is in.
		    @param posx the world position vector but unscaled. 
		    @param posz the world position vector but unscaled.  
		    @param x	result placed in reference to the x index of the page
		    @param z	result placed in reference to the z index of the page
		*/
		inline bool getRealTileIndicesUnscaled(const Real posx, const Real posz, 
						       const unsigned int pagex, const unsigned int pagez, 
						       unsigned int& x, unsigned int& z) const
		{
			// adjust x and z to be local to page
			const int pSize = mOptions->PageSize - 1;
			const int tilex = static_cast< int >((posx - ((pagex * pSize) - mOptions->maxUnScaledX)) * mOptions->invTileSizeMinusOne); 
			const int tilez = static_cast< int >((posz - ((pagez * pSize) - mOptions->maxUnScaledZ)) * mOptions->invTileSizeMinusOne);

            
			const int tilesPerPage = static_cast< int >(mOptions->NumTiles);
			//const int tilesPerPage = static_cast< int >(mOptions->NumTiles(pSize * inv_tSize) - 1);

			if (tilex > tilesPerPage || tilex < 0 || 
			    tilez > tilesPerPage || tilez < 0)
			{
				return false;
			}
			else
			{
				x = static_cast< unsigned int >(tilex);
				z = static_cast< unsigned int >(tilez);
				return true;
			}
		}

		/** Get the Tile indices from a position, returning tile only if position is in.
		    @param posx the world position vector but unscaled. 
		    @param posz the world position vector but unscaled.  
		    @param x	result placed in reference to the x index of the page
		    @param z	result placed in reference to the z index of the page
		*/
		void getNearestTileIndicesUnscaled(const Real posx, const Real posz, const unsigned int pagex, const unsigned int pagez, unsigned int& x, unsigned int& z) const;

		void setTerrainReady(bool isready)
		{
			mTerrainReady = isready;
		};


		void removeFromQueues(PagingLandScapePage* p);

		bool frameStarted(const FrameEvent& evt);
		bool frameEnded(const FrameEvent& evt);

		void setWorldGeometryRenderQueue(uint8 qid);
		RenderQueueGroupID getRenderQueueGroupID()
		{
			return mRenderQueueGroupID;
		};

		void setMapMaterial();
		void WorldDimensionChange();



		/// getter
		unsigned int getCurrentCameraPageX() const;
		/// getter
		unsigned int getCurrentCameraPageZ() const;
		/// getter
		unsigned int getCurrentCameraTileX() const;
		/// getter
		unsigned int getCurrentCameraTileZ() const;
		/// getter
		int getPagePreloadQueueSize() const;
		/// getter
		int getPageLoadQueueSize() const;
		/// getter
		int getPageTextureloadQueueSize() const;
		/// getter
		int getLoadedPageSize() const;
		/// getter
		int getPreLoadedPageSize() const;
		/// getter
		int getTextureLoadedPageSize() const;
		/// getter
		int getUnloadedPagesSize() const;

		RenderQueueGroupID getPageRenderQueue(){return mRenderQueueGroupID;};

		PagingLandScapeOptions*		getOptions(){return mOptions;}
		PagingLandScapeSceneManager* getSceneManager(){return mSceneManager;}

		bool isEnabled ()const {return mEnabled;}
		void setEnabled (const bool enabled){mEnabled = enabled;}

        protected:

		PagingLandScapeOptions*		mOptions;
		PagingLandScapeSceneManager *mSceneManager;



		void processUnloadQueues();
		void processLoadQueues();
		void updateLoadedPages();
		void queuePageNeighbors ();

		void makePageLoadedNow(PagingLandScapePage* p);
		PagingLandScapePage* find_nearest(const Vector3& pos, const unsigned int x, const unsigned int z, PagingLandScapePageList& mQueue) const;

		PagingLandScapeData2DManager*     mData2d;
		PagingLandScapeTextureManager*    mTexture;
		PagingLandScapeRenderableManager* mRenderablesMgr;

	        /** LandScape pages for the terrain.
		 */
	        //PagingLandScapePages mPages;

		PagingLandScapeQueue <PagingLandScapePage> mPageLoadQueue;
		PagingLandScapeQueue <PagingLandScapePage> mPagePreloadQueue;
		PagingLandScapeQueue <PagingLandScapePage> mPageTextureloadQueue;

		PagingLandScapePageList mLoadedPages;
		PagingLandScapePageList mPreLoadedPages;
		PagingLandScapePageList mTextureLoadedPages;

		PagingLandScapePageList mActivePages;
		PagingLandScapePageList mFreePages;
		PagingLandScapePageArray mPagePool;

		unsigned int mWidth;
		unsigned int mHeight;

		int mNextQueueFrameCount;
		int mTimePreLoaded;

		int mPause;

		PagingLandScapeCamera* mCurrentcam;
		bool mTerrainReady;
		bool mOnFrame;

		unsigned int mRenderableLoadInterval;
		unsigned int mPageLoadInterval;

		RenderQueueGroupID mRenderQueueGroupID;

		//if not queued to be removed from frame listener
		//or SM is in paused State
		bool mEnabled;
	};

}

#endif
