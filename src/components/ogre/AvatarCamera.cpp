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

#include "Avatar.h"
#include "AvatarCamera.h"

namespace DimeOgre {

AvatarCamera::AvatarCamera(Ogre::SceneNode* avatarNode, Ogre::SceneManager* sceneManager, Ogre::RenderWindow* window) :
	mAvatarNode(avatarNode),
	mSceneManager(sceneManager),
	mWindow(window),
	degreePitch(0),
	degreeYaw(0),
	mViewPort(0)
//	mLastOrientationOfTheCamera(avatar->getOrientation())
{
	createNodesAndCamera();
}

AvatarCamera::~AvatarCamera()
{}

/*bool AvatarCamera::frameStarted(const Ogre::FrameEvent& event)
{
	*
	 * M.O. is to check if we're aligned behind the avatar.
	 * If not, we'll try to ease ourself into the position.
	 *
	SceneNode* sceneNode = static_cast<SceneNode*>(mCamera->getParentNode());
	//mCameraNode->setOrientation(Ogre::Quaternion::Slerp(0.2, mCameraNode->getOrientation(), mLastOrientationOfTheCamera, false);
	//mLastOrientationOfTheCamera = mCameraNode->getOrientation();
	
	return true;	
}
bool AvatarCamera::frameEnded(const Ogre::FrameEvent& event)
{
	return true;
}
*/


void AvatarCamera::createNodesAndCamera()
{
	//create the nodes for the camera
	
	mAvatarCameraRootNode = static_cast<Ogre::SceneNode*>(mAvatarNode->createChild("AvatarCameraRootNode"));
	//we need to adjust for the height of the avatar mesh
	mAvatarCameraRootNode->setPosition(WF2OGRE_VECTOR3(0,100,-1));
	//rotate to sync with WF world
    mAvatarCameraRootNode->rotate(Ogre::Vector3::UNIT_Y,-90);

	mAvatarCameraPitchNode = static_cast<Ogre::SceneNode*>(mAvatarCameraRootNode->createChild("AvatarCameraPitchNode"));
	mAvatarCameraPitchNode->setPosition(WF2OGRE_VECTOR3(0,0,0));
	mAvatarCameraNode = static_cast<Ogre::SceneNode*>(mAvatarCameraPitchNode->createChild("AvatarCameraPitchNode"));
	Ogre::Vector3 pos = WF2OGRE_VECTOR3(0,0,500);
	mAvatarCameraNode->setPosition(pos);
	
	mCamera = mSceneManager->createCamera("AvatarCamera");
	mAvatarCameraNode->attachObject(mCamera);
	// Look to the Avatar's head
	//mAvatar3pCamera->setAutoTracking(true, mAvatar1pCameraNode);
	mCamera->setNearClipDistance(WF2OGRE(0.01));
	mCamera->setFarClipDistance( WF2OGRE(384) * 100);
	
	createViewPort();
}

void AvatarCamera::createViewPort()
{
	assert(mCamera);
	assert(!mViewPort);
    // Create 1st person viewport, entire window
    mViewPort = mWindow->addViewport(mCamera);
    mViewPort->setBackgroundColour(Ogre::ColourValue(0,0,0));
	
}

void AvatarCamera::setAvatarNode(Ogre::SceneNode* sceneNode)
{
	mAvatarNode = sceneNode;
	mAvatarNode->addChild(mAvatarCameraRootNode);
}


void AvatarCamera::pitch(Ogre::Real degrees)
{
	degreePitch += degrees;
	mAvatarCameraPitchNode->pitch(degrees);
}
void AvatarCamera::yaw(Ogre::Real degrees)
{
	degreeYaw += degrees;
	mAvatarCameraRootNode->yaw(degrees);
	
}

}




