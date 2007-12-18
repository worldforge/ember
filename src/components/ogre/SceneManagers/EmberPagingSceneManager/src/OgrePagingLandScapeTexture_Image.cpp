/***************************************************************************
OgrePagingLandScapeTexture_Image.cpp  -  description
-------------------
begin                : Fri Apr 16 2004
copyright            : (C) 2003-2006 by Jose A Milan && Tuan Kuranes
email                : spoke@supercable.es & tuan.kuranes@free.fr
***************************************************************************/

/***************************************************************************
*                                                                         *
*   This program is free software; you can redistribute it and/or modify  *
*   it under the terms of the GNU General Public License as published by  *
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
#include "OgrePagingLandScapeTextureManager.h"
#include "OgrePagingLandScapeTexture_Image.h"


namespace Ogre
{

    //-----------------------------------------------------------------------
    PagingLandScapeTexture* PagingLandScapeTexture_Image::newTexture()
    {
        return new PagingLandScapeTexture_Image(mParent);
    }
    //-----------------------------------------------------------------------
    bool PagingLandScapeTexture_Image::isMaterialSupported()
    {                
        if (mParent->getOptions()->ImageNameLoad)
            return true;
        return false;
    }
    //-----------------------------------------------------------------------
    PagingLandScapeTexture_Image::PagingLandScapeTexture_Image(PagingLandScapeTextureManager *textureMgr) : 
		PagingLandScapeTexture(textureMgr, "ImagePaging", 1, false)
    {
    }

    //-----------------------------------------------------------------------
    PagingLandScapeTexture_Image::~PagingLandScapeTexture_Image()
    {
    }
} //namespace
