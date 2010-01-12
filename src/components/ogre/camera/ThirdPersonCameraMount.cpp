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

#include "ThirdPersonCameraMount.h"
#include "components/ogre/OgreInfo.h"
#include "framework/Tokeniser.h"
#include "services/EmberServices.h"
#include "services/config/ConfigService.h"
#include <OgreSceneNode.h>
#include <OgreSceneManager.h>

namespace EmberOgre
{
namespace Camera
{

ThirdPersonCameraMount::ThirdPersonCameraMount(Ogre::SceneManager& sceneManager) :
	SetCameraDistance("setcameradistance", this, "Set the distance of the camera."), mCameraRootNode(0), mCameraPitchNode(0), mCameraNode(0), mDegreeOfPitchPerSecond(50), mDegreeOfYawPerSecond(50), mWantedCameraDistance(10), mCurrentCameraDistance(0), mAdjustTerrainRaySceneQuery(0), mCameraRaySceneQuery(0), mIsAdjustedToTerrain(true), mInvertCamera(false)
{
	createNodesForCamera(sceneManager);
	createRayQueries(sceneManager);
}

void ThirdPersonCameraMount::attachToNode(Ogre::Node* sceneNode)
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

Ogre::Degree ThirdPersonCameraMount::pitch(float relativeMovement)
{
	Ogre::Degree degrees(mDegreeOfPitchPerSecond * relativeMovement);

	if (mInvertCamera) {
		degrees -= degrees * 2;
	}

	///prevent the camera from being turned upside down
	const Ogre::Quaternion& orientation(mCameraPitchNode->getOrientation());
	Ogre::Degree pitch(orientation.getPitch());
	if ((pitch.valueDegrees() + degrees.valueDegrees()) > 0) {
		degrees = std::min<float>(degrees.valueDegrees(), 90 - pitch.valueDegrees());
	}
	else {
		degrees = std::max<float>(degrees.valueDegrees(), -90 - pitch.valueDegrees());
	}

	if (degrees.valueDegrees()) {
		mCameraPitchNode->pitch(degrees);

		///We need to manually update the node here to make sure that the derived orientation and position of the camera is updated.
		mCameraPitchNode->_update(true, false);
	}

	//Return true if the current pitch has changed.
	return degrees;
}

Ogre::Degree ThirdPersonCameraMount::yaw(float relativeMovement)
{
	Ogre::Degree degrees(mDegreeOfYawPerSecond * relativeMovement);

	if (degrees.valueDegrees()) {
		mCameraRootNode->yaw(degrees);

		///We need to manually update the node here to make sure that the derived orientation and position of the camera is updated.
		mCameraRootNode->_update(true, false);
	}
	return degrees;
}

void ThirdPersonCameraMount::createNodesForCamera(Ogre::SceneManager& sceneManager)
{
	mCameraRootNode = sceneManager.createSceneNode(OgreInfo::createUniqueResourceName("ThirdPersonCameraRootNode"));
	mCameraRootNode->setInheritOrientation(false);
	//we need to adjust for the height of the  mesh
	mCameraRootNode->setPosition(Ogre::Vector3(0, 2, 0));
	//rotate to sync with WF world
	mCameraRootNode->rotate(Ogre::Vector3::UNIT_Y, Ogre::Degree(-90));

	mCameraPitchNode = mCameraRootNode->createChildSceneNode(OgreInfo::createUniqueResourceName("ThirdPersonCameraPitchNode"));
	mCameraPitchNode->setPosition(Ogre::Vector3(0, 0, 0));
	mCameraNode = mCameraPitchNode->createChildSceneNode(OgreInfo::createUniqueResourceName("ThirdPersonCameraNode"));
	setCameraDistance(10);

}

void ThirdPersonCameraMount::setCameraDistance(Ogre::Real distance)
{
	mWantedCameraDistance = distance;
	_setCameraDistance(distance);
}

void ThirdPersonCameraMount::_setCameraDistance(Ogre::Real distance)
{
	mCurrentCameraDistance = distance;
	Ogre::Vector3 pos(0, 0, distance);
	mCameraNode->setPosition(pos);
	if (mCamera && mCamera->getParentNode()) {
		///We need to mark the parent node of the camera as dirty. The update of the derived orientation and position of the node should normally occur when the scene tree is traversed, but in some instances we need to access the derived position or orientataion of the camera before the traversal occurs, and if we don't mark the node as dirty it won't be updated
		mCamera->getParentNode()->needUpdate(true);
	}
	EventChangedCameraDistance.emit(distance);
}

void ThirdPersonCameraMount::createRayQueries(Ogre::SceneManager& sceneManager)
{
	// attempt to create a query to get back terrain coords
	mAdjustTerrainRaySceneQuery = sceneManager.createRayQuery(mAdjustTerrainRay, Ogre::SceneManager::WORLD_GEOMETRY_TYPE_MASK);
	///only test for terrain
	mAdjustTerrainRaySceneQuery->setWorldFragmentType(Ogre::SceneQuery::WFT_SINGLE_INTERSECTION);
	mAdjustTerrainRaySceneQuery->setSortByDistance(true);
	mAdjustTerrainRaySceneQuery->setQueryTypeMask(Ogre::SceneManager::WORLD_GEOMETRY_TYPE_MASK);
}

void ThirdPersonCameraMount::attachToCamera(MainCamera& camera)
{
	CameraMountBase::attachToCamera(camera);
	mCameraNode->attachObject(mCamera);
}

void ThirdPersonCameraMount::detachFromCamera()
{
	mCameraNode->detachObject(mCamera);
	CameraMountBase::detachFromCamera();
}

void ThirdPersonCameraMount::runCommand(const std::string &command, const std::string &args)
{
	if (SetCameraDistance == command) {
		Ember::Tokeniser tokeniser;
		tokeniser.initTokens(args);
		std::string distance = tokeniser.nextToken();
		if (distance != "") {
			float fDistance = Ogre::StringConverter::parseReal(distance);
			setCameraDistance(fDistance);
		}
	}
}
}
}
