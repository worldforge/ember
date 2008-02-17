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
#include "AvatarController.h"


#include "EmberEntity.h"
#include "EmberPhysicalEntity.h"
#include "AvatarEmberEntity.h"
#include "AvatarCamera.h"
#include "GUIManager.h"
#include "Avatar.h"
#include "EmberOgre.h"
// #include "SceneManagers/EmberPagingSceneManager/include/EmberPagingSceneManager.h"
#include "terrain/TerrainGenerator.h"
#include "terrain/ISceneManagerAdapter.h"


#include "input/Input.h"

#include "framework/Tokeniser.h"
#include "MathConverter.h"
#include "services/EmberServices.h"
#include "services/server/ServerService.h"


using namespace Ogre;
namespace EmberOgre {


AvatarControllerMovement::AvatarControllerMovement() :
	rotationDegHoriz(0),
	rotationDegVert(0),
	timeSlice(0),
	movementDirection(Ogre::Vector3::ZERO),
	mode(AvatarMovementMode::MM_WALK),
	isMoving(false)
{
}



AvatarControllerInputListener::AvatarControllerInputListener(AvatarController& controller)
: mController(controller)
{
	Input::getSingleton().EventMouseButtonPressed.connect(sigc::mem_fun(*this, &AvatarControllerInputListener::input_MouseButtonPressed));
	Input::getSingleton().EventMouseButtonReleased.connect(sigc::mem_fun(*this, &AvatarControllerInputListener::input_MouseButtonReleased));

}

void AvatarControllerInputListener::input_MouseButtonPressed(Input::MouseButton button, Input::InputMode mode)
{
	if (mode == Input::IM_MOVEMENT && button == Input::MouseButtonLeft) {
		mController.mMovementDirection.x = 1;
	}
}

void AvatarControllerInputListener::input_MouseButtonReleased(Input::MouseButton button, Input::InputMode mode)
{
	if (mode == Input::IM_MOVEMENT && button == Input::MouseButtonLeft) {
		mController.mMovementDirection.x = 0;
	}
}

AvatarController::AvatarController(Avatar* avatar, Ogre::RenderWindow* window, GUIManager* guiManager, Ogre::Camera* camera) 
: RunToggle("+run", this, "Toggle running mode.")
, ToggleCameraAttached("toggle_cameraattached", this, "Toggle between the camera being attached to the avatar and free flying.")
, CharacterMoveForward("+character_move_forward", this, "Move the avatar forward.")
, CharacterMoveBackward("+character_move_backward", this, "Move the avatar backward.")
, CharacterMoveDownwards("+character_move_downwards", this, "Move the avatar downwards.")
, CharacterMoveUpwards("+character_move_upwards", this, "Move the avatar upwards.")
, CharacterStrafeLeft("+character_strafe_left", this, "Strafe left.")
, CharacterStrafeRight("+character_strafe_right", this, "Strafe right.")
/*, CharacterRotateLeft("+character_rotate_left", this, "Rotate left.")
, CharacterRotateRight("+character_rotate_right", this, "Rotate right.")*/
, MoveCameraTo("movecamerato", this, "Moves the camera to a point.")
, mMovementCommandMapper("movement", "key_bindings_movement")
, mWindow(window)
, mGUIManager(guiManager)
, mAvatarCamera(0)
, mCamera(camera)
, mEntityUnderCursor(0) 
, mSelectedEntity(0)
, mFreeFlyingCameraNode(0)
, mIsRunning(false)
, mMovementDirection(Ogre::Vector3::ZERO)
, mDecalObject(0)
, mDecalNode(0)
, mControllerInputListener(*this)
{

	mMovementCommandMapper.restrictToInputMode(Input::IM_MOVEMENT );
	
	setAvatar(avatar);
	
	
	mAvatar->setAvatarController(this);
	
	Ogre::Root::getSingleton().addFrameListener(this);
	
	
	mFreeFlyingCameraNode = EmberOgre::getSingleton().getSceneManager()->getRootSceneNode()->createChildSceneNode();
	mFreeFlyingCameraNode->setPosition(0,30,0);
	detachCamera();
	
	
	mMovementCommandMapper.bindToInput(Input::getSingleton());

	GUIManager::getSingleton().getEntityPickListener()->EventPickedEntity.connect(sigc::mem_fun(*this, &AvatarController::entityPicker_PickedEntity));
	
}
AvatarController::~AvatarController()
{
	Ogre::Root::getSingleton().removeFrameListener(this);
	delete mAvatarCamera;
}

void AvatarController::createAvatarCameras(Ogre::SceneNode* avatarSceneNode)
{
	if (mAvatarCamera == 0) {
		mAvatarCamera = new AvatarCamera(avatarSceneNode, EmberOgre::getSingletonPtr()->getSceneManager(), mWindow, mGUIManager, mCamera);
	} else {
		attachCamera();
	}
}

void AvatarController::setAvatar(Avatar* avatar)
{
	mAvatar = avatar;
	createAvatarCameras(avatar->getAvatarSceneNode());
	attachCamera();
}



void AvatarController::runCommand(const std::string &command, const std::string &args)
{
	if (RunToggle == command) {
		mIsRunning = true;
	} else if (RunToggle.getInverseCommand() == command) {
		mIsRunning = false;
	} else if (ToggleCameraAttached == command)
	{
		if (mIsAttached) {
			detachCamera();
		} else {
			attachCamera();
		}
	} else if (CharacterMoveForward == command) {
		mMovementDirection.x = 1;
	} else if (CharacterMoveForward.getInverseCommand() == command) {
		mMovementDirection.x = 0;
	} else if (CharacterMoveBackward == command) {
		mMovementDirection.x = -1;
	} else if (CharacterMoveBackward.getInverseCommand() == command) {
		mMovementDirection.x = 0;
	} else if (CharacterStrafeLeft == command) {
		mMovementDirection.z = -1;
	} else if (CharacterStrafeLeft.getInverseCommand() == command) {
		mMovementDirection.z = 0;
	} else if (CharacterStrafeRight == command) {
		mMovementDirection.z = 1;
	} else if (CharacterStrafeRight.getInverseCommand() == command) {
		mMovementDirection.z = 0;
	} else if (CharacterMoveUpwards == command) {
		mMovementDirection.y = 1;
	} else if (CharacterMoveUpwards.getInverseCommand() == command) {
		mMovementDirection.y = 0;
	} else if (CharacterMoveDownwards == command) {
		mMovementDirection.y = -1;
	} else if (CharacterMoveDownwards.getInverseCommand() == command) {
		mMovementDirection.y = 0;
/*	} else if (CharacterRotateLeft == command) {
		mAvatarCamera->yaw(Ogre::Degree(-15));
	} else if (CharacterRotateRight == command) {
		mAvatarCamera->yaw(Ogre::Degree(15));*/
	} else if (MoveCameraTo == command) {
		if (!mIsAttached) {
			Ember::Tokeniser tokeniser;
			tokeniser.initTokens(args);
			std::string x = tokeniser.nextToken();
			std::string y = tokeniser.nextToken();
			std::string z = tokeniser.nextToken();
			
			if (x == "" || y == "" || z == "") {
				return;
			} else {
				Ogre::Vector3 position(Ogre::StringConverter::parseReal(x),Ogre::StringConverter::parseReal(y),Ogre::StringConverter::parseReal(z));
				mFreeFlyingCameraNode->setPosition(position);
			}
		}
	}
}


void AvatarController::detachCamera() 
{
	mIsAttached = false;
	mAvatarCamera->attach(mFreeFlyingCameraNode);
	//mAvatarCamera->setMode(AvatarCamera::MODE_FIRST_PERSON);
}


void AvatarController::attachCamera() 
{
	mIsAttached = true;
	mAvatarCamera->attach(mAvatar->getAvatarSceneNode());
	//mAvatarCamera->setMode(AvatarCamera::MODE_FIRST_PERSON);
}


bool AvatarController::frameStarted(const Ogre::FrameEvent& event)
{

	if (mDecalObject)
	{
		///hide the decal when we're close to it
		if (mDecalNode->getWorldPosition().distance(mAvatar->getAvatarSceneNode()->getWorldPosition()) < 1) {
			mDecalNode->setVisible(false);
		}
	}

// 	if (mDecalObject) {
// 		Ogre::Real newSize = mPulsatingController->calculate(event.timeSinceLastFrame);
// 		//mDecalNode->setScale(Ogre::Vector3(newSize, 1.0f, newSize));
// // 		mDecalNode->yaw(Ogre::Radian(0.1f));
// 	}

// 	EmberPagingSceneManager* mScnMgr = static_cast<EmberPagingSceneManager*>(EmberOgre::getSingleton().getSceneManager());
// 	if (mGUIManager->getInput()->isKeyDown(SDLK_F4)) {
// /*		Real change;
// 		mScnMgr->getOption( "MaxPixelError", &change );
// 		change -= 0.5f;
// 		mScnMgr->setOption( "MaxPixelError", &change ); */
// 		--Ogre::PagingLandScapeOptions::getSingleton().maxPixelError;
// 		Ogre::PagingLandScapeOptions::getSingleton().calculateCFactor();
// 		mScnMgr->WorldDimensionChange();
// 	}
// 	if (mGUIManager->getInput()->isKeyDown(SDLK_F5)) {
// /*		Real change;
// 		mScnMgr->getOption( "MaxPixelError", &change );
// 		change += 0.5f;
// 		mScnMgr->setOption( "MaxPixelError", &change ); */
// 		++Ogre::PagingLandScapeOptions::getSingleton().maxPixelError; 
// 		Ogre::PagingLandScapeOptions::getSingleton().calculateCFactor();
// 		mScnMgr->WorldDimensionChange();
// 	}
// 	

	movementForFrame = AvatarControllerMovement();
	
/*	if (mMovementDirection != Ogre::Vector3::ZERO) {*/
	movementForFrame.mode = mIsRunning ? AvatarMovementMode::MM_RUN : AvatarMovementMode::MM_WALK;
	movementForFrame.isMoving = true;
	movementForFrame.movementDirection = mMovementDirection;
	movementForFrame.timeSlice = event.timeSinceLastFrame;
	if (movementForFrame.mode != mPreviousMovementForFrame.mode) {
		EventMovementModeChanged.emit(movementForFrame.mode);
	}
// 	} else {
// 	}

// 	if (mGUIManager->isInMovementKeysMode()) {
//		movementForFrame.movementDirection = Ogre::Vector3::ZERO;
//		movementForFrame.mIsRunning = false;
//		movementForFrame.isMoving = false;	
		
/*		checkMovementKeys(event, EmberOgre::getSingleton().getInput());
		
		
		movementForFrame.timeSlice = event.timeSinceLastFrame;
	}	*/
		
	if (mIsAttached) {
//		mAvatarCamera->adjustForTerrain();
		mAvatar->updateFrame(movementForFrame);
	} else {
		Ogre::Real scaler = 50;
		//make this inverse, so that when the run key is pressed, the free flying camera goes slower
		//this is since we assume that one wants to go fast when in free flying mode
		if (movementForFrame.mode == AvatarMovementMode::MM_RUN) {
			scaler = 10;
		}
		Ogre::Vector3 correctDirection(movementForFrame.movementDirection.z, movementForFrame.movementDirection.y, -movementForFrame.movementDirection.x);
		mFreeFlyingCameraNode->translate(mAvatarCamera->getOrientation(false) * (correctDirection * movementForFrame.timeSlice * scaler));

	}
	mPreviousMovementForFrame = movementForFrame;
		
	

	return true;
}


const AvatarControllerMovement & AvatarController::getCurrentMovement() const
{
	return movementForFrame;
}


AvatarCamera* AvatarController::getAvatarCamera() const
{
	return mAvatarCamera;	
}

void AvatarController::entityPicker_PickedEntity(const EntityPickResult& result, const MousePickerArgs& args)
{
///don't do this now that we have a "move to" option in the menu, since it will confuse the users
/*	if (args.pickType == MPT_DOUBLECLICK) {
		moveToPoint(result.position);
	}*/
}

void AvatarController::moveToPoint(const Ogre::Vector3& point)
{
	if (!mDecalNode) {
		createDecal(point);
	}
	
	if (mDecalNode) {
		///make sure it's at the correct height, since the visibility of it is determined by the bounding box
		Ogre::Real height = EmberOgre::getSingleton().getTerrainGenerator()->getAdapter()->getHeightAt(point.x, point.z);
		mDecalNode->setPosition(Ogre::Vector3(point.x, height, point.z));
		mDecalNode->setVisible(true);
	}
	
	WFMath::Vector<3> atlasVector = Ogre2Atlas_Vector3(point);
	WFMath::Point<3> atlasPos(atlasVector.x(), atlasVector.y(), atlasVector.z());
/*	WFMath::Point<3> atlas2dPos(atlasVector.x(), atlasVector.y(), 0);
	WFMath::Point<3> avatar2dPos(mAvatar->getAvatarEmberEntity()->getPosition().x(), mAvatar->getAvatarEmberEntity()->getPosition().y(), 0);
	WFMath::Vector<3> direction(1, 0, 0);
	direction = direction.rotate(mAvatar->getAvatarEmberEntity()->getOrientation());
	WFMath::Vector<3> directionToPoint = atlas2dPos - avatar2dPos;
	WFMath::Quaternion rotation;
	rotation = rotation.rotation(directionToPoint, direction);*/
	//Ember::EmberServices::getSingletonPtr()->getServerService()->moveInDirection(WFMath::Vector<3>(0,0,0), rotation);	
	
	
	Ember::EmberServices::getSingletonPtr()->getServerService()->moveToPoint(atlasPos);
}


void AvatarController::teleportTo(const Ogre::Vector3& point, EmberEntity* locationEntity)
{
	WFMath::Vector<3> atlasVector = Ogre2Atlas_Vector3(point);
	WFMath::Point<3> atlasPos(atlasVector.x(), atlasVector.y(), atlasVector.z());
	
	
	Ember::EmberServices::getSingletonPtr()->getServerService()->place(mAvatar->getAvatarEmberEntity(), locationEntity, atlasPos);
}


void AvatarController::createDecal(Ogre::Vector3 position)
{
	try {
		// Create object MeshDecal
		Ogre::SceneManager* sceneManager = EmberOgre::getSingleton().getSceneManager();
		Ogre::NameValuePairList params;
		params["materialName"] = "/global/ember/terraindecal";
		params["width"] = StringConverter::toString( 2 );
		params["height"] = StringConverter::toString( 2 );
		params["sceneMgrInstance"] = sceneManager->getName();

		mDecalObject = sceneManager->createMovableObject(
			"AvatarControllerMoveToDecal",
			"PagingLandScapeMeshDecal",
			&params );

		// Add MeshDecal to Scene
		mDecalNode = sceneManager->createSceneNode("AvatarControllerMoveToDecalNode");
		///the decal code is a little shaky and relies on us setting the position of the node before we add the moveable object
		EmberOgre::getSingleton().getWorldSceneNode()->addChild(mDecalNode);
		mDecalNode->setPosition(position);
		mDecalNode->attachObject(mDecalObject);
	// 	mDecalNode->showBoundingBox(true);
		
		
		mPulsatingController = new Ogre::WaveformControllerFunction(Ogre::WFT_SINE, 1, 0.33, 0.25);
	} catch (const Ogre::Exception& ex)
	{
		S_LOG_WARNING("Error when creating terrain decal: " << ex.what());
	}
}


}

