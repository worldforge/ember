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
#include "OgrePagingLandScapeTextureManager.h"
#include "OgrePagingLandScapeTexture_Splatting4.h"


namespace Ogre
{
    //-----------------------------------------------------------------------
    void PagingLandScapeTexture_Splatting4::_setPagesize(void)
    {
        mParent->getOptions()->VertexCompression = false;
        mParent->getOptions()->lodMorph = false;
    }
    //-----------------------------------------------------------------------
    void PagingLandScapeTexture_Splatting4::_clearData(void)
    {
    
    }
    //-----------------------------------------------------------------------
    PagingLandScapeTexture* PagingLandScapeTexture_Splatting4::newTexture()
    {
        return new PagingLandScapeTexture_Splatting4(mParent);
    }
    //-----------------------------------------------------------------------
    bool PagingLandScapeTexture_Splatting4::TextureRenderCapabilitesFullfilled()
    {                                                        
		const PagingLandScapeOptions * const opt = mParent->getOptions();
		
		if (opt->NumMatHeightSplat < 3)
			return false;
        if (opt->numTextureUnits < 2)
            return false;
            
        return true;
    }
    //-----------------------------------------------------------------------
    PagingLandScapeTexture_Splatting4::PagingLandScapeTexture_Splatting4(PagingLandScapeTextureManager *textureMgr) : PagingLandScapeTexture(textureMgr)
    {
    }

    //-----------------------------------------------------------------------
    PagingLandScapeTexture_Splatting4::~PagingLandScapeTexture_Splatting4()
    {
    }
    //-----------------------------------------------------------------------
    void PagingLandScapeTexture_Splatting4::_loadMaterial()
    {
	    if (mMaterial.isNull())
	    {
			const PagingLandScapeOptions * const opt = mParent->getOptions();
            
            // Create a new texture using the base image
            const String filename (opt->LandScape_filename);
            const String commonName (StringConverter::toString(mDataZ) + 
                                        String(".") +
                                        StringConverter::toString(mDataX));
            const String matname (String("SplattingMaterial4.") + commonName + filename);
                                
            mMaterial = MaterialManager::getSingleton().getByName(matname);
            if (mMaterial.isNull())
	        {
                const String extname (opt->TextureExtension);
                const String endName ("." + commonName + "." + extname);
                
                mMaterial = MaterialManager::getSingleton().getByName("SplattingMaterial4");
                assert (!mMaterial.isNull());
                mMaterial = mMaterial->clone(matname);

                // loads images
                
                String texname (filename + ".Base" + endName);
                Technique * const t = mMaterial->getTechnique(0);
                    
                TextureManager::getSingleton().load (texname, opt->groupName);    
                t->getPass(0)->getTextureUnitState(0)->setTextureName(texname);
                mMaterial->getTechnique(1)->getPass(0)->getTextureUnitState(0)->setTextureName(texname);

                texname = filename + ".Light" + endName;
                TextureManager::getSingleton().load (texname, opt->groupName);    
                t->getPass(0)->getTextureUnitState(1)->setTextureName(texname);
                mMaterial->getTechnique(1)->getPass(0)->getTextureUnitState(1)->setTextureName(texname);
               
                const String alphamapBeginname = filename + ".Alpha."; 
                
                const uint numSplats = opt->NumMatHeightSplat;
                uint splat_pass = 0; 
                while  (splat_pass < numSplats)
                {            
                    // Create a new texture using the alpha map
                    texname = alphamapBeginname 
                            + StringConverter::toString(splat_pass) 
                            + endName;                     
                    // assign this texture to the material
                    t->getPass(splat_pass + 1)->getTextureUnitState(1)->setTextureName(texname);
                    // assign the splat material
                    t->getPass(splat_pass + 1)->getTextureUnitState(0)->setTextureName(opt->SplatDetailMapNames[splat_pass]); 
                
                    splat_pass++;
                }  
               
                // Now that we have all the resources in place, we load the material
                mMaterial->setLodLevels(opt->lodMaterialDistanceList);
	            mMaterial->setLightingEnabled(false);
                mMaterial->load();
            }
	    }
    }
} //namespace
