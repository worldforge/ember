/*
 Copyright (C) 2009 Erik Ogenvik <erik@ogenvik.org>

 This program is free software; you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation; either version 2 of the License, or
 (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program; if not, write to the Free Software Foundation,
 Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#include "ModelRepresentation.h"

#include "Model.h"
#include "SubModel.h"
#include "ParticleSystemBinding.h"
#include "ModelMount.h"
#include "ModelPartReactivatorVisitor.h"

#include "components/ogre/mapping/EmberEntityMappingManager.h"

#include "components/ogre/sound/SoundEntity.h"

#include "components/ogre/MousePicker.h"
#include "components/ogre/EmberEntityUserObject.h"
#include "components/ogre/MeshCollisionDetector.h"
#include "components/ogre/EmberEntityFactory.h"
#include "components/ogre/EmberOgre.h"
#include "components/ogre/MotionManager.h"
#include "components/ogre/Scene.h"

#include <OgreSceneNode.h>
#include <OgreSceneManager.h>
#include <OgreParticleSystem.h>

#include <Eris/Task.h>

namespace Ember {
namespace OgreView {
namespace Model {

std::string ModelRepresentation::sTypeName("ModelRepresentation");

const char* const ModelRepresentation::ACTION_STAND("idle");

const char* const ModelRepresentation::ACTION_RUN("run");
const char* const ModelRepresentation::ACTION_RUN_RIGHT("run_right");
const char* const ModelRepresentation::ACTION_RUN_LEFT("run_left");
const char* const ModelRepresentation::ACTION_RUN_BACKWARDS("run_backwards");

const char* const ModelRepresentation::ACTION_WALK("walk");
const char* const ModelRepresentation::ACTION_WALK_RIGHT("walk_right");
const char* const ModelRepresentation::ACTION_WALK_LEFT("walk_left");
const char* const ModelRepresentation::ACTION_WALK_BACKWARDS("walk_backwards");

const char* const ModelRepresentation::ACTION_SWIM("swim");
const char* const ModelRepresentation::ACTION_FLOAT("float");

ModelRepresentation::ModelRepresentation(EmberEntity& entity, Model* model, Scene& scene, EntityMapping::EntityMapping& mapping) :
		mEntity(entity),
		mModel(model),
		mScene(scene),
		mMapping(mapping),
		mCurrentMovementAction(nullptr),
		mActiveAction(nullptr),
		mTaskAction(nullptr),
		mSoundEntity(nullptr),
		mUserObject(std::make_shared<EmberEntityUserObject>(entity, new MeshCollisionDetector(model))) {
	//Only connect if we have actions to act on
	if (!model->getDefinition()->getActionDefinitions().empty()) {
		mEntity.Acted.connect(sigc::mem_fun(*this, &ModelRepresentation::entity_Acted));
		mEntity.TaskAdded.connect(sigc::mem_fun(*this, &ModelRepresentation::entity_TaskAdded));
		mEntity.TaskRemoved.connect(sigc::mem_fun(*this, &ModelRepresentation::entity_TaskRemoved));
	}
	//Only connect if we have particles
	if (mModel->hasParticles()) {
		mEntity.Changed.connect(sigc::mem_fun(*this, &ModelRepresentation::entity_Changed));
	}

//listen for reload or reset events from the model. This allows us to alter model definitions at run time and have the in game entities update.
	mModel->Reloaded.connect(sigc::mem_fun(*this, &ModelRepresentation::model_Reloaded));
	mModel->Resetting.connect(sigc::mem_fun(*this, &ModelRepresentation::model_Resetting));

	mModel->setQueryFlags(MousePicker::CM_ENTITY);

	parseMovementMode(mEntity.getPredictedVelocity());

	mModel->setUserObject(mUserObject);

	//start out with the default movement mode
	//onMovementModeChanged(ModelRepresentation::MM_DEFAULT);

}

ModelRepresentation::~ModelRepresentation() {

	const RenderingDefinition* renderingDef = mModel->getDefinition()->getRenderingDefinition();
	if (renderingDef && !renderingDef->getScheme().empty()) {
		mScene.deregisterEntityWithTechnique(mEntity, renderingDef->getScheme());
	}

	delete mSoundEntity;
	delete mModel;
	//mModel->_getManager()->destroyMovableObject(&mModel);

	//make sure it's not in the MotionManager
	//TODO: keep a marker in the entity so we don't need to call this for all entities
	MotionManager::getSingleton().removeAnimated(mEntity.getId());

}

const std::string& ModelRepresentation::getType() const {
	return sTypeName;
}

const std::string& ModelRepresentation::getTypeNameForClass() {
	return sTypeName;
}

EmberEntity& ModelRepresentation::getEntity() const {
	return mEntity;
}

Model& ModelRepresentation::getModel() const {
	return *mModel;
}

void ModelRepresentation::setModelPartShown(const std::string& partName, bool visible) {
	if (mModel->isLoaded()) {

		if (visible) {
			mModel->showPart(partName);
		} else {
			mModel->hidePart(partName);
		}

		mUserObject->getCollisionDetector()->reload();
	}
}

/**
 * Check if any sound action is defined within
 * the model
 */
bool ModelRepresentation::needSoundEntity() {
	const ActionDefinitionsStore& store = mModel->getDefinition()->getActionDefinitions();
	auto I_b = store.begin();
	auto I_e = store.end();
	for (; I_b != I_e; ++I_b) {
		// Setup All Sound Actions
		SoundDefinitionsStore::const_iterator I_sounds = (*I_b)->getSoundDefinitions().begin();
		SoundDefinitionsStore::const_iterator I_sounds_end = (*I_b)->getSoundDefinitions().end();
		for (; I_sounds != I_sounds_end; ++I_sounds) {
			SoundDefinition* sound = (*I_sounds);

			// Once we find a single reference
			// we have an entity to allocate
			if (sound) {
				return true;
			}
		}
	}

	return false;
}

void ModelRepresentation::setSounds() {
	//	if (!mSoundEntity)
	//	{
	//		if (needSoundEntity())
	//		{
	//			mSoundEntity = new SoundEntity(*this);
	//		}
	//	}
}

void ModelRepresentation::setClientVisible(bool visible) {
	//It appears that lights aren't disabled even when they're detached from the node tree (which will happen if the visibity is disabled as the lights are attached to the scale node), so we need to disable them ourselves.
	for (auto& I : mModel->getLights()) {
		I.light->setVisible(visible);
	}
}

void ModelRepresentation::initFromModel() {
	connectEntities();

	//see if we should use a rendering technique different from the default one (which is just using the Model::Model instance)
	const RenderingDefinition* renderingDef = mModel->getDefinition()->getRenderingDefinition();
	if (renderingDef && !renderingDef->getScheme().empty() && mModel->isLoaded()) {
		mScene.registerEntityWithTechnique(mEntity, renderingDef->getScheme());
//		Environment::Forest* forest = EmberOgre::getSingleton().getEntityFactory()->getWorld()->getEnvironment()->getForest();
//		forest->addEmberEntity(this);
	}

	/** If there's an idle animation, we'll randomize the entry value for that so we don't end up with too many similar entities with synchronized animations (such as when you enter the world at origo and have 20 settlers doing the exact same motions. */
	Action* idleaction = mModel->getAction(ActivationDefinition::MOVEMENT, ACTION_STAND);
	if (idleaction) {
		idleaction->getAnimations().addTime(Ogre::Math::RangeRandom(0, 15));
	}

}

Ogre::Vector3 ModelRepresentation::getScale() const {
	return mModel->getScale();
}

void ModelRepresentation::connectEntities() {
}

void ModelRepresentation::model_Reloaded() {
	initFromModel();
	reactivatePartActions();
	//Check if there's any ongoing tasks which we should create an action for.
	if (!mEntity.getTasks().empty()) {
		//select the first available task
		createActionForTask(**mEntity.getTasks().begin());
	}
	//Retrigger a movement change so that animations can be stopped and started now that the model has changed.
	parseMovementMode(mEntity.getPredictedVelocity());
}

void ModelRepresentation::model_Resetting() {
}

void ModelRepresentation::processOutfit(const Atlas::Message::MapType&) {
}

void ModelRepresentation::entity_Changed(const Eris::StringSet& attributeIds) {
	for (const auto& attributeId : attributeIds) {
		attrChanged(attributeId, mEntity.valueOfAttr(attributeId));
	}
}

void ModelRepresentation::attrChanged(const std::string& str, const Atlas::Message::Element& v) {

	//check if the changed attribute should affect any particle systems
	//TODO: refactor this into a system where the Model instead keeps track of whether any particle systems are in use and if so attaches listeners.
	if (mModel->hasParticles()) {
		const ParticleSystemBindingsPtrSet& bindings = mModel->getAllParticleSystemBindings();
		for (auto binding : bindings) {
			if (binding->getVariableName() == str && v.isNum()) {
				binding->scaleValue(static_cast<Ogre::Real>(v.asNum()));
			}
		}
	}

}

Action* ModelRepresentation::getActionForMovement(const WFMath::Vector<3>& velocity) const {
	float mag = 0;
	if (velocity.isValid()) {
		mag = velocity.mag();
	}

	if (mEntity.getPositioningMode() == EmberEntity::PositioningMode::SUBMERGED || mEntity.getPositioningMode() == EmberEntity::PositioningMode::FLOATING) {
		return getFirstAvailableAction(ActivationDefinition::MOVEMENT, {ACTION_SWIM, ACTION_WALK});
	} else {

		if (mag < 0.01f) {
			return mModel->getAction(ActivationDefinition::MOVEMENT, ACTION_STAND);
		} else {

			//The model is moving in some direction; we need to figure out both the direction, and the speed.
			//We'll split up the movement into four arcs: forwards, backwards, left and right
			//We'll use a little bit of padding, so that the side movement arcs are larger.
			bool isRunning = mag > 2.6;
			WFMath::CoordType atan = std::atan2(velocity.x(), velocity.z());

			if (atan <= 0.7 && atan >= -0.7) {
				//moving forwards
				if (isRunning) {
					return getFirstAvailableAction(ActivationDefinition::MOVEMENT, {ACTION_RUN, ACTION_WALK});
				} else {
					return getFirstAvailableAction(ActivationDefinition::MOVEMENT, {ACTION_WALK, ACTION_RUN});
				}
			} else if (atan >= 2.4 || atan <= -2.4) {
				//moving backwards
				if (isRunning) {
					return getFirstAvailableAction(ActivationDefinition::MOVEMENT, {ACTION_RUN_BACKWARDS, ACTION_WALK_BACKWARDS});
				} else {
					return getFirstAvailableAction(ActivationDefinition::MOVEMENT, {ACTION_WALK_BACKWARDS, ACTION_RUN_BACKWARDS});
				}
			} else if (atan > 0.7) {
				//moving to the left
				if (isRunning) {
					return getFirstAvailableAction(ActivationDefinition::MOVEMENT, {ACTION_RUN_LEFT, ACTION_WALK_LEFT, ACTION_RUN, ACTION_WALK});
				} else {
					return getFirstAvailableAction(ActivationDefinition::MOVEMENT, {ACTION_WALK_LEFT, ACTION_RUN_LEFT, ACTION_WALK, ACTION_RUN});
				}
			} else {
				//moving to the right
				if (isRunning) {
					return getFirstAvailableAction(ActivationDefinition::MOVEMENT, {ACTION_RUN_RIGHT, ACTION_WALK_RIGHT, ACTION_RUN, ACTION_WALK});
				} else {
					return getFirstAvailableAction(ActivationDefinition::MOVEMENT, {ACTION_WALK_RIGHT, ACTION_RUN_RIGHT, ACTION_WALK, ACTION_RUN});
				}
			}
		}
	}

}

Action* ModelRepresentation::getFirstAvailableAction(const ActivationDefinition::Type type, std::initializer_list<const char* const> actions) const {
	for (auto& actionName : actions) {
		Action* action = mModel->getAction(type, actionName);
		if (action) {
			return action;
		}
	}
	return nullptr;
}

void ModelRepresentation::parseMovementMode(const WFMath::Vector<3>& velocity) {

	Action* newAction = getActionForMovement(velocity);

	if (newAction != mCurrentMovementAction) {
		//first disable any current action
		if (mCurrentMovementAction) {
			resetAnimations();
		}

		mCurrentMovementAction = newAction;
		if (newAction) {
			MotionManager::getSingleton().addAnimated(mEntity.getId(), this);
		} else {
			MotionManager::getSingleton().removeAnimated(mEntity.getId());
		}
	}

	// Lets inform the sound entity of our movement change.
	//TODO: should this really be here, and not in the sound entity? this places a binding from this class to the sound entity which perhaps could be avoided
//	if (mSoundEntity) {
//		mSoundEntity->playMovementSound(actionName);
//	}


//	EventMovementModeChanged.emit(newMode);
//	mMovementMode = newMode;
}

void ModelRepresentation::setLocalVelocity(const WFMath::Vector<3>& velocity) {
	parseMovementMode(velocity);
}

void ModelRepresentation::updateAnimation(float timeSlice) {
	//This is a bit convoluted, but the logic is as follows:
	//If we're moving, i.e. with a non-zero velocity, we should always prefer to show the movement animation
	//If not, we should prefer to show a current action animation if available
	//If not, we should show any available task animation.
	//And if none of these applies, we should play the current movement action (which should be idle).

	const WFMath::Vector<3>& velocity = mEntity.getPredictedVelocity();
	if (mCurrentMovementAction && velocity.isValid() && velocity.mag() > 0.01f) {
		bool continuePlay = false;
		mCurrentMovementAction->getAnimations().addTime(timeSlice, continuePlay);
	} else if (mActiveAction) {
		bool continuePlay = false;
		mActiveAction->getAnimations().addTime(timeSlice, continuePlay);
		if (!continuePlay) {
			mActiveAction->getAnimations().reset();
			mActiveAction = nullptr;
		}
	} else if (mTaskAction) {
		//Ignore the "continuePlay" for tasks.
		bool continuePlay = false;
		mTaskAction->getAnimations().addTime(timeSlice, continuePlay);
	} else if (mCurrentMovementAction) {
		bool continuePlay = false;
		mCurrentMovementAction->getAnimations().addTime(timeSlice, continuePlay);
	}
}

void ModelRepresentation::resetAnimations() {
	if (mCurrentMovementAction) {
		mCurrentMovementAction->getAnimations().reset();
	}
	if (mActiveAction) {
		mActiveAction->getAnimations().reset();
	}
	if (mTaskAction) {
		mTaskAction->getAnimations().reset();
	}
}

void ModelRepresentation::entity_Acted(const Atlas::Objects::Operation::RootOperation& act)
{
	const std::string& name = act->getParent();

	// If there is a sound entity, ask it to play this action
	if (mSoundEntity) {
		mSoundEntity->playAction(name);
	}

	Action* newAction = mModel->getAction(ActivationDefinition::ACTION, name);

	//If there's no action found, try to see if we have a "default action" defined to play instead.
	if (!newAction) {
		newAction = mModel->getAction(ActivationDefinition::ACTION, "default_action");
	}

	if (newAction) {
		MotionManager::getSingleton().addAnimated(mEntity.getId(), this);
		mActiveAction = newAction;
		resetAnimations();
	}
}

void ModelRepresentation::entity_TaskAdded(Eris::Task* task) {
	createActionForTask(*task);
}

void ModelRepresentation::createActionForTask(const Eris::Task& task) {
	Action* newAction = mModel->getAction(ActivationDefinition::TASK, task.name());
	if (newAction) {
		MotionManager::getSingleton().addAnimated(mEntity.getId(), this);
		mTaskAction = newAction;
		resetAnimations();
	}
}

void ModelRepresentation::entity_TaskRemoved(Eris::Task*) {
	if (mTaskAction) {
		mTaskAction->getAnimations().reset();
		mTaskAction = nullptr;
	}
}

void ModelRepresentation::setVisualize(const std::string& visualization, bool visualize) {
	if (visualization == "CollisionObject") {
		mUserObject->getCollisionDetector()->setVisualize(visualize);
	}
}

bool ModelRepresentation::getVisualize(const std::string& visualization) const {
	if (visualization == "CollisionObject") {
		return mUserObject->getCollisionDetector()->getVisualize();
	}
	return false;
}

void ModelRepresentation::reactivatePartActions() {
	ModelPartReactivatorVisitor visitor;
	mMapping.getBaseCase().accept(visitor);
}

}
}
}
