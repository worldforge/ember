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
#include <wfmath/point.h>

class TerrainGenerator;

namespace DimeOgre {


class DimeTerrainSceneManager  : public Ogre::TerrainSceneManager {
public:



	static DimeTerrainSceneManager & getSingleton(void);


	DimeTerrainSceneManager();
	virtual ~DimeTerrainSceneManager();
	void buildTerrain(long segmentXStart, long segmentZStart, long numberOfSegments);
	void setGenerator(TerrainGenerator* generator) { mGenerator = generator;}
	void setPositionOfAvatar(Ogre::Vector3 point) { mPositionOfAvatar = point; }
	void buildTerrainAroundAvatar();
private:
	static DimeTerrainSceneManager* _instance;
	
	Ogre::Vector3 mPositionOfAvatar;
	TerrainGenerator* mGenerator;
};

}

#endif // DIMETERRAINSCENEMANAGER_H
