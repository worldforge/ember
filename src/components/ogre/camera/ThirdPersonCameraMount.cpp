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

namespace Ember {
namespace OgreView {
namespace Camera {

ThirdPersonCameraMount::ThirdPersonCameraMount(const CameraSettings& cameraSettings, Scene& scene, Terrain::ITerrainAdapter& terrainAdapter) :
		CameraMountBase(cameraSettings),
		SetCameraDistance("setcameradistance", this, "Set the distance of the camera."),
		mScene(scene),
		mSceneManager(scene.getSceneManager()),
		mCameraRootNode(nullptr),
		mCameraPitchNode(nullptr),
		mCameraNode(nullptr),
		mWantedCameraDistance(10),
		mCurrentCameraDistance(0),
		mIsAdjustedToTerrain(true),
		mConfigListenerContainer(new ConfigListenerContainer()),
		mTerrainAdapter(terrainAdapter) {
	createNodesForCamera(mSceneManager);
	mConfigListenerContainer->registerConfigListenerWithDefaults("input", "adjusttoterrain",
																 [&](const std::string& section, const std::string& key, const varconf::Variable& variable) {
																	 if (variable.is_bool()) {
																		 mIsAdjustedToTerrain = static_cast<bool>(variable);
																	 }
																 }, true);


}

ThirdPersonCameraMount::~ThirdPersonCameraMount() {
	if (mCameraRootNode) {
		//This will handle the mCameraPitchNode and the mCameraNode too.
		mCameraRootNode->removeAndDestroyAllChildren();
		mCameraRootNode->getCreator()->destroySceneNode(mCameraRootNode);
	}
	delete mConfigListenerContainer;
}

void ThirdPersonCameraMount::attachToNode(Ogre::Node* sceneNode) {
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

Ogre::Degree ThirdPersonCameraMount::pitch(float relativeMovement) {
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

Ogre::Degree ThirdPersonCameraMount::yaw(float relativeMovement) {
	Ogre::Degree degrees(mCameraSettings.getDegreesPerMouseUnit() * relativeMovement);

	if (!WFMath::Equal(.0f, degrees.valueDegrees())) {
		mCameraRootNode->yaw(degrees);

		//We need to manually update the node here to make sure that the derived orientation and position of the camera is updated.
		mCameraRootNode->_update(true, false);
	}
	return degrees;
}

void ThirdPersonCameraMount::createNodesForCamera(Ogre::SceneManager& sceneManager) {
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

	//The camera node isn't actually attached to the pitch node. We'll instead adjust it each frame to match the pitch node, adjusted by collisions.
	mCameraNode = sceneManager.createSceneNode(OgreInfo::createUniqueResourceName("ThirdPersonCameraNode"));
	mCameraNode->setInheritScale(false);
	mCameraNode->setFixedYawAxis(true); //Important that we set this to disallow the camera from rolling.

	setCameraDistance(10);
	Ogre::Vector3 pos(0, 0, 10);
	mCameraNode->setPosition(pos);

}

void ThirdPersonCameraMount::setCameraDistance(Ogre::Real distance) {
	mWantedCameraDistance = distance;
}

void ThirdPersonCameraMount::attachToCamera(MainCamera& camera) {
	CameraMountBase::attachToCamera(camera);
	mCameraNode->attachObject(mCamera);
	adjustForOcclusion();
}

void ThirdPersonCameraMount::detachFromCamera() {
	mCameraNode->detachObject(mCamera);
	CameraMountBase::detachFromCamera();
}

void ThirdPersonCameraMount::runCommand(const std::string& command, const std::string& args) {
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

void ThirdPersonCameraMount::adjustForOcclusion() {

	//Make sure all position are up to date.
	mCameraRootNode->_update(true, true);

	Ogre::Vector3 direction = mCameraPitchNode->_getDerivedOrientation() * Ogre::Vector3::UNIT_Z;

	auto fromPos = mCameraRootNode->_getDerivedPosition();
	auto toPos = fromPos + (direction * mWantedCameraDistance);

	if (mIsAdjustedToTerrain) {
		float distance = mWantedCameraDistance;

		float terrainHeight;

		if (mTerrainAdapter.getHeightAt(toPos.x, toPos.z, terrainHeight)) {
			terrainHeight += 0.4f;
			if (terrainHeight > toPos.y) {
				toPos.y = terrainHeight;
			}
		}


		btVector3 from(fromPos.x, fromPos.y, fromPos.z);
		btVector3 to(toPos.x, toPos.y, toPos.z);

		btCollisionWorld::AllHitsRayResultCallback callback(from, to);
		mScene.getBulletWorld().getCollisionWorld().rayTest(from, to, callback);

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
						toPos = Ogre::Vector3(callback.m_hitPointWorld[i].x(), callback.m_hitPointWorld[i].y(), callback.m_hitPointWorld[i].z());
					}
				}
			}
		}
	}

	mCameraNode->setPosition(toPos);
	mCameraNode->lookAt(mCameraRootNode->_getDerivedPosition(), Ogre::Node::TS_WORLD);
}

void ThirdPersonCameraMount::update() {
	adjustForOcclusion();
}

}
}
}
