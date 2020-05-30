//
// C++ Implementation: SoundDefinitionManager
//
// Description: 
//
//
// Author: Romulo Fernandes <abra185@gmail.com>, (C) 2008
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

#include "SoundDefinitionManager.h"
#include "XMLSoundDefParser.h"
#include "SoundGroupDefinition.h"

namespace Ember {
namespace OgreView {


SoundDefinitionManager::SoundDefinitionManager()
		: mSoundParser(new XMLSoundDefParser()) {

	Ogre::ResourceGroupManager::getSingleton()._registerScriptLoader(this);
}


SoundDefinitionManager::~SoundDefinitionManager() {
	Ogre::ResourceGroupManager::getSingleton()._unregisterScriptLoader(this);
}

const Ogre::StringVector& SoundDefinitionManager::getScriptPatterns() const {
	static Ogre::StringVector patterns{"*.sounddef", "*.sounddef.xml"};
	return patterns;
}

void SoundDefinitionManager::parseScript(Ogre::DataStreamPtr& stream, const Ogre::String& groupName) {
	auto defs = mSoundParser->parseScript(stream);
	for (auto& entry : defs) {
		mSoundGroupDefinitions.emplace(entry.first, std::move(entry.second));
	}
}

SoundGroupDefinition* SoundDefinitionManager::getSoundGroupDefinition(const std::string& name) {
	auto it(mSoundGroupDefinitions.find(name));
	if (it != mSoundGroupDefinitions.end()) {
		return it->second.get();
	}

	return nullptr;
}

}
}

