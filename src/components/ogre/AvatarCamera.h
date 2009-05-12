/*
    Copyright (C) 2004  Erik Hjortsberg

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

#include "EmberOgrePrerequisites.h"

#include <sigc++/trackable.h>


#include "framework/ConsoleObject.h"

#include "services/input/Input.h"
#include <stack>
#include <memory>

#include <OgreMath.h>
#include <OgreRay.h>
#include <OgreVector3.h>
#include <OgreFrameListener.h>

namespace Ember
{
struct MouseMotion;
}

namespace EmberOgre {

class Avatar;
class EmberEntity;
struct EntityPickResult;
class IWorldPickListener;
struct MousePickerArgs;
class AvatarTerrainCursor;

class Recorder :public Ogre::FrameListener
{
public:
 	Recorder();
	void startRecording();
	void stopRecording();
	/**
	 * Methods from Ogre::FrameListener
	 */
	bool frameStarted(const Ogre::FrameEvent& event);
private:
	int mSequence;
	float mAccruedTime;
	float mFramesPerSecond;
};

/**
The main camera in the system, for historical reasons named "AvatarCamera". While Ogre allows you to have multiple cameras, only one will be used for the "main screen".

This class handles rotation and positioning of the camera as the mouse changes. It can operate in two modes: either attached to an avatar, or in free flying mode.

By default it makes checks against the terrain, so as not to dip below it. It's also not possible to "invert" the camera by moving it so the world is upside down.

It also takes care of initializing picking operations when the user picks something in the world.

TODO: refactor this into a better system, where the different camera setups are handled by different classes, and where the movement of the avatar contra the movement of the camera is handled better. See ICameraMount
*/
class AvatarCamera
:
public sigc::trackable,
public Ember::ConsoleObject,
public Ogre::FrameListener
{
public:

	/**
	The different modes of the camera.
	TODO: this should be removed in favour of different classes instead
	*/
	enum Mode {
		/**
		Third person mode is when the camera is watching the avatar from afar.
		*/
		MODE_THIRD_PERSON = 1,
		/**
		First person mode is when the camera reflects what the avatar is seeing through its eyes.
		*/
		MODE_FIRST_PERSON = 2
	};

	/**
	 *    Ctor.
	 Creates a camera and attaches it to a node.
	 * @param avatarNode The main avatar node to which the camera should be attached.
	 * @param sceneManager The main scene manager, through which the needed Ogre objects (such as Ogre::RayQuery) will be created.
	 * @param window The window to which the camera will render.
	 * @param input
	 * @param camera
	 */
	AvatarCamera(Ogre::SceneNode* avatarNode, Ogre::SceneManager& sceneManager, Ogre::RenderWindow& window, Ember::Input& input, Ogre::Camera& camera);
	virtual ~AvatarCamera();

	/**
	 * @brief Pitches the camera the supplied degrees.
	 * It's not possible to pitch the camera upside down, i.e. over the vertical axis of the node it's attached to. Trying to do so will result in the amount of pitch applied will be truncated.
	 * @param degrees The amount of degrees to pitch.
	 */
	virtual void pitch(Ogre::Degree degrees);

	/**
	 * @brief Yaws the camera the supplied degrees.
	 * @param degrees The amount of degrees to yaw.
	 */
	virtual void yaw(Ogre::Degree degrees);

	/**
	 * @brief Returns the current degrees of pitch from the cameras initial position.
	 * @return The amount of degrees the camera currently is pitched.
	 */
	const Ogre::Degree& getPitch() const;

	/**
	 * @brief Returns the current degrees of yaw from the cameras initial position.
	 * @return The amount of degrees the camera currently is yawed.
	 */
	const Ogre::Degree& getYaw() const;

	/**
	 * @brief Accessor for the main Ogre::Camera instance
	 * @return The main ogre camera instance.
	 */
	Ogre::Camera& getCamera();

	/**
	 * @brief Accessor for the main Ogre::Camera instance
	 * @return The main ogre camera instance.
	 */
	Ogre::Camera& getCamera() const;

	/**
	 * @brief Returns the current camera orientation in the world.
	 * @param onlyHorizontal whether only the horizontal orientation should be returned.
	 * @return The world orientation of the camera.
	 */
	virtual const Ogre::Quaternion& getOrientation(bool onlyHorizontal = true) const;


	/**
	 *  @brief Returns the position of the camera in the world.
	 * @return The world position of the camera.
	 */
	const Ogre::Vector3& getPosition() const;


	/**
	 *    @brief Sets the mode of the camera (for example first or third person).
	 * @param mode The new mode of the camera.
	 */
	void setMode(Mode mode);

	/**
	 * @brief Sets the node to which the camera is attached.
	 * @param sceneNode The node to which the camera should be attached.
	 */
	virtual void setAvatarNode(Ogre::SceneNode* sceneNode);

	/**
	* @brief Emitted when the camera moves.
	* The argument is the camera instance being moved.
	*/
	sigc::signal<void, Ogre::Camera&> MovedCamera;

	/**
	* @brief Emitted when the distance between the camera and the avatar has changed.
    * @param Ogre::Real The new distance, in world units.
	*/
	sigc::signal<void, Ogre::Real> EventChangedCameraDistance;

// 	int xPosition, int yPosition, Ogre::Real xRelativeMovement, Ogre::Real yRelativeMovement, Ogre::Real timeSinceLastMovement);

// 	void mouseMoved (Ogre::MouseEvent *e);
// 	void mouseDragged (Ogre::MouseEvent *e) {};

	/**
	 * @brief Starts an world pick action.
	 * whenever the user clicks the mouse in the world (i.e. not on any gui element) a "picking action" is performed. The outcome of this is dependent on both what the user picks, and what current mode Ember is.
	 * In order to determine what to do, Ember will iterate over its internal list of IWorldPickListener instances, calling each in turn for every object that was picked (typically a lot of different objects will be hit by the ray that we'll use to determine whether anything was picked). It's then up to these instances of IWorldPickListener to determine if something should be done.
	 * The standard case is probably that the user picks an entity in the world, and that the EntityWorldPickListener then acts on this. However, if you want any other behaviour, you can yourself provide an implementation of IWorldPickListener and then register it here, thus allowing you to handle other objects.
	 * @see IWorldPickListener
	 * @see pushWorldPickListener
	 * @param mouseX The relative horizontal mouse position for the picking action (0 is in the left corner, 1 in the right)
	 * @param mouseY The relative vertical mouse position for the picking action (0 is in the top, 1 is in the bottom)
	 * @param args The original mouse picker args
	 */
	void pickInWorld(Ogre::Real mouseX, Ogre::Real mouseY, const MousePickerArgs& args);

// 	void setClosestPickingDistance(Ogre::Real distance);
// 	Ogre::Real getClosestPickingDistance();

	/**
	 * @brief Maps the position of an entity in the world to a 2d screen position.
	 * This will return the 2d position of an entity, in relative terms where 0 is the left upper corner and 1 is the right lower corner.
	 * @param worldPos The world position to map to the screen.
	 * @param screenPos The screen position, in relative terms.
	 * @return True if the entity is within the current screen, else false (i.e. the world position is somewhere it can't be seen from the current camera).
	 */
	bool worldToScreen(const Ogre::Vector3& worldPos, Ogre::Vector2& screenPos);

	/**
	 * @brief Attaches the camera to the specified scene node.
	 * If the camera already is attached to another node, it will first be detached from it.
	 * @param toNode The node to attach to.
	 */
	void attach(Ogre::SceneNode* toNode);

	/**
	 * @brief Adjusts the camera for the terrain, so it doesn't dip below it.
	 * This is done by shooting a ray from the base node of the camera (around which the camera swivels if in third person mode) to the camera. if there's any terrain being hit, the camera will be moved closer to the base node.
	 * @return If the camera had to be adjusted for the terrain (i.e. moved closer to the base node) the method will return true.
	 */
	bool adjustForTerrain();


	/**
	 *    Reimplements the ConsoleObject::runCommand method
	 * @param command
	 * @param args
	 */
	virtual	void runCommand(const std::string &command, const std::string &args);


	/**
	 *    Sets the distance from the camera to the avatar.
	 * @param distance the new distance
	 */
	void setCameraDistance(Ogre::Real distance);

	/**
	 * Methods from Ogre::FrameListener
	 */
	bool frameStarted(const Ogre::FrameEvent& event);
	//bool frameEnded(const Ogre::FrameEvent& event);

	/**
	 * @brief Enables and disables a compositor by name.
	 * @param compositorName
	 * @param enable
	 */
	void enableCompositor(const std::string& compositorName, bool enable);


	/**
	 * @brief Adds a new world pick listener to the queue of listeners.
	 * @param worldPickListener The world pick listener which show start recive world pick events.
	 */
	void pushWorldPickListener(IWorldPickListener* worldPickListener);

	/**
	 * @brief Removes a world pick listener which previously has been added through pushWorldPickListener().
	 * @param worldPickListener The world pick listener which should be removed. If the listener haven't been added nothing will happen.
	 */
	void removeWorldPickListener(IWorldPickListener* worldPickListener);

	const Ember::ConsoleCommandWrapper SetCameraDistance;
	const Ember::ConsoleCommandWrapper ToggleRendermode;
	const Ember::ConsoleCommandWrapper ToggleFullscreen;
	const Ember::ConsoleCommandWrapper Screenshot;
	const Ember::ConsoleCommandWrapper Record;

	/**
	 * @brief Toggles between wireframe and solid render mode.
	 */
	void toggleRenderMode();

	/**
	 * Takes a screen shot and writes it to disk
	 */
	void takeScreenshot();

	/**
	 * @brief Gets the terrain cursor, used for efficiently getting the position of the terrain currently under the cursor.
	 * @return An object which can be used to get the terrain that intersects the terrain at the current cursor.
	 */
	AvatarTerrainCursor& getTerrainCursor();

	/**
	 * @brief Gets the root node, to which the whole camera mount is hinged on.
	 * @return The root node of the camera mount.
	 */
	Ogre::SceneNode* getRootNode() const;


protected:

	typedef std::deque<IWorldPickListener*> WorldPickListenersStore;
	WorldPickListenersStore mPickListeners;

	typedef std::vector<std::string> CompositorNameStore;

	Recorder mRecorder;

	CompositorNameStore mLoadedCompositors;

	Mode mMode;

	bool mIsAttached;

	/**
	If true, the camera is inverted in the y axis.
	*/
	bool mInvertCamera;

	/**
	Creates the rays needed for mouse picking and camera adjustment.
	*/
	void createRayQueries();


	/**
	 * creates all nodes needed for the camera
	 */
	void createNodesForCamera();

	const std::string _takeScreenshot();

	void createViewPort();

	Ogre::Camera& mCamera;
	Ogre::SceneNode* mAvatarNode;
	Ogre::SceneManager& mSceneManager;
	//Ogre::Quaternion mLastOrientationOfTheCamera;

	Ogre::SceneNode* mAvatarCameraRootNode;
	Ogre::SceneNode* mAvatarCameraPitchNode;
	Ogre::SceneNode* mAvatarCameraNode;

	Ogre::Degree mDegreeOfPitchPerSecond;
	Ogre::Degree mDegreeOfYawPerSecond;

	Ogre::Degree degreePitch;
	Ogre::Degree degreeYaw;
	Ogre::RenderWindow& mWindow;
	Ogre::Viewport* mViewPort;

	//in meters how far we can pick objects
	Ogre::Real mClosestPickingDistance;

	Ogre::Vector3 mLastPosition;
	Ogre::Real mWantedCameraDistance, mCurrentCameraDistance;

	Ogre::RaySceneQuery *mAdjustTerrainRaySceneQuery, *mCameraRaySceneQuery;
	Ogre::Ray mAdjustTerrainRay;

	bool mIsAdjustedToTerrain;
	std::auto_ptr<AvatarTerrainCursor> mAvatarTerrainCursor;

	void Input_MouseMoved(const Ember::MouseMotion& motion, Ember::Input::InputMode mode);

	void ConfigService_EventChangedConfigItem(const std::string& section, const std::string& key);

	void updateValuesFromConfig();

	/**
	 *    Internal method for setting the camera distance.
	 * @param distance the new distance
	 */
	void _setCameraDistance(Ogre::Real distance);

	/**
	 * @brief Call this whenever you've moved any node which will make the derived position or orientation of the camera change.
	 * We need to mark the parent node of the camera as dirty. The update of the derived orientation and position of the node should normally occur when the scene tree is traversed, but in some instances we need to access the derived position or orientataion of the camera before the traversal occurs, and if we don't mark the node as dirty it won't be updated
	 */
	void markCameraNodeAsDirty();

};


///inline implementations

	inline const Ogre::Degree& AvatarCamera::getPitch() const
	{
		return degreePitch;
	}

	inline const Ogre::Degree& AvatarCamera::getYaw() const
	{
		return degreeYaw;
	}

	inline Ogre::Camera& AvatarCamera::getCamera()  {
		return mCamera;
	}
	inline Ogre::Camera& AvatarCamera::getCamera() const {
		return mCamera;
	}

	inline AvatarTerrainCursor& AvatarCamera::getTerrainCursor()
	{
		return *mAvatarTerrainCursor.get();
	}

	inline Ogre::SceneNode* AvatarCamera::getRootNode() const
	{
		return mAvatarCameraRootNode;
	}



class ICameraMount
{
	virtual ~ICameraMount() {};

	/**
	 * Pitches the camera the supplied degrees
	 */
	virtual void pitch(Ogre::Degree degrees) = 0;

	/**
	 * Yaws the camera the supplied degrees
	 */
	virtual void yaw(Ogre::Degree degrees) = 0;

	/**
	 * returns the current degrees of pitch from the cameras initial position
	 */
	virtual const Ogre::Degree& getPitch() const = 0;

	/**
	 * returns the current degrees of yaw from the cameras initial position
	 */
	virtual const Ogre::Degree& getYaw() const = 0;

	/**
	 * Returns the current camera orientation in the world
	 */
	virtual const Ogre::Quaternion& getOrientation(bool onlyHorizontal = true) const = 0;

	/**
	 *    Returns the position of the camera in the world.
	 * @return
	 */
	virtual const Ogre::Vector3& getPosition() const = 0;

	void setMode(AvatarCamera::Mode mode);

	/**
	 * sets the node to which the camera is attached
	 */
	virtual void setAvatarNode(Ogre::SceneNode* sceneNode);

};

class MainCamera
{
	virtual ~MainCamera() {}

	/**
	 * returns a pointer to the Ogre::Camera instance
	 */
	Ogre::Camera* getCamera();
	Ogre::Camera* getCamera() const;

	/**
	* emitted when the camra moves
	*/
	sigc::signal<void, Ogre::Camera*> MovedCamera;


	void pickInWorld(Ogre::Real mouseX, Ogre::Real mouseY, const MousePickerArgs& args);

	void setClosestPickingDistance(Ogre::Real distance);
	Ogre::Real getClosestPickingDistance();

	/**
	 * returns true if the worldPos is on screen, putting the screen pos into the x & y of the second Vector3
	 * returns false if the worldPos is off screen
	 * @param worldPos
	 * @param screenPos
	 * @return
	 */
	bool worldToScreen(const Ogre::Vector3& worldPos, Ogre::Vector3& screenPos);

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

};

class AvatarCameraMount : ICameraMount
{
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

};

class FreeFlyingMount : ICameraMount
{
};



}

#endif // AVATARCAMERA_H
