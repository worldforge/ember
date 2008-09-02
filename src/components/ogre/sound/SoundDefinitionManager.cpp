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
// along with this program; if not, write to the Free Software
// Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.//
//
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "SoundDefinitionManager.h"
#include "XMLSoundDefParser.h"
#include "SoundGroupDefinition.h"

namespace EmberOgre {

template<> EmberOgre::SoundDefinitionManager* Ember::Singleton<EmberOgre::SoundDefinitionManager>::ms_Singleton = 0;

SoundDefinitionManager::SoundDefinitionManager()
: mSoundParser(new XMLSoundDefParser(*this))
{
	mResourceType = "SoundDefinition";
	
	mScriptPatterns.push_back("*.sounddef");
	Ogre::ResourceGroupManager::getSingleton()._registerScriptLoader(this);
	Ogre::ResourceGroupManager::getSingleton()._registerResourceManager(mResourceType, this);
}


SoundDefinitionManager::~SoundDefinitionManager()
{
	Ogre::ResourceGroupManager::getSingleton()._unregisterScriptLoader(this);
	Ogre::ResourceGroupManager::getSingleton()._unregisterResourceManager(mResourceType);
	
	for (SoundGroupDefinitionStore::iterator I = mSoundGroupDefinitions.begin(); I != mSoundGroupDefinitions.end(); ++I) {
		delete I->second;
	}
}

void SoundDefinitionManager::parseScript (Ogre::DataStreamPtr &stream, const Ogre::String &groupName)
{
	mSoundParser->parseScript(stream);
}

Ogre::Resource* SoundDefinitionManager::createImpl(const Ogre::String& name, Ogre::ResourceHandle handle, const Ogre::String& group, bool isManual, Ogre::ManualResourceLoader* loader, const Ogre::NameValuePairList* createParams)
{
	return 0;
}

SoundGroupDefinition* SoundDefinitionManager::getSoundGroupDefinition(const std::string& name)
{
	std::map<std::string, SoundGroupDefinition*>::iterator it(mSoundGroupDefinitions.find(name));
	if (it != mSoundGroupDefinitions.end())
	{
		return it->second;
	}

	return NULL;
}

SoundGroupDefinition* SoundDefinitionManager::createSoundGroupDefinition(const std::string& name)
{
	SoundGroupDefinition* newModel = getSoundGroupDefinition(name);
	if (!newModel)
	{
		newModel = new SoundGroupDefinition();
		if (newModel)
		{
			#ifdef THREAD_SAFE
			pthread_mutex_lock(&mGroupModelsMutex);
			#endif

			mSoundGroupDefinitions[name] = newModel;

			#ifdef THREAD_SAFE
			pthread_mutex_unlock(&mGroupModelsMutex);
			#endif

			return newModel;
		}
		else
		{
			S_LOG_FAILURE("Failed to allocate sound group model " << name);
			return NULL;
		}
	}
	else
	{
		S_LOG_INFO("Sound Group definition " << name << " already exists.");
		return NULL;
	}
}

}

