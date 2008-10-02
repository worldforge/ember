/***************************************************************************
OgrePagingLandScapeTexture_InstantBaseTextureShadowed.cpp  -  description
-------------------
begin                : Mon Apr 26 2004
copyright            : (C) 2003-2006 by Jose A Milan & Tuan Kuranes
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
#include "OgrePagingLandScapeTexture_InstantBaseTextureShadowed.h"
#include "OgrePagingLandScapeData2DManager.h"
#include "OgrePagingLandScapeData2D.h"

#include "fileutils.h"

namespace Ogre
{
    
    //-----------------------------------------------------------------------
    PagingLandScapeTexture* PagingLandScapeTexture_InstantBaseTextureShadowed::newTexture()
    {
        return new PagingLandScapeTexture_InstantBaseTextureShadowed(mParent);
    }
    //-----------------------------------------------------------------------
    bool PagingLandScapeTexture_InstantBaseTextureShadowed::isMaterialSupported()
    {
		if (mParent->getOptions()->NumMatHeightSplat > 3)
			return true;
		else
			return false;
    }
    //-----------------------------------------------------------------------
    PagingLandScapeTexture_InstantBaseTextureShadowed::PagingLandScapeTexture_InstantBaseTextureShadowed(PagingLandScapeTextureManager *textureMgr) 
        :
		PagingLandScapeTexture(textureMgr, "InstantBaseShadowed",1, false)
	{
		mIsShadowed = true;
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
			PagingLandScapeOptions * const opt = mParent->getOptions();

            const String filename  = opt->LandScape_filename;
            const String extname   = opt->TextureExtension;
            const String groupName = opt->groupName;

            const String commonName = StringConverter::toString(mDataZ) + 
                                        String(".") +
                                        StringConverter::toString(mDataX);
            const String matname = String("InstantBase.") + commonName + filename;
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
                if (opt->VertexCompression && opt->hasFragmentShader)
                {
                    templateMaterial = MaterialManager::getSingleton ().
                        getByName (String ("InstantBaseShadowedDecompress"));
                    
	                // Create a new texture using the base image
	                mMaterial = templateMaterial->clone(matname, true, groupName);    

                    Pass *p = mMaterial->getTechnique(0)->getPass(0);
                    bindCompressionSettings (p->getVertexProgramParameters());
                    bindCompressionSettings (p->getShadowReceiverVertexProgramParameters ());


                    const String texname (filename +  ".HSP." + commonName + "." + extname);  
                    mMaterial->getTechnique(0)->getPass(0)->getTextureUnitState(2)->setTextureName (texname);
                    mPositiveShadow = true;                   
                }
                else
                {
                    templateMaterial = MaterialManager::getSingleton().getByName(String ("InstantBaseShadowed"));

		            // Create a new texture using the base image
		            mMaterial = templateMaterial->clone (matname, true, groupName);

                    mShadow.load(filename +
                                    ".HS." +
                                    commonName + "." +
                                    extname, 
                                    groupName); 
                    const unsigned int mTextureSize = static_cast<unsigned int>(mParent->mPageSize * opt->TextureStretchFactor);//mShadow.getHeight();
                    uchar *lightdata = new uchar [mTextureSize*mTextureSize];
                    mLightImage.loadDynamicImage (lightdata, 
                                            mTextureSize, 
                                            mTextureSize, 
                                            1, 
                                            PF_L8, 
                                            true); 
                    computeLightMap();     
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
                loadColorTexture(finalTexName, 0);
	            mMaterial->getTechnique(0)->getPass(0)->getTextureUnitState(0)->setTextureName (finalTexName);
            
            }
            else
            {
                loadColorTexture(finalTexName, 0);
            }
           
        }
    }
    
} //namespace
