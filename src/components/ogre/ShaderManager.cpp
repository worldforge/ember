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

#include "ShadowCameraSetup.h"
#include "EmberOgrePrerequisites.h"
#include "EmberOgre.h"
#include "framework/Tokeniser.h"
#include "services/config/ConfigService.h"

#include <list>
#include <exception>
#include <algorithm>
#include <cctype>

#include <OgreMaterialManager.h>
#include <OgreMaterial.h>
#include <OgreSceneManager.h>
#include <OgreRenderWindow.h>

namespace EmberOgre {

ShaderManager::ShaderManager() :
	SetLevel("set_level", this, "Sets the graphics level. Parameters: <level>. Level is one of: high, medium, low."), mGraphicsLevel(LEVEL_DEFAULT), mBestGraphicsLevel(LEVEL_DEFAULT), mShadowCameraSetup(0)
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
	delete mShadowCameraSetup;
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
	delete mShadowCameraSetup;
	mShadowCameraSetup = new ShadowCameraSetup(*EmberOgre::getSingleton().getSceneManager());
}

void ShaderManager::setNoShadows()
{
	delete mShadowCameraSetup;
	mShadowCameraSetup = 0;
	Ogre::SceneManager* sceneMgr = EmberOgre::getSingleton().getSceneManager();
	sceneMgr->setShadowTechnique(Ogre::SHADOWTYPE_NONE);
	///This will make any other camera setup delete itself (unless held by another shared pointer).
	sceneMgr->setShadowCameraSetup(Ogre::ShadowCameraSetupPtr());
}

}
