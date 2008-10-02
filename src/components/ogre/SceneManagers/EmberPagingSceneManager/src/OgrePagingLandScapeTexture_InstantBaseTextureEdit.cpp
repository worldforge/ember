/***************************************************************************
OgrePagingLandScapeTexture_InstantBaseTextureEdit.cpp  -  description
-------------------
begin                : Mon Apr 26 2004
copyright            : (C) 2002-2006 by Jose A Milan & Tuan Kuranes
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
#include "OgreTextureManager.h"
#include "OgreTechnique.h"
#include "OgrePass.h"

#include "OgrePagingLandScapeSceneManager.h"
#include "OgrePagingLandScapeOptions.h"
#include "OgrePagingLandScapeTexture.h"
#include "OgrePagingLandScapeTextureManager.h"
#include "OgrePagingLandScapeTexture_InstantBaseTextureEdit.h"
#include "OgrePagingLandScapeData2DManager.h"
#include "OgrePagingLandScapeData2D.h"

#include "fileutils.h"

namespace Ogre
{
    //-----------------------------------------------------------------------
    PagingLandScapeTexture* PagingLandScapeTexture_InstantBaseTextureEdit::newTexture()
    {
        return new PagingLandScapeTexture_InstantBaseTextureEdit(mParent);
    }
    //-----------------------------------------------------------------------
    bool PagingLandScapeTexture_InstantBaseTextureEdit::isMaterialSupported()
    {
		if (mParent->getOptions()->NumMatHeightSplat > 3)
			return true;
		else
			return false;
    }
    //-----------------------------------------------------------------------
    PagingLandScapeTexture_InstantBaseTextureEdit::PagingLandScapeTexture_InstantBaseTextureEdit(PagingLandScapeTextureManager *textureMgr)
        : 
        PagingLandScapeTexture(textureMgr, "InstantBase", 1, false)
    {
    }
    //-----------------------------------------------------------------------
    PagingLandScapeTexture_InstantBaseTextureEdit::~PagingLandScapeTexture_InstantBaseTextureEdit()
    {
	    
    }
    
} //namespace
