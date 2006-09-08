/***************************************************************************
  OgrePagingLandScapeIndexBufferManager.h  -  description
  -------------------
  begin                : Fri Feb 28 2003
  copyright            : (C) 2003-2006 by Jose A Milan && Tuan Kuranes
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

#ifndef PAGINGLandScapeINDEXBUFFER_H
#define PAGINGLandScapeINDEXBUFFER_H

#include "OgrePagingLandScapePrerequisites.h"

namespace Ogre
{

    class PagingLandScapeIndexBufferManager
    {
        public:


	        PagingLandScapeIndexBufferManager(PagingLandScapeSceneManager * scnMgr);

	        ~PagingLandScapeIndexBufferManager(void);

	        IndexData *getIndex(const int LOD);

            void load();
            void clear();


            /** Utility method to generate stitching indexes on the edge of a tile
            @param neighbor The neighbor direction to stitch
            @param hiLOD The LOD of this tile
            @param loLOD The LOD of the neighbor
            @param omitFirstTri Whether the first triangle of the stitch (always clockwise
	            relative to the center of this tile) is to be omitted because an 
				adjoining edge is also being stitched
            @param omitLastTri Whether the last triangle of the stitch (always clockwise
				relative to the center of this tile) is to be omitted because an 
				adjoining edge is also being stitched
            @param pIdx Pointer to a pointer to the index buffer to push the results 
				into (this pointer will be updated)
            @returns The number of indexes added
            */
            unsigned int stitchEdge(const Neighbor neighbor, const int hiLOD, const int loLOD, const bool omitFirstTri, const bool omitLastTri, void** ppIdx, const bool is32bits) const;

            /// Gets the index data for this tile based on current settings
            IndexData* getIndexData(const int RenderLevel, PagingLandScapeRenderable** Neighbors);

            /// Internal method for generating triangle list terrain indexes
            IndexData* generateTriListIndexes(const bool northStitch, const bool southStitch, const bool eastStitch, const bool westStitch, const int RenderLevel, PagingLandScapeRenderable** Neighbors) const;

			//*******************
			//Added by Fiesch adapted from a post by tonyhnz
			IndexData*	getRawIndexes(int renderLevel);

            PagingLandScapeSceneManager *getSceneManager(){return mScnMgr;}
        protected:

            PagingLandScapeSceneManager *mScnMgr;

            /** Returns the index into the height array for the given coordinates. */
            inline ushort _index16(int x, int z) const
            {
                return (x + z * mTileSize);
            };       
            /** Returns the index into the height array for the given coordinates. */
            inline unsigned int _index32(int x, int z) const
            {
                return (x + z * mTileSize);
            };       

            unsigned int mTileSize;

            IndexArray mCache;
            /// Shared array of IndexData (reuse indexes across tiles)
            LevelArray mLevelIndex;
	        // Store the indexes for every combination
            unsigned int mNumIndexes;

    };

}

#endif
