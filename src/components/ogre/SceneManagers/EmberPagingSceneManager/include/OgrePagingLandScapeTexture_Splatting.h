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

#ifndef PAGINGLANDSCAPETEXTURE_SPLATTING_H
#define PAGINGLANDSCAPETEXTURE_SPLATTING_H

#include "OgrePagingLandScapePrerequisites.h"

namespace Ogre
{
    class PagingLandScapeTexture_Splatting : public PagingLandScapeTexture
    {
        public:
	        PagingLandScapeTexture_Splatting( void );
            virtual String getName(){return String("Splatting");}

            virtual void setPagesize( void ){_setPagesize();};
            virtual void clearData( void ){_clearData();};
            
            static void _setPagesize( void );
            static void _clearData( void );

	        virtual ~PagingLandScapeTexture_Splatting( void );

            virtual PagingLandScapeTexture* newTexture( );
            virtual bool TextureRenderCapabilitesFullfilled(); 

        protected:

	        virtual void _loadMaterial( void );

        private:

	        void _BuildPoint( ColourValue& out, const int j, Real& alpha1, Real& alpha2, Real& alpha3, Real& alpha4 );

	        inline void _InterpolateColour( ColourValue& out, const Real percentaje, const int index1, const int index2 );

	        inline void _InterpolateAlpha( Real& alpha1, Real& alpha2, Real& alpha3, Real& alpha4, const Real percentaje, const int index1, const int index2 );

            bool bAlpha1NotUsed, bAlpha2NotUsed, bAlpha3NotUsed, bAlpha4NotUsed;
    };

}

#endif
