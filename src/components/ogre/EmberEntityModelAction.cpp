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

#include "EmberEntityPartAction.h"

#include "components/ogre/model/Model.h"
#include "components/ogre/model/ModelRepresentation.h"
#include "components/ogre/model/ModelRepresentationManager.h"
#include "components/ogre/EmberOgre.h"
#include "components/ogre/EmberEntity.h"
#include "components/entitymapping/EntityMapping.h"
#include "components/entitymapping/ChangeContext.h"
#include "components/entitymapping/IVisitor.h"
#include "framework/LoggingInstance.h"

namespace EmberOgre
{

/**
 *
 * @author Erik Hjortsberg <erik.hjortsberg@gmail.com>
 *
 * @brief Used to reactivate parts on a model which have already been shown.
 * This is done by visiting all the actions of the mapping and reactivating those part actions which already are active.
 *
 */
class ModelPartReactivatorVisitor: public Ember::EntityMapping::IVisitor
{
private:
	Ember::EntityMapping::ChangeContext mChangeContext;
public:
	virtual void visit(Ember::EntityMapping::Actions::Action& action)
	{
		EmberEntityPartAction* partAction = dynamic_cast<EmberEntityPartAction*>(&action);
		if (partAction) {
			if (partAction->isActive()) {
				partAction->activate(mChangeContext);
			}
		}
	}

	virtual void visit(Ember::EntityMapping::Matches::MatchBase& match)
	{

	}

	virtual void visit(Ember::EntityMapping::Cases::CaseBase& caseBase)
	{

	}

};

EmberEntityModelAction::EmberEntityModelAction(EmberEntity& entity, std::string modelName) :
	mEntity(entity), mModelName(modelName)
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
		model = Model::Model::createModel(EmberOgre::getSingleton().getSceneManager(), mModelName, mEntity.getId());

		///if the model definition isn't valid, use a placeholder
		if (!model->getDefinition()->isValid()) {
			S_LOG_FAILURE( "Could not find " << mModelName << ", using placeholder.");
			///add a placeholder model
			Model::ModelDefnPtr modelDef = model->getDefinition();
			modelDef->createSubModelDefinition("3d_objects/primitives/models/box.mesh")->createPartDefinition("main")->setShow(true);
			modelDef->setValid(true);
			modelDef->reloadAllInstances();
		}

		Model::ModelRepresentation* representation = new Model::ModelRepresentation(mEntity, *model);
		mEntity.setGraphicalRepresentation(representation);
		reactivatePartActions();
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
	//	mEntity.setModel("");
	//	S_LOG_VERBOSE("Hiding model " << mModelName);
}

void EmberEntityModelAction::reactivatePartActions()
{
	ModelPartReactivatorVisitor visitor;
	mEntity.getMapping()->getRootEntityMatch().accept(visitor);
}

}
