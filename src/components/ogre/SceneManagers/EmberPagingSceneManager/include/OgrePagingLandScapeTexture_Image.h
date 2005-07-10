/***************************************************************************
	OgrePagingLandScapeTexture_Image.h  -  description
  -------------------
  begin                : Fri Apr 16 2004
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

#ifndef PAGINGLANDSCAPETEXTURE_IMAGE_H
#define PAGINGLANDSCAPETEXTURE_IMAGE_H

#include "OgrePagingLandScapePrerequisites.h"

namespace Ogre
{
    class PagingLandScapeTexture_Image : public PagingLandScapeTexture
    {
        public:
	        PagingLandScapeTexture_Image( void );
            virtual String getName(){return String("Image");}

	        virtual ~PagingLandScapeTexture_Image( void );

            virtual PagingLandScapeTexture* newTexture( );
            virtual bool TextureRenderCapabilitesFullfilled(); 

        protected:
	        virtual void _loadMaterial( void );

    };
}

#endif
