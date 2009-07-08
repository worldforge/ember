/***************************************************************************
	OgrePagingLandScapePage.h  -  description
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

#ifndef PAGINGLandScapePAGE_H
#define PAGINGLandScapePAGE_H

#include "OgrePagingLandScapePrerequisites.h"

namespace Ogre
{
	class _OgrePagingLandScapeExport PagingLandScapePage
	{
        public:
	        PagingLandScapePage(PagingLandScapePageManager* pageMgr);

	        virtual ~PagingLandScapePage();

	        /** Sets the appropriate neighbor for this TerrainRenderable.  Neighbors are necessary
		    to know when to bridge between LODs.
		*/
		void _setNeighbor(const Neighbor& n, PagingLandScapePage* p);

		/** Returns the page's scene node
		 */
		SceneNode* getPageNode() const { return mPageNode; }

		/** Returns the neighbor TerrainRenderable.
		 */
		PagingLandScapePage* _getNeighbor(const Neighbor& n) const
		{
			return mNeighbors[ n ];
		}

		PagingLandScapeTile* getTile(const unsigned int i , const unsigned int j) const;

		PagingLandScapeTile* getTile(const Vector3& pos);

	        /** Whole Map changes */
		void init(const unsigned int tableX, const unsigned int tableZ);

	        /** Release the page, but keep it reusable if Whole Map changes */
		void uninit();

	        /** Pre-loads the LandScape data using parameters int he given in the constructor. */
	        void preload();

	        /** Loads the LandScape tiles using parameters int he given in the constructor. */
	        void load();

	        /** Unloads the LandScape data, then reloads it */
		void reload();

	        /** Loads the LandScape texture using parameters int he given in the constructor. */
		void loadTexture();

	        /** Unloads the LandScape texture, but doesn't destroy the LandScape data. */
		void unloadTexture();

	        /** Unloads the LandScape data, but doesn't destroy the LandScape page. */
	        void unload();

	        /** Post Unloads the LandScape data, but doesn't destroy the LandScape page. */
	        void postUnload();
            
	        void unsetLoading()
		{
			mIsLoading = false;
		}

	        void unsetPreLoading()
		{
			mIsPreLoading = false;
		}

	        void unsetTextureLoading()
		{
			mIsTextureLoading = false;
		}

	        void unsetUnloading()
		{
			mIsUnloading = false;
		}

	        void unsetPostUnloading()
		{
			mIsPostUnloading = false;
		}

		void unsetTextureunloading()
		{
			mIsTextureunloading = false;
		}
        
		const bool isLoaded() const 
		{
			return mIsLoaded;
		}

		const bool isPreLoaded() const 
		{
			return mIsPreLoaded;
		}

		const bool isTextureLoaded() const 
		{
			return mIsTextureLoaded;
		}

		const bool isLoadable() const 
		{
			return mIsLoadable;
		}

		const bool unloadUntouched();
		void touch();

		bool isVisible() const 
		{
			return mVisible;
		}

	        /** Returns if the camera is over this LandScape page.
		 */
	        int isCameraIn(const Vector3& pos) const;

	        bool _Notify(const Vector3& pos, const PagingLandScapeCamera* const Cam);
		void _Show(const bool do_show);

		void getCoordinates(unsigned int& X, unsigned int& Z) const 
		{
			X = mTableX;
			Z = mTableZ;
		}


	        bool mIsLoading;
	        bool mIsPreLoading;
		bool mIsTextureLoading;

	        bool mIsUnloading;
	        bool mIsPostUnloading;
		bool mIsTextureunloading;

		/** Sets the render queue group which the tiles should be rendered in. */
		void setRenderQueue(uint8 qid);

		void _updateLod();

		void setMapMaterial();

		inline bool isCoord(const unsigned int x, const unsigned int z) { return (mTableZ == z && mTableX == x); }
			
		SceneNode* getSceneNode() { return mPageNode; }
		const AxisAlignedBox& getWorldBbox() const { return mBounds; }
		const Vector3& getCenter() const { return mWorldPosition; }
			
		void updateTerrain();
			
        protected:
	        SceneNode* mPageNode;

	        PagingLandScapeTiles mTiles;

	        bool mIsLoaded;
	        bool mIsPreLoaded;
		bool mIsTextureLoaded; 
            
		// if data needed for this page doesn't exists
		bool mIsLoadable;

		bool mVisible; 
		// ensure page is not flickering due to shadow passes
		// as it unload instantly
		// but loading is queued
		// if not page not showed until mVisibletouch==0 it becomes invisible
		//size_t mVisibletouch;

		// Position of this Terrain Page in the Terrain Page Array
	        unsigned int mTableX;	
	        unsigned int mTableZ;

		unsigned int mNumTiles;

	        Real mIniX;	//, mEndX;	// Max and Min values of the terrain
	        Real mIniZ;	//, mEndZ;

		PagingLandScapePage* mNeighbors[4];

	        // Change Zone values
	        AxisAlignedBox mBounds;
	        AxisAlignedBox mBoundsInt;
	        AxisAlignedBox mBoundsExt;
		Vector3 mWorldPosition;

		PagingLandScapePageRenderable* mRenderable;

		unsigned int mTimeUntouched;

		PageState pageState;
		PageQueuingState pageQueingState;
		PagingLandScapePageManager* mParent;
	};

}

#endif
