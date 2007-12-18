 /***************************************************************************
OgrePagingLandScapeTexture_Splatting.cpp  -  description
	-------------------
	begin                : Mon Apr 16 2004
	copyright            : (C) 2003-2006 by Jose A Milan && Tuan Kuranes
	email                : spoke@supercable.es & tuan.kuranes@free.fr
***************************************************************************/

/***************************************************************************
*                                                                         *
*   This program is free software; you can redistribute it and/or modify  *
*   it under the terms of the GNU General Public License as        *
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
#include "OgrePagingLandScapeTextureManager.h"
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
        return new PagingLandScapeTexture_SplattingShader(mParent);
    }
    //-----------------------------------------------------------------------
    bool PagingLandScapeTexture_SplattingShader::isMaterialSupported()
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
    PagingLandScapeTexture_SplattingShader::PagingLandScapeTexture_SplattingShader(PagingLandScapeTextureManager *textureMgr) : 
        PagingLandScapeTexture(textureMgr, "SplattingShader", 1, true)
    {

    }
    //-----------------------------------------------------------------------
    PagingLandScapeTexture_SplattingShader::~PagingLandScapeTexture_SplattingShader()
    {

    }
} //namespace
