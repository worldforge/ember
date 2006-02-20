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
#include "OgrePagingLandScapeTexture_SplattingShader.h"


namespace Ogre
{
    // static load
    //params->setNamedConstant

    // static unload
    //params->clearAutoConstants()


    //-----------------------------------------------------------------------
    PagingLandScapeTexture* PagingLandScapeTexture_SplattingShader::newTexture()
    {
        return new PagingLandScapeTexture_SplattingShader();
    }
    //-----------------------------------------------------------------------
    bool PagingLandScapeTexture_SplattingShader::TextureRenderCapabilitesFullfilled()
    {      
		const PagingLandScapeOptions * const opt = PagingLandScapeOptions::getSingletonPtr();
            
		if (opt->NumMatHeightSplat < 3)
			return false;
        if (opt->numTextureUnits < 4)
            return false;  
        if (!opt->hasFragmentShader)
            return false;
            
        return true;
    }
    //-----------------------------------------------------------------------
    PagingLandScapeTexture_SplattingShader::PagingLandScapeTexture_SplattingShader() : PagingLandScapeTexture()
    {

    }
    //-----------------------------------------------------------------------
    PagingLandScapeTexture_SplattingShader::~PagingLandScapeTexture_SplattingShader()
    {

    }
    //-----------------------------------------------------------------------
    void PagingLandScapeTexture_SplattingShader::_loadMaterial()
    {
	    if (mMaterial.isNull())
	    {     
			const PagingLandScapeOptions * const opt = PagingLandScapeOptions::getSingletonPtr();
            
            // Create a new texture using the base image
            const String filename = opt->LandScape_filename;
            const String commonName = StringConverter::toString(mDataZ) + String(".") + StringConverter::toString(mDataX);
            const String prefilename = opt->LandScape_filename;
            const String postfilename = commonName + "." + opt->TextureExtension;

            const bool compressed = opt->VertexCompression;
            const String MatClassName = (compressed?String("SplattingMaterialShaderDecompress"):String("SplattingMaterialShader"));
            const String matname =  MatClassName + commonName + filename;
            mMaterial = MaterialManager::getSingleton().getByName(matname);
            if (mMaterial.isNull())
	        {                
                MaterialPtr ClassMaterial = MaterialManager::getSingleton().getByName(MatClassName);
                assert (!ClassMaterial.isNull());
                mMaterial = ClassMaterial->clone(matname);
                   
                Pass * const p = mMaterial->getTechnique(0)->getPass(0); 
                if (compressed)
                {
                    GpuProgramParametersSharedPtr params = p->getVertexProgramParameters();
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
                const String texname (filename + ".Coverage." + postfilename);
                TextureManager::getSingleton().load (texname, opt->groupName);    
                p->getTextureUnitState(0)->setTextureName(texname);
                
                const uint numSplats = opt->NumMatHeightSplat;
                uint splat_pass = 0; 
                while  (splat_pass < numSplats)
                {
                    p->getTextureUnitState(splat_pass + 1)->setTextureName(opt->SplatDetailMapNames[splat_pass]);
                    splat_pass++;
                }
            }
            // Now that we have all the resources in place, we load the material
           
            mMaterial->setLodLevels(opt->lodMaterialDistanceList);
            mMaterial->setLightingEnabled(opt->lit);
            mMaterial->load(); 
	    }
    }

} //namespace
