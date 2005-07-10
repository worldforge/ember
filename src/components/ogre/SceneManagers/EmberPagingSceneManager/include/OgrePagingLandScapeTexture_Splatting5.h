/***************************************************************************
OgrePagingLandScapeTexture_Splatting5.h  -  description
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

#ifndef PAGINGLANDSCAPETEXTURE5_SPLATTING_H
#define PAGINGLANDSCAPETEXTURE5_SPLATTING_H

#include "OgrePagingLandScapePrerequisites.h"
#include "OgrePagingLandScapeTexture.h"

namespace Ogre
{

    class PagingLandScapeTexture_Splatting5 : public PagingLandScapeTexture
    {
        public:
	        PagingLandScapeTexture_Splatting5( void );
            virtual String getName(){return String("Splatting5");}

	        virtual ~PagingLandScapeTexture_Splatting5( void );

            virtual PagingLandScapeTexture* newTexture( );
            virtual bool TextureRenderCapabilitesFullfilled(); 

        protected:
	        virtual void _loadMaterial( void );

    };
}

#endif
