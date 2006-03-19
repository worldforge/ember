/***************************************************************************
OgrePagingLandScapeTexture_InstantBaseTextureEdit.cpp  -  description
-------------------
begin                : Mon Apr 26 2004
copyright            : (C) 2002-2005 by Jose A Milan & Tuan Kuranes
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
#include "OgrePagingLandScapeTexture_InstantBaseTextureEdit.h"
#include "OgrePagingLandScapeData2DManager.h"
#include "OgrePagingLandScapeData2D.h"

#include "fileutils.h"

namespace Ogre
{
    uint PagingLandScapeTexture_InstantBaseTextureEdit::mPageSize = 0;
    Real PagingLandScapeTexture_InstantBaseTextureEdit::heights[4];
    Real PagingLandScapeTexture_InstantBaseTextureEdit::dividers[4];
    ColourValue PagingLandScapeTexture_InstantBaseTextureEdit::colors[4];

    //-----------------------------------------------------------------------
    void PagingLandScapeTexture_InstantBaseTextureEdit::_setPagesize ()
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
    PagingLandScapeTexture* PagingLandScapeTexture_InstantBaseTextureEdit::newTexture()
    {
        return new PagingLandScapeTexture_InstantBaseTextureEdit(mParent);
    }
    //-----------------------------------------------------------------------
    bool PagingLandScapeTexture_InstantBaseTextureEdit::TextureRenderCapabilitesFullfilled()
    {
		if (mParent->getOptions()->NumMatHeightSplat > 3)
			return true;
		else
			return false;
    }
    //-----------------------------------------------------------------------
    void PagingLandScapeTexture_InstantBaseTextureEdit::_clearData ()
    {      
		  
    }
    //-----------------------------------------------------------------------
    PagingLandScapeTexture_InstantBaseTextureEdit::PagingLandScapeTexture_InstantBaseTextureEdit(PagingLandScapeTextureManager *textureMgr) : PagingLandScapeTexture(textureMgr)
    {
    }
    //-----------------------------------------------------------------------
    PagingLandScapeTexture_InstantBaseTextureEdit::~PagingLandScapeTexture_InstantBaseTextureEdit()
    {
	    
    }
    //-----------------------------------------------------------------------
    void PagingLandScapeTexture_InstantBaseTextureEdit::_loadMaterial()
    {
	    if (mMaterial.isNull())
        {
            const String filename  = mParent->getOptions()->LandScape_filename;
            const String extname   = mParent->getOptions()->TextureExtension;
            const String groupName = mParent->getOptions()->groupName;

            const String commonName = StringConverter::toString(mDataZ) + 
                                        String(".") +
                                        StringConverter::toString(mDataX);
            const String matname = String("InstantBaseMaterial.") + commonName + filename;
            const String texname = filename + ".Base." + commonName + ".";

            String finalTexName;
            if (mParent->getOptions()->Deformable &&
                ResourceGroupManager::getSingleton().resourceExists(groupName, 
                                                texname + "modif." +extname))
            {
                finalTexName = texname + "modif." + extname;             
            }
            else
            {
                finalTexName = texname + extname;
            }

            if (!mBaseData)
            {
                mImage.load (finalTexName, groupName);
                mBaseData = mImage.getData();
            } 

            // check need of material loading
            mMaterial = MaterialManager::getSingleton().getByName(matname);
            if (mMaterial.isNull())
            {
                MaterialPtr templateMaterial;
                if (mParent->getOptions()->VertexCompression)
                {
                    templateMaterial = MaterialManager::getSingleton().getByName(String ("InstantBaseMaterialVertexPixelShaded"));
                    
		            // Create a new texture using the base image
		            mMaterial = templateMaterial->clone(matname, true, groupName);    

                    GpuProgramParametersSharedPtr params = mMaterial->getTechnique(0)->getPass(0)->getVertexProgramParameters();
    	            
//                    params->setNamedConstant("FogSettings", Vector4(mParent->getOptions()->scale.x * mParent->getOptions()->PageSize,
//                                                                    mParent->getOptions()->scale.y / 65535, 
//                                                                    mParent->getOptions()->scale.z * mParent->getOptions()->PageSize, 
//                                                                    0.0f));
                     // Check to see if custom param is already there
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
	            mMaterial->setLightingEnabled(mParent->getOptions()->lit);
            }
            else
                _LoadTexture(finalTexName, groupName);
           
        }
    }
    //-----------------------------------------------------------------------
    void PagingLandScapeTexture_InstantBaseTextureEdit::_LoadTexture(const String &TexName,
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
    void PagingLandScapeTexture_InstantBaseTextureEdit::update()
    {    
        assert (!mMaterial.isNull() && "PagingLandScapeTexture_InstantBaseTextureEdit::update()");
        assert (!mTexture.isNull() && "PagingLandScapeTexture_InstantBaseTextureEdit::update()");
        assert (!mBuffer.isNull() && "PagingLandScapeTexture_InstantBaseTextureEdit::update()");

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

               computeInstantBase(data, rect);
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
            const PixelBox srcBox = mImage.getPixelBox().getSubVolume(rect);	
            const PixelBox lock = mBuffer->lock(rect, HardwareBuffer::HBL_DISCARD); 
            PixelUtil::bulkPixelConversion(srcBox, lock); 
            mBuffer->unlock();  	
        } // if (rect.getWidth() && rect.getHeight ())

        if (isDeformed)
            data->resetDeformationRectangle ();

        PagingLandScapeTexture::updated ();
    }
    //-----------------------------------------------------------------------
    void PagingLandScapeTexture_InstantBaseTextureEdit::paint (const uint x, const uint z, 
                                        const Real paintForce, const ColourValue &mPaintColor)
    {
        uchar * const BaseData = mBaseData;

        assert (BaseData && "PagingLandScapeTexture_InstantBaseTextureEdit::paint()");

        const Real blend = paintForce;
        assert (paintForce >= 0.0f && paintForce <= 1.0f && "PagingLandScapeTexture_InstantBaseTextureEdit::paint()");
        const Real invBlend = 1.0f - blend;
    
    
        assert ((x + z*  mPageSize < mPageSize*mPageSize) && "PagingLandScapeTexture_InstantBaseTextureEdit::paint()");

        const uint curr_image_pos = x * 3+ z * mPageSize * 3;//x * 3 + z * mPageSize * 3;
    const uchar bScale = 255;
        BaseData[ curr_image_pos ]    = 
            static_cast <uchar> (bScale * mPaintColor.r * blend + BaseData[ curr_image_pos ] * invBlend);
        BaseData[ curr_image_pos + 1] = 
            static_cast <uchar> (bScale * mPaintColor.g * blend + BaseData[ curr_image_pos + 1] * invBlend);
        BaseData[ curr_image_pos + 2] = 
            static_cast <uchar> (bScale * mPaintColor.b * blend + BaseData[ curr_image_pos + 2] * invBlend);     
        adjustPaintRectangle(x, z);
    }
    //-----------------------------------------------------------------------
    void PagingLandScapeTexture_InstantBaseTextureEdit::deformheight (const uint x, 
                                                                const uint z, 
                                                                const Real paintForce)
    {
        adjustDeformationRectangle(x, z);
    }
    //-----------------------------------------------------------------------
    void PagingLandScapeTexture_InstantBaseTextureEdit::computeInstantBase (
        PagingLandScapeData2D *data, const Image::Box &rect) const
    {       
        uchar * const BaseData = mBaseData;
	    const Real * const mHeightData = data->getHeightData ();

        assert (BaseData && "PagingLandScapeTexture_InstantBaseTextureEdit::computeInstantBase()");
        assert (mHeightData && "PagingLandScapeTexture_InstantBaseTextureEdit::computeInstantBase()");


        const size_t heightfiledsize = mPageSize + 1;
        size_t curr_row = rect.top * heightfiledsize;

        size_t curr_image_pos = rect.top*mPageSize*3 + rect.left*3;
        const size_t image_width = (mPageSize - (rect.right - rect.left))*3;

		const uchar bScale = 255;
        for (size_t k = rect.top; k < rect.bottom; ++k)
        {
		    for (size_t i = rect.left; i < rect.right; ++i)
            {               
                const Real height =  mHeightData[ i + curr_row ];
                uint indx = 1;
                while (height >= heights[indx])
                    indx++;                                

                const uint up_indx = indx;
                const uint down_indx = indx - 1;
                const Real interpol = (height  - heights[down_indx]) * dividers[up_indx];  
                //slope of current point (the y value of the normal)
                const Real Slope = 1.0f - data->getNormal (i, k).y; 

                assert (i < mPageSize && k < mPageSize && "PagingLandScapeTexture_InstantBaseTextureEdit::computeinstantbase()");
                if (Slope < 0.05f)// speed-up as it's invisible
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
                    const Real A = (1.0f - Slope);
                    const Real B = A * (1.0f - interpol);
                    const Real C = A * interpol;
                    const Real D = Slope;
                   
                    // RGB = colors[indx - 1] * B + colors[indx] * C + colors[2] * D;                    
                    BaseData[ curr_image_pos ]     = static_cast <uchar> ((colors[down_indx].r * B + colors[up_indx].r * C + colors[2].r * D)* bScale);
                    BaseData[ curr_image_pos + 1 ] = static_cast <uchar> ((colors[down_indx].g * B + colors[up_indx].g * C + colors[2].g * D)* bScale);
                    BaseData[ curr_image_pos + 2 ] = static_cast <uchar> ((colors[down_indx].b * B + colors[up_indx].b * C + colors[2].b * D)* bScale);
                }
                curr_image_pos += 3;
            }
            curr_row += heightfiledsize;
            curr_image_pos += image_width;
        }           
    }
    //-----------------------------------------------------------------------
    void PagingLandScapeTexture_InstantBaseTextureEdit::computeInstantBaselight () const
    {
               
    }
    //-----------------------------------------------------------------------
    void PagingLandScapeTexture_InstantBaseTextureEdit::_unloadMaterial()
    {
        assert (!mMaterial.isNull() && "PagingLandScapeTexture_InstantBaseTextureEdit::::_unloadMaterial");    
        if (mIsModified)
        { 
            const String fname = mParent->getOptions()->LandScape_filename + 
                                ".Base." + 
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
        mBaseData = 0;
  }
} //namespace
