/***************************************************************************
	OgrePagingLandScapeData2DManager.h  -  description
	-------------------
  begin                : Mon Jun 16 2003
  copyright            : (C) 2003-2006 by Jose A. Milan and Tuan Kuranes
  email                : spoke2@supercable.es && tuan.kuranes@free.fr
***************************************************************************/

/***************************************************************************
*                                                                         *
*   This program is free software; you can redistribute it and/or modify  *
*   it under the terms of the GNU Lesser General Public License as        *
*   published by the Free Software Foundation; either version 2 of the    *
*   License, or (at your option) any later version.                       *
*                                                                         *
***************************************************************************/

#ifndef PAGINGLandScapeDATA2DMANAGER_H
#define PAGINGLandScapeDATA2DMANAGER_H

#include "OgrePagingLandScapePrerequisites.h"

namespace Ogre
{

    class PagingLandScapeData2DManager
    {
	public:
        PagingLandScapeData2DManager(PagingLandScapeSceneManager * scnMgr,
                                    PagingLandScapeOptions * opt );
        
        ~PagingLandScapeData2DManager(void);
        
        void load(void);
        void clear(void);
        void WorldDimensionChange(void);
        void reset(void);

        PagingLandScapeData2D* allocateData2D() const;

        bool load(const unsigned int dataX, const unsigned int dataZ);

        bool reload(const unsigned int dataX, const unsigned int dataZ);

        void unload(const unsigned int dataX, const unsigned int dataZ);
        
        bool isLoaded(const unsigned int dataX, const unsigned int dataZ);
        
        const Real getHeight(const unsigned int dataX, const unsigned int dataZ, const Real x, const Real z);
        
        const Real getHeight(const unsigned int dataX, const unsigned int dataZ, const unsigned int x, const unsigned int z);
        
        const Real getHeightAtPage(const unsigned int dataX, const unsigned int dataZ, const Real x, const Real z);
        
        const Real getHeightAtPage(const unsigned int dataX, const unsigned int dataZ, const int x, const int z);

        bool setHeight(const Vector3& deformationPoint, const Real modificationHeight, const PagingLandScapeTileInfo* info);
        bool deformHeight(const Vector3& deformationPoint, const Real modificationHeight, const PagingLandScapeTileInfo* info);

//        bool addNewHeight(const Sphere newSphere);
//        
//        bool removeNewHeight(const Sphere oldSphere);
        
        //This function will return the max possible value of height base on the current 2D Data implementation
        const Real getMaxHeight(const unsigned int x, const unsigned int z);
        
        const Real getMaxHeight(void) const;

        /** Get the real world height at a particular position
            @remarks
            Method is used to get the terrain height at a world position based on x and z.
            This method just figures out what page the position is on and then asks the page node
            to do the dirty work of getting the height.
            
            @par
            the Real returned is the real world height based on the scale of the world.  If the height could
            not be determined then -1 is returned and this would only occur if the page was not preloaded or loaded
            
            @param x  x world position
            @param z  z world position
        */
        const Real getInterpolatedWorldHeight(const Real x, const Real z, Real *slope = NULL);
        const Real getWorldHeight(const Real x, const Real z);
        
        const ColourValue getCoverageAt(const unsigned int dataX, const unsigned int dataZ, const Real x, const Real z);

        const ColourValue getBaseAt(const unsigned int dataX, const unsigned int dataZ, const Real x, const Real z);
        
       const Real getShadowAt(const unsigned int dataX, const unsigned int dataZ, const unsigned int x, const unsigned int z, const bool& positive);

       const Vector3 getNormalAt(const unsigned int pageX, const unsigned int pageZ, const unsigned int x, const unsigned int z);
       const Real getRealWorldSlope(const Real x, const Real z);
       const Vector3 getNormalAt(const unsigned int pageX, const unsigned int pageZ, const unsigned int x, const unsigned int z, const unsigned int Lod);

        void setPageManager(PagingLandScapeSceneManager *scnMgr);

        
        void registerDataType(PagingLandScapeData2D* source)
        {
            mData2DTypeMap.push_back(source);
//            LogManager::getSingleton().logMessage(
//                "PagingLandScape: Registered a new Data2DType for "
//                "type " + typeName);
        }
        // Walk the heightfield from location1 to location2 and find the max slope.
        // If maxSlope > 0 and a slope of greater than maxSlopeIn is found, maxSlope is returned.
        // This is an expensive operation, so use sparingly.
        Real getMaxSlope(Vector3 location1, Vector3 location2, Real maxSlopeIn);

		PagingLandScapeData2D* getData2D(const unsigned int i , const unsigned int j,
			const bool alwaysReturn = true);
		PagingLandScapeData2D* getNewData2D(const unsigned int i , const unsigned int j);
		void releaseData2D (PagingLandScapeData2D*p );

        PagingLandScapeOptions*		getOptions(){return mOptions;}
        PagingLandScapeSceneManager *getSceneManager(){return mScnMgr;}
    protected:
        PagingLandScapeSceneManager * mScnMgr;
        PagingLandScapeOptions*		mOptions;


        unsigned int                        mData2DType;
        String                      mData2DFormat;

        unsigned int                        mWidth;
        unsigned int                        mHeight;

 
        Real mMaxHeight;
        //PagingLandScapeData2DPages  mData2D;
        PagingLandScapePageManager*	mPageManager;

        /// Map of Data2d source type 
        PagingLandScapeData2DMap mData2DTypeMap;
        /// The currently active page Data2d source
		PagingLandScapeData2D* mActiveData2DType;

		PagingLandScapeData2DList mActiveData2Ds;
		PagingLandScapeData2DList mFreeData2Ds;
		PagingLandScapeData2DArray mData2DPool;
	};
    
} //namespace

#endif
