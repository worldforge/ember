/***************************************************************************
	OgrePagingLandScapeData2D_HeightFieldBlendNeighbor.h  -  description
  -------------------
  begin                : Mon Oct 13 2003
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

#ifndef PagingLandScapeData2D_HeightFieldBlendNeighbor_H
#define PagingLandScapeData2D_HeightFieldBlendNeighbor_H

#include "OgrePagingLandScapePrerequisites.h"

namespace Ogre
{

    /**
    * A specialized class for loading 2D Data from a HeightField file.
    */
    class PagingLandScapeData2D_HeightFieldBlendNeighbor: public PagingLandScapeData2D
    {
        public:
	        PagingLandScapeData2D_HeightFieldBlendNeighbor(PagingLandScapeData2DManager *dataMgr);
            virtual String getName() const{return String("HeightFieldBlendNeighbor");}
	        virtual ~PagingLandScapeData2D_HeightFieldBlendNeighbor(void);

            virtual const Real getShadow(const Real mX, const Real mZ, const bool& positive);

            virtual PagingLandScapeData2D* newPage();
            const Real getMaxAbsoluteHeight(void) const; 

        protected:
            virtual void _save(void);
	        virtual bool _load(const unsigned int x, const unsigned int z);
	        virtual void _load(void);
	        virtual void _unload(void);

        private:
            Image * mShadow;
            unsigned int mBpp;// should be 2 bytes (mImage is 16 bits)
    };

}

#endif //PagingLandScapeData2D_HeightFieldBlendNeighbor_H
