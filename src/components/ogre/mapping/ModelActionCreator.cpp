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

#include "components/entitymapping/ChangeContext.h"
#include "components/entitymapping/Cases/CaseBase.h"
#include "components/ogre/model/ModelDefinitionManager.h"
#include "ModelActionCreator.h"
#include <Atlas/Message/Element.h>
#include <boost/algorithm/string.hpp>

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

ModelActionCreator::PresentAction::PresentAction(ModelActionCreator* creator)
        : mCreator(creator) {
}

void ModelActionCreator::PresentAction::activate(EntityMapping::ChangeContext& context) {

    if (mCreator->mEntity.hasProperty("present")) {
        auto& element = mCreator->mEntity.valueOfProperty("present");
        if (element.isString()) {
            auto& present = element.String();

            //If it's not an entity map it's either a mesh or a model.
            // Check if there's a model created already, if not we'll assume it's a mesh and create a model using that mesh
            if (!boost::ends_with(present, ".entitymap")) {
                if (!Model::ModelDefinitionManager::getSingleton().hasDefinition(present)) {
                    //We'll automatically create a model which shows just the specified mesh.
                    auto modelDef = std::make_shared<Model::ModelDefinition>();
                    modelDef->setOrigin(present);
                    //Create a single submodel definition using the mesh
                    Model::SubModelDefinition subModelDefinition{present};
                    modelDef->addSubModelDefinition(subModelDefinition);
                    Model::ModelDefinitionManager::getSingleton().addDefinition(present, std::move(modelDef));
                }
                mCreator->mShowModelFn(present);
            }
        }
    }

}

ModelActionCreator::ModelActionCreator(Eris::Entity& entity,
                                       std::function<void(const std::string&)> showModelFn,
                                       std::function<void(const std::string&)> showPartFn)
        : mEntity(entity),
          mShowModelFn(std::move(showModelFn)),
          mShowPartFn(std::move(showPartFn)) {

}

void ModelActionCreator::createActions(EntityMapping::EntityMapping& modelMapping, EntityMapping::Cases::CaseBase* aCase, EntityMapping::Definitions::CaseDefinition& caseDefinition) {
    for (auto& actionDef : caseDefinition.getActions()) {
        if (actionDef.Type == "display-part") {
            aCase->addAction(new DisplayPartAction(this, actionDef.getValue()));
        } else if (actionDef.Type == "display-model") {
            aCase->addAction(new DisplayModelAction(this, actionDef.getValue()));
        } else if (actionDef.Type == "present") {
            aCase->addAction(new PresentAction(this));
        }
    }
}

}
}
}