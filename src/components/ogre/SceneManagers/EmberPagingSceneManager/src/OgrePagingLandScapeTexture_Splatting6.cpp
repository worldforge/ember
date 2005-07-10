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
#include "OgrePagingLandScapeTexture_Splatting6.h"



namespace Ogre
{   
    //-----------------------------------------------------------------------
    void PagingLandScapeTexture_Splatting6::_setPagesize( void )
    {
        PagingLandScapeOptions::getSingleton().VertexCompression = false;
        PagingLandScapeOptions::getSingleton().lodMorph = false;
    }
    //-----------------------------------------------------------------------
    void PagingLandScapeTexture_Splatting6::_clearData( void )
    {
    
    }
    //-----------------------------------------------------------------------
    PagingLandScapeTexture* PagingLandScapeTexture_Splatting6::newTexture( )
    {
        return new PagingLandScapeTexture_Splatting6();
    }
    //-----------------------------------------------------------------------
    bool PagingLandScapeTexture_Splatting6::TextureRenderCapabilitesFullfilled()
    {
        if (PagingLandScapeOptions::getSingleton().numTextureUnits > 1)
            return true;
        return false;
    }
    //-----------------------------------------------------------------------
    PagingLandScapeTexture_Splatting6::PagingLandScapeTexture_Splatting6() : PagingLandScapeTexture()
    {
    }
    //-----------------------------------------------------------------------
    PagingLandScapeTexture_Splatting6::~PagingLandScapeTexture_Splatting6()
    {
    }
    //-----------------------------------------------------------------------
    void PagingLandScapeTexture_Splatting6::LoadAlphaMap(const String &filename) const
    {
        TexturePtr tex = TextureManager::getSingleton().getByName (filename);
        if (tex.isNull())
        {
            Image Imageloader;
            const String group = PagingLandScapeOptions::getSingleton().groupName;
            Imageloader.load (filename, group);
            const size_t psize = PagingLandScapeOptions::getSingleton().PageSize - 1;
            Image ImageConvertertoAlphaFormat;
            ImageConvertertoAlphaFormat.loadDynamicImage(Imageloader.getData(), 
                psize, psize, 1, PF_A8, false);

            TextureManager::getSingleton().loadImage (filename, 
                                                    group,
                                                    ImageConvertertoAlphaFormat, TEX_TYPE_2D, 7);
        }
    }
    //-----------------------------------------------------------------------
    void PagingLandScapeTexture_Splatting6::_loadMaterial()
    {
	    if (mMaterial.isNull() )
	    {
            // Create a new texture using the base image
            const String filename = PagingLandScapeOptions::getSingleton().landscape_filename;
            const String commonName = StringConverter::toString(mDataZ) + 
                                        String(".") +
                                        StringConverter::toString(mDataX);
            const String matname = String("SplattingMaterial6.") + commonName + filename;
            mMaterial = MaterialManager::getSingleton().getByName(matname);
            if (mMaterial.isNull() )
	        {
                mMaterial = MaterialManager::getSingleton().getByName("SplattingMaterial6");
                mMaterial = mMaterial->clone(matname);
          
                const String extname = PagingLandScapeOptions::getSingleton().TextureExtension;
                const String alphamapname = ".Alpha.";


                
                String texname = filename + ".Base." +
                commonName + "." + extname;
                // assign this texture to the material
                //mMaterial->getTechnique(0)->getPass(0)->getTextureUnitState(0)->setTextureName(texname);
                mMaterial->getTechnique(1)->getPass(0)->getTextureUnitState(0)->setTextureName(texname);
                
                // Create a new texture using the 1st alpha map
                texname = filename + alphamapname + "0." +
                    commonName + "." + extname;   
                LoadAlphaMap (texname);
                // assign this texture to the material
                mMaterial->getTechnique(0)->getPass(0)->getTextureUnitState(1)->setTextureName(texname);

                // Create a new texture using the 2nd alpha map
                texname = filename + alphamapname + "1." +
                    commonName + "." + extname;   
                LoadAlphaMap (texname);
                // assign this texture to the material
                mMaterial->getTechnique(0)->getPass(1)->getTextureUnitState(1)->setTextureName(texname);

                // Create a new texture using the 3rd alpha map
                texname = filename + alphamapname + "2." +
                    commonName + "." + extname;   
                LoadAlphaMap (texname);
                // assign this texture to the material
                mMaterial->getTechnique(0)->getPass(2)->getTextureUnitState(1)->setTextureName(texname);

                // Create a new texture using the 4th alpha map
                texname = filename + alphamapname + "3." +
                    commonName + "." + extname;   
                LoadAlphaMap (texname);
                // assign this texture to the material
                mMaterial->getTechnique(0)->getPass(3)->getTextureUnitState(1)->setTextureName(texname);


                //texname = filename + ".L." +
                    //commonName + "." + extname;

                //mMaterial->getTechnique(0)->getPass(0)->getTextureUnitState(1)->setTextureName(texname);
                //mMaterial->getTechnique(1)->getPass(0)->getTextureUnitState(1)->setTextureName(texname);

                // Now that we have all the resources in place, we load the material
                mMaterial->load();
	            mMaterial->setLightingEnabled( PagingLandScapeOptions::getSingleton().lit );
            }
	    }
    }

} //namespace
