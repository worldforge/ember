/***************************************************************************
	OgrePagingLandScapeTileManager.h  -  description
	-------------------
  begin                : Mon Jun 16 2003
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

#ifndef PAGINGLandScapeTILEMANAGER_H
#define PAGINGLandScapeTILEMANAGER_H

#include "OgrePagingLandScapePrerequisites.h"
#include "OgrePagingLandScapeQueue.h"

namespace Ogre
{


    class PagingLandScapeTileManager 
    {
        public:

	        PagingLandScapeTileManager(PagingLandScapeSceneManager * scnMgr);

	        virtual ~PagingLandScapeTileManager(void);

	        /** Retrieve a free tile.
	        */
	        PagingLandScapeTile* getTile(void);

	        /** Make a tile free.
	        */
	        void freeTile(PagingLandScapeTile* tile);

            void reset(void);

	        unsigned int numTiles(void) const;

	        size_t numFree(void) const;


            void load(void);
            void clear(void);
	
			// unload invisible tiles after option::mTileInvisibleUnloadFrames tick
			// without being visible.
			void updateLoadedTiles();

            inline PagingLandScapeOptions*		getOptions()
			{
				return mOptions;
			}

            inline PagingLandScapeSceneManager* getSceneManager()
			{
				return mSceneManager;
			}

    protected:

            PagingLandScapeOptions*		mOptions;
            PagingLandScapeSceneManager *mSceneManager;

	        void _addBatch(const unsigned int num);

	        PagingLandScapeTileRow mTiles;

	        PagingLandScapeQueue< PagingLandScapeTile > mQueue;

	        unsigned int mNumTiles;

    };

} //namespace

#endif
