/*
    Copyright (C) 2004  Erik Hjortsberg
    
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

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "EmberPhysicalEntity.h"


#include "framework/ConsoleBackend.h"
#include "MotionManager.h"
#include "model/Model.h"
#include "model/ModelDefinition.h"
#include "model/SubModel.h"
#include "model/ParticleSystemBinding.h"
#include "model/Action.h"

#include "model/mapping/EmberModelMappingManager.h"
#include "model/mapping/ModelMapping.h"
#include "model/mapping/ModelMappingManager.h"

#include "environment/Environment.h"
#include "environment/Forest.h"
#include "EmberEntityFactory.h"
#include "WorldEmberEntity.h"

#include "EmberEntityActionCreator.h"

#include "MousePicker.h"
#include "ModelMount.h"

#include "EmberEntityUserObject.h"
#include "OpcodeCollisionDetector.h"
#include "MeshCollisionDetector.h"

#include "sound/SoundEntity.h"

#include "EmberOgre.h"

#include <OgreException.h>

#include <Eris/Entity.h>
#include <Eris/View.h>
#include <Eris/TypeInfo.h>

namespace EmberOgre {

const char * const EmberPhysicalEntity::ACTION_STAND("__movement_idle");
const char * const EmberPhysicalEntity::ACTION_RUN("__movement_run");
const char * const EmberPhysicalEntity::ACTION_WALK("__movement_walk");
const char * const EmberPhysicalEntity::ACTION_SWIM("__movement_swim");
const char * const EmberPhysicalEntity::ACTION_FLOAT("__movement_float");



EmberPhysicalEntity::EmberPhysicalEntity(const std::string& id, Eris::TypeInfo* ty, Eris::View* vw, Ogre::SceneManager* sceneManager) : 
EmberEntity(id, ty, vw, sceneManager),
mCurrentMovementAction(0),
mActiveAction(0),
mModelAttachedTo(0), 
mModelMarkedToAttachTo(0),
mModel(0),
mModelMount(0),
mSoundEntity(0),
mModelMapping(0)
{
}

EmberPhysicalEntity::~EmberPhysicalEntity()
{
	delete mSoundEntity;
	delete mModelMapping;

	///When the modelmount is deleted the scale node will also be destroyed.
	delete mModelMount;
	
	if (mModel) {
		delete mModel->getUserObject();
		getSceneManager()->destroyMovableObject(mModel);
	}
	
	///make sure it's not in the MotionManager
	///TODO: keep a marker in the entity so we don't need to call this for all entities
	MotionManager::getSingleton().removeAnimatedEntity(this);

}

EmberEntity* EmberPhysicalEntity::getEntityAttachedToPoint(const std::string& attachPoint)
{
	///first check with the attach points
	const std::string* entityId(0);
	for(AttachedEntitiesStore::const_iterator I = mAttachedEntities.begin(); I != mAttachedEntities.end(); ++I) {
		if (I->second == attachPoint) {
			entityId = &I->first;
			break;
		}
	}	

	if (entityId) {
		///then get the entity from the world
		EmberEntity* entity = EmberOgre::getSingleton().getEmberEntity(*entityId);
		return entity;
	}
	return 0;
}


void EmberPhysicalEntity::setModel(const std::string& modelName) 
{
	
	if (mModel) {
		if (mModel->getDefinition()->getName() == modelName) {
			return;
		} else {
			///Reset the model mount to start with.
			delete mModelMount;
			mModelMount = 0;
			getSceneManager()->destroyMovableObject(mModel);
		}
	}
	mModel = Model::Model::createModel(EmberOgre::getSingleton().getSceneManager(), modelName, getId());

	///if the model definition isn't valid, use a placeholder
	if (!mModel->getDefinition()->isValid()) {
		S_LOG_FAILURE( "Could not find " << modelName << ", using placeholder.");
		///add a placeholder model
		Model::ModelDefnPtr modelDef = mModel->getDefinition();
		modelDef->createSubModelDefinition("3d_objects/primitives/models/box.mesh")->createPartDefinition("main")->setShow( true);
		modelDef->setValid( true);
		modelDef->reloadAllInstances();
	}
	
	mModelMount = new ModelMount(*mModel, getSceneNode());
}


void EmberPhysicalEntity::showModelPart(const std::string& partName) 
{
	Model::Model* model = getModel();
	if (model) {
		model->showPart(partName);
		
		///if we already have set up a collision object we must reload it
		EmberEntityUserObject* userObject = static_cast<EmberEntityUserObject*>(getModel()->getUserObject());
		if (userObject && userObject->getCollisionDetector()) {
			userObject->getCollisionDetector()->reload();
		}
	}
}

void EmberPhysicalEntity::hideModelPart(const std::string& partName) 
{
	Model::Model* model = getModel();
	if (model) {
		model->hidePart(partName);
		
		///if we already have set up a collision object we must reload it
		EmberEntityUserObject* userObject = static_cast<EmberEntityUserObject*>(getModel()->getUserObject());
		if (userObject && userObject->getCollisionDetector()) {
			userObject->getCollisionDetector()->reload();
		}
	}
}

/**
 * Check if any sound action is defined within
 * the model
 */
bool EmberPhysicalEntity::needSoundEntity()
{
	const ActionDefinitionsStore& store = mModel->getDefinition()->getActionDefinitions();
	ActionDefinitionsStore::const_iterator I_b = store.begin();
	ActionDefinitionsStore::const_iterator I_e = store.end();
	for (; I_b != I_e; ++I_b)
	{
		// Setup All Sound Actions
		SoundDefinitionsStore::const_iterator I_sounds = (*I_b)->getSoundDefinitions().begin();
		SoundDefinitionsStore::const_iterator I_sounds_end = (*I_b)->getSoundDefinitions().end();
		for (; I_sounds != I_sounds_end; ++I_sounds)
		{
			Model::SoundDefinition* sound = (*I_sounds);
			
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

void EmberPhysicalEntity::setSounds()
{
	if (!mSoundEntity)
	{
		if (needSoundEntity())
		{
			mSoundEntity = new SoundEntity(*this);
		}
	}
}

void EmberPhysicalEntity::init(const Atlas::Objects::Entity::RootEntity &ge, bool fromCreateOp)
{
	/// we need a model mapping
	createModelMapping();
	
	assert(mModelMapping);
	///calling this will result in a call to setModel(...)
	mModelMapping->initialize();
	
	///if the model mapping framework couldn't produce a model to use, we'll fall back to the placeholder
	if (!mModel) {
		S_LOG_WARNING("Entity of type " << getType()->getName() << " have no default model, using placeholder.");
		setModel("placeholder");
	}

	// Setup Sounds
	setSounds();

	///start out with the default movement mode
	onModeChanged(EmberEntity::MM_DEFAULT);
	
	EmberEntity::init(ge, fromCreateOp);
	getModel()->setQueryFlags(MousePicker::CM_ENTITY);

	initFromModel();
	
	/** If there's an idle animation, we'll randomize the entry value for that so we don't end up with too many similiar entities with synched animations (such as when you enter the world at origo and have 20 settlers doing the exact same motions. */ 
	Model::Action* idleaction = mModel->getAction(ACTION_STAND);
	if (idleaction) {
		idleaction->getAnimations().addTime(Ogre::Math::RangeRandom(0, 15));
	}
	
	
	///check if we should do delayed attachment
	if (mModelMarkedToAttachTo) {
		attachToPointOnModel(mAttachPointMarkedToAttachTo, mModelMarkedToAttachTo);
		mModelMarkedToAttachTo = 0;
		mAttachPointMarkedToAttachTo = "";
	}

	///NOTE: for now, add all particle systems. we will want to add some visibility flag or something in the future
	for (Model::ParticleSystemSet::iterator I = mModel->getParticleSystems().begin(); I != mModel->getParticleSystems().end(); ++I) 
	{
		getScaleNode()->attachObject((*I)->getOgreParticleSystem());
	}

	for (Model::LightSet::iterator I = mModel->getLights().begin(); I != mModel->getLights().end(); ++I) 
	{
		Ogre::SceneNode* lightNode = getScaleNode()->createChildSceneNode();
		lightNode->attachObject(I->light);
		lightNode->setPosition(I->position);
	}

	///listen for reload or reset events from the model. This allows us to alter model definitions at run time and have the in game entities update.
	getModel()->Reloaded.connect(sigc::mem_fun(*this, &EmberPhysicalEntity::Model_Reloaded));
	getModel()->Resetting.connect(sigc::mem_fun(*this, &EmberPhysicalEntity::Model_Resetting));
	
	

}

void EmberPhysicalEntity::initFromModel()
{
	scaleNode();
	
	connectEntities();
	
	///see if we should use a rendering technique different from the default one (which is just using the Model::Model instance)
	const Model::RenderingDefinition* renderingDef = mModel->getDefinition()->getRenderingDefinition();
	if (renderingDef && renderingDef->getScheme() == "forest" && mModel) {
		Environment::Forest* forest = EmberOgre::getSingleton().getEntityFactory()->getWorld()->getEnvironment()->getForest();
		forest->addEmberEntity(this);
	}
	
}

void EmberPhysicalEntity::createModelMapping()
{
	delete mModelMapping;
	///the creator binds the model mapping and this instance together by creating instance of EmberEntityModelAction and EmberEntityPartAction which in turn calls the setModel(..) and show/hideModelPart(...) methods.
	EmberEntityActionCreator creator(*this);
	mModelMapping = ::EmberOgre::Model::Mapping::EmberModelMappingManager::getSingleton().getManager().createMapping(this, &creator);
}

void EmberPhysicalEntity::connectEntities()
{
	if (getModel()) {
		if (getModel()->getUserObject()) {
			delete getModel()->getUserObject();
		}
		///we'll create an instance of ICollisionDetector and pass on the user object, which is then responsible for properly deleting it
//		ICollisionDetector* collisionDetector = new OpcodeCollisionDetector(getModel());
		ICollisionDetector* collisionDetector = new MeshCollisionDetector(getModel());
		EmberEntityUserObject* userObject = new EmberEntityUserObject(this, getModel(),  collisionDetector);
		getModel()->setUserObject(userObject);
	}
}

Ogre::SceneNode* EmberPhysicalEntity::getScaleNode() const
{
	if (mModelMount) {
		return mModelMount->getScaleNode();
	}
	return 0;
}

void EmberPhysicalEntity::attachToPointOnModel(const std::string& point, Model::Model* model)
{
	///if we're not initialized, delay attachment until after init
	if (!isInitialized()) {
		mModelMarkedToAttachTo = model;
		mAttachPointMarkedToAttachTo = point;
	} else {
		if (model->hasAttachPoint(point) && model->getSkeleton()) {
			getScaleNode()->detachObject(getModel());
			getModel()->setVisible(true);
			model->attachObjectToAttachPoint( point, getModel(), getScaleNode()->getScale(), getModel()->getDefinition()->getRotation());
			mModelAttachedTo = model;
		}
	}
}

void EmberPhysicalEntity::detachFromModel()
{
	if (mModelAttachedTo) {
		mModelAttachedTo->detachObjectFromBone(getModel()->getName());
		getScaleNode()->attachObject(getModel());
		checkClientVisibility(isVisible());
		mModelAttachedTo = 0;
	}
}

void EmberPhysicalEntity::showOgreBoundingBox(bool show)
{
	getScaleNode()->showBoundingBox(show);
}


bool EmberPhysicalEntity::getShowOgreBoundingBox() const
{
	return getScaleNode()->getShowBoundingBox();
}

void EmberPhysicalEntity::Model_Reloaded()
{
	initFromModel();
	attachAllEntities();
}

void EmberPhysicalEntity::Model_Resetting()
{
	if (getModel()->getUserObject()) {
		delete getModel()->getUserObject();
	}
	getModel()->setUserObject(0);
	detachAllEntities();
}

void EmberPhysicalEntity::processWield(const std::string& wieldName, const Atlas::Message::Element& idElement)
{
	S_LOG_VERBOSE("Set " << wieldName << " to " << idElement.asString());
	const std::string& id = idElement.asString();
	if (id.empty()) {
		detachEntity(wieldName);
	} else {
		//detach first
		detachEntity(wieldName);
		attachEntity(wieldName, id);
	}
}

void EmberPhysicalEntity::processOutfit(const Atlas::Message::MapType & outfitMap)
{
}


void EmberPhysicalEntity::onAttrChanged(const std::string& str, const Atlas::Message::Element& v) {
	EmberEntity::onAttrChanged(str, v);
	
	///this is kind of a hack, but it allows characters to wield other entities in their hands
	if (str == "right_hand_wield" || str == "left_hand_wield") {
		processWield(str, v);
		return;		
	}

	///check if the changed attribute should affect any particle systems
	///TODO: refactor this into a system where the Model instead keeps track of whether any particle systems are in use and if so attaches listeners.
	if (mModel->hasParticles()) {
		const Model::ParticleSystemBindingsPtrSet& bindings = mModel->getAllParticleSystemBindings();
		for (Model::ParticleSystemBindingsPtrSet::const_iterator I = bindings.begin(); I != bindings.end(); ++I) {
			if ((*I)->getVariableName() == str && v.isNum()) {
				(*I)->scaleValue(v.asNum());
			}
		}
	}


}

void EmberPhysicalEntity::onModeChanged(MovementMode newMode)
{
/*	if (newMode != mMovementMode) 
	{*/
		const char * actionName;
		if (newMode == EmberEntity::MM_WALKING) {
			actionName = ACTION_WALK;
		} else if (newMode == EmberEntity::MM_RUNNING) {
			actionName = ACTION_RUN;
		} else if (newMode == EmberEntity::MM_SWIMMING) {
			actionName = ACTION_SWIM;
		} else {
			actionName = ACTION_STAND;
		}

		/// Lets inform the sound entity of our movement change.
		///TODO: should this really be here, and not in the sound entity? this places a binding from this class to the sound entity which perhaps could be avoided
		if (mSoundEntity)
		{
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
			
			Model::Action* newAction = mModel->getAction(actionName);
			mCurrentMovementAction = newAction;
			if (newAction) {
				MotionManager::getSingleton().addAnimatedEntity(this);
//				mCurrentMovementAction->getAnimations()->setEnabled(true);
				
			} else {
				MotionManager::getSingleton().removeAnimatedEntity(this);
			}
		}
		//might set mCurrentMovementAction to 0
// 	}

	EmberEntity::onModeChanged(newMode);
}


void EmberPhysicalEntity::onChildAdded(Entity *e) 
{
	Eris::Entity::onChildAdded(e);
	//see if it's in our attach map
	if (mAttachedEntities.find(e->getId()) != mAttachedEntities.end()) {
		EmberEntity* emberEntity = static_cast<EmberEntity*>(e);
		emberEntity->attachToPointOnModel(mAttachedEntities[e->getId()], getModel());
	}
	
/*	if (hasChild(entityId)) {
	}*/
	
	
}



void EmberPhysicalEntity::onChildRemoved(Entity *e)
{
	///NOTE: we don't have to do detachment here, like we do attachment in onChildAdded, since that is done by the EmberEntity::onLocationChanged(...) method
	Eris::Entity::onChildRemoved(e);
}


void EmberPhysicalEntity::scaleNode() 
{
	if (mModelMount) {
		if (hasBBox()) {
			mModelMount->rescale(&getBBox());
		} else {
			mModelMount->rescale(0);
		}
	}
}

const Ogre::Vector3& EmberPhysicalEntity::getOffsetForContainedNode(const Ogre::Vector3& position, EmberEntity* const entity)
{
	///if the model has an offset specified, use that, else just send to the base class
	const Ogre::Vector3& offset(getModel()->getDefinition()->getContentOffset());
	if (offset != Ogre::Vector3::ZERO) {
		return offset;
	} else {
		return EmberEntity::getOffsetForContainedNode(position, entity);
	}
	
}


void EmberPhysicalEntity::updateMotion(Ogre::Real timeSlice)
{
	EmberEntity::updateMotion(timeSlice);
}


void EmberPhysicalEntity::updateAnimation(Ogre::Real timeSlice)
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
			if (static_cast<int>((WFMath::Vector<3>(getVelocity()).rotate((getOrientation().inverse()))).x()) < 0) {
				mCurrentMovementAction->getAnimations().addTime(-timeSlice, continuePlay);
			} else {
				mCurrentMovementAction->getAnimations().addTime(timeSlice, continuePlay);
			}
		}
	}
}

void EmberPhysicalEntity::setMoving(bool moving)
{
// 	MotionManager* motionManager = &MotionManager::getSingleton();
// 	if (moving) {
// 		if (getModel()->isAnimated()) {
// 			getModel()->stopAnimation("idle");
// 			getModel()->startAnimation("walk");
// 		}
// 	} else {
// 		if (getModel()->isAnimated()) {
// 			getModel()->stopAnimation("walk");
// 			getModel()->startAnimation("idle");
// 		}
// 	}
	EmberEntity::setMoving(moving);
}

void EmberPhysicalEntity::detachEntity(const std::string & attachPoint)
{
	///See if we've already have attached the entity that we now want to detach. Since attachments can be delayed the situation where we want to detach something which haven't yet been attached to the Model can arise, and in those instances we won't do anything more.
	const std::string* entityId(0);
	for(AttachedEntitiesStore::const_iterator I = mAttachedEntities.begin(); I != mAttachedEntities.end(); ++I) {
		if (I->second == attachPoint) {
			entityId = &I->first;
			break;
		}
	}
	
	if (entityId) {
		///We've found the entity within the map of attached entities, so we'll detach it (if we indeed have added it as a child too). 
		if (hasChild(*entityId)) {
			///we already have the entity, do the detachment
			EmberEntity* entity = EmberOgre::getSingleton().getEmberEntity(*entityId);
			if (entity) {
				entity->detachFromModel();
			}
		}
		mAttachedEntities.erase(*entityId);
	}
}
	
void EmberPhysicalEntity::attachEntity(const std::string & attachPoint, const std::string & entityId)
{
	mAttachedEntities[entityId] = attachPoint;
	///Sometimes we get the op for attaching a certain entity before we've actually recieved the entity in question from the server. In those cases we'll instead wait until onChildAdded and do the attachment there.
	if (hasChild(entityId)) {
		///we already have the entity, do the attachment now, else we will just wait for the onChildAdded event
		EmberEntity* entity = EmberOgre::getSingleton().getEmberEntity(entityId);
		if (entity) {
			entity->attachToPointOnModel(attachPoint, getModel());
		}
	}
}

void EmberPhysicalEntity::detachAllEntities()
{
	for(AttachedEntitiesStore::const_iterator I = mAttachedEntities.begin(); I != mAttachedEntities.end(); ++I) {
		detachEntity(I->first);
	}
}

void EmberPhysicalEntity::attachAllEntities()
{
///HACK: this should be data driven
	if (hasAttr("right_hand_wield")) {
		const Atlas::Message::Element& idElement = valueOfAttr("right_hand_wield");
		attachEntity("right_hand_wield", idElement.asString());
	} else if (hasAttr("left_hand_wield")) {
		const Atlas::Message::Element& idElement = valueOfAttr("left_hand_wield");
		attachEntity("left_hand_wield", idElement.asString());
	}
}


void EmberPhysicalEntity::onBboxChanged()
{
	///When the bounding box of the entity changes we'll also rescale the scaleNode (to which the Model is attached).
	EmberEntity::onBboxChanged();
	scaleNode();
}

const Ogre::AxisAlignedBox& EmberPhysicalEntity::getWorldBoundingBox(bool derive) const
{
	return getModel()->getWorldBoundingBox(derive);
}

const Ogre::Sphere & EmberPhysicalEntity::getWorldBoundingSphere (bool derive) const
{
	return getModel()->getWorldBoundingSphere(derive);
}

void EmberPhysicalEntity::onAction(const Atlas::Objects::Operation::RootOperation& act)
{
	const std::list<std::string> &p = act->getParents();
	std::list<std::string>::const_iterator I = p.begin();
	
	if (I != p.end()) {
		const std::string& name = *I;

		// If there is a sound entity, ask it to play this action
		if (mSoundEntity)
		{
			mSoundEntity->playAction(name);
		}
		
		Model::Action* newAction = mModel->getAction(name);
		
		///If there's no action found, try to see if we have a "default action" defined to play instead.
		if (!newAction) {
			newAction = mModel->getAction("default_action");
		}
		
		if (newAction) {
			MotionManager::getSingleton().addAnimatedEntity(this);
			mActiveAction = newAction;
			newAction->getAnimations().reset();
			mCurrentMovementAction->getAnimations().reset();
		}
	}
	EmberEntity::onAction(act);
}

bool EmberPhysicalEntity::allowVisibilityOfMember(EmberEntity* entity) {
	return mModel->getDefinition()->getShowContained();
}

void EmberPhysicalEntity::setVisualize(const std::string& visualization, bool visualize)
{
	if (visualization == "CollisionObject") {
		if (getModel()) {
			EmberEntityUserObject* userObject = static_cast<EmberEntityUserObject*>(getModel()->getUserObject());
			if (userObject && userObject->getCollisionDetector()) {
				userObject->getCollisionDetector()->setVisualize(visualize);
			}
		}
	} else {
		EmberEntity::setVisualize(visualization, visualize);
	}
}

bool EmberPhysicalEntity::getVisualize(const std::string& visualization) const
{
	if (visualization == "CollisionObject") {
		if (getModel()) {
			EmberEntityUserObject* userObject = static_cast<EmberEntityUserObject*>(getModel()->getUserObject());
			if (userObject && userObject->getCollisionDetector()) {
				return userObject->getCollisionDetector()->getVisualize();
			}
		}
		return false;
	} else {
		return EmberEntity::getVisualize(visualization);
	}
}

/*
void EmberPhysicalEntity::handleTalk(const std::string &msg)
{
	
    std::string message = "<";
    message.append(getName());
    message.append("> ");
    message.append(msg);
	std::cout << "TRACE - ENTITY SAYS: [" << message << "]\n" << std::endl;
	Ember::ConsoleBackend::getSingletonPtr()->pushMessage(message);
}
*/



/*
void EmberPhysicalEntity::setContainer(Entity *pr)
{
		
	EmberEntity* EmberEntity = dynamic_cast<EmberEntity*>(pr);
	if (EmberEntity) {
		//detach from our current object and add to the new entity
		getSceneNode()->getParent()->removeChild(getSceneNode()->getName());
		EmberEntity->getSceneNode()->addChild(getSceneNode());
				
	} else {
		//detach from our current object and add to the world
		getSceneNode()->getParent()->removeChild(getSceneNode()->getName());
		getSceneNode()->getCreator()->getRootSceneNode()->addChild(getSceneNode());
	}		
	
}
*/






}
