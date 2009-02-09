//
// C++ Implementation: PolygonPoint
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

#include "PolygonPoint.h"
#include "PolygonPointMover.h"
#include "Polygon.h"
#include "IPolygonPositionProvider.h"

#include "MovementAdapter.h"

#include "../MathConverter.h"
#include "../MousePicker.h"
#include "services/logging/LoggingService.h"

#include <OgreVector3.h>

namespace EmberOgre {

namespace Manipulation {

unsigned int PolygonPoint::sPointCounter = 0;

PolygonPoint::PolygonPoint(Polygon& polygon, const WFMath::Point<2>& localPosition)
: mPolygon(polygon), mUserObject(*this), mNode(0), mMover(0)
{
	Ogre::Vector3 nodePosition = Atlas2Ogre(localPosition);
	if (polygon.getPositionProvider()) {
		nodePosition.y = polygon.getPositionProvider()->getHeightForPosition(localPosition);
	}
	mNode = mPolygon.getBaseNode()->createChildSceneNode(nodePosition);
	mNode->setScale(0.25f, 0.25f, 0.25f);
	
	std::stringstream ss;
	ss << "PolygonPoint" << sPointCounter++;
	Ogre::Entity* entity(0);
	try {
		entity = mNode->getCreator()->createEntity(ss.str(), "3d_objects/primitives/models/sphere.mesh");
		///start out with a normal material
		entity->setMaterialName("BasePointMarkerMaterial");
		entity->setRenderingDistance(300);
		entity->setQueryFlags(MousePicker::CM_UNDEFINED);
	} catch (const std::exception& ex) {
		S_LOG_FAILURE("Error when creating polygon point marker entity: " << ex.what());
		return;
	}
	
	if (!entity) {
		S_LOG_FAILURE("Unexpected error when creating polygon point marker entity.");
		return;
	}
	mNode->attachObject(entity);
}

PolygonPoint::~PolygonPoint()
{
	endMovement();
	mPolygon.getBaseNode()->removeAndDestroyChild(mNode->getName());
}

Ogre::SceneNode* PolygonPoint::getNode()
{
	return mNode;
}

Ogre::SceneNode* PolygonPoint::getNode() const
{
	return mNode;
}

void PolygonPoint::startMovement()
{
	delete mMover;
	delete mMoveAdapter;
	mMoveAdapter = new MovementAdapter();
	mMover = new PolygonPointMover(*this);
	mMoveAdapter->attachToBridge(mMover);
}

void PolygonPoint::endMovement()
{
	delete mMover;
	mMover = 0;
	delete mMoveAdapter;
	mMoveAdapter = 0;

}

void PolygonPoint::translate(const WFMath::Vector<2>& translation)
{
	Ogre::Vector2 ogrePos = Atlas2Ogre(translation);
	getNode()->translate(Ogre::Vector3(ogrePos.x, 0, ogrePos.y));
}


}

}
