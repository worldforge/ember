/***************************************************************************
OgrePagingLandScapeTexture_InstantBaseTextureEdit.h  -  description
  -------------------
  begin                : Mon Apr 26 2004
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

#ifndef PagingLandScapeTexture_InstantBaseTextureEdit_H
#define PagingLandScapeTexture_InstantBaseTextureEdit_H

#include "OgrePagingLandScapePrerequisites.h"

namespace Ogre
{


    class PagingLandScapeTexture_InstantBaseTextureEdit : public PagingLandScapeTexture
    {
        public:

	        PagingLandScapeTexture_InstantBaseTextureEdit(PagingLandScapeTextureManager *textureMgr);        	
	        virtual ~PagingLandScapeTexture_InstantBaseTextureEdit(void);

            virtual String getName() const{return String("InstantBaseTextureEdit");}
          
            virtual PagingLandScapeTexture* newTexture();
            virtual bool isMaterialSupported(); 
    };
}

#endif
