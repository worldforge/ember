//
// C++ Implementation: PolygonPointMover
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

#include "PolygonPointMover.h"
#include "PolygonPoint.h"
#include "Polygon.h"
#include "services/input/Input.h"

#include "../MathConverter.h"

namespace EmberOgre {

namespace Manipulation {

PolygonPointMover::PolygonPointMover(PolygonPoint& point, IMovementListener* listener)
: mPoint(point), mNewPoint(0), mDeleted(false), mPointAfterDeleted(0), mInitialPosition(point.getLocalPosition()), mListener(listener)
{
	Ember::Input::getSingleton().EventKeyPressed.connect(sigc::mem_fun(*this, &PolygonPointMover::input_KeyPressed));
	Ember::Input::getSingleton().EventKeyReleased.connect(sigc::mem_fun(*this, &PolygonPointMover::input_KeyReleased));
}


PolygonPointMover::~PolygonPointMover()
{
}

const WFMath::Quaternion& PolygonPointMover::getOrientation() const
{
	static WFMath::Quaternion orientation;
	return orientation;
}

const WFMath::Point<3>& PolygonPointMover::getPosition() const
{
	mPosition = Ogre2Atlas(getActivePoint()->getNode()->_getDerivedPosition());
	return mPosition;
}
void PolygonPointMover::setPosition(const WFMath::Point<3>& position)
{
	if (position.isValid()) {
		///We need to offset into local space.
		Ogre::Vector3 posOffset = Ogre::Vector3::ZERO;
		if (getActivePoint()->getNode()->getParent()) {
			posOffset = getActivePoint()->getNode()->getParent()->_getDerivedPosition();
		}
		Ogre::Vector3 newPos = Atlas2Ogre(position) - posOffset;
		newPos = getActivePoint()->getNode()->getParent()->_getDerivedOrientation().Inverse() * newPos;

		WFMath::Vector<3> translation = Ogre2Atlas_Vector3(newPos - getActivePoint()->getNode()->getPosition());
		///adjust it so that it moves according to the ground for example
		getActivePoint()->translate(WFMath::Vector<2>(translation.x(), translation.y()));
	}
}
void PolygonPointMover::move(const WFMath::Vector<3>& directionVector)
{
	if (directionVector.isValid()) {
		getActivePoint()->translate(WFMath::Vector<2>(directionVector.x(), directionVector.y()));
	}
}
void PolygonPointMover::setRotation (int axis, WFMath::CoordType angle)
{
	///not implemented yet
}

void PolygonPointMover::yaw(WFMath::CoordType angle)
{
}

void PolygonPointMover::setOrientation(const WFMath::Quaternion& rotation)
{
}

void PolygonPointMover::finalizeMovement()
{
	//TODO: clean up if the point has been deleted
	mListener->endMovement();
}

void PolygonPointMover::cancelMovement()
{
	mPoint.setLocalPosition(mInitialPosition);
	if (mNewPoint) {
		switchToExistingPointMode();
	}
	mListener->cancelMovement();
// 	mPoint.getPolygon().endMovement();
}

PolygonPoint* PolygonPointMover::getActivePoint() const
{
	if (mNewPoint) {
		return mNewPoint;
	}
	return &mPoint;
}

void PolygonPointMover::input_KeyPressed(const SDL_keysym& key, Ember::Input::InputMode mode)
{
	if (key.sym == SDLK_LCTRL || key.sym == SDLK_RCTRL) {
		if (!mNewPoint) {
			switchToNewPointMode();
		}
	} else if (key.sym == SDLK_LALT || key.sym == SDLK_RALT) {
		switchToDeleteMode();
	}
}

void PolygonPointMover::input_KeyReleased(const SDL_keysym& key, Ember::Input::InputMode mode)
{
	if (key.sym == SDLK_LCTRL || key.sym == SDLK_RCTRL) {
		if (mNewPoint) {
			switchToExistingPointMode();
		}
	} else if (key.sym == SDLK_LALT || key.sym == SDLK_RALT) {
		switchToExistingPointMode();
	}
}

void PolygonPointMover::switchToNewPointMode()
{
	if (!mDeleted) {
		if (!mNewPoint) {
			///Get the two nearest points and position the new point to the one's that's closest
			PolygonPoint* point1 = mPoint.getPolygon().getPointAfter(mPoint);
			PolygonPoint* point2 = mPoint.getPolygon().getPointBefore(mPoint);
			if (point1 && point2) {
				float initialDistance1 = WFMath::Distance(mInitialPosition, point1->getLocalPosition());
				float initialDistance2 = WFMath::Distance(mInitialPosition, point2->getLocalPosition());
				float currentDistance1 = WFMath::Distance(mPoint.getLocalPosition(), point1->getLocalPosition());
				float currentDistance2 = WFMath::Distance(mPoint.getLocalPosition(), point2->getLocalPosition());
				
				float distanceDiff1 = initialDistance1 - currentDistance1;
				float distanceDiff2 = initialDistance2 - currentDistance2;
				if (distanceDiff1 < distanceDiff2) {
					mNewPoint = mPoint.getPolygon().insertPointBefore(mPoint);
				} else {
					mNewPoint = mPoint.getPolygon().insertPointBefore(*point1);
				}
				
				mNewPoint->setLocalPosition(mPoint.getLocalPosition());
				mPoint.setLocalPosition(mInitialPosition);
				mPoint.getPolygon().updateRender();
			}
		}
	}
}

void PolygonPointMover::switchToExistingPointMode()
{
	if (mDeleted) {
		if (mPointAfterDeleted) {
			mPoint.getPolygon().insertPointBefore(*mPointAfterDeleted);
		} else {
			mPoint.getPolygon().insertPoint(0, mPoint);
		}
		mPoint.setVisible(true);
		mDeleted = false;
		mPointAfterDeleted = 0;
		mPoint.getPolygon().updateRender();
	}
	if (mNewPoint) {
		if (mPoint.getPolygon().removePoint(*mNewPoint)) {
			mPoint.setLocalPosition(mNewPoint->getLocalPosition());
			delete mNewPoint;
			mNewPoint = 0;
			mPoint.getPolygon().updateRender();
		}
	}
}

void PolygonPointMover::switchToDeleteMode()
{
	mPointAfterDeleted = mPoint.getPolygon().getPointAfter(mPoint);
	mPoint.getPolygon().removePoint(mPoint);
	mPoint.setVisible(false);
	mDeleted = true;
	mPoint.getPolygon().updateRender();
}


}

}
