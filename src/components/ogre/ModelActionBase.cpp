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

#include "ModelActionBase.h"
#include "components/ogre/model/Model.h"
#include "components/ogre/model/ModelRepresentation.h"
#include "components/ogre/model/ModelDefinitionManager.h"
#include "components/entitymapping/ChangeContext.h"
#include "components/entitymapping/EntityMapping.h"
#include "components/ogre/Scene.h"

namespace Ember {
namespace OgreView {


/**
 * @brief Reactivates any model action which has previously been activated.
 * @author Erik Ogenvik <erik@ogenvik.org>
 */
class ModelReactivatorVisitor : public EntityMapping::IVisitor {
private:
	EntityMapping::ChangeContext mChangeContext;
	ModelActionBase* mAction;
public:

	ModelReactivatorVisitor() :
			mAction(nullptr) {
	}

	~ModelReactivatorVisitor() override {
		if (mAction) {
			mAction->activate(mChangeContext);
		}
	}

	void visit(EntityMapping::Actions::Action& action) override {
		auto modelAction = dynamic_cast<ModelActionBase*> (&action);
		if (modelAction) {
			if (modelAction->getCase()->getIsActive()) {
				mAction = modelAction;
			}
		}
	}

	void visit(EntityMapping::Matches::MatchBase& match) override {

	}

	void visit(EntityMapping::Cases::CaseBase& caseBase) override {

	}

};


ModelActionBase::ModelActionBase(EmberEntity& entity, Scene& scene, EntityMapping::EntityMapping& mapping, AttachmentFunction attachmentFunction)
		: mEntity(entity),
		  mScene(scene),
		  mMapping(mapping),
		  mAttachmentFunction(std::move(attachmentFunction)) {

}

void ModelActionBase::deactivate(EntityMapping::ChangeContext& context) {
	mAttachmentFunction({});
	//As we've now deactivated our model action, removing the graphical representation, we should after the change context is complete also check if there are any other model actions which should be reactivated
	context.EventContextComplete.connect([&] {
		//If the entity has no graphical representation, check if there are any existing active model actions which we should reactivate.
		if (!mEntity.getGraphicalRepresentation()) {
			ModelReactivatorVisitor visitor;
			mMapping.getBaseCase().accept(visitor);
		}
	});
}

void ModelActionBase::showModel(const std::string& modelName) {

	Model::Model* existingModel = Model::ModelRepresentation::getModelForEntity(mEntity);
	if (!existingModel || existingModel->getDefinition()->getOrigin() != modelName) {

		Model::ModelDefinitionManager& modelDefinitionManager = Model::ModelDefinitionManager::getSingleton();
		try {
			auto definition = modelDefinitionManager.getByName(modelName);
			if (!definition) {
				S_LOG_FAILURE("Could not find model " << modelName << ", using placeholder.");
				//add a placeholder model
				definition = modelDefinitionManager.getByName("common/primitives/placeholder.modeldef");
			}
			if (definition) {
				auto model = std::make_unique<Model::Model>(mScene.getSceneManager(), definition, mEntity.getId());
				model->setVisible(mEntity.isVisible());
				model->load();

				auto newRepresentation = std::make_unique<Model::ModelRepresentation>(mEntity, std::move(model), mScene, mMapping);
				newRepresentation->initFromModel();
				mAttachmentFunction(std::move(newRepresentation));
			}
		} catch (const std::exception& ex) {
			S_LOG_FAILURE("Could not load model of type " << modelName << " from group 'ModelDefinitions'." << ex);
		}
	}
}


}
}
