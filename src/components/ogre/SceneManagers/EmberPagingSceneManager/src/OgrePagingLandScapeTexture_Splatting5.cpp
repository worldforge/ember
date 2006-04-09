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
#include "OgrePagingLandScapeTexture_Splatting5.h"


namespace Ogre
{
    // static load
    //params->setNamedConstant

    // static unload
    //params->clearAutoConstants()
                                           
    //-----------------------------------------------------------------------
    void PagingLandScapeTexture_Splatting5::_setPagesize(void)
    {
        mParent->getOptions()->normals = true;
    }                    
    //-----------------------------------------------------------------------
    void PagingLandScapeTexture_Splatting5::_clearData(void)
    {
       
    }     
    //-----------------------------------------------------------------------
    PagingLandScapeTexture* PagingLandScapeTexture_Splatting5::newTexture()
    {
        return new PagingLandScapeTexture_Splatting5(mParent);
    }
    //-----------------------------------------------------------------------
    bool PagingLandScapeTexture_Splatting5::TextureRenderCapabilitesFullfilled()
    {                      
		const PagingLandScapeOptions * const opt = mParent->getOptions();
            
		if (opt->NumMatHeightSplat < 3)
			return false;
        if (opt->numTextureUnits < 4)
            return false;  
        if (!opt->hasFragmentShader)
            return false;    
            
        return true;
    }
    //-----------------------------------------------------------------------
    PagingLandScapeTexture_Splatting5::PagingLandScapeTexture_Splatting5(PagingLandScapeTextureManager *textureMgr) : PagingLandScapeTexture(textureMgr)
    {

    }
    //-----------------------------------------------------------------------
    PagingLandScapeTexture_Splatting5::~PagingLandScapeTexture_Splatting5()
    {

    }
    //-----------------------------------------------------------------------
    void PagingLandScapeTexture_Splatting5::_loadMaterial()
    {
	    if (mMaterial.isNull())
	    {       
			const PagingLandScapeOptions * const opt = mParent->getOptions();
            
            // Create a new texture using the base image
            const String commonName = StringConverter::toString(mDataZ) + String(".") + StringConverter::toString(mDataX);
            //const String prefilename = opt->LandScape_filename;
            //const String postfilename = commonName + "." + opt->TextureExtension;

            if (opt->VertexCompression)
            {
                mMaterial = (MaterialManager::getSingleton().getByName("SplattingMaterial5Decompress"));

                Pass *p = mMaterial->getTechnique(0)->getPass(0);
                GpuProgramParametersSharedPtr params = p->getVertexProgramParameters();
                
	            params->setNamedConstant("splatSettings", Vector4(opt->matHeight[1], 
                                                                  opt->matHeight[2], 
                                                                  opt->maxValue, 
                                                                    0.0));
                
                bindCompressionSettings (params);
                bindCompressionSettings (p->getShadowReceiverVertexProgramParameters());
            }
            else
            {
                mMaterial = MaterialManager::getSingleton().getByName("SplattingMaterial5");


                GpuProgramParametersSharedPtr params = mMaterial->getTechnique(0)->getPass(0)->getVertexProgramParameters();
	            params->setNamedConstant("splatSettings", Vector4(opt->matHeight[1], 
                                                                  opt->matHeight[2], 
                                                                  opt->maxValue, 
                                                                  0.0));
                    
            }     
            Pass * const p = mMaterial->getTechnique(0)->getPass(0); 
            const uint numSplats = opt->NumMatHeightSplat;
            uint splat_pass = 0; 
            while  (splat_pass < numSplats)
            {
                p->getTextureUnitState(splat_pass)->setTextureName(opt->SplatDetailMapNames[splat_pass]);
                splat_pass++;
            }       
            // Now that we have all the resources in place, we load the material
            mMaterial->load(); 
	    }
    }

} //namespace
