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

#ifndef MovementController_H
#define MovementController_H

#include "EmberOgrePrerequisites.h"
#include "EntityWorldPickListener.h"
#include "components/ogre/IMovementProvider.h"

#include "services/input/Input.h"
#include "services/input/InputCommandMapper.h"
#include "framework/ConsoleObject.h"
#include <OgreVector3.h>
#include <OgreQuaternion.h>
#include <OgreFrameListener.h>

#include <wfmath/vector.h>
#include <sigc++/trackable.h>


namespace EmberOgre {

namespace Camera {
class FirstPersonCameraMount;
class MainCamera;
}
class FreeFlyingCameraMotionHandler;
class Avatar;
class EmberEntity;

class GUIManager;

class InputManager;
class MovementController;

/**
The movement mode of the avatar, run or walk.
*/
class MovementControllerMode
{
public:
	enum Mode
	{
		MM_WALK = 0,
		MM_RUN = 1
	};
};

/**
Listens for left mouse button pressed in movement mode and moves the character forward.
*/
class MovementControllerInputListener : public virtual sigc::trackable
{
public:
	MovementControllerInputListener(MovementController& controller);

protected:

	void input_MouseButtonPressed(Ember::Input::MouseButton button, Ember::Input::InputMode mode);
	void input_MouseButtonReleased(Ember::Input::MouseButton button, Ember::Input::InputMode mode);
	MovementController& mController;
};

/**
Controls the avatar. All avatar movement is handled by an instance of this class.
*/
class MovementController
: public Ogre::FrameListener,
public sigc::trackable,
public Ember::ConsoleObject,
public IMovementProvider
{
public:
    friend class MovementControllerInputListener;

    /**
     * @brief Ctor.
     * @param avatar The main avatar.
     * @param camera The main camera.
     */
    MovementController(Avatar& avatar, Camera::MainCamera& camera);

	virtual ~MovementController();

	/**
	Each frame we check if we should update the avatar.
	*/
	virtual bool frameStarted(const Ogre::FrameEvent & event);


	/**
	Emitted when the movement mode changes between run and walk.
	*/
	sigc::signal<void, MovementControllerMode::Mode> EventMovementModeChanged;

	/**
	 *    Gets the current movement for this frame.
	 * @return
	 */
//	const MovementControllerMovement& getCurrentMovement() const;

	const Ember::ConsoleCommandWrapper RunToggle;
	const Ember::ConsoleCommandWrapper ToggleCameraAttached;

	const Ember::ConsoleCommandWrapper MovementMoveForward;
	const Ember::ConsoleCommandWrapper MovementMoveBackward;
	const Ember::ConsoleCommandWrapper MovementMoveDownwards;
	const Ember::ConsoleCommandWrapper MovementMoveUpwards;
	const Ember::ConsoleCommandWrapper MovementStrafeLeft;
	const Ember::ConsoleCommandWrapper MovementStrafeRight;
/*	const Ember::ConsoleCommandWrapper MovementRotateLeft;
	const Ember::ConsoleCommandWrapper MovementRotateRight;*/


//	const Ember::ConsoleCommandWrapper MoveCameraTo;

	/**
	 * @brief Repositions the free flying camera on the avatar.
	 * This command is useful when you want to find the avatar when in free flying mode, or if you're moving the avatar through teleporting and want the camera to follow.
	 */
	const Ember::ConsoleCommandWrapper CameraOnAvatar;

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


	virtual WFMath::Vector<3> getMovementForCurrentFrame() const;

	MovementControllerMode::Mode getMode() const;

	void setCameraFreeFlying(bool freeFlying);


protected:

	/**
	 * @brief The main camera.
	 */
	Camera::MainCamera& mCamera;

	Ember::InputCommandMapper mMovementCommandMapper;

	/**
	True if we're in running mode.
	*/
	bool mIsRunning;

	WFMath::Vector<3> mMovementDirection;

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

	MovementControllerInputListener mControllerInputListener;

	Avatar& mAvatar;

	Ogre::SceneNode* mFreeFlyingNode;
	std::auto_ptr<FreeFlyingCameraMotionHandler> mFreeFlyingMotionHandler;
	std::auto_ptr<Camera::FirstPersonCameraMount> mCameraMount;
	bool mIsFreeFlying;
};



}

#endif // MovementController_H
