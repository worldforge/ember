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
#include "components/ogre/IWorldPickListener.h"

#include "services/config/ConfigListenerContainer.h"
#include "services/input/Input.h"

#include "framework/Tokeniser.h"

#include <OgreRoot.h>
#include <OgreRenderWindow.h>
#include <OgreCompositorManager.h>
#include <OgreCompositorInstance.h>
#include <OgreCompositionTechnique.h>
#include <OgreCompositionTargetPass.h>
#include <OgreCompositionPass.h>
#include <OgreRenderSystem.h>
#include <OgreRenderSystemCapabilities.h>
#include <OgreSceneQuery.h>

#include <OgreMath.h>
#include <OgreRay.h>
#include <OgreVector3.h>

#ifdef _WIN32
#include "platform/platform_windows.h"
#endif

using namespace Ember;
namespace Ember
{
namespace OgreView
{

namespace Camera
{

MainCamera::MainCamera(Ogre::SceneManager& sceneManager, Ogre::RenderWindow& window, Input& input, Ogre::Camera& camera) :
		mSceneManager(sceneManager), mCamera(camera), mCameraMount(0), mWindow(window), mClosestPickingDistance(10000), mCameraRaySceneQuery(0), mAvatarTerrainCursor(new AvatarTerrainCursor(camera)), mCameraOrientationChangedThisFrame(false), mMovementProvider(0), mCameraSettings(new CameraSettings), mConfigListenerContainer(new ConfigListenerContainer())
{

	mCamera.setAutoAspectRatio(true);

	createRayQueries(sceneManager);

	// Register this as a frame listener
	Ogre::Root::getSingleton().addFrameListener(this);

	input.EventMouseMoved.connect(sigc::mem_fun(*this, &MainCamera::Input_MouseMoved));

	mConfigListenerContainer->registerConfigListenerWithDefaults("graphics", "clipdistances", sigc::mem_fun(*this, &MainCamera::Config_ClipDistances), "0.5 1000");
	mConfigListenerContainer->registerConfigListenerWithDefaults("graphics", "compositors", sigc::mem_fun(*this, &MainCamera::Config_Compositors), "");

}

MainCamera::~MainCamera()
{
	if (mCameraRaySceneQuery) {
		mSceneManager.destroyQuery(mCameraRaySceneQuery);
	}
	Ogre::Root::getSingleton().removeFrameListener(this);
	delete mCameraSettings;
	delete mConfigListenerContainer;
	Ogre::CompositorManager::getSingleton().removeCompositorChain(mWindow.getViewport(0));
}

Ogre::Camera& MainCamera::getCamera()
{
	return mCamera;
}

Ogre::Camera& MainCamera::getCamera() const
{
	return mCamera;
}

AvatarTerrainCursor& MainCamera::getTerrainCursor() const
{
	return *mAvatarTerrainCursor.get();
}

const CameraSettings& MainCamera::getCameraSettings() const
{
	return *mCameraSettings;
}

void MainCamera::Config_ClipDistances(const std::string& section, const std::string& key, varconf::Variable& variable)
{
	if (variable.is_string()) {
		Tokeniser tokeniser(variable);
		float nearDistance = atof(tokeniser.nextToken().c_str());
		float farDistance = atof(tokeniser.nextToken().c_str());

		S_LOG_INFO("Setting main camera clip distances to near: " << nearDistance << " far: " << farDistance);

		mCamera.setNearClipDistance(nearDistance);

		//set the far clip distance high to make sure that the sky is completely shown
		if (Ogre::Root::getSingleton().getRenderSystem()->getCapabilities()->hasCapability(Ogre::RSC_INFINITE_FAR_PLANE)) {
			/*		//NOTE: this won't currently work with the sky
			 mCamera.setFarClipDistance(0);*/

			mCamera.setFarClipDistance(farDistance);
		} else {
			mCamera.setFarClipDistance(farDistance);
		}
	}
}

void MainCamera::Config_Compositors(const std::string& section, const std::string& key, varconf::Variable& variable)
{
	if (variable.is_string()) {
		const std::vector<std::string> tokens = Tokeniser::split(variable.as_string(), ",");

		for (std::vector<std::string>::const_iterator I = tokens.begin(); I != tokens.end(); ++I) {
			enableCompositor(*I, true);
		}
		std::vector<std::string> compositorsToDisable;
		for (CompositorNameStore::const_iterator I = mLoadedCompositors.begin(); I != mLoadedCompositors.end(); ++I) {
			if (std::find(tokens.begin(), tokens.end(), *I) == tokens.end()) {
				compositorsToDisable.push_back(*I);
			}
		}
		for (std::vector<std::string>::const_iterator I = compositorsToDisable.begin(); I != compositorsToDisable.end(); ++I) {
			enableCompositor(*I, false);
		}
	}
}

const Ogre::Quaternion& MainCamera::getOrientation(bool onlyHorizontal) const
{
	if (!onlyHorizontal) {
		return mCamera.getRealOrientation();
	} else {
		static Ogre::Quaternion quat;
		quat = mCamera.getRealOrientation();
		quat.x = 0;
		quat.z = 0;
		return quat;
	}
}

const Ogre::Vector3& MainCamera::getPosition() const
{
	return mCamera.getDerivedPosition();
}

void MainCamera::markCameraNodeAsDirty()
{
	if (mCamera.getParentNode()) {
		//We need to mark the parent node of the camera as dirty. The update of the derived orientation and position of the node should normally occur when the scene tree is traversed, but in some instances we need to access the derived position or orientataion of the camera before the traversal occurs, and if we don't mark the node as dirty it won't be updated
		mCamera.getParentNode()->needUpdate(true);
	}
}

void MainCamera::pickInWorld(Ogre::Real mouseX, Ogre::Real mouseY, const MousePickerArgs& mousePickerArgs)
{
	//Handle different pick requests differently.
	//For mouse press and hover events, we want the listeners to perform a line check to see what's being picked.
	//For all other pressed we however want the listeners to act on the previous results.
	//The reason is that we want the user to have a great fidelity in picking, and even though there might be a short delay
	//between pressing the mouse button and getting a selection menu, we want to present the items which were under the
	//mouse at the time the mouse button was pressed.
	if (mousePickerArgs.pickType == MPT_PRESS || mousePickerArgs.pickType == MPT_HOVER) {
		//now check the entity picking
		Ogre::RaySceneQueryResult& queryResult = mCameraRaySceneQuery->getLastResults();
		bool continuePicking = true;
		unsigned int queryMask = 0;

		WorldPickListenersStore participatingListeners;
		for (WorldPickListenersStore::iterator I = mPickListeners.begin(); I != mPickListeners.end(); ++I) {
			bool willParticipate = false;
			unsigned int pickerQueryMask = 0;
			(*I)->initializePickingContext(willParticipate, pickerQueryMask, mousePickerArgs);
			if (willParticipate) {
				queryMask |= pickerQueryMask;
				participatingListeners.push_back(*I);
			}
		}

		//Only perform picking if there are any participating pick listeners.
		if (participatingListeners.size() != 0) {
			mCameraRaySceneQuery->setQueryMask(queryMask);
			// Start a new ray query
			Ogre::Ray cameraRay = getCamera().getCameraToViewportRay(mouseX, mouseY);

			mCameraRaySceneQuery->setRay(cameraRay);
			mCameraRaySceneQuery->execute();

			Ogre::RaySceneQueryResult::iterator rayIterator = queryResult.begin();
			Ogre::RaySceneQueryResult::iterator rayIterator_end = queryResult.end();
			if (rayIterator != rayIterator_end) {
				for (; rayIterator != rayIterator_end && continuePicking; rayIterator++) {
					for (WorldPickListenersStore::iterator I = participatingListeners.begin(); I != participatingListeners.end(); ++I) {
						(*I)->processPickResult(continuePicking, *rayIterator, cameraRay, mousePickerArgs);
						if (!continuePicking) {
							break;
						}
					}
				}
			}

			for (WorldPickListenersStore::iterator I = participatingListeners.begin(); I != participatingListeners.end(); ++I) {
				(*I)->endPickingContext(mousePickerArgs);
			}
		}
	} else {
		for (auto listener : mPickListeners) {
			listener->processDelayedPick(mousePickerArgs);
		}

	}

}

void MainCamera::setClosestPickingDistance(Ogre::Real distance)
{
	mClosestPickingDistance = distance;
}
Ogre::Real MainCamera::getClosestPickingDistance() const
{
	return mClosestPickingDistance;
}

void MainCamera::setMovementProvider(IMovementProvider* movementProvider)
{
	mMovementProvider = movementProvider;
}

bool MainCamera::worldToScreen(const Ogre::Vector3& worldPos, Ogre::Vector2& screenPos)
{
	Ogre::Vector3 hcsPosition = mCamera.getProjectionMatrix() * (mCamera.getViewMatrix() * worldPos);

	if ((hcsPosition.x < -1.0f) || (hcsPosition.x > 1.0f) || (hcsPosition.y < -1.0f) || (hcsPosition.y > 1.0f))
		return false;

	screenPos.x = (hcsPosition.x + 1) * 0.5;
	screenPos.y = (-hcsPosition.y + 1) * 0.5;

	return true;
}

bool MainCamera::frameStarted(const Ogre::FrameEvent& event)
{
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
	}
	return true;
}

void MainCamera::Input_MouseMoved(const MouseMotion& motion, Input::InputMode mode)
{
	if (mCameraMount) {
		if (mode == Input::IM_MOVEMENT) {

			bool moved = false;
			if (motion.xRelativeMovement) {
				moved = mCameraMount->yaw(motion.xRelativeMovement).valueDegrees() || moved;
			}
			if (motion.yRelativeMovement) {
				moved = mCameraMount->pitch(motion.yRelativeMovement).valueDegrees() || moved;
			}

			if (moved) {
				markCameraNodeAsDirty();
				mCameraOrientationChangedThisFrame = true;
				MovedCamera.emit(mCamera);
			}
		}
	}
}

void MainCamera::enableCompositor(const std::string& compositorName, bool enable)
{
	if (enable) {
		S_LOG_INFO("Enabling compositor '" << compositorName<< "'.");
	} else {
		S_LOG_INFO("Disabling compositor '" << compositorName<< "'.");
	}
	if (std::find(mLoadedCompositors.begin(), mLoadedCompositors.end(), compositorName) == mLoadedCompositors.end()) {
		Ogre::CompositorInstance* compositor = Ogre::CompositorManager::getSingleton().addCompositor(mWindow.getViewport(0), compositorName);
		if (compositor) {
			bool hasErrors = false;
			//There's a bug in Ogre which will causes a segfault during rendering if a compositor has an invalid shader.
			//We therefore need to check for this here, and disable the shader if so is the case.
			compositor->getCompositor()->load();
			hasErrors = !validateCompositionTargetPass(*compositor->getTechnique()->getOutputTargetPass());
			if (!hasErrors) {
				Ogre::CompositionTechnique::TargetPassIterator targetPassIter = compositor->getTechnique()->getTargetPassIterator();
				while (targetPassIter.hasMoreElements() && !hasErrors) {
					hasErrors = !validateCompositionTargetPass(*targetPassIter.getNext());
				}
			}
			if (hasErrors) {
				S_LOG_FAILURE("Compositor "<< compositorName <<" has errors and will be disabled.");
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

bool MainCamera::validateCompositionTargetPass(Ogre::CompositionTargetPass& compositionPass)
{
	Ogre::CompositionTargetPass::PassIterator compPassIter = compositionPass.getPassIterator();
	while (compPassIter.hasMoreElements()) {
		Ogre::CompositionPass* compositorPass = compPassIter.getNext();
		compositorPass->getMaterial()->load();
		Ogre::Material::TechniqueIterator techIter = compositorPass->getMaterial()->getSupportedTechniqueIterator();
		while (techIter.hasMoreElements()) {
			Ogre::Technique::PassIterator _passIter = techIter.getNext()->getPassIterator();
			while (_passIter.hasMoreElements()) {
				Ogre::Pass* pass = _passIter.getNext();
				//Also disallow camera polygon mode overide. This is because if it's turned on,
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

void MainCamera::pushWorldPickListener(IWorldPickListener* worldPickListener)
{
	mPickListeners.push_front(worldPickListener);

}

void MainCamera::removeWorldPickListener(IWorldPickListener* worldPickListener)
{
	if (worldPickListener) {
		WorldPickListenersStore::iterator I = std::find(mPickListeners.begin(), mPickListeners.end(), worldPickListener);
		if (I != mPickListeners.end()) {
			mPickListeners.erase(I);
		}
	}
}

void MainCamera::createRayQueries(Ogre::SceneManager& sceneManager)
{
	mCameraRaySceneQuery = sceneManager.createRayQuery(Ogre::Ray(), 0);
	mCameraRaySceneQuery->setWorldFragmentType(Ogre::SceneQuery::WFT_SINGLE_INTERSECTION);
	mCameraRaySceneQuery->setSortByDistance(true);
}

ICameraMount* MainCamera::attachToMount(ICameraMount* newCameraMount)
{
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
