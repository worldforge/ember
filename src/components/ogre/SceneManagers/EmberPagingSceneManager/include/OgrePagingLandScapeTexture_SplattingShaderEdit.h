/***************************************************************************
OgrePagingLandScapeTexture_SplattingShader.h  -  description
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

#ifndef PAGINGLandScapeTEXTURE_SPLATTINGSHADEREDIT_H
#define PAGINGLandScapeTEXTURE_SPLATTINGSHADEREDIT_H

#include "OgrePagingLandScapePrerequisites.h"
#include "OgrePagingLandScapeTexture.h"

namespace Ogre
{

    class PagingLandScapeTexture_SplattingShaderEdit : public PagingLandScapeTexture
    {
    public:
        PagingLandScapeTexture_SplattingShaderEdit(PagingLandScapeTextureManager *textureMgr);
        virtual String getName(){return String("SplattingShaderEdit");}

        virtual ~PagingLandScapeTexture_SplattingShaderEdit(void);

        virtual const uint getNumChannels (void) const {return 4;};
        virtual void paint (const uint x, const uint z, 
                            const Real paintForce, 
                            const uint paintChannel);  


        virtual void setPagesize(void){_setPagesize();};
        virtual void clearData(void){_clearData();};

        void _setPagesize(void);
        void _clearData(void);

        virtual PagingLandScapeTexture* newTexture();
        virtual bool TextureRenderCapabilitesFullfilled(); 

    protected:
        virtual void _loadMaterial(void);
        virtual void _unloadMaterial(void);

    private :
        void _LoadTexture(const String& TexName, const String& groupName);

        void computePoint(const uint imagePos,
            const Real height, 
            const Real slope);
        virtual void upload(const Image::Box& textureRect);

        static uint mPageSize;
        static Real heights[4];
        static Real dividers[4];
        static ColourValue colors[4];
        //static ColourValue colors[4];


        Image mImage;
        uchar *mCoverageData;
        HardwarePixelBufferSharedPtr mBuffer;

        //! Pointer to ogre texture
        TexturePtr mTexture;
    };
}

#endif //PAGINGLandScapeTEXTURE_SPLATTINGSHADEREDIT_H
