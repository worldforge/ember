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

#include "framework/ConsoleObject.h"
#include <sigc++/trackable.h>

#include "services/input/Input.h"
#include <stack>
#include <memory>


#include <OgreFrameListener.h>


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
}

namespace EmberOgre {
class Avatar;
class EmberEntity;
struct EntityPickResult;
class IWorldPickListener;
class IMovementProvider;
class ICameraMotionHandler;
struct MousePickerArgs;
class AvatarTerrainCursor;

/**
 * @brief Namespace for all camera related classes and activities.
 */
namespace Camera
{
class ICameraMount;
class Recorder;

class MainCamera : public Ogre::FrameListener, public Ember::ConsoleObject, public virtual sigc::trackable
{
public:
	MainCamera(Ogre::SceneManager& sceneManager, Ogre::RenderWindow& window, Ember::Input& input, Ogre::Camera& camera);
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
	 *    Reimplements the ConsoleObject::runCommand method
	 * @param command
	 * @param args
	 */
	virtual	void runCommand(const std::string &command, const std::string &args);

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


	const Ember::ConsoleCommandWrapper ToggleRendermode;
	const Ember::ConsoleCommandWrapper ToggleFullscreen;
	const Ember::ConsoleCommandWrapper Screenshot;
	const Ember::ConsoleCommandWrapper Record;

	/**
		Toggles between wireframe and solid render mode.
	*/
	void toggleRenderMode();

	/**
	 * takes a screen shot and writes it to disk
	 */
	void takeScreenshot();


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

private:
	typedef std::deque<IWorldPickListener*> WorldPickListenersStore;
	typedef std::vector<std::string> CompositorNameStore;

	Ogre::Camera& mCamera;
	ICameraMount* mCameraMount;

	WorldPickListenersStore mPickListeners;

	std::auto_ptr<Recorder> mRecorder;

	CompositorNameStore mLoadedCompositors;

	Ogre::RenderWindow& mWindow;

	//in meters how far we can pick objects
	Ogre::Real mClosestPickingDistance;

	Ogre::RaySceneQuery* mCameraRaySceneQuery;

	std::auto_ptr<AvatarTerrainCursor> mAvatarTerrainCursor;

	bool mCameraOrientationChangedThisFrame;

	IMovementProvider* mMovementProvider;


	void ConfigService_EventChangedConfigItem(const std::string& section, const std::string& key);

	void updateValuesFromConfig();

	/**
	 * @brief Call this whenever you've moved any node which will make the derived position or orientation of the camera change.
	 * We need to mark the parent node of the camera as dirty. The update of the derived orientation and position of the node should normally occur when the scene tree is traversed, but in some instances we need to access the derived position or orientataion of the camera before the traversal occurs, and if we don't mark the node as dirty it won't be updated
	 */
	void markCameraNodeAsDirty();

	/**
	Creates the rays needed for mouse picking and camera adjustment.
	*/
	void createRayQueries(Ogre::SceneManager& sceneManager);

	const std::string _takeScreenshot();

	void Input_MouseMoved(const Ember::MouseMotion& motion, Ember::Input::InputMode mode);

};

}

}

#endif // AVATARCAMERA_H
