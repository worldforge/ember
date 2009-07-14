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

#include "components/ogre/model/Model.h"
#include "components/ogre/EmberOgre.h"
#include "components/ogre/model/ModelRepresentation.h"
#include "components/ogre/model/ModelRepresentationManager.h"
#include "framework/LoggingInstance.h"

namespace EmberOgre {

EmberEntityModelAction::EmberEntityModelAction(EmberEntity& entity, std::string modelName)
: mEntity(entity), mModelName(modelName)
{
}


EmberEntityModelAction::~EmberEntityModelAction()
{
}

void EmberEntityModelAction::activate()
{
	Model::Model* model = Model::ModelRepresentationManager::getSingleton().getModelForEntity(mEntity);
	if (!model || model->getDefinition()->getName() != mModelName) {
		mEntity.setGraphicalRepresentation(0);
		model = Model::Model::createModel(EmberOgre::getSingleton().getSceneManager(), mModelName, mEntity.getId());

		///if the model definition isn't valid, use a placeholder
		if (!model->getDefinition()->isValid())
		{
			S_LOG_FAILURE( "Could not find " << mModelName << ", using placeholder.");
			///add a placeholder model
			Model::ModelDefnPtr modelDef = model->getDefinition();
			modelDef->createSubModelDefinition("3d_objects/primitives/models/box.mesh")->createPartDefinition("main")->setShow(true);
			modelDef->setValid(true);
			modelDef->reloadAllInstances();
		}

		Model::ModelRepresentation* representation = new Model::ModelRepresentation(mEntity, *model);
		mEntity.setGraphicalRepresentation(representation);
	}
//	S_LOG_VERBOSE("Showing model " << mModelName);
}

void EmberEntityModelAction::deactivate()
{
	mEntity.setGraphicalRepresentation(0);
//	mEntity.setModel("");
//	S_LOG_VERBOSE("Hiding model " << mModelName);
}


}
