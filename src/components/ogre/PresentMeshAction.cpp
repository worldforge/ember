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

#include "PresentMeshAction.h"
#include "components/ogre/model/Model.h"
#include "components/ogre/model/ModelRepresentation.h"
#include "components/ogre/model/ModelRepresentationManager.h"
#include "components/ogre/model/ModelDefinitionManager.h"
#include "components/entitymapping/ChangeContext.h"
#include "components/entitymapping/EntityMapping.h"
#include "components/ogre/Scene.h"

namespace Ember {
namespace OgreView {

PresentMeshAction::PresentMeshAction(EmberEntity& entity, Scene& scene, EntityMapping::EntityMapping& mapping)
		: ModelActionBase(entity, scene, mapping) {

}

void PresentMeshAction::activate(EntityMapping::ChangeContext& context) {

	if (mEntity.hasAttr("present-mesh")) {
		auto& element = mEntity.valueOfAttr("present-mesh");
		if (element.isString()) {
			showMesh(element.String());
		}
	}
}

void PresentMeshAction::showMesh(const std::string& meshName) {

	//We'll automatically create a model which shows just the specified mesh.
	if (!Model::ModelDefinitionManager::getSingleton().resourceExists(meshName, Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME)) {
		auto modelDef = Model::ModelDefinitionManager::getSingleton().create(meshName, Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
		//Create a single submodel definition using the mesh
		modelDef->createSubModelDefinition(meshName);
	}
	showModel(meshName);
}
}
}