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

#include "input/Input.h"
#include <stack>

namespace EmberOgre {

class Avatar;
class InputManager;
class GUIManager;
class EmberEntity;
struct MouseMotion;
struct EntityPickResult;
class IWorldPickListener;
class MousePickerArgs;

class AvatarCamera 
:
public sigc::trackable,
public Ember::ConsoleObject,
public Ogre::FrameListener
{
public:

	enum Mode {
		MODE_THIRD_PERSON = 1,
		MODE_FIRST_PERSON = 2
	};
	
	/**
	Command for setting the distance between the camera and the avatar.
	*/
	static const std::string SETCAMERADISTANCE;
	
	AvatarCamera(Ogre::SceneNode* avatarNode, Ogre::SceneManager* sceneManager, Ogre::RenderWindow* window, GUIManager* guiManager, Ogre::Camera* camera);
	virtual ~AvatarCamera();

	/**
	 * Pitches the camera the supplied degrees
	 */
	virtual void pitch(Ogre::Degree degrees);
	
	/**
	 * Yaws the camera the supplied degrees
	 */
	virtual void yaw(Ogre::Degree degrees);
	
	/**
	 * returns the current degrees of pitch from the cameras initial position
	 */
	const Ogre::Degree& getPitch() const
	{
		return degreePitch;
	}
	
	/**
	 * returns the current degrees of yaw from the cameras initial position
	 */
	const Ogre::Degree& getYaw() const
	{
		return degreeYaw;
	}
	
	/**
	 * returns a pointer to the Ogre::Camera instance 
	 */
	virtual Ogre::Camera* getCamera()  {
		return mCamera;	
	}
	virtual Ogre::Camera* getCamera() const {
		return mCamera;	
	}
	
	/**
	 * Returns the current camera orientation in the world
	 */
	virtual const Ogre::Quaternion& getOrientation(bool onlyHorizontal = true) const;
	
	
	/**
	 *    Returns the position of the camera in the world.
	 * @return 
	 */
	const Ogre::Vector3& getPosition() const;
	
	
	void setMode(Mode mode);
	
	/**
	 * sets the node to which the camera is attached
	 */
	virtual void setAvatarNode(Ogre::SceneNode* sceneNode);
	
	/**
	* emitted when the camra moves
	*/
	sigc::signal<void, Ogre::Camera*> MovedCamera;
	
	/**
	* emitted when the distance between the camera and the avatar has changed
    * @param Ogre::Real the new distance
	*/
	sigc::signal<void, Ogre::Real> EventChangedCameraDistance;
	
// 	int xPosition, int yPosition, Ogre::Real xRelativeMovement, Ogre::Real yRelativeMovement, Ogre::Real timeSinceLastMovement);

// 	void mouseMoved (Ogre::MouseEvent *e);
// 	void mouseDragged (Ogre::MouseEvent *e) {};

	void pickInWorld(Ogre::Real mouseX, Ogre::Real mouseY, const MousePickerArgs& args);
	
// 	EntityPickResult pickAnEntity(Ogre::Real mouseX, Ogre::Real mouseY);
// 	std::vector<Ogre::RaySceneQueryResultEntry> AvatarCamera::pickObject(Ogre::Real mouseX, Ogre::Real mouseY, std::vector<Ogre::UserDefinedObject*> exclude, unsigned long querymask );

	void setClosestPickingDistance(Ogre::Real distance);
	Ogre::Real getClosestPickingDistance();

	// worldToScreenPos 
	// 
	// returns true if the worldPos is on screen, putting the screen pos into the x & y of the 
	// second Vector2 
	// returns false if the worldPos is off screen 
	// 
	bool worldToScreen(const Ogre::Vector3& worldPos, Ogre::Vector3& screenPos);
	
	/**
	 *    picks a point in the terrain
	 * @param mouseX
	 * @param mouseY 
	 * @param resultVector 
	 * @return true if the picking was successful
	 */
// 	bool pickInTerrain(Ogre::Real mouseX, Ogre::Real mouseY, Ogre::Vector3& resultVector);
	
	void attach(Ogre::SceneNode* toNode);
	
	/**
	 *    Adjusts the camera for the terrain, so it doesn't dip below it.
	 * @return 
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
	 *    Enables and disables a compositor by name.
	 * @param compositorName 
	 * @param enable 
	 */
	void enableCompositor(const std::string& compositorName, bool enable);
	
	
	void pushWorldPickListener(IWorldPickListener* worldPickListener);
	
protected:

	typedef std::deque<IWorldPickListener*> WorldPickListenersStore;
	WorldPickListenersStore mPickListeners;
		
	typedef std::vector<std::string> CompositorNameStore;
	
	CompositorNameStore mLoadedCompositors;

	Mode mMode;
	
	bool mIsAttached;
	
	/**
	If true, the camera is inverted in the y axis.
	*/
	bool mInvertCamera;
	
	/**
	 * creates all nodes needed for the camera 
	 */
	void createNodesForCamera();
	
	void createViewPort();
	GUIManager* mGUIManager;
	
	
	Ogre::Camera* mCamera;
	Ogre::SceneNode* mAvatarNode;
	Ogre::SceneManager* mSceneManager;
	//Ogre::Quaternion mLastOrientationOfTheCamera;

	Ogre::SceneNode* mAvatarCameraRootNode;
	Ogre::SceneNode* mAvatarCameraPitchNode;
	Ogre::SceneNode* mAvatarCameraNode;

	Ogre::Degree mDegreeOfPitchPerSecond;
	Ogre::Degree mDegreeOfYawPerSecond;

	Ogre::Degree degreePitch;	
	Ogre::Degree degreeYaw;	
	Ogre::RenderWindow* mWindow;
	Ogre::Viewport* mViewPort;
	
	//in meters how far we can pick objects
	Ogre::Real mClosestPickingDistance;

	void Input_MouseMoved(const MouseMotion& motion, Input::InputMode mode);

	void ConfigService_EventChangedConfigItem(const std::string& section, const std::string& key);
	
	void updateValuesFromConfig();

};

}

#endif // AVATARCAMERA_H
