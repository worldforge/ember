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

#include "LodManager.h"
#include "LodDefinition.h"
#include "LodDefinitionManager.h"
#include "EmberOgreMesh.h"
#include "QueuedProgressiveMeshGenerator.h"

#include <OgrePixelCountLodStrategy.h>
#include <OgreDistanceLodStrategy.h>

template<>
Ember::OgreView::Lod::LodManager * Ember::Singleton<Ember::OgreView::Lod::LodManager>::ms_Singleton = 0;

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

void LodManager::loadLod(Ogre::MeshPtr mesh)
{
	assert(mesh->getNumLodLevels() == 1);
	std::string lodDefName = convertMeshNameToLodName(mesh->getName());

	try {
		Ogre::ResourcePtr resource = LodDefinitionManager::getSingleton().load(lodDefName, "General");
		const LodDefinition& def = *static_cast<const LodDefinition*>(resource.get());
		loadLod(mesh, def);
	} catch (const Ogre::FileNotFoundException& ex) {
		// Exception is thrown if a mesh hasn't got a loddef.
		// By default, use the automatic mesh lod management system.
		loadAutomaticLod(mesh);
	}
}

void LodManager::loadLod(Ogre::MeshPtr mesh, const LodDefinition& def)
{
	if (def.getUseAutomaticLod()) {
		loadAutomaticLod(mesh);
	} else if (def.getLodDistanceCount() == 0) {
		mesh->removeLodLevels();
		return;
	} else {
		Ogre::LodStrategy* strategy;
		if (def.getStrategy() == LodDefinition::LS_DISTANCE) {
			strategy = &Ogre::DistanceLodStrategy::getSingleton();
		} else {
			strategy = &Ogre::PixelCountLodStrategy::getSingleton();
		}
		mesh->setLodStrategy(strategy);

		if (def.getType() == LodDefinition::LT_AUTOMATIC_VERTEX_REDUCTION) {
			// Automatic vertex reduction
			LodConfig lodConfig;
			lodConfig.mesh = mesh;
			const LodDefinition::LodDistanceMap& data = def.getManualLodData();
			if (def.getStrategy() == LodDefinition::LS_DISTANCE) {
				// TODO: Use C++11 lambda, instead of template.
				loadAutomaticLodImpl(data.begin(), data.end(), lodConfig);
			} else {
				loadAutomaticLodImpl(data.rbegin(), data.rend(), lodConfig);
			}
			// Uncomment the ProgressiveMesh of your choice.
			// NOTE: OgreProgressiveMeshExt doesn't support collapse cost based reduction.
			// OgreProgressiveMeshExt pm;
			// ProgressiveMeshGenerator pm;
			QueuedProgressiveMeshGenerator pm;
			pm.build(lodConfig);
		} else {
			// User created Lod

			mesh->removeLodLevels();

			const LodDefinition::LodDistanceMap& data = def.getManualLodData();
			if (def.getStrategy() == LodDefinition::LS_DISTANCE) {
				// TODO: Use C++11 lambda, instead of template.
				loadUserLodImpl(data.begin(), data.end(), mesh.get());
			} else {
				loadUserLodImpl(data.rbegin(), data.rend(), mesh.get());
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
void LodManager::loadAutomaticLod(Ogre::MeshPtr mesh)
{
	LodConfig lodConfigs;
	lodConfigs.mesh = mesh;
	mesh->setLodStrategy(&Ogre::PixelCountLodStrategy::getSingleton());
	LodLevel lodLevel;
	lodLevel.reductionMethod = LodLevel::VRM_COLLAPSE_COST;
	Ogre::Real radius = mesh->getBoundingSphereRadius();
	for (int i = 2; i < 6; i++) {
		Ogre::Real i4 = (Ogre::Real) (i * i * i * i);
		Ogre::Real i5 = i4 * (Ogre::Real) i;
		// Distance = pixel count
		// Constant: zoom of the Lod. This could be scaled based on resolution.
		//     Higher constant means first Lod is nearer to camera. Smaller constant means the first Lod is further away from camera.
		// i4: The stretching. Normally you want to have more Lods in the near, then in far away.
		//     i4 means distance is divided by 16=(2*2*2*2), 81, 256, 625=(5*5*5*5).
		//     if 16 would be smaller, the first Lod would be nearer. if 625 would be bigger, the last Lod would be further awaay.
		// if you increase 16 and decrease 625, first and Last Lod distance would be smaller.
		lodLevel.distance = 3388608.f / i4;
		
		// reductionValue = collapse cost
		// Radius: Edges are multiplied by the length, when calculating collapse cost. So as a base value we use radius, which should help in balancing collapse cost to any mesh size.
		// The constant and i5 are playing together. 1/(1/100k*i5)
		// You need to determine the quality of nearest Lod and the furthest away first.
		// I have choosen 1/(1/100k*(2^5)) = 3125 for nearest Lod and 1/(1/100k*(5^5)) = 32 for nearest Lod.
		// if you divide radius by a bigger number, it means smaller reduction. So radius/3125 is very small reduction for nearest Lod.
		// if you divide radius by a smaller number, it means bigger reduction. So radius/32 means agressive reduction for furthest away lod.
		// current values: 3125, 411, 97, 32
		lodLevel.reductionValue = radius / 100000.f * i5;
		lodConfigs.levels.push_back(lodLevel);
	}

	QueuedProgressiveMeshGenerator pm;
	pm.build(lodConfigs);
}

template<typename T>
void LodManager::loadAutomaticLodImpl(T it, T itEnd, LodConfig& lodConfig)
{
	for (; it != itEnd; it++) {
		const LodDistance& dist = it->second;
		LodLevel lodLevel;
		lodLevel.distance = it->first;
		lodLevel.reductionMethod = dist.getReductionMethod();
		lodLevel.reductionValue = dist.getReductionValue();
		lodConfig.levels.push_back(lodLevel);
	}
}

template<typename T>
void LodManager::loadUserLodImpl(T it, T itEnd, Ogre::Mesh* mesh)
{
	for (; it != itEnd; it++) {
		const Ogre::String& meshName = it->second.getMeshName();
		if (meshName != "") {
			assert(Ogre::ResourceGroupManager::getSingleton().resourceExistsInAnyGroup(meshName));
			mesh->createManualLodLevel(it->first, meshName);
		}
	}
}

}
}
}
