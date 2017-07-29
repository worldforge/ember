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

#include "components/ogre/EmberOgrePrerequisites.h"

#include <OgreResourceManager.h>
#include "framework/Singleton.h"
#include <memory>
#include <unordered_map>

namespace Ember {
namespace OgreView {

class SoundGroupDefinition;
class XMLSoundDefParser;
/**
 * @author Romulo Fernandes <abra185@gmail.com>
 * @brief Handles all sound definitions.
 */
class SoundDefinitionManager : public Ogre::ResourceManager, public Singleton<SoundDefinitionManager>
{
public:
	typedef std::unordered_map<std::string, SoundGroupDefinition*> SoundGroupDefinitionStore;

	SoundDefinitionManager();

	virtual ~SoundDefinitionManager();

	virtual void parseScript(Ogre::DataStreamPtr &stream, const Ogre::String &groupName);
	
	/**
	* Register a new SoundDefinition used to define soundgroups
	*
	* @return A pointer to the new created SoundDefinition, if it fails, returns nullptr
	* TODO: move this to components/ogre since it belongs there and not here
	*/
	SoundGroupDefinition* createSoundGroupDefinition(const std::string& name);

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

	Ogre::Resource* createImpl(const Ogre::String& name, Ogre::ResourceHandle handle, const Ogre::String& group, bool isManual, Ogre::ManualResourceLoader* loader, const Ogre::NameValuePairList* createParams);
	
	/**
	* Thats the list of the sound groups parsed in
	* sounddefs
	*/
	SoundGroupDefinitionStore mSoundGroupDefinitions;
};

}

}

#endif
