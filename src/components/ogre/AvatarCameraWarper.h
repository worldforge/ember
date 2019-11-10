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

#ifndef ADMINMOVEMENTADJUSTER_H_
#define ADMINMOVEMENTADJUSTER_H_

#include "OgreIncludes.h"
#include "framework/TimeFrame.h"
#include "framework/ConsoleCommandWrapper.h"
#include "framework/ConsoleObject.h"

#include <OgreVector3.h>
#include <sigc++/trackable.h>

namespace Eris
{
class View;
}
namespace Ember
{
namespace OgreView
{

namespace Camera
{
class MainCamera;
}

class MovementController;

/**
 * @brief Warps the position of the avatar to match the camera.
 *
 * This is mainly of use when authoring and using the free flying mode.
 * In fact, the class will check with MovementController if we indeed are in free flying mode, and if so adjust the position.
 *
 * Note that this can only be applied when logged in as an admin (since otherwise you won't have the ability to set the position of the avatar).
 */
class AvatarCameraWarper: public ConsoleObject, public virtual sigc::trackable
{
public:
	/**
	 * @brief Ctor.
	 * @param movementController The main movement controller.
	 * @param camera The main camera.
	 * @param view The view of the world.
	 * @param movementThreshold Default threshold for movement. If the distance between the camera and the avatar is larger than this, the avatar is moved.
	 */
	AvatarCameraWarper(MovementController& movementController, const Camera::MainCamera& camera, Eris::View& view, float movementThreshold = 20.0f);

	/**
	 * @brief Dtor.
	 */
	~AvatarCameraWarper() override;

	/**
	 * @brief Sets whether the adjuster should be enabled or not.
	 * @param enabled
	 */
	void setEnabled(bool enabled);

	void runCommand(const std::string& command, const std::string& args) override;

	ConsoleCommandWrapper AvatarFollowsCamera;
private:

	/**
	 * @brief The main movement controller.
	 */
	MovementController& mMovementController;

	/**
	 * @brief The main camera.
	 */
	const Camera::MainCamera& mCamera;

	/**
	 * @brief The view of the world.
	 */
	Eris::View& mView;

	/**
	 * @brief Threshold for movement. If the distance between the camera and the avatar is larger than this, the avatar is moved.
	 */
	float mMovementThreshold;

	/**
	 * @brief Keep track of the position of the camera when we last moved.
	 *
	 * This is used to determine if we should move the avatar again.
	 */
	Ogre::Vector3 mLastPosition;

	/**
	 * @brief True if enabled.
	 */
	bool mEnabled;

	void frameProcessed(const TimeFrame& timeFrame, unsigned int eventMask);

	/**
	 * @brief Updated the position of the avatar.
	 * @param worldPosition The new position in the world.
	 */
	void updateAvatarPosition(const Ogre::Vector3& worldPosition);
};

}
}
#endif /* ADMINMOVEMENTADJUSTER_H_ */
