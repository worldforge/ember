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

#ifndef THIRDPERSONCAMERAMOUNT_H_
#define THIRDPERSONCAMERAMOUNT_H_

#include "CameraMountBase.h"
#include "framework/ConsoleObject.h"

#include <sigc++/signal.h>
#include <OgreMath.h>
#include <OgreVector3.h>
#include <OgreQuaternion.h>
#include <OgreRay.h>

namespace EmberOgre
{
namespace Camera
{

class ThirdPersonCameraMount : public CameraMountBase, public Ember::ConsoleObject
{
public:
	ThirdPersonCameraMount(Ogre::SceneManager& sceneManager);

	/**
	* emitted when the distance between the camera and the avatar has changed
    * @param Ogre::Real the new distance
	*/
	sigc::signal<void, Ogre::Real> EventChangedCameraDistance;

	/**
	 *    Sets the distance from the camera to the avatar.
	 * @param distance the new distance
	 */
	void setCameraDistance(Ogre::Real distance);

	const Ember::ConsoleCommandWrapper SetCameraDistance;

	/**
	 * sets the node to which the camera is attached
	 */
	virtual void attachToNode(Ogre::SceneNode* sceneNode);


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

 	virtual void attachToCamera(MainCamera& camera);

 	virtual void detachFromCamera();

	/**
	 *    Reimplements the ConsoleObject::runCommand method
	 * @param command
	 * @param args
	 */
	virtual	void runCommand(const std::string &command, const std::string &args);

protected:

	Ogre::SceneNode* mAvatarCameraRootNode;
	Ogre::SceneNode* mAvatarCameraPitchNode;
	Ogre::SceneNode* mAvatarCameraNode;


	Ogre::Degree mDegreeOfPitchPerSecond;
	Ogre::Degree mDegreeOfYawPerSecond;

	Ogre::Vector3 mLastPosition;
	Ogre::Real mWantedCameraDistance, mCurrentCameraDistance;

	Ogre::RaySceneQuery *mAdjustTerrainRaySceneQuery, *mCameraRaySceneQuery;
	Ogre::Ray mAdjustTerrainRay;

	bool mIsAdjustedToTerrain;

	/**
	If true, the camera is inverted in the y axis.
	*/
	bool mInvertCamera;

	/**
	Creates the rays needed for mouse picking and camera adjustment.
	*/
	void createRayQueries(Ogre::SceneManager& sceneManager);

	/**
	 * creates all nodes needed for the camera
	 */
	void createNodesForCamera(Ogre::SceneManager& sceneManager);

	/**
	 *    Internal method for setting the camera distance.
	 * @param distance the new distance
	 */
	void _setCameraDistance(Ogre::Real distance);
};
}
}

#endif /* THIRDPERSONCAMERAMOUNT_H_ */
