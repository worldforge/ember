//
// C++ Implementation: Sky
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
#include "Sky.h"
#include "../EmberOgrePrerequisites.h"
#include "services/EmberServices.h"
#include "services/config/ConfigService.h"
#include "../EmberOgre.h"
#include "../TerrainGenerator.h"

//#include "../EmberSceneManager/include/EmberTerrainSceneManager.h"



namespace EmberOgre {

Sky::Sky(Ogre::Camera* camera, Ogre::SceneManager* sceneMgr)
{
  sceneMgr->setSkyBox(true, "/global/environment/sky/day", 253);

  
	Ogre::ColourValue fadeColour(0.9,0.9,0.9);
	double fogstartDistance = 96; //default for fog
	double fogmaxDistance = 256; //default for fog gradient endind (i.e. where the fog is at 100%)
	if (Ember::EmberServices::getInstance()->getConfigService()->itemExists("graphics", "fogstart")) {
		fogstartDistance = (double)Ember::EmberServices::getInstance()->getConfigService()->getValue("graphics", "fogstart");
	}
	if (Ember::EmberServices::getInstance()->getConfigService()->itemExists("graphics", "fogmax")) {
		fogmaxDistance = (double)Ember::EmberServices::getInstance()->getConfigService()->getValue("graphics", "fogmax");
	}
	sceneMgr->setFog( Ogre::FOG_LINEAR, fadeColour, .001, fogstartDistance, fogmaxDistance);
	
//	EmberOgre::getSingleton().getTerrainGenerator()->getTerrainOptions().fogEnd = fogendDistance * fogendDistance;

  
  	try {
		//set up values for the splatting shader and the linear fog shader
		Ogre::HighLevelGpuProgramPtr splatProgram = Ogre::HighLevelGpuProgramManager::getSingleton().getByName("splat_cg");
		if (!splatProgram.isNull() && splatProgram->isSupported()) {
			splatProgram->load();
	//		splatProgram->createParameters();
			Ogre::GpuProgramParametersSharedPtr fpParams = splatProgram->getDefaultParameters();
			fpParams->setAutoAddParamName(true);
			Ogre::ColourValue fogColour = sceneMgr->getFogColour();
			fpParams->setNamedConstant("iFogColour",  fogColour);
		}
	} catch (...) {}
	
  	try {
		Ogre::HighLevelGpuProgramPtr fogProgram = Ogre::HighLevelGpuProgramManager::getSingleton().getByName("fog_linear_vp");
		if (!fogProgram.isNull() && fogProgram->isSupported()) {
			fogProgram->load();
	//		fogProgram->createParameters();
			Ogre::GpuProgramParametersSharedPtr vpParams = fogProgram->getDefaultParameters();
			vpParams->setAutoAddParamName(true);
			Ogre::ColourValue fogColour = sceneMgr->getFogColour();
			vpParams->setNamedAutoConstant("worldViewProj",  Ogre::GpuProgramParameters::ACT_WORLDVIEWPROJ_MATRIX);
			//get the values for the linear fog from the SceneManager
			vpParams->setNamedConstant("iFogStart",  sceneMgr->getFogStart());
			vpParams->setNamedConstant("iFogMax",  sceneMgr->getFogEnd());
		}
	} catch (...) {}

}


Sky::~Sky()
{
}


};
