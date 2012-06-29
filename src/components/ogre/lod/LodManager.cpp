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

#include "LodManager.h"
#include "LodDefinitionManager.h"

template<>
Ember::OgreView::Lod::LodManager* Ember::Singleton<Ember::OgreView::Lod::LodManager>::ms_Singleton = 0;

namespace Ember
{
namespace OgreView
{
namespace Lod
{

LodManager::LodManager()
{
	Ogre::ResourceGroupManager::getSingleton().addResourceGroupListener(this);
}

LodManager::~LodManager()
{
	Ogre::ResourceGroupManager::getSingleton().removeResourceGroupListener(this);
}

void LodManager::resourceLoadStarted(const Ogre::ResourcePtr& resource)
{
	std::string type = resource->getCreator()->getResourceType();
	if (type == "Mesh") {
		mCurrentMesh = resource;
	} else if (!mCurrentMesh.isNull()) {
		mCurrentMesh.setNull();
	}
}

void LodManager::resourceLoadEnded(void)
{
	if (!mCurrentMesh.isNull()) {
		LodManager::getSingleton().LoadLod(mCurrentMesh);
	}
}

bool LodManager::LoadLod(const Ogre::MeshPtr& mesh)
{
	mesh->removeLodLevels();
	std::string lodDefName = convertMeshNameToLodName(mesh->getName());
	try {
		Lod::LodDefinitionManager::getSingleton().load(lodDefName, "General");
	} catch (Ogre::FileNotFoundException ex) {
		// Exception is thrown if a mesh hasn't got a loddef.
		return false;
	}
	return true;
}

std::string LodManager::convertMeshNameToLodName(std::string meshName)
{
	size_t start = meshName.find_last_of("/\\");
	if (start != std::string::npos) {
		meshName = meshName.substr(start + 1);
	}

	size_t end = meshName.find_last_of(".");
	if (end != std::string::npos) {
		meshName = meshName.substr(0, end);
	}

	meshName += ".loddef";
	return meshName;
}

}
}
}
