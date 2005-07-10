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

#ifndef PAGINGLANDSCAPEPAGEMANAGER_H
#define PAGINGLANDSCAPEPAGEMANAGER_H

#include "OgrePagingLandScapePrerequisites.h"
#include "OgreSingleton.h"
#include "OgreFrameListener.h"
#include "OgrePagingLandScapeQueue.h"

#include "OgrePagingLandScapePage.h"

namespace Ogre
{
    class PagingLandScapePageManager : public FrameListener, public Singleton< PagingLandScapePageManager >
    {
        public:
	        PagingLandScapePageManager( void );
	        ~PagingLandScapePageManager( void );

            void load( void );
            void clear( void );
            void reset( void );

            void LoadFirstPage( PagingLandScapeCamera* cam );
            void LoadFirstPage( const Vector3 &pos );
	        void updatePaging( PagingLandScapeCamera* cam );

	        uint getCurrentCameraPageX( void ) const;
            uint getCurrentCameraPageZ( void ) const;
            uint getCurrentCameraTileX( void ) const;
            uint getCurrentCameraTileZ( void ) const;

	        int getPagePreloadQueueSize( void ) const;
	        int getPageLoadQueueSize( void ) const;
            int getPageTextureloadQueueSize( void ) const;

            void _updateLod( void );

            int getLoadedPageSize( void ) const;
            int getTextureLoadedPageSize( void ) const;
            int getPreLoadedPageSize( void ) const;
            int getUnloadedPageSize( void ) const;

            PagingLandScapePage* getPage( const uint i , const uint j )
            {
                return mPages[i][j];
            }

            PagingLandScapeTile* getTile( const Real posx, const Real posz, bool alwaysAnswer );
            PagingLandScapeTile* getTileUnscaled( const Real posx, const Real posz, bool alwaysAnswer );
            PagingLandScapeTile* getTile( const Real posx, const Real posz, const uint pagex, const uint pagez, bool alwaysAnswer );

            PagingLandScapeTile* getTileUnscaled( const Real posx, const Real posz, const uint pagex, const uint pagez, bool alwaysAnswer );
            PagingLandScapeTile* getTilePage (uint &posx, uint &posz, const uint pagex, const uint pagez);

            void getGlobalToPage( Real& x, Real& z ) const;
            /** Get the Page indices from a position
                @param posx the world position vector.
                @param posz the world position vector. 
                @param x	result placed in reference to the x index of the page
                @param z	result placed in reference to the z index of the page
            */
            bool getPageIndices( const Real posx, const Real posz, uint& x, uint& z, bool alwaysAnswer ) const;

            bool getRealPageIndicesUnscaled( const Real posx, const Real posz, uint& x, uint& z ) const; 
            void getNearestPageIndicesUnscaled( const Real posx, const Real posz, uint& x, uint& z ) const; 
            
			/** Get the Tile indices from a position
                @param posx the world position vector. 
                @param posz the world position vector.
                @param pagex the world position page number. 
                @param pagez the world position page number. 
                @param x	result placed in reference to the x index of the page
                @param z	result placed in reference to the z index of the page
            */
            bool getTileIndices( const Real posx, const Real posz, const uint pagex, const uint pagez, uint& x, uint& z, bool alwaysAnswer ) const;
            bool getRealTileIndicesUnscaled( const Real posx, const Real posz, const uint pagex, const uint pagez, uint& x, uint& z ) const;
            void getNearestTileIndicesUnscaled( const Real posx, const Real posz, const uint pagex, const uint pagez, uint& x, uint& z ) const;

            void setTerrainReady( bool isready )
			{
				mTerrainReady = isready;
			};

	        static PagingLandScapePageManager& getSingleton( void );

	        static PagingLandScapePageManager* getSingletonPtr( void );

            void removeFromQueues( PagingLandScapePage* p );
            
            bool frameStarted( const FrameEvent& evt );
            bool frameEnded( const FrameEvent& evt );

            void setWorldGeometryRenderQueue( RenderQueueGroupID qid );
            RenderQueueGroupID getRenderQueueGroupID( void )
			{
				return mRenderQueueGroupID;
			};

            void setMapMaterial( void );
            void WorldDimensionChange( void );

        protected:

            PagingLandScapePage* find_nearest( const Vector3& pos, const uint x, const uint z, PagingLandScapePagelist& mQueue ) const;

            PagingLandScapeOptions*           mOptions;
            PagingLandScapeData2DManager*     mData2d;
            PagingLandScapeTextureManager*    mTexture;
            PagingLandScapeRenderableManager* mRenderable;

	        /** LandScape pages for the terrain.
	        */
	        PagingLandScapePages mPages;

            PagingLandScapePagelist mPageLoadQueue;
            PagingLandScapePagelist mPagePreloadQueue;
            PagingLandScapePagelist mPageTextureloadQueue;

            PagingLandScapePagelist mLoadedPages;
            PagingLandScapePagelist mPreLoadedPages;
            PagingLandScapePagelist mTextureLoadedPages;
            PagingLandScapePagelist mUnloadedPages;

            uint mWidth;
            uint mHeight;

            uint mPageSize; 
            uint mTileSize;

            int mNextQueueFrameCount;
            int mTimePreLoaded;
	int mPause;
            PagingLandScapeCamera* mCurrentcam;
            bool mTerrainReady;

            bool mOnFrame;

            RenderQueueGroupID mRenderQueueGroupID;

    };

}

#endif
