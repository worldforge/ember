/***************************************************************************
	OgrePagingLandScapeData2D.h  -  description
  -------------------
  begin                : Wen Mar 5 2003
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

#ifndef PAGINGLandScapeDATA2D_H
#define PAGINGLandScapeDATA2D_H

#include "OgrePagingLandScapePrerequisites.h"

#include "OgrePagingLandScapeData2DManager.h"
#include "OgrePagingLandScapeOptions.h"

namespace Ogre
{
/**
 * Encapsulating 2D Data reading.
 */
class PagingLandScapeData2D
{
public:
	PagingLandScapeData2D(PagingLandScapeData2DManager* pageMgr);

	virtual ~PagingLandScapeData2D();

	virtual PagingLandScapeData2D* newPage() = 0;

	virtual Ogre::String getName() const = 0;
	virtual bool load(unsigned int mX, unsigned int mZ);

	virtual void load();

	virtual void init();
	virtual void uninit();

	virtual void unload();

#ifndef _MAPSPLITTER
	/**
	 *    deform Height Data of the terrain.
	 * \param &deformationPoint 
	 *       Where modification is, in world coordinates
	 * \param &modificationHeight 
	 *        What modification do to terrain
	 * \param info 
	 *        Give some info on tile to 
	 *       help coordinate system change
	 */
	bool deformHeight(const Vector3& deformationPoint, Ogre::Real& modificationHeight);

	/**
	 *
	 *   deform Height Data of the terrain.
	 * \param &x 
	 *       x Position on 2d height grid
	 * \param &z 
	 *       z Position on 2d height grid
	 * \param &modificationHeight 
	 *        What modification do to terrain
	 */
	bool deformHeight(unsigned int x, unsigned int z, Ogre::Real& modificationHeight);


	bool setHeight(unsigned int x, unsigned int z, const Ogre::Real& h);
	bool setHeight(unsigned int x, unsigned int z, unsigned int Pos, const Ogre::Real& h);
	/**
	 *
	 *    get smallest rectangle containing all deformation
	 *   done before an update. (where rectangle is reseted.)
	 * 
	 * \return Box struct describing rectangle
	 *        
	 */
	const Image::Box& getDeformationRectangle() const;

	/**
	 *
	 *    Adjust smallest rectangle to make it contain the point
	 * \param &x 
	 *       x Position on 2d height grid
	 * \param &z 
	 *       z Position on 2d height grid
	 */
	void adjustDeformationRectangle(unsigned int x, unsigned int z);

	/**
	 *
	 *    Reset Deformation rectangle
	 */
	void resetDeformationRectangle();
#endif // _MAPSPLITTER

	virtual Ogre::Vector3 getNormal(const Ogre::Real& mX, const Ogre::Real& mZ) const;

	virtual Ogre::ColourValue getBase(const Ogre::Real& mX, const Ogre::Real& mZ) const 
	{
		return ColourValue::White;
	}

	virtual Ogre::ColourValue getCoverage(const Ogre::Real& mX, const Ogre::Real& mZ) const 
	{
		return ColourValue::White;
	}

	virtual Ogre::Real getShadow(const Ogre::Real& mX, const Ogre::Real& mZ, bool positive) const 
	{
		return 0.0f;
	}

	const Ogre::Real& getShiftX() const
	{   
		return mShiftX;
	}

	const Ogre::Real& getShiftZ() const
	{   
		return mShiftZ;
	}

	inline Ogre::Real getHeightAbsolute(const Ogre::Real& x, const Ogre::Real& z) const
	{   
		const Vector3& invScale = mParent->getOptions()->invScale;

		// adjust x and z to be local to page
		int i_x = static_cast<int> (x * invScale.x - mShiftX);
		int i_z = static_cast<int> (z * invScale.z - mShiftZ);

		// due to Real imprecision on Reals, we have to use boundaries here
		// otherwise we'll hit asserts.
		int size = static_cast<int> (mSize-1);
		if (i_x > size)
			i_x = size; 
		else if (i_x < 0)
			i_x = 0;

		if (i_z > size)
			i_z = size;
		else if (i_z < 0)
			i_z = 0;

		const unsigned int u_x = static_cast<unsigned int> (i_x);
		const unsigned int u_z = static_cast<unsigned int> (i_z);

		const size_t arraypos = u_z * mSize + u_x; 
		assert (mHeightData && arraypos < mMaxArrayPos);
		return mHeightData[arraypos];
	}

	inline Ogre::Real getHeight(const Ogre::Real& x, const Ogre::Real& z) const 
	{
                assert (z < mSize && x < mSize);
                assert (mHeightData);                
                const unsigned int Pos = static_cast< unsigned int > (z * mSize + x);
                assert (mMaxArrayPos > Pos);
                return mHeightData[ Pos ];    
	}

	inline Ogre::Real getHeight(unsigned int x, unsigned int z) const 
	{
                assert (mHeightData);
                assert (z < mSize && x < mSize);
                const unsigned int Pos = static_cast <unsigned int> (z * mSize + x);
                assert (mMaxArrayPos > Pos);
                return mHeightData[ Pos ];  
	}

	inline Ogre::Real getHeight(int x, int z) const 
	{
                assert (mHeightData);
                assert (static_cast< unsigned int >(z) < mSize && static_cast< unsigned int >(x) < mSize);
                const unsigned int Pos = static_cast< unsigned int >(z * mSize + x);
                assert (mMaxArrayPos > Pos);
                return mHeightData[ Pos ]; 
	}

	inline Ogre::Real getHeight(unsigned int pos) const 
	{
		assert (mHeightData);
		assert (mMaxArrayPos > pos);
		return mHeightData[ pos ]; 
	}

	inline Ogre::Real getMaxHeight() const 
	{
		return mMaxheight;
	}

	// useful to know max height before data is loaded.
	virtual Ogre::Real getMaxAbsoluteHeight() const = 0;

	Ogre::Real* getHeightData() const
	{
		return mHeightData;
	}

	bool isLoaded() const { return mIsLoaded; };

	void computePowerof2PlusOneSize();
            
	virtual size_t getXDimension() const 
	{
		return mXDimension;
	}

	virtual size_t getZDimension() const 
	{
		return mZDimension;
	}

	virtual size_t getSize() const 
	{
		return mSize;
	};

	void getCoordinates(unsigned int& X, unsigned int& Z) const 
	{
		X = mPageX;
		Z = mPageZ;
	}

	inline bool isCoord(unsigned int x, unsigned int z) const { return (mPageX == x && mPageZ == z); };


protected:
	virtual void _save() = 0;
	virtual bool _load(unsigned int x, unsigned int z) = 0;
	virtual void _load() = 0;
	virtual void _unload() = 0;
	bool _checkSize(size_t s);

	//  computed Height Data  (scaled)
	Real* mHeightData;
	//  maximum position in Array
	unsigned int mMaxArrayPos;
	//  data side maximum size
	size_t mSize;
	//  data source width
	size_t mXDimension;;
	//  data source height
	size_t mZDimension;
	//  image data  maximum size
	unsigned int mMax;
	// maximum page/data2d height. (scaled)
	Real mMaxheight;
	// if data loaded or not
	bool mIsLoaded;   

	// if data modified or not
	bool mIsModified;  
	// if data modified but not yet
	// readied by other objects (texture or renderable)
	// to get modification to rect only once per frame.
	bool mIsRectModified;

	// page number
	unsigned int mPageX; 
	// page number
	unsigned int mPageZ;

	// coordinate shift based on page number
	Real mShiftX;
	// coordinate shift based on page number
	Real mShiftZ;
	PagingLandScapeData2DManager* mParent;

private:
	Image::Box mRect;
};

}

#endif
