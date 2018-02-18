/*
 Copyright (C) 2018 Erik Ogenvik

 This program is free software; you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation; either version 2 of the License, or
 (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program; if not, write to the Free Software
 Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#include <components/ogre/model/ModelDefinitionManager.h>
#include <components/ogre/mapping/EmberEntityMappingManager.h>
#include <boost/algorithm/string.hpp>
#include "PresentAction.h"
#include "components/ogre/model/Model.h"
#include "components/ogre/model/ModelRepresentation.h"
#include "components/ogre/model/ModelRepresentationManager.h"
#include "components/entitymapping/ChangeContext.h"
#include "components/entitymapping/EntityMapping.h"
#include "components/ogre/Scene.h"

#include <OgreMeshManager.h>

namespace Ember {
namespace OgreView {

PresentAction::PresentAction(EmberEntity& entity, Scene& scene, EntityMapping::EntityMapping& mapping)
		: ModelActionBase(entity, scene, mapping) {
}

void PresentAction::activate(EntityMapping::ChangeContext& context) {

	if (mEntity.hasAttr("present")) {
		auto& element = mEntity.valueOfAttr("present");
		if (element.isString()) {
			auto& present = element.String();

			//If it's not an entity map it's either a mesh or a model.
			// Check if there's a model created already, if not we'll assume it's a mesh and create a model using that mesh
			if (!boost::ends_with(present, ".entitymap")) {
				if (!Model::ModelDefinitionManager::getSingleton().resourceExists(present, Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME)) {
					//We'll automatically create a model which shows just the specified mesh.
					auto modelDef = Model::ModelDefinitionManager::getSingleton().create(present, Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
					//Create a single submodel definition using the mesh
					modelDef->createSubModelDefinition(present);
				}
				showModel(present);
			}
		}
	}

}

}
}