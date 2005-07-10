/***************************************************************************
OgrePagingLandScapeTexture_InstantBaseTextureEdit.h  -  description
  -------------------
  begin                : Mon Apr 26 2004
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

#ifndef PagingLandScapeTexture_InstantBaseTextureEdit_H
#define PagingLandScapeTexture_InstantBaseTextureEdit_H

#include "OgrePagingLandScapePrerequisites.h"

namespace Ogre
{


    class PagingLandScapeTexture_InstantBaseTextureEdit : public PagingLandScapeTexture
    {
        public:

	        PagingLandScapeTexture_InstantBaseTextureEdit( void );        	
	        virtual ~PagingLandScapeTexture_InstantBaseTextureEdit( void );

            virtual String getName(){return String("InstantBaseTextureEdit");}

            virtual void setPagesize( void ){_setPagesize();};
            virtual void clearData( void ){_clearData();};
            
            static void _setPagesize( void );
            static void _clearData( void );
            
            virtual const uint getNumChannels ( void ) const {return 1;};
            virtual void paint (const uint x, const uint z, 
                const Real paintForce, const ColourValue &mPaintColor);  

            virtual void deformheight (const uint x, const uint z, 
                        const Real paintForce);

            virtual PagingLandScapeTexture* newTexture( );
            virtual bool TextureRenderCapabilitesFullfilled(); 

        protected:

	        virtual void _loadMaterial( void );
        	virtual void update( void );
	        virtual void _unloadMaterial( void );

        private :
            void _LoadTexture( const String& TexName, const String& groupName );
            void computeInstantBase( PagingLandScapeData2D* data, const Image::Box& rect ) const;    
            void computeInstantBaselight( void ) const;


            HardwarePixelBufferSharedPtr mBuffer;

            static uint mPageSize;
            static Real heights[4];
            static Real dividers[4];
            static ColourValue colors[4];
            //static ColourValue colors[4];

		    Image mImage;
		    uchar *mBaseData;
            
	        //! Pointer to ogre texture
	        TexturePtr mTexture;
    };
}

#endif
