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
 along with this program; if not, write to the Free Software Foundation,
 Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#include "ThirdPersonCameraMount.h"
#include "CameraSettings.h"
#include "components/ogre/Scene.h"
#include "components/ogre/OgreInfo.h"
#include "components/ogre/EntityCollisionInfo.h"
#include "components/ogre/terrain/ITerrainAdapter.h"
#include "framework/Tokeniser.h"
#include <OgreSceneNode.h>
#include <OgreSceneManager.h>
#include <OgreRoot.h>
#include <OgreCamera.h>
#include <OgreEntity.h>

#include <wfmath/const.h>
#include <components/ogre/MousePicker.h>
#include <components/ogre/BulletCollisionDetector.h>

#include <Eris/View.h>
#include <Eris/Avatar.h>
#include <Eris/TypeInfo.h>

namespace Ember
{
namespace OgreView
{
namespace Camera
{

ThirdPersonCameraMount::ThirdPersonCameraMount(const CameraSettings& cameraSettings, Scene& scene, Terrain::ITerrainAdapter& terrainAdapter) :
	CameraMountBase(cameraSettings),
	SetCameraDistance("setcameradistance", this, "Set the distance of the camera."),
	mScene(scene),
	mSceneManager(scene.getSceneManager()),
	mCameraRootNode(nullptr),
	mCameraPitchNode(nullptr),
	mCameraNode(nullptr),
	mLastPosition(Ogre::Vector3::ZERO),
	mWantedCameraDistance(10),
	mCurrentCameraDistance(0),
	mIsAdjustedToTerrain(true),
	mConfigListenerContainer(new ConfigListenerContainer()),
	mTerrainAdapter(terrainAdapter)
{
	createNodesForCamera(mSceneManager);
	mConfigListenerContainer->registerConfigListenerWithDefaults("input", "adjusttoterrain", sigc::mem_fun(*this, &ThirdPersonCameraMount::Config_AdjustToTerrain), true);

	mCameraRaySceneQuery = mSceneManager.createRayQuery(mAdjustTerrainRay);
}

ThirdPersonCameraMount::~ThirdPersonCameraMount()
{
	if (mCameraRootNode) {
		//This will handle the mCameraPitchNode and the mCameraNode too.
		mCameraRootNode->removeAndDestroyAllChildren();
		mCameraRootNode->getCreator()->destroySceneNode(mCameraRootNode);
	}
	Ogre::Root::getSingleton().removeFrameListener(this);
	delete mConfigListenerContainer;
}

void ThirdPersonCameraMount::attachToNode(Ogre::Node* sceneNode)
{
	//If we're asked to reconnect to the scene node we're already connected to we should bail now.
	if (sceneNode == mCameraRootNode->getParentSceneNode()) {
		return;
	}
	if (mCameraRootNode->getParentSceneNode()) {
		//first detach from our current node
		mCameraRootNode->getParentSceneNode()->removeChild(mCameraRootNode);
	}
	//If a null pointer was submitted we should attach to the root scene node of the scene manager
	if (sceneNode) {
		sceneNode->addChild(mCameraRootNode);
	} else {
		mCameraRootNode->getCreator()->getRootSceneNode()->addChild(mCameraRootNode);
	}
}

Ogre::Degree ThirdPersonCameraMount::pitch(float relativeMovement)
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

		//We need to manually update the node here to make sure that the derived orientation and position of the camera is updated.
		mCameraPitchNode->_update(true, false);
	}

	//Return true if the current pitch has changed.
	return degrees;
}

Ogre::Degree ThirdPersonCameraMount::yaw(float relativeMovement)
{
	Ogre::Degree degrees(mCameraSettings.getDegreesPerMouseUnit() * relativeMovement);

	if (!WFMath::Equal(.0f, degrees.valueDegrees())) {
		mCameraRootNode->yaw(degrees);

		//We need to manually update the node here to make sure that the derived orientation and position of the camera is updated.
		mCameraRootNode->_update(true, false);
	}
	return degrees;
}

void ThirdPersonCameraMount::createNodesForCamera(Ogre::SceneManager& sceneManager)
{
	mCameraRootNode = sceneManager.createSceneNode(OgreInfo::createUniqueResourceName("ThirdPersonCameraRootNode"));
	mCameraRootNode->setInheritOrientation(false);
	mCameraRootNode->setInheritScale(false);
	//we need to adjust for the height of the  mesh
	mCameraRootNode->setPosition(Ogre::Vector3(0, 2, 0));
	//Start camera behind avatar
	mCameraRootNode->yaw(Ogre::Degree(180));

	mCameraPitchNode = mCameraRootNode->createChildSceneNode(OgreInfo::createUniqueResourceName("ThirdPersonCameraPitchNode"));
	mCameraPitchNode->setPosition(Ogre::Vector3(0, 0, 0));
	mCameraPitchNode->setInheritScale(false);
	mCameraNode = mCameraPitchNode->createChildSceneNode(OgreInfo::createUniqueResourceName("ThirdPersonCameraNode"));
	mCameraNode->setInheritScale(false);

	setCameraDistance(10);
	Ogre::Vector3 pos(0, 0, 10);
	mCameraNode->setPosition(pos);

}

void ThirdPersonCameraMount::setCameraDistance(Ogre::Real distance)
{
	mWantedCameraDistance = distance;
	setActualCameraDistance(distance);
}

void ThirdPersonCameraMount::setActualCameraDistance(Ogre::Real distance)
{
	mCurrentCameraDistance = distance;
	Ogre::Vector3 pos(0, 0, distance);
	mCameraNode->setPosition(pos);
	if (mCamera && mCamera->getParentNode()) {
		//We need to mark the parent node of the camera as dirty. The update of the derived orientation and position of the node should normally occur when the scene tree is traversed, but in some instances we need to access the derived position or orientataion of the camera before the traversal occurs, and if we don't mark the node as dirty it won't be updated
		mCamera->getParentNode()->needUpdate(true);
	}
	EventChangedCameraDistance.emit(distance);
}

void ThirdPersonCameraMount::attachToCamera(MainCamera& camera)
{
	CameraMountBase::attachToCamera(camera);
	Ogre::Root::getSingleton().addFrameListener(this);
	mCameraNode->attachObject(mCamera);
}

void ThirdPersonCameraMount::detachFromCamera()
{
	mCameraNode->detachObject(mCamera);
	Ogre::Root::getSingleton().removeFrameListener(this);
	CameraMountBase::detachFromCamera();
}

void ThirdPersonCameraMount::runCommand(const std::string &command, const std::string &args)
{
	if (SetCameraDistance == command) {
		Tokeniser tokeniser;
		tokeniser.initTokens(args);
		std::string distance = tokeniser.nextToken();
		if (!distance.empty()) {
			float fDistance = Ogre::StringConverter::parseReal(distance);
			setCameraDistance(fDistance);
		}
	}
}

bool ThirdPersonCameraMount::adjustForCollision()
{
	// We will shoot a ray from the camera base node to the camera. If it hits anything on the way we know that there's something between the camera and the avatar and we'll position the camera closer to the avatar. Thus we'll avoid having the camera dip below the terrain
	//For now we'll only check against the terrain
	const Ogre::Vector3 direction(-mCamera->getDerivedDirection());

	auto fromPos = mCameraRootNode->_getDerivedPosition();
	auto desiredPos = fromPos + (direction * mWantedCameraDistance);
	auto toPos = desiredPos;

	btVector3 from(fromPos.x, fromPos.y, fromPos.z);
	btVector3 to(toPos.x, toPos.y, toPos.z);

	btCollisionWorld::AllHitsRayResultCallback callback(from, to);
	mScene.getBulletWorld().getCollisionWorld().rayTest(from, to, callback);


	float distance = mWantedCameraDistance;

	for (int i = 0; i < callback.m_collisionObjects.size(); i++) {
		auto* collisionObject = callback.m_collisionObjects[i];
		auto* collisionDetector = static_cast<BulletCollisionDetector*>(collisionObject->getUserPointer());
		if (collisionDetector->collisionInfo.type() == typeid(EntityCollisionInfo)) {
			auto& entityCollisionInfo = boost::any_cast<EntityCollisionInfo&>(collisionDetector->collisionInfo);
			//Ignore the avatar entity.
			if (entityCollisionInfo.entity != entityCollisionInfo.entity->getView()->getAvatar()->getEntity()) {
				float hitDistance = from.distance(callback.m_hitPointWorld[i]);
				if (hitDistance < distance) {
					distance = hitDistance;
				}
			}
		}
	}


	//If the direction if pointing straight upwards we'll end up in an infinite loop in the ray query
	if (!WFMath::Equal(direction.z, 0)) {
		mAdjustTerrainRay.setDirection(direction);
		mAdjustTerrainRay.setOrigin(fromPos);

		auto result = mTerrainAdapter.rayIntersects(mAdjustTerrainRay);
		// If there is terrain between the camera base and the camera, decrease camera distance
		if (result.first) {
			Ogre::Real terrainDistance = fromPos.distance(result.second);
			if (terrainDistance < distance) {
				distance = terrainDistance * 0.9f;
			}
		}
	}

	setActualCameraDistance(distance);

	return false;
}

bool ThirdPersonCameraMount::frameStarted(const Ogre::FrameEvent&)
{
	if (mIsAdjustedToTerrain && mCamera) {
		if (mCamera->getDerivedPosition() != mLastPosition) {
			adjustForCollision();
			mLastPosition = mCamera->getDerivedPosition();
		}
	}
	return true;
}

void ThirdPersonCameraMount::Config_AdjustToTerrain(const std::string&, const std::string&, varconf::Variable& variable)
{
	if (variable.is_bool()) {
		mIsAdjustedToTerrain = static_cast<bool>(variable);
		//Reset if disabled.
		if (!mIsAdjustedToTerrain) {
			setActualCameraDistance(mWantedCameraDistance);
		}
	}
}

}
}
}
