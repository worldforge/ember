//
// C++ Interface: Foliage
//
// Description: 
//
//
// Author: Erik Hjortsberg <erik@katastrof.nu>, (C) 2004
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
// 
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.//
//
#ifndef DIMEOGREFOLIAGE_H
#define DIMEOGREFOLIAGE_H

#include <Ogre.h>

namespace EmberOgre {

class GroundCover;
/**
@author Erik Hjortsberg
*/
class Foliage : public Ogre::FrameListener {
public:
    Foliage(Ogre::Camera* camera, Ogre::SceneManager* mSceneMgr);

    ~Foliage();
	
	void setVisible(bool visible);
	
	bool frameStarted(const Ogre::FrameEvent & evt);
	void generateUnderVegetation(long segmentXStart, long segmentZStart, long numberOfSegments);

protected:
	GroundCover* mGround;
	
	bool mVisible;
	
	Ogre::Camera* mCamera;
	Ogre::SceneManager* mSceneMgr;

};

};

#endif
