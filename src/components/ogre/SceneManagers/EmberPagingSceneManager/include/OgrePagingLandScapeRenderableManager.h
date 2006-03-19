/***************************************************************************
  OgrePagingLandScapeRenderableManager.h  -  description
  -------------------
	begin                : Mon Jun 16 2003
	copyright            : (C) 2003-2005 by Jose A Milan && Tuan Kuranes
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

namespace Ogre
{
    /** Class to manage the creation, destruction and use of PagingLandScapeRenderables.
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

	        uint numRenderables(void) const;
	        int numFree(void) const;
	        int numLoading(void) const;

            void InitTextureCoordinatesBuffers(void);
            void freeTextureCoordinatesBuffers(void);
            HardwareVertexBufferSharedPtr getTextureCoordinatesBuffers(const uint tilex, const uint tilez);
            void setTextureCoordinatesBuffers(const uint tilex, const uint tilez, const HardwareVertexBufferSharedPtr& data);

            int PagingLandScapeRenderableManager::numVisibles(void) 
            {
                return mRenderablesVisibles;
            };
            void resetVisibles(void)
			{
				mRenderablesVisibles = 0;
			};
            void addVisible(void)
			{
				++mRenderablesVisibles;
			};


            void load(void);
            void clear(void);

			/// unload some Tiles/renderables if no more in use 
			void processTileUnload();

            PagingLandScapeOptions*		getOptions(){return mOptions;}
            PagingLandScapeSceneManager* getSceneManager(){return mSceneManager;}
    protected:

            PagingLandScapeOptions*		mOptions;
            PagingLandScapeSceneManager *mSceneManager;


	        void _addBatch(const uint num);


	        PagingLandScapeRenderableVector mRenderables;

	        PagingLandScapeQueue< PagingLandScapeRenderable > mQueue;

	        /** Queue to batch the process of loading the Renderables.
		        This avoid the plug-in to load a lot of renderables in a single Frame, 
                dropping the FPS.
	        */
            PagingLandScapeQueue< PagingLandScapeTile > mTilesLoadRenderableQueue;

            uint mRenderablesVisibles;
	        uint mNumRenderables; 
            uint mNumRenderablesIncrement;
            uint mNumRenderableLoading;

			uint mRenderableLoadInterval;
			int  mLoadInterval;
    };

}

#endif
