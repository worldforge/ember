//
// C++ Implementation: EmberEntityModelAction
//
// Description:
//
//
// Author: Erik Hjortsberg <erik.hjortsberg@gmail.com>, (C) 2007
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.//
//
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif
#include "EmberEntityModelAction.h"
#include "Scene.h"

#include "components/ogre/model/Model.h"
#include "components/ogre/model/ModelRepresentation.h"
#include "components/ogre/model/ModelRepresentationManager.h"
#include "components/ogre/EmberEntity.h"
#include "framework/LoggingInstance.h"
#include "components/entitymapping/ChangeContext.h"
#include "components/entitymapping/EntityMapping.h"
#include "components/entitymapping/IVisitor.h"
#include "components/entitymapping/Cases/CaseBase.h"

namespace EmberOgre
{

/**
 * @brief Reactivates any model action which has previously been activated.
 * @author Erik Hjortsberg <erik.hjortsberg@gmail.com>
 */
class ModelReactivatorVisitor: public Ember::EntityMapping::IVisitor
{
private:
	Ember::EntityMapping::ChangeContext mChangeContext;
	EmberEntityModelAction* mAction;
public:

	ModelReactivatorVisitor() :
		mAction(0)
	{

	}

	~ModelReactivatorVisitor()
	{
		if (mAction) {
			mAction->activate(mChangeContext);
		}
	}

	void visit(Ember::EntityMapping::Actions::Action& action)
	{
		EmberEntityModelAction* modelAction = dynamic_cast<EmberEntityModelAction*> (&action);
		if (modelAction) {
			if (modelAction->getCase()->getIsActive()) {
				mAction = modelAction;
			}
		}
	}

	void visit(Ember::EntityMapping::Matches::MatchBase& match)
	{

	}

	void visit(Ember::EntityMapping::Cases::CaseBase& caseBase)
	{

	}

};

EmberEntityModelAction::EmberEntityModelAction(EmberEntity& entity, std::string modelName, Scene& scene) :
	mEntity(entity), mModelName(modelName), mScene(scene)
{
}

EmberEntityModelAction::~EmberEntityModelAction()
{
}

void EmberEntityModelAction::activate(Ember::EntityMapping::ChangeContext& context)
{
	Model::Model* model = Model::ModelRepresentationManager::getSingleton().getModelForEntity(mEntity);
	if (!model || model->getDefinition()->getName() != mModelName) {
		mEntity.setGraphicalRepresentation(0);
		model = Model::Model::createModel(mScene.getSceneManager(), mModelName, mEntity.getId());

		///if the model definition isn't valid, use a placeholder
		if (!model->getDefinition()->isValid()) {
			S_LOG_FAILURE( "Could not find " << mModelName << ", using placeholder.");
			///add a placeholder model
			Model::ModelDefnPtr modelDef = model->getDefinition();
			modelDef->createSubModelDefinition("3d_objects/primitives/models/box.mesh")->createPartDefinition("main")->setShow(true);
			modelDef->setValid(true);
			modelDef->reloadAllInstances();
		}

		Model::ModelRepresentation* representation = new Model::ModelRepresentation(mEntity, *model, mScene);
		mEntity.setGraphicalRepresentation(representation);
		//		if (model->getDefinition()->isValid())
		//		{
		//			mEntity.getMapping()->getRootEntityMatch().evaluateChanges();
		//		}
	}
	//	S_LOG_VERBOSE("Showing model " << mModelName);
}

void EmberEntityModelAction::deactivate(Ember::EntityMapping::ChangeContext& context)
{
	mEntity.setGraphicalRepresentation(0);
	//As we've now deactivated our model action, removing the graphical representation, we should after the change context is complete also check if there are any other model actions which should be reactivated
	context.EventContextComplete.connect(sigc::mem_fun(*this, &EmberEntityModelAction::ChangeContext_ContextComplete));
}

void EmberEntityModelAction::ChangeContext_ContextComplete()
{
	//If the entity has no graphical representation, check if there are any existing active model actions which we should reactivate.
	if (!mEntity.getGraphicalRepresentation()) {
		if (mEntity.getMapping()) {
			ModelReactivatorVisitor visitor;
			mEntity.getMapping()->getRootEntityMatch().accept(visitor);
		}
	}
}

}
