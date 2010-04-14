/***************************************************************************
	OgrePagingLandScapeTexture.h  -  description
  -------------------
  begin                : Fri Apr 16 2004
  copyright            : (C) 2002-2006 by Jose A Milan & Tuan Kuranes
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

#ifndef PAGINGLandScapeTEXTURE_H
#define PAGINGLandScapeTEXTURE_H

#include "OgrePagingLandScapePrerequisites.h"
#include <OgreGpuProgram.h>

namespace Ogre
{

/**
 * A simple class for encapsulating Texture generation.
 */
class PagingLandScapeTexture
{
public:
	PagingLandScapeTexture(PagingLandScapeTextureManager *pageMgr,
			       const String materialName,
			       const unsigned int numTexture = 0,
			       bool isSplatMode = false);

	// Method that must be overridden
	virtual Ogre::String getName() const = 0;

	virtual PagingLandScapeTexture* newTexture() = 0;

	// Method that can be overridden
	virtual ~PagingLandScapeTexture();

	virtual void bindCompressionSettings(GpuProgramParametersSharedPtr params);
	virtual void bindCompressionSettings();

	virtual void load(unsigned int mX, unsigned int mZ);
	virtual void unload();


	// Real Method
	bool getCastsShadows() const { return false; }

	bool isLoaded() const;

	const MaterialPtr& getMaterial() const;


	void getCoordinates(unsigned int& X, unsigned int& Z) const
	{
		X = mDataX;
		Z = mDataZ;
	}

	inline bool isCoord(unsigned int x, unsigned int z) const
	{
		return (mDataX == x && mDataZ == z);
	}

	const String& getMaterialName() const;

protected:

	// Method that can be overridden
	virtual void _loadMaterial();
	// Method that can be overridden
	virtual void _unloadMaterial();


	//! Pointer to ogre material
	MaterialPtr mMaterial;
	//! what page it correspond to
	unsigned int  mDataX, mDataZ;

	PagingLandScapeTextureManager *mParent;

	bool mIsLoaded;


private :
	void loadTexturesToModify();
};
}

#endif
