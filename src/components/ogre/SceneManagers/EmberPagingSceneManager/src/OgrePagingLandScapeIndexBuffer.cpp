/***************************************************************************
  OgrePagingLandScapeIndexBuffer.cpp  -  description
  -------------------
  begin                : Fri Feb 28 2003
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

#include "OgreHardwareBufferManager.h"
#include "OgreVertexIndexData.h"

#include "OgreVector3.h"

#include "OgreSimpleRenderable.h"

#include "OgrePagingLandScapeRenderable.h"
#include "OgrePagingLandScapeOptions.h"
#include "OgrePagingLandScapeIndexBuffer.h"

namespace Ogre
{
    //-----------------------------------------------------------------------
    template<> PagingLandScapeIndexBuffer* Singleton<PagingLandScapeIndexBuffer>::ms_Singleton = 0;
    PagingLandScapeIndexBuffer* PagingLandScapeIndexBuffer::getSingletonPtr(void)
    {
	    return ms_Singleton;
    }
    //-----------------------------------------------------------------------
    PagingLandScapeIndexBuffer& PagingLandScapeIndexBuffer::getSingleton(void)
    {  
	    assert( ms_Singleton );  return ( *ms_Singleton );  
    }

    //-----------------------------------------------------------------------
    PagingLandScapeIndexBuffer::PagingLandScapeIndexBuffer( ) :
        mTileSize (0),
        mNumIndexes (0)
    {
      
    }
    //-----------------------------------------------------------------------
    PagingLandScapeIndexBuffer::~PagingLandScapeIndexBuffer()
    {
       clear();
    }
    //-----------------------------------------------------------------------
    void PagingLandScapeIndexBuffer::load()
    {
        const uint tileSize = PagingLandScapeOptions::getSingleton().TileSize;
        const uint numIndexes = PagingLandScapeOptions::getSingleton().maxRenderLevel + 1;
        
        if (tileSize != mTileSize || numIndexes != mNumIndexes)
        {
            clear();

            mLevelIndex.reserve (numIndexes);
            mLevelIndex.resize (numIndexes);
            for ( uint k = 0; k < numIndexes; k++ )
            {
                mLevelIndex[k] = new IndexMap();
            }
           
            mNumIndexes = numIndexes;
            mTileSize = tileSize;
        }
    }
    //-----------------------------------------------------------------------
    void PagingLandScapeIndexBuffer::clear()
    {
        if (!mLevelIndex.empty())
        {
            for (uint i = 0; i < mNumIndexes; i++)
            {
                delete mLevelIndex[i];
            }
            mLevelIndex.clear();
        }
        if (!mCache.empty())
        {
            const size_t cachesize = mCache.size ();
            for(size_t j = 0; j < cachesize; j++)
            {
                delete mCache[j];
            }
            mCache.clear();
        }
    }
    //-----------------------------------------------------------------------
    IndexData *PagingLandScapeIndexBuffer::getIndexData( const int renderLevel,
                                                        PagingLandScapeRenderable **Neighbors )
    {
        assert (renderLevel < static_cast <int> (mNumIndexes));
        assert (renderLevel >= 0) ;

        uint stitchFlags = 0;

        bool eastStitch;
        if ( Neighbors[ EAST ] &&
            Neighbors[ EAST ]->isLoaded () && Neighbors[ EAST ]->isVisible ()  &&
            Neighbors[ EAST ] -> mRenderLevel > renderLevel)
        { 
            assert (Neighbors[ EAST ] -> mRenderLevel < static_cast <int> (mNumIndexes));
            stitchFlags |= STITCH_EAST;
            stitchFlags |= 
                    (Neighbors[ EAST ] -> mRenderLevel - renderLevel) << STITCH_EAST_SHIFT;
            eastStitch = true;
        }
        else
        {
            eastStitch = false;
        }

        bool westStitch;
        if ( Neighbors[ WEST ] &&  
            Neighbors[ WEST ]->isLoaded () && Neighbors[ WEST ]->isVisible ()  &&
            Neighbors[ WEST ] -> mRenderLevel > renderLevel)
        {
            assert (Neighbors[ WEST ] -> mRenderLevel < static_cast <int> (mNumIndexes));
            stitchFlags |= STITCH_WEST;
            stitchFlags |= 
                (Neighbors[ WEST ] -> mRenderLevel - renderLevel) << STITCH_WEST_SHIFT;
            westStitch = true;
        }    
        else
        {
            westStitch = false;
        }

        bool northStitch;
        if ( Neighbors[ NORTH ] && 
            Neighbors[ NORTH ]->isLoaded () && Neighbors[ NORTH ]->isVisible () &&
            Neighbors[ NORTH ] -> mRenderLevel > renderLevel )
        {
            assert (Neighbors[ NORTH ] -> mRenderLevel < static_cast <int> (mNumIndexes));
            stitchFlags |= STITCH_NORTH;
            stitchFlags |= 
                (Neighbors[ NORTH ] -> mRenderLevel - renderLevel) << STITCH_NORTH_SHIFT;
            northStitch = true;
        }    
        else
        {
            northStitch = false;
        }

        bool southStitch;
        if (Neighbors[ SOUTH ] && 
            Neighbors[ SOUTH ]->isLoaded () && Neighbors[ SOUTH ]->isVisible () &&
            Neighbors[ SOUTH ] -> mRenderLevel > renderLevel )
        {
            assert (Neighbors[ SOUTH ] -> mRenderLevel < static_cast <int> (mNumIndexes));
            stitchFlags |= STITCH_SOUTH;
            stitchFlags |= 
                (Neighbors[ SOUTH ] -> mRenderLevel - renderLevel) << STITCH_SOUTH_SHIFT;
            southStitch = true;
        }    
        else
        {
            southStitch = false;
        }


        // Check preexisting
        assert (mLevelIndex.size() > renderLevel);
        IndexMap::iterator ii = mLevelIndex[ renderLevel ]->find( stitchFlags );
        if ( ii == mLevelIndex[ renderLevel ]->end())
        {
            // Create
            IndexData* indexData = generateTriListIndexes(northStitch, 
                                                        southStitch,
                                                        eastStitch,
                                                        westStitch, 
                                                        renderLevel,
                                                        Neighbors);
            mLevelIndex[ renderLevel ]->insert(
                IndexMap::value_type(stitchFlags, indexData));
            //indexData->optimiseVertexCacheTriList();
            return indexData;
        }
        else
        {
            return  ii->second;
        }

    }

    //-----------------------------------------------------------------------
    IndexData* PagingLandScapeIndexBuffer::generateTriListIndexes(const bool northStitch, 
                                                                  const bool southStitch,
                                                                  const bool eastStitch,
                                                                  const bool westStitch,
                                                                const int RenderLevel,
                                                                PagingLandScapeRenderable **Neighbors)
    {
        const int step = 1 << RenderLevel;
        assert (step > 0);


        const uint tileSize = mTileSize;
        const uint new_length = ( tileSize / step ) * ( tileSize / step ) * 2 * 2 * 2;
        
        IndexData* indexData = new IndexData();
        assert (indexData);

        const bool is32bits = (tileSize * tileSize) > 65535  ? true : false;      
        indexData->indexBuffer = 
            HardwareBufferManager::getSingleton().createIndexBuffer(
                                        (is32bits)  ? HardwareIndexBuffer::IT_32BIT : HardwareIndexBuffer::IT_16BIT,
                                        new_length, 
                                        HardwareBuffer::HBU_STATIC_WRITE_ONLY);

        mCache.push_back( indexData );


        
        const uint north = northStitch ? step : 0;
        const uint south = southStitch ? step : 0;
        const uint east = eastStitch ? step : 0;
        const uint west = westStitch ? step : 0;


        uint numIndexes = 0;
        const uint step_offset = step * tileSize;
        uint height_count = north * tileSize;
        void *idx;
        if (is32bits)
        {
             /** Returns the index into the height array for the given coordinates. */
            uint* pIdx = static_cast<uint*>(indexData->indexBuffer->lock(0, 
                                                indexData->indexBuffer->getSizeInBytes(), 
                                                HardwareBuffer::HBL_DISCARD));
            assert (pIdx);
            for (uint j = north; j < tileSize - 1 - south; j += step )
            {
                for (uint i = west; i <  tileSize - 1 - east; i += step )
                {
                    //triangles
                    assert ((i + step + height_count + step_offset) < (tileSize * tileSize));


                    *pIdx++ = static_cast<uint> (i        + height_count);                 numIndexes++;      
                    *pIdx++ = static_cast<uint> (i        + height_count + step_offset);   numIndexes++; 
                    *pIdx++ = static_cast<uint> (i + step + height_count);                 numIndexes++; 
                    
                    *pIdx++ = static_cast<uint> (i        + height_count + step_offset);   numIndexes++;    
                    *pIdx++ = static_cast<uint> (i + step + height_count + step_offset);   numIndexes++; 
                    *pIdx++ = static_cast<uint> (i + step + height_count);                 numIndexes++; 
                }
                height_count += step_offset;
            }  
            idx = pIdx;
        }
        else
        {            
            /** Returns the index into the height array for the given coordinates. */
            ushort* pIdx = static_cast<ushort*>(indexData->indexBuffer->lock(0, 
                                                indexData->indexBuffer->getSizeInBytes(), 
                                                HardwareBuffer::HBL_DISCARD));
            assert (pIdx);
            for (uint j = north; j < tileSize - 1 - south; j += step )
            {
                for (uint i = west; i <  tileSize - 1 - east; i += step )
                {
                    //triangles
                    assert ((i + step + height_count + step_offset) < (tileSize * tileSize));


                    *pIdx++ = static_cast<ushort> (i        + height_count);                 numIndexes++;      
                    *pIdx++ = static_cast<ushort> (i        + height_count + step_offset);   numIndexes++; 
                    *pIdx++ = static_cast<ushort> (i + step + height_count);                 numIndexes++; 
                    
                    *pIdx++ = static_cast<ushort> (i        + height_count + step_offset);   numIndexes++;    
                    *pIdx++ = static_cast<ushort> (i + step + height_count + step_offset);   numIndexes++; 
                    *pIdx++ = static_cast<ushort> (i + step + height_count);                 numIndexes++; 
                }
                height_count += step_offset;
            }
            idx = pIdx;
        }
        assert (numIndexes < new_length);

        // North stitching
        assert (RenderLevel < static_cast <int> (mNumIndexes));
        if ( northStitch )
        {
            assert (Neighbors[ NORTH ]);
            assert (Neighbors[ NORTH ]->isLoaded ());
            assert (Neighbors[ NORTH ]->isVisible ());
            assert (Neighbors[ NORTH ] -> mRenderLevel > RenderLevel);
            assert (Neighbors[ NORTH ] -> mRenderLevel < static_cast <int> (mNumIndexes));
            numIndexes += stitchEdge(NORTH, RenderLevel, Neighbors[NORTH]->mRenderLevel,
                westStitch , eastStitch , &idx, is32bits);
        }
        // East stitching
        if ( eastStitch )
        {
            assert (Neighbors[ EAST ]);
            assert (Neighbors[ EAST ]->isLoaded ());
            assert (Neighbors[ EAST ]->isVisible ());
            assert (Neighbors[ EAST ] -> mRenderLevel > RenderLevel);
            assert (Neighbors[ EAST ] -> mRenderLevel < static_cast <int> (mNumIndexes));
            numIndexes += stitchEdge(EAST, RenderLevel, Neighbors[EAST]->mRenderLevel,
                northStitch, southStitch, &idx, is32bits);
        }
        // South stitching
        if ( southStitch )
        {
            assert (Neighbors[ SOUTH ]);
            assert (Neighbors[ SOUTH ]->isLoaded ());
            assert (Neighbors[ SOUTH ]->isVisible ());
            assert (Neighbors[ SOUTH ] -> mRenderLevel > RenderLevel);
            assert (Neighbors[ SOUTH ] -> mRenderLevel < static_cast <int> (mNumIndexes));
            numIndexes += stitchEdge(SOUTH, RenderLevel, Neighbors[SOUTH]->mRenderLevel,
                eastStitch , westStitch, &idx, is32bits);
        }
        // West stitching
        if ( westStitch )
        {
            assert (Neighbors[ WEST ]);
            assert (Neighbors[ WEST ]->isLoaded ());
            assert (Neighbors[ WEST ]->isVisible ());
            assert (Neighbors[ WEST ] -> mRenderLevel > RenderLevel);
            assert (Neighbors[ WEST ] -> mRenderLevel < static_cast <int> (mNumIndexes));
            numIndexes += stitchEdge(WEST, RenderLevel, Neighbors[WEST]->mRenderLevel,
                southStitch , northStitch, &idx, is32bits);
        }

        assert (numIndexes < new_length);

        indexData->indexBuffer->unlock();
        indexData->indexCount = numIndexes;
        indexData->indexStart = 0;

        return indexData;
    }
    //-----------------------------------------------------------------------
    uint PagingLandScapeIndexBuffer::stitchEdge(const Neighbor neighbor, 
                                                const int hiLOD, const int loLOD, 
                                                const bool omitFirstTri, const bool omitLastTri, 
                                                void** ppIdx, 
                                                const bool is32bits) const
    {
        assert(loLOD > hiLOD);
        /* 
        Now do the stitching; we can stitch from any level to any level.
        The stitch pattern is like this for each pair of vertices in the lower LOD
        (excuse the poor ascii art):

        lower LOD
        *-----------*
        |\  \ 3 /  /|
        |1\2 \ / 4/5|
        *--*--*--*--*
        higher LOD

        The algorithm is, for each pair of lower LOD vertices:
        1. Iterate over the higher LOD vertices, generating tris connected to the 
        first lower LOD vertex, up to and including 1/2 the span of the lower LOD 
        over the higher LOD (tris 1-2). Skip the first tri if it is on the edge 
        of the tile and that edge is to be stitched itself.
        2. Generate a single tri for the middle using the 2 lower LOD vertices and 
        the middle vertex of the higher LOD (tri 3). 
        3. Iterate over the higher LOD vertices from 1/2 the span of the lower LOD
        to the end, generating tris connected to the second lower LOD vertex 
        (tris 4-5). Skip the last tri if it is on the edge of a tile and that
        edge is to be stitched itself.

        The same algorithm works for all edges of the patch; stitching is done
        clockwise so that the origin and steps used change, but the general
        approach does not.
        */


        // Work out the steps ie how to increment indexes
        // Step from one vertex to another in the high detail version
        int step = 1 << hiLOD;
        // Step from one vertex to another in the low detail version
        int superstep = 1 << loLOD;
        // Step half way between low detail steps
        int halfsuperstep = superstep >> 1;

        // Work out the starting points and sign of increments
        // We always work the strip clockwise
        int startx, starty, endx, rowstep;
        bool horizontal;
        
        const int tileSize = static_cast <int> (mTileSize - 1);
        switch (neighbor)
        {
            case NORTH:
                startx = starty = 0;
                endx =  tileSize;
                rowstep = step;
                horizontal = true;
                break;
            case SOUTH:
                // invert x AND y direction, helps to keep same winding
                startx = starty = tileSize;
                endx = 0;
                rowstep = -step;
                step = -step;
                superstep = -superstep;
                halfsuperstep = -halfsuperstep;
                horizontal = true;
                break;
            case EAST:
                startx = 0;
                endx = tileSize;
                starty = tileSize;
                rowstep = -step;
                horizontal = false;
                break;
            case WEST:
                startx = tileSize;
                endx = 0;
                starty = 0;
                rowstep = step;
                step = -step;
                superstep = -superstep;
                halfsuperstep = -halfsuperstep;
                horizontal = false;
                break;
	        default:
                assert (0);
		    break;
        };
        assert (*ppIdx);
        uint numIndexes = 0;
        const bool isHorizontal = horizontal;
        if (is32bits)
        {
            // Get pointer to be updated
            uint* pIdx = static_cast <uint*> (*ppIdx);
            for ( int j = startx; j != endx; j += superstep )
            {
                int k; 
                for (k = 0; k != halfsuperstep; k += step)
                {
                    int jk = j + k;
                    //skip the first bit of the corner?
                    if ( j != startx || k != 0 || !omitFirstTri )
                    {
                        if (isHorizontal)
                        {
                            *pIdx++ = _index32( j , starty ); numIndexes++;
                            *pIdx++ = _index32( jk, starty + rowstep ); numIndexes++;
                            *pIdx++ = _index32( jk + step, starty + rowstep ); numIndexes++;
                        }
                        else
                        {
                            *pIdx++ = _index32( starty, j ); numIndexes++;
                            *pIdx++ = _index32( starty + rowstep, jk ); numIndexes++;
                            *pIdx++ = _index32( starty + rowstep, jk + step); numIndexes++;
                        }
                    }
                }

                // Middle tri
                if (isHorizontal)
                {
                    *pIdx++ = _index32( j, starty ); numIndexes++;
                    *pIdx++ = _index32( j + halfsuperstep, starty + rowstep); numIndexes++;
                    *pIdx++ = _index32( j + superstep, starty ); numIndexes++;
                }
                else
                {
                    *pIdx++ = _index32( starty, j ); numIndexes++;
                    *pIdx++ = _index32( starty + rowstep, j + halfsuperstep ); numIndexes++;
                    *pIdx++ = _index32( starty, j + superstep ); numIndexes++;
                }

                for (k = halfsuperstep; k != superstep; k += step)
                {
                    int jk = j + k;
                    if ( j != endx - superstep || k != superstep - step || !omitLastTri )
                    {
                        if (isHorizontal)
                        {
                            *pIdx++ = _index32( j + superstep, starty ); numIndexes++;
                            *pIdx++ = _index32( jk, starty + rowstep ); numIndexes++;
                            *pIdx++ = _index32( jk + step, starty + rowstep ); numIndexes++;
                        }
                        else
                        {
                            *pIdx++ = _index32( starty, j + superstep ); numIndexes++;
                            *pIdx++ = _index32( starty + rowstep, jk ); numIndexes++;
                            *pIdx++ = _index32( starty + rowstep, jk + step ); numIndexes++;
                        }
                    }
                }
            }
            *ppIdx = pIdx;
        }
        else
        {
            // Get pointer to be updated
            ushort* pIdx = static_cast <ushort*> (*ppIdx);
            for ( int j = startx; j != endx; j += superstep )
            {
                int k; 
                for (k = 0; k != halfsuperstep; k += step)
                {
                    int jk = j + k;
                    //skip the first bit of the corner?
                    if ( j != startx || k != 0 || !omitFirstTri )
                    {
                        if (isHorizontal)
                        {
                            *pIdx++ = _index16( j , starty ); numIndexes++;
                            *pIdx++ = _index16( jk, starty + rowstep ); numIndexes++;
                            *pIdx++ = _index16( jk + step, starty + rowstep ); numIndexes++;
                        }
                        else
                        {
                            *pIdx++ = _index16( starty, j ); numIndexes++;
                            *pIdx++ = _index16( starty + rowstep, jk ); numIndexes++;
                            *pIdx++ = _index16( starty + rowstep, jk + step); numIndexes++;
                        }
                    }
                }

                // Middle tri
                if (isHorizontal)
                {
                    *pIdx++ = _index16( j, starty ); numIndexes++;
                    *pIdx++ = _index16( j + halfsuperstep, starty + rowstep); numIndexes++;
                    *pIdx++ = _index16( j + superstep, starty ); numIndexes++;
                }
                else
                {
                    *pIdx++ = _index16( starty, j ); numIndexes++;
                    *pIdx++ = _index16( starty + rowstep, j + halfsuperstep ); numIndexes++;
                    *pIdx++ = _index16( starty, j + superstep ); numIndexes++;
                }

                for (k = halfsuperstep; k != superstep; k += step)
                {
                    int jk = j + k;
                    if ( j != endx - superstep || k != superstep - step || !omitLastTri )
                    {
                        if (isHorizontal)
                        {
                            *pIdx++ = _index16( j + superstep, starty ); numIndexes++;
                            *pIdx++ = _index16( jk, starty + rowstep ); numIndexes++;
                            *pIdx++ = _index16( jk + step, starty + rowstep ); numIndexes++;
                        }
                        else
                        {
                            *pIdx++ = _index16( starty, j + superstep ); numIndexes++;
                            *pIdx++ = _index16( starty + rowstep, jk ); numIndexes++;
                            *pIdx++ = _index16( starty + rowstep, jk + step ); numIndexes++;
                        }
                    }
                }
            }
            *ppIdx = pIdx;
        }
        return numIndexes;
    }

} //namespace
