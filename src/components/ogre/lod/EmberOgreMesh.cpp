/*
 * Copyright (c) 2013 Peter Szucs <peter.szucs.dev@gmail.com>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#include "EmberOgreMesh.h"
#include "LodManager.h"
#include "ProgressiveMeshGenerator.h"

#include "framework/LoggingInstance.h"

#include <OgreSubMesh.h>
#include <OgreLodStrategy.h>
#include <OgrePixelCountLodStrategy.h>
#include <OgreMeshManager.h>

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
	S_LOG_VERBOSE("Loading mesh " << name << ".");
}

void EmberOgreMesh::_configureMeshLodUsage(const LodConfig& lodConfigs)
{
	// In theory every mesh should have a submesh.
	assert(getNumSubMeshes() > 0);
	Ogre::SubMesh* submesh = getSubMesh(0);
	mNumLods = submesh->mLodFaceList.size() + 1;
	mMeshLodUsageList.resize(mNumLods);
	for (size_t n = 0, i = 0; i < lodConfigs.levels.size(); i++) {
		// Record usages. First Lod usage is the mesh itself.

		// Skip lods, which have the same amount of vertices. No buffer generated for them.
		if (!lodConfigs.levels[i].outSkipped) {

			// Generated buffers are less then the reported by ProgressiveMesh.
			// This would fail if you use QueuedProgressiveMesh and the MeshPtr is force unloaded before lod generation completes.
			assert(mMeshLodUsageList.size() > n + 1);
			Ogre::MeshLodUsage& lod = mMeshLodUsageList[++n];
			lod.userValue = lodConfigs.levels[i].distance;
			lod.value = getLodStrategy()->transformUserValue(lod.userValue);
			lod.edgeData = 0;
			lod.manualMesh.setNull();
		}
	}

	// Fix bug in Ogre with pixel count Lod strategy.
	// Changes [0, 20, 15, 10, 5] to [max, 20, 15, 10, 5].
	// Fixes PixelCountLodStrategy::getIndex() function, which returned always 0 index.
	if (getLodStrategy() == Ogre::PixelCountLodStrategy::getSingletonPtr()) {
		mMeshLodUsageList[0].userValue = std::numeric_limits<Ogre::Real>::max();
		mMeshLodUsageList[0].value = std::numeric_limits<Ogre::Real>::max();
	} else {
		mMeshLodUsageList[0].userValue = 0;
		mMeshLodUsageList[0].value = 0;
	}
}
void EmberOgreMesh::loadImpl()
{
	// Load the mesh.
	Ogre::Mesh::loadImpl();

	// We need a shared pointer of this.
	// TODO: Find a more effective way to get sharedPtr of itself.
	Ogre::MeshPtr thisPtr = static_cast<Ogre::MeshPtr>(Ogre::MeshManager::getSingleton().getByHandle(this->getHandle()));

	// If we loaded the Lod from the mesh file, then skip it.
	if (getNumLodLevels() == 1) {
		LodManager::getSingleton().loadLod(thisPtr);
	}
}

}
}
}
