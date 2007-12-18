/***************************************************************************
	OgrePagingLandScapeHorizon.h  -  description
  -------------------
  begin                : Sat Mar 08 2003
  copyright            : (C) 2003-2006 by Jose A. Milan and Tuan Kuranes
  email                : spoke2@supercable.es && tuan.kuranes@free.fr
***************************************************************************/

/***************************************************************************
*                                                                         *
*   This program is free software; you can redistribute it and/or modify  *
*   it under the terms of the GNU General Public License as        *
*   published by the Free Software Foundation; either version 2 of the    *
*   License, or (at your option) any later version.                       *
*                                                                         *
***************************************************************************/

#ifndef PagingLandScapeHorizon_H
#define PagingLandScapeHorizon_H

#include "OgrePagingLandScapePrerequisites.h"

namespace Ogre
{
    class PagingLandScapeHorizon
    {
        public:
	       PagingLandScapeHorizon(const PagingLandScapeOptions * const options);
           ~PagingLandScapeHorizon(void);
   
            void registerMinMaxHeightPage(const unsigned int pageX, const unsigned int pageZ, const Real minHeight, const Real maxHeight);

            void registerMinMaxHeightTile(const PagingLandScapeTileInfo* info, const Real minHeight, const Real maxHeight);

            bool IsPageVisible(const PagingLandScapeCamera* cam, const unsigned int destpageX, const unsigned int destpageZ);

            /**
             *
             * \param cam Give info on current tile and camera height
             * \param *destinfo info on the tile we want to test visibility on,
             * \return 
             */
            bool IsTileVisible(const PagingLandScapeCamera* cam, const PagingLandScapeTileInfo* destinfo);


            MaterialPtr getVisibilityMaterial(void);

            void AddVisibleTile(const unsigned int Tilex, const unsigned int Tilez, const bool visible);
            void AddVisibleTile(const PagingLandScapeTileInfo* info, const bool visible);
        
            void update(void);
            void prepare(const PagingLandScapeCamera* cam);

        private :

             const PagingLandScapeOptions *	const mOptions;

             bool calcVis(const Vector3& src, const Vector3& dest, const Real* const heightMap, const unsigned int mapWidth, const unsigned int mapHeight);

              Real* mMaxPageHeights;
              Real* mMinPageHeights;
              unsigned int mPageWidth;
              unsigned int mPageHeight;

              unsigned int mNumTilesPage;

              Real* mMaxTileHeights;
              Real* mMinTileHeights;
              unsigned int mTileWidth;
              unsigned int mTileHeight;

              MaterialPtr mVisibilityMaterial;
              uchar* mVisData;
              Image mVisImage;
              TexturePtr mVisTex;

              bool material_enabled;

    };

}

#endif
