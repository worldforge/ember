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

#include "OgrePagingLandScapeOptions.h"
#include "OgrePagingLandScapeTexture.h"
#include "OgrePagingLandScapeTexture_Splatting2.h"


namespace Ogre
{
    //-----------------------------------------------------------------------
    void PagingLandScapeTexture_Splatting2::_setPagesize(void)
    {
        PagingLandScapeOptions::getSingleton().VertexCompression = false;
        PagingLandScapeOptions::getSingleton().lodMorph = false;
    }
    //-----------------------------------------------------------------------
    void PagingLandScapeTexture_Splatting2::_clearData(void)
    {
    
    }
    //-----------------------------------------------------------------------
    PagingLandScapeTexture* PagingLandScapeTexture_Splatting2::newTexture()
    {
        return new PagingLandScapeTexture_Splatting2();
    }
    //-----------------------------------------------------------------------
    bool PagingLandScapeTexture_Splatting2::TextureRenderCapabilitesFullfilled()
    {             
		const PagingLandScapeOptions * const opt = PagingLandScapeOptions::getSingletonPtr();
		
		if (opt->NumMatHeightSplat < 3)
			return false;
        if (opt->numTextureUnits < 2)
            return false;
            
        return true;
    }
    //-----------------------------------------------------------------------
    PagingLandScapeTexture_Splatting2::PagingLandScapeTexture_Splatting2() : PagingLandScapeTexture()
    {
    }
    //-----------------------------------------------------------------------
    PagingLandScapeTexture_Splatting2::~PagingLandScapeTexture_Splatting2()
    {
    }
    //-----------------------------------------------------------------------
    void PagingLandScapeTexture_Splatting2::LoadAlphaMap(const String &filename) const
    {
       TexturePtr tex = TextureManager::getSingleton().getByName (filename);
       if (tex.isNull())
       {
	        Image Imageloader;
			const PagingLandScapeOptions * const opt = PagingLandScapeOptions::getSingletonPtr();
            const String group = opt->groupName;
            Imageloader.load (filename, group);
            Image ImageConvertertoAlphaFormat;
            ImageConvertertoAlphaFormat.loadDynamicImage(Imageloader.getData(), 
                                                            Imageloader.getWidth(), 
                                                            Imageloader.getHeight(), 
                                                            1, PF_A8, false);

            TextureManager::getSingleton().loadImage (filename, 
                                                    group,
                                                    ImageConvertertoAlphaFormat);
       }
    }
    //-----------------------------------------------------------------------
    void PagingLandScapeTexture_Splatting2::_loadMaterial()
    {
	    if (mMaterial.isNull())
	    {
			const PagingLandScapeOptions * const opt = PagingLandScapeOptions::getSingletonPtr();
            const String filename (opt->LandScape_filename);
            const String commonName (StringConverter::toString(mDataZ) + 
                                        String(".") +
                                        StringConverter::toString(mDataX));
                                        
            const String matname (String("SplattingMaterial2.") + commonName + filename);
		    mMaterial = MaterialManager::getSingleton().getByName(matname);
            if (mMaterial.isNull())
	        {
                const String extname (opt->getSingleton().TextureExtension);
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
                    LoadAlphaMap (texname);
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
    }
} //namespace
