/***************************************************************************
OgrePagingLandScapeTexture_Splatting7Edit.h  -  description
	-------------------
	begin                : Mon Apr 16 2004
	copyright            : (C) 2002-2006 by Jose A Milan & Tuan Kuranes
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

#ifndef PAGINGLandScapeTEXTURE7Edit_SPLATTING_H
#define PAGINGLandScapeTEXTURE7Edit_SPLATTING_H

#include "OgrePagingLandScapePrerequisites.h"

namespace Ogre
{
    class PagingLandScapeTexture_Splatting7Edit : public PagingLandScapeTexture
    {
        public:
	        PagingLandScapeTexture_Splatting7Edit(PagingLandScapeTextureManager *textureMgr);
               
             String getName() const{return String ("Splatting7Edit");};
           
			void setOptions(void);

	         ~PagingLandScapeTexture_Splatting7Edit(void);

             PagingLandScapeTexture* newTexture();
             bool isMaterialSupported(); 

    };
}

#endif
