//
// C++ Implementation: Foliage
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
#include "services/EmberServices.h"
#include "services/logging/LoggingService.h"
#include "services/config/ConfigService.h"

#include "Foliage.h"

//#include "GroundCover.h"
#include "components/ogre/EmberOgre.h"
#include "components/ogre/TerrainGenerator.h"


namespace EmberOgre {

Foliage::Foliage(Ogre::Camera* camera, Ogre::SceneManager* sceneMgr)
: mCamera(camera), mSceneMgr(sceneMgr)
{
}


Foliage::~Foliage()
{
}

void Foliage::setVisible(bool visible)
{
	mVisible = visible;
	//mGround->getSceneNode()->setVisible(mVisible);
}



//This is all very quick and messy, to be replaced by something better in the future
void Foliage::generateUnderVegetation(TerrainPosition minExtent, TerrainPosition maxExtent)
{


		
}

bool Foliage::frameStarted(const Ogre::FrameEvent & evt)
{	
	if (mVisible) {
		//mGround->update(mCamera);
	}
	return true;


}




};
