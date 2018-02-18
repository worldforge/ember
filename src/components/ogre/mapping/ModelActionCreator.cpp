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

#include <OgreResourceGroupManager.h>
#include "components/entitymapping/ChangeContext.h"
#include "components/entitymapping/Cases/CaseBase.h"
#include "components/ogre/model/ModelDefinitionManager.h"
#include "ModelActionCreator.h"
#include <Atlas/Message/Element.h>

namespace Ember {
namespace OgreView {
namespace Mapping {

ModelActionCreator::DisplayModelAction::DisplayModelAction(ModelActionCreator* creator, std::string modelName)
		: mCreator(creator),
		  mModelName(std::move(modelName)) {
}

void ModelActionCreator::DisplayModelAction::activate(EntityMapping::ChangeContext& context) {
	mCreator->mShowModelFn(mModelName);
}


ModelActionCreator::DisplayPartAction::DisplayPartAction(ModelActionCreator* creator, std::string partName)
		: mCreator(creator),
		  mPartName(std::move(partName)) {
}

void ModelActionCreator::DisplayPartAction::activate(EntityMapping::ChangeContext& context) {
	mCreator->mShowPartFn(mPartName);
}

ModelActionCreator::PresentModelAction::PresentModelAction(ModelActionCreator* creator)
		: mCreator(creator) {
}

void ModelActionCreator::PresentModelAction::activate(EntityMapping::ChangeContext& context) {
	if (mCreator->mEntity.hasAttr("present-model")) {
		auto& element = mCreator->mEntity.valueOfAttr("present-model");
		if (element.isString()) {
			mCreator->mShowModelFn(element.String());
		}
	}
}


ModelActionCreator::PresentMeshAction::PresentMeshAction(ModelActionCreator* creator)
		: mCreator(creator) {
}

void ModelActionCreator::PresentMeshAction::activate(EntityMapping::ChangeContext& context) {
	if (mCreator->mEntity.hasAttr("present-mesh")) {
		auto& element = mCreator->mEntity.valueOfAttr("present-mesh");
		if (element.isString()) {
			auto& meshName = element.String();
			//We'll automatically create a model which shows just the specified mesh.
			if (!Model::ModelDefinitionManager::getSingleton().resourceExists(meshName, Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME)) {
				auto modelDef = Model::ModelDefinitionManager::getSingleton().create(meshName, Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
				//Create a single submodel definition using the mesh
				modelDef->createSubModelDefinition(meshName);
			}
			mCreator->mShowModelFn(meshName);
		}
	}
}


ModelActionCreator::ModelActionCreator(Eris::Entity& entity, std::function<void(std::string)> showModelFn, std::function<void(std::string)> showPartFn)
		: mEntity(entity),
		  mShowModelFn(std::move(showModelFn)),
		  mShowPartFn(std::move(showPartFn)) {

}

void ModelActionCreator::createActions(EntityMapping::EntityMapping& modelMapping, EntityMapping::Cases::CaseBase* aCase, EntityMapping::Definitions::CaseDefinition& caseDefinition) {
	for (auto& actionDef : caseDefinition.getActions()) {
		if (actionDef.getType() == "display-part") {
			aCase->addAction(new DisplayPartAction(this, actionDef.getValue()));
		} else if (actionDef.getType() == "display-model") {
			aCase->addAction(new DisplayModelAction(this, actionDef.getValue()));
		} else if (actionDef.getType() == "present-model") {
			aCase->addAction(new PresentModelAction(this));
		} else if (actionDef.getType() == "present-mesh") {
			aCase->addAction(new PresentMeshAction(this));
		}
	}
}

}
}
}