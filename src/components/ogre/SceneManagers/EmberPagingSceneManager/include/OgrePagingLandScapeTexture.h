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
	virtual Ogre::String getName() const { return mMaterialBaseName; }

	virtual PagingLandScapeTexture* newTexture()
	{
		return new PagingLandScapeTexture(mParent, mMaterialBaseName);
	}

	// Method that can be overridden
	virtual ~PagingLandScapeTexture();

	virtual void bindCompressionSettings(GpuProgramParametersSharedPtr params);
	virtual void bindCompressionSettings();

	virtual void load(unsigned int mX, unsigned int mZ);
	virtual void update();
	virtual void unload();

	virtual bool isMaterialSupported(bool recursive = true);
	virtual void setOptions();


	virtual unsigned int getNumChannels() const
	{
		return mNumTexture;
	}

	virtual unsigned int getNumChannelsperTexture(size_t i) const
	{
		return static_cast<unsigned int>(mNumChannelperTexture[i]);
	}

	// Real Method
	bool getCastsShadows() const { return false; }

	bool isLoaded() const;

	void setNeedUpdate();
	void updated();
	bool needUpdate() const;

	const MaterialPtr& getMaterial() const;

	void paint(unsigned int x, unsigned int z, const Real& paintForce);


	void adjustDeformationRectangle(unsigned int x, unsigned int z);
	void adjustPaintRectangle(unsigned int x, unsigned int z);

	virtual void lightUpdate();

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

	void compute(PagingLandScapeData2D* data, const Image::Box& dataRect, const Image::Box& textureRect);

	void computePointAlpha(unsigned int imagePos, Ogre::Real height, Ogre::Real slope);

	void computePointColor(unsigned int imagePos, Ogre::Real height, Ogre::Real slope);

	void paintPoint(unsigned int imagePos, Ogre::Real paintForce);

	void upload(const Image::Box& textureRect);

	void setNumTexture();

	// Method that can be overridden
	virtual void _loadMaterial();
	// Method that can be overridden
	virtual void _unloadMaterial();
	// Method that can be overridden
	virtual void _save();

	// properties that can be accessed from children
	bool mIsSplatMode;
	bool mIsBaseMode;
	bool mIsLoaded;
	bool mIsModified;
	//! Pointer to ogre material
	MaterialPtr mMaterial;
	//! what page it correspond to
	unsigned int  mDataX, mDataZ;

	Image::Box  mPaintRect;
	bool        mIsPaintRectModified;
	Image::Box  mDeformRect;
	bool        mIsDeformRectModified;
	PagingLandScapeTextureManager *mParent;

	// Edit, deform and update
	unsigned int					mNumTexture;
	std::vector<unsigned int>			mNumChannelperTexture;
	std::vector<Image>				mImages;
	std::vector<TexturePtr>				mTextures;
	std::vector<HardwarePixelBufferSharedPtr>	mBuffers;
	std::vector<bool>				doTextureNeedUpdate;
	std::vector<bool>				isTextureModified;

	// special case alpha loading Image as A8 only.
	void loadAlphaTexture(const String& filename, unsigned int channel);
	//  loading Image, buffer and texture in channels
	void loadColorTexture(const String& TexName, unsigned int channel);
	// loading Image, buffer and texture.
	void loadTexture(const String& filename, Image& img);
	// Dynamic Lighting
	void computeLightMap() const;
	Image mShadow;
	Image mLightImage;
	TexturePtr mLightTexture;
	HardwarePixelBufferSharedPtr mLightBuffer;
	bool mPositiveShadow;
	bool mIsShaderShadowed;
	bool mIsShadowed;

	// Name that helps building textures names on each page
	String mMaterialBaseName;

private :
	void loadTexturesToModify();
};
}

#endif
