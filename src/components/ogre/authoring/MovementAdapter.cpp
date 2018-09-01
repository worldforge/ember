//
// C++ Implementation: MovementAdapter
//
// Description:
//
//
// Author: Erik Ogenvik <erik@ogenvik.org>, (C) 2006
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

#include "MovementAdapter.h"
#include "IMovementBridge.h"
#include "components/ogre/camera/MainCamera.h"
#include "components/ogre/EntityCollisionInfo.h"
#include "../AvatarTerrainCursor.h"
#include "../Convert.h"
#include <OgreRoot.h>
#include <OgreCamera.h>

using namespace WFMath;
using namespace Ember;

namespace Ember {
namespace OgreView {
namespace Authoring {

MovementAdapterWorkerBase::MovementAdapterWorkerBase(MovementAdapter& adapter) :
		mAdapter(adapter) {
}

std::shared_ptr<IMovementBridge> MovementAdapterWorkerBase::getBridge() {
	return mAdapter.mBridge;
}

const Camera::MainCamera& MovementAdapterWorkerBase::getCamera() const {
	return mAdapter.mCamera;
}


MovementAdapterWorkerDiscrete::MovementAdapterWorkerDiscrete(MovementAdapter& adapter) :
		MovementAdapterWorkerBase(adapter),
		mMovementSpeed(10) {
}

bool MovementAdapterWorkerDiscrete::injectMouseMove(const MouseMotion& motion, bool& freezeMouse) {
	//this will move the entity instead of the mouse

	Vector<3> direction;
	direction.zero();
	direction.x() = -motion.xRelativeMovement;
	direction.z() = -motion.yRelativeMovement;
	direction = direction * mMovementSpeed;
	//hard coded to allow the shift button to increase the speed
	// 	if (Input::getSingleton().isKeyDown(SDLK_RSHIFT) || Input::getSingleton().isKeyDown(SDLK_LSHIFT)) {
	// 		direction = direction * 5;
	// 	}

	Quaternion orientation = Convert::toWF(getCamera().getOrientation());

	//We need to constraint the orientation to only around the y axis.
	WFMath::Vector<3> rotator(0.0, 0.0, 1.0f);
	rotator.rotate(orientation);
	auto atan = std::atan2(rotator.x(), rotator.z());
	orientation.rotation(1, atan);

	//move it relative to the camera
	direction = direction.rotate(orientation);

	getBridge()->move(direction);//move the entity a fixed distance for each mouse movement.

	//we don't want to move the cursor
	freezeMouse = true;

	return false;

}

MovementAdapterWorkerTerrainCursor::MovementAdapterWorkerTerrainCursor(MovementAdapter& adapter) :
		MovementAdapterWorkerBase(adapter) {
	// Register this as a frame listener
	Ogre::Root::getSingleton().addFrameListener(this);
}

MovementAdapterWorkerTerrainCursor::~MovementAdapterWorkerTerrainCursor() {
	Ogre::Root::getSingleton().removeFrameListener(this);
}

void MovementAdapterWorkerTerrainCursor::update() {
	updatePosition(true);
}

bool MovementAdapterWorkerTerrainCursor::frameStarted(const Ogre::FrameEvent&) {
	updatePosition();
	return true;
}

void MovementAdapterWorkerTerrainCursor::updatePosition(bool forceUpdate) {

	const MousePosition& mousePosition(Input::getSingleton().getMousePosition());
	Ogre::Ray cameraRay = getCamera().getCamera().getCameraToViewportRay(mousePosition.xRelativePosition, mousePosition.yRelativePosition);
	auto results = getCamera().pick(cameraRay, 300);

	for (auto& result : results) {
		if (mAdapter.getBridge()->isCollisionResultValid(result)) {
			getBridge()->setPosition(result.point);
			break;
		}
	}
}


MovementAdapterWorkerHeightOffset::MovementAdapterWorkerHeightOffset(MovementAdapter& adapter)
		: MovementAdapterWorkerBase(adapter) {

}

bool MovementAdapterWorkerHeightOffset::injectMouseMove(const MouseMotion& motion, bool& freezeMouse) {
	auto existingOffset = getBridge()->getOffset();
	boost::optional<float> offset(0);

	if (existingOffset) {
		offset = existingOffset;
	}
	offset.get() += motion.yRelativeMovement * 10;
	getBridge()->setOffset(offset);
	freezeMouse = true;
	return false;
}

MovementAdapter::MovementAdapter(const Camera::MainCamera& camera) :
		mCamera(camera),
		mBridge(nullptr),
		mWorker(nullptr) {
}

MovementAdapter::~MovementAdapter() {
	//detach(); //A call to this will delete both the bridge and the adapter.
}

void MovementAdapter::finalizeMovement() {
	removeAdapter();
	//We need to do it this way since there's a chance that the call to IMovementBridge::finalizeMovement will delete this instance, and then we can't reference mBridge anymore
	auto bridge = mBridge;
	mBridge.reset();
	bridge->finalizeMovement();
}

void MovementAdapter::cancelMovement() {
	removeAdapter();
	//We need to do it this way since there's a chance that the call to IMovementBridge::cancelMovement will delete this instance, and then we can't reference mBridge anymore
	auto bridge = mBridge;
	mBridge.reset();
	bridge->cancelMovement();
}

bool MovementAdapter::injectMouseMove(const MouseMotion& motion, bool& freezeMouse) {
	if (mWorker) {
		return mWorker->injectMouseMove(motion, freezeMouse);
	}
	return true;
}

bool MovementAdapter::injectMouseButtonUp(const Input::MouseButton& button) {
	if (button == Input::MouseButtonLeft) {
		finalizeMovement();
		//After we've finalized we've done here, so we should let other IInputAdapters handle the mouse button up (we've had an issue where cegui didn't receive the mouse up event, which made it think that icons that were dragged were still being dragged (as the end-drag event is emitted on mouse up))
		return true;
	} else if (button == Input::MouseButtonRight) {
	} else {
		return false;
	}

	return false;
}

bool MovementAdapter::injectMouseButtonDown(const Input::MouseButton& button) {
	if (button == Input::MouseButtonLeft) {
	} else if (button == Input::MouseButtonRight) {

	} else if (button == Input::MouseButtonMiddle) {

	} else if (button == Input::MouseWheelUp) {
		int movementDegrees = 10;
		if (Input::getSingleton().isKeyDown(SDL_SCANCODE_LSHIFT) || Input::getSingleton().isKeyDown(SDL_SCANCODE_RSHIFT)) {
			movementDegrees = 1;
		}
		mBridge->yaw(movementDegrees);
	} else if (button == Input::MouseWheelDown) {
		int movementDegrees = 10;
		if (Input::getSingleton().isKeyDown(SDL_SCANCODE_LSHIFT) || Input::getSingleton().isKeyDown(SDL_SCANCODE_RSHIFT)) {
			movementDegrees = 1;
		}
		mBridge->yaw(-movementDegrees);
	}

	return false;
}

bool MovementAdapter::injectChar(int character) {
	return true;
}

bool MovementAdapter::injectKeyDown(const SDL_Scancode& key) {
	if (mWorker) {
		//by pressing and holding shift we'll allow the user to position it with more precision. We do this by switching the worker instances.
		if (key == SDL_SCANCODE_LSHIFT || key == SDL_SCANCODE_RSHIFT) {
			mWorker.reset(new MovementAdapterWorkerDiscrete(*this));
		}

		if (key == SDL_SCANCODE_Q) {
			mWorker.reset(new MovementAdapterWorkerHeightOffset(*this));
		}
	}
	return true;
}

bool MovementAdapter::injectKeyUp(const SDL_Scancode& key) {
	if (key == SDL_SCANCODE_ESCAPE) {
		cancelMovement();
		return false;
	} else if (key == SDL_SCANCODE_LSHIFT || key == SDL_SCANCODE_RSHIFT || key == SDL_SCANCODE_Q) {
		if (mWorker) {
			mWorker.reset(new MovementAdapterWorkerTerrainCursor(*this));
		}
	}

	return true;
}

void MovementAdapter::attachToBridge(std::shared_ptr<IMovementBridge> bridge) {
	mBridge = bridge;
	addAdapter();
}

void MovementAdapter::detach() {
	mBridge.reset();
	removeAdapter();
}

void MovementAdapter::removeAdapter() {
	Input::getSingleton().removeAdapter(this);
	mWorker.reset();
}

void MovementAdapter::addAdapter() {
	Input::getSingleton().addAdapter(this);
	//default to the terrain cursor positioning mode
	mWorker.reset(new MovementAdapterWorkerTerrainCursor(*this));
}

void MovementAdapter::update() {
	if (mWorker) {
		mWorker->update();
	}
}

const std::shared_ptr<IMovementBridge>& MovementAdapter::getBridge() const {
	return mBridge;
}

}

}
}
