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
	mSceneMgr = sceneManager; // TODO: assert it's not null

	// Create the Avatar tree of nodes, with a base entity
	// and attach all the needed cameras

	// The avatar itself
	mAvatarNode = dynamic_cast<Ogre::SceneNode*>(mSceneMgr->getRootSceneNode()->createChild());
	mAvatarNode->setPosition(0,0,0);
	//mAvatarNode->setOrientation(0,0,0,1);
	mAvatarNode->setScale(0.01,0.01,0.01);
	// TODO: very important! When the scale is set to default (1,1,1)
	// and a mesh with normal scale is put here (like 2 meters high and things like that)
	// we'll need to rescale the local space position for the nodes (shown below)
	// - will be like dividing by 100 or something
	mAvatarEntity = mSceneMgr->createEntity("AvatarEntity", "robot.mesh");
	mAvatarNode->attachObject(mAvatarEntity);

	// 1st person, 3rd person and Top camera nodes
	mAvatar1pCameraNode = dynamic_cast<Ogre::SceneNode*>(mAvatarNode->createChild("Avatar1pCameraNode"));
	mAvatar1pCameraNode->setPosition(0,8500,0);
	mAvatar3pCameraNode = dynamic_cast<Ogre::SceneNode*>(mAvatar1pCameraNode->createChild("Avatar3pCameraNode"));
	Ogre::Vector3 pos = Ogre::Vector3(-15000,2500,0);
	//mAvatar3pCameraNode->setPosition(15000,2500,0);
	mAvatar3pCameraNode->setPosition(pos);
	mAvatarTopCameraNode = dynamic_cast<Ogre::SceneNode*>(mAvatarNode->createChild("AvatarTopCameraNode"));
	mAvatarTopCameraNode->setPosition(2,100000,0);

	// TODO: put all this code into "createAvatar" or something similar

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
	mAvatar3pCamera->setAutoTracking(true, mAvatar1pCameraNode);
	mAvatar3pCamera->setNearClipDistance(1);

	// Create the Top camera
	mAvatarTopCamera = mSceneMgr->createCamera("AvatarTopCamera");
	mAvatarTopCameraNode->attachObject(mAvatarTopCamera);
	// Look to the Avatar's head
	mAvatarTopCamera->setAutoTracking(true, mAvatarNode);   // TODO: make this (And the others) a "following" camera
	mAvatarTopCamera->setNearClipDistance(1);
}

Avatar::~Avatar()
{
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
