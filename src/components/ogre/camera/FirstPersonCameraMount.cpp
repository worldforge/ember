/*
 Copyright (C) 2009 Erik Hjortsberg <erik.hjortsberg@gmail.com>

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

#include "components/ogre/OgreInfo.h"

#include <OgreSceneManager.h>
#include <OgreSceneNode.h>

namespace EmberOgre
{
namespace Camera
{

FirstPersonCameraMount::FirstPersonCameraMount(Ogre::SceneManager& sceneManager) :
	mDegreeOfPitchPerSecond(50), mDegreeOfYawPerSecond(50), mCameraNode(0), mInvertCamera(false)
{
	mCameraNode = sceneManager.createSceneNode(OgreInfo::createUniqueResourceName("FirstPersonCameraNode"));
}

FirstPersonCameraMount::~FirstPersonCameraMount()
{
	if (mCameraNode) {
		mCameraNode->getCreator()->destroySceneNode(mCameraNode);
	}
}


void FirstPersonCameraMount::attachToNode(Ogre::Node* sceneNode)
{
	if (sceneNode == mCameraNode->getParentSceneNode()) {
		return;
	}
	if (mCameraNode->getParentSceneNode()) {
		//first detach from our current node
		mCameraNode->getParentSceneNode()->removeChild(mCameraNode);
	}
	if (sceneNode) {
		sceneNode->addChild(mCameraNode);
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
	Ogre::Degree degrees(mDegreeOfPitchPerSecond * relativeMovement);

	if (mInvertCamera) {
		degrees -= degrees * 2;
	}

	///prevent the camera from being turned upside down
	const Ogre::Quaternion& orientation(mCameraNode->getOrientation());
	Ogre::Degree pitch(orientation.getPitch());
	if ((pitch.valueDegrees() + degrees.valueDegrees()) > 0) {
		degrees = std::min<float>(degrees.valueDegrees(), 90 - pitch.valueDegrees());
	} else {
		degrees = std::max<float>(degrees.valueDegrees(), -90 - pitch.valueDegrees());
	}

	if (degrees.valueDegrees()) {
		mCameraNode->pitch(degrees);

		///We need to manually update the node here to make sure that the derived orientation and position of the camera is updated.
		mCameraNode->_update(true, false);
	}

	//Return true if the current pitch has changed.
	return degrees;
}

Ogre::Degree FirstPersonCameraMount::yaw(float relativeMovement)
{
	Ogre::Degree degrees(mDegreeOfYawPerSecond * relativeMovement);

	if (degrees.valueDegrees()) {
		//Yaw in relation to the world to prevent the camera being tilted when it's yawed along with being pitched
		mCameraNode->yaw(degrees, Ogre::Node::TS_WORLD);

		///We need to manually update the node here to make sure that the derived orientation and position of the camera is updated.
		mCameraNode->_update(true, false);
	}
	return degrees;
}

}
}
