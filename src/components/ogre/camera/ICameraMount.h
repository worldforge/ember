/*
 Copyright (C) 2009 Erik Hjortsberg <erik.hjortsberg@gmail.com>

 This program is free software; you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation; either version 2 of the License, or
 (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program; if not, write to the Free Software
 Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#ifndef ICAMERAMOUNT_H_
#define ICAMERAMOUNT_H_

#include "components/ogre/OgreIncludes.h"

namespace WFMath
{
template<int> class Vector;
class Quaternion;
}

namespace EmberOgre
{
class ICameraMotionHandler;

namespace Camera
{
class MainCamera;

/**
 * @author Erik Hjortsberg <erik.hjortsberg@gmail.com>
 *
 * @brief A mount for a camera.
 *
 * The camera mount is responsible for handling how the camera is moved and rotated. This is independent from the MainCamera class to allow for many different styles of camera attachment. The most obvious ones would be a third person camera mount, and a first person mount.
 * The MainCamera class can only be attached to one mount at a time. This is done by calling the MainCamera::attachToMount() method. When the camera is attached to a mount, that mount will receive calls to pitch() and yaw(), asking it to update its orientation.
 * When the user wants to move, the mount will also receive calls to move(). This allow the different types of mounts to implement different movement styles. A camera which is attached to an entity would for example probably want to try to move the entity within the world, whereas a free flying camera would want to move the camera directly.
 * In most cases the handling of the motion will need to be separated from the actual mount, and therefore the setMotionHandler() method is provided.
 */
class ICameraMount
{
public:

	/**
	 * @brief Dtor.
	 */
	virtual ~ICameraMount()
	{
	}
	;

	/**
	 * @brief Pitches the camera the supplied degrees
	 * @param degrees
	 */
	virtual Ogre::Degree pitch(float relativeMovement) = 0;

	/**
	 * @brief Yaws the camera the supplied degrees
	 * @param degrees
	 */
	virtual Ogre::Degree yaw(float relativeMovement) = 0;

	/**
	 * @brief Returns the current degrees of pitch from the camera's initial position.
	 * @return The current degrees of pitch from the camera's initial position.
	 */
	virtual const Ogre::Degree& getPitch() const = 0;

	/**
	 * @brief Returns the current degrees of yaw from the camera's initial position.
	 * @return The current degree of yaw from the camera's intial position.
	 */
	virtual const Ogre::Degree& getYaw() const = 0;

	/**
	 * @brief Asks the mount to move itself.
	 * Depending on the type of mount, this movement can be carried out differently.
	 * @param movement The movement, in normalized form, where 1.0 means max speed. The movement is also relative to the orientation sent in the "orientation" parameter, with y = 1 meaning forward movement.
	 * @param orientation The orientation of the movement.
	 * @param timeslice The time slice for the movement. This should in most cases be used to determine the final movement for this frame.
	 */
	virtual void move(const WFMath::Vector<3>& movement, const WFMath::Quaternion& orientation, float timeslice) = 0;

	/**
	 * @brief Attaches this mount to a camera.
	 * @note This is only called by MainCamera, do not call this yourself.
	 * @param camera The camera to attach to.
	 */
	virtual void attachToCamera(MainCamera& camera) = 0;

	/**
	 * @brief Detaches from the attached camera.
	 * @note This is only called by MainCamera, do not call this yourself.
	 */
	virtual void detachFromCamera() = 0;

	/**
	 * @brief Sets a motion handler, which can be used for handling the motion of the mount.
	 * In most cases the mount would want to hand over the actual motion handling to an instance of ICameraMotionHandler. This method allows the setting of this instance.
	 */
	virtual void setMotionHandler(ICameraMotionHandler* handler) = 0;

};
}
}
#endif /* ICAMERAMOUNT_H_ */
