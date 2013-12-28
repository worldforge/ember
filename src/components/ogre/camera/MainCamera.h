/*
 Copyright (C) 2004  Erik Hjortsberg <erik.hjortsberg@gmail.com>

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

/*
 * An instance of this is a player controlled camera fastened to the Avatar.
 * It should be possible to subclass this in order to provide different behaviour
 */

#ifndef AVATARCAMERA_H
#define AVATARCAMERA_H

#include "components/ogre/OgreIncludes.h"

#include "services/input/Input.h"

#include <sigc++/trackable.h>

#include <stack>
#include <memory>

#include <OgreFrameListener.h>
#include <OgreSceneQuery.h>

namespace WFMath
{
template<int> class Vector;
class Quaternion;
}

namespace Ember
{
struct MouseMotion;
class InputCommandMapper;
class Input;
class ConfigListenerContainer;

namespace OgreView
{
class Avatar;
class EmberEntity;
struct EntityPickResult;
class IWorldPickListener;
class IMovementProvider;
class ICameraMotionHandler;
struct MousePickerArgs;
class AvatarTerrainCursor;

namespace Terrain
{
class ITerrainAdapter;
}

/**
 * @brief Namespace for all camera related classes and activities.
 */
namespace Camera
{
class ICameraMount;
class Recorder;
class CameraSettings;

/**
 * @author Erik Ogenvik <erik@ogenvik.org>
 * @brief The main camera in the system.
 *
 * While there can be many different cameras in the system, this represent the "main" one. It renders directly to the main Ogre::RenderWindow.
 */
class MainCamera: public Ogre::FrameListener, public virtual sigc::trackable
{
public:

	/**
	 * Ctor.
	 * @param sceneManager The scene manager to which the camera belongs.
	 * @param window The main rendering window.
	 * @param input The input instance.
	 * @param camera The main camera.
	 * @param terrainAdapter
	 */
	MainCamera(Ogre::SceneManager& sceneManager, Ogre::RenderWindow& window, Input& input, Ogre::Camera& camera, Terrain::ITerrainAdapter& terrainAdapter);

	/**
	 * Dtor.
	 */
	virtual ~MainCamera();

	/**
	 * @brief Accessor for the main Ogre::Camera instance.
	 * @return The main Ogre::Camera instance.
	 */
	Ogre::Camera& getCamera();

	/**
	 * @brief Accessor for the main Ogre::Camera instance.
	 * @return The main Ogre::Camera instance.
	 */
	Ogre::Camera& getCamera() const;

	/**
	 * @brief Returns the current camera orientation in the world
	 * @param onlyHorizontal
	 * @return
	 */
	const Ogre::Quaternion& getOrientation(bool onlyHorizontal = false) const;

	/**
	 * @brief Returns the position of the camera in the world.
	 * @return
	 */
	const Ogre::Vector3& getPosition() const;

	/**
	 * @brief Emitted when the camera moves
	 */
	sigc::signal<void, Ogre::Camera&> MovedCamera;

	void pickInWorld(Ogre::Real mouseX, Ogre::Real mouseY, const MousePickerArgs& args);

	void setClosestPickingDistance(Ogre::Real distance);

	Ogre::Real getClosestPickingDistance() const;

	/**
	 * returns true if the worldPos is on screen, putting the screen pos into the x & y of the second Vector3
	 * returns false if the worldPos is off screen
	 * @param worldPos
	 * @param screenPos
	 * @return
	 */
	bool worldToScreen(const Ogre::Vector3& worldPos, Ogre::Vector2& screenPos);

	/**
	 * Methods from Ogre::FrameListener
	 */
	bool frameStarted(const Ogre::FrameEvent& event);

	/**
	 *    Enables and disables a compositor by name.
	 * @param compositorName
	 * @param enable
	 */
	void enableCompositor(const std::string& compositorName, bool enable);

	/**
	 *    Adds a new world pick listener to the queue of listeners.
	 * @param worldPickListener
	 */
	void pushWorldPickListener(IWorldPickListener* worldPickListener);

	void removeWorldPickListener(IWorldPickListener* worldPickListener);

	/**
	 * @brief Attaches the camera to a new mount, returning the previous mount, if any.
	 * @param  newCameraMount The new camera mount.
	 * @return If the camera already was attached to a mount it's returned here, after the camera has been detached from it.
	 */
	ICameraMount* attachToMount(ICameraMount* newCameraMount);

	void setMovementProvider(IMovementProvider* movementProvider);

	/**
	 * @brief Gets the terrain cursor, used for efficiently getting the position of the terrain currently under the cursor.
	 * @return An object which can be used to get the terrain that intersects the terrain at the current cursor.
	 */
	AvatarTerrainCursor& getTerrainCursor() const;

	/**
	 * Accessor for the shared camera settings.
	 * @return The shared camera settings.
	 */
	const CameraSettings& getCameraSettings() const;

private:
	typedef std::deque<IWorldPickListener*> WorldPickListenersStore;
	typedef std::vector<std::string> CompositorNameStore;

	Ogre::SceneManager& mSceneManager;
	Ogre::Camera& mCamera;
	ICameraMount* mCameraMount;

	WorldPickListenersStore mPickListeners;

	CompositorNameStore mLoadedCompositors;

	Ogre::RenderWindow& mWindow;

	//in meters how far we can pick objects
	Ogre::Real mClosestPickingDistance;

	Ogre::RaySceneQuery* mCameraRaySceneQuery;

	std::unique_ptr<AvatarTerrainCursor> mAvatarTerrainCursor;

	bool mCameraOrientationChangedThisFrame;

	IMovementProvider* mMovementProvider;

	/**
	 * @brief The shared camera settings, which is used by the various mounts.
	 */
	CameraSettings* mCameraSettings;

	/**
	 * @brief Listens for config changes.
	 */
	ConfigListenerContainer* mConfigListenerContainer;

	/**
	 * @brief The terrain adapter used to check for intersections with the terrain.
	 */
	Terrain::ITerrainAdapter& mTerrainAdapter;

	/**
	 * @brief A WorldFragment used when injecting a custom terrain intersection result into the scene query
	 */
	Ogre::SceneQuery::WorldFragment mTerrainResultWorldFragment;

	/**
	 * @brief Sets the near and far clip distances of the camera.
	 * @param section
	 * @param key
	 * @param variable
	 */
	void Config_ClipDistances(const std::string& section, const std::string& key, varconf::Variable& variable);

	/**
	 * @brief Sets the compositors which are loaded.
	 * @param section
	 * @param key
	 * @param variable
	 */
	void Config_Compositors(const std::string& section, const std::string& key, varconf::Variable& variable);

	/**
	 * @brief Call this whenever you've moved any node which will make the derived position or orientation of the camera change.
	 * We need to mark the parent node of the camera as dirty. The update of the derived orientation and position of the node should normally occur when the scene tree is traversed, but in some instances we need to access the derived position or orientataion of the camera before the traversal occurs, and if we don't mark the node as dirty it won't be updated
	 */
	void markCameraNodeAsDirty();

	/**
	 * @brief Creates the rays needed for mouse picking and camera adjustment.
	 */
	void createRayQueries(Ogre::SceneManager& sceneManager);

	/**
	 * @brief Called when the mouse has moved.
	 * @param motion The mouse motion.
	 * @param mode The current input mode.
	 */
	void Input_MouseMoved(const MouseMotion& motion, Input::InputMode mode);

	/**
	 * @brief Validates that the composition output pass is correct.
	 * We have to do this when a compositor is loaded, since else we run the risk of getting an exception thrown during rendering.
	 * @param compositionPass A composition pass.
	 * @return True if the pass is valid.
	 */
	bool validateCompositionTargetPass(Ogre::CompositionTargetPass& compositionPass);

};

}

}

}

#endif // AVATARCAMERA_H
