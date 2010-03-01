/*
 Copyright (C) 2004  Erik Hjortsberg
 some parts Copyright (C) 2004 bad_camel at Ogre3d forums

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
#include "MovementController.h"

#include "components/ogre/EmberEntity.h"
#include "components/ogre/GUIManager.h"
#include "components/ogre/Avatar.h"
#include "components/ogre/EmberOgre.h"
#include "components/ogre/OgreInfo.h"
#include "components/ogre/Convert.h"
#include "components/ogre/FreeFlyingCameraMotionHandler.h"
#include "components/ogre/camera/MainCamera.h"
#include "components/ogre/camera/ThirdPersonCameraMount.h"
#include "components/ogre/terrain/TerrainManager.h"
#include "components/ogre/terrain/ISceneManagerAdapter.h"

#include "services/input/Input.h"
#include "services/EmberServices.h"
#include "services/server/ServerService.h"

#include "framework/Tokeniser.h"
#include "framework/LoggingInstance.h"

#include <OgreRoot.h>

using namespace Ogre;
using namespace Ember;
namespace EmberOgre
{

MovementControllerInputListener::MovementControllerInputListener(MovementController& controller) :
	mController(controller)
{
	Input::getSingleton().EventMouseButtonPressed.connect(sigc::mem_fun(*this, &MovementControllerInputListener::input_MouseButtonPressed));
	Input::getSingleton().EventMouseButtonReleased.connect(sigc::mem_fun(*this, &MovementControllerInputListener::input_MouseButtonReleased));

}

void MovementControllerInputListener::input_MouseButtonPressed(Input::MouseButton button, Input::InputMode mode)
{
	if (mode == Input::IM_MOVEMENT && button == Input::MouseButtonLeft) {
		mController.mMovementDirection.y() = 1;
	}
}

void MovementControllerInputListener::input_MouseButtonReleased(Input::MouseButton button, Input::InputMode mode)
{
	if (mode == Input::IM_MOVEMENT && button == Input::MouseButtonLeft) {
		mController.mMovementDirection.y() = 0;
	}
}

MovementController::MovementController(Avatar& avatar) :
	RunToggle("+run", this, "Toggle running mode."), ToggleCameraAttached("toggle_cameraattached", this, "Toggle between the camera being attached to the avatar and free flying."), MovementMoveForward("+movement_move_forward", this, "Move forward."), MovementMoveBackward("+movement_move_backward", this, "Move backward."), MovementMoveDownwards("+movement_move_downwards", this, "Move downwards."), MovementMoveUpwards("+movement_move_upwards", this, "Move upwards."), MovementStrafeLeft("+movement_strafe_left", this, "Strafe left."), MovementStrafeRight("+movement_strafe_right", this, "Strafe right."), CameraOnAvatar("camera_on_avatar", this, "Positions the free flying camera on the avatar.")
	/*, MovementRotateLeft("+Movement_rotate_left", this, "Rotate left.")
	 , MovementRotateRight("+Movement_rotate_right", this, "Rotate right.")*/
	//, MoveCameraTo("movecamerato", this, "Moves the camera to a point.")
			, mMovementCommandMapper("movement", "key_bindings_movement"), mIsRunning(false), mMovementDirection(WFMath::Vector<3>::ZERO()), mDecalObject(0), mDecalNode(0), mControllerInputListener(*this), mAvatar(avatar), mFreeFlyingNode(0), mIsFreeFlying(false)
{

	mMovementCommandMapper.restrictToInputMode(Input::IM_MOVEMENT);

	Ogre::Root::getSingleton().addFrameListener(this);

	mMovementCommandMapper.bindToInput(Input::getSingleton());
	Ember::Input::getSingleton().setMovementModeEnabled(true);

	try {
		mFreeFlyingNode = EmberOgre::getSingleton().getSceneManager()->getRootSceneNode()->createChildSceneNode(OgreInfo::createUniqueResourceName("FreeFlyingCameraNode"));
		if (mFreeFlyingNode) {
			if (mAvatar.getEmberEntity().getPredictedPos().isValid()) {
				mFreeFlyingNode->setPosition(Convert::toOgre(mAvatar.getEmberEntity().getPredictedPos()));
			}
			mFreeFlyingNode->translate(Ogre::Vector3(0, 3, 0)); //put it a little on top of the avatar node
			mFreeFlyingMotionHandler = std::auto_ptr<FreeFlyingCameraMotionHandler>(new FreeFlyingCameraMotionHandler(*mFreeFlyingNode));
			mCameraMount = std::auto_ptr<Camera::ThirdPersonCameraMount>(new Camera::ThirdPersonCameraMount(*EmberOgre::getSingleton().getSceneManager()));
			mCameraMount->setMotionHandler(mFreeFlyingMotionHandler.get());
			mCameraMount->attachToNode(mFreeFlyingNode);
		}

	} catch (const std::exception& ex) {
		S_LOG_FAILURE("Error when setting up free flying camera mount." << ex);
	}

}
MovementController::~MovementController()
{
	Ogre::Root::getSingleton().removeFrameListener(this);
}

void MovementController::setCameraFreeFlying(bool freeFlying)
{
	if (freeFlying && mCameraMount.get()) {
		EmberOgre::getSingleton().getMainCamera()->attachToMount(mCameraMount.get());
		mIsFreeFlying = true;
	} else {
		EmberOgre::getSingleton().getMainCamera()->attachToMount(&mAvatar.getCameraMount());
		mIsFreeFlying = false;
	}
}

void MovementController::runCommand(const std::string &command, const std::string &args)
{
	if (RunToggle == command) {
		mIsRunning = true;
	} else if (RunToggle.getInverseCommand() == command) {
		mIsRunning = false;
	} else if (ToggleCameraAttached == command) {
		if (mIsFreeFlying) {
			setCameraFreeFlying(false);
		} else {
			setCameraFreeFlying(true);
		}
	} else if (MovementMoveForward == command) {
		mMovementDirection.y() = 1;
	} else if (MovementMoveForward.getInverseCommand() == command) {
		mMovementDirection.y() = 0;
	} else if (MovementMoveBackward == command) {
		mMovementDirection.y() = -1;
	} else if (MovementMoveBackward.getInverseCommand() == command) {
		mMovementDirection.y() = 0;
	} else if (MovementStrafeLeft == command) {
		mMovementDirection.x() = -1;
	} else if (MovementStrafeLeft.getInverseCommand() == command) {
		mMovementDirection.x() = 0;
	} else if (MovementStrafeRight == command) {
		mMovementDirection.x() = 1;
	} else if (MovementStrafeRight.getInverseCommand() == command) {
		mMovementDirection.x() = 0;
	} else if (MovementMoveUpwards == command) {
		mMovementDirection.z() = 1;
	} else if (MovementMoveUpwards.getInverseCommand() == command) {
		mMovementDirection.z() = 0;
	} else if (MovementMoveDownwards == command) {
		mMovementDirection.z() = -1;
	} else if (MovementMoveDownwards.getInverseCommand() == command) {
		mMovementDirection.z() = 0;
		/*	} else if (MovementRotateLeft == command) {
		 mAvatarCamera->yaw(Ogre::Degree(-15));
		 } else if (MovementRotateRight == command) {
		 mAvatarCamera->yaw(Ogre::Degree(15));*/
		//	} else if (MoveCameraTo == command) {
		//		if (!mIsAttached) {
		//			Ember::Tokeniser tokeniser;
		//			tokeniser.initTokens(args);
		//			std::string x = tokeniser.nextToken();
		//			std::string y = tokeniser.nextToken();
		//			std::string z = tokeniser.nextToken();
		//
		//			if (x == "" || y == "" || z == "") {
		//				return;
		//			} else {
		//				Ogre::Vector3 position(Ogre::StringConverter::parseReal(x),Ogre::StringConverter::parseReal(y),Ogre::StringConverter::parseReal(z));
		//				mFreeFlyingCameraNode->setPosition(position);
		//			}
		//		}
	} else if (CameraOnAvatar == command) {
		if (mFreeFlyingNode && mAvatar.getEmberEntity().getPosition().isValid()) {
			mFreeFlyingNode->setPosition(Convert::toOgre(mAvatar.getEmberEntity().getViewPosition()));
		}
	}
}

bool MovementController::frameStarted(const Ogre::FrameEvent& event)
{

	if (mDecalObject) {
		///hide the decal when we're close to it
		if (mDecalNode->_getDerivedPosition().distance(mAvatar.getAvatarSceneNode()->_getDerivedPosition()) < 1) {
			mDecalNode->setVisible(false);
		}
	}

	return true;
}

MovementControllerMode::Mode MovementController::getMode() const
{
	return mIsRunning ? MovementControllerMode::MM_RUN : MovementControllerMode::MM_WALK;
}

void MovementController::moveToPoint(const Ogre::Vector3& point)
{
	if (!mDecalNode) {
		createDecal(point);
	}

	//TODO: reapply the terrain lookup without using the terrain manager directly
	//	if (mDecalNode) {
	//		///make sure it's at the correct height, since the visibility of it is determined by the bounding box
	//		Ogre::Real height = EmberOgre::getSingleton().getTerrainManager()->getAdapter()->getHeightAt(point.x, point.z);
	//		mDecalNode->setPosition(Ogre::Vector3(point.x, height, point.z));
	//		mDecalNode->setVisible(true);
	//	}

	WFMath::Vector<3> atlasVector = Convert::toWF<WFMath::Vector<3> >(point);
	WFMath::Point<3> atlasPos(atlasVector.x(), atlasVector.y(), atlasVector.z());

	Ember::EmberServices::getSingletonPtr()->getServerService()->moveToPoint(atlasPos);
}

void MovementController::teleportTo(const Ogre::Vector3& point, EmberEntity* locationEntity)
{
	WFMath::Vector<3> atlasVector = Convert::toWF<WFMath::Vector<3> >(point);
	WFMath::Point<3> atlasPos(atlasVector.x(), atlasVector.y(), atlasVector.z());

	Ember::EmberServices::getSingletonPtr()->getServerService()->place(&mAvatar.getEmberEntity(), locationEntity, atlasPos);
}

WFMath::Vector<3> MovementController::getMovementForCurrentFrame() const
{
	if (mMovementDirection.isValid()) {
		//If the vector is null we'll get an error if we normalize, so we'll just return a zero vector then.
		if (mMovementDirection != WFMath::Vector<3>::ZERO()) {
			//When not running, we assume that the speed is half of the max speed (which is 1.0)
			if (mIsRunning) {
				return WFMath::Vector<3>(mMovementDirection).normalize(1.0);
			} else {
				return WFMath::Vector<3>(mMovementDirection).normalize(1.0) * 0.5;
			}
		} else {
			return mMovementDirection;
		}
	} else {
		return WFMath::Vector<3>::ZERO();
	}
}

void MovementController::createDecal(Ogre::Vector3 position)
{
	try {
		// Create object MeshDecal
		Ogre::SceneManager* sceneManager = EmberOgre::getSingleton().getSceneManager();
		Ogre::NameValuePairList params;
		params["materialName"] = "/global/ember/terraindecal";
		params["width"] = StringConverter::toString(2);
		params["height"] = StringConverter::toString(2);
		params["sceneMgrInstance"] = sceneManager->getName();

		mDecalObject = sceneManager->createMovableObject("MovementControllerMoveToDecal", "PagingLandScapeMeshDecal", &params);

		// Add MeshDecal to Scene
		mDecalNode = sceneManager->createSceneNode("MovementControllerMoveToDecalNode");
		///the decal code is a little shaky and relies on us setting the position of the node before we add the moveable object
		EmberOgre::getSingleton().getWorldSceneNode()->addChild(mDecalNode);
		mDecalNode->setPosition(position);
		mDecalNode->attachObject(mDecalObject);
		// 	mDecalNode->showBoundingBox(true);


		// 		mPulsatingController = new Ogre::WaveformControllerFunction(Ogre::WFT_SINE, 1, 0.33, 0.25);
	} catch (const Ogre::Exception& ex) {
		S_LOG_WARNING("Error when creating terrain decal." << ex);
	}
}

}

