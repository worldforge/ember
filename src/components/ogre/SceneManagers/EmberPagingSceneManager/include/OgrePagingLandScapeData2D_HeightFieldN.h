/***************************************************************************
	OgrePagingLandScapeData2D_HeightFieldN.h  -  description
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

#ifndef PagingLandScapeData2D_HeightFieldN_H
#define PagingLandScapeData2D_HeightFieldN_H

#include "OgrePagingLandScapePrerequisites.h"


namespace Ogre
{

/**
  * A specialized class for loading 2D Data from a HeightField file.
  */

class PagingLandScapeData2D_HeightFieldN: public PagingLandScapeData2D
{
    public:
	    PagingLandScapeData2D_HeightFieldN( void );
            virtual String getName(){return String("HeightFieldN");}
	    virtual ~PagingLandScapeData2D_HeightFieldN( void );

        virtual const Vector3 getNormal( const Real mX, const Real mZ );
        virtual const ColourValue getBase( const Real mX, const Real mZ );
        virtual const ColourValue getCoverage( const Real mX, const Real mZ);
        virtual const Real getShadow( const Real mX, const Real mZ, const bool& positive );

	    virtual PagingLandScapeData2D* newPage();

    protected:
        virtual void _save( void );
	    virtual bool _load( const uint x, const uint z );
	    virtual void _load( void );
	    virtual void _unload( void );

    private:
	    Image* mImage;
        Image* mBase;
        Image* mShadow;
        Image* mCoverage;

        size_t mBpp;// should be 4 bytes (mImage is RGBA)
};

}

#endif
