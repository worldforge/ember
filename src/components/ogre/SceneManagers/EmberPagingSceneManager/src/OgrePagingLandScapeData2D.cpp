/***************************************************************************
OgrePagingLandScapeData2D.cpp  -  description
-------------------
begin                : Wen Mar 06 2003
copyright            : (C) 2003-2005 by Jose A Milan && Tuan Kuranes
email                : spoke@supercable.es & tuan.kuranes@free.fr
***************************************************************************/

/***************************************************************************
*                                                                         *
*   This program is free software; you can redistribute it and/or modify  *
*   it under the terms of the GNU Lesser General Public License as        *
*   published by the Free Software Foundation; either version 2 of the    *
*   License, or (at your option) any later version.                       *
*                                                                         *
***************************************************************************/

#include "OgreVector3.h"
#include "OgreColourValue.h"

#include "OgreSphere.h"

#include "OgrePagingLandScapeData2DManager.h"
#include "OgrePagingLandScapeData2D.h"
#include "OgrePagingLandScapeOptions.h"


#ifndef _MAPSPLITTER

#include "OgreAxisAlignedBox.h"
#include "OgreMovableObject.h"
#include "OgreSimpleRenderable.h"


#include "OgrePagingLandScapePageManager.h"
#include "OgrePagingLandScapeTile.h"
#include "OgrePagingLandScapeRenderable.h"

#endif// _MAPSPLITTER

namespace Ogre
{
    PagingLandScapeData2D::PagingLandScapeData2D():
        mHeightData (0),
        mIsLoaded (false),
        mIsModified (false),
        mIsRectModified (false),
        mRect (0, 0, 0, 0, 0, 1)
    {
    }
    //-----------------------------------------------------------------------
    PagingLandScapeData2D::~PagingLandScapeData2D()
    {    
    }
    //-----------------------------------------------------------------------
    void PagingLandScapeData2D::init()
    {   
       //
    } 
    //-----------------------------------------------------------------------
    void PagingLandScapeData2D::uninit()
    {    
        PagingLandScapeData2D::unload ();
    }
    //-----------------------------------------------------------------------
    bool PagingLandScapeData2D::load(const uint mX, const  uint mZ)
    {
    	if (!mIsLoaded)
        {
	        mPageX = static_cast<uint> (mX); 
	        mPageZ = static_cast<uint> (mZ);
        	
	        mSize = PagingLandScapeOptions::getSingleton().PageSize;
        	
            mShiftX = (mPageX * (mSize - 1)) - PagingLandScapeOptions::getSingleton().maxUnScaledX;
            mShiftZ = (mPageZ * (mSize - 1)) - PagingLandScapeOptions::getSingleton().maxUnScaledZ;
    	
	        const bool isLoadable = _load (mX, mZ);
	        mIsLoaded = true;  
            mIsModified = false;
            mIsRectModified = false;
            return isLoadable;
        }
        return true;        
    }
    //-----------------------------------------------------------------------
    void PagingLandScapeData2D::load()
    {	
        if (!mIsLoaded)
        {
	        _load();
	        mIsLoaded = true;
            mIsModified = false;
        }
    }    
    //-----------------------------------------------------------------------
    void PagingLandScapeData2D::unload()
    {
        if (mIsLoaded)
        {
            if (mIsModified)
                _save ();
            delete[] mHeightData;
            mHeightData = 0;
	        _unload();
	        mIsLoaded = false;
            mIsModified = false;
            mIsRectModified = false;
            #ifndef _MAPSPLITTER
                resetDeformationRectangle ();
            #endif
        }
    }
    //-----------------------------------------------------------------------
    bool PagingLandScapeData2D::isLoaded()
    {
	    return mIsLoaded;
    }
     //-----------------------------------------------------------------------
    void PagingLandScapeData2D::computePowerof2PlusOneSize()
    {
	    mSize = PagingLandScapeOptions::getSingleton().PageSize;
        const size_t p2size = mSize - 1;

        size_t pagex = mXDimension / p2size;
        if (mXDimension - (pagex * p2size) > 1)
            pagex++;
        mXDimension = pagex * p2size + 1;

        size_t pagez = mZDimension / p2size;
        if (mZDimension - (pagez * p2size) > 1)
            pagez++;
        mZDimension =  pagez * p2size + 1;
    }
    //-----------------------------------------------------------------------
    bool PagingLandScapeData2D::_checkSize( const size_t s )
    {
        // ispow2 - 1
        const int p = static_cast <uint> (s - 1); 
        // ispow2
        return ((p & (p - 1)) == 0);
    }
    //-----------------------------------------------------------------------
    const Real PagingLandScapeData2D::getHeightAbsolute(const Real x, const Real z)
    {   
        const Vector3 scale = PagingLandScapeOptions::getSingleton().scale;
	
        // adjust x and z to be local to page
        const int i_x = static_cast<int> (x / scale.x - mShiftX);
        const int i_z = static_cast<int> (z / scale.z - mShiftZ);

        // due to Real imprecision on Reals, we have to use boundaries here
        // otherwise we'll hit asserts.
        const int size =  static_cast<int> (mSize-1);
        const uint u_x = static_cast<uint> (std::max(std::min (i_x, size), 0));
        const uint u_z = static_cast<uint> (std::max(std::min (i_z, size), 0));
	
        const size_t arraypos = u_z * mSize + u_x; 
        assert (mHeightData && arraypos < mMaxArrayPos);
	    return mHeightData[arraypos];
    }

#ifndef _MAPSPLITTER
    //-----------------------------------------------------------------------
    void PagingLandScapeData2D::resetDeformationRectangle ()
    {
        mRect.left = 0;
        mRect.right = 0;
        mRect.top = 0;
        mRect.bottom = 0;
        mIsRectModified = false;
    }
    //-----------------------------------------------------------------------
    Image::Box PagingLandScapeData2D::getDeformationRectangle ()
    {
        return mRect;
    }
    //-----------------------------------------------------------------------
    void PagingLandScapeData2D::adjustDeformationRectangle(const uint x, const uint z)
    {
        if (mIsRectModified)
        {
            if (mRect.left > x)
                mRect.left = x;
            if (mRect.right < x)
                mRect.right = x;

            if (mRect.top > z)
                mRect.top = z;
            if (mRect.bottom < z)
                mRect.bottom = z;
        }
        else
        {
            // first modification :
            // deformation rectangle is the point
            mRect.left = x;
            mRect.right = x;
            mRect.top = z;
            mRect.bottom = z;
            mIsRectModified = true;
            mIsModified = true;
        }
    }
    //-----------------------------------------------------------------------
    const bool PagingLandScapeData2D::DeformHeight(const Vector3 &deformationPoint,
                                                   Real &modificationHeight)
    { 
        // adjust x and z to be local to page
        const int x = static_cast<int> (deformationPoint.x - mShiftX);
        const int z = static_cast<int> (deformationPoint.z - mShiftZ);
        // due to Real imprecision on Reals, we have to use boundaries here
        // otherwise we'll hit asserts.
        const int size =  static_cast<int> (mSize-1);
        const uint ux = static_cast<uint> (std::max(std::min (x, size), 0));
        const uint uz = static_cast<uint> (std::max(std::min (z, size), 0));
	    return DeformHeight(ux, uz, modificationHeight);
    } 
    //-----------------------------------------------------------------------
    const bool PagingLandScapeData2D::DeformHeight(const uint x,
                                                    const uint z,
                                                    Real &modificationHeight)
    {  
        const uint arraypos = z * mSize + x;
        assert (mHeightData && arraypos < mMaxArrayPos);

        const Real maxH = PagingLandScapeData2DManager::getSingleton ().getMaxHeight();
        const Real newH = mHeightData[arraypos] - modificationHeight;

        bool did_modif = false;
	    if (newH <= 0.0f)
        {
            if (mHeightData[arraypos] != 0.0f)
                did_modif = setHeight(x, z, arraypos, 0.0f);
            modificationHeight = 0.0f;
        }
        else if (newH >= maxH)  
        {
            if (mHeightData[arraypos] != maxH)
                did_modif = setHeight(x, z, arraypos, maxH);
            modificationHeight = maxH;
        }
	    else 
        {
           did_modif = setHeight(x, z, arraypos, newH);
            modificationHeight = newH;
        }
        return did_modif;

    }   
    //-----------------------------------------------------------------------
    bool PagingLandScapeData2D::setHeight( const uint x, const uint z,
                                           const uint Pos, const Real h )
    {
        if (mHeightData[Pos] != h)
        {
	        mHeightData[ Pos ] = h;    

		    uint tileposx = x;
		    uint tileposz = z;
		// Make position local to tiles.
		// and return a Tile.
	        PagingLandScapeTile *t = 
		    PagingLandScapePageManager::getSingleton().getTilePage(tileposx, tileposz, 
									mPageX, mPageZ);
	        if (t && t->isLoaded ())
	        { 
		        // tells what tile portion needs to be updated.
	            PagingLandScapeRenderable * const r = t->getRenderable ();                
		        r->adjustDeformationRectangle (tileposx, tileposz); 
        		
		        const uint tSize = PagingLandScapeOptions::getSingleton ().TileSize - 1;
		        const uint NumTiles = PagingLandScapeOptions::getSingleton ().NumTiles;

                const PagingLandScapeTileInfo * const info =  t->getInfo();
                const uint tX = info->tileX;
                const uint tZ = info->tileZ;

                // If we're on a page edge, we must duplicate the change on the 
                // neighbour tile (if it has one...)
                // could be a direct neighbour or a diagonal one (in a corner.)
                const bool left  = (tileposx == 0     && tX != 0);
                const bool right = (tileposx == tSize && tX != NumTiles - 1);
                const bool down  = (tileposz == 0     && tZ != 0);
                const bool up    = (tileposz == tSize && tZ != NumTiles - 1);


		        if (left)
		        { 
			        PagingLandScapeRenderable * const rn = r->_getNeighbor(WEST);
                    if (rn)
                    {
                        if (down)
                        {
                            PagingLandScapeRenderable * const rnUp = rn->_getNeighbor(NORTH);
                            if (rnUp && rnUp->isLoaded ())
	                        { 
                                rnUp->adjustDeformationRectangle (tSize, tSize); 
                            }
                        }
                        else  if (up)
                        {
                            PagingLandScapeRenderable * const rnDown = rn->_getNeighbor(SOUTH);
                            if (rnDown && rnDown->isLoaded ())
	                        { 
                                rnDown->adjustDeformationRectangle (tSize, 0); 
                            }
                        }
			            if (rn->isLoaded ())
			            { 
			                rn->adjustDeformationRectangle (tSize, tileposz); 
			            }
                    }
		        }
                else  if (right)
                {
                    PagingLandScapeRenderable * const rn = r->_getNeighbor(EAST);
                    if (rn)
	                { 
                        if (down)
                        {
                            PagingLandScapeRenderable * const rnUp = rn->_getNeighbor(NORTH);
                            if (rnUp && rnUp->isLoaded ())
	                        { 
                                rnUp->adjustDeformationRectangle (0, tSize); 
                            }
                        }
                        else  if (up)
                        {
                            PagingLandScapeRenderable * const rnDown = rn->_getNeighbor(SOUTH);
                            if (rnDown && rnDown->isLoaded ())
	                        { 
                                rnDown->adjustDeformationRectangle (0, 0); 
                            }
                        }
                        if (rn)
	                    { 
                            rn->adjustDeformationRectangle (0, tileposz); 
                        }
                    }
                }
                if (down)
                {
                    PagingLandScapeRenderable * const rn = r->_getNeighbor(NORTH);
                    if (rn && rn->isLoaded ())
	                { 
                        rn->adjustDeformationRectangle (tileposx, tSize); 
                    }
                }
                else  if (up)
                {
                    PagingLandScapeRenderable * const rn = r->_getNeighbor(SOUTH);
                    if (rn && rn->isLoaded ())
	                { 
                        rn->adjustDeformationRectangle (tileposx, 0); 
                    }
                }
	        }
    	
	        adjustDeformationRectangle (x, z);
            return true;
        }
       return false;
    }
    //-----------------------------------------------------------------------
    void PagingLandScapeData2D::setHeight( const uint x, const uint z, 
					                       const Real h  )
    {       
        const uint Pos = ( z * mSize )+ x;
        assert  (mHeightData &&  mMaxArrayPos > Pos ); 
        setHeight( x, z, Pos, h);
    }
#endif //_MAPSPLITTER
    //-----------------------------------------------------------------------
    const Vector3 PagingLandScapeData2D::getNormal (const Real mX, const Real mZ)
    {
        // First General method : (9 adds and 6 muls + a normalization)
        //        *---v3--*
        //        |   |   |
        //        |   |   |
        //        v1--X--v2
        //        |   |   |
        //        |   |   |
        //        *---v4--*
        //
        //        U = v2 - v1;
        //        V = v4 - v3;
        //        N = Cross(U, V);
        //        N.normalise;
        //
        // BUT IN CASE OF A HEIGHTMAP :
        //
        //   if you do some math by hand before you code,
        //   you can see that N is immediately given by
        //  Approximation (2 adds and a normalization)
        //
        //        N = Vector3(z[x-1][y] - z[x+1][y], z[x][y-1] - z[x][y+1], 2);
        //        N.normalise();
        //
        // or even using SOBEL operator VERY accurate!
        // (14 adds and a normalization)
        //
        //       N = Vector3 (z[x-1][y-1] + z[x-1][y] + z[x-1][y] + z[x-1][y+1] - z[x+1][y-1] - z[x+1][y] - z[x+1][y] - z[x+1][y+1],
        //                     z[x-1][y-1] + z[x][y-1] + z[x][y-1] + z[x+1][y-1] - z[x-1][y+1] - z[x][y+1] - z[x][y+1] - z[x+1][y+1],
        //                     8);
        //       N.normalize();


        // Fast SOBEL filter

        const size_t pageSize = mSize - 1;

        // the divider make sure we do respect proportion  (height and width proportional to y )
        const Real Divider = static_cast <Real> (pageSize) / PagingLandScapeOptions::getSingleton ().scale.y;

        assert (mHeightData);

        #define  getisIn(a, b) (mHeightData[static_cast<uint> (a) + static_cast<uint> (b) * mSize])

        if ( mX > 0 && mZ > 0 &&
             mX < pageSize && mZ < pageSize)
        {

            //                        Vector3 result (getisIn(x-1,z-1) + getisIn (x-1, z) + getisIn (x-1, z) + getisIn (x-1, z+1) - getisIn (x+1, z-1) - getisIn (x+1, z) - getisIn (x+1, z) - getisIn (x+1, z+1),
            //                                            8.0f,
            //                                        getisIn (x-1, z-1) + getisIn (x, z-1) + getisIn (x, z-1) + getisIn (x+1, z-1) - getisIn (x-1, z+1) - getisIn (x, z+1) - getisIn (x, z+1) - getisIn (x+1, z+1));

                Vector3 result((getisIn (mX - 1 , mZ)     - getisIn (mX + 1 , mZ)) * Divider,
                                2.0f,
                                (getisIn (mX,      mZ - 1) - getisIn (mX     , mZ + 1 )) * Divider);

                result.normalise ();

                return result;
        }
        else
        {   
            uint x = static_cast <uint> (mX);
            uint z = static_cast <uint> (mZ);
            Real a,b,c,d;

            if (x == 0)
            {                
                    a = getisIn (x  , z);
                    b = getisIn (x + 1, z);
            }
            else if (x == pageSize)
            {                
                    a = getisIn (x - 1 , z);
                    b = getisIn (x , z);
            }
            else
            {
                a = getisIn (x - 1 , z);
                b = getisIn (x + 1, z);
            }

            if (z == 0)
            {
                 c = getisIn (x , z);
                 d = getisIn (x , z + 1);
            }
            else if (z == pageSize)
            {
                 c = getisIn (x , z - 1);
                 d = getisIn (x , z);
            }
            else
            {
                c = getisIn (x , z - 1);
                d = getisIn (x , z + 1);
            }


            Vector3 result((a     - b) * Divider,
                            2.0f,
                            (c - d) * Divider);
            result.normalise ();
            return result;

#ifdef _NOWAY
#ifndef _MAPSPLITTER
        #define  getisOut(a, b) (PagingLandScapeData2DManager::getSingleton ().getHeightAtPage (mPageX, mPageZ, static_cast<int> (a), static_cast<int> (b)))

//                       Vector3 result (getisOut(x-1,z-1) + getisOut (x-1, z) + getisOut (x-1, z) + getisOut (x-1, z+1) - getisOut (x+1, z-1) - getisOut (x+1, z) - getisOut (x+1, z) - getisOut (x+1, z+1),
//                                        8.0f,
//                                        getisOut (x-1, z-1) + getisOut (x, z-1) + getisOut (x, z-1) + getisOut (x+1, z-1) - getisOut (x-1, z+1) - getisOut (x, z+1) - getisOut (x, z+1) - getisOut (x+1, z+1));

                Vector3 result((getisOut (mX - 1 , mZ)     - getisOut (mX + 1 , mZ)) * Divider,
                                2.0f,
                                (getisOut (mX,      mZ - 1) - getisOut (mX     , mZ + 1 )) * Divider);

                result.normalise ();


              return result;
#endif //_MAPSPLITTER
#endif //

        }
    }
} //namespace


















