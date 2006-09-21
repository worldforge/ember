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
#include "../terrain/TerrainGenerator.h"
#include "framework/Tokeniser.h"



#include <sigc++/object_slot.h>




namespace EmberOgre {

Sky::Sky(Ogre::Camera* camera, Ogre::SceneManager* sceneMgr)
{
  sceneMgr->setSkyBox(true, "/global/environment/sky/day", 253);

  updateFogValuesFromConfig();
  Ember::EmberServices::getSingletonPtr()->getConfigService()->EventChangedConfigItem.connect(sigc::mem_fun(*this, &Sky::ConfigService_EventChangedConfigItem));


}



Sky::~Sky()
{
}

void Sky::updateFogValuesFromConfig()
{
	Ogre::ColourValue fadeColour(0.9,0.9,0.9);
	float fogstartDistance = 96; //default for fog
	float fogmaxDistance = 256; //default for fog gradient endind (i.e. where the fog is at 100%)
	if (Ember::EmberServices::getSingletonPtr()->getConfigService()->itemExists("graphics", "fogstart")) {
		fogstartDistance = static_cast<double>(Ember::EmberServices::getSingletonPtr()->getConfigService()->getValue("graphics", "fogstart"));
	}
	if (Ember::EmberServices::getSingletonPtr()->getConfigService()->itemExists("graphics", "fogmax")) {
		fogmaxDistance = static_cast<double>(Ember::EmberServices::getSingletonPtr()->getConfigService()->getValue("graphics", "fogmax"));
	}
	if (Ember::EmberServices::getSingletonPtr()->getConfigService()->itemExists("graphics", "fogcolour")) {
		varconf::Variable var = Ember::EmberServices::getSingletonPtr()->getConfigService()->getValue("graphics", "fogcolour");
		std::string stringValue(var);
		
		Ember::Tokeniser tokeniser;
		tokeniser.initTokens(stringValue);
		std::string r = tokeniser.nextToken();
		std::string b = tokeniser.nextToken();
		std::string g = tokeniser.nextToken();
		
		if (r != "" && b != "" && g != "") {
			fadeColour = Ogre::ColourValue(Ogre::StringConverter::parseReal(r), Ogre::StringConverter::parseReal(g), Ogre::StringConverter::parseReal(b));
		}
	}
	
	setFogValues(fogstartDistance, fogmaxDistance, fadeColour);
}

void Sky::ConfigService_EventChangedConfigItem(const std::string& section, const std::string& key)
{
	if (section == "graphics") {
		if (key == "fogstart" || key == "fogcolour" || key == "fogmax") {
			updateFogValuesFromConfig();
		}
	}
}

void Sky::setFogValues(float start, float end, Ogre::ColourValue colour) 
{
	Ogre::SceneManager* sceneMgr = EmberOgre::getSingleton().getSceneManager();
	sceneMgr->setFog( Ogre::FOG_LINEAR, colour, .001, start, end);
	
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


};
