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


#include "TerrainGenerator.h"
#include "MathConverter.h"

#include "EmberTerrainRenderable.h"

using namespace Ogre;
namespace EmberOgre {

#define POSITION_BINDING 0
#define NORMAL_BINDING 1
#define TEXCOORD_BINDING 2
#define COLOUR_BINDING 3

EmberTerrainRenderable::EmberTerrainRenderable() : TerrainRenderable::TerrainRenderable()
{}
EmberTerrainRenderable::~EmberTerrainRenderable() {}

/*
void EmberTerrainRenderable::initialise(int startx, int startz, Real* pageHeightData, Vector3 center)
{
	Ogre::TerrainRenderable::initialise(startx, startz, pageHeightData);
	
	mCenter = center
	/*
	 *  Vector3( ( startx * msOptions->scale.x + (endx - 1) * msOptions->scale.x ) / 2,
            ( min + max ) / 2,
            ( startz * msOptions->scale.z + (endz - 1) * msOptions->scale.z ) / 2 );

}
*/
}

