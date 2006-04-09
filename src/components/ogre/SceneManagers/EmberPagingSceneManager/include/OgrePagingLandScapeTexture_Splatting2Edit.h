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
	        PagingLandScapeTexture_Splatting2Edit(PagingLandScapeTextureManager *textureMgr);
            virtual String getName(){return String("Splatting2Edit");}

            virtual void setPagesize(void){_setPagesize();};
            virtual void clearData(void){_clearData();};
            
            void _setPagesize(void);
            void _clearData(void);

	        virtual ~PagingLandScapeTexture_Splatting2Edit(void);

            virtual const uint getNumChannels (void) const {return 4;};

            virtual void paint (const uint x, const uint z, 
                        const Real paintForce, const uint mPaintChannel);

            virtual PagingLandScapeTexture* newTexture();
            virtual bool TextureRenderCapabilitesFullfilled(); 


        protected:
	        virtual void _loadMaterial(void);
            virtual void _unloadMaterial(void);

            void computePoint(const uint imagePos,
                const Real height, 
                const Real slope);

            void upload(const Image::Box &textureRect);

        private:
            void LoadAlphaMap(const String& filename, const uint channel);

            Image mImages[4];            
	        //! Pointer to ogre texture
            TexturePtr mTextures[4];
            HardwarePixelBufferSharedPtr mBuffers[4];
            bool isChannelNeedUpdate[4];
            bool isChannelModified[4];

            static uint mPageSize;
            static Real heights[4];
            static Real dividers[4];
            static ColourValue colors[4];
            //static ColourValue colors[4];
    };
}

#endif
