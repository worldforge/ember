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


#ifndef DIMETERRAINSCENEMANAGER_H
#define DIMETERRAINSCENEMANAGER_H

#include <Ogre.h>
#include <OgreTerrainSceneManager.h>
#include <OgreTerrainRenderable.h>
#include <OgreTerrainPage.h>
#include <wfmath/point.h>

class TerrainGenerator;

namespace DimeOgre {


/*
 * This is a specialization of Ogre::TerrainSceneManager.
 * Basically, some sort of paging has been added.
 * I'm not sure if we're going to use this, or some other plugin
 * such as paginglandscape, instead.
 * 
 * @see Ogre::TerrainSceneManager
 */
class DimeTerrainSceneManager  : public Ogre::TerrainSceneManager {
public:





	DimeTerrainSceneManager();
	virtual ~DimeTerrainSceneManager();

	void attachPage(Ogre::ushort pageX, Ogre::ushort pageZ, Ogre::TerrainPage* page,float maxY, float minY);
	Ogre::TerrainPage* getTerrainPage( const Ogre::Vector3 & pt );

	/*
	 * Resizes the octree. Do this after adding pages.
	 */
	void doResize();
	
	/*
	 * The scenemanager stores the pages in vectors. This does not allow
	 * for pages with negative indices.
	 * But WF uses negative terrain coordinates.
	 * Thus we need to offset the indices.
	 */
	int getPageOffset();
	
protected:

	/* 
	 * @see DimeOgre::DimeTerrainSceneManager::getPageOffset()
	 */
	Ogre::ushort mPageOffset;
	
	/*
	 * Stitches the neighbours, preventing gaps
	 */
	void setupPageNeighbors(Ogre::ushort pageX, Ogre::ushort pageZ, Ogre::TerrainPage* page); 

	/*
	 * Max and min values for the world. Used to resize the octree.
	 */
	float mMaxX;
	float mMaxY;
	float mMaxZ;
	float mMinX;
	float mMinY;
	float mMinZ;
	

private:
};

}

#endif // DIMETERRAINSCENEMANAGER_H
