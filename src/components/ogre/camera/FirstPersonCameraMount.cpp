/*
 Copyright (C) 2009 Erik Ogenvik <erik@ogenvik.org>

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

#include "FirstPersonCameraMount.h"

#include "CameraSettings.h"

#include "components/ogre/OgreInfo.h"

#include <OgreSceneManager.h>

#include <wfmath/const.h>

namespace Ember
{
namespace OgreView
{
namespace Camera
{

FirstPersonCameraMount::FirstPersonCameraMount(const CameraSettings& cameraSettings, Ogre::SceneManager& sceneManager) :
	CameraMountBase(cameraSettings), mCameraNode(0)
{
	mCameraRootNode = sceneManager.createSceneNode(OgreInfo::createUniqueResourceName("FirstPersonCameraNodeRootNode"));
	mCameraRootNode->setInheritOrientation(false);
	//rotate to sync with WF world
	mCameraRootNode->rotate(Ogre::Vector3::UNIT_Y, Ogre::Degree(-90));

	mCameraPitchNode = mCameraRootNode->createChildSceneNode(OgreInfo::createUniqueResourceName("FirstPersonCameraPitchNode"));
	mCameraNode = mCameraPitchNode->createChildSceneNode(OgreInfo::createUniqueResourceName("FirstPersonCameraNode"));

}

FirstPersonCameraMount::~FirstPersonCameraMount()
{
	if (mCameraRootNode) {
		//This will handle the mCameraPitchNode and the mCameraNode too.
		mCameraRootNode->removeAndDestroyAllChildren();
		mCameraRootNode->getCreator()->destroySceneNode(mCameraRootNode);
	}
}


void FirstPersonCameraMount::attachToNode(Ogre::Node* sceneNode)
{
	if (sceneNode == mCameraRootNode->getParentSceneNode()) {
		return;
	}
	if (mCameraRootNode->getParentSceneNode()) {
		//first detach from our current node
		mCameraRootNode->getParentSceneNode()->removeChild(mCameraRootNode);
	}
	if (sceneNode) {
		sceneNode->addChild(mCameraRootNode);
	}
}

void FirstPersonCameraMount::attachToCamera(MainCamera& camera)
{
	CameraMountBase::attachToCamera(camera);
	mCameraNode->attachObject(mCamera);
}

void FirstPersonCameraMount::detachFromCamera()
{
	mCameraNode->detachObject(mCamera);
	CameraMountBase::detachFromCamera();
}

Ogre::Degree FirstPersonCameraMount::pitch(float relativeMovement)
{
	Ogre::Degree degrees(mCameraSettings.getDegreesPerMouseUnit() * relativeMovement);

	if (mCameraSettings.getInvertCamera()) {
		degrees -= degrees * 2;
	}

	//prevent the camera from being turned upside down
	const Ogre::Quaternion& orientation(mCameraPitchNode->getOrientation());
	Ogre::Degree pitch(orientation.getPitch());
	if ((pitch.valueDegrees() + degrees.valueDegrees()) > 0) {
		degrees = std::min<float>(degrees.valueDegrees(), 90 - pitch.valueDegrees());
	} else {
		degrees = std::max<float>(degrees.valueDegrees(), -90 - pitch.valueDegrees());
	}

	if (!WFMath::Equal(.0f, degrees.valueDegrees())) {
		mCameraPitchNode->pitch(degrees);
		mCameraPitchNode->_update(true, false);
	}

	//Return true if the current pitch has changed.
	return degrees;
}

Ogre::Degree FirstPersonCameraMount::yaw(float relativeMovement)
{
	Ogre::Degree degrees(mCameraSettings.getDegreesPerMouseUnit() * relativeMovement);

	if (!WFMath::Equal(.0f, degrees.valueDegrees())) {
		//Yaw in relation to the world to prevent the camera being tilted when it's yawed along with being pitched
		mCameraRootNode->yaw(degrees);

		//We need to manually update the node here to make sure that the derived orientation and position of the camera is updated.
		mCameraRootNode->_update(true, false);
	}
	return degrees;
}

}
}
}
