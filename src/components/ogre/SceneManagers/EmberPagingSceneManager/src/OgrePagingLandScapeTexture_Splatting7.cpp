/***************************************************************************
OgrePagingLandScapeTexture_Splatting7.cpp  -  description
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
#include "OgrePagingLandScapeTexture_Splatting7.h"



namespace Ogre
{
    //-----------------------------------------------------------------------
    void PagingLandScapeTexture_Splatting7::_setPagesize(void)
    {
        PagingLandScapeOptions::getSingleton().VertexCompression = false;
        PagingLandScapeOptions::getSingleton().lodMorph = false;
    }
    //-----------------------------------------------------------------------
    void PagingLandScapeTexture_Splatting7::_clearData(void)
    {
    
    }
    //-----------------------------------------------------------------------
    PagingLandScapeTexture* PagingLandScapeTexture_Splatting7::newTexture()
    {
        return new PagingLandScapeTexture_Splatting7();
    }
    //-----------------------------------------------------------------------
    bool PagingLandScapeTexture_Splatting7::TextureRenderCapabilitesFullfilled()
    {        
		const PagingLandScapeOptions * const opt = PagingLandScapeOptions::getSingletonPtr();
		
		if (opt->NumMatHeightSplat < 2)
			return false;
        if (opt->numTextureUnits < 6)
            return false;
            
        return true;
    }
    //-----------------------------------------------------------------------
    PagingLandScapeTexture_Splatting7::PagingLandScapeTexture_Splatting7() : PagingLandScapeTexture()
    {
    }

    //-----------------------------------------------------------------------
    PagingLandScapeTexture_Splatting7::~PagingLandScapeTexture_Splatting7()
    {
    }
    //-----------------------------------------------------------------------
    void PagingLandScapeTexture_Splatting7::LoadAlphaMap(const String &filename) const
    {
        TexturePtr tex = TextureManager::getSingleton().getByName (filename);
        if (tex.isNull())
        {
	        Image Imageloader;
            const String group = PagingLandScapeOptions::getSingleton().groupName;
            Imageloader.load (filename, group); 
            Image ImageConvertertoAlphaFormat;
            ImageConvertertoAlphaFormat.loadDynamicImage(Imageloader.getData(), 
                                                    Imageloader.getWidth(), 
                                                    Imageloader.getHeight(), 1, PF_A8, false);

            TextureManager::getSingleton().loadImage (filename, 
                                                    group,
                                                    ImageConvertertoAlphaFormat);
        }
    }
    //-----------------------------------------------------------------------
    void PagingLandScapeTexture_Splatting7::_loadMaterial()
    {
	    if (mMaterial.isNull())
	    {
			const PagingLandScapeOptions * const opt = PagingLandScapeOptions::getSingletonPtr();
            
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
                        + StringConverter::toString(alpha_pass) 
                        + alphamapEndname; 
                LoadAlphaMap (texname);
                
                // assign the splat material                               
                p->getTextureUnitState(splat_pass++)->setTextureName(opt->SplatDetailMapNames[alpha_pass++]);
                // assign this texture to the material
                p->getTextureUnitState(splat_pass++)->setTextureName(texname);

                const uint numSplats = 6;
                while  (splat_pass < numSplats)
                {            
                    // Create a new texture using the alpha map
                    texname = alphamapBeginname 
                            + StringConverter::toString(alpha_pass) 
                            + alphamapEndname; 
                    LoadAlphaMap (texname);
                    
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
	    }
    }
} //namespace
