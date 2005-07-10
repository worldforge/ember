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
#include "OgrePagingLandScapeTexture_Splatting5.h"


namespace Ogre
{
    // static load
    //params->setNamedConstant

    // static unload
    //params->clearAutoConstants()


    //-----------------------------------------------------------------------
    PagingLandScapeTexture* PagingLandScapeTexture_Splatting5::newTexture( )
    {
        return new PagingLandScapeTexture_Splatting5();
    }
    //-----------------------------------------------------------------------
    bool PagingLandScapeTexture_Splatting5::TextureRenderCapabilitesFullfilled()
    {
        if (PagingLandScapeOptions::getSingleton().hasFragmentShader2)
        {
            PagingLandScapeOptions::getSingleton().normals = true;
            return true;
        }
        return false;
    }
    //-----------------------------------------------------------------------
    PagingLandScapeTexture_Splatting5::PagingLandScapeTexture_Splatting5() : PagingLandScapeTexture()
    {

    }
    //-----------------------------------------------------------------------
    PagingLandScapeTexture_Splatting5::~PagingLandScapeTexture_Splatting5()
    {

    }
    //-----------------------------------------------------------------------
    void PagingLandScapeTexture_Splatting5::_loadMaterial()
    {
	    if (mMaterial.isNull() )
	    {
            // Create a new texture using the base image
            const String commonName = StringConverter::toString(mDataZ) + String(".") + StringConverter::toString(mDataX);
            //const String prefilename = PagingLandScapeOptions::getSingleton().landscape_filename;
            //const String postfilename = commonName + "." + PagingLandScapeOptions::getSingleton().TextureExtension;


            String Materialname;
            if (PagingLandScapeOptions::getSingleton().VertexCompression)
            {
                mMaterial = (MaterialManager::getSingleton().getByName("SplattingMaterial5Decompress"));


                GpuProgramParametersSharedPtr params = mMaterial->getTechnique(0)->getPass(0)->getVertexProgramParameters();
	            params->setNamedConstant("splatSettings", Vector4(PagingLandScapeOptions::getSingleton().matHeight[1], 
                                                                    PagingLandScapeOptions::getSingleton().matHeight[2], 
                                                                    PagingLandScapeOptions::getSingleton().maxValue, 
                                                                    0.0));
    //            params->setNamedConstant("compressionSettings", Vector4(PagingLandScapeOptions::getSingleton().scale.x * PagingLandScapeOptions::getSingleton().PageSize, 
    //                                                                PagingLandScapeOptions::getSingleton().scale.y / 65535, 
    //                                                                PagingLandScapeOptions::getSingleton().scale.z * PagingLandScapeOptions::getSingleton().PageSize, 
    //                                                                0.0));
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
                mMaterial = MaterialManager::getSingleton().getByName("SplattingMaterial5");


                GpuProgramParametersSharedPtr params = mMaterial->getTechnique(0)->getPass(0)->getVertexProgramParameters();
	            params->setNamedConstant("splatSettings", Vector4(PagingLandScapeOptions::getSingleton().matHeight[1], 
                                                                    PagingLandScapeOptions::getSingleton().matHeight[2], 
                                                                    PagingLandScapeOptions::getSingleton().maxValue, 
                                                                    0.0));
                    
            }

            //  should really be done only once...
    //        GpuProgramParametersSharedPtr params = mMaterial->getTechnique(0)->getPass(0)->getVertexProgramParameters();
    //	    params->setNamedConstant("configSettings", Vector4(PagingLandScapeOptions::getSingleton().matHeight[0], 
    //                                                            PagingLandScapeOptions::getSingleton().matHeight[1], 
    //                                                            PagingLandScapeOptions::getSingleton().maxValue, 
    //                                                            0.0));

            // Now that we have all the resources in place, we load the material
            mMaterial->load(); 
	    }
    }

} //namespace
