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

#ifndef AVATARCONTROLLER_H
#define AVATARCONTROLLER_H

#include "EmberOgrePrerequisites.h"


// #include <SDL.h>
#include <sigc++/trackable.h>

#include "services/input/Input.h"
#include "services/input/InputCommandMapper.h"
#include "framework/ConsoleObject.h"
#include "EntityWorldPickListener.h"
#include <OgreVector3.h>
#include <OgreQuaternion.h>
#include <OgreFrameListener.h>



namespace EmberOgre {
class Avatar;
class EmberEntity;
class AvatarCamera;

class GUIManager;

class InputManager;
class AvatarController;

/**
The movement mode of the avatar, run or walk.
*/
class AvatarMovementMode {
public:
	enum Mode
	{
		MM_WALK = 0,
		MM_RUN = 1
	};
};


/**
Used for sending the current desired movement to the actual avatar.
*/
struct AvatarControllerMovement
{
	AvatarControllerMovement();

	float rotationDegHoriz;
	float rotationDegVert;
	Ogre::Real timeSlice;
	Ogre::Vector3 movementDirection;
	AvatarMovementMode::Mode mode;
	bool isMoving;
	Ogre::Quaternion cameraOrientation;
};

/**
Listens for left mouse button pressed in movement mode and moves the character forward.
*/
class AvatarControllerInputListener
{
public:
	AvatarControllerInputListener(AvatarController& controller);

protected:

	void input_MouseButtonPressed(Ember::Input::MouseButton button, Ember::Input::InputMode mode);
	void input_MouseButtonReleased(Ember::Input::MouseButton button, Ember::Input::InputMode mode);
	AvatarController& mController;
};

/**
Controls the avatar. All avatar movement is handled by an instance of this class.
*/
class AvatarController
: public Ogre::FrameListener,
public sigc::trackable,
public Ember::ConsoleObject
{
public:
    friend class AvatarControllerInputListener;

    AvatarController(Avatar& avatar, Ogre::RenderWindow& window, GUIManager& guiManager, Ogre::Camera& camera);

	virtual ~AvatarController();

	/**
	Each frame we check if we should update the avatar.
	*/
	virtual bool frameStarted(const Ogre::FrameEvent & event);


	/**
	Emitted when the movement mode changes between run and walk.
	*/
	sigc::signal<void, AvatarMovementMode::Mode> EventMovementModeChanged;



	void createAvatarCameras(Ogre::SceneNode* avatarSceneNode);

	/**
	 * Gets the AvatarCamera.
	 * @return
	 */
	AvatarCamera* getAvatarCamera() const;

	/**
	 *    Detaches the camera from the avatar and attaches it to the free flying node.
	 */
	void detachCamera();

	/**
	 *    Attaches the camera to the avatar.
	 */
	void attachCamera();

	/**
	 *    Gets the current movement for this frame.
	 * @return
	 */
	const AvatarControllerMovement& getCurrentMovement() const;

	const Ember::ConsoleCommandWrapper RunToggle;
	const Ember::ConsoleCommandWrapper ToggleCameraAttached;

	const Ember::ConsoleCommandWrapper CharacterMoveForward;
	const Ember::ConsoleCommandWrapper CharacterMoveBackward;
	const Ember::ConsoleCommandWrapper CharacterMoveDownwards;
	const Ember::ConsoleCommandWrapper CharacterMoveUpwards;
	const Ember::ConsoleCommandWrapper CharacterStrafeLeft;
	const Ember::ConsoleCommandWrapper CharacterStrafeRight;
/*	const Ember::ConsoleCommandWrapper CharacterRotateLeft;
	const Ember::ConsoleCommandWrapper CharacterRotateRight;*/

	const Ember::ConsoleCommandWrapper MoveCameraTo;


	/**
	 *    Reimplements the ConsoleObject::runCommand method
	 * @param command
	 * @param args
	 */
	virtual	void runCommand(const std::string &command, const std::string &args);

	/**
	Moves the avatar to the specified point.
	A terrain decal will be shown.
	*/
	void moveToPoint(const Ogre::Vector3& point);

	/**
	 *    Teleports the avatar to the specified point.
	 * @param point
	 * @param locationEntity
	 */
	void teleportTo(const Ogre::Vector3& point, EmberEntity* locationEntity);

protected:

	Ember::InputCommandMapper mMovementCommandMapper;

	Ogre::RenderWindow& mWindow;

	GUIManager& mGUIManager;


// 	void checkMovementKeys(const Ogre::FrameEvent & event, const Input& input);


	AvatarCamera* mAvatarCamera;
	Ogre::Camera& mCamera;


	/**
	 * Avatar
	 */
	Avatar& mAvatar;

    EmberEntity* mEntityUnderCursor;
    EmberEntity* mSelectedEntity;

    AvatarControllerMovement movementForFrame, mPreviousMovementForFrame;

	Ogre::SceneNode* mFreeFlyingCameraNode;
	bool mIsAttached;
	/**
	True if we're in running mode.
	*/
	bool mIsRunning;

	Ogre::Vector3 mMovementDirection;

	/**
	Listen for double clicks and send the avatar to the double clicked position.
	*/
	void entityPicker_PickedEntity(const EntityPickResult& result, const MousePickerArgs& args);

	/**
	Creates the terrain decal needed for displaying where the avatar is heading.
	*/
	void createDecal(Ogre::Vector3 position);

	/**
	A decal object for showing a decal on the terrain when the user uses the "move to here" functionality.
	The decal will be shown at the destination, and removed when the user either gets close to it, or aborts the "move to here" movement (for example by moving manually).
	*/
	Ogre::MovableObject* mDecalObject;

	/**
	The scene node to which the decal object for showing the destination of a "move to here" movement operation is attached.
	*/
	Ogre::SceneNode* mDecalNode;

	/**
	Controller for making the decal pulsate a little.
	@note Not used currently.
	*/
// 	Ogre::WaveformControllerFunction* mPulsatingController;

	AvatarControllerInputListener mControllerInputListener;
};



}

#endif // AvatarController_H
