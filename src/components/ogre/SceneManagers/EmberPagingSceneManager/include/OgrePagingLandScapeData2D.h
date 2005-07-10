/***************************************************************************
	OgrePagingLandScapeData2D.h  -  description
  -------------------
  begin                : Wen Mar 5 2003
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

#ifndef PAGINGLANDSCAPEDATA2D_H
#define PAGINGLANDSCAPEDATA2D_H

#include "OgrePagingLandScapePrerequisites.h"

namespace Ogre
{
    /**
    * Encapsulating 2D Data reading.
    */
    class PagingLandScapeData2D
    {
        public:
	        PagingLandScapeData2D( void );

	        virtual ~PagingLandScapeData2D( void );

	        virtual PagingLandScapeData2D* newPage( ) = 0;

            virtual String getName()= 0;
	        virtual bool load( const uint mX, const uint mZ );

            virtual void load( void );

	        virtual void init( void );
	        virtual void uninit( void );

	        virtual void unload( void );

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
            const bool DeformHeight( const Vector3& deformationPoint, Real &modificationHeight );

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
            const bool DeformHeight( const uint x, const uint z, Real &modificationHeight );


            void setHeight( const uint x, const uint z, const Real h );
            bool setHeight( const uint x, const uint z, const uint Pos, const Real h );
             /**
            *
            *    get smallest rectangle containing all deformation
            *   done before an update. (where rectangle is reseted.)
            * 
            * \return Box struct describing rectangle
            *        
            */
            Image::Box getDeformationRectangle( void );

            /**
            *
            *    Adjust smallest rectangle to make it contain the point
            * \param &x 
            *       x Position on 2d height grid
            * \param &z 
            *       z Position on 2d height grid
            */
            void adjustDeformationRectangle( const uint x, const uint z );

            /**
            *
            *    Reset Deformation rectangle
            */
            void resetDeformationRectangle( void );
#endif // _MAPSPLITTER

            virtual const Vector3 getNormal( const Real mX, const Real mZ );
            virtual const ColourValue getBase( const Real mX, const Real mZ )
            {
				return ColourValue::White;
			};

            virtual const ColourValue getCoverage( const Real mX, const Real mZ )
            {
				return ColourValue::White;
			};

            virtual const Real getShadow( const Real mX, const Real mZ, const bool& positive )
            {
				return 0.0f;
			};

            const Real getHeightAbsolute( const Real x, const Real z );

            inline const Real getHeight( const Real x, const Real z )
            {
                assert( z < mSize && x < mSize );
                assert( mHeightData );                
                const uint Pos = static_cast< uint > ( z * mSize + x );
                assert( mMaxArrayPos > Pos );
                return mHeightData[ Pos ];    
            };

            inline const Real getHeight( const uint x, const uint z )
            {
                assert( mHeightData );
                assert( z < mSize && x < mSize );
                const uint Pos = z * mSize + x;
                assert( mMaxArrayPos > Pos );
                return mHeightData[ Pos ];  
            };

            inline const Real getHeight( const int x, const int z )
            {
                assert( mHeightData );
                assert( static_cast< uint >( z ) < mSize && static_cast< uint >( x ) < mSize );
                const uint Pos = static_cast< uint >( z * mSize + x );
                assert( mMaxArrayPos > Pos );
                return mHeightData[ Pos ]; 
            };
            
            inline const Real getMaxHeight( void )
			{
				return mMaxheight;
			};

            Real* getHeightData( void )
			{
				return mHeightData;
			};

	        bool isLoaded( void );

            void computePowerof2PlusOneSize( void );
            
			virtual size_t getXDimension( void )
			{
				return mXDimension;
			};

            virtual size_t getZDimension( void )
			{
				return mZDimension;
			};

            virtual size_t getSize( void )
			{
				return mSize;
			};

        protected:
            virtual void _save( void ) = 0;
            virtual bool _load( const uint x, const uint z ) = 0;
	        virtual void _load( void ) = 0;
	        virtual void _unload( void ) = 0;
            bool _checkSize( const size_t s );

            //  computed Height Data  (scaled)
            Real *mHeightData;
            //  maximum position in Array
            uint mMaxArrayPos;
            //  data side maximum size
            size_t mSize;
            //  data source width
            size_t mXDimension;;
            //  data source height
            size_t mZDimension;
            //  image data  maximum size
            uint mMax;
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
	        uint mPageX; 
            // page number
	        uint mPageZ;

            // coordinate shift based on page number
            Real mShiftX;
            // coordinate shift based on page number
            Real mShiftZ;

        private :
            Image::Box mRect;
    };

}

#endif
