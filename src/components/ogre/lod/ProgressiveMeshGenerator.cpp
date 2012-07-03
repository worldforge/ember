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

#include "ProgressiveMeshGenerator.h"

namespace Ember
{
namespace OgreView
{
namespace Lod
{

ProgressiveMeshGenerator::ProgressiveMeshGenerator(const Ogre::VertexData* vertexData, const Ogre::IndexData* indexData) :
	Ogre::ProgressiveMesh(vertexData, indexData)
{

}

void ProgressiveMeshGenerator::build(const LodConfigList& lodConfigs, LODFaceList& outList)
{
#ifndef NDEBUG
	// Do not call this with empty Lod.
	assert(lodConfigs.size());

	// Lod distances needs to be sorted.
	for (int i = 1; i < lodConfigs.size(); i++) {
		assert(lodConfigs[i - 1].distance < lodConfigs[i].distance);
	}
#endif

	computeAllCosts();

	mCurrNumIndexes = mpIndexData->indexCount;
	int numVerts = mNumCommonVertices;
	bool abandon = false;
	int lodCount = lodConfigs.size();
	outList.resize(lodCount);
	for (int i = 0; i < lodCount; i++) {

		size_t neededVerts = calcLodVertexCount(lodConfigs[i]);
		neededVerts = std::max(neededVerts, (size_t) 3);

		// Vertex count should be more then 3 and less then max vertices.
		for (; !abandon && neededVerts < numVerts; numVerts--) {
			size_t nextIndex = getNextCollapser();

			int workDataLen = mWorkingData.size();
			for (int i = 0; i < workDataLen; i++) {
				PMVertex& collapser = mWorkingData[i].mVertList.at(nextIndex);
				// This will reduce mCurrNumIndexes and recalc costs as required.
				if (collapser.collapseTo == NULL) {
					// Must have run out of valid collapsables.
					abandon = true;
					break;
				}
				assert(collapser.collapseTo->removed == false);
				collapse(&collapser);
			}
		}

		// Bake a new LOD and add it to the list
		Ogre::IndexData* newLod = OGRE_NEW Ogre::IndexData();
		bakeNewLOD(newLod);
		outList[i] = newLod;
	}
}

size_t ProgressiveMeshGenerator::calcLodVertexCount(const LodConfig& lodConfig)
{
	switch (lodConfig.reductionMethod) {
	case ProgressiveMeshGenerator::VRM_PROPORTIONAL:
		return mNumCommonVertices - (mNumCommonVertices * lodConfig.reductionValue);

	case ProgressiveMeshGenerator::VRM_CONSTANT:
		return mNumCommonVertices - lodConfig.reductionValue;

	default:
		assert(0);
		return mNumCommonVertices;
	}
}

}
}
}
