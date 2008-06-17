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
#include "AvatarTerrainCursor.h"

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
	inline const Ogre::Degree& getPitch() const;
	
	/**
	 * returns the current degrees of yaw from the cameras initial position
	 */
	inline const Ogre::Degree& getYaw() const;
	
	/**
	 * returns a pointer to the Ogre::Camera instance 
	 */
	inline Ogre::Camera* getCamera();
	inline Ogre::Camera* getCamera() const;
	
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

	/**
	 * returns true if the worldPos is on screen, putting the screen pos into the x & y of the second Vector3 
	 * returns false if the worldPos is off screen 
	 * @param worldPos 
	 * @param screenPos 
	 * @return 
	 */
	bool worldToScreen(const Ogre::Vector3& worldPos, Ogre::Vector3& screenPos);
	
	/**
	 *    Attaches the camera to the specified scene node.
	 * @param toNode 
	 */
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
	
	
	/**
	 *    Adds a new world pick listener to the queue of listeners.
	 * @param worldPickListener 
	 */
	void pushWorldPickListener(IWorldPickListener* worldPickListener);
	
	const Ember::ConsoleCommandWrapper SetCameraDistance;
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
	GUIManager* mGUIManager;
	
	AvatarTerrainCursor mAvatarTerrainCursor;
	
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

	Ogre::Vector3 mLastPosition;
	Ogre::Real mWantedCameraDistance, mCurrentCameraDistance;
	
	Ogre::RaySceneQuery *mAdjustTerrainRaySceneQuery, *mCameraRaySceneQuery;
	Ogre::Ray mAdjustTerrainRay;
	
	bool mIsAdjustedToTerrain;

	void Input_MouseMoved(const MouseMotion& motion, Input::InputMode mode);

	void ConfigService_EventChangedConfigItem(const std::string& section, const std::string& key);
	
	void updateValuesFromConfig();

	/**
	 *    Internal method for setting the camera distance.
	 * @param distance the new distance
	 */
	void _setCameraDistance(Ogre::Real distance);

};


///inline implementations

	const Ogre::Degree& AvatarCamera::getPitch() const
	{
		return degreePitch;
	}
	
	const Ogre::Degree& AvatarCamera::getYaw() const
	{
		return degreeYaw;
	}
	
	Ogre::Camera* AvatarCamera::getCamera()  {
		return mCamera;	
	}
	Ogre::Camera* AvatarCamera::getCamera() const {
		return mCamera;	
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
	inline Ogre::Camera* getCamera();
	inline Ogre::Camera* getCamera() const;
	
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
