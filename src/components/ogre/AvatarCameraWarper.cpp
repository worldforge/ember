/*
 Copyright (C) 2013 Erik Ogenvik

 This program is free software; you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation; either version 2 of the License, or
 (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program; if not, write to the Free Software Foundation,
 Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "AvatarCameraWarper.h"
#include "MovementController.h"
#include "domain/EmberEntity.h"
#include "camera/MainCamera.h"
#include "framework/MainLoopController.h"
#include "framework/Tokeniser.h"

#include <Eris/View.h>
#include <OgreSceneNode.h>

#include <sstream>

namespace Ember {
namespace OgreView {

AvatarCameraWarper::AvatarCameraWarper(MovementController& movementController, const Camera::MainCamera& camera, Eris::View& view, float movementThreshold) :
		AvatarFollowsCamera("toggle_avatar_follows_camera", this, "Toggles the avatar being moved when the first person camera is moved. An optional threshold value can also be specified."),
		mMovementController(movementController),
		mCamera(camera),
		mView(view),
		mMovementThreshold(movementThreshold),
		mLastPosition(camera.getPosition()),
		mEnabled(false) {
	MainLoopController::getSingleton().EventFrameProcessed.connect(sigc::mem_fun(*this, &AvatarCameraWarper::frameProcessed));
}

AvatarCameraWarper::~AvatarCameraWarper() = default;

void AvatarCameraWarper::setEnabled(bool enabled) {
	mEnabled = true;
}

void AvatarCameraWarper::runCommand(const std::string& command, const std::string& args) {
	if (AvatarFollowsCamera == command) {
		mEnabled = !mEnabled;
		S_LOG_INFO("Setting avatar camera warper to " << (mEnabled ? "enabled" : "disabled") << ".");
		Tokeniser tokeniser(args);
		if (tokeniser.hasRemainingTokens()) {
			try {
				std::istringstream(tokeniser.nextToken()) >> mMovementThreshold;
			} catch (const std::exception& e) {
				S_LOG_WARNING("Could not update movement threshold in avatar camera warper." << e);
			}
		}
	}
}

void AvatarCameraWarper::frameProcessed(const TimeFrame&, unsigned int) {
	if (mEnabled && mMovementController.isCameraFreeFlying()) {
		const Ogre::Vector3& cameraPosition = mCamera.getPosition();
		if (cameraPosition.distance(mLastPosition) > mMovementThreshold) {
			updateAvatarPosition(cameraPosition);
			mLastPosition = cameraPosition;
		}
	}
}

void AvatarCameraWarper::updateAvatarPosition(const Ogre::Vector3& worldPosition) {
	if (mView.getTopLevel()) {
		mMovementController.teleportTo(worldPosition, dynamic_cast<EmberEntity*>(mView.getTopLevel()));
	}
}

}
}
