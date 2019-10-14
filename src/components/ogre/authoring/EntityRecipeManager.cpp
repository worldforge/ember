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

namespace Ember
{
namespace OgreView
{
namespace Authoring
{
EntityRecipeManager::EntityRecipeManager() :
	mXmlSerializer()
{
	// Internal params for script loader
	mLoadOrder = 300.0f;
	mResourceType = "EntityRecipe";

	mScriptPatterns.emplace_back("*.entityrecipe");
	mScriptPatterns.emplace_back("*.entityrecipe.xml");
	Ogre::ResourceGroupManager::getSingleton()._registerScriptLoader(this);

	Ogre::ResourceGroupManager::getSingleton()._registerResourceManager(mResourceType, this);
}

EntityRecipeManager::~EntityRecipeManager()
{
	Ogre::ResourceGroupManager::getSingleton()._unregisterResourceManager(mResourceType);
	Ogre::ResourceGroupManager::getSingleton()._unregisterScriptLoader(this);
}

EntityRecipePtr EntityRecipeManager::create (const Ogre::String& name, const Ogre::String& group,
        bool isManual, Ogre::ManualResourceLoader* loader,
        const Ogre::NameValuePairList* createParams)
{
    return Ogre::static_pointer_cast<EntityRecipe>(createResource(name, group, isManual, loader, createParams));
}

void EntityRecipeManager::parseScript(Ogre::DataStreamPtr& stream, const Ogre::String& groupName)
{
	mXmlSerializer.parseScript(stream, groupName);
}

Ogre::Resource* EntityRecipeManager::createImpl(const Ogre::String& name, Ogre::ResourceHandle handle, const Ogre::String& group, bool isManual, Ogre::ManualResourceLoader* loader, const Ogre::NameValuePairList* /*createParams*/)
{
	return OGRE_NEW EntityRecipe(this, name, handle, group, isManual, loader);
}

}
}
}
