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

#include "OgrePagingLandScapeOptions.h"
#include "OgrePagingLandScapeSceneManager.h"
#include "OgrePagingLandScapeTextureManager.h"
#include "OgrePagingLandScapeData2DManager.h"
#include "OgrePagingLandScapeData2D.h"

namespace Ogre
{
    //-----------------------------------------------------------------------
    PagingLandScapeTexture::PagingLandScapeTexture(PagingLandScapeTextureManager *textureMgr, bool supportModif) :
        mParent(textureMgr),
	    mIsLoaded (false),
        mIsModified (false),
	    mDataX (0),
	    mDataZ (0),
        mNumChannel(0),
        mPaintRect (0, 0, 0, 0, 0, 1),
        mIsPaintRectModified(false),
        mDeformRect (0, 0, 0, 0, 0, 1),
        mIsDeformRectModified(false),
        mSupportModif(supportModif)
    {
        mMaterial.setNull();
    }

    //-----------------------------------------------------------------------
    PagingLandScapeTexture::~PagingLandScapeTexture()
    {
    }
    //-----------------------------------------------------------------------
    const String &PagingLandScapeTexture::getMaterialName()
    {
        return mMaterial->getName ();
    }
    //-----------------------------------------------------------------------
    void PagingLandScapeTexture::bindCompressionSettings(GpuProgramParametersSharedPtr params)
    {
        GpuProgramParameters::AutoConstantIterator aci = params->getAutoConstantIterator();
        bool found = false;
        while (aci.hasMoreElements())
        {
            const GpuProgramParameters::AutoConstantEntry& ace = aci.getNext();
            if (ace.paramType == GpuProgramParameters::ACT_CUSTOM && 
                ace.data == MORPH_CUSTOM_PARAM_ID)
            {
                found = true;
            }
        }
        if (!found)
        {                        
            params->setNamedAutoConstant("compressionSettings", 
                GpuProgramParameters::ACT_CUSTOM, MORPH_CUSTOM_PARAM_ID);                       
        }
    }
    //-----------------------------------------------------------------------
    void PagingLandScapeTexture::load(uint x, uint z)
    {
        if (!mIsLoaded)
        {
	        mDataX = x;
			mDataZ = z;

			updated();
	        _loadMaterial();

	        mMaterial->setReceiveShadows(true);
	        mIsLoaded = true;
            mIsModified = false; 
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
    void PagingLandScapeTexture::compute(PagingLandScapeData2D* data, 
        const Image::Box& dataRect,
        const Image::Box& textureRect)
    {

        const size_t heightfiledsize = mParent->getOptions()->PageSize - 1;
        const Real textureScale = mParent->getOptions()->TextureStretchFactor;
        const uint textureSize = heightfiledsize * textureScale;
        size_t curr_image_pos = textureRect.top*textureSize + textureRect.left;
        const size_t image_width = (textureSize - (textureRect.right - textureRect.left));
        const Real inv_scale = 1 / textureScale;

        const Real * const ogre_restrict heightData = data->getHeightData ();
        assert (heightData && "PagingLandScapeTexture::compute()");

        for (size_t k = textureRect.top; k < textureRect.bottom; ++k)
        {
            const uint k_terrain = (uint)(k * inv_scale);
            const size_t curr_row = k_terrain * heightfiledsize;
            for (size_t i = textureRect.left; i < textureRect.right; ++i)
            {             
                const uint i_terrain = (uint)(i * inv_scale);

                assert (i < textureSize && k < textureSize && 
                    "PagingLandScapeTexture::compute()");

                assert (i_terrain < heightfiledsize && k_terrain < heightfiledsize && 
                    "PagingLandScapeTexture::compute()");

                computePoint(curr_image_pos,
                            heightData[i_terrain + curr_row], 
                            1.0f - data->getNormal (i_terrain, k_terrain).y);

                curr_image_pos += 1;
            }
            curr_image_pos += image_width;
        }  
    }
    //-----------------------------------------------------------------------
    void PagingLandScapeTexture::update()
    {       
        if (!mSupportModif)
            return;

        // at least deformed once, so need to save texture if asked by user (option)
        mIsModified = true; 

        Image::Box dataRect (0, 0, 0, 0, 0, 1);
        Image::Box texturerect (0, 0, 0, 0, 0, 1);

        // computes deformation
        PagingLandScapeData2D *data;
        if (mIsDeformRectModified)
        {
            dataRect = mDeformRect;
            data = mParent->getSceneManager()->getData2DManager()->getData2D(mDataX, mDataZ);
            //            rect = data->getDeformationRectangle ();
            if (dataRect.getWidth() && dataRect.getHeight ())
            {

                const Real textureScale = mParent->getOptions()->TextureStretchFactor;

                texturerect.left = dataRect.left * textureScale;
                texturerect.top  = dataRect.top * textureScale;
                texturerect.right = dataRect.right * textureScale + 1;
                texturerect.bottom = dataRect.bottom * textureScale + 1;

                dataRect.right += 1;
                dataRect.bottom += 1;

                compute(data, dataRect, texturerect);
            }
        }
        // try to upload only the smallest rectangle containing modification
        if (mIsPaintRectModified)
        {
            if (mIsDeformRectModified)
            {
                dataRect.left = std::min (mPaintRect.left, dataRect.left);
                dataRect.right = std::max (mPaintRect.right, dataRect.right);
                dataRect.top =  std::min (mPaintRect.top, dataRect.top);
                dataRect.bottom = std::max (mPaintRect.bottom, dataRect.bottom);
            } // if (mNeedUpdate)
            else
            {
                dataRect = mPaintRect;

                dataRect.right += 1;
                dataRect.bottom += 1;

                const Real textureScale = mParent->getOptions()->TextureStretchFactor;

                texturerect.left = dataRect.left * textureScale;
                texturerect.right = dataRect.right * textureScale;
                texturerect.top  = dataRect.top * textureScale;
                texturerect.bottom = dataRect.bottom * textureScale;
            }
        } // if (mIsRectModified)


        // Upload any changes (deformation or)
        if (texturerect.getWidth() && texturerect.getHeight ())
        {
            upload (texturerect);           
        } // if (rect.getWidth() && rect.getHeight ())

        if (mIsDeformRectModified)
            data->resetDeformationRectangle ();
        PagingLandScapeTexture::updated ();
    }
    //-----------------------------------------------------------------------
    void PagingLandScapeTexture::deformheight (const uint x, 
        const uint z, 
        const Real paintForce)
    {
        adjustDeformationRectangle(x, z);
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
