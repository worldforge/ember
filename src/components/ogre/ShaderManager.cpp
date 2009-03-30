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

ShaderManager::ShaderManager()
{
	init();
}

void ShaderManager::init()
{
	std::list<std::string> schemesToCheck;
	schemesToCheck.push_back("High");
	schemesToCheck.push_back("Medium");
	schemesToCheck.push_back("Low");
	schemesToCheck.push_back("Default");

	std::list<std::string> materialsToCheck;
	materialsToCheck.push_back("/base/normalmap");
	materialsToCheck.push_back("/base/normalmap/specular");

	bool supported;

	for (std::list<std::string>::iterator I = schemesToCheck.begin(); I != schemesToCheck.end(); ++I) {

		Ogre::MaterialManager::getSingleton().setActiveScheme(*I);

		supported = true;

		for (std::list<std::string>::iterator J = materialsToCheck.begin(); J != materialsToCheck.end(); ++J) {
			supported &= checkMaterial(*J, *I);
			// Break when found first unsupported material, no need to check others
			if (!supported) {
				break;
			}
		}

		// Found some supported sheme, ok
		if (supported) {
			break;
		}
	}

	// No scheme is supported, this GFX sucks
	if (!supported) {
		throw "Ember won't run on this video card.";
	}
}

bool ShaderManager::checkMaterial(std::string materialName, std::string schemeName)
{
	Ogre::MaterialPtr material = static_cast<Ogre::MaterialPtr>(Ogre::MaterialManager::getSingleton().create(materialName, "General"));
	// Loading material to compile it
	material->load();
	if (material->getNumSupportedTechniques() == 0) {
		S_LOG_INFO("The material '" << material->getName() << "' has no supported techniques with scheme '" << schemeName << "'. The reason for this is: \n" << material->getUnsupportedTechniquesExplanation());
		return false;
	}
	if (material->getBestTechnique()->getSchemeName() != schemeName) {
		S_LOG_INFO("The material '" << material->getName() << "' has best supported scheme '" << material->getBestTechnique()->getSchemeName() << "'. Expected '" << schemeName << "'.");
		return false;
	}
	S_LOG_INFO("The material '" << material->getName() << "' supported with scheme '" << schemeName << "'.");
	return true;
}

ShaderManager::~ShaderManager()
{
}

}
