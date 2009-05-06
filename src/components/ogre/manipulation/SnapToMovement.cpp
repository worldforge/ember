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
#include "components/ogre/MathConverter.h"

#include <wfmath/rotbox.h>
#include <Eris/Entity.h>

#include <OgreSceneManager.h>
#include <memory>

namespace EmberOgre {

namespace Manipulation {

SnapToMovement::SnapToMovement(Eris::Entity& entity, Ogre::SceneNode& node, float snapThreshold, bool showDebugOverlay)
: mEntity(entity), mNode(node), mSnapThreshold(snapThreshold)
{
	if (showDebugOverlay) {
		Ogre::SceneManager* sceneMngr = mNode.getCreator();
		for (int i = 0; i < 30; ++i) {
			Ogre::SceneNode* node = sceneMngr->getRootSceneNode()->createChildSceneNode();
			Ogre::Entity* entity = sceneMngr->createEntity(node->getName() + "_entity", "3d_objects/primitives/models/sphere.mesh");
			///start out with a normal material
			entity->setMaterialName("/global/authoring/point");
			entity->setRenderingDistance(300);
	// 		entity->setQueryFlags(MousePicker::CM_UNDEFINED);
			node->setScale(0.25, 0.25, 0.25);
			node->attachObject(entity);
			node->setVisible(false);
			mDebugNodes.push_back(node);
		}
	}
}


SnapToMovement::~SnapToMovement()
{
	Ogre::SceneManager* sceneMngr = mNode.getCreator();
	for (std::vector<Ogre::SceneNode*>::iterator I = mDebugNodes.begin(); I != mDebugNodes.end(); ++I) {
		Ogre::SceneNode* node = *I;
		node->removeAndDestroyAllChildren();
		sceneMngr->destroySceneNode(node);
	}

}

bool SnapToMovement::testSnapTo(const WFMath::Point<3>& position, const WFMath::Quaternion& orientation, WFMath::Vector<3>& adjustment, EmberEntity* snappedToEntity)
{

	for (std::vector<Ogre::SceneNode*>::iterator I = mDebugNodes.begin(); I != mDebugNodes.end(); ++I) {
		Ogre::SceneNode* node = *I;
		node->setVisible(false);
		Ogre::Entity* entity = static_cast<Ogre::Entity*>(node->getAttachedObject(0));
		entity->setMaterialName("/global/authoring/point");
	}

	std::vector<Ogre::SceneNode*>::iterator nodeIterator = mDebugNodes.begin();

	std::auto_ptr<SnapPointCandidate> closestSnapping(0);

	WFMath::AxisBox<3> currentBbox = mEntity.getBBox();
	WFMath::RotBox<3> currentRotbox;
	currentRotbox.size() = currentBbox.highCorner() - currentBbox.lowCorner();
	currentRotbox.corner0() = currentBbox.lowCorner();
	currentRotbox.orientation().identity();
	currentRotbox.rotatePoint(orientation, WFMath::Point<3>(0, 0, 0));
	currentRotbox.shift(WFMath::Vector<3>(position));

	for (int j = 0; j < currentRotbox.numCorners(); ++j) {
		WFMath::Point<3> currentPoint = currentRotbox.getCorner(j);
		if (currentPoint.isValid() && nodeIterator != mDebugNodes.end()) {

			Ogre::SceneNode* node = *nodeIterator;
			node->setPosition(Atlas2Ogre(currentPoint));
			node->setVisible(true);
			nodeIterator++;
		}
	}

	///First find all entities which are close enough
	///Then try to do a snap movement based on the points of the eris bounding boxes. I.e. we only provide support for snapping one corner of a bounding box to another corner (for now).
	Ogre::SceneManager* sceneMngr = mNode.getCreator();
	if (sceneMngr) {
		WFMath::Ball<3> boundingSphere = mEntity.getBBox().boundingSphere();
		Ogre::Sphere sphere(mNode._getDerivedPosition(), boundingSphere.radius() * 2);
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
					WFMath::RotBox<3> rotbox;
					rotbox.size() = bbox.highCorner() - bbox.lowCorner();
					rotbox.corner0() = bbox.lowCorner();
					rotbox.orientation().identity();
					rotbox.rotatePoint(entity->getViewOrientation(), WFMath::Point<3>(0, 0, 0));
					rotbox.shift(WFMath::Vector<3>(entity->getViewPosition()));

					for (int i = 0; i < rotbox.numCorners(); ++i) {
						WFMath::Point<3> point = rotbox.getCorner(i);
						Ogre::SceneNode* currentNode(0);
						if (nodeIterator != mDebugNodes.end()) {
							currentNode = *nodeIterator;
							currentNode->setPosition(Atlas2Ogre(point));
							currentNode->setVisible(true);
							nodeIterator++;
						}
						point.z() = 0;
						for (int j = 0; j < currentRotbox.numCorners(); ++j) {
							WFMath::Point<3> currentPoint = currentRotbox.getCorner(j);
							currentPoint.z() = 0;
							WFMath::CoordType distance = WFMath::Distance(currentPoint, point);
							if (distance <= mSnapThreshold) {
								if (currentNode) {
									Ogre::Entity* entity = static_cast<Ogre::Entity*>(currentNode->getAttachedObject(0));
									entity->setMaterialName("/global/authoring/point/moved");
								}
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
