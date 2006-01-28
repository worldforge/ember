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
#include "OgrePagingLandScapeTexture_Splatting3.h"


namespace Ogre
{
    //-----------------------------------------------------------------------
    void PagingLandScapeTexture_Splatting3::_setPagesize(void)
    {
        PagingLandScapeOptions::getSingleton().VertexCompression = false;
        PagingLandScapeOptions::getSingleton().lodMorph = false;
        PagingLandScapeOptions::getSingleton().coverage_vertex_color = true;
        PagingLandScapeOptions::getSingleton().colored = true;
    }
    //-----------------------------------------------------------------------
    void PagingLandScapeTexture_Splatting3::_clearData(void)
    {
    
    }
    //-----------------------------------------------------------------------
    PagingLandScapeTexture* PagingLandScapeTexture_Splatting3::newTexture()
    {
        return new PagingLandScapeTexture_Splatting3();
    }
    //-----------------------------------------------------------------------
    bool PagingLandScapeTexture_Splatting3::TextureRenderCapabilitesFullfilled()
    {                                                           
		const PagingLandScapeOptions * const opt = PagingLandScapeOptions::getSingletonPtr();
		
		if (opt->NumMatHeightSplat < 3)
			return false; 
        if (!opt->isRenderGL)
            return false;         
        if (opt->numTextureUnits < 4)
            return false;
            
        return true;
    }
    //-----------------------------------------------------------------------
    PagingLandScapeTexture_Splatting3::PagingLandScapeTexture_Splatting3() : PagingLandScapeTexture()
    {
    }
    //-----------------------------------------------------------------------
    PagingLandScapeTexture_Splatting3::~PagingLandScapeTexture_Splatting3()
    {
    }
    //-----------------------------------------------------------------------
    void PagingLandScapeTexture_Splatting3::_loadMaterial()
    {
	    if (mMaterial.isNull())
	    {
			const PagingLandScapeOptions * const opt = PagingLandScapeOptions::getSingletonPtr();
           
            // Create a new texture using the base image
            // Create a new texture using the base image
            const String filename (opt->LandScape_filename);
            const String commonName (StringConverter::toString(mDataZ) + 
                                        String(".") +
                                        StringConverter::toString(mDataX));
            const String matname (String("SplattingMaterial3.") + commonName + filename);
            
            mMaterial = MaterialManager::getSingleton().getByName(matname);
	        if (mMaterial.isNull())
	        {
                mMaterial = MaterialManager::getSingleton().getByName("SplattingMaterial3");  
                assert (!mMaterial.isNull());
                mMaterial = mMaterial->clone(matname);
                
                const String extname (opt->TextureExtension);
                const String endName ("." + commonName + "." + extname);    
                
                Technique * const t = mMaterial->getTechnique(0);                         
                uint splat_pass = 0; 
                while  (splat_pass < 3)
                {             
                    // assign the splat material
                    t->getPass(0)->getTextureUnitState(splat_pass)->setTextureName(opt->SplatDetailMapNames[splat_pass]); 
                    splat_pass++;
                }  
                
                String texname (filename + ".Light" + endName);
                TextureManager::getSingleton().load (texname, opt->groupName);    
                mMaterial->getTechnique(0)->getPass(0)->getTextureUnitState(3)->setTextureName(texname);
                mMaterial->getTechnique(1)->getPass(0)->getTextureUnitState(1)->setTextureName(texname);

                // Base image 
                texname = filename + ".Base" + endName;
                // assign this texture to the material
                TextureManager::getSingleton().load (texname, opt->groupName);    
                mMaterial->getTechnique(1)->getPass(0)->getTextureUnitState(0)->setTextureName(texname);
            
                mMaterial->setLodLevels(opt->lodMaterialDistanceList);
	            mMaterial->setLightingEnabled(opt->lit);
                mMaterial->load();   
            }
	    }
    }
} //namespace
