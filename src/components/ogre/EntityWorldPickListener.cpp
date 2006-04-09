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
#include "EntityWorldPickListener.h"
#include "EmberOgre.h"
#include "EmberEntityUserObject.h"

#include "ogreopcode/include/OgreCollisionObject.h"
#include "ogreopcode/include/OgreCollisionManager.h"
#include "ogreopcode/include/OgreCollisionShape.h"
#include "model/Model.h"
#include "EmberEntityFactory.h"

#include "EmberEntity.h"
#include "WorldEmberEntity.h"

namespace EmberOgre {

EntityWorldPickListener::EntityWorldPickListener()
{
}


EntityWorldPickListener::~EntityWorldPickListener()
{
} 

void EntityWorldPickListener::processPickResult(bool& continuePicking, Ogre::RaySceneQueryResultEntry& entry, Ogre::Ray& cameraRay, const MousePickerArgs& mousePickerArgs)
{

	if (entry.worldFragment) {
		///this is terrain
		EntityPickResult result;
		Ogre::SceneQuery::WorldFragment* wf = entry.worldFragment;
			
		result.entity = EmberOgre::getSingleton().getEntityFactory()->getWorld();
		result.position = wf->singleIntersection;
		result.distance = entry.distance; 
		std::stringstream ss;
		ss << wf->singleIntersection;
		S_LOG_VERBOSE("Picked in terrain: " << ss.str() << " distance: " << result.distance);
		EventPickedEntity(result, mousePickerArgs);
		continuePicking = false;
	
	} else if (entry.movable) {
		Ogre::MovableObject* pickedMovable = entry.movable;
		if (pickedMovable->isVisible() && pickedMovable->getUserObject() != 0 && pickedMovable->getUserObject()->getTypeName() == "EmberEntityPickerObject") {
			EmberEntityUserObject* anUserObject = static_cast<EmberEntityUserObject*>(pickedMovable->getUserObject());
			///refit the opcode mesh to adjust for changes in the mesh (for example animations)
			anUserObject->refit();
			
			EmberEntityUserObject::CollisionObjectStore* collisionObjects = anUserObject->getCollisionObjects();
			
			///only do opcode detection if there's a CollisionObject
			for (EmberEntityUserObject::CollisionObjectStore::iterator I = collisionObjects->begin(); I != collisionObjects->end(); ++I) {
				OgreOpcode::ICollisionShape* collisionShape = (*I)->getShape();
				OgreOpcode::CollisionPair pick_result;
				
				if (collisionShape->rayCheck(OgreOpcode::COLLTYPE_QUICK,anUserObject->getModel()->_getParentNodeFullTransform(),cameraRay, 1000, pick_result)) {
					EntityPickResult result;
					result.entity = anUserObject->getEmberEntity();
					result.position = pick_result.contact;
					result.distance = pick_result.distance;
					
					std::stringstream ss;
					ss << result.position;
					S_LOG_VERBOSE("Picked entity: " << ss.str() << " distance: " << result.distance);
					EventPickedEntity(result, mousePickerArgs);
					continuePicking = false;
					
				}
			}
		}
	}

}


}
