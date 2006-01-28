/***************************************************************************
	OgrePagingLandScapePageManager.h  -  description
	-------------------
	begin                : Sat May 01 2004
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

#ifndef PAGINGLandScapePAGEMANAGER_H
#define PAGINGLandScapePAGEMANAGER_H

#include "OgrePagingLandScapePrerequisites.h"
#include "OgreSingleton.h"
#include "OgreFrameListener.h"
#include "OgrePagingLandScapeQueue.h"

#include "OgrePagingLandScapePage.h"

namespace Ogre
{
    class _OgrePagingLandScapeExport PagingLandScapePageManager : public FrameListener, public Singleton< PagingLandScapePageManager >
    {
        public:
	        
			///ctor
			PagingLandScapePageManager(void);
			///dtor
	        ~PagingLandScapePageManager(void);

			/// upon new landscape
			void load(void);
			/// empty before loading a landscape or final deletion
            void clear(void);
			/// reset paging but keep page pool in memory
            void reset(void);

			/// used to load first page earlier than first updatePaging() call
			/// say like just after scene manager setWorldGeom()
            void LoadFirstPage(PagingLandScapeCamera* cam);
			/// Make sure page under camera is loaded, 
			/// that neighbor pages are preLoaded, preLoading
			/// and process queues accordingly
	        void updatePaging(PagingLandScapeCamera* cam);

			// recursively call LOD update on all page and tiles
			void _updateLod(void);
			
			// load everything around camera just now.
            void loadNow(PagingLandScapeCamera *cam);

			/// if page is already instantiated get page at this pos,
			/// otherwise allocate one, if alwaysReturn is set true.
			PagingLandScapePage* getPage(const uint i , const uint j,
				const bool alwaysReturn = true);
			/// Instantiate a new page from pool.
			PagingLandScapePage* getNewPage(const uint i , const uint j);
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
										 const uint pagex, const uint pagez, 
										 bool alwaysAnswer);
			// get Tile at relative position but unscaled by landscape scale (in the page containing the tile page coordinate system) in space, 
			/// return answer even when position is outside landscape
			/// when alwaysAnswer is true
			PagingLandScapeTile* getTileUnscaled(const Real posx, const Real posz, 
												 const uint pagex, const uint pagez, 
												 bool alwaysAnswer);
			// get Tile at relative position but unscaled by landscape scale (in the page containing the tile page coordinate system) in space, 
			/// return answer even when position is outside landscape
			/// when alwaysAnswer is true
			PagingLandScapeTile* getTilePage (uint &posx, uint &posz, const uint pagex, const uint pagez);

			void getGlobalToPage(Real& x, Real& z) const;
			/** Get the Page indices from a position
			@param posx the world position vector.
			@param posz the world position vector. 
			@param x	result placed in reference to the x index of the page
			@param z	result placed in reference to the z index of the page
			*/
			bool getPageIndices(const Real posx, const Real posz, uint& x, uint& z, bool alwaysAnswer) const;

			/** Get the Page indices from a position, returning page only if position is in.
			@param posx the world position vector but unscaled. 
			@param posz the world position vector but unscaled.  
			@param x	result placed in reference to the x index of the page
			@param z	result placed in reference to the z index of the page
			*/
			bool getRealPageIndicesUnscaled(const Real posx, const Real posz, uint& x, uint& z) const; 

			/** Get the Page indices from a position, always returning a page.
			@param posx the world position vector but unscaled. 
			@param posz the world position vector but unscaled.  
			@param x	result placed in reference to the x index of the page
			@param z	result placed in reference to the z index of the page
			*/
			void getNearestPageIndicesUnscaled(const Real posx, const Real posz, uint& x, uint& z) const; 

			/** Get the Tile indices from a position
			@param posx the world position vector. 
			@param posz the world position vector.
			@param pagex the world position page number. 
			@param pagez the world position page number. 
			@param x	result placed in reference to the x index of the page
			@param z	result placed in reference to the z index of the page
			*/
			bool getTileIndices(const Real posx, const Real posz, const uint pagex, const uint pagez, uint& x, uint& z, bool alwaysAnswer) const;

			/** Get the Tile indices from a position, returning tile only if position is in.
			@param posx the world position vector but unscaled. 
			@param posz the world position vector but unscaled.  
			@param x	result placed in reference to the x index of the page
			@param z	result placed in reference to the z index of the page
			*/
			bool getRealTileIndicesUnscaled(const Real posx, const Real posz, const uint pagex, const uint pagez, uint& x, uint& z) const;

			/** Get the Tile indices from a position, returning tile only if position is in.
			@param posx the world position vector but unscaled. 
			@param posz the world position vector but unscaled.  
			@param x	result placed in reference to the x index of the page
			@param z	result placed in reference to the z index of the page
			*/
			void getNearestTileIndicesUnscaled(const Real posx, const Real posz, const uint pagex, const uint pagez, uint& x, uint& z) const;

			void setTerrainReady(bool isready)
			{
				mTerrainReady = isready;
			};

			static PagingLandScapePageManager& getSingleton(void);

			static PagingLandScapePageManager* getSingletonPtr(void);

			void removeFromQueues(PagingLandScapePage* p);

			bool frameStarted(const FrameEvent& evt);
			bool frameEnded(const FrameEvent& evt);

			void setWorldGeometryRenderQueue(RenderQueueGroupID qid);
			RenderQueueGroupID getRenderQueueGroupID(void)
			{
				return mRenderQueueGroupID;
			};

			void setMapMaterial(void);
			void WorldDimensionChange(void);



			/// getter
			uint getCurrentCameraPageX(void) const;
			/// getter
			uint getCurrentCameraPageZ(void) const;
			/// getter
			uint getCurrentCameraTileX(void) const;
			/// getter
            uint getCurrentCameraTileZ(void) const;
			/// getter
			int getPagePreloadQueueSize(void) const;
			/// getter
			int getPageLoadQueueSize(void) const;
			/// getter
            int getPageTextureloadQueueSize(void) const;
			/// getter
			int getLoadedPageSize(void) const;
			/// getter
			int getPreLoadedPageSize(void) const;
			/// getter
			int getTextureLoadedPageSize(void) const;
			/// getter
			int getUnloadedPagesSize(void) const;

            RenderQueueGroupID getPageRenderQueue(){return mRenderQueueGroupID;};
        protected:

			void processUnloadQueues();
			void processLoadQueues();
			void updateLoadedPages();
			void queuePageNeighbors ();

			void makePageLoadedNow(PagingLandScapePage* p);
            PagingLandScapePage* find_nearest(const Vector3& pos, const uint x, const uint z, PagingLandScapePageList& mQueue) const;

            PagingLandScapeOptions*           mOptions;
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

            uint mWidth;
            uint mHeight;

            int mNextQueueFrameCount;
            int mTimePreLoaded;

			int mPause;

            PagingLandScapeCamera* mCurrentcam;
            bool mTerrainReady;
            bool mOnFrame;

			uint mRenderableLoadInterval;
			uint mPageLoadInterval;

            RenderQueueGroupID mRenderQueueGroupID;
	};

}

#endif
