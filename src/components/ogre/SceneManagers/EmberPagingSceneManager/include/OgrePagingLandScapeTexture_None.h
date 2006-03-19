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

#ifndef PAGINGLandScapeTEXTURE_NONE_H
#define PAGINGLandScapeTEXTURE_NONE_H

#include "OgrePagingLandScapePrerequisites.h"
#include "OgrePagingLandScapeTexture.h"

namespace Ogre
{

    class PagingLandScapeTexture_None : public PagingLandScapeTexture
    {
        public:
            PagingLandScapeTexture_None(PagingLandScapeTextureManager *textureMgr) : 
              PagingLandScapeTexture(textureMgr)
              { 
                  mIsLoaded = true; 
                  mIsModified = false; 
              };
            virtual ~PagingLandScapeTexture_None(void) { };

            virtual String getName() {return String("None");}
            virtual void setPagesize(void) { };
            virtual void clearData(void) { };
            virtual const uint getNumChannels (void) const {return 1;};
            virtual void deformheight (const uint x, const uint z, const Real paintForce) { };
            virtual PagingLandScapeTexture* newTexture() { return new PagingLandScapeTexture_None(mParent); };
            virtual void load(uint mX, uint mZ) { mDataX = mX; mDataZ = mZ; mIsLoaded = true; mIsModified = false; };
            virtual void unload(void) { };

        protected:
           virtual void _loadMaterial(void) { };

    };
}

#endif
