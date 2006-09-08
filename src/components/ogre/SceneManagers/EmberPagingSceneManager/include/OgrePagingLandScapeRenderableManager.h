/***************************************************************************
OgrePagingLandScapeRenderableManager.h  -  description
-------------------
begin                : Mon Jun 16 2003
copyright            : (C) 2003-2006 by Jose A Milan && Tuan Kuranes
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

#ifndef PAGINGLandScapeRENDERABLEMANAGER_H
#define PAGINGLandScapeRENDERABLEMANAGER_H

#include "OgrePagingLandScapePrerequisites.h"
#include "OgrePagingLandScapeQueue.h"
#include "OgrePagingLandScapeIndexBuffer.h"

#include "OgrePagingLandScapeTileInfo.h"

#include "OgrePagingLandScapePoolSet.h"

namespace Ogre
{



	/** Class to manage A collection of pre-allocated PagingLandScapeRenderable
	*/
	class PagingLandScapeRenderableSet : public PoolSet<PagingLandScapeRenderable> 
	{
	public:

		PagingLandScapeRenderableSet::PagingLandScapeRenderableSet();
		void setRenderableManager (PagingLandScapeRenderableManager *rMgr)
		{
			mRenderableManager = rMgr;
		}
	protected:

		PagingLandScapeRenderable* allocate ();
		void deallocate (PagingLandScapeRenderable *r);


	private:
		PagingLandScapeRenderableManager *mRenderableManager;

	};
	/** Class to manage the PagingLandScapeRenderables (Pool and Loading Queue)
	@remarks
	This class is used as a store of PagingLandScapeRenderables and the are reference and dereference in order to not being created and
	destroyed every time the plug-in need to change the LOD.
	*/
	class PagingLandScapeRenderableManager
	{
	public:
		/** Initializes the PagingLandScapeRenderableManager with the 
		* given options and allocate the necessary memory.
		*/
		PagingLandScapeRenderableManager (PagingLandScapeSceneManager * scnMgr);

		virtual ~PagingLandScapeRenderableManager(void);

		/** Retrieve a free renderable.
		*/
		PagingLandScapeRenderable* getRenderable(void);

		/** Make a renderable free.
		*/
		void freeRenderable(PagingLandScapeRenderable* rend);

		/** Set this renderable to be loaded
		*/
		void queueRenderableLoading(PagingLandScapeTile* tile);

		/** Set this renderable to be unloaded
		*/
		void unqueueRenderable(PagingLandScapeTile* tile);

		/** Load a set of renderables
		*/
		bool executeRenderableLoading(const Vector3 &Cameraposition);

		size_t numRenderables(void) const;
		size_t numFree(void) const;
		size_t numLoading(void) const;

		void InitTextureCoordinatesBuffers(void);
		void freeTextureCoordinatesBuffers(void);
		HardwareVertexBufferSharedPtr getTextureCoordinatesBuffers(const unsigned int tilex, const unsigned int tilez);
		void setTextureCoordinatesBuffers(const unsigned int tilex, const unsigned int tilez, const HardwareVertexBufferSharedPtr& data);

		unsigned int numVisibles(void) const
		{
			return mLastRenderablesVisibles;
		};
		void resetVisibles(void)
		{
			mLastRenderablesVisibles = mRenderablesVisibles;
			mRenderablesVisibles = 0;
		};
		void addVisible(void)
		{
			mRenderablesVisibles++;
		};


		void load(void);
		void clear(void);

		/// unload some Tiles/renderables if no more in use 
		void processTileUnload();

		inline PagingLandScapeOptions*		getOptions(){return mOptions;}
		inline PagingLandScapeSceneManager* getSceneManager(){return mSceneManager;}
	protected:

		PagingLandScapeOptions*		mOptions;
		PagingLandScapeSceneManager *mSceneManager;


		void _addBatch(const unsigned int num);


		///Keep a set of pre-allocated PagingLandscapeRenderables.
		PagingLandScapeRenderableSet mRenderablePool;

		/** Queue to batch the process of loading the Renderables.
		This avoid the plug-in to load a lot of renderables in a single Frame, 
		dropping the FPS.
		*/
		PagingLandScapeQueue< PagingLandScapeTile > mTilesLoadRenderableQueue;

		unsigned int mRenderablesVisibles;
		unsigned int mLastRenderablesVisibles;
		unsigned int mNumRenderablesIncrement;
		unsigned int mNumRenderableLoading;

		unsigned int mRenderableLoadInterval;
		int  mLoadInterval;

	};

}

#endif
