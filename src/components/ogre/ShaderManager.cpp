//
// C++ Implementation: ShaderManager
//
// Description: 
//
//
// Author: Alexey Torkhov <atorkhov@gmail.com>, (C) 2009
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
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "ShaderManager.h"

#include "EmberOgrePrerequisites.h"
#include "EmberOgre.h"
#include "framework/Tokeniser.h"
#include "services/config/ConfigService.h"

#include <list>
#include <exception>
#include <algorithm>
#include <cctype>

namespace EmberOgre {

ShaderManager::ShaderManager() :
	SetLevel("set_level", this, "Sets the graphics level. Parameters: <level>. Level is one of: high, medium, low."), mGraphicsLevel(LEVEL_DEFAULT), mBestGraphicsLevel(LEVEL_DEFAULT)	
{
	mGraphicSchemes[LEVEL_DEFAULT]		= std::string("Default");
	mGraphicSchemes[LEVEL_LOW]			= std::string("Low");
	mGraphicSchemes[LEVEL_MEDIUM]		= std::string("Medium");
	mGraphicSchemes[LEVEL_HIGH]			= std::string("High");
	mGraphicSchemes[LEVEL_EXPERIMENTAL]	= std::string("Experimental");

	init();
	
	registerConfigListener("graphics", "level", sigc::mem_fun(*this, &ShaderManager::Config_Level), false);
	
}

void ShaderManager::init()
{
	// We normally want to check base materials
	std::list<std::string> materialsToCheck;
	materialsToCheck.push_back("/base/simple");
	materialsToCheck.push_back("/base/normalmap");
	materialsToCheck.push_back("/base/normalmap/specular");

	bool supported;

	// Iterate schemes from best to worst
	for (std::map<GraphicsLevel, std::string>::reverse_iterator I = mGraphicSchemes.rbegin(); I != mGraphicSchemes.rend(); ++I) {

		Ogre::MaterialManager::getSingleton().setActiveScheme(I->second);

		supported = true;
		for (std::list<std::string>::iterator J = materialsToCheck.begin(); J != materialsToCheck.end(); ++J) {
			supported &= checkMaterial(*J, I->second);
			// Break when found first unsupported material, no need to check others
			if (!supported) {
				break;
			}
		}

		// Found some supported sheme, ok
		if (supported) {
			mBestGraphicsLevel = I->first;
			S_LOG_INFO("Best graphics level is " << I->second);
			break;
		}
	}

	// No scheme is supported, something wrong with graphics
	if (!supported) {
		S_LOG_FAILURE("No schemes is supported");
		throw Ember::Exception("No schemes is supported, something wrong with graphics");
	}

	// Don't start in experimental level
	mGraphicsLevel = (mBestGraphicsLevel < LEVEL_EXPERIMENTAL) ? mBestGraphicsLevel : LEVEL_HIGH;

	GraphicsLevel configLevel = getLevelByName(std::string(Ember::EmberServices::getSingleton().getConfigService()->getValue("graphics", "level")));
	if (configLevel <= mBestGraphicsLevel) {
		mGraphicsLevel = configLevel;
	}

	setGraphicsLevel(mGraphicsLevel);
}

bool ShaderManager::checkMaterial(const std::string& materialName, const std::string& schemeName)
{
	// OGRE scheme is switched in caller
	Ogre::MaterialPtr material = static_cast<Ogre::MaterialPtr>(Ogre::MaterialManager::getSingleton().load(materialName, "General"));
	if (material->getNumSupportedTechniques() == 0) {
		S_LOG_INFO("The material '" << material->getName() << "' has no supported techniques with scheme " << schemeName << ". The reason for this is: \n" << material->getUnsupportedTechniquesExplanation());
		return false;
	}

	S_LOG_INFO("The material '" << material->getName() << "' has " << material->getNumSupportedTechniques() << " supported techniques out of " << material->getNumTechniques());

	// Check that we use desired scheme, but not fallbacked to default
	if (material->getBestTechnique()->getSchemeName() != schemeName) {
		S_LOG_INFO("The material '" << material->getName() << "' has best supported scheme " << material->getBestTechnique()->getSchemeName() << ". Was looking for " << schemeName);
		return false;
	}
	S_LOG_INFO("The material '" << material->getName() << "' supported with scheme " << schemeName);
	return true;
}

ShaderManager::~ShaderManager()
{
}

ShaderManager::GraphicsLevel ShaderManager::getGraphicsLevel()
{
	return mGraphicsLevel;
}

void ShaderManager::runCommand(const std::string &command, const std::string &args)
{
	if (SetLevel == command) {
		Ember::Tokeniser tokeniser;
		tokeniser.initTokens(args);
		std::string levelString = tokeniser.nextToken();
		Ember::EmberServices::getSingleton().getConfigService()->setValue("graphics", "level", levelString);
	}
}

void ShaderManager::Config_Level(const std::string& section, const std::string& key, varconf::Variable& variable)
{
	if (variable.is_string()) {
		setGraphicsLevel(getLevelByName(std::string(variable)));
	}
}


ShaderManager::GraphicsLevel ShaderManager::getLevelByName(const std::string &level)
{
	std::string levelString = level;
	std::transform(levelString.begin(), levelString.end(), levelString.begin(), (int(*)(int)) std::tolower);

	for (std::map<GraphicsLevel, std::string>::iterator I = mGraphicSchemes.begin(); I != mGraphicSchemes.end(); ++I) {
		std::string scheme = I->second;
		std::transform(scheme.begin(), scheme.end(), scheme.begin(), (int(*)(int)) std::tolower);
		if (levelString == scheme) {
			return I->first;
		}
	}

	return LEVEL_DEFAULT;
}

ShaderManager::GraphicsLevel ShaderManager::setGraphicsLevel(ShaderManager::GraphicsLevel newLevel)
{
	if (newLevel > mBestGraphicsLevel) {
		S_LOG_FAILURE("Cannot set graphics level " << mGraphicSchemes[newLevel]);
		return mGraphicsLevel;
	}

	S_LOG_INFO("Using graphics level " << mGraphicSchemes[newLevel]);
	Ogre::MaterialManager::getSingleton().setActiveScheme(mGraphicSchemes[newLevel]);

	Ogre::RenderWindow* window = EmberOgre::getSingleton().getRenderWindow();
	for (int i = 0; i < window->getNumViewports(); ++i) {
		window->getViewport(i)->setMaterialScheme(mGraphicSchemes[newLevel]);
	}

	switch (newLevel) {
	case LEVEL_EXPERIMENTAL:
	case LEVEL_HIGH:
		setPSSMShadows();
		break;

	case LEVEL_MEDIUM:
	case LEVEL_LOW:
	case LEVEL_DEFAULT:
		setNoShadows();
		break;
	}
	mGraphicsLevel = newLevel;
	
	EventLevelChanged.emit();
	
	return mGraphicsLevel;
}

void ShaderManager::setPSSMShadows()
{
	// Need to detect D3D or GL for best depth shadowmapping
	bool isOpenGL;
	if (Ogre::Root::getSingleton().getRenderSystem()->getName().find("GL") != Ogre::String::npos) {
		isOpenGL = true;
	} else {
		isOpenGL = false;
	}

	Ogre::SceneManager* sceneMgr = EmberOgre::getSingleton().getSceneManager();
	sceneMgr->setShadowTechnique(Ogre::SHADOWTYPE_TEXTURE_ADDITIVE_INTEGRATED);

	sceneMgr->setShadowTextureCount(5);
	//sceneMgr->setShadowColour(Ogre::ColourValue(0.1, 0.1, 0.1));
	sceneMgr->setShadowTextureSelfShadow(true);

	sceneMgr->setShadowTextureCountPerLightType(Ogre::Light::LT_DIRECTIONAL, 3);
	sceneMgr->setShadowTextureCountPerLightType(Ogre::Light::LT_POINT, 1);
	sceneMgr->setShadowTextureCountPerLightType(Ogre::Light::LT_SPOTLIGHT, 1);

	// Set shadow far distance before creating any lights
	//sceneMgr->setShadowFarDistance(100);

	if (isOpenGL) {
		// GL performs much better if you pick half-float format
		sceneMgr->setShadowTexturePixelFormat(Ogre::PF_FLOAT16_R);
	} else {
		// D3D is the opposite - if you ask for PF_FLOAT16_R you
		// get an integer format instead! You can ask for PF_FLOAT16_GR
		// but the precision doesn't work well
		sceneMgr->setShadowTexturePixelFormat(Ogre::PF_FLOAT32_R);
	}
	sceneMgr->setShadowTextureSize(1024);

	/*
	sceneMgr->setShadowTexturePixelFormat(PF_L8);
	sceneMgr->setShadowTextureCasterMaterial(StringUtil::BLANK);
	sceneMgr->setShadowTextureReceiverMaterial(StringUtil::BLANK);
	*/

	// Shadow camera setup
	//Ogre::LiSPSMShadowCameraSetup* liSPSMSetup = new Ogre::LiSPSMShadowCameraSetup();
	////liSPSMSetup->setUseAggressiveFocusRegion(false);

	Ogre::PSSMShadowCameraSetup* pssmSetup = OGRE_NEW Ogre::PSSMShadowCameraSetup();

	Ogre::PSSMShadowCameraSetup::SplitPointList splitPointList = pssmSetup->getSplitPoints();;
	splitPointList[0] = 1;
	splitPointList[1] = 15.0;
	splitPointList[2] = 50.0;
	splitPointList[3] = 500.0;
	pssmSetup->setSplitPoints(splitPointList);

	//Ogre::Camera& camera = EmberOgre::getSingleton().getMainCamera()->getCamera();
	//pssmSetup->calculateSplitPoints(3, camera.getNearClipDistance(), camera.getFarClipDistance());
	//pssmSetup->calculateSplitPoints(3, 1, 500);

	//pssmSetup->setUseAggressiveFocusRegion(false);

	pssmSetup->setSplitPadding(10.0);

	// Set the LISPM adjustment factor (see API documentation for these)
	pssmSetup->setOptimalAdjustFactor(0, 1.0);
	pssmSetup->setOptimalAdjustFactor(1, 1.0);
	pssmSetup->setOptimalAdjustFactor(2, 1.0);

	sceneMgr->setShadowCameraSetup(Ogre::ShadowCameraSetupPtr(pssmSetup));
////liSPSMSetup->setUseAggressiveFocusRegion(false);
}

void ShaderManager::setNoShadows()
{
	Ogre::SceneManager* sceneMgr = EmberOgre::getSingleton().getSceneManager();
	sceneMgr->setShadowTechnique(Ogre::SHADOWTYPE_NONE);
	///This will make any other camera setup delete itself (unless held by another shared pointer).
	sceneMgr->setShadowCameraSetup(Ogre::ShadowCameraSetupPtr());
}

}
