/*
    Copyright (C) 2004  Erik Hjortsberg

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
*/


#ifndef DIMETERRAINRENDERTILE_H
#define DIMETERRAINRENDERTILE_H

#include <Ogre.h>
#include <OgreTerrainRenderable.h>

namespace DimeOgre {

class TerrainGenerator;

class DimeTerrainRenderable : public Ogre::TerrainRenderable {
public:

	DimeTerrainRenderable();
	virtual ~DimeTerrainRenderable();
	//void DimeTerrainRenderable::init( Ogre::TerrainOptions &options, TerrainGenerator* generator );
    
    /** Initializes the TerrainRenderable.
    @param startx, startz 
    The starting points of the top-left of this tile, in terms of the
    number of vertices.
    @param pageHeightData The source height data for the entire parent page
    */
   // void initialise(int startx, int startz, Real* pageHeightData, Vector3 center));
};

}

#endif // DIMETERRAINRENDERTILE_H
