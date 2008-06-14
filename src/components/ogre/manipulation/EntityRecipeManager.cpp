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
// along with this program; if not, write to the Free Software
// Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.//
//

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "EntityRecipeManager.h"
#include "EntityRecipe.h"
#include "framework/tinyxml/tinyxml.h"
#include "components/ogre/XMLHelper.h"

template<> EmberOgre::EntityRecipeManager* Ember::Singleton<EmberOgre::EntityRecipeManager>::ms_Singleton = 0;

namespace EmberOgre
{

EntityRecipeManager::EntityRecipeManager() : mXmlSerializer()
{
	// Internal params for script loader
	mLoadOrder = 300.0f;
	mResourceType = "EntityRecipe";

	mScriptPatterns.push_back("*.entityrecipe");
	mScriptPatterns.push_back("*.entityrecipe.xml");
	Ogre::ResourceGroupManager::getSingleton()._registerScriptLoader(this);

	Ogre::ResourceGroupManager::getSingleton()._registerResourceManager(mResourceType, this);
}

EntityRecipeManager::~EntityRecipeManager()
{
	Ogre::ResourceGroupManager::getSingleton()._unregisterResourceManager(mResourceType);
	Ogre::ResourceGroupManager::getSingleton()._unregisterScriptLoader(this);
}

Ogre::ResourcePtr EntityRecipeManager::create(const Ogre::String& name, const Ogre::String& group,
	bool isManual, Ogre::ManualResourceLoader* loader, const Ogre::NameValuePairList* createParams)
{
	Ogre::ResourcePtr ret = getByName(name);
	if (ret.isNull())
	{
		return Ogre::ResourceManager::create(name, group, isManual, loader, createParams);
	}
	S_LOG_WARNING("EntityRecipe with name " << name << " already exists.");
	return Ogre::ResourcePtr();
}

void EntityRecipeManager::parseScript(Ogre::DataStreamPtr& stream, const Ogre::String& groupName)
{
	mXmlSerializer.parseScript(stream, groupName);
}

Ogre::Resource* EntityRecipeManager::createImpl(const Ogre::String& name, Ogre::ResourceHandle handle,
    const Ogre::String& group, bool isManual, Ogre::ManualResourceLoader* loader,
    const Ogre::NameValuePairList* createParams)
{
	return new EntityRecipe(this, name, handle, group, isManual, loader);
}

}
