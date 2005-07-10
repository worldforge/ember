/***************************************************************************
OgrePagingLandScapeTexture_BaseTexture2.cpp  -  description
-------------------
begin                : Mon Apr 26 2004
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
#include "OgrePagingLandScapeTexture_BaseTexture2.h"
#include "OgrePagingLandScapeData2DManager.h"


namespace Ogre
{
  //-----------------------------------------------------------------------
    void PagingLandScapeTexture_BaseTexture2::_setPagesize( void )
    {
        PagingLandScapeOptions::getSingleton().VertexCompression = false;
        PagingLandScapeOptions::getSingleton().lodMorph = false;
    }
    //-----------------------------------------------------------------------
    void PagingLandScapeTexture_BaseTexture2::_clearData( void )
    {
    
    }
//-----------------------------------------------------------------------
PagingLandScapeTexture* PagingLandScapeTexture_BaseTexture2::newTexture( )
{
    return new PagingLandScapeTexture_BaseTexture2();
}
//-----------------------------------------------------------------------
bool PagingLandScapeTexture_BaseTexture2::TextureRenderCapabilitesFullfilled()
{
    return true;
}
//-----------------------------------------------------------------------
PagingLandScapeTexture_BaseTexture2::PagingLandScapeTexture_BaseTexture2() : PagingLandScapeTexture()
{
}

//-----------------------------------------------------------------------
PagingLandScapeTexture_BaseTexture2::~PagingLandScapeTexture_BaseTexture2()
{
}

//-----------------------------------------------------------------------
void PagingLandScapeTexture_BaseTexture2::_loadMaterial()
{
	
	if ( mMaterial.isNull() )
    {
        const String filename = PagingLandScapeOptions::getSingleton().landscape_filename;
        const String commonName = StringConverter::toString(mDataZ) + 
            String(".") +
            StringConverter::toString(mDataX);
        const String matname = String("BaseMaterial2.") + commonName + filename;
		mMaterial = MaterialManager::getSingleton().getByName(matname);
	    if ( mMaterial.isNull() )
	    {
            mMaterial = MaterialManager::getSingleton().getByName("BaseMaterial");
		    // Create a new texture using the base image
            mMaterial = mMaterial->clone(matname);

            const String texname = filename + 
                ".Base." + 
                commonName + "." + PagingLandScapeOptions::getSingleton().TextureExtension;

		    // assign this texture to the material
		    mMaterial->getTechnique(0)->getPass(0)->getTextureUnitState(0)->setTextureName(texname);
		    mMaterial->getTechnique(1)->getPass(0)->getTextureUnitState(0)->setTextureName(texname);

		    // Now that we have all the resources in place, we load the material
		    mMaterial->load(); 
            mMaterial->setLightingEnabled( PagingLandScapeOptions::getSingleton().lit );
        }
	}
}

//-----------------------------------------------------------------------
void PagingLandScapeTexture_BaseTexture2::_unloadMaterial()
{
	if ( !mMaterial.isNull() )
	{
		mMaterial->unload();
		// We can´t destroy material here.
		//mMaterial -> destroy();
		//delete mMaterial;
		//mMaterial = 0;
	}
}

} //namespace
