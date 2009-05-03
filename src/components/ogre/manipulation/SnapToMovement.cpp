//
// C++ Implementation: SnapToMovement
//
// Description: 
//
//
// Author: Erik Hjortsberg <erik.hjortsberg@gmail.com>, (C) 2009
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

#include "SnapToMovement.h"
#include "components/ogre/EmberEntity.h"
#include "components/ogre/EmberEntityUserObject.h"

#include <wfmath/rotbox.h>

#include <OgreSceneManager.h>

namespace EmberOgre {

namespace Manipulation {

SnapToMovement::SnapToMovement(EmberEntity& entity, float snapThreshold)
: mEntity(entity), mSnapThreshold(snapThreshold)
{
}


SnapToMovement::~SnapToMovement()
{
}

bool SnapToMovement::testSnapTo(const WFMath::Point<3>& position, const WFMath::Quaternion& orientation, WFMath::Vector<3>& adjustment, EmberEntity* snappedToEntity)
{
	std::auto_ptr<SnapPointCandidate> closestSnapping(0);

	WFMath::AxisBox<3> currentBbox = mEntity.getBBox();
	currentBbox.shift(WFMath::Vector<3>(position));
	WFMath::RotBox<3> currentRotbox(currentBbox.lowCorner(), currentBbox.highCorner() - currentBbox.lowCorner(), orientation);
	
	
	///First find all entities which are close enough
	///Then try to do a snap movement based on the points of the eris bounding boxes. I.e. we only provide support for snapping one corner of a bounding box to another corner (for now).
	Ogre::SceneManager* sceneMngr = mEntity.getSceneNode()->getCreator();
	if (sceneMngr) {
		WFMath::Ball<3> boundingSphere = mEntity.getBBox().boundingSphere();
		Ogre::Sphere sphere(mEntity.getSceneNode()->_getDerivedPosition(), boundingSphere.radius() * 2);
		Ogre::SphereSceneQuery* query = sceneMngr->createSphereQuery(sphere);
		Ogre::SceneQueryResult& result = query->execute();
		for (Ogre::SceneQueryResultMovableList::const_iterator I = result.movables.begin(); I != result.movables.end(); ++I) {
			Ogre::MovableObject* movable = *I;
			if (movable->getUserObject() != 0 && movable->getUserObject()->getTypeName() == "EmberEntityPickerObject") {
				EmberEntityUserObject* anUserObject = static_cast<EmberEntityUserObject*>(movable->getUserObject());
				EmberEntity* entity = anUserObject->getEmberEntity();
				if (entity && entity != &mEntity && entity->hasBBox()) {
					///Ok, we have an entity which is close to our entity. Now check if any of the points of the bounding box is close.
					WFMath::AxisBox<3> bbox = entity->getBBox();
					bbox.shift(WFMath::Vector<3>(entity->getViewPosition()));
					WFMath::RotBox<3> rotbox(bbox.lowCorner(), bbox.highCorner() - bbox.lowCorner(), entity->getViewOrientation());
					
					for (int i = 0; i < rotbox.numCorners(); ++i) {
						WFMath::Point<3> point = rotbox.getCorner(i);
						point.z() = 0;
						for (int j = 0; j < currentRotbox.numCorners(); ++j) {
							WFMath::Point<3> currentPoint = currentRotbox.getCorner(i);
							currentPoint.z() = 0;
							WFMath::CoordType distance = WFMath::Distance(currentPoint, point);
							if (distance <= mSnapThreshold) {
								if (!closestSnapping.get()) {
									closestSnapping = std::auto_ptr<SnapPointCandidate>(new SnapPointCandidate());
									closestSnapping->entity = entity;
									closestSnapping->distance = distance;
									closestSnapping->adjustment = point - currentPoint;
								} else if (distance < closestSnapping->distance){
									closestSnapping->entity = entity;
									closestSnapping->distance = distance;
									closestSnapping->adjustment = point - currentPoint;
								}
							}
						}
						
					}
				}
			}
		}
		sceneMngr->destroyQuery(query);
		if (closestSnapping.get()) {
			adjustment = closestSnapping->adjustment;
			snappedToEntity = closestSnapping->entity;
			return true;
		}
	}
	return false;
}



}

}
