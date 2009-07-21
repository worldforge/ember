/***************************************************************************
	OgrePagingLandScapePage.h  -  description
  -------------------
  begin                : Sat Mar 08 2003
  copyright            : (C) 2003-2006 by Jose A. Milan and Tuan Kuranes
  email                : spoke2@supercable.es && tuan.kuranes@free.fr

  * Copyright (C) 2009 Manuel A. Fernandez Montecelo <manuel.montezelo@gmail.com>

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
	/** Class representing a terrain page for the Paging LandScape
	 */
	class _OgrePagingLandScapeExport PagingLandScapePage
	{
        public:
		/** Possible queues in which the page lives */
		enum PageInQueue {
			QUEUE_NONE,
			QUEUE_PRELOAD,
			QUEUE_TEXTURELOAD,
			QUEUE_LOAD
		};


	        /** Ctor. */
	        PagingLandScapePage(PagingLandScapePageManager& pageMgr);

	        /** Dtor. */
	        virtual ~PagingLandScapePage();

	        /** Whole Map changes */
		void init(unsigned int tableX, unsigned int tableZ);

	        /** Release the page, but keep it reusable if Whole Map changes */
		void uninit();

	        /** Loads the LandScape tiles using parameters given in the constructor. */
	        void load();

	        /** Loads the LandScape texture using parameters given in the constructor. */
		void loadTexture();

	        /** Pre-loads the LandScape data using parameters given in the constructor. */
	        void preload();

	        /** Show/hide the page */
		void show(bool do_show);

		/** Set material map of the page */
		void setMapMaterial();

		/** Sets the render queue group which the tiles should be rendered in. */
		void setRenderQueue(uint8 qid);

		/** Update the terrain data */
		void updateTerrain();

		/** Update Level of Detail. */
		void updateLod();

		/** Notify the tiles that we are near the camera on next render
		 *
		 * @return true when loaded and tiles were notified
		 */
	        bool notify(const Vector3& pos, const PagingLandScapeCamera* cam);

		/** Touch the page when there are operations (marking is at being recently used) */
		void touch();

		/** Unload unused pages */
		bool unloadUntouched();

		/** Event of the data for the page being loaded, called by the data 2D manager */
		void eventData2DLoaded(bool operationOK);

		/** Get tile at the given position (checking that it's loaded).
		 *
		 * @return 0 for invalid or unloaded page, the pointer otherwise
		 */
		PagingLandScapeTile* getTile(const Vector3& pos) const;
		/** Get tile at the given indices in the matrix holding them.
		 *
		 * @return 0 for invalid page, the pointer otherwise
		 */
		PagingLandScapeTile* getTile(unsigned int i , unsigned int j) const;

	        /** Get coordinates of the page */
		inline void getCoordinates(unsigned int& x, unsigned int& z) const
		{
			x = mTableX;
			z = mTableZ;
		}

	        /** Check whether the coordinates match those of the page */
		inline bool doCoordinatesMatch(unsigned int x, unsigned int z)
		{
			return (mTableZ == z && mTableX == x);
		}

		/** Get the SceneNode */
		inline SceneNode* getSceneNode()
		{
			return mPageNode;
		}

		/** Get the center position of the page */
		inline const Vector3& getCenter() const
		{
			return mWorldPosition;
		}

		/** Whether the page is loaded */
		inline bool isLoaded() const 
		{
			return (mPageState == STATE_LOADED);
		}

		/** Whether the page is preloaded */
		inline bool isPreLoaded() const 
		{
			return (mPageState == STATE_PRELOADED);
		}

		/** Whether the page has the texture loaded */
		inline bool isTextureLoaded() const 
		{
			return (mPageState == STATE_TEXTURELOADED);
		}

		/** Whether the page is visible loaded */
		inline bool isVisible() const 
		{
			return mVisible;
		}

		/** Whether the page is in the corresponding queue */
		inline bool isInPreloadQueue() const 
		{
			return (mPageInQueue == QUEUE_PRELOAD);
		}
		/** Whether the page is in the corresponding queue */
		inline bool isInTextureloadQueue() const 
		{
			return (mPageInQueue == QUEUE_TEXTURELOAD);
		}
		/** Whether the page is in the corresponding queue */
		inline bool isInLoadQueue() const 
		{
			return (mPageInQueue == QUEUE_LOAD);
		}
		/** Whether the page is not in any queue */
		inline bool isNotInAnyQueue() const 
		{
			return (mPageInQueue == QUEUE_NONE);
		}
		/** Set the page in the corresponding queue */
		inline void setInQueue(PageInQueue queue) 
		{
			mPageInQueue = queue;
		}

        protected:
		/** Possible states of the page */
		enum PageState {
			STATE_UNINITED,
			STATE_INITED,
			STATE_PRELOADING,
			STATE_PRELOADED,
			STATE_TEXTURELOADED,
			STATE_LOADED,
			STATE_NOTLOADABLE
		};

		/** Events emitted by this class */
		enum EventType {
			EVENT_PRELOADED,
			EVENT_LOADED,
			EVENT_UNLOADED,
			EVENT_POSTUNLOADED,
			EVENT_SHOW,
			EVENT_HIDE
		};


		/// Reference to the manager
		PagingLandScapePageManager& mPageMgr;

		/// State of this page
		PageState mPageState;

		/// Queue in which this page lives at a given moment
		PageInQueue mPageInQueue;

		/// Position of this Terrain Page in the Terrain Page Array
	        unsigned int mTableX;
		/// Position of this Terrain Page in the Terrain Page Array
	        unsigned int mTableZ;
	        /// Bounds of the terrain page
	        AxisAlignedBox mBounds;
	        /// Position in the world of the terrain page
		Vector3 mWorldPosition;

		/// Whether the page is visible or not
		bool mVisible;

		/// Variable to control if it's been used recently
		unsigned int mTimeUntouched;

		/// SceneNode corresponding to this Terrain Page
		Ogre::SceneNode* mPageNode;

		/// Renderable for this page
		PagingLandScapePageRenderable* mRenderable;

		/// Number of tiles of the page
		unsigned int mNumTiles;
		/// Tiles of the page
	        PagingLandScapeTiles mTiles;

		/// Neighbor pages
		PagingLandScapePage* mNeighbors[4];


	        /** Sets neighbors. */
		void setNeighbors();
	        /** Reset neighbors. */
		void resetNeighbors();
	        /** Sets the appropriate neighbor for this TerrainRenderable.
		 * Neighbors are necessary to know when to bridge between LODs.
		 */
		void setNeighbor(Neighbor n, PagingLandScapePage* p);

	        /** Fire event */
		void fireEvent(EventType type);

	};

}

#endif
