/*
	Avatar.h by Miguel Guzman (Aglanor)
	Copyright (C) 2002 Miguel Guzman & The Worldforge

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

//TODO: reorder headers, etc


#ifdef HAVE_CONFIG_H
#include "config.h"
#endif
#include <Ogre.h>
/*
#include "services/server/ServerService.h"
#include "services/DimeServices.h"

#include "EntityListener.h"
#include <Ogre.h>

#include <Eris/PollDefault.h>
#include <Eris/Log.h>
#include <Eris/TypeInfo.h>

#include "MathConverter.h"
*/

#include "Avatar.h"


Avatar::Avatar()
{
}

Avatar::Avatar(Ogre::SceneManager* sceneManager)
{
	mWalkSpeed = WF2OGRE(2.0);
	mRunSpeed = WF2OGRE(5.0);

	mSceneMgr = sceneManager; // TODO: assert it's not null

	// Create the Avatar tree of nodes, with a base entity
	// and attach all the needed cameras
	
	createAvatar();
	createAvatarCameras();


}

Avatar::~Avatar()
{
}

void Avatar::createAvatar()
{
	// The avatar itself
	mAvatarNode = dynamic_cast<Ogre::SceneNode*>(mSceneMgr->getRootSceneNode()->createChild());
	mAvatarNode->setPosition(WF2OGRE_VECTOR3(0,0,5));
	//mAvatarNode->setOrientation(0,1,0,0);
	//mAvatarNode->setScale(OGRESCALER);
	// TODO: very important! When the scale is set to default (1,1,1)
	// and a mesh with normal scale is put here (like 2 meters high and things like that)
	// we'll need to rescale the local space position for the nodes (shown below)
	// - will be like dividing by 100 or something

	// Model Node and Entity for display
	// TODO: do also the scaling here! That way the other nodes can be positioned in their real places
	mAvatarModelNode = dynamic_cast<Ogre::SceneNode*>(mAvatarNode->createChild("AvatarModelNode"));
	mAvatarEntity = mSceneMgr->createEntity("AvatarEntity", "robot.mesh");
	mAvatarModelNode->attachObject(mAvatarEntity);
	mAvatarModelNode->rotate(Ogre::Vector3::UNIT_Y,90);

	//let's set the current animation state to walking
	mAnimStateWalk = mAvatarEntity->getAnimationState("Walk");	
	
	Ogre::ControllerManager* controllerManager = &Ogre::ControllerManager::getSingleton();
	mAvatarAnimationControllerFunction = new Ogre::AnimationControllerFunction(mAnimStateWalk->getLength());
	mAvatarAnimationController = controllerManager->createController(controllerManager->getFrameTimeSource(), mAnimStateWalk, mAvatarAnimationControllerFunction);
}

void Avatar::createAvatarCameras()
{
	// 1st person, 3rd person and Top camera nodes
	mAvatar1pCameraNode = dynamic_cast<Ogre::SceneNode*>(mAvatarNode->createChild("Avatar1pCameraNode"));
	mAvatar1pCameraNode->setPosition(WF2OGRE_VECTOR3(0,0.8,-0.1));
	//mAvatar1pCameraNode->showBoundingBox(true);
	mAvatar3pCameraNode = dynamic_cast<Ogre::SceneNode*>(mAvatar1pCameraNode->createChild("Avatar3pCameraNode"));
	Ogre::Vector3 pos = WF2OGRE_VECTOR3(0,0,5);
	//mAvatar3pCameraNode->setPosition(15000,2500,0);
	mAvatar3pCameraNode->setPosition(pos);
	mAvatarTopCameraNode = dynamic_cast<Ogre::SceneNode*>(mAvatarNode->createChild("AvatarTopCameraNode"));
	mAvatarTopCameraNode->setPosition(WF2OGRE_VECTOR3(2,200,0));

	// Create the camera
	mAvatar1pCamera = mSceneMgr->createCamera("Avatar1pCam");
	mAvatar1pCameraNode->attachObject(mAvatar1pCamera);

	// Look back along x axis
	//mAvatar1pCamera->lookAt(mOgreHeadNode->getPosition());
	mAvatar1pCamera->setNearClipDistance(1);

	// do not clip at far distance
	// so we can see the skydome
	//mCamera->setFarClipDistance( 384 );

	//3rd person camera
	mAvatar3pCamera = mSceneMgr->createCamera("Avatar3pCamera");
	mAvatar3pCameraNode->attachObject(mAvatar3pCamera);
	// Look to the Avatar's head
	//mAvatar3pCamera->setAutoTracking(true, mAvatar1pCameraNode);
	mAvatar3pCamera->setNearClipDistance(1);

	// Create the Top camera
	mAvatarTopCamera = mSceneMgr->createCamera("AvatarTopCamera");
	mAvatarTopCameraNode->attachObject(mAvatarTopCamera);
	// Look to the Avatar's head
	mAvatarTopCamera->setAutoTracking(true, mAvatarNode);
	mAvatarTopCamera->setNearClipDistance(1);
	mAvatarTopCameraNode->rotate(Ogre::Vector3::UNIT_Y,-90.0);
	
}
void Avatar::attemptMove(Ogre::Vector3 move, bool isRunning)
{
	if (!mIsMoving) {
		fprintf(stderr, "TRACE - AVATAR START WALKING ANIMATION\n");
		mIsMoving = true;
		mAnimStateWalk->setEnabled(true);
	}
	mAvatarNode->translate(mAvatarNode->getOrientation()* (move* (isRunning ? mRunSpeed : mWalkSpeed)));
}

void Avatar::attemptStop()
{
	if (mIsMoving) {
		fprintf(stderr, "TRACE - AVATAR STOP WALKING ANIMATION\n");
		mIsMoving = false;
		mAnimStateWalk->setEnabled(false);
	}
}

void Avatar::attemptJump() {}


void Avatar::attemptRotate(float degHoriz, float degVert, Ogre::Real timeSlice)
{
	// rotate the Avatar Node only in X position (no vertical rotation)
	mAvatarNode->rotate(Ogre::Vector3::UNIT_Y,degHoriz);

	// pitch the 1p Camera
	//mAvatar1pCameraNode->yaw(degHoriz);
	mAvatar1pCameraNode->pitch(degVert);
	// TODO: rotate 3p cam and rotate *back* top camera

	// Rotate top camera *back*
	// The mini-map will always be in the same position
	// (Up is North)
	// TODO: study why do I need twice the angle!
	//mAvatarTopCameraNode->rotate(Ogre::Vector3::UNIT_Y,-degHoriz);
	//mAvatarTopCameraNode->rotate(Ogre::Vector3::UNIT_Y,-degHoriz);
}


Ogre::Camera* Avatar::getAvatar1pCamera()
{
	return mAvatar1pCamera;
}

Ogre::Camera* Avatar::getAvatar3pCamera()
{
	return mAvatar3pCamera;
}

Ogre::Camera* Avatar::getAvatarTopCamera()
{
	return mAvatarTopCamera;
}

void Avatar::enteredWorld(Eris::Entity *e)
{
	
	mAvatarNode->setPosition(WF2OGRE_VECTOR3(1,1,1) * Atlas2Ogre(e->getPosition()));
	mAvatarNode->setOrientation(Atlas2Ogre(e->getOrientation()));
}

