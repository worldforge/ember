//
// C++ Implementation: Sun
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
#include "Sun.h"
#include "components/ogre/MathConverter.h"

namespace EmberOgre {

Sun::Sun(Ogre::Camera* camera, Ogre::SceneManager* sceneMgr)
{
  mSun = sceneMgr->createLight("SunLight");
  mSun->setType(Ogre::Light::LT_DIRECTIONAL);
  mSun->setPosition(-1500,50,150);
	Ogre::Vector3 dir;
	dir = -mSun->getPosition();
	dir.normalise();
	mSun->setDirection(dir);
  mSun->setDiffuseColour(1, 1, 0.7); //yellow
  mSun->setSpecularColour(1, 1, 0.7); //yellow
  mSun->setCastShadows(false);
  mSun->setAttenuation(10000, 1, 0, 0);
  
  sceneMgr->setAmbientLight(Ogre::ColourValue(0.4, 0.4, 0.25));

}


Sun::~Sun()
{
}


};
