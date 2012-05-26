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

#include <boost/shared_ptr.hpp>

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
		VisualizePicking("visualize_picking", this, "Visualize mouse pickings."), mClosestPickingDistance(0), mFurthestPickingDistance(0), mVisualizer(nullptr), mView(view), mScene(scene)
{
}

EntityWorldPickListener::~EntityWorldPickListener()
{
}

void EntityWorldPickListener::initializePickingContext(bool& willParticipate, unsigned int& queryMask, const MousePickerArgs& pickArgs)
{
	if (pickArgs.pickType == MPT_CLICK || pickArgs.pickType == MPT_HOVER || pickArgs.pickType == MPT_PRESSED) {
		willParticipate = true;

		queryMask = Ogre::SceneManager::WORLD_GEOMETRY_TYPE_MASK;
		queryMask |= MousePicker::CM_AVATAR;
		queryMask |= MousePicker::CM_ENTITY;
		queryMask |= MousePicker::CM_NATURE;

		mClosestPickingDistance = 0;
		mFurthestPickingDistance = 0;
		mResult.clear();
	}
}

void EntityWorldPickListener::endPickingContext(const MousePickerArgs& mousePickerArgs)
{
	if (mResult.size()) {
		std::stringstream ss;
		ss << mResult.begin()->position;
		S_LOG_VERBOSE("Picked " << mResult.size() << " entities at " << ss.str() << " distance: " << mResult.begin()->distance << ". Type of pick: " << mousePickerArgs.pickType);
		EventPickedEntity(mResult, mousePickerArgs);
	}
}

void EntityWorldPickListener::processPickResult(bool& continuePicking, Ogre::RaySceneQueryResultEntry& entry, Ogre::Ray& cameraRay, const MousePickerArgs& mousePickerArgs)
{

	if (entry.worldFragment) {
		//this is terrain
		//a position of -1, -1, -1 is not valid terrain
		Ogre::SceneQuery::WorldFragment* wf = entry.worldFragment;
		static Ogre::Vector3 invalidPos(-1, -1, -1);
		if (wf->singleIntersection != invalidPos) {

			if (mFurthestPickingDistance == 0 || !mResult.size()) {
				EntityPickResult result;
				result.entity = static_cast<EmberEntity*>(mView.getTopLevel());
				result.position = wf->singleIntersection;
				result.distance = entry.distance;
				result.isTransparent = false;
				mResult.push_back(result);
				continuePicking = false;
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
					continuePicking = false;
				}
			}
		}
		/*		std::stringstream ss;
		 ss << wf->singleIntersection;
		 S_LOG_VERBOSE("Picked in terrain: " << ss.str() << " distance: " << mResult.distance);*/

	} else if (entry.movable) {
		Ogre::MovableObject* pickedMovable = entry.movable;
		if (pickedMovable->isVisible() && pickedMovable->getUserAny().getType() == typeid(EmberEntityUserObject::SharedPtr)) {
			EmberEntityUserObject* anUserObject = Ogre::any_cast<EmberEntityUserObject::SharedPtr>(pickedMovable->getUserAny()).get();
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
							continuePicking = false;
							return;
						} else {
							if (mResult.size() && mResult[mResult.size() - 1].distance > collisionResult.distance) {
								//If the last result is transparent, add another result, but if it's not replace it.
								if (mResult.size() && !mResult[mResult.size() - 1].isTransparent) {
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
