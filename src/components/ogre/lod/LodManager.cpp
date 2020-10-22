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
#include "LodDefinitionManager.h"
#include "ScaledPixelCountLodStrategy.h"

#include <MeshLodGenerator/OgreMeshLodGenerator.h>
#include <OgrePixelCountLodStrategy.h>
#include <OgreDistanceLodStrategy.h>

namespace Ember
{
namespace OgreView
{
namespace Lod
{

LodManager::LodManager() = default;

LodManager::~LodManager() = default;

void LodManager::loadLod(Ogre::MeshPtr mesh)
{
	assert(mesh->getNumLodLevels() == 1);
	std::string lodDefName = convertMeshNameToLodName(mesh->getName());

	try {
		Ogre::ResourcePtr resource = LodDefinitionManager::getSingleton().load(lodDefName, "General");
		const LodDefinition& def = *static_cast<const LodDefinition*>(resource.get());
		loadLod(mesh, def);
	} catch (const Ogre::FileNotFoundException&) {
		// Exception is thrown if a mesh hasn't got a loddef.
		// By default, use the automatic mesh lod management system.
		Ogre::MeshLodGenerator::getSingleton().generateAutoconfiguredLodLevels(mesh);
	}
}

void LodManager::loadLod(Ogre::MeshPtr mesh, const LodDefinition& def)
{
	if (def.getUseAutomaticLod()) {
		Ogre::MeshLodGenerator::getSingleton().generateAutoconfiguredLodLevels(mesh);
	} else if (def.getLodDistanceCount() == 0) {
		mesh->removeLodLevels();
		return;
	} else {
		Ogre::LodStrategy* strategy;
		if (def.getStrategy() == LodDefinition::LS_DISTANCE) {
			strategy = &Ogre::DistanceLodBoxStrategy::getSingleton();
		} else {
			strategy = &ScaledPixelCountLodStrategy::getSingleton();
		}
		mesh->setLodStrategy(strategy);

		if (def.getType() == LodDefinition::LT_AUTOMATIC_VERTEX_REDUCTION) {
			// Automatic vertex reduction
			Ogre::LodConfig lodConfig;
			lodConfig.mesh = mesh;
			lodConfig.strategy = strategy;
			const LodDefinition::LodDistanceMap& data = def.getManualLodData();
			if (def.getStrategy() == LodDefinition::LS_DISTANCE) {
				// TODO: Use C++11 lambda, instead of template.
				loadAutomaticLodImpl(data.begin(), data.end(), lodConfig);
			} else {
				loadAutomaticLodImpl(data.rbegin(), data.rend(), lodConfig);
			}
			Ogre::MeshLodGenerator::getSingleton().generateLodLevels(lodConfig);
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

template<typename T>
void LodManager::loadAutomaticLodImpl(T it, T itEnd, Ogre::LodConfig& lodConfig)
{
	for (; it != itEnd; it++) {
		const LodDistance& dist = it->second;
		Ogre::LodLevel lodLevel;
		lodLevel.distance = it->first;
		lodLevel.reductionMethod = dist.reductionMethod;
		lodLevel.reductionValue = dist.reductionValue;
		lodConfig.levels.push_back(lodLevel);
	}
}

template<typename T>
void LodManager::loadUserLodImpl(T it, T itEnd, Ogre::Mesh* mesh)
{
	for (; it != itEnd; it++) {
		const Ogre::String& meshName = it->second.meshName;
		if (meshName != "") {
			assert(Ogre::ResourceGroupManager::getSingleton().resourceExistsInAnyGroup(meshName));
			mesh->updateManualLodLevel(static_cast<Ogre::ushort>(it->first), meshName);
		}
	}
}

}
}
}
