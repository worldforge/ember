/***************************************************************************
OgrePagingLandScapeTexture_Image.cpp  -  description
-------------------
begin                : Fri Apr 16 2004
copyright            : (C) 2003-2005 by Jose A Milan && Tuan Kuranes
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
#include "OgreTechnique.h"
#include "OgrePass.h"

#include "OgrePagingLandScapeOptions.h"
#include "OgrePagingLandScapeTexture.h"
#include "OgrePagingLandScapeTexture_Image.h"


namespace Ogre
{

    //-----------------------------------------------------------------------
    PagingLandScapeTexture* PagingLandScapeTexture_Image::newTexture( )
    {
        return new PagingLandScapeTexture_Image();
    }
    //-----------------------------------------------------------------------
    bool PagingLandScapeTexture_Image::TextureRenderCapabilitesFullfilled()
    {                
        if (PagingLandScapeOptions::getSingleton().ImageNameLoad)
            return true;
        return false;
    }
    //-----------------------------------------------------------------------
    PagingLandScapeTexture_Image::PagingLandScapeTexture_Image() : PagingLandScapeTexture()
    {
    }

    //-----------------------------------------------------------------------
    PagingLandScapeTexture_Image::~PagingLandScapeTexture_Image()
    {
    }

    //-----------------------------------------------------------------------
    void PagingLandScapeTexture_Image::_loadMaterial()
    {
	    if ( mMaterial.isNull() )
	    {
            if (PagingLandScapeOptions::getSingleton().ImageNameLoad)
            {  
                const String filename = PagingLandScapeOptions::getSingleton().image_filename;
                const String commonName = StringConverter::toString(mDataZ) + 
                                            String(".") +
                                            StringConverter::toString(mDataX);
                const String matname = String("ImageTexture.") + commonName + filename;
                mMaterial = MaterialManager::getSingleton().getByName(matname);
                if (mMaterial.isNull())
                {
                    MaterialPtr templateMaterial;
                    if (PagingLandScapeOptions::getSingleton ().VertexCompression)
                    {
                        templateMaterial = MaterialManager::getSingleton().getByName(String ("PagingLandScape.Template.VertexPixelShaded"));
                        mMaterial = templateMaterial->clone(matname);    

                        GpuProgramParametersSharedPtr params = mMaterial->getTechnique(0)->getPass(0)->getVertexProgramParameters();
    	                
    //                    params->setNamedConstant("compressionSettings", Vector4(PagingLandScapeOptions::getSingleton().scale.x * PagingLandScapeOptions::getSingleton().PageSize, 
    //                                                                        PagingLandScapeOptions::getSingleton().scale.y / 65535, 
    //                                                                        PagingLandScapeOptions::getSingleton().scale.z * PagingLandScapeOptions::getSingleton().PageSize, 
    //                                                                        0.0));
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
                        templateMaterial = MaterialManager::getSingleton().getByName(String ("PagingLandScape.Template"));
                        mMaterial = templateMaterial->clone(matname);
                    }   
          
                }
                const String texname = filename + "." +
                    commonName + "." + PagingLandScapeOptions::getSingleton().TextureExtension;       
                // assign this texture to the material
                mMaterial->getTechnique(0)->getPass(0)->getTextureUnitState(0)->setTextureName(texname);
            }
            else
            {
                // JEFF - all material settings configured through material script
                mMaterial = (MaterialManager::getSingleton().getByName("PagingLandScape." +
                    StringConverter::toString( mDataZ ) + "." + 
                    StringConverter::toString( mDataX )));
            }
               
		    mMaterial->load(); 
            mMaterial->setLightingEnabled( PagingLandScapeOptions::getSingleton().lit );
	    }
    }


} //namespace
