//
// C++ Implementation: EntityWorldPickListener
//
// Description: 
//
//
// Author: Erik Hjortsberg <erik@katastrof.nu>, (C) 2006
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
#include "EmberOgre.h"
#include "EmberEntityUserObject.h"

#include "model/Model.h"
#include "EmberEntityFactory.h"

#include "EmberEntity.h"
#include "WorldEmberEntity.h"

#include "MousePicker.h"

namespace EmberOgre {

EntityWorldPickListenerVisualizer::EntityWorldPickListenerVisualizer(EntityWorldPickListener& pickListener)
: mEntity(0), mDebugNode(0)
{
	mDebugNode = EmberOgre::getSingleton().getSceneManager()->getRootSceneNode()->createChildSceneNode();
	try {
		Ogre::Entity* mEntity = EmberOgre::getSingleton().getSceneManager()->createEntity("pickerDebugObject", "3d_objects/primitives/models/sphere.mesh");
		///start out with a normal material
		mEntity->setMaterialName("BasePointMarkerMaterial");
		mEntity->setRenderingDistance(300);
		mEntity->setQueryFlags(MousePicker::CM_NONPICKABLE);
		mDebugNode->attachObject(mEntity);
	} catch (const std::exception& ex) {
		S_LOG_WARNING("Error when creating picking visualizer: " << ex.what());
	}
	
	pickListener.EventPickedEntity.connect(sigc::mem_fun(*this, &EntityWorldPickListenerVisualizer::picker_EventPickedEntity));
}

EntityWorldPickListenerVisualizer::~EntityWorldPickListenerVisualizer()
{
	if (mEntity) {
		EmberOgre::getSingleton().getSceneManager()->destroyEntity(mEntity);
	}
	if (mDebugNode) {
		EmberOgre::getSingleton().getSceneManager()->destroySceneNode(mDebugNode->getName());
	}
}

void EntityWorldPickListenerVisualizer::picker_EventPickedEntity(const EntityPickResult& result, const MousePickerArgs& mouseArgs)
{
	mDebugNode->setPosition(result.position);
}



EntityWorldPickListener::EntityWorldPickListener()
: VisualizePicking("visualize_picking", this, "Visualize mouse pickings.")
, mClosestPickingDistance(0)
, mFurthestPickingDistance(0)
, mVisualizer(0)
{
}


EntityWorldPickListener::~EntityWorldPickListener()
{
} 

void EntityWorldPickListener::initializePickingContext()
{
	mClosestPickingDistance = 0;
	mFurthestPickingDistance = 0;
	mResult = EntityPickResult();
	mResult.entity = 0;
	mResult.position = Ogre::Vector3::ZERO;
	mResult.distance = 0;
}

void EntityWorldPickListener::endPickingContext(const MousePickerArgs& mousePickerArgs)
{
	if (mResult.entity) {
		std::stringstream ss;
		ss << mResult.position;
		S_LOG_VERBOSE("Picked entity: " << ss.str() << " distance: " << mResult.distance);
		EventPickedEntity(mResult, mousePickerArgs);
	}
}




void EntityWorldPickListener::processPickResult(bool& continuePicking, Ogre::RaySceneQueryResultEntry& entry, Ogre::Ray& cameraRay, const MousePickerArgs& mousePickerArgs) 
{

	if (entry.worldFragment) {
		///this is terrain
		///a position of -1, -1, -1 is not valid terrain
		Ogre::SceneQuery::WorldFragment* wf = entry.worldFragment;
		static Ogre::Vector3 invalidPos(-1, -1, -1);
		if (wf->singleIntersection != invalidPos) {
			if (mFurthestPickingDistance == 0) {
				mResult.entity = EmberOgre::getSingleton().getEntityFactory()->getWorld();
				mResult.position = wf->singleIntersection;
				mResult.distance = entry.distance;
				continuePicking = false;
			} else {
				if (entry.distance < mResult.distance) {
					mResult.entity = EmberOgre::getSingleton().getEntityFactory()->getWorld();
					mResult.position = wf->singleIntersection;
					mResult.distance = entry.distance;
					continuePicking = false;
				}
			}
		}
/*		std::stringstream ss;
		ss << wf->singleIntersection;
		S_LOG_VERBOSE("Picked in terrain: " << ss.str() << " distance: " << mResult.distance);*/
	
	} else if (entry.movable) {
		Ogre::MovableObject* pickedMovable = entry.movable;
		if (pickedMovable->isVisible() && pickedMovable->getUserObject() != 0 && pickedMovable->getUserObject()->getTypeName() == "EmberEntityPickerObject") {
			EmberEntityUserObject* anUserObject = static_cast<EmberEntityUserObject*>(pickedMovable->getUserObject());
			///refit the opcode mesh to adjust for changes in the mesh (for example animations)
			anUserObject->refit();
			
			ICollisionDetector* collisionDetector = anUserObject->getCollisionDetector();
			if (collisionDetector) {
				CollisionResult collisionResult;
				collisionResult.collided = false;
				collisionDetector->testCollision(cameraRay, collisionResult);
				if (collisionResult.collided) {
					EntityPickResult result;
					result.entity = anUserObject->getEmberEntity();
					result.position = collisionResult.position;
					result.distance = collisionResult.distance;
					if (mFurthestPickingDistance == 0) {
						///test all objects that fall into this distance
						mFurthestPickingDistance = (pickedMovable->getParentSceneNode()->getWorldPosition() - cameraRay.getOrigin()).length() + pickedMovable->getBoundingRadius();
						mResult = result;
					} else {
						if ((pickedMovable->getParentSceneNode()->getWorldPosition() - cameraRay.getOrigin()).length() - pickedMovable->getBoundingRadius() > mFurthestPickingDistance) {
							continuePicking = false;		
						} else {
							if (result.distance < mResult.distance) {
								mResult = result;
							}
						}
					}
					
				}
			}
			
			
			
			
			///only do opcode detection if there's a CollisionObject
// 			for (EmberEntityUserObject::CollisionObjectStore::iterator I = collisionObjects->begin(); I != collisionObjects->end(); ++I) {
// 				OgreOpcode::ICollisionShape* collisionShape = (*I)->getShape();
// 				OgreOpcode::CollisionPair pick_result;
// 				
// 				if (collisionShape->rayCheck(OgreOpcode::COLLTYPE_QUICK,anUserObject->getModel()->_getParentNodeFullTransform(),cameraRay, 1000, pick_result)) {
// 					EntityPickResult result;
// 					result.entity = anUserObject->getEmberEntity();
// 					result.position = pick_result.contact;
// 					result.distance = pick_result.distance;
// 					
// 					std::stringstream ss;
// 					ss << result.position;
// 					S_LOG_VERBOSE("Picked entity: " << ss.str() << " distance: " << result.distance);
// 					EventPickedEntity(result, mousePickerArgs);
// 					continuePicking = false;
// 					
// 				}
// 			}
		}
	}

}

void EntityWorldPickListener::runCommand(const std::string &command, const std::string &args)
{
	if(VisualizePicking == command) {
		if (mVisualizer.get()) {
			mVisualizer.reset();
		} else {
			mVisualizer = std::auto_ptr<EntityWorldPickListenerVisualizer>(new EntityWorldPickListenerVisualizer(*this));
		}
	}
}

}
