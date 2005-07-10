/***************************************************************************
	OgrePagingLandScapeData2DManager.h  -  description
	-------------------
  begin                : Mon Jun 16 2003
  copyright            : (C) 2003-2005 by Jose A. Milan and Tuan Kuranes
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

#ifndef PAGINGLANDSCAPEDATA2DMANAGER_H
#define PAGINGLANDSCAPEDATA2DMANAGER_H

#include "OgrePagingLandScapePrerequisites.h"
#include "OgreSingleton.h"

namespace Ogre
{

    class PagingLandScapeData2DManager : public Singleton< PagingLandScapeData2DManager >
    {
	public:
        PagingLandScapeData2DManager( void );
        
        ~PagingLandScapeData2DManager( void );
        
        void load( void );
        void clear( void );
        void WorldDimensionChange( void );
        void reset( void );

        PagingLandScapeData2D* allocateData2D( ) const;

        bool load( const uint dataX, const uint dataZ );
        
        void unload( const uint dataX, const uint dataZ );
        
        bool isLoaded( const uint dataX, const uint dataZ ) const;
        
        const Real getHeight( const uint dataX, const uint dataZ, const Real x, const Real z ) const;
        
        const Real getHeight( const uint dataX, const uint dataZ, const uint x, const uint z ) const;
        
        const Real getHeightAtPage( const uint dataX, const uint dataZ, const Real x, const Real z ) const;
        
        const Real getHeightAtPage( const uint dataX, const uint dataZ, const int x, const int z) const;
      
        bool DeformHeight( const Vector3& deformationPoint, const Real modificationHeight, const PagingLandScapeTileInfo* info );

//        bool addNewHeight( const Sphere newSphere );
//        
//        bool removeNewHeight( const Sphere oldSphere );
        
        //This function will return the max possible value of height base on the current 2D Data implementation
        const Real getMaxHeight( const uint x, const uint z ) const;
        
        const Real getMaxHeight( void ) const;

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
        const Real getRealWorldHeight( const Real x, const Real z ) const;
        const Real getRealWorldHeight( const Real x, const Real z, const PagingLandScapeTileInfo* info ) const;
        const Real getWorldHeight(const Real x, const Real z) const;

        const Real getRealPageHeight( const Real x, const Real z, const uint pageX, const uint pageZ, const uint Lod ) const;
        
        const ColourValue getCoverageAt( const uint dataX, const uint dataZ, const Real x, const Real z );

        const ColourValue getBaseAt( const uint dataX, const uint dataZ, const Real x, const Real z );
        
       const Real getShadowAt( const uint dataX, const uint dataZ, const uint x, const uint z, const bool& positive );

        const Vector3 getNormalAt( const uint dataX, const uint dataZ, const uint x, const uint z );

        PagingLandScapeData2D* getData2d( const uint x, const uint z );
        void setPageManager( void );

        static PagingLandScapeData2DManager& getSingleton( void );
        
        static PagingLandScapeData2DManager* getSingletonPtr( void );
        
        void registerDataType(PagingLandScapeData2D* source)
        {
            mData2DTypeMap.push_back(source);
//            LogManager::getSingleton().logMessage(
//                "PagingLandScape: Registered a new Data2DType for "
//                "type " + typeName);
        }

    protected:
        PagingLandScapeOptions*		mOptions;


        uint                        mData2DType;
        String                      mData2DFormat;

        uint                        mWidth;
        uint                        mHeight;

 
        Real mMaxHeight;
        PagingLandScapeData2DPages  mData2D;
        PagingLandScapePageManager*	mPageManager;

        /// Map of Data2d source type 
        PagingLandScapeData2DMap mData2DTypeMap;
        /// The currently active page Data2d source
        PagingLandScapeData2D* mActiveData2DType;
	};
    
} //namespace

#endif
