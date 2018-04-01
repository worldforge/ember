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

#ifndef THIRDPERSONCAMERAMOUNT_H_
#define THIRDPERSONCAMERAMOUNT_H_

#include "CameraMountBase.h"
#include "framework/ConsoleObject.h"

#include <sigc++/signal.h>
#include <sigc++/trackable.h>
#include <OgreMath.h>
#include <OgreVector3.h>
#include <OgreQuaternion.h>
#include <OgreRay.h>

namespace varconf {
class Variable;
}

namespace Ember
{
class ConfigListenerContainer;
namespace OgreView
{
class Scene;
namespace Terrain
{
class ITerrainAdapter;
}
namespace Camera
{

/**
 * @author Erik Ogenvik <erik@ogenvik.org>
 * @brief A third person camera mount, where the camera is circling a node. This node is then in most cases attached to an entity.
 */
class ThirdPersonCameraMount: public CameraMountBase, public ConsoleObject, public virtual sigc::trackable
{
public:

	/**
	 * @brief Ctor.
	 * @param cameraSettings Shared camera settings.
	 * @param sceneManager A scene manager, needed for creating new node instances.
	 */
	ThirdPersonCameraMount(const CameraSettings& cameraSettings, Scene& scene, Terrain::ITerrainAdapter& terrainAdapter);

	/**
	 * @brief Dtor.
	 */
	~ThirdPersonCameraMount() override;

	/**
	 * @brief Sets the distance from the camera to the entity node.
	 * @param distance The new distance, in world units.
	 */
	void setCameraDistance(Ogre::Real distance);

	/**
	 * @brief Console command for setting the distance of the camera from the node it's observing.
	 */
	const ConsoleCommandWrapper SetCameraDistance;

	/**
	 * @brief Sets the node to which the camera is attached.
	 */
	virtual void attachToNode(Ogre::Node* sceneNode);

	/**
	 * @brief Pitches the camera the supplied degrees.
	 * @param degrees
	 */
	Ogre::Degree pitch(float relativeMovement) override;

	/**
	 * @brief Yaws the camera the supplied degrees.
	 * @param degrees
	 */
	Ogre::Degree yaw(float relativeMovement) override;

	/**
	 * @copydoc ICameraMount::attachToCamera
	 */
	void attachToCamera(MainCamera& camera) override;

	/**
	 * @copydoc ICameraMount::detachFromCamera
	 */
	void detachFromCamera() override;

	/**
	 * @copydoc ConsoleObject::runCommand method
	 */
	void runCommand(const std::string &command, const std::string &args) override;

	void update() override;

protected:

	Scene& mScene;

	/**
	 * @brief The scene manager to which the camera belongs.
	 */
	Ogre::SceneManager& mSceneManager;

	/**
	 * @brief The root node of the camera mount.
	 *
	 * This is positioned right at the entity we're following, and the camera is looking straight at it.
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
	 * This is attached to the root node and adjusted each frame to match the pitch node. This allows us to adjust it to avoid collisions.
	 */
	Ogre::SceneNode* mCameraNode;

	/**
	 * @brief The desired distance of the camera from the root node, in world units.
	 *
	 * This might differ from the actual distance if the camera is adjusted for the terrain.
	 */
	Ogre::Real mWantedCameraDistance;

	/**
	 * @brief The actual distance of the camera from the root node, in world units.
	 */
	Ogre::Real mCurrentCameraDistance;

	/**
	 * @brief If true, the camera should be adjusted to the terrain, so that it never dips below it.
	 */
	bool mIsAdjustedToTerrain;

	/**
	 * @brief Listens to configuration changes.
	 */
	ConfigListenerContainer* mConfigListenerContainer;

	/**
	 * @brief The terrain adapter used to find terrain intersections.
	 */
	Terrain::ITerrainAdapter& mTerrainAdapter;

	/**
	 * @brief Creates all nodes needed for the camera.
	 */
	void createNodesForCamera(Ogre::SceneManager& sceneManager);

	/**
	 * @brief Checks whether the camera should be adjusted to avoid occlusion by terrain or other geometry.
	 */
	void adjustForOcclusion();

};
}
}

}

#endif /* THIRDPERSONCAMERAMOUNT_H_ */
