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

#if SIGC_MAJOR_VERSION == 1 && SIGC_MINOR_VERSION == 0
#include <sigc++/signal_system.h>
#else
#include <sigc++/object.h>
#include <sigc++/signal.h>
#include <sigc++/slot.h>
#include <sigc++/bind.h>
#include <sigc++/object_slot.h>
#endif

#include "GUIManager.h"
#include "input/Input.h"
// #include "jesus/JesusMousePicker.h"

namespace EmberOgre {

class Avatar;
class InputManager;
class GUIManager;
class EmberEntity;

class AvatarCamera 
:virtual public SigC::Object 
//: public Ogre::FrameListener 
{
public:

	AvatarCamera(Ogre::SceneNode* avatarNode, Ogre::SceneManager* sceneManager, Ogre::RenderWindow* window, GUIManager* guiManager);
	virtual ~AvatarCamera();

	/*
	 * Methods from Ogre::FrameListener
	 *
	bool frameStarted(const Ogre::FrameEvent& event);
	bool frameEnded(const Ogre::FrameEvent& event);
	*/
	
	/*
	 * Pitches the camera the supplied degrees
	 */
	virtual void pitch(Ogre::Degree degrees);
	
	/*
	 * Yaws the camera the supplied degrees
	 */
	virtual void yaw(Ogre::Degree degrees);
	
	/*
	 * returns the current degrees of pitch from the cameras initial position
	 */
	Ogre::Degree getPitch()
	{
		return degreePitch;
	}
	
	/*
	 * returns the current degrees of yaw from the cameras initial position
	 */
	Ogre::Degree getYaw()
	{
		return degreeYaw;
	}
	
	/*
	 * returns a pointer to the Ogre::Camera instance 
	 */
	virtual Ogre::Camera* getCamera()  {
		return mCamera;	
	}
	
	/*
	 * Returns the current camera orientation in the world
	 */
	virtual Ogre::Quaternion getOrienation(bool onlyHorizontal = true) const {
		if (onlyHorizontal) {
			return mAvatarCameraRootNode->getWorldOrientation();
		} else {
			return mAvatarCameraPitchNode->getWorldOrientation();
		}
	}
	
	
	/*
	 * sets the node to which the camera is attached
	 */
	virtual void setAvatarNode(Ogre::SceneNode* sceneNode);
	
	SigC::Signal1<void, Ogre::Camera*> MovedCamera;
	
	void mouseMoved(const MouseMotion& motion, bool isInGuimode);
// 	int xPosition, int yPosition, Ogre::Real xRelativeMovement, Ogre::Real yRelativeMovement, Ogre::Real timeSinceLastMovement);

// 	void mouseMoved (Ogre::MouseEvent *e);
// 	void mouseDragged (Ogre::MouseEvent *e) {};

	EmberEntity* pickAnEntity(Ogre::Real mouseX, Ogre::Real mouseY);
	std::vector<Ogre::RaySceneQueryResultEntry> AvatarCamera::pickObject(Ogre::Real mouseX, Ogre::Real mouseY, std::vector<Ogre::UserDefinedObject*> exclude, unsigned long querymask );

	inline void setClosestPickingDistance(Ogre::Real distance) { mClosestPickingDistance = distance; }
	inline Ogre::Real getClosestPickingDistance() { return mClosestPickingDistance; }

	// worldToScreenPos 
	// 
	// returns true if the worldPos is on screen, putting the screen pos into the x & y of the 
	// second Vector2 
	// returns false if the worldPos is off screen 
	// 
	bool worldToScreen(Ogre::Vector3& worldPos, Ogre::Vector3& screenPos);

	
protected:

	/*
	 * creates all nodes needed and the camera 
	 */
	void createNodesAndCamera();
	
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

};

}

#endif // AVATARCAMERA_H
