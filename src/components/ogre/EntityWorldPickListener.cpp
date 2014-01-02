//
// C++ Implementation: EntityWorldPickListener
//
// Description:
//
//
// Author: Erik Hjortsberg <erik.hjortsberg@gmail.com>, (C) 2006
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
#include "EntityWorldPickListener.h"
#include "ICollisionDetector.h"
#include "EmberEntityUserObject.h"
#include "Scene.h"

#include "EmberEntity.h"

#include "MousePicker.h"

#include <OgreSceneQuery.h>
#include <OgreRay.h>
#include <OgreSceneManager.h>
#include <OgreSceneNode.h>
#include <OgreEntity.h>

#include <Eris/View.h>

namespace Ember
{
namespace OgreView
{

EntityWorldPickListenerVisualizer::EntityWorldPickListenerVisualizer(EntityWorldPickListener& pickListener, Ogre::SceneManager& sceneManager) :
		mEntity(0), mDebugNode(0)
{
	mDebugNode = sceneManager.getRootSceneNode()->createChildSceneNode();
	try {
		mEntity = sceneManager.createEntity("pickerDebugObject", "3d_objects/primitives/models/sphere.mesh");
		//start out with a normal material
		mEntity->setMaterialName("BasePointMarkerMaterial");
		mEntity->setRenderingDistance(300);
		mEntity->setQueryFlags(MousePicker::CM_NONPICKABLE);
		mDebugNode->attachObject(mEntity);
	} catch (const std::exception& ex) {
		S_LOG_WARNING("Error when creating picking visualizer." << ex);
	}

	pickListener.EventPickedEntity.connect(sigc::mem_fun(*this, &EntityWorldPickListenerVisualizer::picker_EventPickedEntity));
}

EntityWorldPickListenerVisualizer::~EntityWorldPickListenerVisualizer()
{
	if (mEntity) {
		mEntity->_getManager()->destroyEntity(mEntity);
	}
	if (mDebugNode) {
		mDebugNode->getCreator()->destroySceneNode(mDebugNode);
	}
}

void EntityWorldPickListenerVisualizer::picker_EventPickedEntity(const std::vector<EntityPickResult>& result, const MousePickerArgs& mouseArgs)
{
	mDebugNode->setPosition(result.begin()->position);
}

EntityWorldPickListener::EntityWorldPickListener(Eris::View& view, Scene& scene) :
		VisualizePicking("visualize_picking", this, "Visualize mouse pickings."), mClosestPickingDistance(0), mFurthestPickingDistance(0), mContinuePickingThisContext(true), mVisualizer(nullptr), mView(view), mScene(scene)
{
}

EntityWorldPickListener::~EntityWorldPickListener()
{
}

void EntityWorldPickListener::initializePickingContext(bool& willParticipate, unsigned int& queryMask, const MousePickerArgs& pickArgs)
{
	mResult.clear();
	mPersistedResult.clear();
	if (pickArgs.pickType == MPT_PRESS || pickArgs.pickType == MPT_HOVER) {
		willParticipate = true;

		queryMask = MousePicker::CM_AVATAR;
		queryMask |= MousePicker::CM_ENTITY;
		queryMask |= MousePicker::CM_NATURE;

		mClosestPickingDistance = 0;
		mFurthestPickingDistance = 0;
		mContinuePickingThisContext = true;
	}
}

void EntityWorldPickListener::endPickingContext(const MousePickerArgs& mousePickerArgs)
{
	if (!mResult.empty()) {
		EventPickedEntity(mResult, mousePickerArgs);

		if (mousePickerArgs.pickType != MPT_HOVER) {
			mPersistedResult.reserve(mResult.size());
			for (auto& resultEntry : mResult) {
				PersistentEntityPickResult persistedEntry;
				persistedEntry.entityRef = Eris::EntityRef(resultEntry.entity);
				persistedEntry.distance = resultEntry.distance;
				persistedEntry.isTransparent = resultEntry.isTransparent;
				persistedEntry.position = resultEntry.position;
				mPersistedResult.push_back(persistedEntry);
			}
		}
	}
}

void EntityWorldPickListener::processPickResult(bool& continuePicking, Ogre::RaySceneQueryResultEntry& entry, Ogre::Ray& cameraRay, const MousePickerArgs& mousePickerArgs)
{
	if (!mContinuePickingThisContext) {
		return;
	}

	if (entry.worldFragment) {
		//this is terrain
		//a position of -1, -1, -1 is not valid terrain
		Ogre::SceneQuery::WorldFragment* wf = entry.worldFragment;
		static const Ogre::Vector3 invalidPos(-1, -1, -1);
		if (wf->singleIntersection != invalidPos) {

			if (mFurthestPickingDistance == 0 || mResult.empty()) {
				EntityPickResult result;
				result.entity = static_cast<EmberEntity*>(mView.getTopLevel());
				result.position = wf->singleIntersection;
				result.distance = entry.distance;
				result.isTransparent = false;
				mResult.push_back(result);
				mContinuePickingThisContext = false;
			} else {
				if (entry.distance < mResult[mResult.size() - 1].distance) {
					//If the last result is transparent, add another result, but if it's not replace it.
					if (mResult.size() && !mResult[mResult.size() - 1].isTransparent) {
						mResult.pop_back();
					}
					EntityPickResult result;
					result.entity = static_cast<EmberEntity*>(mView.getTopLevel());
					result.position = wf->singleIntersection;
					result.distance = entry.distance;
					result.isTransparent = false;
					mResult.push_back(result);
					mContinuePickingThisContext = false;
				}
			}
		}
		/*		std::stringstream ss;
		 ss << wf->singleIntersection;
		 S_LOG_VERBOSE("Picked in terrain: " << ss.str() << " distance: " << mResult.distance);*/

	} else if (entry.movable) {
		Ogre::MovableObject* pickedMovable = entry.movable;
		if (pickedMovable->isVisible() && pickedMovable->getUserObjectBindings().getUserAny().getType() == typeid(EmberEntityUserObject::SharedPtr)) {
			EmberEntityUserObject* anUserObject = Ogre::any_cast<EmberEntityUserObject::SharedPtr>(pickedMovable->getUserObjectBindings().getUserAny()).get();
			//refit the opcode mesh to adjust for changes in the mesh (for example animations)
			anUserObject->refit();

			ICollisionDetector* collisionDetector = anUserObject->getCollisionDetector();
			if (collisionDetector) {
				CollisionResult collisionResult;
				collisionResult.collided = false;
				collisionResult.isTransparent = false;
				collisionDetector->testCollision(cameraRay, collisionResult);
				if (collisionResult.collided) {
					if (mFurthestPickingDistance == 0) {
						//If the current collision is transparent, also check for entities which are further away.
						if (!collisionResult.isTransparent) {
							//test all objects that fall into this distance
							mFurthestPickingDistance = (pickedMovable->getParentNode()->_getDerivedPosition() - cameraRay.getOrigin()).length() + pickedMovable->getBoundingRadius();
						}
					} else {
						if (collisionResult.distance > mFurthestPickingDistance) {
							mContinuePickingThisContext = false;
							return;
						} else {
							if (!mResult.empty() && mResult[mResult.size() - 1].distance > collisionResult.distance) {
								//If the last result is transparent, add another result, but if it's not replace it.
								if (!mResult[mResult.size() - 1].isTransparent) {
									mResult.pop_back();
								}
							} else {
								return;
							}
						}
					}
					EmberEntity& pickedEntity = anUserObject->getEmberEntity();

					std::list<EmberEntity*> entities;
					entities.push_front(&pickedEntity);
					EmberEntity* entity = pickedEntity.getEmberLocation();
					while (entity) {
						if (entity->getCompositionMode() == EmberEntity::CM_COMPOSITION) {
							entities.push_front(entity);
						} else if (entity->getCompositionMode() == EmberEntity::CM_COMPOSITION_EXCLUSIVE) {
							entities.clear();
							entities.push_front(entity);
						}
						entity = entity->getEmberLocation();
					}

					for (std::list<EmberEntity*>::const_iterator I = entities.begin(); I != entities.end(); ++I) {
						EntityPickResult result;
						result.entity = *I;
						result.position = collisionResult.position;
						result.distance = collisionResult.distance;
						result.isTransparent = collisionResult.isTransparent;
						mResult.push_back(result);
					}
				}
			}
		}
	}
}

void EntityWorldPickListener::processDelayedPick(const MousePickerArgs& mousePickerArgs)
{
	if (!mPersistedResult.empty()) {
		std::vector<EntityPickResult> resolvedResult;

		for (auto& persistedEntry : mPersistedResult) {
			if (persistedEntry.entityRef.get()) {
				EntityPickResult entry;
				entry.entity = static_cast<EmberEntity*>(persistedEntry.entityRef.get());
				entry.distance = persistedEntry.distance;
				entry.isTransparent = persistedEntry.isTransparent;
				entry.position = persistedEntry.position;
				resolvedResult.push_back(entry);
			}
		}

		if (!resolvedResult.empty()) {
			EventPickedEntity(resolvedResult, mousePickerArgs);
		}

	}
}

void EntityWorldPickListener::runCommand(const std::string &command, const std::string &args)
{
	if (VisualizePicking == command) {
		if (mVisualizer.get()) {
			mVisualizer.reset();
		} else {
			mVisualizer = std::unique_ptr < EntityWorldPickListenerVisualizer > (new EntityWorldPickListenerVisualizer(*this, mScene.getSceneManager()));
		}
	}
}

}
}
