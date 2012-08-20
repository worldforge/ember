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
	} catch (Ogre::FileNotFoundException ex) {
		// Exception is thrown if a mesh hasn't got a loddef.
		// By default, use the automatic mesh lod management system.
		loadAutomaticLod(mesh);
	}
}

void LodManager::loadLod(Ogre::MeshPtr mesh, const LodDefinition& def)
{
	assert(mesh->getNumLodLevels() == 1);
	if (def.getUseAutomaticLod()) {
		loadAutomaticLod(mesh);
	} else {
		// Load manual configs.
		LodConfig lodConfigs;
		lodConfigs.mesh = mesh;
		mesh->setLodStrategy(&Ogre::DistanceLodStrategy::getSingleton());
		const LodDefinition::LodDistanceMap& data = def.getManualLodData();
		LodDefinition::LodDistanceMap::const_iterator it;
		for (it = data.begin(); it != data.end(); it++) {
			const LodDistance& dist = it->second;

			if (dist.getType() == LodDistance::LDT_AUTOMATIC_VERTEX_REDUCTION) {
				LodLevel lodLevel;
				lodLevel.distance = it->first;
				lodLevel.reductionMethod = dist.getReductionMethod();
				lodLevel.reductionValue = dist.getReductionValue();
				lodConfigs.levels.push_back(lodLevel);
			} else {
				mesh->createManualLodLevel(it->first, dist.getMeshName());
			}
		}
		if (lodConfigs.levels.size() > 0) {

			// Uncomment the ProgressiveMesh of your choice.
			// NOTE: OgreProgressiveMeshExt doesn't support collapse cost based reduction.
			// OgreProgressiveMeshExt pm;
			// ProgressiveMeshGenerator pm;
			QueuedProgressiveMeshGenerator pm;
			pm.build(lodConfigs);
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
	// TODO: Implement automatic mesh lod management system!
}

}
}
}
