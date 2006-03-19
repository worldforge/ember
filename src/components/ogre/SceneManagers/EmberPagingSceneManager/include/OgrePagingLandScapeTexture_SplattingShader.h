/***************************************************************************
OgrePagingLandScapeTexture_SplattingShader.h  -  description
	-------------------
	begin                : Mon Apr 16 2004
	copyright            : (C) 2002-2005 by Jose A Milan & Tuan Kuranes
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

#ifndef PAGINGLandScapeTEXTURE_SPLATTINGSHADER_H
#define PAGINGLandScapeTEXTURE_SPLATTINGSHADER_H

#include "OgrePagingLandScapePrerequisites.h"
#include "OgrePagingLandScapeTexture.h"

namespace Ogre
{

    class PagingLandScapeTexture_SplattingShader : public PagingLandScapeTexture
    {
        public:
	        PagingLandScapeTexture_SplattingShader(PagingLandScapeTextureManager *textureMgr);
            virtual String getName(){return String("SplattingShader");}

	        virtual ~PagingLandScapeTexture_SplattingShader(void);

            virtual PagingLandScapeTexture* newTexture();
            virtual bool TextureRenderCapabilitesFullfilled(); 

        protected:
	        virtual void _loadMaterial(void);

    };
}

#endif
