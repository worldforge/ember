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
#include "OgrePagingLandScapeTexture_Splatting2Edit.h"

#include "OgrePagingLandScapeData2DManager.h"
#include "OgrePagingLandScapeData2D.h"

#include "fileutils.h"

namespace Ogre
{
    uint PagingLandScapeTexture_Splatting2Edit::mPageSize = 0;
    Real PagingLandScapeTexture_Splatting2Edit::heights[4];
    Real PagingLandScapeTexture_Splatting2Edit::dividers[4];
    ColourValue PagingLandScapeTexture_Splatting2Edit::colors[4];
    //-----------------------------------------------------------------------
    void PagingLandScapeTexture_Splatting2Edit::_setPagesize(void)
    {
        mParent->getOptions()->VertexCompression = false;
        mParent->getOptions()->lodMorph = false;

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
    void PagingLandScapeTexture_Splatting2Edit::_clearData(void)
    {
    
    }
    //-----------------------------------------------------------------------
    PagingLandScapeTexture* PagingLandScapeTexture_Splatting2Edit::newTexture()
    {
        return new PagingLandScapeTexture_Splatting2Edit(mParent);
    }
    //-----------------------------------------------------------------------
    bool PagingLandScapeTexture_Splatting2Edit::TextureRenderCapabilitesFullfilled()
    {        
		const PagingLandScapeOptions * const opt = mParent->getOptions();
		
		if (opt->NumMatHeightSplat < 3)
			return false;
        if (opt->numTextureUnits < 2)
            return false;
            
        return true;
    }
    //-----------------------------------------------------------------------
    PagingLandScapeTexture_Splatting2Edit::PagingLandScapeTexture_Splatting2Edit(PagingLandScapeTextureManager *textureMgr) : PagingLandScapeTexture(textureMgr)
    {
    }
    //-----------------------------------------------------------------------
    PagingLandScapeTexture_Splatting2Edit::~PagingLandScapeTexture_Splatting2Edit()
    {
    }
    //-----------------------------------------------------------------------
    void PagingLandScapeTexture_Splatting2Edit::LoadAlphaMap(const String &filename, const uint channel)
    {
       if (mTextures[channel].isNull())
       {
            mTextures[channel] = TextureManager::getSingleton().getByName (filename);
            if (mTextures[channel].isNull())
            {
	            Image Imageloader;
                const String group = mParent->getOptions()->groupName;
                Imageloader.load(filename, group);
                const size_t size = Imageloader.getWidth()*Imageloader.getHeight();
                uchar *data = new uchar [size];
                memcpy (data, Imageloader.getData(), size*sizeof(uchar));
                mImages[channel].loadDynamicImage(data, 
                                                    Imageloader.getWidth(), 
                                                    Imageloader.getHeight(), 
                                                    1, PF_A8, true);

                mTextures[channel] =  TextureManager::getSingleton().loadImage (filename, 
                                                      group,
                                                      mImages[channel]);
            }
       }
       mBuffers[channel] = mTextures[channel]->getBuffer ();
    }
    //-----------------------------------------------------------------------
    void PagingLandScapeTexture_Splatting2Edit::_loadMaterial()
    {
	    if (mMaterial.isNull())
	    {
			const PagingLandScapeOptions * const opt = mParent->getOptions();
            const String filename (opt->LandScape_filename);
            const String commonName (StringConverter::toString(mDataZ) + 
                                        String(".") +
                                        StringConverter::toString(mDataX));
                                        
            const String matname (String("SplattingMaterial2.") + commonName + filename);
		    mMaterial = MaterialManager::getSingleton().getByName(matname);
            if (mMaterial.isNull())
	        {
                const String extname (opt->TextureExtension);
		        mMaterial = MaterialManager::getSingleton().getByName("SplattingMaterial2");
                assert (!mMaterial.isNull());
                mMaterial = mMaterial->clone(matname);
                
              
                uint passnb = 0;
	            // assign this texture to the material
                // Create the base image        
		        const String endName ("." + commonName + "." + extname);  
                String texname (filename + ".Base" + endName);
                TextureManager::getSingleton().load (texname, opt->groupName);                                    
                mMaterial->getTechnique(1)->getPass(0)->getTextureUnitState(0)->setTextureName(texname);
                 
                Technique * const t = mMaterial->getTechnique(0); 
                const uint numSplats = opt->NumMatHeightSplat;
                const String alphamapBeginname (filename + ".Alpha."); 
                uint splat_pass = 0;  // if we remove a pass, splat still increases
                while  (splat_pass < numSplats)
                {  
                    // Create a new texture alpha map
                    texname = alphamapBeginname 
                            + StringConverter::toString(splat_pass) 
                            + endName; 
                    LoadAlphaMap (texname, splat_pass);
                    // assign this texture to the material
                    t->getPass(splat_pass)->getTextureUnitState(1)->setTextureName(texname);
                    t->getPass(splat_pass)->getTextureUnitState(0)->setTextureName(opt->SplatDetailMapNames[splat_pass]);
                    
                    splat_pass++;
                }
                    
		        // Now that we have all the resources in place, we load the material
                mMaterial->setLightingEnabled(opt->lit);
                mMaterial->setLodLevels(opt->lodMaterialDistanceList);
		        mMaterial->load(); 
            }
	    }
        for (uint i = 0; i < 4; i++)
        {
            isChannelNeedUpdate[i] = false;
            isChannelModified[i] = false;
        }
    }
    //-----------------------------------------------------------------------
    void PagingLandScapeTexture_Splatting2Edit::upload(const Image::Box& textureRect)
    {    
        assert (!mMaterial.isNull() && "PagingLandScapeTexture_Splatting2Edit::update()");

        for (uint i = 0; i < 4; i++)
        {
            if (isChannelNeedUpdate[i])
            {
                const PixelBox srcBox = mImages[i].getPixelBox().getSubVolume(textureRect);	
                const PixelBox lock = mBuffers[i]->lock(textureRect, HardwareBuffer::HBL_DISCARD); 
                PixelUtil::bulkPixelConversion(srcBox, lock); 
                mBuffers[i]->unlock();  	

                isChannelNeedUpdate[i] = false;
                isChannelModified[i] = true;
            }
        }

    }
    //-----------------------------------------------------------------------
    void PagingLandScapeTexture_Splatting2Edit::computePoint(const uint imagePos,
        const Real height, 
        const Real slope)
    {       
        const uint curr_image_pos = imagePos;//bpp

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
            assert(mImages[isplat].getData());
            mImages[isplat].getData()[curr_image_pos] = static_cast <uchar> (alpha[isplat]*bScale);
        }
    }
    //-----------------------------------------------------------------------
    void  PagingLandScapeTexture_Splatting2Edit::paint (const uint x, const uint z, 
        const Real paintForce, const uint mPaintChannel)
    {
        assert (mPaintChannel < 4);
        uchar * const BaseData = mImages[mPaintChannel].getData();

        assert (BaseData && "PagingLandScapeTexture_Splatting2Edit::paint()");

        const Real blend = paintForce;
        assert (paintForce >= 0.0f && paintForce <= 1.0f && "PagingLandScapeTexture_Splatting2Edit::paint()");
        const Real invBlend = 1.0f - blend;
    
    
        const size_t psize = mParent->getOptions()->PageSize - 1;
        assert ((x + z*  psize < psize*psize) && "PagingLandScapeTexture_Splatting2Edit::paint()");

        const Real textureScale = mParent->getOptions ()->TextureStretchFactor;
        const uint curr_image_pos = static_cast <uint> (x*textureScale + z * psize*textureScale);
        const  uchar previousValue = BaseData[ curr_image_pos ];
        const  uchar newValue = static_cast <uchar> (255 * blend + previousValue * invBlend);

        if (newValue > 0)
            BaseData[ curr_image_pos ] = newValue;
        else
            BaseData[ curr_image_pos ] = 0;

        const uchar AlphaChange = previousValue -  BaseData[ curr_image_pos ];

        for (uint i = 0; i < 4; i++)
        {
            if (i == mPaintChannel)
                continue;

            uchar * const otherBaseData = mImages[i].getData();
            const uchar otherValue = otherBaseData[ curr_image_pos ];
            if (otherValue > 0)
            {
                if (otherValue - AlphaChange > 0)
                    otherBaseData[ curr_image_pos ] = otherValue - AlphaChange;
                else
                    otherBaseData[ curr_image_pos ] = 0;
                isChannelNeedUpdate[i] = true;

            } // if (otherValue > 0)
        } // for (uint i = 0; i < 4; i++)

        isChannelNeedUpdate[mPaintChannel] = true;
        adjustPaintRectangle(x, z);  
    }
 //-----------------------------------------------------------------------
    void PagingLandScapeTexture_Splatting2Edit::_unloadMaterial()
    {
        assert (!mMaterial.isNull() && "PagingLandScapeTexture_Splatting2Edit::::_unloadMaterial");    
         
        if (mIsModified)
        {
            const String Alphafname = mParent->getOptions()->LandScape_filename + 
                                        ".Alpha.";

            const String PageNumberName = String(".") + StringConverter::toString(mDataZ) + 
                                                String(".") + 
                                        StringConverter::toString(mDataX) + 
                                        String(".");
            const String extname = mParent->getOptions()->TextureExtension;

            for (uint i = 0; i < 4; i++)
            {
                if (isChannelModified[i])
                {
                        
                    const String fname = Alphafname + StringConverter::toString(i) + PageNumberName;

                    FileInfoListPtr finfo =  ResourceGroupManager::getSingleton().findResourceFileInfo (
                            mParent->getOptions()->groupName, 
                            fname + extname);
                    FileInfoList::iterator it = finfo->begin();
                    if (it != finfo->end())
                    {
                        //FileInfo *inf = &(*it);
                        char *olddir = ChangeToDir (const_cast< char * > (((it)->archive->getName()).c_str()));
                        //FileSystemArchive::pushDirectory()
                        mImages[i].save(fname + "modif." + extname);   
                        //FileSystemArchive::pushDirectory();
                        RetablishDir (olddir);

                    } // if (it != finfo->end())
                } // if (isChannelNeedUpdate[i])
            } // for (uint i = 0; i < 4; i++)
        } // if (mIsModified)

        for (uint i = 0; i < 4; i++)
        {
            mBuffers[i].setNull ();
            mImages[i].loadDynamicImage (0, 0, 0, PF_BYTE_RGB);  
            mTextures[i].setNull();
        }
  }
} //namespace
