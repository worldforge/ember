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

#ifndef PAGINGLandScapePAGE_H
#define PAGINGLandScapePAGE_H

#include "OgrePagingLandScapePrerequisites.h"

namespace Ogre
{


    class _OgrePagingLandScapeExport PagingLandScapePage
    {
        public:
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
            };

            PagingLandScapeTile* getTile(const uint i , const uint j) const;

            PagingLandScapeTile* getTile(const Vector3& pos);
	        PagingLandScapePage(PagingLandScapePageManager *pageMgr);

	        virtual ~PagingLandScapePage(void);

	        /** Whole Map changes */
            void init(const uint tableX, const uint tableZ);

	        /** Release the page, but keep it reusable if Whole Map changes */
            void uninit(void);

	        /** Pre-loads the LandScape data using parameters int he given in the constructor. */
	        void preload(void);

	        /** Loads the LandScape tiles using parameters int he given in the constructor. */
	        void load(void);

	        /** Unloads the LandScape data, then reloads it */
			void reload(void);

	        /** Loads the LandScape texture using parameters int he given in the constructor. */
            void loadTexture(void);

	        /** Unloads the LandScape texture, but doesn´t destroy the LandScape data. */
            void unloadTexture(void);

	        /** Unloads the LandScape data, but doesn´t destroy the LandScape page. */
	        void unload(void);

	        /** Post Unloads the LandScape data, but doesn´t destroy the LandScape page. */
	        void postUnload(void);
            
	        void unsetLoading(void)
			{
				mIsLoading = false;
			};

	        void unsetPreLoading(void)
			{
				mIsPreLoading = false;
			};

	        void unsetTextureLoading(void)
			{
				mIsTextureLoading = false;
			};

	        void unsetUnloading(void)
			{
				mIsUnloading = false;
			};

	        void unsetPostUnloading(void)
			{
				mIsPostUnloading = false;
			};

            void unsetTextureunloading(void)
			{
				mIsTextureunloading = false;
			};
        
            const bool isLoaded(void) const 
			{
				return mIsLoaded;
			};

            const bool isPreLoaded(void) const 
			{
				return mIsPreLoaded;
			};

            const bool isTextureLoaded(void) const 
			{
				return mIsTextureLoaded;
			};

            const bool isLoadable(void) const 
			{
				return mIsLoadable;
			};

            const bool touched(void);
            void touch(void);

            bool isVisible(void) const 
			{
				return mVisible;
			}

	        /** Returns if the camera is over this LandScape page.
	        */
	        int isCameraIn(const Vector3& pos) const;

	        bool _Notify(const Vector3& pos, PagingLandScapeCamera* Cam);
            void _Show(const bool do_show);

            void getCoordinates(uint& X, uint& Z) const 
			{
				X = mTableX;
				Z = mTableZ;
			};


	        bool mIsLoading;
	        bool mIsPreLoading;
            bool mIsTextureLoading;

           
	        bool mIsUnloading;
	        bool mIsPostUnloading;
            bool mIsTextureunloading;

		    /** Sets the render queue group which the tiles should be rendered in. */
		    void setRenderQueue(RenderQueueGroupID qid);

            void _updateLod(void);

            void setMapMaterial(void);

			inline bool isCoord(const uint x, const uint z){return (mTableZ == z && mTableX == x);};
			
			SceneNode *getSceneNode(){return mPageNode;};
			const AxisAlignedBox &getWorldBbox() const {return mBounds;};
			const Vector3 &getCenter(void) const {return mWorldPosition;};
			
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
			Vector3 mWorldPosition;

            PagingLandScapePageRenderable* mRenderable;

            uint mTimePreLoaded;

			PageState pageState;
            PageQueuingState pageQueingState;
            PagingLandScapePageManager *mParent;
    };

}

#endif
