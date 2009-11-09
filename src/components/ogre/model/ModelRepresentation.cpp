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
#include "Action.h"
#include "ModelMount.h"
#include "ModelPartReactivatorVisitor.h"

#include "components/ogre/mapping/EmberEntityMappingManager.h"

#include "components/ogre/environment/Environment.h"
#include "components/ogre/environment/Forest.h"
#include "components/ogre/sound/SoundEntity.h"

#include "components/ogre/Convert.h"
#include "components/ogre/MousePicker.h"
#include "components/ogre/EmberEntityUserObject.h"
#include "components/ogre/MeshCollisionDetector.h"
#include "components/ogre/EmberEntity.h"
#include "components/ogre/EmberEntityFactory.h"
#include "components/ogre/WorldEmberEntity.h"
#include "components/ogre/EmberOgre.h"
#include "components/ogre/MotionManager.h"

#include "components/entitymapping/EntityMapping.h"
#include "components/entitymapping/EntityMappingManager.h"

#include <OgreException.h>

#include <OgreSceneNode.h>
#include <OgreSceneManager.h>
#include <OgreParticleSystem.h>

#include <Eris/Types.h>

namespace EmberOgre
{
namespace Model
{

std::string ModelRepresentation::sTypeName("ModelRepresentation");

const char * const ModelRepresentation::ACTION_STAND("__movement_idle");
const char * const ModelRepresentation::ACTION_RUN("__movement_run");
const char * const ModelRepresentation::ACTION_WALK("__movement_walk");
const char * const ModelRepresentation::ACTION_SWIM("__movement_swim");
const char * const ModelRepresentation::ACTION_FLOAT("__movement_float");

ModelRepresentation::ModelRepresentation(::EmberOgre::EmberEntity& entity, Model& model) :
	mEntity(entity), mModel(model), mCurrentMovementAction(0), mActiveAction(0), mSoundEntity(0), mMovementMode(MM_DEFAULT)
{
	mEntity.Acted.connect(sigc::mem_fun(*this, &ModelRepresentation::entity_Acted));
	mEntity.Changed.connect(sigc::mem_fun(*this, &ModelRepresentation::entity_Changed));

	///listen for reload or reset events from the model. This allows us to alter model definitions at run time and have the in game entities update.
	mModel.Reloaded.connect(sigc::mem_fun(*this, &ModelRepresentation::model_Reloaded));
	mModel.Resetting.connect(sigc::mem_fun(*this, &ModelRepresentation::model_Resetting));

	mModel.setQueryFlags(MousePicker::CM_ENTITY);

	initFromModel();

	/** If there's an idle animation, we'll randomize the entry value for that so we don't end up with too many similiar entities with synched animations (such as when you enter the world at origo and have 20 settlers doing the exact same motions. */
	Action* idleaction = mModel.getAction(ACTION_STAND);
	if (idleaction) {
		idleaction->getAnimations().addTime(Ogre::Math::RangeRandom(0, 15));
	}

	///start out with the default movement mode
	onMovementModeChanged(ModelRepresentation::MM_DEFAULT);

}

ModelRepresentation::~ModelRepresentation()
{
	delete mSoundEntity;

	delete mModel.getUserObject();
	mModel._getManager()->destroyMovableObject(&mModel);

	///make sure it's not in the MotionManager
	///TODO: keep a marker in the entity so we don't need to call this for all entities
	MotionManager::getSingleton().removeAnimated(mEntity.getId());

}

const std::string& ModelRepresentation::getType() const
{
	return sTypeName;
}

const std::string& ModelRepresentation::getTypeNameForClass()
{
	return sTypeName;
}

::EmberOgre::EmberEntity & ModelRepresentation::getEntity() const
{
	return mEntity;
}

Model & ModelRepresentation::getModel() const
{
	return mModel;
}

void ModelRepresentation::setModelPartShown(const std::string& partName, bool visible)
{
	if (mModel.isLoaded()) {

		if (visible) {
			mModel.showPart(partName);
		} else {
			mModel.hidePart(partName);
		}

		///if we already have set up a collision object we must reload it
		EmberEntityUserObject* userObject = static_cast<EmberEntityUserObject*> (mModel.getUserObject());
		if (userObject && userObject->getCollisionDetector()) {
			userObject->getCollisionDetector()->reload();
		}
	}
}

/**
 * Check if any sound action is defined within
 * the model
 */
bool ModelRepresentation::needSoundEntity()
{
	const ActionDefinitionsStore& store = mModel.getDefinition()->getActionDefinitions();
	ActionDefinitionsStore::const_iterator I_b = store.begin();
	ActionDefinitionsStore::const_iterator I_e = store.end();
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

void ModelRepresentation::setSounds()
{
	//	if (!mSoundEntity)
	//	{
	//		if (needSoundEntity())
	//		{
	//			mSoundEntity = new SoundEntity(*this);
	//		}
	//	}
}

void ModelRepresentation::setClientVisible(bool visible)
{
	///It appears that lights aren't disabled even when they're detached from the node tree (which will happen if the visibity is disabled as the lights are attached to the scale node), so we need to disable them ourselves.
	for (LightSet::iterator I = mModel.getLights().begin(); I != mModel.getLights().end(); ++I) {
		I->light->setVisible(visible);
	}
}

void ModelRepresentation::initFromModel()
{
	connectEntities();

	///see if we should use a rendering technique different from the default one (which is just using the Model::Model instance)
	const RenderingDefinition* renderingDef = mModel.getDefinition()->getRenderingDefinition();
	if (renderingDef && renderingDef->getScheme() == "forest") {
		Environment::Forest* forest = EmberOgre::getSingleton().getEntityFactory()->getWorld()->getEnvironment()->getForest();
		forest->addEmberEntity(this);
	}

}

Ogre::Vector3 ModelRepresentation::getScale() const
{
	if (mModel.getParentNode()) {
		return mModel.getParentNode()->_getDerivedScale();
	}
	return Ogre::Vector3::UNIT_SCALE;
}

void ModelRepresentation::connectEntities()
{
	if (getModel().getUserObject()) {
		delete getModel().getUserObject();
	}
	///we'll create an instance of ICollisionDetector and pass on the user object, which is then responsible for properly deleting it
	//		ICollisionDetector* collisionDetector = new OpcodeCollisionDetector(getModel());
	ICollisionDetector* collisionDetector = new MeshCollisionDetector(&getModel());
	EmberEntityUserObject* userObject = new EmberEntityUserObject(getEntity(), collisionDetector);
	getModel().setUserObject(userObject);

}

void ModelRepresentation::model_Reloaded()
{
	initFromModel();
	reactivatePartActions();
	///Retrigger a movement change so that animations can be stopped and started now that the model has changed.
	onMovementModeChanged(getMovementMode());
}

void ModelRepresentation::model_Resetting()
{
	if (getModel().getUserObject()) {
		delete getModel().getUserObject();
	}
	getModel().setUserObject(0);
}

void ModelRepresentation::processOutfit(const Atlas::Message::MapType & outfitMap)
{
}

void ModelRepresentation::entity_Changed(const Eris::StringSet& attributeIds)
{
	for (Eris::StringSet::const_iterator I = attributeIds.begin(); I != attributeIds.end(); ++I) {
		attrChanged(*I, mEntity.valueOfAttr(*I));
	}
}

void ModelRepresentation::attrChanged(const std::string& str, const Atlas::Message::Element& v)
{

	///check if the changed attribute should affect any particle systems
	///TODO: refactor this into a system where the Model instead keeps track of whether any particle systems are in use and if so attaches listeners.
	if (mModel.hasParticles()) {
		const ParticleSystemBindingsPtrSet& bindings = mModel.getAllParticleSystemBindings();
		for (ParticleSystemBindingsPtrSet::const_iterator I = bindings.begin(); I != bindings.end(); ++I) {
			if ((*I)->getVariableName() == str && v.isNum()) {
				(*I)->scaleValue(v.asNum());
			}
		}
	}

}

void ModelRepresentation::onMovementModeChanged(MovementMode newMode)
{
	/*	if (newMode != mMovementMode)
	 {*/
	const char * actionName;
	if (newMode == ModelRepresentation::MM_WALKING || newMode == ModelRepresentation::MM_WALKING_BACKWARDS) {
		actionName = ACTION_WALK;
	} else if (newMode == ModelRepresentation::MM_RUNNING) {
		actionName = ACTION_RUN;
	} else if (newMode == ModelRepresentation::MM_SWIMMING) {
		actionName = ACTION_SWIM;
	} else {
		actionName = ACTION_STAND;
	}

	/// Lets inform the sound entity of our movement change.
	///TODO: should this really be here, and not in the sound entity? this places a binding from this class to the sound entity which perhaps could be avoided
	if (mSoundEntity) {
		mSoundEntity->playMovementSound(actionName);
	}

	if (!mCurrentMovementAction || mCurrentMovementAction->getName() != actionName) {

		///first disable the current action
		if (mCurrentMovementAction) {
			mCurrentMovementAction->getAnimations().reset();
		}

		///also abort any current active action in favour of the movement action; this needs to be replaced with a better system where we can blend different animations together
		if (mActiveAction) {
			mActiveAction->getAnimations().reset();
			mActiveAction = 0;
		}

		Action* newAction = mModel.getAction(actionName);
		mCurrentMovementAction = newAction;
		if (newAction) {
			MotionManager::getSingleton().addAnimated(mEntity.getId(), this);
			//				mCurrentMovementAction->getAnimations()->setEnabled(true);

		} else {
			MotionManager::getSingleton().removeAnimated(mEntity.getId());
		}
	}

	EventMovementModeChanged.emit(newMode);
	mMovementMode = newMode;
}

ModelRepresentation::MovementMode ModelRepresentation::getMovementMode() const
{
	return mMovementMode;
}

void ModelRepresentation::parseMovementMode(const WFMath::Vector<3>& velocity)
{
	MovementMode newMode = MM_DEFAULT;

	if (velocity != WFMath::Vector<3>::ZERO()) {
		if (velocity.isValid()) {
			//Use WFMATH_EPSILON to remove any rounding errors
			if (velocity.x() + WFMATH_EPSILON < 0.0f) {
				newMode = MM_WALKING_BACKWARDS;
			} else {
				if (velocity.mag() > 2.6) {
					newMode = MM_RUNNING;
				} else {
					newMode = MM_WALKING;
				}
			}
		}
	}
	if (newMode != mMovementMode) {
		onMovementModeChanged(newMode);
	}
}

void ModelRepresentation::setLocalVelocity(const WFMath::Vector<3>& velocity)
{
	parseMovementMode(velocity);
}

void ModelRepresentation::updateAnimation(Ogre::Real timeSlice)
{
	if (mActiveAction) {
		bool continuePlay = false;
		mActiveAction->getAnimations().addTime(timeSlice, continuePlay);
		if (!continuePlay) {
			mActiveAction->getAnimations().reset();
			mActiveAction = 0;
		}
	} else {
		if (mCurrentMovementAction) {
			bool continuePlay = false;
			///Check if we're walking backward. This is a bit of a hack (we should preferrably have a separate animation for backwards walking.
			if (getMovementMode() == MM_WALKING_BACKWARDS) {
				mCurrentMovementAction->getAnimations().addTime(-timeSlice, continuePlay);
			} else {
				mCurrentMovementAction->getAnimations().addTime(timeSlice, continuePlay);
			}
		}
	}
}

const Ogre::AxisAlignedBox& ModelRepresentation::getWorldBoundingBox(bool derive) const
{
	return getModel().getWorldBoundingBox(derive);
}

const Ogre::Sphere & ModelRepresentation::getWorldBoundingSphere(bool derive) const
{
	return getModel().getWorldBoundingSphere(derive);
}

void ModelRepresentation::entity_Acted(const Atlas::Objects::Operation::RootOperation& act)
{
	const std::list<std::string> &p = act->getParents();
	std::list<std::string>::const_iterator I = p.begin();

	if (I != p.end()) {
		const std::string& name = *I;

		// If there is a sound entity, ask it to play this action
		if (mSoundEntity) {
			mSoundEntity->playAction(name);
		}

		Action* newAction = mModel.getAction(name);

		///If there's no action found, try to see if we have a "default action" defined to play instead.
		if (!newAction) {
			newAction = mModel.getAction("default_action");
		}

		if (newAction) {
			MotionManager::getSingleton().addAnimated(mEntity.getId(), this);
			mActiveAction = newAction;
			newAction->getAnimations().reset();
			mCurrentMovementAction->getAnimations().reset();
		}
	}
}

void ModelRepresentation::setVisualize(const std::string& visualization, bool visualize)
{
	if (visualization == "CollisionObject") {
		EmberEntityUserObject* userObject = static_cast<EmberEntityUserObject*> (getModel().getUserObject());
		if (userObject && userObject->getCollisionDetector()) {
			userObject->getCollisionDetector()->setVisualize(visualize);
		}
	}
}

bool ModelRepresentation::getVisualize(const std::string& visualization) const
{
	if (visualization == "CollisionObject") {
		EmberEntityUserObject* userObject = static_cast<EmberEntityUserObject*> (getModel().getUserObject());
		if (userObject && userObject->getCollisionDetector()) {
			return userObject->getCollisionDetector()->getVisualize();
		}
	}
	return false;
}

void ModelRepresentation::reactivatePartActions()
{
	ModelPartReactivatorVisitor visitor;
	mEntity.getMapping()->getRootEntityMatch().accept(visitor);
}

}
}
