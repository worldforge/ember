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
#include "LodManager.h"

template<>
Ember::OgreView::Lod::LodDefinitionManager * Ember::Singleton<Ember::OgreView::Lod::LodDefinitionManager>::ms_Singleton = 0;

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
	Ogre::ResourceGroupManager::getSingleton()._registerScriptLoader(this);
}

LodDefinitionManager::~LodDefinitionManager()
{
	Ogre::ResourceGroupManager::getSingleton()._unregisterScriptLoader(this);
}

Ogre::Resource* LodDefinitionManager::createImpl(const Ogre::String& name,
                                                 Ogre::ResourceHandle handle,
                                                 const Ogre::String& group,
                                                 bool isManual,
                                                 Ogre::ManualResourceLoader* loader,
                                                 const Ogre::NameValuePairList* createParams)
{
	return OGRE_NEW LodDefinition(this, name, handle, group, isManual, loader);
}

void LodDefinitionManager::parseScript(Ogre::DataStreamPtr& stream, const Ogre::String& groupName)
{
	std::string lodName = stream->getName();

	// Extract file name from path.
	size_t start = lodName.find_last_of("/\\");
	if (start != std::string::npos) {
		lodName = lodName.substr(start + 1);
	}

	// Create resource
	Ogre::ResourcePtr resource = create(lodName, groupName);

	// Set origin of resource.
	resource->_notifyOrigin(stream->getName());

	LodDefinition& loddef = *static_cast<LodDefinition*>(resource.get());
	mLodDefinitionSerializer.importLodDefinition(stream, loddef);
}

void LodDefinitionManager::exportScript(std::string meshName, LodDefinitionPtr definition)
{
	std::string lodName = LodManager::getSingleton().convertMeshNameToLodName(meshName);
	mLodDefinitionSerializer.exportScript(definition, lodName);
}

}
}
}
