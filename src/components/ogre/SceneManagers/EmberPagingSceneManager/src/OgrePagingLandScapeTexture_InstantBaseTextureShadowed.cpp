/***************************************************************************
OgrePagingLandScapeTexture_InstantBaseTextureShadowed.cpp  -  description
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

#include "OgrePagingLandScapeOptions.h"
#include "OgrePagingLandScapeTexture.h"
#include "OgrePagingLandScapeTexture_InstantBaseTextureShadowed.h"
#include "OgrePagingLandScapeData2DManager.h"
#include "OgrePagingLandScapeData2D.h"

#include "fileutils.h"

namespace Ogre
{

    uint PagingLandScapeTexture_InstantBaseTextureShadowed::mPageSize = 0;
	Image PagingLandScapeTexture_InstantBaseTextureShadowed::mImage;
    Real PagingLandScapeTexture_InstantBaseTextureShadowed::heights[4];
    Real PagingLandScapeTexture_InstantBaseTextureShadowed::dividers[4];
    ColourValue PagingLandScapeTexture_InstantBaseTextureShadowed::colors[4];

    //-----------------------------------------------------------------------
    void PagingLandScapeTexture_InstantBaseTextureShadowed::_setPagesize ()
    {
		PagingLandScapeOptions * const opt = PagingLandScapeOptions::getSingletonPtr();
        mPageSize = opt->PageSize - 1;
        Real Texturescale = opt->TextureStretchFactor;
        const uint textureSize = (uint) (mPageSize * Texturescale);
        uchar *data = new uchar[textureSize * textureSize * 3];
		mImage.loadDynamicImage (data, textureSize, textureSize, PF_BYTE_RGB);


        colors[0] = opt->matColor[0];
        colors[1] = opt->matColor[1];
        colors[2] = opt->matColor[2];
        colors[3] = opt->matColor[3];

        //  slope[] ??

        heights[0] = 0.0f;
        heights[1] = opt->matHeight[1];
        heights[2] = opt->matHeight[2];
        heights[3] = PagingLandScapeData2DManager::getSingleton().getMaxHeight ();

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
    PagingLandScapeTexture* PagingLandScapeTexture_InstantBaseTextureShadowed::newTexture()
    {
        return new PagingLandScapeTexture_InstantBaseTextureShadowed();
    }
    //-----------------------------------------------------------------------
    bool PagingLandScapeTexture_InstantBaseTextureShadowed::TextureRenderCapabilitesFullfilled()
    {
		if (PagingLandScapeOptions::getSingleton().NumMatHeightSplat > 3)
			return true;
		else
			return false;
    }
    //-----------------------------------------------------------------------
    void PagingLandScapeTexture_InstantBaseTextureShadowed::_clearData ()
    {      
		delete [] mImage.getData ();    
		mImage.loadDynamicImage (0, 0, 0, PF_BYTE_RGB);    
    }
    //-----------------------------------------------------------------------
    PagingLandScapeTexture_InstantBaseTextureShadowed::PagingLandScapeTexture_InstantBaseTextureShadowed() : PagingLandScapeTexture()
    {
    }
    //-----------------------------------------------------------------------
    PagingLandScapeTexture_InstantBaseTextureShadowed::~PagingLandScapeTexture_InstantBaseTextureShadowed()
    {
	    
    }
    //-----------------------------------------------------------------------
    void PagingLandScapeTexture_InstantBaseTextureShadowed::_loadMaterial()
    {
	    if (mMaterial.isNull())
        {
			PagingLandScapeOptions * const opt = PagingLandScapeOptions::getSingletonPtr();

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
                    templateMaterial = MaterialManager::getSingleton ().
                        getByName (String ("InstantBaseMaterialVertexPixelShadedShadowed"));
                    
		            // Create a new texture using the base image
		            mMaterial = templateMaterial->clone(matname, true, groupName);    

                    GpuProgramParametersSharedPtr params = mMaterial->getTechnique(0)->getPass(0)->getVertexProgramParameters();
    	            
//                    params->setNamedConstant("FogSettings", Vector4(opt->scale.x * opt->PageSize,
//                                                                    opt->scale.y / 65535, 
//                                                                    opt->scale.z * opt->PageSize, 
//                                                                    0.0f));
                     // Check to see if custom parameter is already there
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


                    const String texname (filename +  ".HSP." + commonName + "." + extname);   
                    TextureManager::getSingleton().load (texname, groupName);  
	                mMaterial->getTechnique(0)->getPass(0)->getTextureUnitState(2)->setTextureName (texname);
                    mPositiveShadow = true;
                }
                else
                {
                    templateMaterial = MaterialManager::getSingleton().getByName(String ("InstantBaseMaterialShadowed"));

		            // Create a new texture using the base image
		            mMaterial = templateMaterial->clone (matname, true, groupName);

                    mShadow.load(filename +
                                    ".HS." +
                                    commonName + "." +
                                    extname, 
                                    groupName); 
                    const uint mTextureSize = mPageSize * opt->TextureStretchFactor;//mShadow.getHeight();
                    uchar *lightdata = new uchar [mTextureSize*mTextureSize];
                    mLightImage.loadDynamicImage (lightdata, 
                                            mTextureSize, 
                                            mTextureSize, 
                                            1, 
                                            PF_L8, 
                                            true); 
                    computeInstantBaselight();     
                    finalTexName = filename +  ".L." + commonName + "." + extname;
					
					// debug only save.
                    //mLightImage.save(finalTexName);

                    mLightTexture = TextureManager::getSingleton().loadImage(finalTexName, 
                                    groupName,
                                    mLightImage);
                    mLightBuffer = mLightTexture->getBuffer ();
                      
                    mMaterial->getTechnique(0)->getPass(0)->getTextureUnitState(2)->setTextureName (finalTexName);
                    

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
    void PagingLandScapeTexture_InstantBaseTextureShadowed::_LoadTexture(const String &TexName,
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
    void PagingLandScapeTexture_InstantBaseTextureShadowed::update()
    {    
        assert (mImage.getData() && "PagingLandScapeTexture_InstantBaseTextureShadowed::update()");
        assert (!mMaterial.isNull() && "PagingLandScapeTexture_InstantBaseTextureShadowed::update()");
        assert (!mTexture.isNull() && "PagingLandScapeTexture_InstantBaseTextureShadowed::update()");
        assert (!mBuffer.isNull() && "PagingLandScapeTexture_InstantBaseTextureShadowed::update()");

        // at least deformed once, so need to save texture if asked by user (option)
        mIsModified = true; 

        
        Image::Box dataRect (0, 0, 0, 0, 0, 1);
        Image::Box texturerect (0, 0, 0, 0, 0, 1);

        // computes deformation
        PagingLandScapeData2D *data;
        if (mIsDeformRectModified)
        {
            dataRect = mDeformRect;
            data = PagingLandScapeData2DManager::getSingleton().getData2D(mDataX, mDataZ);
//            rect = data->getDeformationRectangle ();
            if (dataRect.getWidth() && dataRect.getHeight ())
            {
                const Real textureScale = PagingLandScapeOptions::getSingleton().TextureStretchFactor;
                texturerect.left = dataRect.left * textureScale;
                texturerect.right = dataRect.right *  textureScale;
                texturerect.top  = dataRect.top *  textureScale;
                texturerect.bottom = dataRect.bottom *  textureScale;

                dataRect.right += 1;
                dataRect.bottom += 1;

                texturerect.right += 1;
                texturerect.bottom += 1;

                computeInstantBase(data, dataRect, texturerect);
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

                const Real textureScale = PagingLandScapeOptions::getSingleton().TextureStretchFactor;
                
                texturerect.left = dataRect.left *  textureScale;
                texturerect.right = dataRect.right *  textureScale;
                texturerect.top  = dataRect.top *  textureScale;
                texturerect.bottom = dataRect.bottom *  textureScale;

                dataRect.right += 1;
                dataRect.bottom += 1;

                texturerect.right += 1;
                texturerect.bottom += 1;
            }
        } // if (mIsRectModified)


        // Upload any changes (deformation or)
        if (texturerect.getWidth() && texturerect.getHeight ())
        {
            const PixelBox srcBox = mImage.getPixelBox().getSubVolume(texturerect);	
            const PixelBox lock = mBuffer->lock(texturerect, HardwareBuffer::HBL_DISCARD); 
            PixelUtil::bulkPixelConversion(srcBox, lock); 
            mBuffer->unlock();  

            #ifdef _Missed_Spot                
                        // debug can help finding missed spot.
                        const PixelBox srcBox = mImage.getPixelBox();	
                        const uint mTextureSize = mPageSize * PagingLandScapeOptions::getSingleton().TextureStretchFactor;
                        const Image::Box rect (0, 0, 0, mTextureSize, mTextureSize, 1);
                        const PixelBox lock = mBuffer->lock (rect ,HardwareBuffer::HBL_DISCARD); 
                        PixelUtil::bulkPixelConversion(srcBox, lock); 
                        mBuffer->unlock();
            #endif //_Missed_Spot
        } 

        if (mIsDeformRectModified)
            data->resetDeformationRectangle ();
        PagingLandScapeTexture::updated ();
    }
    //-----------------------------------------------------------------------
    void PagingLandScapeTexture_InstantBaseTextureShadowed::paint (const uint x, const uint z, 
                                        const Real paintForce, const ColourValue &mPaintColor)
    {
        uchar * const BaseData = mImage.getData();

        assert (BaseData && "PagingLandScapeTexture_InstantBaseTextureShadowed::paint()");

        const Real blend = 1.0f;//;paintForce;
        assert (paintForce >= 0.0f && paintForce <= 1.0f && "PagingLandScapeTexture_InstantBaseTextureShadowed::paint()");
        const Real invBlend = 1.0f - blend;
    
    
//        Image::Box rect (x, z, x, z, 0, 1);
//
//        const PixelBox srcBox = mImage.getPixelBox().getSubVolume(rect);	
//        const PixelBox lock = mBuffer->lock(rect, HardwareBuffer::HBL_DISCARD); 
//        PixelUtil::bulkPixelConversion(srcBox, lock); 
//        mBuffer->unlock();  	
        assert ((x + z*  mPageSize < mPageSize*mPageSize) && "PagingLandScapeTexture_InstantBaseTextureShadowed::paint()");
        adjustPaintRectangle(x, z);

        const Real textureScale = PagingLandScapeOptions::getSingleton().TextureStretchFactor;

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
    void PagingLandScapeTexture_InstantBaseTextureShadowed::deformheight (const uint x, 
                                                                const uint z, 
                                                                const Real paintForce)
    {
        adjustDeformationRectangle(x, z);
    }
    //-----------------------------------------------------------------------
    void PagingLandScapeTexture_InstantBaseTextureShadowed::computeInstantBase (
        PagingLandScapeData2D *data, const Image::Box &dataRect, const Image::Box &textureRect) const
    {       
        uchar * const ogre_restrict BaseData = mImage.getData();
	    const Real * const ogre_restrict mHeightData = data->getHeightData ();

        assert (BaseData && "PagingLandScapeTexture_InstantBaseTextureShadowed::computeInstantBase()");
        assert (mHeightData && "PagingLandScapeTexture_InstantBaseTextureShadowed::computeInstantBase()");


        const size_t heightfiledsize = mPageSize + 1;

        const Real textureScale = PagingLandScapeOptions::getSingleton().TextureStretchFactor;

        const uint textureSize = mPageSize * textureScale;
        size_t curr_image_pos = textureRect.top*textureSize*3 + textureRect.left*3;
        const size_t image_width = (textureSize - (textureRect.right - textureRect.left))*3;

        const Real inv_scale = 1 / textureScale;
        const uchar bScale = 255;
        for (size_t k = textureRect.top; k < textureRect.bottom; ++k)
        {
            const uint k_terrain = (uint)(k * inv_scale);
            const uint curr_row = k_terrain*heightfiledsize;
		    for (size_t i = textureRect.left; i < textureRect.right; ++i)
            {             
                const uint i_terrain = (uint)(i * inv_scale);

                const Real height =  mHeightData[ i_terrain + curr_row ];

                uint indx = 1;
                while (height >= heights[indx])
                    indx++;                                

                const uint up_indx = indx;
                const uint down_indx = indx - 1;
                const Real interpol = (height  - heights[down_indx]) * dividers[up_indx];  
                //slope of current point (the y value of the normal)
                const Real Slope = 1.0f - data->getNormal (i_terrain, k_terrain).y; 

                assert (i < textureSize && k < textureSize && "PagingLandScapeTexture_InstantBaseTextureShadowed::computeinstantbase()");
                const uint mul = 255;
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
            curr_image_pos += image_width;
        }           
    }
    //-----------------------------------------------------------------------
    void PagingLandScapeTexture_InstantBaseTextureShadowed::lightUpdate()
    {
		PagingLandScapeOptions * const opt = PagingLandScapeOptions::getSingletonPtr();
        if (opt->VertexCompression)
        {
            const Real SunAngle = opt->SunAngle;
            const Vector3 SunDir = opt->Sun;
            
            const bool positiveHorizon =  (SunDir.y > 0);// Sun is west (true), east (false);
            assert (fabs (SunAngle) < 1.1f);

            const Real LightAngle =  (positiveHorizon)? SunAngle  : -SunAngle;
            if (positiveHorizon != mPositiveShadow)
            {
                const String texname  (opt->LandScape_filename
                                            +
                                            ((positiveHorizon)? String(".HSP.") : String(".HSN."))
                                            +
                                            StringConverter::toString(mDataZ) + 
                                            String(".") +
                                            StringConverter::toString(mDataX) +  "." + 
                                            opt->TextureExtension);
                TextureManager::getSingleton().load (texname, opt->groupName);   
                mMaterial->getTechnique(0)->getPass(0)->getTextureUnitState(2)->setTextureName (texname);                
                mPositiveShadow = positiveHorizon;
            }

            GpuProgramParametersSharedPtr params = mMaterial->getTechnique(0)->getPass(0)->getFragmentProgramParameters();    	            
            params->setNamedConstant("HorizonSettings", Vector4(SunDir.x,
                                                                SunDir.y, 
                                                                SunDir.z, 
                                                                LightAngle));
                   
        }
        else
        {
            computeInstantBaselight ();

            // Upload changes         
            const PixelBox srcBox = mLightImage.getPixelBox();	
            const uint mTextureSize = mPageSize * opt->TextureStretchFactor;
            const Image::Box rect (0, 0, 0, mTextureSize, mTextureSize, 1);
            const PixelBox lock = mLightBuffer->lock (rect ,HardwareBuffer::HBL_DISCARD); 
            PixelUtil::bulkPixelConversion(srcBox, lock); 
            mLightBuffer->unlock();
        }
    }
    //-----------------------------------------------------------------------
    void PagingLandScapeTexture_InstantBaseTextureShadowed::computeInstantBaselight () const
    {    
		PagingLandScapeOptions * const opt = PagingLandScapeOptions::getSingletonPtr();
        const Vector3 LightDir = opt->Sun;
        const Real SunAngle = opt->SunAngle;
            
        const bool positiveHorizon =  (LightDir.y > 0);// Sun is west (true), east (false);
        assert (fabs (SunAngle) < 1.1f);

        const Real LightAngle =  (positiveHorizon)? SunAngle  : -SunAngle;       
        const size_t offsetneg = (positiveHorizon)? 0: 1;
        //const Real LightAngle = SunAngle;

        
        uint curr_rowY = 0;
        const uchar BScale = 255;
        const Real uchardivider = 1.0f / BScale;
        uint curr_image_pos = 0;
        uchar * const ogre_restrict lightmap = (uchar *) (mLightImage.getData ());
        const uchar * const ogre_restrict HorizonAngle = mShadow.getData(); 
        const Real textureScale = opt->TextureStretchFactor;

        const uint mTextureSize = mPageSize * textureScale;
        const uint rowpitch = mTextureSize*3;
        for(uint nZ = 0; nZ < mTextureSize ; nZ++) 
        {
            uint curr_image_posX = 0;
            for(uint nX = 0; nX < mTextureSize; nX++)
            {            
                const uint nVert = curr_rowY + curr_image_posX  + offsetneg; 
                const Real hAngle = HorizonAngle[nVert] * uchardivider;
                if (hAngle < LightAngle) 
                {           
                    const Real intensity =  1 - (LightAngle - hAngle);
                    if (intensity > 0.0f) 
                    {                       
                        //intensity *= std::max(LightDir.dotProduct (data->getNormal (nX, nZ)), 0.0f);
                        lightmap[curr_image_pos] = static_cast <uchar> (intensity * BScale);
                        
					}
                    else
                    {
                        // totally in shadow    
                        lightmap[curr_image_pos] = 0;    
                    }
                }
                else             
                {
                    // if Vertex is lighted
                    const Real intensity = BScale;           
                    //const Real intensity = BScale * std::min(1.0f, LightDir.dotProduct (data->getNormal (nX, nZ))); 
                    lightmap[curr_image_pos] = static_cast <uchar> (intensity);
                 }               
                // if colored light should use a rgb map..

                curr_image_pos ++;
                curr_image_posX += 3;
            }
            curr_rowY += rowpitch;
        }    
        
    }
    //-----------------------------------------------------------------------
    void PagingLandScapeTexture_InstantBaseTextureShadowed::_unloadMaterial()
    {
        assert (!mMaterial.isNull() && "PagingLandScapeTexture_InstantBaseTextureShadowed::::_unloadMaterial");    
        if (mIsModified)
        { 

		    PagingLandScapeOptions * const opt = PagingLandScapeOptions::getSingletonPtr();
            const Real Texturescale = opt->TextureStretchFactor;
            const Image::Box datarect (0, 0, 0, mPageSize*Texturescale, mPageSize*Texturescale, 1);
            computeInstantBase(PagingLandScapeData2DManager::getSingleton().getData2D(mDataX, mDataZ), 
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
