/********************************************************************************
	OgrePagingLandScapeData2D_Spline.cpp
	*****************************************************************************
	A NURBS-based heightfield generator for use with the paginglandscapeplugin
	
	Note that it could easily be adapted for use as a general NURBS surface
	generator.
	*****************************************************************************
	begin                : Sat Nov 9 2003
	copyright            : (C) 2003 Chris "Antiarc" Heald
	email                : antiarc@captionthis.com
********************************************************************************/

/********************************************************************************
*																				*
*   This program is free software; you can redistribute it and/or modify		*
*   it under the terms of the GNU Lesser General Public License as published by *
*   the Free Software Foundation; either version 2 of the License, or			*
*   (at your option) any later version.											*
*																				*
********************************************************************************/


#include "OgreVector3.h"
#include "OgreColourValue.h"

#include "OgrePagingLandScapeData2D.h"
#include "OgrePagingLandScapeOptions.h"

#include "OgrePagingLandScapeData2D_Spline.h"

namespace Ogre
{

    //-----------------------------------------------------------------------
	PagingLandScapeData2D* PagingLandScapeData2D_Spline::newPage( )
    {
       return new PagingLandScapeData2D_Spline();
    }
    //-----------------------------------------------------------------------
    PagingLandScapeData2D_Spline::PagingLandScapeData2D_Spline()
    : PagingLandScapeData2D(),
        mSurface (0),
        mPoints (0)
    {
        mMaxheight = 256.0f * PagingLandScapeOptions::getSingleton().scale.y;
    }

    //-----------------------------------------------------------------------
    PagingLandScapeData2D_Spline::~PagingLandScapeData2D_Spline()
    {
		    delete mSurface;
		    delete[] mPoints;
    }
    //-------------------------------------------------------------------
    void PagingLandScapeData2D_Spline::_save()
    {
        
    }
    //-----------------------------------------------------------------------
    bool PagingLandScapeData2D_Spline::_load(const  uint mX, const uint mZ )
    {
		int resolution	= PagingLandScapeOptions::getSingleton().PageSize;
		mSize		    = resolution;
		mMax			= static_cast <uint> (mSize * mSize);
		int pCount		= 50;
		int mDegree		= 3;
		Real MAX		= 500;
		int tessLevel	= static_cast <uint> (mSize);

		srand (time(NULL));

		mPoints = new Point4D[pCount * pCount];
		const int knotVecSize = pCount + mDegree + 1;
		Real *knots = new Real[knotVecSize];
		int i;
		for (i = 0; i < knotVecSize; i++)
		{
			if (i < mDegree)
			{
				knots[i] = 0;
			}
			else if (i > knotVecSize - mDegree)
			{
				knots[i] = knotVecSize - (2 * mDegree) + 1;
			}
			else
			{
				knots[i] = i - mDegree + 1;
			}
		}
		int dataSize = pCount * pCount;
		for (i = 0; i < dataSize; i++)
		{
			mPoints[i].x = (int) (i/pCount);
			mPoints[i].y = double (rand()) / MAX;
			mPoints[i].w = 1;
			mPoints[i].z = 0;//i % pCount;
		}
		mSurface = new CDRGNURBSSurface();
		mSurface->Init(mDegree, mDegree, 
                        pCount, pCount, 
                        mPoints, 
                        knots, knots, 
                        tessLevel, tessLevel);
		mSurface->TessellateSurface();
		delete[] knots;
		delete[] mPoints;

        mXDimension = mSize;
        mZDimension = mSize;
        mMaxArrayPos = resolution * resolution;
        mHeightData = new Real[mMaxArrayPos];
        Real scale = PagingLandScapeOptions::getSingleton().scale.y; 
        mMaxheight = 0.0f;
        uint k;
        for (k = 0; k < mMaxArrayPos;  k ++ )
        {  
            Real h =  static_cast<Real> (mSurface->getData(k).y * scale); 
            mHeightData[k] = h;
            mMaxheight = std::max (h, mMaxheight);
        }    
		mIsLoaded = true;	   
        return true;
    }
    //-----------------------------------------------------------------------
    void PagingLandScapeData2D_Spline::_load()
    {
        
    }
    //-----------------------------------------------------------------------
    void PagingLandScapeData2D_Spline::_unload()
    {	    
		delete mSurface;
        mSurface = 0;
	    mIsLoaded = false;
    }

} //namespace
