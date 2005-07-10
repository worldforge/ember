/***************************************************************************
	OgrePagingLandScapeHorizon.h  -  description
  -------------------
  begin                : Sat Mar 08 2003
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

#ifndef PagingLandScapeHorizon_H
#define PagingLandScapeHorizon_H

#include "OgrePagingLandScapePrerequisites.h"

namespace Ogre
{
    class PagingLandScapeHorizon: public Singleton< PagingLandScapeHorizon >
    {
        public:
	       PagingLandScapeHorizon( const PagingLandScapeOptions& options );
           ~PagingLandScapeHorizon( void );
   
            void registerMinMaxHeightPage( const uint pageX, const uint pageZ, const Real minHeight, const Real maxHeight );

            void registerMinMaxHeightTile( const PagingLandScapeTileInfo* info, const Real minHeight, const Real maxHeight );

            bool IsPageVisible( const PagingLandScapeCamera* cam, const uint destpageX, const uint destpageZ );

            /**
             *
             * \param cam Give info on current tile and camera height
             * \param *destinfo info on the tile we want to test visibility on,
             * \return 
             */
            bool IsTileVisible( const PagingLandScapeCamera* cam, const PagingLandScapeTileInfo* destinfo );

	        static PagingLandScapeHorizon& getSingleton( void );

	        static PagingLandScapeHorizon* getSingletonPtr( void );

            MaterialPtr getVisibilityMaterial( void );

            void AddVisibleTile( const uint Tilex, const uint Tilez, const bool visible );
            void AddVisibleTile( const PagingLandScapeTileInfo* info, const bool visible );
        
            void update( void );
            void prepare( const PagingLandScapeCamera* cam );

        private :
             bool calcVis( const Vector3& src, const Vector3& dest, const Real* const heightMap, const uint mapWidth, const uint mapHeight );

              Real* mMaxPageHeights;
              Real* mMinPageHeights;
              uint mPageWidth;
              uint mPageHeight;

              uint mNumTilesPage;

              Real* mMaxTileHeights;
              Real* mMinTileHeights;
              uint mTileWidth;
              uint mTileHeight;

              MaterialPtr mVisibilityMaterial;
              uchar* mVisData;
              Image mVisImage;
              TexturePtr mVisTex;

              bool material_enabled;

    };

}

#endif
