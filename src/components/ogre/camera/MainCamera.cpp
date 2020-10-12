/*
 Copyright (C) 2004  Erik Ogenvik

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
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "MainCamera.h"

#include "ICameraMount.h"
#include "CameraSettings.h"

#include "components/ogre/AvatarTerrainCursor.h"

#include "components/ogre/Convert.h"
#include "components/ogre/ICameraMotionHandler.h"
#include "components/ogre/IMovementProvider.h"
#include "components/ogre/Scene.h"

#include "components/ogre/terrain/ITerrainAdapter.h"

#include "framework/Tokeniser.h"

#include <OgreRoot.h>
#include <OgreRenderWindow.h>
#include <OgreCompositorManager.h>
#include <OgreCompositorInstance.h>
#include <OgreCompositionTargetPass.h>
#include <OgreCompositionPass.h>
#include <OgreCompositor.h>
#include <OgreCamera.h>
#include <OgreTechnique.h>
#include <OgreSceneNode.h>
#include <OgreInstancedEntity.h>

#include <components/ogre/model/ModelDefinitionManager.h>
#include <components/ogre/model/Model.h>
#include <components/ogre/BulletCollisionDetector.h>
#include <components/ogre/EntityCollisionInfo.h>

#ifdef _WIN32
#include "platform/platform_windows.h"
#endif

using namespace Ember;
namespace Ember {
namespace OgreView {

namespace Camera {

MainCamera::MainCamera(Scene& scene, Ogre::RenderWindow& window, Input& input, Terrain::ITerrainAdapter& terrainAdapter) :
		mScene(scene),
		mCameraMount(nullptr),
		mWindow(window),
		mClosestPickingDistance(10000),
		mCameraRaySceneQuery(nullptr),
		mAvatarTerrainCursor(new AvatarTerrainCursor(scene.getMainCamera(), terrainAdapter)),
		mCameraOrientationChangedThisFrame(false),
		mMovementProvider(nullptr),
		mCameraSettings(new CameraSettings),
		mConfigListenerContainer(new ConfigListenerContainer()),
		mTerrainAdapter(terrainAdapter) {

	scene.getMainCamera().setAutoAspectRatio(true);

	createRayQueries(scene.getSceneManager());

	// Register this as a frame listener
	Ogre::Root::getSingleton().addFrameListener(this);

	input.EventMouseMoved.connect(sigc::mem_fun(*this, &MainCamera::Input_MouseMoved));

	mConfigListenerContainer->registerConfigListenerWithDefaults("graphics", "clipdistances", sigc::mem_fun(*this, &MainCamera::Config_ClipDistances), "0.5 1000");
	mConfigListenerContainer->registerConfigListenerWithDefaults("graphics", "compositors", sigc::mem_fun(*this, &MainCamera::Config_Compositors), "");

}

MainCamera::~MainCamera() {
	if (mCameraRaySceneQuery) {
		mScene.getSceneManager().destroyQuery(mCameraRaySceneQuery);
	}
	Ogre::Root::getSingleton().removeFrameListener(this);
	Ogre::CompositorManager::getSingleton().removeCompositorChain(mWindow.getViewport(0));
}

Ogre::Camera& MainCamera::getCamera() {
	return mScene.getMainCamera();
}

Ogre::Camera& MainCamera::getCamera() const {
	return mScene.getMainCamera();
}

AvatarTerrainCursor& MainCamera::getTerrainCursor() const {
	return *mAvatarTerrainCursor;
}

const CameraSettings& MainCamera::getCameraSettings() const {
	return *mCameraSettings;
}

void MainCamera::Config_ClipDistances(const std::string& /*section*/, const std::string& /*key*/, varconf::Variable& variable) {
	if (variable.is_string()) {
		Tokeniser tokeniser(variable.as_string());
		float nearDistance = std::stof(tokeniser.nextToken());
		float farDistance = std::stof(tokeniser.nextToken());

		S_LOG_INFO("Setting main camera clip distances to near: " << nearDistance << " far: " << farDistance);

		mScene.getMainCamera().setNearClipDistance(nearDistance);

		//set the far clip distance high to make sure that the sky is completely shown
		//if (Ogre::Root::getSingleton().getRenderSystem()->getCapabilities()->hasCapability(Ogre::RSC_INFINITE_FAR_PLANE)) {
		//	//NOTE: this won't currently work with the sky
		//	mCamera.setFarClipDistance(0);
		//} else {
		mScene.getMainCamera().setFarClipDistance(farDistance);
		//}
	}
}

void MainCamera::Config_Compositors(const std::string& /*section*/, const std::string& /*key*/, varconf::Variable& variable) {
	if (variable.is_string()) {
		const std::vector<std::string> tokens = Tokeniser::split(variable.as_string(), ",");

		for (const auto& token : tokens) {
			enableCompositor(token, true);
		}
		std::vector<std::string> compositorsToDisable;
		for (auto& compositor : mLoadedCompositors) {
			if (std::find(tokens.begin(), tokens.end(), compositor) == tokens.end()) {
				compositorsToDisable.push_back(compositor);
			}
		}
		for (auto& compositor : compositorsToDisable) {
			enableCompositor(compositor, false);
		}
	}
}

const Ogre::Quaternion& MainCamera::getOrientation(bool onlyHorizontal) const {
	if (!onlyHorizontal) {
		return mScene.getMainCamera().getRealOrientation();
	} else {
		static Ogre::Quaternion quat;
		quat = mScene.getMainCamera().getRealOrientation();
		quat.x = 0;
		quat.z = 0;
		return quat;
	}
}

const Ogre::Vector3& MainCamera::getPosition() const {
	return mScene.getMainCamera().getDerivedPosition();
}

void MainCamera::markCameraNodeAsDirty() {
	if (mScene.getMainCamera().getParentNode()) {
		//We need to mark the parent node of the camera as dirty. The update of the derived orientation and position of the node should normally occur when the scene tree is traversed, but in some instances we need to access the derived position or orientataion of the camera before the traversal occurs, and if we don't mark the node as dirty it won't be updated
		mScene.getMainCamera().getParentNode()->needUpdate(true);
	}
}

void MainCamera::pickInWorld(Ogre::Real mouseX, Ogre::Real mouseY, const MousePickerArgs& mousePickerArgs) {
	//Handle different pick requests differently.
	//For mouse press and hover events, we want the listeners to perform a line check to see what's being picked.
	//For all other pressed we however want the listeners to act on the previous results.
	//The reason is that we want the user to have a great fidelity in picking, and even though there might be a short delay
	//between pressing the mouse button and getting a selection menu, we want to present the items which were under the
	//mouse at the time the mouse button was pressed.
	if (mousePickerArgs.pickType == MPT_PRESS || mousePickerArgs.pickType == MPT_HOVER || mousePickerArgs.pickType == MPT_SELECT) {
		// First, calculate the ray between camera and screen coordinates
		Ogre::Ray cameraRay = getCamera().getCameraToViewportRay(mouseX, mouseY);
		bool continuePicking = true;

		decltype(mPickListeners) participatingListeners;
		for (auto listener : mPickListeners) {
			bool willParticipate = false;
			listener->initializePickingContext(willParticipate, mousePickerArgs);
			if (willParticipate) {
				participatingListeners.emplace_back(listener);
			}
		}

		//Only perform picking if there are any participating pick listeners.
		if (!participatingListeners.empty()) {
			auto results = pick(cameraRay, mousePickerArgs.distance);

			for (auto& result : results) {
				for (auto listener : participatingListeners) {
					listener->processPickResult(continuePicking, result, cameraRay, mousePickerArgs);
					if (!continuePicking) {
						break;
					}
				}
			}

			for (auto listener : participatingListeners) {
				listener->endPickingContext(mousePickerArgs);
			}
		}
	} else {
		for (auto listener : mPickListeners) {
			listener->processDelayedPick(mousePickerArgs);
		}
	}
}


std::vector<PickResult> MainCamera::pick(const Ogre::Ray& cameraRay, float distance) const {
	std::vector<PickResult> results;

	auto& origin = cameraRay.getOrigin();
	btVector3 rayFrom(origin.x, origin.y, origin.z);
	auto to = cameraRay.getPoint(distance);
	btVector3 rayTo(to.x, to.y, to.z);

	btCollisionWorld::AllHitsRayResultCallback callback(rayFrom, rayTo);
	//Only get those that are pickable
	callback.m_collisionFilterMask = COLLISION_MASK_PICKABLE;
	mScene.getBulletWorld().getCollisionWorld().rayTest(rayFrom, rayTo, callback);

	std::set<const btCollisionObject*> collidedObjects;
	for (int i = callback.m_collisionObjects.size() - 1; i >= 0; --i) {
		if (collidedObjects.find(callback.m_collisionObjects[i]) == collidedObjects.end()) {
			PickResult result{};
			auto collisionDetector = static_cast<BulletCollisionDetector*>(callback.m_collisionObjects[i]->getUserPointer());
			result.collisionInfo = collisionDetector->collisionInfo;
			result.point = WFMath::Point<3>(callback.m_hitPointWorld[i].x(), callback.m_hitPointWorld[i].y(), callback.m_hitPointWorld[i].z());
			result.distance = rayFrom.distance(btVector3(result.point.x(), result.point.y(), result.point.z()));
			auto insertion_iterator = std::lower_bound(results.begin(), results.end(), result);
			results.insert(insertion_iterator, std::move(result));
			collidedObjects.insert(callback.m_collisionObjects[i]);
		}
	}


	// Manually add results for terrain since they have to be queried in a different way
	auto terrainIntersectionResult = mTerrainAdapter.rayIntersects(cameraRay);

	if (terrainIntersectionResult.first) {

		PickResult terrainResult{};

		terrainResult.point = WFMath::Point<3>(terrainIntersectionResult.second.x, terrainIntersectionResult.second.y, terrainIntersectionResult.second.z);
		terrainResult.collisionInfo = EntityCollisionInfo{terrainIntersectionResult.first, false};
		terrainResult.distance = rayFrom.distance(btVector3(terrainResult.point.x(), terrainResult.point.y(), terrainResult.point.z()));

		// Insert at correct position because the listeners expect results sorted by distance
		auto insertion_iterator = std::lower_bound(results.begin(), results.end(), terrainResult);
		results.insert(insertion_iterator, terrainResult);
	}

	return results;
}

void MainCamera::setClosestPickingDistance(Ogre::Real distance) {
	mClosestPickingDistance = distance;
}

Ogre::Real MainCamera::getClosestPickingDistance() const {
	return mClosestPickingDistance;
}

void MainCamera::setMovementProvider(IMovementProvider* movementProvider) {
	mMovementProvider = movementProvider;
}

bool MainCamera::worldToScreen(const Ogre::Vector3& worldPos, Ogre::Vector2& screenPos) {
	auto cameraRelativePos = mScene.getMainCamera().getViewMatrix() * worldPos;
	if (cameraRelativePos.z >= 0) {
		return false;
	}
	Ogre::Vector3 hcsPosition = mScene.getMainCamera().getProjectionMatrix() * cameraRelativePos;

	if ((hcsPosition.x < -1.0f) || (hcsPosition.x > 1.0f) || (hcsPosition.y < -1.0f) || (hcsPosition.y > 1.0f)) {
		return false;
	}

	screenPos.x = (hcsPosition.x + 1) * 0.5f;
	screenPos.y = (-hcsPosition.y + 1) * 0.5f;

	return true;
}

Ogre::Vector2 MainCamera::worldToScreen(Ogre::Camera& camera, const Ogre::Vector3& worldPos) {
	auto cameraRelativePos = camera.getViewMatrix() * worldPos;
	if (cameraRelativePos.z >= 0) {
		return {NAN, NAN};
	}
	Ogre::Vector3 hcsPosition = camera.getProjectionMatrix() * cameraRelativePos;

	if ((hcsPosition.x < -1.0f) || (hcsPosition.x > 1.0f) || (hcsPosition.y < -1.0f) || (hcsPosition.y > 1.0f)) {
		return {NAN, NAN};
	}

	Ogre::Vector2 screenPos;
	screenPos.x = (hcsPosition.x + 1) * 0.5f;
	screenPos.y = (-hcsPosition.y + 1) * 0.5f;

	return screenPos;
}

bool MainCamera::frameStarted(const Ogre::FrameEvent& event) {
	if (mCameraMount) {
		if (mMovementProvider) {
			WFMath::Vector<3> movement = mMovementProvider->getMovementForCurrentFrame();
			if (movement != WFMath::Vector<3>::ZERO() || mCameraOrientationChangedThisFrame) {
				mCameraMount->move(movement, Convert::toWF(getOrientation()), event.timeSinceLastFrame);
			}
		} else {
			mCameraMount->move(WFMath::Vector<3>::ZERO(), Convert::toWF(getOrientation()), event.timeSinceLastFrame);
		}
		mCameraOrientationChangedThisFrame = false;
		mCameraMount->update();
	}
	return true;
}

void MainCamera::Input_MouseMoved(const MouseMotion& motion, Input::InputMode mode) {
	if (mCameraMount) {
		if (mode == Input::IM_MOVEMENT) {

			bool moved = false;
			if (motion.xRelativeMovement != 0) {
				moved = mCameraMount->yaw(motion.xRelativeMovement).valueDegrees() != 0;
			}
			if (motion.yRelativeMovement != 0) {
				moved = mCameraMount->pitch(motion.yRelativeMovement).valueDegrees() != 0 || moved;
			}

			if (moved) {
				markCameraNodeAsDirty();
				mCameraOrientationChangedThisFrame = true;
				MovedCamera.emit(mScene.getMainCamera());
			}
		}
	}
}

void MainCamera::enableCompositor(const std::string& compositorName, bool enable) {
	if (enable) {
		S_LOG_INFO("Enabling compositor '" << compositorName << "'.");
	} else {
		S_LOG_INFO("Disabling compositor '" << compositorName << "'.");
	}
	if (std::find(mLoadedCompositors.begin(), mLoadedCompositors.end(), compositorName) == mLoadedCompositors.end()) {
		Ogre::CompositorInstance* compositor = Ogre::CompositorManager::getSingleton().addCompositor(mWindow.getViewport(0), compositorName);
		if (compositor) {
			//There's a bug in Ogre which will causes a segfault during rendering if a compositor has an invalid shader.
			//We therefore need to check for this here, and disable the shader if so is the case.
			compositor->getCompositor()->load();
			bool hasErrors = !validateCompositionTargetPass(*compositor->getTechnique()->getOutputTargetPass());
			if (!hasErrors) {
				auto& passes = compositor->getTechnique()->getTargetPasses();
				for (auto& pass : passes) {
					hasErrors = !validateCompositionTargetPass(*pass);
					if (hasErrors) {
						break;
					}
				}
			}
			if (hasErrors) {
				S_LOG_FAILURE("Compositor " << compositorName << " has errors and will be disabled.");
				Ogre::CompositorManager::getSingleton().removeCompositor(mWindow.getViewport(0), compositorName);
			} else {
				compositor->setEnabled(true);
				mLoadedCompositors.push_back(compositorName);
			}
		}
	} else {
		Ogre::CompositorManager::getSingleton().setCompositorEnabled(mWindow.getViewport(0), compositorName, enable);
	}
}

bool MainCamera::validateCompositionTargetPass(Ogre::CompositionTargetPass& compositionPass) {
	for (auto& compositorPass : compositionPass.getPasses()) {
		compositorPass->getMaterial()->load();


		for (auto* technique : compositorPass->getMaterial()->getSupportedTechniques()) {
			for (auto* pass : technique->getPasses()) {
				//Also disallow camera polygon mode override. This is because if it's turned on,
				//and the camera is switched to polygon mode, the end result will be one single
				//large polygon being shown. This is not what we want.
				pass->setPolygonModeOverrideable(false);
				if (pass->hasFragmentProgram()) {
					if (pass->getFragmentProgram()->hasCompileError()) {
						return false;
					}
				}
			}
		}
	}
	return true;
}

void MainCamera::pushWorldPickListener(IWorldPickListener* worldPickListener) {
	mPickListeners.push_front(worldPickListener);

}

void MainCamera::removeWorldPickListener(IWorldPickListener* worldPickListener) {
	if (worldPickListener) {
		auto I = std::find(mPickListeners.begin(), mPickListeners.end(), worldPickListener);
		if (I != mPickListeners.end()) {
			mPickListeners.erase(I);
		}
	}
}

void MainCamera::createRayQueries(Ogre::SceneManager& sceneManager) {
	mCameraRaySceneQuery = sceneManager.createRayQuery(Ogre::Ray());
	mCameraRaySceneQuery->setSortByDistance(true);
}

ICameraMount* MainCamera::attachToMount(ICameraMount* newCameraMount) {
	if (mCameraMount) {
		mCameraMount->detachFromCamera();
	}
	mCameraMount = newCameraMount;
	if (mCameraMount) {
		mCameraMount->attachToCamera(*this);
	}
	return mCameraMount;
}


}

}
}
