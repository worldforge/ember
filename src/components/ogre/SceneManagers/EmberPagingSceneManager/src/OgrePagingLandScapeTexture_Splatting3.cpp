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
    void PagingLandScapeTexture_Splatting3::_setPagesize( void )
    {
        PagingLandScapeOptions::getSingleton().VertexCompression = false;
        PagingLandScapeOptions::getSingleton().lodMorph = false;
    }
    //-----------------------------------------------------------------------
    void PagingLandScapeTexture_Splatting3::_clearData( void )
    {
    
    }
    //-----------------------------------------------------------------------
    PagingLandScapeTexture* PagingLandScapeTexture_Splatting3::newTexture( )
    {
        return new PagingLandScapeTexture_Splatting3();
    }
    //-----------------------------------------------------------------------
    bool PagingLandScapeTexture_Splatting3::TextureRenderCapabilitesFullfilled()
    {
        if (PagingLandScapeOptions::getSingleton().isRenderGL)
            return true;
        return false;
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
	    if (mMaterial.isNull() )
	    {
            const String filename = PagingLandScapeOptions::getSingleton().landscape_filename;
            const String commonName = StringConverter::toString(mDataZ) + 
                                        String(".") +
                                        StringConverter::toString(mDataX);
            const String matname = String("SplattingMaterial3.") + commonName + filename;
            mMaterial = MaterialManager::getSingleton().getByName(matname);
	        if (mMaterial.isNull() )
	        {
                mMaterial = MaterialManager::getSingleton().getByName("SplattingMaterial3");  
                mMaterial = mMaterial->clone(matname);

                const String extname = PagingLandScapeOptions::getSingleton().TextureExtension;
                String texname = filename + ".L." + 
                    commonName + "." + extname;

                mMaterial->getTechnique(0)->getPass(0)->getTextureUnitState(3)->setTextureName(texname);
                mMaterial->getTechnique(1)->getPass(0)->getTextureUnitState(1)->setTextureName(texname);

                // Base image 
                texname = filename + ".Base." + 
                    commonName + "." + extname;

                // assign this texture to the material
                mMaterial->getTechnique(1)->getPass(0)->getTextureUnitState(0)->setTextureName(texname);
            
                mMaterial->load();   
                mMaterial->setLightingEnabled( PagingLandScapeOptions::getSingleton().lit );
            }
	    }
    }
} //namespace
