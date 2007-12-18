/***************************************************************************
	OgrePagingLandScapeData2D_HeightField.h  -  description
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

#ifndef PAGINGLandScapeDATA2D_HEIGHTFIELD_H
#define PAGINGLandScapeDATA2D_HEIGHTFIELD_H

#include "OgrePagingLandScapePrerequisites.h"


namespace Ogre
{

/**
  * A specialized class for loading 2D Data from a HeightField file.
  */

class PagingLandScapeData2D_HeightField: public PagingLandScapeData2D
{
public:
	PagingLandScapeData2D_HeightField(PagingLandScapeData2DManager *dataMgr);
            virtual String getName() const{return String("HeightField");}
	virtual ~PagingLandScapeData2D_HeightField(void);

    virtual const Vector3 getNormal(const Real mX, const Real mZ);
    virtual const ColourValue getBase(const Real mX, const Real mZ);
    virtual const ColourValue getCoverage(const Real mX, const Real mZ);
    virtual const Real getShadow(const Real mX, const Real mZ, const bool& positive);

    const Real getMaxAbsoluteHeight(void) const; 
	virtual PagingLandScapeData2D* newPage();
protected:

    virtual void _save(void);
	virtual bool _load(const unsigned int x, const unsigned int z);
	virtual void _load(void);
	virtual void _unload(void);

private:
	Real getScale() const;
	Real getInvScale() const;

	Image* mImage;
    Image* mBase;
    Image* mShadow;
    Image* mCoverage;

    /// should be 4 bytes (mImage is RGBA)
    size_t mBpp;
};

}

#endif
