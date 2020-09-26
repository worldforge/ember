//
// C++ Implementation: EntityRecipeManager
//
// Description:
//
//
// Author: Alexey Torkhov <atorkhov@gmail.com>, (C) 2008
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
// along with this program; if not, write to the Free Software Foundation,
// Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.//
//

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "EntityRecipeManager.h"

namespace Ember {
namespace OgreView {
namespace Authoring {
EntityRecipeManager::EntityRecipeManager() :
		mXmlSerializer() {
	Ogre::ResourceGroupManager::getSingleton()._registerScriptLoader(this);
}

EntityRecipeManager::~EntityRecipeManager() {
	Ogre::ResourceGroupManager::getSingleton()._unregisterScriptLoader(this);
}

const Ogre::StringVector& EntityRecipeManager::getScriptPatterns() const {
	static Ogre::StringVector patterns{"*.entityrecipe", "*.entityrecipe.xml"};
	return patterns;
}

Ogre::Real EntityRecipeManager::getLoadingOrder() const {
	return 300;
}

void EntityRecipeManager::parseScript(Ogre::DataStreamPtr& stream, const Ogre::String& groupName) {
	auto entries = mXmlSerializer.parseScript(stream);
	for (auto& entry: entries) {
		mEntries.emplace(entry.first, std::move(entry.second));
	}
}


}
}
}
