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

#ifndef PagingLandScapeTexture_Splatting2Edit_H
#define PagingLandScapeTexture_Splatting2Edit_H

#include "OgrePagingLandScapePrerequisites.h"

namespace Ogre
{
    class PagingLandScapeTexture_Splatting2Edit : public PagingLandScapeTexture
    {
        public:
	        PagingLandScapeTexture_Splatting2Edit( void );
            virtual String getName(){return String("Splatting2Edit");}

            virtual void setPagesize( void ){_setPagesize();};
            virtual void clearData( void ){_clearData();};
            
            static void _setPagesize( void );
            static void _clearData( void );

	        virtual ~PagingLandScapeTexture_Splatting2Edit( void );

            virtual const uint getNumChannels ( void ) const {return 4;};

            virtual void paint (const uint x, const uint z, 
                        const Real paintForce, const uint mPaintChannel);

            virtual PagingLandScapeTexture* newTexture( );
            virtual bool TextureRenderCapabilitesFullfilled(); 


        protected:
	        virtual void _loadMaterial( void);
        	virtual void update( void );
            virtual void _unloadMaterial( void );
    
        private:
            void LoadAlphaMap( const String& filename, const uint channel );

            Image mImages[4];            
	        //! Pointer to ogre texture
            TexturePtr mTextures[4];
            HardwarePixelBufferSharedPtr mBuffers[4];
            bool isChannelNeedUpdate[4];
            bool isChannelModified[4];
    };
}

#endif
