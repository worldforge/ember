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

#include <Ogre.h>
namespace DimeOgre {

class Avatar;

class AvatarCamera 
//: public Ogre::FrameListener 
{
public:

	AvatarCamera(Ogre::SceneNode* avatarNode, Ogre::SceneManager* sceneManager, Ogre::RenderWindow* window);
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
	virtual void pitch(Ogre::Real degrees);
	
	/*
	 * Yaws the camera the supplied degrees
	 */
	virtual void yaw(Ogre::Real degrees);
	
	/*
	 * returns the current degrees of pitch from the cameras initial position
	 */
	Ogre::Real getPitch()
	{
		return degreePitch;
	}
	
	/*
	 * returns the current degrees of yaw from the cameras initial position
	 */
	Ogre::Real getYaw()
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
	
protected:

	/*
	 * creates all nodes needed and the camera 
	 */
	void createNodesAndCamera();
	
	void createViewPort();
	
	
	Ogre::Camera* mCamera;
	Ogre::SceneNode* mAvatarNode;
	Ogre::SceneManager* mSceneManager;
	//Ogre::Quaternion mLastOrientationOfTheCamera;

	Ogre::SceneNode* mAvatarCameraRootNode;
	Ogre::SceneNode* mAvatarCameraPitchNode;
	Ogre::SceneNode* mAvatarCameraNode;

	Ogre::Real degreePitch;	
	Ogre::Real degreeYaw;	
	Ogre::RenderWindow* mWindow;
	Ogre::Viewport* mViewPort;
};

}

#endif // AVATARCAMERA_H
