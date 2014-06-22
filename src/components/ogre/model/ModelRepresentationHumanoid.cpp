/*
 Copyright (C) 2009 Erik Hjortsberg <erik.hjortsberg@gmail.com>

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

#include "ModelRepresentation.h"

#include "Model.h"
#include "ModelDefinition.h"
#include "SubModel.h"
#include "ParticleSystemBinding.h"
#include "ModelAction.h"
#include "ModelMount.h"
#include "ModelPartReactivatorVisitor.h"

#include "components/ogre/mapping/EmberEntityMappingManager.h"

#include "components/ogre/sound/SoundEntity.h"

#include "components/ogre/Convert.h"
#include "components/ogre/MousePicker.h"
#include "components/ogre/EmberEntityUserObject.h"
#include "components/ogre/MeshCollisionDetector.h"
#include "domain/EmberEntity.h"
#include "components/ogre/EmberEntityFactory.h"
#include "components/ogre/EmberOgre.h"
#include "components/ogre/MotionManager.h"
#include "components/ogre/Scene.h"

#include "components/entitymapping/EntityMapping.h"
#include "components/entitymapping/EntityMappingManager.h"

#include <OgreException.h>

#include <OgreSceneNode.h>
#include <OgreSceneManager.h>
#include <OgreParticleSystem.h>

#include <Eris/Types.h>
#include <Eris/Task.h>

#include <memory>

namespace Ember
{
namespace OgreView
{
namespace Model
{

std::string ModelRepresentationHumanoid::sTypeName("ModelRepresentationHumanoid");

/*const char * const ModelRepresentation::ACTION_STAND("idle");
const char * const ModelRepresentation::ACTION_RUN("run");
const char * const ModelRepresentation::ACTION_WALK("walk");
const char * const ModelRepresentation::ACTION_SWIM("swim");
const char * const ModelRepresentation::ACTION_FLOAT("float");*/

ModelRepresentationHumanoid::ModelRepresentationHumanoid(EmberEntity& entity, Model& model, Scene& scene, EntityMapping::EntityMapping& mapping, SmartBodyManager *sbManager, const std::string& sbSkName) :
		mEntity(entity), mModel(model), mScene(scene), mMapping(mapping), mCurrentMovementAction(0), mActiveAction(0), mTaskAction(0), mSoundEntity(0), mMovementMode(MM_DEFAULT)
{
	//Only connect if we have actions to act on
	if (!model.getDefinition()->getActionDefinitions().empty()) {
		mEntity.Acted.connect(sigc::mem_fun(*this, &ModelRepresentation::entity_Acted));
		mEntity.TaskAdded.connect(sigc::mem_fun(*this, &ModelRepresentation::entity_TaskAdded));
		mEntity.TaskRemoved.connect(sigc::mem_fun(*this, &ModelRepresentation::entity_TaskRemoved));
	}
	//Only connect if we have particles
	if (mModel.hasParticles()) {
		mEntity.Changed.connect(sigc::mem_fun(*this, &ModelRepresentation::entity_Changed));
	}

	//listen for reload or reset events from the model. This allows us to alter model definitions at run time and have the in game entities update.
	mModel.Reloaded.connect(sigc::mem_fun(*this, &ModelRepresentation::model_Reloaded));
	mModel.Resetting.connect(sigc::mem_fun(*this, &ModelRepresentation::model_Resetting));

	mModel.setQueryFlags(MousePicker::CM_ENTITY);

	//start out with the default movement mode
	onMovementModeChanged(ModelRepresentation::MM_DEFAULT);

	//Create the SmartBody character.
	mCharacter = sbManager->createCharacter(entity.getName(), mModel.getDefinition()->getName(), sbSkName);
}

ModelRepresentationHumanoid::~ModelRepresentationHumanoid()
{
	//TODO : Destroy the Sb character ?

	const RenderingDefinition* renderingDef = mModel.getDefinition()->getRenderingDefinition();
	if (renderingDef && renderingDef->getScheme() != "") {
		mScene.deregisterEntityWithTechnique(mEntity, renderingDef->getScheme());
	}

	delete mSoundEntity;

	mModel._getManager()->destroyMovableObject(&mModel);

	//make sure it's not in the MotionManager
	//TODO: keep a marker in the entity so we don't need to call this for all entities
	MotionManager::getSingleton().removeAnimated(mEntity.getId());

}

const std::string& ModelRepresentationHumanoid::getType() const
{
	return sTypeName;
}

const std::string& ModelRepresentationHumanoid::getTypeNameForClass()
{
	return sTypeName;
}


}
}
}
