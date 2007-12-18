/***************************************************************************
OgrePagingLandScapeTileInfo.h  -  description
-------------------
begin                : Sat Jan 31 2004
copyright            : (C) 2003-2006 by Jose A Milan && Tuan Kuranes
email                : spoke2@supercable.es && tuan.kuranes@free.fr
***************************************************************************/

/***************************************************************************
*                                                                         *
*   This program is free software; you can redistribute it and/or modify  *
*   it under the terms of the GNU General Public License as published by  *
*   the Free Software Foundation; either version 2 of the License, or     *
*   (at your option) any later version.                                   *
*                                                                         *
***************************************************************************/

#ifndef PAGINGLandScapeTILEINFO_H
#define PAGINGLandScapeTILEINFO_H

namespace Ogre
{
	/** This class holds the Tile info
		@remarks
			This will avoid to pass a lot of data to the Renderable class.
	*/
	class 
	PagingLandScapeTileInfo
	{
		public:
			PagingLandScapeTileInfo(const uint pageX, const uint pageZ, 
					  const uint tileX, const uint tileZ)
				  :
			  mMinLevelDistSqr(0),
				  mPageX (pageX),
				  mPageZ (pageZ),
				  mTileX (tileX),
				  mTileZ (tileZ)
			  {
			  }
		   ~PagingLandScapeTileInfo()
		   {
				delete mMinLevelDistSqr;
		   }

			//This is the Page Index in the Page Array
			unsigned short int mPageX;
			unsigned short int mPageZ;
	    		
			//This is the tile Index in the Tile Array
			unsigned short int mTileX;
			unsigned short int mTileZ;

			/// List of squared distances at which LODs change
			// costly to compute, so we cache it there.
			std::vector<Real>* mMinLevelDistSqr;
	};
}

#endif
