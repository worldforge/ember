/***************************************************************************
	OgrePagingLandScapeData2D_HeightFieldTC.h  -  description
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

#ifndef PAGINGLANDSCAPEDATA2D_HEIGHTFIELDTC_H
#define PAGINGLANDSCAPEDATA2D_HEIGHTFIELDTC_H

#include "OgrePagingLandScapePrerequisites.h"


namespace Ogre
{

    /**
    * A specialized class for loading 2D Data from a compressed HeightField file.
    */

    class PagingLandScapeData2D_HeightFieldTC: public PagingLandScapeData2D
    {
        public:
	        PagingLandScapeData2D_HeightFieldTC( void );

	        virtual ~PagingLandScapeData2D_HeightFieldTC( void );
            virtual String getName(){return String("HeightFieldTC");}

            virtual const ColourValue getBase( const Real mX, const Real mZ );
            virtual const ColourValue getCoverage( const Real mX, const Real mZ );

	        virtual PagingLandScapeData2D* newPage( );;

        protected:
            virtual void _save( void );

	        virtual bool _load( const uint x, const uint z );

            virtual void _load( void );

	        virtual void _unload( void );

        private:
            inline Real _decodeTC( const Real encoded ) const;
            inline uchar _encodeTC( const Real decoded ) const;

	        Real input_max, input_min;

	        Image* mImage;
            size_t mBpp;// should be 4 bytes (mImage is RGBA)

    };

}

#endif
