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
    void PagingLandScapeTexture_Splatting2::_setPagesize( void )
    {
        PagingLandScapeOptions::getSingleton().VertexCompression = false;
        PagingLandScapeOptions::getSingleton().lodMorph = false;
    }
    //-----------------------------------------------------------------------
    void PagingLandScapeTexture_Splatting2::_clearData( void )
    {
    
    }
    //-----------------------------------------------------------------------
    PagingLandScapeTexture* PagingLandScapeTexture_Splatting2::newTexture( )
    {
        return new PagingLandScapeTexture_Splatting2();
    }
    //-----------------------------------------------------------------------
    bool PagingLandScapeTexture_Splatting2::TextureRenderCapabilitesFullfilled()
    {        
        if (PagingLandScapeOptions::getSingleton().numTextureUnits >= 2)
            return true;
        return false;
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
            const String group = PagingLandScapeOptions::getSingleton().groupName;
            Imageloader.load (filename, group);
            const size_t psize = PagingLandScapeOptions::getSingleton().PageSize - 1;
            Image ImageConvertertoAlphaFormat;
            ImageConvertertoAlphaFormat.loadDynamicImage(Imageloader.getData(), psize, psize, 1, PF_A8, false);

            TextureManager::getSingleton().loadImage (filename, 
                                                    group,
                                                    ImageConvertertoAlphaFormat);
       }
    }
    //-----------------------------------------------------------------------
    void PagingLandScapeTexture_Splatting2::_loadMaterial()
    {
	    if (mMaterial.isNull() )
	    {

            // Create a new texture using the base image
            const String filename = PagingLandScapeOptions::getSingleton().landscape_filename;
            const String commonName = StringConverter::toString(mDataZ) + 
                                        String(".") +
                                        StringConverter::toString(mDataX);
            const String matname = String("SplattingMaterial2.") + commonName + filename;
		    mMaterial = MaterialManager::getSingleton().getByName(matname);
            if (mMaterial.isNull() )
	        {
                const String extname = PagingLandScapeOptions::getSingleton().TextureExtension;
		        mMaterial = MaterialManager::getSingleton().getByName("SplattingMaterial2");
                const String alphamapname = ".Alpha.";           
                mMaterial = mMaterial->clone(matname);

              
                uint passnb = 0;
	            // assign this texture to the material
                // Create the base image        
                String texname= filename + ".Base." + 
                                commonName + "." + extname;
		        //mMaterial->getTechnique(0)->getPass(passnb++)->getTextureUnitState(0)->setTextureName(texname);
                mMaterial->getTechnique(1)->getPass(0)->getTextureUnitState(0)->setTextureName(texname);
                    
                    
                // Create a new texture using the 1st alpha map
                texname = filename + alphamapname + "0." + 
                    commonName + "." + extname;
                LoadAlphaMap (texname);
                // assign this texture to the material
                mMaterial->getTechnique(0)->getPass(passnb++)->getTextureUnitState(1)->setTextureName(texname);
                    
                // Create a new texture using the 2nd alpha map
                texname = filename + alphamapname + "1." + 
                    commonName + "." + extname;
                LoadAlphaMap (texname);
                // assign this texture to the material
                mMaterial->getTechnique(0)->getPass(passnb++)->getTextureUnitState(1)->setTextureName(texname);
                    
                    // Create a new texture using the 3rd alpha map
                texname = filename + alphamapname + "2." + 
                    commonName + "." + extname;
                LoadAlphaMap (texname);
                // assign this texture to the material
                mMaterial->getTechnique(0)->getPass(passnb++)->getTextureUnitState(1)->setTextureName(texname);
                    
                    // Create a new texture using the 4th alpha map
                texname = filename + alphamapname + "3." + 
                    commonName + "." + extname;
                LoadAlphaMap (texname);
                // assign this texture to the material
                mMaterial->getTechnique(0)->getPass(passnb++)->getTextureUnitState(1)->setTextureName(texname);

                    
		        // Now that we have all the resources in place, we load the material
		        mMaterial->load(); 
                mMaterial->setLightingEnabled( PagingLandScapeOptions::getSingleton().lit );
            }
	    }
    }
} //namespace
