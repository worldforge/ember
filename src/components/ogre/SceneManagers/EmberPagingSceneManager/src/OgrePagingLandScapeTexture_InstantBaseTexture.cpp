/***************************************************************************
OgrePagingLandScapeTexture_InstantBaseTexture.cpp  -  description
-------------------
begin                : Mon Apr 26 2004
copyright            : (C) 2003-2005 by Jose A Milan & Tuan Kuranes
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

#include "OgrePagingLandScapeSceneManager.h"
#include "OgrePagingLandScapeOptions.h"
#include "OgrePagingLandScapeTexture.h"
#include "OgrePagingLandScapeTextureManager.h"
#include "OgrePagingLandScapeTexture_InstantBaseTexture.h"
#include "OgrePagingLandScapeData2DManager.h"
#include "OgrePagingLandScapeData2D.h"

#include "fileutils.h"

namespace Ogre
{

    uint PagingLandScapeTexture_InstantBaseTexture::mPageSize = 0;
	Image PagingLandScapeTexture_InstantBaseTexture::mImage;
	std::vector <Real> PagingLandScapeTexture_InstantBaseTexture::heights;
    std::vector <Real> PagingLandScapeTexture_InstantBaseTexture::dividers;
    std::vector <ColourValue> PagingLandScapeTexture_InstantBaseTexture::colors;

    //-----------------------------------------------------------------------
    void PagingLandScapeTexture_InstantBaseTexture::_setPagesize ()
    {
		PagingLandScapeOptions * const opt = mParent->getOptions();
        mPageSize = opt->PageSize - 1;
        const Real Texturescale = opt->TextureStretchFactor;
        const uint textureSize = mPageSize * Texturescale;
        uchar *data = new uchar[textureSize * textureSize * 3];
		mImage.loadDynamicImage (data, textureSize, textureSize, PF_BYTE_RGB);

		const uint numColors = opt->NumMatHeightSplat;

		uint i;
		colors.reserve (numColors);
		colors.resize (numColors);
		for (i = 0; i < numColors; i++)
		{
			 colors[i] = opt->matColor[i];
		}
		heights.reserve (numColors);
		heights.resize (numColors);
		for (i = 0; i < numColors; i++)
		{
			 heights[i] = opt->matHeight[i];
		}

        //  slope[] ??

        if (numColors  && heights[numColors - 1] == 0.0f) 
			heights[numColors - 1] = mParent->getSceneManager()->getData2DManager()->getMaxHeight ();

		dividers.reserve (numColors);
		dividers.resize (numColors);
		if (numColors)
		{
			dividers[0] = 1.0f;
			
			for (i = 1; i < numColors; i++) 
			{   
				if ((heights[i]  - heights[i - 1])  > 0)
					dividers[i] = 1 / (heights[i]  - heights[i - 1]);
				else 
					dividers[i] = 0.0f;
			}		
		}
    }
    //-----------------------------------------------------------------------
    PagingLandScapeTexture* PagingLandScapeTexture_InstantBaseTexture::newTexture()
    {
        return new PagingLandScapeTexture_InstantBaseTexture(mParent);
    }
    //-----------------------------------------------------------------------
    bool PagingLandScapeTexture_InstantBaseTexture::TextureRenderCapabilitesFullfilled()
    {        
		if (mParent->getOptions()->NumMatHeightSplat > 3)
			return true;
		else
			return false;
    }
    //-----------------------------------------------------------------------
    void PagingLandScapeTexture_InstantBaseTexture::_clearData ()
    {      
		delete [] mImage.getData ();    
		mImage.loadDynamicImage (0, 0, 0, PF_BYTE_RGB);    
    }
    //-----------------------------------------------------------------------
    PagingLandScapeTexture_InstantBaseTexture::PagingLandScapeTexture_InstantBaseTexture(PagingLandScapeTextureManager *textureMgr) 
        : 
        PagingLandScapeTexture(textureMgr, true)
    {

    }
    //-----------------------------------------------------------------------
    PagingLandScapeTexture_InstantBaseTexture::~PagingLandScapeTexture_InstantBaseTexture()
    {
	    
    }
    //-----------------------------------------------------------------------
    void PagingLandScapeTexture_InstantBaseTexture::_loadMaterial()
    {
	    if (mMaterial.isNull())
		{
			PagingLandScapeOptions * const opt = mParent->getOptions();
			const String filename  = opt->LandScape_filename;
            const String extname   = opt->TextureExtension;
            const String groupName = opt->groupName;

            const String commonName = StringConverter::toString(mDataZ) + 
                                        String(".") +
                                        StringConverter::toString(mDataX);
            const String matname = String("InstantBaseMaterial.") + commonName + filename;
            const String texname = filename + ".Base." + commonName + ".";

            String finalTexName;
            if (opt->Deformable &&
                ResourceGroupManager::getSingleton().resourceExists(groupName, 
                                                texname + "modif." +extname))
            {
                finalTexName = texname + "modif." + extname;             
            }
            else
            {
                finalTexName = texname + extname;
            }

            // check need of material loading
            mMaterial = MaterialManager::getSingleton().getByName(matname);
            if (mMaterial.isNull())
            {
                MaterialPtr templateMaterial;
                if (opt->VertexCompression)
                {
                    templateMaterial = MaterialManager::getSingleton().getByName(String ("InstantBaseMaterialVertexPixelShaded"));
                    
		            // Create a new texture using the base image
		            mMaterial = templateMaterial->clone(matname, true, groupName);    

                    Pass *p = mMaterial->getTechnique(0)->getPass(0);
                    bindCompressionSettings (p->getVertexProgramParameters());
                    bindCompressionSettings (p->getShadowReceiverVertexProgramParameters ());
                }
                else
                {
                    templateMaterial = MaterialManager::getSingleton().getByName(String ("InstantBaseMaterial"));
		            // Create a new texture using the base image
		            mMaterial = templateMaterial->clone(matname, true, groupName);
                }   
               
	            // assign this texture to the material
                _LoadTexture(finalTexName, groupName);
	            mMaterial->getTechnique(0)->getPass(0)->getTextureUnitState(0)->setTextureName (finalTexName);
            
     
                mMaterial->load();
	            mMaterial->setLightingEnabled(false);
            }
            else
            {
                _LoadTexture(finalTexName, groupName);
            }
           
        }
    }
    //-----------------------------------------------------------------------
    void PagingLandScapeTexture_InstantBaseTexture::_LoadTexture(const String &TexName,
                                                                const String &groupName)
    {  
         // check need of Texture loading
        if (mTexture.isNull())
        {
            mTexture = TextureManager::getSingleton().getByName(TexName);
            if (mTexture.isNull())
            {
                mTexture = TextureManager::getSingleton().load (TexName, 
                                            groupName, 
                                            TEX_TYPE_2D, 7, 1.0f);
            }
        }
        mBuffer = mTexture->getBuffer ();
    }
    //-----------------------------------------------------------------------
    void PagingLandScapeTexture_InstantBaseTexture::upload(const Image::Box& textureRect) 
    {    
        assert (mImage.getData() && "PagingLandScapeTexture_InstantBaseTexture::update()");
        assert (!mMaterial.isNull() && "PagingLandScapeTexture_InstantBaseTexture::update()");
        assert (!mTexture.isNull() && "PagingLandScapeTexture_InstantBaseTexture::update()");
        assert (!mBuffer.isNull() && "PagingLandScapeTexture_InstantBaseTexture::update()");
                
        const PixelBox srcBox = mImage.getPixelBox().getSubVolume(textureRect);	
        const PixelBox lock = mBuffer->lock(textureRect, HardwareBuffer::HBL_DISCARD); 
        PixelUtil::bulkPixelConversion(srcBox, lock); 
        mBuffer->unlock();  	       
    }
    //-----------------------------------------------------------------------
    void PagingLandScapeTexture_InstantBaseTexture::paint (const uint x, const uint z, 
                                        const Real paintForce, const ColourValue &mPaintColor)
    {
        uchar * const BaseData = mImage.getData();

        assert (BaseData && "PagingLandScapeTexture_InstantBaseTexture::paint()");

        const Real blend = 1.0f;//;paintForce;
        assert (paintForce >= 0.0f && paintForce <= 1.0f && "PagingLandScapeTexture_InstantBaseTexture::paint()");
        const Real invBlend = 1.0f - blend;
    
    
//        Image::Box rect (x, z, x, z, 0, 1);
//
//        const PixelBox srcBox = mImage.getPixelBox().getSubVolume(rect);	
//        const PixelBox lock = mBuffer->lock(rect, HardwareBuffer::HBL_DISCARD); 
//        PixelUtil::bulkPixelConversion(srcBox, lock); 
//        mBuffer->unlock();  	
        assert ((x + z*  mPageSize < mPageSize*mPageSize) && "PagingLandScapeTexture_InstantBaseTexture::paint()");
        adjustPaintRectangle(x, z);

        const Real textureScale = mParent->getOptions()->TextureStretchFactor;

        const uint textureX = x * textureScale;
        const uint textureZ = z * textureScale;
        const uint textureSize = mPageSize * textureScale;

        const uchar bScale = 255;
        const uint curr_image_pos = textureX * 3+ textureZ * textureSize * 3;//x * 3 + z * mPageSize * 3;
        BaseData[ curr_image_pos ]    = 
            static_cast <uchar> (bScale * mPaintColor.r * blend + BaseData[ curr_image_pos ] * invBlend);
        BaseData[ curr_image_pos + 1] = 
            static_cast <uchar> (bScale * mPaintColor.g * blend + BaseData[ curr_image_pos + 1] * invBlend);
        BaseData[ curr_image_pos + 2] = 
            static_cast <uchar> (bScale * mPaintColor.b * blend + BaseData[ curr_image_pos + 2] * invBlend);    
    }
    //-----------------------------------------------------------------------
    void PagingLandScapeTexture_InstantBaseTexture::computePoint(
        const uint imagePos,
        const Real height, 
        const Real slope) 
    {       
        uchar * const ogre_restrict BaseData = mImage.getData();
	    assert (BaseData && "PagingLandScapeTexture_InstantBaseTexture::computeInstantBase()");
        const uint curr_image_pos = imagePos*3;//Bpp

        uint indx = 1;
        const uint numHeights = mParent->getOptions()->NumMatHeightSplat;
        while (height >= heights[indx] && indx < numHeights)
               indx++;                                

        const uint bScale = 255;
        const uint up_indx = indx;
        const uint down_indx = indx - 1;
        const Real interpol = (height  - heights[down_indx]) * dividers[up_indx];  
                    
        if (slope < 0.05f)// speed-up as it's invisible
        {
            const Real B = (1.0f - interpol);
            const Real C = interpol;

            // RGB = colors[indx - 1] * B + colors[indx] * C;
            BaseData[ curr_image_pos ]    = static_cast <uchar> ((colors[down_indx].r * B + colors[up_indx].r * C)* bScale);
            BaseData[ curr_image_pos + 1] = static_cast <uchar> ((colors[down_indx].g * B + colors[up_indx].g * C)* bScale);
            BaseData[ curr_image_pos + 2] = static_cast <uchar> ((colors[down_indx].b * B + colors[up_indx].b * C)* bScale);
        }
        else 
        {
            const Real A = (1.0f - slope);
            const Real B = A * (1.0f - interpol);
            const Real C = A * interpol;
            const Real D = slope;
           
            // RGB = colors[indx - 1] * B + colors[indx] * C + colors[2] * D;                    
            BaseData[ curr_image_pos ]     = static_cast <uchar> ((colors[down_indx].r * B + colors[up_indx].r * C + colors[2].r * D)* bScale);
            BaseData[ curr_image_pos + 1 ] = static_cast <uchar> ((colors[down_indx].g * B + colors[up_indx].g * C + colors[2].g * D)* bScale);
            BaseData[ curr_image_pos + 2 ] = static_cast <uchar> ((colors[down_indx].b * B + colors[up_indx].b * C + colors[2].b * D)* bScale);
        }
    }
    //-----------------------------------------------------------------------
    void PagingLandScapeTexture_InstantBaseTexture::_unloadMaterial()
    {
        assert (!mMaterial.isNull() && "PagingLandScapeTexture_InstantBaseTexture::::_unloadMaterial");    
        if (mIsModified)
		{ 
			PagingLandScapeOptions * const opt = mParent->getOptions();
			const Real Texturescale = opt->TextureStretchFactor;
            const Image::Box datarect (0, 0, 0, mPageSize*Texturescale, mPageSize*Texturescale, 1);
            compute(mParent->getSceneManager()->getData2DManager()->getData2D(mDataX, mDataZ), 
                datarect, 
                mImage.getPixelBox());

            const String fname = opt->LandScape_filename + 
                                ".Base." + 
                                StringConverter::toString(mDataZ) + 
                                String(".") + 
                                StringConverter::toString(mDataX) + ".";
            const String extname = opt->TextureExtension;


            FileInfoListPtr finfo =  ResourceGroupManager::getSingleton().findResourceFileInfo (
                    opt->groupName, 
                    fname + extname);
            FileInfoList::iterator it = finfo->begin();

            if (it != finfo->end())
            {
                //FileInfo *inf = &(*it);
                char *olddir = ChangeToDir (const_cast< char * > (((it)->archive->getName()).c_str()));
                //FileSystemArchive::pushDirectory()
                mImage.save(fname + "modif." + extname);   
                //FileSystemArchive::pushDirectory();
                RetablishDir (olddir);
            }
        }
        // Anyway, they're surely null already, as they're freed by delete page()
        mBuffer.setNull ();
        mTexture.setNull ();
  }
} //namespace
