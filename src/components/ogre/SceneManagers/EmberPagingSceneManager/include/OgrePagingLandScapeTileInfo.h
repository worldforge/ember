/***************************************************************************
OgrePagingLandScapeTileInfo.h  -  description
-------------------
begin                : Sat Jan 31 2004
copyright            : (C) 2003-2005 by Jose A Milan && Tuan Kuranes
email                : spoke2@supercable.es && tuan.kuranes@free.fr
***************************************************************************/

/***************************************************************************
*                                                                         *
*   This program is free software; you can redistribute it and/or modify  *
*   it under the terms of the GNU Lesser General Public License as published by  *
*   the Free Software Foundation; either version 2 of the License, or     *
*   (at your option) any later version.                                   *
*                                                                         *
***************************************************************************/

#ifndef PAGINGLANDSCAPETILEINFO_H
#define PAGINGLANDSCAPETILEINFO_H

namespace Ogre
{

/** This class holds the Tile info
	@remarks
		This will avoid to pass a lot of data to the Renderable class.
*/

class PagingLandScapeTileInfo
{
    public:
	    //This is the Page Index in the Page Array
	    uint pageX;
	    uint pageZ;
    		
	    //This is the tile Index in the Tile Array
	    uint tileX;
	    uint tileZ;

	    //This is the spatial position of this Tile
	    Real posX;
	    Real posZ;
};

}

#endif
