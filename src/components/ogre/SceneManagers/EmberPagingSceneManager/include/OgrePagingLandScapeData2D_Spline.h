/***************************************************************************
	OgrePagingLandScapeData2D_Spline.h
	Header for a NURBS-based heightfield generator
	-------------------
	begin                : Sat Nov 9 2003
	copyright            : (C) 2003 Chris "Antiarc" Heald
	email                : antiarc@captionthis.com
***************************************************************************/

/***************************************************************************
*                                                                         *
*   This program is free software; you can redistribute it and/or modify  *
*   it under the terms of the GNU Lesser General Public License as        *
*   published by the Free Software Foundation; either version 2 of the    *
*   License, or (at your option) any later version.                       *
*                                                                         *
***************************************************************************/

#ifndef PAGINGLANDSCAPEDATA2D_SPLINE_H
#define PAGINGLANDSCAPEDATA2D_SPLINE_H

#include "OgrePagingLandScapePrerequisites.h"

#include "DRGNURBSSurface.h"

namespace Ogre
{

    /**
    * A specialized class for loading 2D Data from a Spline file.
    */

    class PagingLandScapeData2D_Spline: public PagingLandScapeData2D
    {
        public:
	        PagingLandScapeData2D_Spline( void );

	        ~PagingLandScapeData2D_Spline( void );

	        virtual PagingLandScapeData2D* newPage( );
            virtual String getName(){return String("Spline");}

        protected:
            virtual void _save( void );

	        virtual bool _load( const uint x, const uint z );

	        virtual void _load( void );

	        virtual void _unload( void );

        private:
	        int degree;

	        CDRGNURBSSurface* mSurface;
	        Point4D* mPoints;
    };

}

#endif
