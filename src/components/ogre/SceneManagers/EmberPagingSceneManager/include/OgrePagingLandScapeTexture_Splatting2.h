/***************************************************************************
OgrePagingLandScapeTexture_Splatting.h  -  description
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

#ifndef PAGINGLandScapeTEXTURE_SPLATTING2_H
#define PAGINGLandScapeTEXTURE_SPLATTING2_H

#include "OgrePagingLandScapePrerequisites.h"

namespace Ogre
{
    class PagingLandScapeTexture_Splatting2 : public PagingLandScapeTexture
    {
        public:
            PagingLandScapeTexture_Splatting2(PagingLandScapeTextureManager *textureMgr);
            virtual String getName(){return String("Splatting2");}

            virtual void setPagesize(void){_setPagesize();};
            virtual void clearData(void){_clearData();};
            
            void _setPagesize(void);
            void _clearData(void);

	        virtual ~PagingLandScapeTexture_Splatting2(void);

            virtual const uint getNumChannels (void) const {return 0;};


            virtual PagingLandScapeTexture* newTexture();
            virtual bool TextureRenderCapabilitesFullfilled(); 

        protected:
	        virtual void _loadMaterial(void);


    };
}

#endif
