/***************************************************************************
	OgrePagingLandScapeData2D_HeightFieldRaw.h  -  description
  -------------------
  begin                : Mon Oct 13 2003
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

#ifndef PagingLandScapeData2D_HeightFieldRaw_H
#define PagingLandScapeData2D_HeightFieldRaw_H

#include "OgrePagingLandScapePrerequisites.h"

namespace Ogre
{

    /**
    * A specialized class for loading 2D Data from a HeightField file.
    */
    class PagingLandScapeData2D_HeightFieldRaw: public PagingLandScapeData2D
    {
        public:
	        PagingLandScapeData2D_HeightFieldRaw( void );
            virtual String getName(){return String("HeightFieldRaw");}
	        virtual ~PagingLandScapeData2D_HeightFieldRaw( void );

            virtual const Real getShadow( const Real mX, const Real mZ, const bool& positive );

	        virtual PagingLandScapeData2D* newPage( );;

        protected:
            virtual void _save( void );
	        virtual bool _load( const uint x, const uint z );
	        virtual void _load( void );
	        virtual void _unload( void );

        private:
            Image * mShadow;
            uint mBpp;// should be 2 bytes (mImage is 16 bits)
    };

}

#endif //PagingLandScapeData2D_HeightFieldRaw_H
