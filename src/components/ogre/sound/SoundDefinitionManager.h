//
// C++ Interface: SoundDefinitionManager
//
// Description: 
//
//
// Author: Erik Ogenvik <erik@ogenvik.org>, (C) 2007
// 		Romulo Fernandes <abra185@gmail.com>, (C) 2008
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
#ifndef EMBEROGRE_SOUNDDEFINITIONMANAGER_H
#define EMBEROGRE_SOUNDDEFINITIONMANAGER_H


#include "SoundGroupDefinition.h"
#include "SoundDefinition.h"
#include "components/ogre/EmberOgrePrerequisites.h"

#include <OgreResourceManager.h>
#include "framework/Singleton.h"
#include <memory>
#include <unordered_map>

namespace Ember {
namespace OgreView {


class XMLSoundDefParser;

/**
 * @author Romulo Fernandes <abra185@gmail.com>
 * @brief Handles all sound definitions.
 */
class SoundDefinitionManager :
		public Ogre::ScriptLoader,
		public Singleton<SoundDefinitionManager> {
public:

	SoundDefinitionManager();

	~SoundDefinitionManager() override;

	void parseScript(Ogre::DataStreamPtr& stream, const Ogre::String& groupName) override;

	const Ogre::StringVector& getScriptPatterns() const override;

	Ogre::Real getLoadingOrder() const override {
		return 300.0;
	}

	/**
	* Returns the SoundDefinition from its name
	*
	* @param name The desired SoundDefinition name
	* @return A pointer to the SoundDefinition or nullptr if it can't be found
	* TODO: move this to components/ogre since it belongs there and not here
	*/
	SoundGroupDefinition* getSoundGroupDefinition(const std::string& name);

protected:
	std::unique_ptr<XMLSoundDefParser> mSoundParser;

	/**
	* Thats the list of the sound groups parsed in
	* sounddefs
	*/
	std::unordered_map<std::string, std::unique_ptr<SoundGroupDefinition>> mSoundGroupDefinitions;
};

}

}

#endif
