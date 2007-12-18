/***************************************************************************
OgrePagingLandScapeTexture_SplattingShader.h  -  description
	-------------------
	begin                : Mon Apr 16 2004
	copyright            : (C) 2002-2006 by Jose A Milan & Tuan Kuranes
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

#ifndef PAGINGLandScapeTEXTURE_SPLATTINGSHADEREDIT_H
#define PAGINGLandScapeTEXTURE_SPLATTINGSHADEREDIT_H

#include "OgrePagingLandScapePrerequisites.h"
#include "OgrePagingLandScapeTexture.h"

namespace Ogre
{

    class PagingLandScapeTexture_SplattingShaderEdit : public PagingLandScapeTexture
    {
    public:
        PagingLandScapeTexture_SplattingShaderEdit(PagingLandScapeTextureManager *textureMgr);
        String getName() const{return String("SplattingShaderEdit");}

        ~PagingLandScapeTexture_SplattingShaderEdit(void);
        PagingLandScapeTexture* newTexture();
        bool isMaterialSupported(); 
    };
}

#endif //PAGINGLandScapeTEXTURE_SPLATTINGSHADEREDIT_H
