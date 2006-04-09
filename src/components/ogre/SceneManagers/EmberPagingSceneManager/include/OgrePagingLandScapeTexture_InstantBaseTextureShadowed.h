/***************************************************************************
OgrePagingLandScapeTexture_InstantBaseTexture.h  -  description
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

#ifndef PagingLandScapeTexture_InstantBaseTextureShadowed_H
#define PagingLandScapeTexture_InstantBaseTextureShadowed_H

#include "OgrePagingLandScapePrerequisites.h"

namespace Ogre
{

    class PagingLandScapeTexture_InstantBaseTextureShadowed : public PagingLandScapeTexture
    {
        public:

            PagingLandScapeTexture_InstantBaseTextureShadowed(PagingLandScapeTextureManager *textureMgr);        	
	        virtual ~PagingLandScapeTexture_InstantBaseTextureShadowed(void);

            virtual String getName(){return String("InstantBaseTextureShadowed");}

            virtual void setPagesize(void){_setPagesize();};
            virtual void clearData(void){_clearData();};
            
            void _setPagesize(void);
            void _clearData(void);

            virtual void paint (const uint x, const uint z, 
                const Real paintForce, const ColourValue &mPaintColor);  

            virtual const uint getNumChannels (void) const {return 1;};


            virtual PagingLandScapeTexture* newTexture();
            virtual bool TextureRenderCapabilitesFullfilled(); 

            virtual void lightUpdate ();
        protected:

	        virtual void _loadMaterial(void);
	        virtual void _unloadMaterial(void);

            void computePoint(const uint imagePos,
                const Real height, 
                const Real slope);

            void upload(const Image::Box &textureRect);

        private :
            void _LoadTexture(const String& TexName, const String& groupName);
          
            void computeInstantBaselight(void) const;

            HardwarePixelBufferSharedPtr mBuffer;

            static uint mPageSize;
		    static Image mImage;
            static Real heights[4];
            static Real dividers[4];
            static ColourValue colors[4];

	        //! Pointer to ogre texture
	        TexturePtr mTexture;

            Image mShadow;
            Image mLightImage;
	        TexturePtr mLightTexture;
            HardwarePixelBufferSharedPtr mLightBuffer;

            bool mPositiveShadow;
    };
}

#endif
