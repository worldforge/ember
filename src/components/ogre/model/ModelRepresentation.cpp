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


#include "components/ogre/MotionManager.h"
#include "Model.h"
#include "ModelDefinition.h"
#include "SubModel.h"
#include "ParticleSystemBinding.h"
#include "Action.h"

#include "components/ogre/mapping/EmberEntityMappingManager.h"
#include "components/entitymapping/EntityMapping.h"
#include "components/entitymapping/EntityMappingManager.h"

#include "components/ogre/environment/Environment.h"
#include "components/ogre/environment/Forest.h"
#include "components/ogre/EmberEntityFactory.h"
#include "components/ogre/WorldEmberEntity.h"

#include "components/ogre/Convert.h"

#include "components/ogre/MousePicker.h"
#include "ModelMount.h"

#include "components/ogre/EmberEntityUserObject.h"
#include "components/ogre/OpcodeCollisionDetector.h"
#include "components/ogre/MeshCollisionDetector.h"

#include "components/ogre/sound/SoundEntity.h"

#include "components/ogre/EmberEntity.h"
#include "components/ogre/EmberOgre.h"

#include <OgreException.h>

#include <Eris/Entity.h>
#include <Eris/View.h>
#include <Eris/TypeInfo.h>

#include <OgreSceneNode.h>
#include <OgreSceneManager.h>
#include <OgreParticleSystem.h>


namespace EmberOgre {
namespace Model {

std::string ModelRepresentation::sTypeName("ModelRepresentation");

const char * const ModelRepresentation::ACTION_STAND("__movement_idle");
const char * const ModelRepresentation::ACTION_RUN("__movement_run");
const char * const ModelRepresentation::ACTION_WALK("__movement_walk");
const char * const ModelRepresentation::ACTION_SWIM("__movement_swim");
const char * const ModelRepresentation::ACTION_FLOAT("__movement_float");

ModelRepresentation::ModelRepresentation(::EmberOgre::EmberEntity& entity, Model& model)
: mEntity(entity), mModel(model), mCurrentMovementAction(0), mActiveAction(0), mModelAttachedTo(0), mModelMarkedToAttachTo(0), mModelMount(0), mSoundEntity(0)
{
	mModelMount = new ModelMount(mModel, mEntity.getSceneNode());
	mEntity.Acted.connect(sigc::mem_fun(*this, &ModelRepresentation::entity_Acted));
	mEntity.LocationChanged.connect(sigc::mem_fun(*this, &ModelRepresentation::entity_LocationChanged));
	mEntity.Changed.connect(sigc::mem_fun(*this, &ModelRepresentation::entity_Changed));
	mEntity.ChildAdded.connect(sigc::mem_fun(*this, &ModelRepresentation::entity_ChildAdded));
	mEntity.ChildRemoved.connect(sigc::mem_fun(*this, &ModelRepresentation::entity_ChildRemoved));
	mEntity.EventMovementModeChanged.connect(sigc::mem_fun(*this, &ModelRepresentation::entity_MovementModeChanged));

	///listen for reload or reset events from the model. This allows us to alter model definitions at run time and have the in game entities update.
	mModel.Reloaded.connect(sigc::mem_fun(*this, &ModelRepresentation::model_Reloaded));
	mModel.Resetting.connect(sigc::mem_fun(*this, &ModelRepresentation::model_Resetting));


	mModel.setQueryFlags(MousePicker::CM_ENTITY);

	initFromModel();

	/** If there's an idle animation, we'll randomize the entry value for that so we don't end up with too many similiar entities with synched animations (such as when you enter the world at origo and have 20 settlers doing the exact same motions. */
	Action* idleaction = mModel.getAction(ACTION_STAND);
	if (idleaction)
	{
		idleaction->getAnimations().addTime(Ogre::Math::RangeRandom(0, 15));
	}

	///NOTE: for now, add all particle systems. we will want to add some visibility flag or something in the future
	for (ParticleSystemSet::iterator I = mModel.getParticleSystems().begin(); I != mModel.getParticleSystems().end(); ++I)
	{
		(*I)->getOgreParticleSystem()->detatchFromParent();
		getScaleNode()->attachObject((*I)->getOgreParticleSystem());
	}

	for (LightSet::iterator I = mModel.getLights().begin(); I != mModel.getLights().end(); ++I)
	{
		I->light->detatchFromParent();
		Ogre::SceneNode* lightNode = getScaleNode()->createChildSceneNode();
		lightNode->attachObject(I->light);
		lightNode->setPosition(I->position);
		mLightNodes.push_back(lightNode);
	}

	///check if we should do delayed attachment
	if (mModelMarkedToAttachTo)
	{
		attachToPointOnModel(mModelMarkedToAttachTo->attachPoint, mModelMarkedToAttachTo->model, mModelMarkedToAttachTo->orientation, mModelMarkedToAttachTo->offset);
		delete mModelMarkedToAttachTo;
		mModelMarkedToAttachTo = 0;
	}
}

ModelRepresentation::~ModelRepresentation()
{
	delete mSoundEntity;
	delete mModelAttachedTo;
	delete mModelMarkedToAttachTo;

	///When the modelmount is deleted the scale node will also be destroyed.
	///Note that there's no need to destroy the light nodes since they are attached to the scale node, which is deleted (along with its children) when the model mount is destroyed.
	delete mModelMount;

	delete mModel.getUserObject();
	mModel._getManager()->destroyMovableObject(&mModel);


	///make sure it's not in the MotionManager
	///TODO: keep a marker in the entity so we don't need to call this for all entities
	MotionManager::getSingleton().removeAnimatedEntity(mEntity.getId());

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

EmberEntity* ModelRepresentation::getEntityAttachedToPoint(const std::string& attachPoint)
{
	///first check with the attach points
	const std::string* entityId(0);
	for (AttachedEntitiesStore::const_iterator I = mAttachedEntities.begin(); I != mAttachedEntities.end(); ++I)
	{
		if (I->second == attachPoint)
		{
			entityId = &I->first;
			break;
		}
	}

	if (entityId)
	{
		///then get the entity from the world
		EmberEntity* entity = EmberOgre::getSingleton().getEmberEntity(*entityId);
		return entity;
	}
	return 0;
}


void ModelRepresentation::setModelPartShown(const std::string& partName, bool visible)
{

	if (visible) {
		mModel.showPart(partName);
	} else {
		mModel.hidePart(partName);
	}

	///if we already have set up a collision object we must reload it
	EmberEntityUserObject* userObject = static_cast<EmberEntityUserObject*> (mModel.getUserObject());
	if (userObject && userObject->getCollisionDetector())
	{
		userObject->getCollisionDetector()->reload();
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
	for (; I_b != I_e; ++I_b)
	{
		// Setup All Sound Actions
		SoundDefinitionsStore::const_iterator I_sounds = (*I_b)->getSoundDefinitions().begin();
		SoundDefinitionsStore::const_iterator I_sounds_end = (*I_b)->getSoundDefinitions().end();
		for (; I_sounds != I_sounds_end; ++I_sounds)
		{
			SoundDefinition* sound = (*I_sounds);

			// Once we find a single reference
			// we have an entity to allocate
			if (sound)
			{
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
	for (LightSet::iterator I = mModel.getLights().begin(); I != mModel.getLights().end(); ++I)
	{
		I->light->setVisible(visible);
	}
}

void ModelRepresentation::initFromModel()
{
	scaleNode();

	connectEntities();

	///see if we should use a rendering technique different from the default one (which is just using the Model::Model instance)
	const RenderingDefinition* renderingDef = mModel.getDefinition()->getRenderingDefinition();
	if (renderingDef && renderingDef->getScheme() == "forest")
	{
		Environment::Forest* forest = EmberOgre::getSingleton().getEntityFactory()->getWorld()->getEnvironment()->getForest();
		forest->addEmberEntity(this);
	}

}

void ModelRepresentation::connectEntities()
{
	if (getModel().getUserObject())
	{
		delete getModel().getUserObject();
	}
	///we'll create an instance of ICollisionDetector and pass on the user object, which is then responsible for properly deleting it
	//		ICollisionDetector* collisionDetector = new OpcodeCollisionDetector(getModel());
	ICollisionDetector* collisionDetector = new MeshCollisionDetector(&getModel());
	EmberEntityUserObject* userObject = new EmberEntityUserObject(&getEntity(), &getModel(), collisionDetector);
	getModel().setUserObject(userObject);

}

Ogre::SceneNode* ModelRepresentation::getScaleNode() const
{
	if (mModelMount)
	{
		return mModelMount->getScaleNode();
	}
	return 0;
}

void ModelRepresentation::attachToPointOnModel(const std::string& point, Model::Model* model, Ogre::Quaternion orientation, Ogre::Vector3 offset)
{
//	///if we're not initialized, delay attachment until after init
//	if (!isInitialized())
//	{
//		delete mModelMarkedToAttachTo;
//		mModelMarkedToAttachTo = new ModelAttachment();
//		mModelMarkedToAttachTo->model = model;
//		mModelMarkedToAttachTo->attachPoint = point;
//		mModelMarkedToAttachTo->orientation = orientation;
//		mModelMarkedToAttachTo->offset = offset;
//	}
//	else
//	{
//		if (model->hasAttachPoint(point) && model->getSkeleton())
//		{
//			getModel().detatchFromParent();
//			getModel().setVisible(true);
//			model->attachObjectToAttachPoint(point, getModel(), getScaleNode()->getScale(), getModel().getDefinition()->getRotation() * orientation, offset);
//			//Now also attach all lights and particles
//			Model::LightSet::const_iterator lightI = getModel().getLights().begin();
//			Model::LightSet::const_iterator lightI_end = getModel().getLights().end();
//			for (; lightI != lightI_end; ++lightI)
//			{
//				Ogre::Light* light = lightI->light;
//				if (light)
//				{
//					light->detatchFromParent();
//					model->attachObjectToAttachPoint(point, light, getScaleNode()->getScale(), getModel().getDefinition()->getRotation() * orientation, offset);
//				}
//			}
//
//			Model::ParticleSystemSet::const_iterator particleI = getModel().getParticleSystems().begin();
//			Model::ParticleSystemSet::const_iterator particleI_end = getModel().getParticleSystems().end();
//			for (; particleI != particleI_end; ++particleI)
//			{
//				Model::ParticleSystem* system = *particleI;
//				if (system && system->getOgreParticleSystem())
//				{
//					system->getOgreParticleSystem()->detatchFromParent();
//					model->attachObjectToAttachPoint(point, system->getOgreParticleSystem(), getScaleNode()->getScale(), getModel().getDefinition()->getRotation() * orientation, offset);
//				}
//			}
//			mModelAttachedTo = new ModelAttachment();
//			mModelAttachedTo->model = model;
//			mModelAttachedTo->attachPoint = point;
//			mModelAttachedTo->offset = offset;
//			mModelAttachedTo->orientation = getModel().getDefinition()->getRotation() * orientation;
//		}
//		for (unsigned int i = 0; i < numContained(); ++i)
//		{
//			EmberEntity* entity = static_cast<EmberEntity*>(getContained(i));
//			entity->attachToPointOnModel(point, model, getScaleNode()->getOrientation() * orientation* Convert::toOgre(entity->getOrientation()), getScaleNode()->getPosition() + offset + Convert::toOgre(entity->getPosition()));
//		}
//
//	}
}

void ModelRepresentation::detachFromModel()
{
//	if (mModelAttachedTo)
//	{
//		Model::Model* model = mModelAttachedTo->model;
//		model->detachObjectFromBone(getModel().getName());
//		getScaleNode()->attachObject(getModel());
//		for (Model::LightSet::const_iterator lightI = getModel().getLights().begin(); lightI != getModel().getLights().end(); ++lightI)
//		{
//			Ogre::Light* light = lightI->light;
//			if (light)
//			{
//				model->detachObjectFromBone(light->getName());
//				getScaleNode()->attachObject(light);
//			}
//		}
//
//		for (Model::ParticleSystemSet::const_iterator particleI = getModel().getParticleSystems().begin(); particleI != getModel().getParticleSystems().end(); ++particleI)
//		{
//			Model::ParticleSystem* system = *particleI;
//			if (system && system->getOgreParticleSystem())
//			{
//				model->detachObjectFromBone(system->getOgreParticleSystem()->getName());
//				getScaleNode()->attachObject(system->getOgreParticleSystem());
//			}
//		}
//
//		checkClientVisibility(isVisible());
//		delete mModelAttachedTo;
//		mModelAttachedTo = 0;
//	}
//	for (unsigned int i = 0; i < numContained(); ++i)
//	{
//		EmberEntity* entity = static_cast<EmberEntity*>(getContained(i));
//		entity->detachFromModel();
//	}
}

void ModelRepresentation::showOgreBoundingBox(bool show)
{
	getScaleNode()->showBoundingBox(show);
}

bool ModelRepresentation::getShowOgreBoundingBox() const
{
	return getScaleNode()->getShowBoundingBox();
}

void ModelRepresentation::model_Reloaded()
{
	initFromModel();
	///Retrigger a movement change so that animations can be stopped and started now that the model has changed.
	entity_MovementModeChanged(mEntity.getMovementMode());
	attachAllEntities();
}

void ModelRepresentation::model_Resetting()
{
	if (getModel().getUserObject())
	{
		delete getModel().getUserObject();
	}
	getModel().setUserObject(0);
	detachAllEntities();
}

void ModelRepresentation::processWield(const std::string& wieldName, const Atlas::Message::Element& idElement)
{
	S_LOG_VERBOSE("Set " << wieldName << " to " << idElement.asString());
	const std::string& id = idElement.asString();
	if (id.empty())
	{
		detachEntity(wieldName);
	}
	else
	{
		//detach first
		detachEntity(wieldName);
		attachEntity(wieldName, id);
	}
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
	///this is kind of a hack, but it allows characters to wield other entities in their hands
	if (str == "right_hand_wield" || str == "left_hand_wield")
	{
		processWield(str, v);
		return;
	} if (str == "bbox") {
		bboxChanged();
	}

	///check if the changed attribute should affect any particle systems
	///TODO: refactor this into a system where the Model instead keeps track of whether any particle systems are in use and if so attaches listeners.
	if (mModel.hasParticles())
	{
		const ParticleSystemBindingsPtrSet& bindings = mModel.getAllParticleSystemBindings();
		for (ParticleSystemBindingsPtrSet::const_iterator I = bindings.begin(); I != bindings.end(); ++I)
		{
			if ((*I)->getVariableName() == str && v.isNum())
			{
				(*I)->scaleValue(v.asNum());
			}
		}
	}

}

void ModelRepresentation::entity_MovementModeChanged(EmberEntity::MovementMode newMode)
{
	/*	if (newMode != mMovementMode)
	 {*/
	const char * actionName;
	if (newMode == EmberEntity::MM_WALKING)
	{
		actionName = ACTION_WALK;
	}
	else if (newMode == EmberEntity::MM_RUNNING)
	{
		actionName = ACTION_RUN;
	}
	else if (newMode == EmberEntity::MM_SWIMMING)
	{
		actionName = ACTION_SWIM;
	}
	else
	{
		actionName = ACTION_STAND;
	}

	/// Lets inform the sound entity of our movement change.
	///TODO: should this really be here, and not in the sound entity? this places a binding from this class to the sound entity which perhaps could be avoided
	if (mSoundEntity)
	{
		mSoundEntity->playMovementSound(actionName);
	}

	if (!mCurrentMovementAction || mCurrentMovementAction->getName() != actionName)
	{

		///first disable the current action
		if (mCurrentMovementAction)
		{
			mCurrentMovementAction->getAnimations().reset();
		}

		///also abort any current active action in favour of the movement action; this needs to be replaced with a better system where we can blend different animations together
		if (mActiveAction)
		{
			mActiveAction->getAnimations().reset();
			mActiveAction = 0;
		}

		Action* newAction = mModel.getAction(actionName);
		mCurrentMovementAction = newAction;
		if (newAction)
		{
			MotionManager::getSingleton().addAnimatedEntity(mEntity.getId(), this);
			//				mCurrentMovementAction->getAnimations()->setEnabled(true);

		}
		else
		{
			MotionManager::getSingleton().removeAnimatedEntity(mEntity.getId());
		}
	}
	//might set mCurrentMovementAction to 0
	// 	}

}

void ModelRepresentation::entity_ChildAdded(Eris::Entity *e)
{
//	//see if it's in our attach map
//	if (mAttachedEntities.find(e->getId()) != mAttachedEntities.end())
//	{
//		EmberEntity* emberEntity = static_cast<EmberEntity*> (e);
//		emberEntity->attachToPointOnModel(mAttachedEntities[e->getId()], getModel());
//	} else {
//		if (mModelAttachedTo)
//		{
//			EmberEntity* emberEntity = static_cast<EmberEntity*> (e);
//	//		entity->attachToPointOnModel(point, model, getScaleNode()->getOrientation() * orientation* Convert::toOgre(entity->getOrientation()), getScaleNode()->getPosition() + offset + Convert::toOgre(entity->getPosition()));
//			emberEntity->attachToPointOnModel(mModelAttachedTo->attachPoint, mModelAttachedTo->model, Convert::toOgre(getOrientation()) * mModelAttachedTo->orientation * Convert::toOgre(emberEntity->getOrientation()), Convert::toOgre(getPosition()) + mModelAttachedTo->offset + Convert::toOgre(emberEntity->getPosition()));
//		}
//	}


}

void ModelRepresentation::entity_ChildRemoved(Eris::Entity *e)
{
	///NOTE: we don't have to do detachment here, like we do attachment in onChildAdded, since that is done by the EmberEntity::onLocationChanged(...) method
//	Eris::Entity::onChildRemoved(e);

}

void ModelRepresentation::entity_LocationChanged(Eris::Entity *oldLocation)
{
//	bool requireRescaling = false;
//	if (mModelAttachedTo)
//	{
//		requireRescaling = true;
//	}
//	EmberEntity::onLocationChanged(oldLocation);
//	if (requireRescaling)
//	{
		scaleNode();
//	}
}

void ModelRepresentation::scaleNode()
{
	if (mModelMount)
	{
		if (mEntity.hasBBox())
		{
			mModelMount->rescale(&mEntity.getBBox());
		}
		else
		{
			mModelMount->rescale(0);
		}
	}
}

const Ogre::Vector3& ModelRepresentation::getOffsetForContainedNode(const Ogre::Vector3& position, const EmberEntity& entity)
{
//	///if the model has an offset specified, use that, else just send to the base class
//	const Ogre::Vector3& offset(getModel().getDefinition()->getContentOffset());
//	if (offset != Ogre::Vector3::ZERO)
//	{
//		return offset;
//	}
//	else
//	{
//		return EmberEntity::getOffsetForContainedNode(position, entity);
//	}

}

void ModelRepresentation::updateAnimation(Ogre::Real timeSlice)
{
	if (mActiveAction)
	{
		bool continuePlay = false;
		mActiveAction->getAnimations().addTime(timeSlice, continuePlay);
		if (!continuePlay)
		{
			mActiveAction->getAnimations().reset();
			mActiveAction = 0;
		}
	}
	else
	{
		if (mCurrentMovementAction)
		{
			bool continuePlay = false;
			///Check if we're walking backward. This is a bit of a hack (we should preferrably have a separate animation for backwards walking.
			if (mEntity.getVelocity().isValid() && static_cast<int> ((WFMath::Vector<3>(mEntity.getVelocity()).rotate((mEntity.getOrientation().inverse()))).x()) < 0)
			{
				mCurrentMovementAction->getAnimations().addTime(-timeSlice, continuePlay);
			}
			else
			{
				mCurrentMovementAction->getAnimations().addTime(timeSlice, continuePlay);
			}
		}
	}
}

void ModelRepresentation::detachEntity(const std::string & attachPoint)
{
//	///See if we've already have attached the entity that we now want to detach. Since attachments can be delayed the situation where we want to detach something which haven't yet been attached to the Model can arise, and in those instances we won't do anything more.
//	const std::string* entityId(0);
//	for (AttachedEntitiesStore::const_iterator I = mAttachedEntities.begin(); I != mAttachedEntities.end(); ++I)
//	{
//		if (I->second == attachPoint)
//		{
//			entityId = &I->first;
//			break;
//		}
//	}
//
//	if (entityId)
//	{
//		///We've found the entity within the map of attached entities, so we'll detach it (if we indeed have added it as a child too).
//		if (hasChild(*entityId))
//		{
//			///we already have the entity, do the detachment
//			EmberEntity* entity = EmberOgre::getSingleton().getEmberEntity(*entityId);
//			if (entity)
//			{
//				entity->detachFromModel();
//			}
//		}
//		mAttachedEntities.erase(*entityId);
//	}
}

void ModelRepresentation::attachEntity(const std::string & attachPoint, const std::string & entityId)
{
//	mAttachedEntities[entityId] = attachPoint;
//	///Sometimes we get the op for attaching a certain entity before we've actually recieved the entity in question from the server. In those cases we'll instead wait until onChildAdded and do the attachment there.
//	if (hasChild(entityId))
//	{
//		///we already have the entity, do the attachment now, else we will just wait for the onChildAdded event
//		EmberEntity* entity = EmberOgre::getSingleton().getEmberEntity(entityId);
//		if (entity)
//		{
//			entity->attachToPointOnModel(attachPoint, getModel());
//		}
//	}
}

void ModelRepresentation::detachAllEntities()
{
	for (AttachedEntitiesStore::const_iterator I = mAttachedEntities.begin(); I != mAttachedEntities.end(); ++I)
	{
		detachEntity(I->first);
	}
}

void ModelRepresentation::attachAllEntities()
{
//	///HACK: this should be data driven
//	if (hasAttr("right_hand_wield"))
//	{
//		const Atlas::Message::Element& idElement = valueOfAttr("right_hand_wield");
//		attachEntity("right_hand_wield", idElement.asString());
//	}
//	else if (hasAttr("left_hand_wield"))
//	{
//		const Atlas::Message::Element& idElement = valueOfAttr("left_hand_wield");
//		attachEntity("left_hand_wield", idElement.asString());
//	}
}

void ModelRepresentation::bboxChanged()
{
	///When the bounding box of the entity changes we'll also rescale the scaleNode (to which the Model is attached).
	scaleNode();
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

	if (I != p.end())
	{
		const std::string& name = *I;

		// If there is a sound entity, ask it to play this action
		if (mSoundEntity)
		{
			mSoundEntity->playAction(name);
		}

		Action* newAction = mModel.getAction(name);

		///If there's no action found, try to see if we have a "default action" defined to play instead.
		if (!newAction)
		{
			newAction = mModel.getAction("default_action");
		}

		if (newAction)
		{
			MotionManager::getSingleton().addAnimatedEntity(mEntity.getId(), this);
			mActiveAction = newAction;
			newAction->getAnimations().reset();
			mCurrentMovementAction->getAnimations().reset();
		}
	}
}

bool ModelRepresentation::allowVisibilityOfMember(EmberEntity* entity)
{
	return mModel.getDefinition()->getShowContained();
}

void ModelRepresentation::setVisualize(const std::string& visualization, bool visualize)
{
	if (visualization == "CollisionObject")
	{
		EmberEntityUserObject* userObject = static_cast<EmberEntityUserObject*> (getModel().getUserObject());
		if (userObject && userObject->getCollisionDetector())
		{
			userObject->getCollisionDetector()->setVisualize(visualize);
		}
	}
}

bool ModelRepresentation::getVisualize(const std::string& visualization) const
{
	if (visualization == "CollisionObject")
	{
		EmberEntityUserObject* userObject = static_cast<EmberEntityUserObject*> (getModel().getUserObject());
		if (userObject && userObject->getCollisionDetector())
		{
			return userObject->getCollisionDetector()->getVisualize();
		}
		return false;
	}
}



}
}
