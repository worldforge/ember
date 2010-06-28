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

#ifndef FIRSTPERSONCAMERAMOUNT_H_
#define FIRSTPERSONCAMERAMOUNT_H_

#include "CameraMountBase.h"
#include <OgreMath.h>
namespace EmberOgre
{
namespace Camera
{

/**
 * @author Erik Hjortsberg <erik.hjortsberg@gmail.com>
 *
 * @brief A first person camera mount, where the mount is directly attached to something.
 */
class FirstPersonCameraMount : public CameraMountBase
{
public:

	/**
	 * @brief Ctor.
	 * @param sceneManager The main scene manager.
	 */
	FirstPersonCameraMount(Ogre::SceneManager& sceneManager);

	/**
	 * @brief Dtor.
	 */
	virtual ~FirstPersonCameraMount();


	/**
	 * @brief Sets the node to which the camera is attached.
	 * @param sceneNode The scene node to which the camera should be attached. Or null if the camera shouldn't be attached to anything.
	 */
	virtual void attachToNode(Ogre::Node* sceneNode);

	/**
	 * @brief Pitches the camera the supplied degrees
	 * @param degrees
	 */
	virtual Ogre::Degree pitch(float relativeMovement);

	/**
	 * @brief Yaws the camera the supplied degrees
	 * @param degrees
	 */
	virtual Ogre::Degree yaw(float relativeMovement);

	/**
	 * @copydoc ICameraMount::attachToCamera
	 */
	virtual void attachToCamera(MainCamera& camera);

	/**
	 * @copydoc ICameraMount::detachFromCamera
	 */
	virtual void detachFromCamera();

private:

	/**
	 * @brief The amount of degrees to pitch per second.
	 */
	Ogre::Degree mDegreeOfPitchPerSecond;

	/**
	 * @brief The amount of degrees to yaw per second.
	 */
	Ogre::Degree mDegreeOfYawPerSecond;

	/**
	 * @brief The main camera node.
	 */
	Ogre::SceneNode* mCameraNode;

	/**
	 * @brief If true, the camera is inverted in the y axis.
	 */
	bool mInvertCamera;

	/**
	 * @brief Creates all nodes needed for the camera.
	 */
	void createNodesForCamera(Ogre::SceneManager& sceneManager);

};

}
}

#endif /* FIRSTPERSONCAMERAMOUNT_H_ */
