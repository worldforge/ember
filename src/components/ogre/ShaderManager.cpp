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

#include <list>

namespace EmberOgre {

ShaderManager::ShaderManager() : mGraphicsLevel(LEVEL_DEFAULT), mBestGraphicsLevel(LEVEL_DEFAULT)
{
	mGraphicSchemes[LEVEL_DEFAULT]		= std::string("Default");
	mGraphicSchemes[LEVEL_LOW]			= std::string("Low");
	mGraphicSchemes[LEVEL_MEDIUM]		= std::string("Medium");
	mGraphicSchemes[LEVEL_HIGH]			= std::string("High");
	mGraphicSchemes[LEVEL_EXPERIMENTAL]	= std::string("Experimental");

	init();
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
		throw "Something wrong with graphics";
	}

	// Don't start in experimental level
	if (mBestGraphicsLevel == LEVEL_EXPERIMENTAL)
	{
		setGraphicsLevel(LEVEL_HIGH);
	}
	else
	{
		setGraphicsLevel(mBestGraphicsLevel);
	}
}

bool ShaderManager::checkMaterial(std::string materialName, std::string schemeName)
{
	// OGRE scheme is switched in caller
	Ogre::MaterialPtr material = static_cast<Ogre::MaterialPtr>(Ogre::MaterialManager::getSingleton().create(materialName, "General"));
	// Loading material to compile it
	material->load();
	if (material->getNumSupportedTechniques() == 0) {
		S_LOG_INFO("The material '" << material->getName() << "' has no supported techniques with scheme " << schemeName << ". The reason for this is: \n" << material->getUnsupportedTechniquesExplanation());
		return false;
	}
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

ShaderManager::GraphicsLevel ShaderManager::setGraphicsLevel(ShaderManager::GraphicsLevel newLevel)
{
	S_LOG_INFO("Using graphics level " << mGraphicSchemes[newLevel]);
	mGraphicsLevel = newLevel;
}

}
