/*
 Copyright (C) 2009 Erik Ogenvik <erik@ogenvik.org>

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

#ifndef FIRSTPERSONCAMERAMOUNT_H_
#define FIRSTPERSONCAMERAMOUNT_H_

#include "CameraMountBase.h"
#include <OgreMath.h>
namespace Ember
{
namespace OgreView
{
namespace Camera
{

/**
 * @author Erik Ogenvik <erik@ogenvik.org>
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
	FirstPersonCameraMount(const CameraSettings& cameraSettings, Ogre::SceneManager& sceneManager);

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
	 * @brief The root node of the camera mount.
	 *
	 * This is attached to the controlling node.
	 * This node is rotated, but never pitched. It never changes position.
	 */
	Ogre::SceneNode* mCameraRootNode;

	/**
	 * @brief A node attached to the mCameraRootNode and placed at the same location.
	 *
	 * This node is pitched, but never rotated. It never changes position.
	 */
	Ogre::SceneNode* mCameraPitchNode;

	/**
	 * @brief The node to which the camera is attached.
	 *
	 * This is attached to the mCameraPitchNode, and oriented so that the camera is looking at the direction of the mCameraPitchNode.
	 * This node never changes its orientation.
	 */
	Ogre::SceneNode* mCameraNode;

};

}
}

}

#endif /* FIRSTPERSONCAMERAMOUNT_H_ */
