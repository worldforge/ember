/***************************************************************************
OgrePagingLandScapeTexture_Splatting.h  -  description
	-------------------
	begin                : Mon Apr 16 2004
	copyright            : (C) 2002-2006 by Jose A Milan & Tuan Kuranes
	email                : spoke@supercable.es & tuan.kuranes@free.fr
***************************************************************************/

/***************************************************************************
*                                                                         *
*   This program is free software; you can redistribute it and/or modify  *
*   it under the terms of the GNU General Public License as        *
*   published by the Free Software Foundation; either version 2 of the    *
*   License, or (at your option) any later version.                       *
*                                                                         *
***************************************************************************/

#ifndef PAGINGLandScapeTEXTURE_SPLATTING_H
#define PAGINGLandScapeTEXTURE_SPLATTING_H

#include "OgrePagingLandScapePrerequisites.h"

namespace Ogre
{
    class PagingLandScapeTexture_Splatting : public PagingLandScapeTexture
    {
        public:
	        PagingLandScapeTexture_Splatting(PagingLandScapeTextureManager *textureMgr);
             String getName() const{return String("Splatting");}

			 void setOptions(void);
			 ~PagingLandScapeTexture_Splatting(void);

             PagingLandScapeTexture* newTexture();
             bool isMaterialSupported(); 

        protected:

	         void _loadMaterial(void);

        private:

	        void _BuildPoint(const unsigned int i, const int j, 
	                         ColourValue& out, std::vector<Real> &alpha);

	        inline void _InterpolateAlpha(std::vector<Real> &alpha, const Real percentaje, 
	                                    const int index1, const int index2);

            bool bAlpha1NotUsed, bAlpha2NotUsed, bAlpha3NotUsed, bAlpha4NotUsed;
    };

}

#endif
