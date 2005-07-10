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

#include "OgrePagingLandScapeOptions.h"
#include "OgrePagingLandScapeTexture.h"
#include "OgrePagingLandScapeTexture_Splatting7Edit.h"


#include "OgrePagingLandScapeData2DManager.h"
#include "OgrePagingLandScapeData2D.h"

#include "fileutils.h"

namespace Ogre
{
    //-----------------------------------------------------------------------
    void PagingLandScapeTexture_Splatting7Edit::_setPagesize( void )
    {
        PagingLandScapeOptions::getSingleton().VertexCompression = false;
        PagingLandScapeOptions::getSingleton().lodMorph = false;
    }
    //-----------------------------------------------------------------------
    void PagingLandScapeTexture_Splatting7Edit::_clearData( void )
    {
    
    }
    //-----------------------------------------------------------------------
    PagingLandScapeTexture* PagingLandScapeTexture_Splatting7Edit::newTexture( )
    {
        return new PagingLandScapeTexture_Splatting7Edit();
    }
    //-----------------------------------------------------------------------
    bool PagingLandScapeTexture_Splatting7Edit::TextureRenderCapabilitesFullfilled()
    {
        
        if (PagingLandScapeOptions::getSingleton().numTextureUnits >= 6)
            return true;
        return false;
    }
    //-----------------------------------------------------------------------
    PagingLandScapeTexture_Splatting7Edit::PagingLandScapeTexture_Splatting7Edit() : PagingLandScapeTexture()
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
                const String group = PagingLandScapeOptions::getSingleton().groupName;
                Imageloader.load (filename, group);
                const size_t psize = PagingLandScapeOptions::getSingleton().PageSize - 1;

                uchar *data = new uchar [psize*psize];
                memcpy (data, Imageloader.getData(), psize*psize*sizeof(uchar));
                mImages[channel].loadDynamicImage(data, psize, psize, 1, PF_A8, true);

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
	    if (mMaterial.isNull() )
	    {
            // Create a new texture using the base image
            const String filename = PagingLandScapeOptions::getSingleton().landscape_filename;
            const String commonName = StringConverter::toString(mDataZ) + 
                                        String(".") +
                                        StringConverter::toString(mDataX);
            const String matname = String("SplattingMaterial7.") + commonName + filename;
            mMaterial = MaterialManager::getSingleton().getByName(matname);
	        if (mMaterial.isNull() )
	        {
                mMaterial = MaterialManager::getSingleton().getByName("SplattingMaterial7");
                mMaterial = mMaterial->clone(matname);
                   

                const String extname = PagingLandScapeOptions::getSingleton().TextureExtension;

                String texname = filename + ".Base." +
                    commonName + "." + extname;
              
                // assign this texture to the material
                //mMaterial->getTechnique(0)->getPass(0)->getTextureUnitState(0)->setTextureName(texname);
                mMaterial->getTechnique(1)->getPass(0)->getTextureUnitState(0)->setTextureName(texname);

                const String alphamapname = ".Alpha.";

                // Create a new texture using the 1st alpha map
                texname = filename + alphamapname + "0." + 
                        commonName + "." + extname;   
                 
                LoadAlphaMap (texname, 0);
                // assign this texture to the material
                mMaterial->getTechnique(0)->getPass(0)->getTextureUnitState(1)->setTextureName(texname);

                // Create a new texture using the 2nd alpha map
                texname = filename + alphamapname + "1." + 
                        commonName + "." + extname;   
                  
                LoadAlphaMap (texname, 1);
                // assign this texture to the material
                mMaterial->getTechnique(0)->getPass(0)->getTextureUnitState(2)->setTextureName(texname);

                // Create a new texture using the 3rd alpha map
                texname = filename + alphamapname + "2." + 
                        commonName + "." + extname;   
                  
                LoadAlphaMap (texname, 2);
                // assign this texture to the material
                mMaterial->getTechnique(0)->getPass(0)->getTextureUnitState(4)->setTextureName(texname);

                // Now that we have all the resources in place, we load the material
                mMaterial->load(); 
                mMaterial->setLightingEnabled( PagingLandScapeOptions::getSingleton().lit );
            }
	    } // if (mMaterial.isNull() )
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
            data = PagingLandScapeData2DManager::getSingleton().getData2d(mDataX, mDataZ);
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

        // Upload any changes (deformation or )
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
    
    
        const size_t psize = PagingLandScapeOptions::getSingleton().PageSize - 1;
        assert ((x + z*  psize < psize*psize) && "PagingLandScapeTexture_Splatting2Edit::paint()");

        const uint curr_image_pos = x + z * psize;
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
            const String Alphafname = PagingLandScapeOptions::getSingleton().landscape_filename + 
                                        ".Alpha.";

            const String PageNumberName = String(".") + StringConverter::toString(mDataZ) + 
                                                String(".") + 
                                        StringConverter::toString(mDataX) + 
                                        String(".");
            const String extname = PagingLandScapeOptions::getSingleton().TextureExtension;

            for (uint i = 0; i < 3; i++)
            {
                if (isChannelModified[i])
                {
                        
                    const String fname = Alphafname + StringConverter::toString(i) + PageNumberName;

                    FileInfoListPtr finfo =  ResourceGroupManager::getSingleton().findResourceFileInfo (
                            PagingLandScapeOptions::getSingleton().groupName, 
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
