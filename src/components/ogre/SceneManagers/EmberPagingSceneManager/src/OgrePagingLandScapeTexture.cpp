/***************************************************************************
OgrePagingLandScapeTexture.cpp  -  description
-------------------
begin                : Fri Apr 16 2004
copyright            : (C) 2003-2005 by Jose A Milan && Tuan Kuranes
email                : spoke@supercable.es & tuan.kuranes@free.fr
***************************************************************************/

/***************************************************************************
*                                                                         *
*   This program is free software; you can redistribute it and/or modify  *
*   it under the terms of the GNU Lesser General Public License as published by  *
*   the Free Software Foundation; either version 2 of the License, or     *
*   (at your option) any later version.                                   *
*                                                                         *
***************************************************************************/

#include "OgreVector3.h"
#include "OgreColourValue.h"

#include "OgreStringConverter.h"
#include "OgreMaterialManager.h"
#include "OgreTextureManager.h"
#include "OgreTechnique.h"
#include "OgrePass.h"

#include "OgrePagingLandScapeTexture.h"

namespace Ogre
{
    //-----------------------------------------------------------------------
    PagingLandScapeTexture::PagingLandScapeTexture() :
	    mIsLoaded (false),
        mIsModified (false),
	    mDataX (0),
	    mDataZ (0),
        mNumChannel(0),
        mPaintRect (0, 0, 0, 0, 0, 1),
        mIsPaintRectModified(false),
        mDeformRect (0, 0, 0, 0, 0, 1),
        mIsDeformRectModified(false)
    {
        mMaterial.setNull();
    }

    //-----------------------------------------------------------------------
    PagingLandScapeTexture::~PagingLandScapeTexture()
    {
    }
    //-----------------------------------------------------------------------
    void PagingLandScapeTexture::load(uint mX, uint mZ)
    {
        if (!mIsLoaded)
        {
	        mDataX = mX;
	        mDataZ = mZ;
	        _loadMaterial();
	        mMaterial->setReceiveShadows(true);
	        mIsLoaded = true;
            mIsModified = false; 
            updated();
        }
    }
    //-----------------------------------------------------------------------
    void PagingLandScapeTexture::unload()
    {
        if (mIsLoaded)
        {
	        _unloadMaterial();

            assert (!mMaterial.isNull() && "PagingLandScapeTexture::unload");      
	        mMaterial->unload();
            mMaterial.setNull();

	        mIsLoaded = false;
            mIsModified = false; 
            updated();
        }
    }
    //-----------------------------------------------------------------------
    bool PagingLandScapeTexture::isLoaded() const
    {
	    return mIsLoaded;
    }
    //-----------------------------------------------------------------------
    const MaterialPtr&  PagingLandScapeTexture::getMaterial() const
    {
	    return mMaterial;
    }
    //-----------------------------------------------------------------------
    void PagingLandScapeTexture::updated ()
    {
        mPaintRect.left = 0;
        mPaintRect.right = 0;
        mPaintRect.top = 0;
        mPaintRect.bottom = 0;
        mIsPaintRectModified = false;

        mDeformRect.left = 0;
        mDeformRect.right = 0;
        mDeformRect.top = 0;
        mDeformRect.bottom = 0;
        mIsDeformRectModified = false;
    }
    //-----------------------------------------------------------------------
    bool PagingLandScapeTexture::needUpdate () const
    {
        return mIsDeformRectModified || mIsPaintRectModified;
    } 
    //-----------------------------------------------------------------------
	void PagingLandScapeTexture::adjustDeformationRectangle(uint x, uint z)
    {
        
        if (mIsDeformRectModified)
        {
            if (mDeformRect.left > x)
                mDeformRect.left = x;
            if (mDeformRect.right < x)
                mDeformRect.right = x;

            if (mDeformRect.top > z)
                mDeformRect.top = z;
            if (mDeformRect.bottom < z)
                mDeformRect.bottom = z;
        }
        else
        {
            // first modification :
            // deformation rectangle is the point
            mDeformRect.left = x;
            mDeformRect.right = x;
            mDeformRect.top = z;
            mDeformRect.bottom = z;
            mIsDeformRectModified = true;
        }
    } 
    //-----------------------------------------------------------------------
	void PagingLandScapeTexture::adjustPaintRectangle(uint x, uint z)
    {
        
        if (mIsPaintRectModified)
        {
            if (mPaintRect.left > x)
                mPaintRect.left = x;
            if (mPaintRect.right < x)
                mPaintRect.right = x;

            if (mPaintRect.top > z)
                mPaintRect.top = z;
            if (mPaintRect.bottom < z)
                mPaintRect.bottom = z;
        }
        else
        {
            // first modification :
            // deformation rectangle is the point
            mPaintRect.left = x;
            mPaintRect.right = x;
            mPaintRect.top = z;
            mPaintRect.bottom = z;
            mIsPaintRectModified = true;
        }
    }
} //namespace
