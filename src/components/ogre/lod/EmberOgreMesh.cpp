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

#include "EmberOgreMesh.h"
#include "LodManager.h"
#include "ProgressiveMeshGenerator.h"

#include "framework/LoggingInstance.h"

#include <OgreSubMesh.h>
#include <OgreLodStrategy.h>

namespace Ember
{
namespace OgreView
{
namespace Lod
{

EmberOgreMesh::EmberOgreMesh(Ogre::ResourceManager* creator,
                             const Ogre::String& name,
                             Ogre::ResourceHandle handle,
                             const Ogre::String& group,
                             bool isManual,
                             Ogre::ManualResourceLoader* loader) :
	Ogre::Mesh(creator, name, handle, group, isManual, loader)
{
	S_LOG_INFO("Loading mesh " << mName << ".");
}

void EmberOgreMesh::generateLodLevels(ProgressiveMeshGenerator::LodConfigList& lodConfigs)
{
	removeLodLevels();

	ProgressiveMeshGenerator pm(*this);
	pm.build(lodConfigs);

	Ogre::SubMesh* submesh = getSubMesh(0);
	mNumLods = submesh->mLodFaceList.size() + 1;
	mMeshLodUsageList.resize(mNumLods);
	for (int n = 0, i = 0; i < lodConfigs.size(); i++) {
		// Record usage. First Lod usage is the mesh itself.
		if(!lodConfigs[i].outSkipped){
			Ogre::MeshLodUsage& lod = mMeshLodUsageList[++n];
			lod.userValue = lodConfigs[i].distance;
			lod.value = mLodStrategy->transformUserValue(lod.userValue);
			lod.edgeData = 0;
			lod.manualMesh.setNull();
		}
	}
}

void EmberOgreMesh::loadImpl()
{
	// Load the mesh.
	Ogre::Mesh::loadImpl();

	// If we loaded the Lod from the mesh file, then skip it.
	if (getNumLodLevels() == 1) {
		LodManager::getSingleton().loadLod(*this);
	}
}

}
}
}
