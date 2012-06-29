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
#include "LodDefinition.h"
#include "LodDefinitionManager.h"
#include <OgreSubMesh.h>

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
}

LodManager::~LodManager()
{

}

void LodManager::LoadLod(Ogre::Mesh& mesh)
{
	assert(mesh.getNumLodLevels() == 1);
	std::string lodDefName = convertMeshNameToLodName(mesh.getName());

	try {
		Ogre::ResourcePtr resource = LodDefinitionManager::getSingleton().load(lodDefName, "General");
		const LodDefinition& def = *static_cast<const LodDefinition*>(resource.get());
		LoadLod(mesh, def);
	} catch (Ogre::FileNotFoundException ex) {
		// Exception is thrown if a mesh hasn't got a loddef.
		// By default, use the automatic mesh lod management system.
		LoadAutomaticLod(mesh);
	}
}

void LodManager::LoadLod(Ogre::Mesh& mesh, const LodDefinition& def)
{
	if (def.getUseAutomaticLod()) {
		LoadAutomaticLod(mesh);
	} else {
		// Load manual configs.
		Ogre::Mesh::LodValueList values(1);
		const std::map<int, LodDistance>& data = def.getManualLodData();
		std::map<int, LodDistance>::const_iterator it;
		for (it = data.begin(); it != data.end(); it++) {

			const LodDistance& dist = it->second;
			if (dist.getType() == LodDistance::LDT_AUTOMATIC_VERTEX_REDUCTION) {
				values[0] = it->first;
				mesh.generateLodLevels(values, dist.getReductionMethod(), dist.getReductionValue());
			} else {
				mesh.createManualLodLevel(it->first, dist.getMeshName());
			}
		}
	}
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

void LodManager::LoadAutomaticLod(Ogre::Mesh& mesh)
{
	// TODO: Implement automatic mesh lod management system!
}

void LodManager::reduceVertexCount(Ogre::Mesh& mesh,
                                   Ogre::ProgressiveMesh::VertexReductionQuota reductionMethod,
                                   Ogre::Real reductionValue)
{
	Ogre::Mesh::SubMeshIterator iter = mesh.getSubMeshIterator();
	while (iter.hasMoreElements()) {
		Ogre::SubMesh& submesh = *iter.getNext();

		// Check if triangles are present
		if (submesh.indexData->indexCount > 0) {
			// Set up data for reduction
			Ogre::VertexData* pVertexData = submesh.useSharedVertices ? mesh.sharedVertexData : submesh.vertexData;

			Ogre::ProgressiveMesh pm(pVertexData, submesh.indexData);
			pm.build(
			    1,
			    &(submesh.mLodFaceList),
			    reductionMethod, reductionValue);
		} else {
			// create empty index data for each lod
			submesh.mLodFaceList.push_back(OGRE_NEW Ogre::IndexData);
		}
	}
	/* Dead end: I can't access mMeshLodUsageList to insert new Lod level outside of Ogre::Mesh.
	 *
	 * // Iterate over the lods and record usage
	 * LodValueList::const_iterator ivalue, ivalueend;
	 * ivalueend = lodValues.end();
	 * mMeshLodUsageList.resize(lodValues.size() + 1);
	 * MeshLodUsageList::iterator ilod = mMeshLodUsageList.begin();
	 * for (ivalue = lodValues.begin(); ivalue != ivalueend; ++ivalue)
	 * {
	 *  // Record usage
	 *  MeshLodUsage& lod = *++ilod;
	 *  lod.userValue = (*ivalue);
	 *  lod.value = mLodStrategy->transformUserValue(lod.userValue);
	 *  lod.edgeData = 0;
	 *  lod.manualMesh.setNull();
	 * }
	 * mNumLods = static_cast<ushort>(lodValues.size() + 1);
	 */
}

}
}
}
