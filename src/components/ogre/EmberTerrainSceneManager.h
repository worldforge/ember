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


class DimeTerrainSceneManager  : public Ogre::TerrainSceneManager {
public:



//	static DimeTerrainSceneManager & getSingleton(void);


	DimeTerrainSceneManager();
	virtual ~DimeTerrainSceneManager();

	void attachPage(Ogre::ushort pageX, Ogre::ushort pageZ, Ogre::TerrainPage* page,float maxY, float minY);
	Ogre::TerrainPage* getTerrainPage( const Ogre::Vector3 & pt );

	void doResize();
	
	int getPageOffset();
	
//	void buildTerrain(long segmentXStart, long segmentZStart, long numberOfSegments);
//	void setGenerator(TerrainGenerator* generator) { mGenerator = generator;}
//	void setPositionOfAvatar(Ogre::Vector3 point) { mPositionOfAvatar = point; }
//	void buildTerrainAroundAvatar();
protected:
	Ogre::ushort mPageOffset;
	void setupPageNeighbors(Ogre::ushort pageX, Ogre::ushort pageZ, Ogre::TerrainPage* page); 

	float mMaxX;
	float mMaxY;
	float mMaxZ;
	float mMinX;
	float mMinY;
	float mMinZ;
	

private:
//	static DimeTerrainSceneManager* _instance;
	
//	Ogre::Vector3 mPositionOfAvatar;
//	TerrainGenerator* mGenerator;
};

}

#endif // DIMETERRAINSCENEMANAGER_H
