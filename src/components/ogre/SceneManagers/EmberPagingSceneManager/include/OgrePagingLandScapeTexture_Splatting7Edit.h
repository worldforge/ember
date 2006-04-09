/***************************************************************************
OgrePagingLandScapeTexture_Splatting7Edit.h  -  description
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

#ifndef PAGINGLandScapeTEXTURE7Edit_SPLATTING_H
#define PAGINGLandScapeTEXTURE7Edit_SPLATTING_H

#include "OgrePagingLandScapePrerequisites.h"

namespace Ogre
{
    class PagingLandScapeTexture_Splatting7Edit : public PagingLandScapeTexture
    {
        public:
	        PagingLandScapeTexture_Splatting7Edit(PagingLandScapeTextureManager *textureMgr);

            virtual void setPagesize(void){_setPagesize();};
            virtual void clearData(void){_clearData();};
            
            void _setPagesize(void);
            void _clearData(void);

               
            virtual String getName(){return String ("Splatting7Edit");};
            virtual const uint getNumChannels (void) const {return 3;};

            virtual void paint (const uint x, const uint z, 
                        const Real paintForce, const uint mPaintChannel);

	        virtual ~PagingLandScapeTexture_Splatting7Edit(void);

            virtual PagingLandScapeTexture* newTexture();
            virtual bool TextureRenderCapabilitesFullfilled(); 

        protected:
	        virtual void _loadMaterial(void);
            virtual void _unloadMaterial(void);

            void computePoint(const uint imagePos,
                const Real height, 
                const Real slope);
            void upload(const Image::Box& textureRect);

        private :
            void LoadAlphaMap(const String &filename, const uint channel);
            
            Image mImages[3];            
	        //! Pointer to ogre texture
            TexturePtr mTextures[3];
            HardwarePixelBufferSharedPtr mBuffers[3];
            bool isChannelNeedUpdate[3];
            bool isChannelModified[3];

            static uint mPageSize;
            static Real heights[4];
            static Real dividers[4];
            static ColourValue colors[4];
            //static ColourValue colors[4];
    };
}

#endif
