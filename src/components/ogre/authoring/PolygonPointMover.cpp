//
// C++ Implementation: PolygonPointMover
//
// Description:
//
//
// Author: Erik Ogenvik <erik@ogenvik.org>, (C) 2009
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
// along with this program; if not, write to the Free Software Foundation,
// Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.//
//
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "PolygonPointMover.h"
#include "PolygonPoint.h"
#include "Polygon.h"
#include "components/ogre/EntityCollisionInfo.h"

#include "../Convert.h"
#include <OgreSceneNode.h>
#include <SDL_keyboard.h>

namespace Ember {
namespace OgreView {

namespace Authoring {

PolygonPointMover::PolygonPointMover(Polygon& polygon, PolygonPoint& point, IMovementListener* listener) :
		mPolygon(polygon),
		mPoint(point),
		mNewPoint(nullptr),
		mDeleted(false),
		mPointAfterDeleted(nullptr),
		mInitialPosition(point.getLocalPosition()),
		mListener(listener) {
	Input::getSingleton().EventKeyPressed.connect(sigc::mem_fun(*this, &PolygonPointMover::input_KeyPressed));
	Input::getSingleton().EventKeyReleased.connect(sigc::mem_fun(*this, &PolygonPointMover::input_KeyReleased));
}

PolygonPointMover::~PolygonPointMover() {
	if (mDeleted) {
		//TODO: don't do this
		delete &mPoint;
	}
}

const WFMath::Quaternion& PolygonPointMover::getOrientation() const {
	static WFMath::Quaternion orientation;
	return orientation;
}

const WFMath::Point<3>& PolygonPointMover::getPosition() const {
	mPosition = Convert::toWF<WFMath::Point<3>>(getActivePoint()->getNode()->_getDerivedPosition());
	return mPosition;
}

void PolygonPointMover::setPosition(const WFMath::Point<3>& position) {
	if (position.isValid()) {
		//We need to offset into local space.
		Ogre::Vector3 posOffset = Ogre::Vector3::ZERO;
		if (getActivePoint()->getNode()->getParent()) {
			posOffset = getActivePoint()->getNode()->getParent()->_getDerivedPosition();
		}
		Ogre::Vector3 newPos = Convert::toOgre(position) - posOffset;
		newPos = getActivePoint()->getNode()->getParent()->_getDerivedOrientation().Inverse() * newPos;

		WFMath::Vector<3> translation = Convert::toWF<WFMath::Vector<3>>(newPos - getActivePoint()->getNode()->getPosition());
		//adjust it so that it moves according to the ground for example
		getActivePoint()->translate(WFMath::Vector<2>(translation.x(), translation.z()));
		mPolygon.updateRender();
	}
}

void PolygonPointMover::move(const WFMath::Vector<3>& directionVector) {
	if (directionVector.isValid()) {
		getActivePoint()->translate(WFMath::Vector<2>(directionVector.x(), directionVector.z()));
		mPolygon.updateRender();
	}
}

void PolygonPointMover::setRotation(int /*axis*/, WFMath::CoordType /*angle*/) {
	//not implemented yet
}

void PolygonPointMover::yaw(WFMath::CoordType /*angle*/) {
}

void PolygonPointMover::setOrientation(const WFMath::Quaternion& /*rotation*/) {
}

void PolygonPointMover::finalizeMovement() {
	//TODO: clean up if the point has been deleted
	mListener->endMovement();
}

void PolygonPointMover::cancelMovement() {
	mPoint.setLocalPosition(mInitialPosition);
	if (mNewPoint) {
		switchToExistingPointMode();
	}
	mListener->cancelMovement();
	// 	mPolygon.endMovement();
}

PolygonPoint* PolygonPointMover::getActivePoint() const {
	if (mNewPoint) {
		return mNewPoint;
	}
	return &mPoint;
}

void PolygonPointMover::input_KeyPressed(const SDL_Keysym& key, Input::InputMode /*mode*/) {
	if (key.sym == SDLK_LCTRL || key.sym == SDLK_RCTRL) {
		if (!mNewPoint) {
			switchToNewPointMode();
		}
	} else if (key.sym == SDLK_LALT || key.sym == SDLK_RALT) {
		switchToDeleteMode();
	}
}

void PolygonPointMover::input_KeyReleased(const SDL_Keysym& key, Input::InputMode /*mode*/) {
	if (key.sym == SDLK_LCTRL || key.sym == SDLK_RCTRL) {
		if (mNewPoint) {
			switchToExistingPointMode();
		}
	} else if (key.sym == SDLK_LALT || key.sym == SDLK_RALT) {
		switchToExistingPointMode();
	}
}

void PolygonPointMover::switchToNewPointMode() {
	if (!mDeleted) {
		if (!mNewPoint) {
			//Get the two nearest points and position the new point to the one's that's closest
			PolygonPoint* point1 = mPolygon.getPointAfter(mPoint);
			PolygonPoint* point2 = mPolygon.getPointBefore(mPoint);
			if (point1 && point2) {
				auto initialDistance1 = WFMath::Distance(mInitialPosition, point1->getLocalPosition());
				auto initialDistance2 = WFMath::Distance(mInitialPosition, point2->getLocalPosition());
				auto currentDistance1 = WFMath::Distance(mPoint.getLocalPosition(), point1->getLocalPosition());
				auto currentDistance2 = WFMath::Distance(mPoint.getLocalPosition(), point2->getLocalPosition());

				auto distanceDiff1 = initialDistance1 - currentDistance1;
				auto distanceDiff2 = initialDistance2 - currentDistance2;
				if (distanceDiff1 < distanceDiff2) {
					mNewPoint = mPolygon.insertPointBefore(mPoint);
				} else {
					mNewPoint = mPolygon.insertPointBefore(*point1);
				}

				mNewPoint->setLocalPosition(mPoint.getLocalPosition());
				mPoint.setLocalPosition(mInitialPosition);
				mPolygon.updateRender();
			}
		}
	}
}

void PolygonPointMover::switchToExistingPointMode() {
	if (mDeleted) {
		if (mPointAfterDeleted) {
			mPolygon.reInsertPointBefore(*mPointAfterDeleted, mPoint);
		} else {
			mPolygon.reInsertPoint(0, mPoint);
		}
		mPoint.setVisible(true);
		mDeleted = false;
		mPointAfterDeleted = nullptr;
		mPolygon.updateRender();
	}
	if (mNewPoint) {
		if (mPolygon.removePoint(*mNewPoint)) {
			mPoint.setLocalPosition(mNewPoint->getLocalPosition());
			delete mNewPoint;
			mNewPoint = nullptr;
			mPolygon.updateRender();
		}
	}
}

void PolygonPointMover::switchToDeleteMode() {
	mPointAfterDeleted = mPolygon.getPointAfter(mPoint);
	mPolygon.removePoint(mPoint);
	mPoint.setVisible(false);
	mDeleted = true;
	mPolygon.updateRender();
}

void PolygonPointMover::setOffset(boost::optional<float>) {

}

boost::optional<float> PolygonPointMover::getOffset() const {
	return boost::none;
}

void PolygonPointMover::processPickResults(const std::vector<PickResult>& results) {
	for (auto& result : results) {
		if (result.collisionInfo.type() == typeid(EntityCollisionInfo)) {
			auto& entityCollisionInfo = boost::any_cast<const EntityCollisionInfo&>(result.collisionInfo);
			//It's a valid entry if it's not transparent and not the entity which is being moved itself.
			if (!entityCollisionInfo.isTransparent && entityCollisionInfo.entity->hasProperty("terrain")) {
				setPosition(result.point);
				return;
			}
		}
	}
}

}

}
}
