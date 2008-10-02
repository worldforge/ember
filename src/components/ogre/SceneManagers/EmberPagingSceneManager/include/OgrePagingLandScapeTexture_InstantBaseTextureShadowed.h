/***************************************************************************
OgrePagingLandScapeTexture_InstantBaseTexture.h  -  description
  -------------------
  begin                : Mon Apr 26 2004
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

#ifndef PagingLandScapeTexture_InstantBaseTextureShadowed_H
#define PagingLandScapeTexture_InstantBaseTextureShadowed_H

#include "OgrePagingLandScapePrerequisites.h"

namespace Ogre
{

    class PagingLandScapeTexture_InstantBaseTextureShadowed : public PagingLandScapeTexture
    {
        public:

            PagingLandScapeTexture_InstantBaseTextureShadowed(PagingLandScapeTextureManager *textureMgr);        	
	         ~PagingLandScapeTexture_InstantBaseTextureShadowed(void);

             String getName() const{return String("InstantBaseTextureShadowed");}

             void paint (const unsigned int x, const unsigned int z, 
                const Real paintForce, const ColourValue &mPaintColor);  


             PagingLandScapeTexture* newTexture();
             bool isMaterialSupported(); 

        protected:
	         void _loadMaterial(void);

    };
}

#endif
