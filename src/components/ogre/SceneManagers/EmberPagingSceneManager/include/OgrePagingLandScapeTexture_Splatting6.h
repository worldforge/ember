/***************************************************************************
OgrePagingLandScapeTexture_Splatting4.h  -  description
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

#ifndef PAGINGLANDSCAPETEXTURE6_SPLATTING_H
#define PAGINGLANDSCAPETEXTURE6_SPLATTING_H

#include "OgrePagingLandScapePrerequisites.h"

namespace Ogre
{
    class PagingLandScapeTexture_Splatting6 : public PagingLandScapeTexture
    {
        public:
	        PagingLandScapeTexture_Splatting6( void );
            virtual String getName(){return String("Splatting6");}

            virtual void setPagesize( void ){_setPagesize();};
            virtual void clearData( void ){_clearData();};
            
            static void _setPagesize( void );
            static void _clearData( void );

	        virtual ~PagingLandScapeTexture_Splatting6( void );

            virtual PagingLandScapeTexture* newTexture( );
            virtual bool TextureRenderCapabilitesFullfilled(); 

        protected:
	        virtual void _loadMaterial( void );

        private:
            void LoadAlphaMap( const String& filename ) const;

    };

}

#endif
