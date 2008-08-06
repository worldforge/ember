//
// C++ Interface: Water
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
#ifndef EMBEROGRE_WATER_H
#define EMBEROGRE_WATER_H

#include "components/ogre/EmberOgrePrerequisites.h"
#include "Environment.h"

namespace EmberOgre {

namespace Environment {

class RefractionTextureListener;
class ReflectionTextureListener;


/**
@author Erik Hjortsberg
*/
class Water : public IWater
{


public:
    Water(Ogre::Camera& camera, Ogre::SceneManager* mSceneMgr);

    ~Water();
	
protected:
	
	Ogre::Plane reflectionPlane;
	
	Ogre::Camera& mCamera;

    RefractionTextureListener*  mRefractionListener;
    ReflectionTextureListener* mReflectionListener;
	void createFresnelWater(Ogre::Camera &camera, Ogre::SceneManager* mSceneMgr);
	void createSimpleWater(Ogre::Camera &camera, Ogre::SceneManager* mSceneMgr);
	Ogre::SceneNode *mWaterNode;

};

}

}

#endif
