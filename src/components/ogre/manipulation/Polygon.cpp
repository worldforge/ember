//
// C++ Implementation: Polygon
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

#include "Polygon.h"


#include <Ogre.h>
#include <wfmath/vector.h>
#include <wfmath/polygon.h>

#include "services/logging/LoggingService.h"

#include "../MathConverter.h"
#include "../MousePicker.h"

namespace EmberOgre {

namespace Manipulation {

unsigned int PolygonRenderer::sCounter = 0;

PolygonRenderer::PolygonRenderer(Polygon& polygon)
: mPolygon(polygon), mManualObject(0)
{
	std::stringstream ss;
	ss << "PolygonRenderer" << sCounter++;
	mManualObject = static_cast<Ogre::ManualObject*>(polygon.getBaseNode()->getCreator()->createMovableObject(ss.str(), Ogre::ManualObjectFactory::FACTORY_TYPE_NAME));
	mManualObject->setDynamic(true); ///We'll be updating this a lot if the use alters the polygon
	mPolygon.getBaseNode()->attachObject(mManualObject);
	
}

PolygonRenderer::~PolygonRenderer()
{
	if (mManualObject) {
		mPolygon.getBaseNode()->removeAndDestroyChild(mManualObject->getName());
	}
}

void PolygonRenderer::update()
{
	mManualObject->clear();
	mManualObject->begin("/global/authoring/polygon/line", Ogre::RenderOperation::OT_LINE_STRIP);
	
	for (Polygon::PointStore::const_iterator I = mPolygon.getPoints().begin(); I != mPolygon.getPoints().end(); ++I) {
		const PolygonPoint* point(*I);
		mManualObject->position(point->getNode()->getPosition());
	}
	mManualObject->end();

	
}



void PolygonPointPickListener::processPickResult(bool& continuePicking, Ogre::RaySceneQueryResultEntry& entry, Ogre::Ray& cameraRay, const MousePickerArgs& mousePickerArgs)
{
	if (entry.movable) {
		Ogre::MovableObject* pickedMovable = entry.movable;
		if (pickedMovable->isVisible() && pickedMovable->getUserObject() != 0 && pickedMovable->getUserObject()->getTypeName() == PolygonPointUserObject::s_TypeName) {
			mPickedUserObject = static_cast<PolygonPointUserObject*>(pickedMovable->getUserObject());
			continuePicking = false;
		}
	}
}

void PolygonPointPickListener::initializePickingContext()
{
	mPickedUserObject = 0;
}

void PolygonPointPickListener::endPickingContext(const MousePickerArgs& mousePickerArgs)
{
	if (mPickedUserObject) {
		mPickedUserObject->getPoint().startMovement();
	}
}


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
// 	mMover = new PolygonPointMover(*this);
}

void PolygonPoint::endMovement()
{
	delete mMover;
	mMover = 0;
}

const std::string PolygonPointUserObject::s_TypeName("PolygonPointMarker");


PolygonPointUserObject::PolygonPointUserObject(PolygonPoint& point)
: mPoint(point)
{
}

PolygonPointUserObject::~PolygonPointUserObject()
{
}

PolygonPoint& PolygonPointUserObject::getPoint()
{
	return mPoint;
}

const Ogre::String & PolygonPointUserObject::getTypeName (void) const
{
	return s_TypeName;
}


Polygon::Polygon(Ogre::SceneNode* baseNode, IPolygonPositionProvider* positionProvider)
: mBaseNode(baseNode), mPositionProvider(positionProvider), mRenderer(*this)
{
}


Polygon::~Polygon()
{
	clear();
}

Ogre::SceneNode* Polygon::getBaseNode()
{
	return mBaseNode;
}

void Polygon::loadFromShape(const WFMath::Polygon<2>& shape)
{
	clear();
	for(int i = 0; i < shape.numCorners(); ++i) {
		const WFMath::Point<2>& position = shape[i];
		PolygonPoint* point = new PolygonPoint(*this, position);
		mPoints.push_back(point);
	}
	mRenderer.update();
}

void Polygon::clear()
{
	for (PointStore::iterator I = mPoints.begin(); I != mPoints.end(); ++I) {
		delete *I;
	}
	mPoints.clear();
	mRenderer.update();
}

const Polygon::PointStore& Polygon::getPoints() const
{
	return mPoints;
}


IPolygonPositionProvider* Polygon::getPositionProvider() const
{
	return mPositionProvider;
}



}

}
