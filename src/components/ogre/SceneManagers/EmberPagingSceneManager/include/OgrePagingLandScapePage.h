/***************************************************************************
	OgrePagingLandScapePage.h  -  description
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

#ifndef PAGINGLANDSCAPEPAGE_H
#define PAGINGLANDSCAPEPAGE_H

#include "OgrePagingLandScapePrerequisites.h"

namespace Ogre
{

#define PAGE_INSIDE  0x00000001
#define PAGE_CHANGE  0x00000002
#define PAGE_OUTSIDE 0x00000004

    class PagingLandScapePage
    {
        public:
	        /** Sets the appropriate neighbor for this TerrainRenderable.  Neighbors are necessary
            to know when to bridge between LODs.
            */
            void _setNeighbor( const Neighbor& n, PagingLandScapePage* t )
            {
                mNeighbors[ n ] = t;
            };

            /** Returns the neighbor TerrainRenderable.
            */
            PagingLandScapePage* _getNeighbor( const Neighbor& n )
            {
                return mNeighbors[ n ];
            };

            PagingLandScapeTile* getTile(const uint i , const uint j)
            {
                if ( mIsLoaded )
				{
                    return mTiles[i][j];
				}
                return 0;
            }

            PagingLandScapeTile* getTile( const Vector3& pos );
	        PagingLandScapePage( const uint tableX, const uint tableZ );

	        virtual ~PagingLandScapePage( void );

	        /** Whole Map changes */
            void init( void );

	        /** Release the page, but keep it reusable if Whole Map changes */
            void uninit( void );

	        /** Pre-loads the landscape data using parameters int he given in the constructor. */
	        void preload( void );

	        /** Loads the landscape tiles using parameters int he given in the constructor. */
	        void load( void );

	        /** Loads the landscape texture using parameters int he given in the constructor. */
            void loadTexture( void );

	        /** Unloads the landscape texture, but doesn´t destroy the landscape data. */
            void unloadTexture( void );

	        /** Unloads the landscape data, but doesn´t destroy the landscape page. */
	        void unload( void );

	        /** Post Unloads the landscape data, but doesn´t destroy the landscape page. */
	        void postUnload( void );
            
	        void unsetLoading( void )
			{
				mIsLoading = false;
			};

	        void unsetPreLoading( void )
			{
				mIsPreLoading = false;
			};

	        void unsetTextureLoading( void )
			{
				mIsTextureLoading = false;
			};

	        void unsetUnloading( void )
			{
				mIsUnloading = false;
			};

	        void unsetPostUnloading( void )
			{
				mIsPostUnloading = false;
			};

            void unsetTextureunloading( void )
			{
				mIsTextureunloading = false;
			};
        
            const bool isLoaded( void )
			{
				return mIsLoaded;
			};

            const bool isPreLoaded( void )
			{
				return mIsPreLoaded;
			};

            const bool isTextureLoaded( void )
			{
				return mIsTextureLoaded;
			};

            const bool isLoadable( void )
			{
				return mIsLoadable;
			};

            const bool touched( void );
            void touch( void );

            bool isVisible( void )
			{
				return mVisible;
			}

	        /** Returns if the camera is over this landscape page.
	        */
	        int isCameraIn( const Vector3& pos ) const;

	        bool _Notify( const Vector3& pos, PagingLandScapeCamera* Cam );
            void _Show( const bool do_show );

            void getCoordinates( uint& X, uint& Z )
			{
				X = mTableX;
				Z=mTableZ;
			};

            Vector3 getCenter( void )
			{
				return mBounds.getCenter( );
			};

	        bool mIsLoading;
	        bool mIsPreLoading;
            bool mIsTextureLoading;

           
	        bool mIsUnloading;
	        bool mIsPostUnloading;
            bool mIsTextureunloading;

		    /** Sets the render queue group which the tiles should be rendered in. */
		    void setRenderQueue( RenderQueueGroupID qid );

            void _updateLod( void );

            void setMapMaterial( void );

        protected:
	        SceneNode* mPageNode;

	        PagingLandScapeTiles mTiles;

	        bool mIsLoaded;
	        bool mIsPreLoaded;
            bool mIsTextureLoaded; 
            
            // if data needed for this page doesn't exists
            bool mIsLoadable;

            bool mVisible;

	        uint mTableX;	// Position of this Terrain Page in the Terrain Page Array
	        uint mTableZ;

            uint mNumTiles;

	        Real mIniX;	//, mEndX;	// Max and Min values of the terrain
	        Real mIniZ;	//, mEndZ;

            PagingLandScapePage* mNeighbors[4];

	        // Change Zone values
	        AxisAlignedBox mBounds;
	        AxisAlignedBox mBoundsInt;
	        AxisAlignedBox mBoundsExt;

            PagingLandScapePageRenderable* mRenderable;

            uint mTimePreLoaded;
    };

}

#endif
