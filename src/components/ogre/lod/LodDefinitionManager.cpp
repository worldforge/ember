/*
 * Copyright (C) 2012 Peter Szucs <peter.szucs.dev@gmail.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#include "LodDefinitionManager.h"

template<>
Ember::OgreView::Lod::LodDefinitionManager* Ember::Singleton<Ember::OgreView::Lod::LodDefinitionManager>::ms_Singleton = 0;

namespace Ember
{
namespace OgreView
{
namespace Lod
{

LodDefinitionManager::LodDefinitionManager(const std::string& exportDirectory) :
	mLodDefinitionSerializer(exportDirectory)
{
	// MeshManager has a load order of 350, so this should be bigger then that.
	mLoadOrder = 400.0f;
	mResourceType = "LodDefinition";
	mScriptPatterns.push_back("*.loddef");
	Ogre::ResourceGroupManager::getSingleton()._registerResourceManager(mResourceType, this);
}

LodDefinitionManager::~LodDefinitionManager()
{
	Ogre::ResourceGroupManager::getSingleton()._unregisterResourceManager(mResourceType);
}

Ogre::Resource* LodDefinitionManager::createImpl(const Ogre::String& name,
                                                 Ogre::ResourceHandle handle,
                                                 const Ogre::String& group,
                                                 bool isManual,
                                                 Ogre::ManualResourceLoader* loader,
                                                 const Ogre::NameValuePairList* createParams)
{
	return new LodDefinition(this, name, handle, group, isManual, loader);
}

}
}
}
