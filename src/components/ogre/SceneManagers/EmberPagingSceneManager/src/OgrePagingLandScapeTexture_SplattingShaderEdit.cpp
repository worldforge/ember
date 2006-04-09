 /***************************************************************************
OgrePagingLandScapeTexture_Splatting.cpp  -  description
	-------------------
	begin                : Mon Apr 16 2004
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

#include "OgreStringConverter.h"
#include "OgreMaterialManager.h"
#include "OgreTextureManager.h"
#include "OgreTechnique.h"
#include "OgrePass.h"

#include "OgrePagingLandScapeSceneManager.h"
#include "OgrePagingLandScapeOptions.h"
#include "OgrePagingLandScapeTexture.h"
#include "OgrePagingLandScapeTextureManager.h"
#include "OgrePagingLandScapeData2DManager.h"
#include "OgrePagingLandScapeData2D.h"

#include "OgrePagingLandScapeTexture_SplattingShaderEdit.h"

#include "fileutils.h"

namespace Ogre
{
    // static load
    //params->setNamedConstant

    // static unload
    //params->clearAutoConstants()

    uint PagingLandScapeTexture_SplattingShaderEdit::mPageSize = 0;
    Real PagingLandScapeTexture_SplattingShaderEdit::heights[4];
    Real PagingLandScapeTexture_SplattingShaderEdit::dividers[4];
    ColourValue PagingLandScapeTexture_SplattingShaderEdit::colors[4];

    //-----------------------------------------------------------------------
    void PagingLandScapeTexture_SplattingShaderEdit::_setPagesize ()
    {
        mPageSize = mParent->getOptions()->PageSize - 1;

        colors[0] = mParent->getOptions()->matColor[0];
        colors[1] = mParent->getOptions()->matColor[1];
        colors[2] = mParent->getOptions()->matColor[2];
        colors[3] = mParent->getOptions()->matColor[3];

        //  slope[] ??

        heights[0] = 0.0f;
        heights[1] = mParent->getOptions()->matHeight[1];
        heights[2] = mParent->getOptions()->matHeight[2];
        heights[3] = mParent->getSceneManager()->getData2DManager()->getMaxHeight ();

        dividers[0] = 1.0f;
        if (heights[1] > 0)
            dividers[1] = 1.0f / heights[1];
        else 
            dividers[1] = 0.0f;
        if ((heights[2]  - heights[1]) > 0)
            dividers[2] = 1.0f / (heights[2]  - heights[1]);
        else 
            dividers[1] = 0.0f;
        if ((heights[3]  - heights[2]) > 0)
            dividers[3] = 1.0f / (heights[3]  - heights[2]);
        else 
            dividers[1] = 0.0f;

    }
    //-----------------------------------------------------------------------
    void PagingLandScapeTexture_SplattingShaderEdit::_clearData ()
    {      

    }
    //-----------------------------------------------------------------------
    PagingLandScapeTexture* PagingLandScapeTexture_SplattingShaderEdit::newTexture()
    {
        return new PagingLandScapeTexture_SplattingShaderEdit(mParent);
    }
    //-----------------------------------------------------------------------
    bool PagingLandScapeTexture_SplattingShaderEdit::TextureRenderCapabilitesFullfilled()
    {      
		const PagingLandScapeOptions * const opt = mParent->getOptions();
            
		if (opt->NumMatHeightSplat < 3)
			return false;
        if (opt->numTextureUnits < 4)
            return false;  
        if (!opt->hasFragmentShader)
            return false;
            
        return true;
    }
    //-----------------------------------------------------------------------
    PagingLandScapeTexture_SplattingShaderEdit::PagingLandScapeTexture_SplattingShaderEdit(PagingLandScapeTextureManager *textureMgr) : 
        PagingLandScapeTexture(textureMgr, true)
    {

    }
    //-----------------------------------------------------------------------
    PagingLandScapeTexture_SplattingShaderEdit::~PagingLandScapeTexture_SplattingShaderEdit()
    {

    }
    //-----------------------------------------------------------------------
    void PagingLandScapeTexture_SplattingShaderEdit::_LoadTexture(const String &TexName,
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
    void PagingLandScapeTexture_SplattingShaderEdit::upload(const Image::Box& textureRect)
    {    
        assert (!mMaterial.isNull() && "PagingLandScapeTexture_SplattingShaderEdit::update()");
        assert (!mTexture.isNull() && "PagingLandScapeTexture_SplattingShaderEdit::update()");
        assert (!mBuffer.isNull() && "PagingLandScapeTexture_SplattingShaderEdit::update()");

        const PixelBox srcBox = mImage.getPixelBox().getSubVolume(textureRect);	
        const PixelBox lock = mBuffer->lock(textureRect, HardwareBuffer::HBL_DISCARD); 
        PixelUtil::bulkPixelConversion(srcBox, lock); 
        mBuffer->unlock(); 

//#define _Missed_Spot  0

#ifdef _Missed_Spot                
        // debug can help finding missed spot.
        const PixelBox srcSpotBox = mImage.getPixelBox();	
        const uint textureSize = (mParent->getOptions()->PageSize - 1) * mParent->getOptions()->TextureStretchFactor;
        const Image::Box rect (0, 0, 0, textureSize, textureSize, 1);
        const PixelBox lockSpot = mBuffer->lock (rect ,HardwareBuffer::HBL_DISCARD); 
        PixelUtil::bulkPixelConversion(srcSpotBox, lockSpot); 
        mBuffer->unlock();
#endif //_Missed_Spot 	
        
    }
    //-----------------------------------------------------------------------
    void PagingLandScapeTexture_SplattingShaderEdit::paint (const uint x, const uint z, 
        const Real paintForce, const uint paintChannel)
    {
        assert (paintChannel < 4);
        uchar * const BaseData = mCoverageData;

        assert (BaseData && "PagingLandScapeTexture_SplattingShaderEdit::paint()");

        const Real blend = paintForce;
        assert (paintForce >= 0.0f && paintForce <= 1.0f && "PagingLandScapeTexture_SplattingShaderEdit::paint()");
        const Real invBlend = 1.0f - blend;


        const size_t psize = mParent->getOptions()->PageSize - 1;
        assert ((x + z*  psize < psize*psize) && "PagingLandScapeTexture_SplattingShaderEdit::paint()");
       
        const Real textureScale = mParent->getOptions ()->TextureStretchFactor;
        const uint curr_image_pos = static_cast <uint> (x*textureScale*4 + z * psize*textureScale*4);
        const  uchar previousValue = BaseData[ curr_image_pos ];
        const  uchar newValue = static_cast <uchar> (255 * blend + previousValue * invBlend);

        if (newValue > 0)
            BaseData[ curr_image_pos + paintChannel] = newValue;
        else
            BaseData[ curr_image_pos + paintChannel] = 0;

        const uchar AlphaChange = previousValue -  BaseData[ curr_image_pos  + paintChannel];

        for (uint i = 0; i < 4; i++)
        {
            if (i == paintChannel)
                continue;

            const uchar otherValue = BaseData[ curr_image_pos   + i];
            if (otherValue > 0)
            {
                if (otherValue - AlphaChange > 0)
                    BaseData[ curr_image_pos  + i] = otherValue - AlphaChange;
                else
                    BaseData[ curr_image_pos  + i] = 0;

            } // if (otherValue > 0)
        } // for (uint i = 0; i < 4; i++)

        adjustPaintRectangle(x, z); 
    }
    //-----------------------------------------------------------------------
    void PagingLandScapeTexture_SplattingShaderEdit::computePoint(const uint imagePos,
                                                                    const Real height, 
                                                                    const Real slope)
    {       
        uchar * const BaseData = mCoverageData;
        assert (BaseData && "PagingLandScapeTexture_InstantBaseTextureEdit::computeInstantBase()");
        const uint curr_image_pos = imagePos*4;//bpp

        Real alpha[4];   
                
        uint indx = 1;
        const uint numHeights = mParent->getOptions()->NumMatHeightSplat;
        while (height >= heights[indx] && indx < numHeights)
            indx++;                                

        const uint bScale = 255;
        const uint up_indx = indx;
        const uint down_indx = indx - 1;
        const Real interpol = (height  - heights[down_indx]) * dividers[up_indx];  
        
        //ColourValue color;
        for (uint ialpha = 0; ialpha < 4; ialpha++) 
        {    
            alpha[ialpha] = 0.0f;
        }

        if (slope < 0.05f)// speed-up as it's invisible
        {
            const Real B = (1.0f - interpol);
            const Real C = interpol;

            alpha[indx - 1] = B;
            alpha[indx] = C; 
        }
        else 
        {
            const Real A = (1.0f - slope);
            const Real B = A * (1.0f - interpol);
            const Real C = A * interpol;
            const Real D = slope;

            alpha[indx - 1] = B;
            alpha[indx] = C; 

            alpha[ 2 ] = alpha[ 2 ] + slope;
            alpha[ 2 ] = alpha[ 2 ] > 1.0f ? 1.0f : alpha[ 2 ];
        }
        for (uint isplat = 0; isplat < 4; isplat++) 
        {    
            BaseData[ curr_image_pos + isplat] = static_cast <uchar> (alpha[isplat]*bScale);
        }
    }
    //-----------------------------------------------------------------------
    void PagingLandScapeTexture_SplattingShaderEdit::_loadMaterial()
    {
	    if (mMaterial.isNull())
	    {     
			const PagingLandScapeOptions * const opt = mParent->getOptions();
            
            // Create a new texture using the base image
            const String filename = opt->LandScape_filename;
            const String commonName = StringConverter::toString(mDataZ) + String(".") + StringConverter::toString(mDataX);
            const String prefilename = opt->LandScape_filename;
            const String postfilename = commonName + "." + opt->TextureExtension;

            const bool compressed = opt->VertexCompression;
            const String MatClassName = (compressed?String("SplattingMaterialShaderDecompress"):String("SplattingMaterialShader"));
            const String matname =  MatClassName + commonName + filename;
            mMaterial = MaterialManager::getSingleton().getByName(matname);

            const String texname = filename + ".Coverage." + commonName + ".";

            String finalTexName;
            if (mParent->getOptions()->Deformable &&
                ResourceGroupManager::getSingleton().resourceExists(opt->groupName, 
                texname + "modif." + opt->TextureExtension))
            {
                finalTexName = texname + "modif." + opt->TextureExtension;             
            }
            else
            {
                finalTexName = texname + opt->TextureExtension;
            }

            if (!mCoverageData)
            {
                mImage.load (finalTexName, opt->groupName);
                mCoverageData = mImage.getData();
            } 

            if (mMaterial.isNull())
	        {                
                MaterialPtr ClassMaterial = MaterialManager::getSingleton().getByName(MatClassName);
                assert (!ClassMaterial.isNull());
                mMaterial = ClassMaterial->clone(matname);
                   
                Pass * const p = mMaterial->getTechnique(0)->getPass(0); 
                if (compressed)
                {
                    bindCompressionSettings (p->getVertexProgramParameters());
                    bindCompressionSettings (p->getShadowReceiverVertexProgramParameters ());
                }

                const String texname (filename + ".Coverage." + postfilename);
                _LoadTexture (finalTexName, opt->groupName);
                //TextureManager::getSingleton().load (finalTexName, opt->groupName);    
                p->getTextureUnitState(0)->setTextureName(texname);
                
                const uint numSplats = opt->NumMatHeightSplat;
                uint splat_pass = 0; 
                while  (splat_pass < numSplats)
                {
                    p->getTextureUnitState(splat_pass + 1)->setTextureName(opt->SplatDetailMapNames[splat_pass]);
                    splat_pass++;
                }
            }
            // Now that we have all the resources in place, we load the material
           
            mMaterial->setLodLevels(opt->lodMaterialDistanceList);
            mMaterial->setLightingEnabled(opt->lit);
            mMaterial->load(); 
	    }
    }

    //-----------------------------------------------------------------------
    void PagingLandScapeTexture_SplattingShaderEdit::_unloadMaterial()
    {
        assert (!mMaterial.isNull() && "PagingLandScapeTexture_SplattingShaderEdit::::_unloadMaterial");    
        if (mIsModified)
        { 
            const String fname = mParent->getOptions()->LandScape_filename + 
                ".Coverage." + 
                StringConverter::toString(mDataZ) + 
                String(".") + 
                StringConverter::toString(mDataX) + ".";
            const String extname = mParent->getOptions()->TextureExtension;


            FileInfoListPtr finfo =  ResourceGroupManager::getSingleton().findResourceFileInfo (
                mParent->getOptions()->groupName, 
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
        mImage.loadDynamicImage (0, 0, 0, PF_BYTE_RGB);  
        mCoverageData = 0;
    }
} //namespace
