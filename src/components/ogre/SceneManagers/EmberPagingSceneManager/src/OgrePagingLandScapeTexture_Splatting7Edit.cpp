/***************************************************************************
OgrePagingLandScapeTexture_Splatting7Edit.cpp  -  description
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
#include "OgrePagingLandScapeTexture_Splatting7Edit.h"


#include "OgrePagingLandScapeData2DManager.h"
#include "OgrePagingLandScapeData2D.h"

#include "fileutils.h"

namespace Ogre
{
    //-----------------------------------------------------------------------
    void PagingLandScapeTexture_Splatting7Edit::_setPagesize(void)
    {
        mParent->getOptions()->VertexCompression = false;
        mParent->getOptions()->lodMorph = false;
    }
    //-----------------------------------------------------------------------
    void PagingLandScapeTexture_Splatting7Edit::_clearData(void)
    {
    
    }
    //-----------------------------------------------------------------------
    PagingLandScapeTexture* PagingLandScapeTexture_Splatting7Edit::newTexture()
    {
        return new PagingLandScapeTexture_Splatting7Edit(mParent);
    }
    //-----------------------------------------------------------------------
    bool PagingLandScapeTexture_Splatting7Edit::TextureRenderCapabilitesFullfilled()
    {                   
		const PagingLandScapeOptions * const opt = mParent->getOptions();
		
		if (opt->NumMatHeightSplat < 2)
			return false;
        if (opt->numTextureUnits < 6)
            return false;
            
        return true;
    }
    //-----------------------------------------------------------------------
    PagingLandScapeTexture_Splatting7Edit::PagingLandScapeTexture_Splatting7Edit(PagingLandScapeTextureManager *textureMgr) : PagingLandScapeTexture(textureMgr)
    {
    }

    //-----------------------------------------------------------------------
    PagingLandScapeTexture_Splatting7Edit::~PagingLandScapeTexture_Splatting7Edit()
    {
    }
    //-----------------------------------------------------------------------
    void PagingLandScapeTexture_Splatting7Edit::LoadAlphaMap(const String &filename, const uint channel)
    {       
       if (mTextures[channel].isNull())
       {
            mTextures[channel] = TextureManager::getSingleton().getByName (filename);
            if (mTextures[channel].isNull())
            {
	            Image Imageloader;
                const String group = mParent->getOptions()->groupName;
                Imageloader.load (filename, group);
                const size_t size = Imageloader.getWidth()*Imageloader.getHeight();
                uchar *data = new uchar [size];
                memcpy (data, Imageloader.getData(), size*sizeof(uchar));
                mImages[channel].loadDynamicImage(data, 
                                                    Imageloader.getWidth(), 
                                                    Imageloader.getHeight(), 
                                                     1, 
                                                     PF_A8, true);

                mTextures[channel] =  TextureManager::getSingleton().loadImage (filename, 
                                                      group,
                                                      mImages[channel]);
            }
       }
       mBuffers[channel] = mTextures[channel]->getBuffer ();
    
    }
    //-----------------------------------------------------------------------
    void PagingLandScapeTexture_Splatting7Edit::_loadMaterial()
    {
	    if (mMaterial.isNull())
	    {              
			const PagingLandScapeOptions * const opt = mParent->getOptions();
            
            // Create a new texture using the base image
            const String filename = opt->LandScape_filename;
            const String commonName = StringConverter::toString(mDataZ) + 
                                        String(".") +
                                        StringConverter::toString(mDataX);
            const String matname = String("SplattingMaterial7.") + commonName + filename;
            mMaterial = MaterialManager::getSingleton().getByName(matname);
	        if (mMaterial.isNull())
	        {
                mMaterial = MaterialManager::getSingleton().getByName("SplattingMaterial7");
                assert (!mMaterial.isNull());
                mMaterial = mMaterial->clone(matname);
                   

                const String extname = opt->TextureExtension;

                String texname = filename + ".Base." +
                    commonName + "." + extname;
                TextureManager::getSingleton().load (texname, opt->groupName);    
                                                      
                Pass * const p = mMaterial->getTechnique(0)->getPass(0); 
                // assign this texture to the material
                p->getTextureUnitState(0)->setTextureName(texname);
                mMaterial->getTechnique(1)->getPass(0)->getTextureUnitState(0)->setTextureName(texname);

                                          
                const String alphamapBeginname = filename + ".Alpha."; 
                const String alphamapEndname = "." + commonName + "." + extname;

                uint splat_pass = 0;  
                uint alpha_pass = 0;            
                // Create a new texture using the alpha map
                texname = alphamapBeginname 
                        + StringConverter::toString(splat_pass) 
                        + alphamapEndname; 
                LoadAlphaMap (texname, alpha_pass);
                
                // assign the splat material                               
                p->getTextureUnitState(splat_pass++)->setTextureName(opt->SplatDetailMapNames[alpha_pass++]);
                // assign this texture to the material
                p->getTextureUnitState(splat_pass++)->setTextureName(texname);

                const uint numSplats = 6;
                while  (splat_pass < numSplats)
                {            
                    // Create a new texture using the alpha map
                    texname = alphamapBeginname 
                            + StringConverter::toString(splat_pass) 
                            + alphamapEndname; 
                    LoadAlphaMap (texname, alpha_pass);
                    
                    // assign this texture to the material
                    p->getTextureUnitState(splat_pass++)->setTextureName(texname);
                    // assign the splat material
                    p->getTextureUnitState(splat_pass++)->setTextureName(opt->SplatDetailMapNames[alpha_pass++]); 
                      
                }      
                

                // Now that we have all the resources in place, we load the material
                mMaterial->setLodLevels(opt->lodMaterialDistanceList);
                mMaterial->setLightingEnabled(opt->lit);
                mMaterial->load(); 
            }
	    } // if (mMaterial.isNull())
        for (uint i = 0; i < 3; i++)
        {
            isChannelNeedUpdate[i] = false;
            isChannelModified[i] = false;
        }
    }

    //-----------------------------------------------------------------------
    void PagingLandScapeTexture_Splatting7Edit::update()
    {    
        assert (!mMaterial.isNull() && "PagingLandScapeTexture_Splatting2Edit::update()");

        // at least deformed once, so need to save texture if asked by user (option)
        mIsModified = true; 

        
        Image::Box rect (0, 0, 0, 0, 0, 1);

        // computes deformation
        PagingLandScapeData2D *data;
        const bool isDeformed = mIsDeformRectModified;
        if (isDeformed)
        {
            rect = mDeformRect;
            data = mParent->getSceneManager()->getData2DManager()->getData2D(mDataX, mDataZ);
//            rect = data->getDeformationRectangle ();
            if (rect.getWidth() && rect.getHeight ())
            {
                rect.right += 1;
                rect.bottom += 1;

                // Do here some alpha map modification based on deformation
                // see computeInstantBase(data, rect); of instant base texture.
            }
        }
        // try to upload only the smallest rectangle containing modification
        if (mIsPaintRectModified)
        {
            if (isDeformed)
            {
                rect.left = std::min (mPaintRect.left, rect.left);
                rect.right = std::max (mPaintRect.right, rect.right);
                rect.top =  std::min (mPaintRect.top, rect.top);
                rect.bottom = std::max (mPaintRect.bottom, rect.bottom);
            } // if (mNeedUpdate)
            else
            {
                rect = mPaintRect;
                rect.right += 1;
                rect.bottom += 1;
            }
        } // if (mIsRectModified)

        // Upload any changes (deformation or)
        if (rect.getWidth() && rect.getHeight ())
        {

            for (uint i = 0; i < 3; i++)
            {
                if (isChannelNeedUpdate[i])
                {
                    const PixelBox srcBox = mImages[i].getPixelBox().getSubVolume(rect);	
                    const PixelBox lock = mBuffers[i]->lock(rect, HardwareBuffer::HBL_DISCARD); 
                    PixelUtil::bulkPixelConversion(srcBox, lock); 
                    mBuffers[i]->unlock();  	

                    isChannelNeedUpdate[i] = false;
                    isChannelModified[i] = true;
                }
            }
        } // if (rect.getWidth() && rect.getHeight ())

        if (isDeformed)
            data->resetDeformationRectangle ();

        PagingLandScapeTexture::updated ();
    }
    //-----------------------------------------------------------------------
    void  PagingLandScapeTexture_Splatting7Edit::paint (const uint x, const uint z, 
        const Real paintForce, const uint mPaintChannel)
    {
        assert (mPaintChannel < 3);
        uchar * const BaseData = mImages[mPaintChannel].getData();

        assert (BaseData && "PagingLandScapeTexture_Splatting2Edit::paint()");

        const Real blend = paintForce;
        assert (paintForce >= 0.0f && paintForce <= 1.0f && "PagingLandScapeTexture_Splatting2Edit::paint()");
        const Real invBlend = 1.0f - blend;
    
    
        const size_t psize = mParent->getOptions()->PageSize - 1;
        assert ((x + z*  psize < psize*psize) && "PagingLandScapeTexture_Splatting2Edit::paint()");

        const uint curr_image_pos = static_cast <uint> (x + z * psize);
        const  uchar previousValue = BaseData[ curr_image_pos ];
        const  uchar newValue = static_cast <uchar> (255 * blend + previousValue * invBlend);

        if (newValue > 0)
            BaseData[ curr_image_pos ] = newValue;
        else
            BaseData[ curr_image_pos ] = 0;

        const uchar AlphaChange = previousValue -  BaseData[ curr_image_pos ];

        for (uint i = 0; i < 3; i++)
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
        } // for (uint i = 0; i < 3; i++)

        isChannelNeedUpdate[mPaintChannel] = true;
        adjustPaintRectangle(x, z);  
    }
 //-----------------------------------------------------------------------
    void PagingLandScapeTexture_Splatting7Edit::_unloadMaterial()
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

            for (uint i = 0; i < 3; i++)
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
            } // for (uint i = 0; i < 3; i++)
        } // if (mIsModified)

        for (uint i = 0; i < 3; i++)
        {
            mBuffers[i].setNull ();
            mImages[i].loadDynamicImage (0, 0, 0, PF_BYTE_RGB);  
            mTextures[i].setNull();
        }
  }
} //namespace
