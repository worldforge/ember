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

namespace EmberOgre {

ShaderManager::ShaderManager() {
	init();
}

void ShaderManager::init() {
	std::list<std::string> schemesToCheck;
	schemesToCheck.push_back("High");
	schemesToCheck.push_back("Medium");
	schemesToCheck.push_back("Low");
	schemesToCheck.push_back("Default");

	std::list<std::string> materialsToCheck;
	materialsToCheck.push_back("/base/normalmap");
	materialsToCheck.push_back("/base/normalmap/specular");

	for (schemesToCheck::iterator I = schemesToCheck.begin(); I != schemesToCheck.end(); ++I) {

		bool supported = true;
		Ogre::MaterialManager::getSingleton()::setActiveScheme(*I);

		for (schemesToCheck::iterator J = schemesToCheck.begin(); J != materialsToCheck.end(); ++J) {
			supported &= checkMaterial(*J, *I);
			// Break when found first unsupported material
			if (!supported)
			{
				break;
			}
		}

		// Found some supported sheme, ok
		if (supported)
		{
			break;
		}
	}

	// No scheme is supported, this GFX sucks
	if (!supported)
	{
		throw "Ember won't run on this video card.";
	}
}

bool checkMaterial(std::string materialName, std::string schemeName)
{
	Ogre::MaterialPtr material = static_cast<Ogre::MaterialPtr>(Ogre::MaterialManager::getSingleton().create(materialName, "General"));
	if (material->getNumSupportedTechniques() == 0)
	{
		return false;
	}
	if (material->getBestTechnique()->getSchemeName() != schemeName)
	{
		return false;
	}
	return true;
}

ShaderManager::~ShaderManager() {
}

}
