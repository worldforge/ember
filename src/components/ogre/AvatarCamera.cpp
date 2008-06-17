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
#include "AvatarCamera.h"
#include "Avatar.h"
#include "GUIManager.h"
#include "EmberOgre.h"
#include "EmberEntity.h"
// #include "WorldEmberEntity.h"
#include "MathConverter.h"


#include "services/EmberServices.h"
#include "services/config/ConfigService.h"
#include "services/time/TimeService.h"
#ifndef WIN32
#include "services/sound/SoundService.h"
#endif

#include "MousePicker.h"
// #include "jesus/JesusPickerObject.h"

#include "SceneManagers/EmberPagingSceneManager/include/OgrePagingLandScapeRaySceneQuery.h"
#include "framework/Tokeniser.h"
#include "framework/ConsoleBackend.h"

#include "GUIManager.h"
#include "input/Input.h"

#include "IWorldPickListener.h"

#include "framework/osdir.h"

#ifdef __WIN32__
#include <windows.h>
#include <direct.h>
#endif


namespace EmberOgre {
 
Recorder::Recorder(): mSequence(0), mAccruedTime(0.0f), mFramesPerSecond(20.0f)
{
}

void Recorder::startRecording()
{
	Ogre::Root::getSingleton().addFrameListener(this);
}
void Recorder::stopRecording()
{
	Ogre::Root::getSingleton().removeFrameListener(this);
}

bool Recorder::frameStarted(const Ogre::FrameEvent& event)
{
	mAccruedTime += event.timeSinceLastFrame;
	if (mAccruedTime >= (1.0f / mFramesPerSecond)) {
		mAccruedTime = 0.0f;
		std::stringstream filename;
		filename << "screenshot_" << mSequence++ << ".tga";
		const std::string dir = Ember::EmberServices::getSingletonPtr()->getConfigService()->getHomeDirectory() + "/recordings/";
		try {
			//make sure the directory exists
			
			struct stat tagStat;
			int ret;
			ret = stat( dir.c_str(), &tagStat );
			if (ret == -1) {
			#ifdef __WIN32__
				mkdir(dir.c_str());
			#else
				mkdir(dir.c_str(), S_IRWXU);
			#endif
			}
		} catch (const std::exception& ex) {
			S_LOG_FAILURE("Error when creating directory for screenshots. Message: " << std::string(ex.what()));
			stopRecording();
			return true;
		}
		try {
			// take screenshot
			EmberOgre::getSingleton().getRenderWindow()->writeContentsToFile(dir + filename.str());
		} catch (const Ogre::Exception& ex) {
			S_LOG_FAILURE("Could not write screenshot to disc. Message: "<< ex.getFullDescription());
			stopRecording();
			return true;
		}
	}
	return true;
}

 

AvatarCamera::AvatarCamera(Ogre::SceneNode* avatarNode, Ogre::SceneManager* sceneManager, Ogre::RenderWindow* window, GUIManager* guiManager, Ogre::Camera* camera) :
	SetCameraDistance("setcameradistance", this, "Set the distance of the camera."),
	ToggleRendermode("toggle_rendermode", this, "Toggle between wireframe and solid render modes."),
	ToggleFullscreen("toggle_fullscreen", this, "Switch between windowed and full screen mode."),
	Screenshot("screenshot", this, "Take a screenshot and write to disk."),
	Record("+record", this, "Record to disk."),
	mInvertCamera(false),
	mGUIManager(guiManager),
	mCamera(camera),
	mAvatarNode(0),
	mSceneManager(sceneManager),
	mDegreeOfPitchPerSecond(50),
	mDegreeOfYawPerSecond(50),
	degreePitch(0),
	degreeYaw(0),
	mWindow(window),
	mViewPort(0),
	mClosestPickingDistance(10000),
	mLastPosition(Ogre::Vector3::ZERO),
	mAdjustTerrainRaySceneQuery(0),
	mCameraRaySceneQuery(0),
	mIsAdjustedToTerrain(true),
	mAvatarTerrainCursor(*this)
//	mLastOrientationOfTheCamera(avatar->getOrientation())
{
	createNodesForCamera();
	createViewPort();
	setAvatarNode(avatarNode);
	
	/// Register this as a frame listener
	Ogre::Root::getSingleton().addFrameListener(this);
	
	if (mGUIManager) {
		mGUIManager->getInput().EventMouseMoved.connect(sigc::mem_fun(*this, &AvatarCamera::Input_MouseMoved));
	}

	Ember::EmberServices::getSingletonPtr()->getConfigService()->EventChangedConfigItem.connect(sigc::mem_fun(*this, &AvatarCamera::ConfigService_EventChangedConfigItem));
	
	updateValuesFromConfig();
	createRayQueries();
}

AvatarCamera::~AvatarCamera()
{
	Ogre::Root::getSingleton().removeFrameListener(this);
	EmberOgre::getSingleton().getSceneManager()->destroyQuery(mAdjustTerrainRaySceneQuery);
	EmberOgre::getSingleton().getSceneManager()->destroyQuery(mCameraRaySceneQuery);
}

void AvatarCamera::createRayQueries()
{
    // attempt to create a query to get back terrain coords
	mAdjustTerrainRaySceneQuery = EmberOgre::getSingletonPtr()->getSceneManager()->createRayQuery(mAdjustTerrainRay, Ogre::SceneManager::WORLD_GEOMETRY_TYPE_MASK);
	///only test for terrain
	mAdjustTerrainRaySceneQuery->setWorldFragmentType(Ogre::SceneQuery::WFT_SINGLE_INTERSECTION);
	mAdjustTerrainRaySceneQuery->setSortByDistance(true);
	mAdjustTerrainRaySceneQuery->setQueryTypeMask(Ogre::SceneManager::WORLD_GEOMETRY_TYPE_MASK);
	
	
	unsigned long queryMask = Ogre::SceneManager::WORLD_GEOMETRY_TYPE_MASK;
	queryMask |= MousePicker::CM_AVATAR;
	queryMask |= MousePicker::CM_ENTITY;
	queryMask |= MousePicker::CM_NATURE;
	queryMask |= MousePicker::CM_UNDEFINED;
// 	queryMask |= Ogre::RSQ_FirstTerrain;
	
	mCameraRaySceneQuery = mSceneManager->createRayQuery( Ogre::Ray(), queryMask); 
	mCameraRaySceneQuery->setWorldFragmentType(Ogre::SceneQuery::WFT_SINGLE_INTERSECTION);
	mCameraRaySceneQuery->setSortByDistance(true);

}


void AvatarCamera::createNodesForCamera()
{
	mAvatarCameraRootNode = mSceneManager->createSceneNode("AvatarCameraRootNode");
	//we need to adjust for the height of the avatar mesh
	mAvatarCameraRootNode->setPosition(Ogre::Vector3(0,2,0));
	//rotate to sync with WF world
    mAvatarCameraRootNode->rotate(Ogre::Vector3::UNIT_Y,(Ogre::Degree)-90);

	mAvatarCameraPitchNode = mAvatarCameraRootNode->createChildSceneNode("AvatarCameraPitchNode");
	mAvatarCameraPitchNode->setPosition(Ogre::Vector3(0,0,0));
	mAvatarCameraNode = mAvatarCameraPitchNode->createChildSceneNode("AvatarCameraNode");
	setCameraDistance(10);
	
//	mCamera = mSceneManager->createCamera("AvatarCamera");
	mAvatarCameraNode->attachObject(mCamera);
	// Look to the Avatar's head
	//mAvatar3pCamera->setAutoTracking(true, mAvatar1pCameraNode);
	mCamera->setNearClipDistance(0.5);
	
	///set the far clip distance high to make sure that the sky is completely shown
	if (Ogre::Root::getSingleton().getRenderSystem()->getCapabilities()->hasCapability(Ogre::RSC_INFINITE_FAR_PLANE))
	{
/*		//NOTE: this won't currently work with the sky
		mCamera->setFarClipDistance(0);*/
		
		mCamera->setFarClipDistance(10000);
	} else {
		mCamera->setFarClipDistance(10000);
	}
	
	//create the nodes for the camera
	setMode(MODE_THIRD_PERSON);
// 	createViewPort();
}

void AvatarCamera::setMode(Mode mode)
{
	mMode = mode;
/*	if (mMode == MODE_THIRD_PERSON) {
		mCamera->setAutoTracking(true, mAvatarCameraRootNode);
	} else {
		mCamera->setAutoTracking(false);
	}*/
	
	
}

const Ogre::Quaternion& AvatarCamera::getOrientation(bool onlyHorizontal) const {
	if (!onlyHorizontal) {
		return getCamera()->getDerivedOrientation();
	} else {
		static Ogre::Quaternion quat;
		quat = getCamera()->getDerivedOrientation();
		quat.x = 0;
		quat.z = 0;
		return quat;
	}
}

const Ogre::Vector3& AvatarCamera::getPosition() const
{
	return mCamera->getDerivedPosition();
}

void AvatarCamera::attach(Ogre::SceneNode* toNode) {
	mIsAttached = true;
	assert(mAvatarCameraRootNode);
	if (mAvatarCameraRootNode->getParent()) {
		mAvatarCameraRootNode->getParent()->removeChild(mAvatarCameraRootNode->getName());
	}
	toNode->addChild(mAvatarCameraRootNode);
	
	setCameraDistance(10);
	mAvatarCameraNode->setOrientation(Ogre::Quaternion::IDENTITY);
	mAvatarCameraNode->_update(true, true);
	std::stringstream ss;
	ss << "Attached camera to node: " << toNode->getName() <<". New position: " << mCamera->getWorldPosition() << " New orientation: " << mCamera->getWorldOrientation();
	S_LOG_VERBOSE(ss.str());
}


void AvatarCamera::enableCompositor(const std::string& compositorName, bool enable)
{
	if (std::find(mLoadedCompositors.begin(), mLoadedCompositors.end(), compositorName) == mLoadedCompositors.end()) {
		Ogre::CompositorManager::getSingleton().addCompositor(mWindow->getViewport(0), compositorName);
	}
	Ogre::CompositorManager::getSingleton().setCompositorEnabled(mWindow->getViewport(0), compositorName, enable);
}

void AvatarCamera::createViewPort()
{

//	Ogre::CompositorManager::getSingleton().addCompositor(mWindow->getViewport(0), "Bloom");
// 	Ogre::CompositorManager::getSingleton().setCompositorEnabled(mWindow->getViewport(0), "Bloom", true);

// 	assert(mCamera);
// 	assert(!mViewPort);
//     // Create 1st person viewport, entire window
//     mViewPort = mWindow->addViewport(mCamera);
//     mViewPort->setBackgroundColour(Ogre::ColourValue(0,0,0));
//     mCamera->setAspectRatio(
// 		Ogre::Real(mViewPort->getActualWidth()) / Ogre::Real(mViewPort->getActualHeight()));

	
}
		
		
void AvatarCamera::toggleRenderMode()
{
	S_LOG_INFO("Switching render mode.");
	if (mCamera->getPolygonMode() == Ogre::PM_SOLID) {
		mCamera->setPolygonMode(Ogre::PM_WIREFRAME);
	} else {
		mCamera->setPolygonMode(Ogre::PM_SOLID);
	}
}

void AvatarCamera::setAvatarNode(Ogre::SceneNode* sceneNode)
{
	mAvatarNode = sceneNode;
	attach(mAvatarNode);
}

void AvatarCamera::setCameraDistance(Ogre::Real distance)
{
	mWantedCameraDistance = distance;
	_setCameraDistance(distance);
}

void AvatarCamera::_setCameraDistance(Ogre::Real distance)
{
	mCurrentCameraDistance = distance;
	Ogre::Vector3 pos(0,0,distance);
	mAvatarCameraNode->setPosition(pos);
	EventChangedCameraDistance.emit(distance);
}

void AvatarCamera::pitch(Ogre::Degree degrees)
{
	if (mInvertCamera) {
		degrees -= degrees * 2;
	}
	if (mMode == MODE_THIRD_PERSON) {
		degreePitch += degrees;
		mAvatarCameraPitchNode->pitch(degrees);
	} else {
		mAvatarCameraNode->pitch(degrees);
	}
}
void AvatarCamera::yaw(Ogre::Degree degrees)
{
	if (mMode == MODE_THIRD_PERSON) {
		degreeYaw += degrees;
		mAvatarCameraRootNode->yaw(degrees);
		
		mAvatarCameraRootNode->needUpdate();
//		Ogre::Quaternion q = mAvatarCameraRootNode->_getDerivedOrientation();
	} else {
		mAvatarCameraNode->yaw(degrees);
	}
	
}

void AvatarCamera::Input_MouseMoved(const MouseMotion& motion, Input::InputMode mode)
/*(int xPosition, int yPosition, Ogre::Real xRelativeMovement, Ogre::Real yRelativeMovement, Ogre::Real timeSinceLastMovement)*/
{
	if (mode == Input::IM_MOVEMENT) {
		Ogre::Degree diffX(mDegreeOfYawPerSecond * motion.xRelativeMovement);
		Ogre::Degree diffY(mDegreeOfPitchPerSecond * motion.yRelativeMovement);
	
		if (diffX.valueDegrees()) {
			this->yaw(diffX);
	//		this->yaw(diffX * e->timeSinceLastFrame);
		}
		if (diffY.valueDegrees()) {
			this->pitch(diffY);
	//		this->pitch(diffY * e->timeSinceLastFrame);
		}
		
		if (diffY.valueDegrees() || diffX.valueDegrees()) {
			MovedCamera.emit(mCamera);
		}
	} 
	
	
}




void AvatarCamera::pickInWorld(Ogre::Real mouseX, Ogre::Real mouseY, const MousePickerArgs& mousePickerArgs)
{
	S_LOG_INFO("Trying to pick an entity at mouse coords: "  << Ogre::StringConverter::toString(mouseX) << ":" << Ogre::StringConverter::toString(mouseY) << ".");

	// get the terrain vector for mouse coords when a pick event happens
	mAvatarTerrainCursor.getTerrainCursorPosition();
	
	/// Start a new ray query 
	Ogre::Ray cameraRay = getCamera()->getCameraToViewportRay( mouseX, mouseY ); 

	mCameraRaySceneQuery->setRay(cameraRay);
	mCameraRaySceneQuery->execute(); 
	
	
	///now check the entity picking
	Ogre::RaySceneQueryResult& queryResult = mCameraRaySceneQuery->getLastResults(); 
	bool continuePicking = true;
			
	for (WorldPickListenersStore::iterator I = mPickListeners.begin(); I != mPickListeners.end(); ++I) {
		(*I)->initializePickingContext();
	}
			
			
	Ogre::RaySceneQueryResult::iterator rayIterator = queryResult.begin( ); 
	Ogre::RaySceneQueryResult::iterator rayIterator_end = queryResult.end( );
	if (rayIterator != rayIterator_end) {
		for ( ; rayIterator != rayIterator_end && continuePicking; rayIterator++ ) {
			for (WorldPickListenersStore::iterator I = mPickListeners.begin(); I != mPickListeners.end(); ++I) {
				(*I)->processPickResult(continuePicking, *rayIterator, cameraRay, mousePickerArgs);
				if (!continuePicking) {
					break;
				}
			}
		}
	}
	
	for (WorldPickListenersStore::iterator I = mPickListeners.begin(); I != mPickListeners.end(); ++I) {
		(*I)->endPickingContext(mousePickerArgs);
	}
}

	bool AvatarCamera::worldToScreen(const Ogre::Vector3& worldPos, Ogre::Vector3& screenPos) 
	{
		
		Ogre::Vector3 hcsPosition = mCamera->getProjectionMatrix() * (mCamera->getViewMatrix() * worldPos); 
	
		if ((hcsPosition.x < -1.0f) || 
		(hcsPosition.x > 1.0f) || 
		(hcsPosition.y < -1.0f) || 
		(hcsPosition.y > 1.0f)) 
		return false; 
	
	
		screenPos.x = (hcsPosition.x + 1) * 0.5; 
		screenPos.y = (-hcsPosition.y + 1) * 0.5; 
	
		return true; 
	}

	void AvatarCamera::setClosestPickingDistance(Ogre::Real distance)
	{
		mClosestPickingDistance = distance; 
	}
	
	Ogre::Real AvatarCamera::getClosestPickingDistance() 
	{ 
		return mClosestPickingDistance; 
	}
	
	bool AvatarCamera::adjustForTerrain()
	{
		/// We will shoot a ray from the camera base node to the camera. If it hits anything on the way we know that there's something between the camera and the avatar and we'll position the camera closer to the avatar. Thus we'll avoid having the camera dip below the terrain
		///For now we'll only check against the terrain
		const Ogre::Vector3 direction(-mCamera->getDerivedDirection());
		///If the direction if pointing straight upwards we'll end up in an infinite loop in the ray query
		if (direction.z != 0) {
		
			mAdjustTerrainRay.setDirection(direction);
			mAdjustTerrainRay.setOrigin(mAvatarCameraRootNode->getWorldPosition());
			
 			mAdjustTerrainRaySceneQuery->setRay(mAdjustTerrainRay);
			
			mAdjustTerrainRaySceneQuery->execute(); 
			
			Ogre::RaySceneQueryResult queryResult = mAdjustTerrainRaySceneQuery->getLastResults(); 
			Ogre::RaySceneQueryResult::iterator rayIterator = queryResult.begin( );
			for ( ; rayIterator != queryResult.end(); ++rayIterator ) {
				Ogre::RaySceneQueryResultEntry& entry = *rayIterator;
				
				if (entry.worldFragment) {
					Ogre::Vector3 position = entry.worldFragment->singleIntersection;
					Ogre::Real distance = mAvatarCameraRootNode->getWorldPosition().distance(position);
					if (distance < mWantedCameraDistance) {
						_setCameraDistance(distance - 0.1);
					} else {
						if (mWantedCameraDistance != mCurrentCameraDistance) {
							_setCameraDistance(mWantedCameraDistance);
						}
					}
					break;
				}
			}
		}
		
/*		Ogre::RaySceneQuery *raySceneQueryHeight = EmberOgre::getSingletonPtr()->getSceneManager()->createRayQuery( Ogre::Ray(mCamera->getDerivedPosition(), Ogre::Vector3::NEGATIVE_UNIT_Y), Ogre::SceneManager::WORLD_GEOMETRY_TYPE_MASK); 
		
		
		raySceneQueryHeight->execute(); 
		
		//first check the terrain picking
		Ogre::RaySceneQueryResult queryResult = raySceneQueryHeight->getLastResults(); 
		
		if (queryResult.begin( ) != queryResult.end()) {
			Ogre::Vector3 position = queryResult.begin()->worldFragment->singleIntersection;
			Ogre::Real terrainHeight = position.y;
			//pad it a little
			//terrainHeight += 0.4;
			Ogre::Real cameraHeight = mCamera->getDerivedPosition().y;
			Ogre::Real cameraNodeHeight = mAvatarCameraNode->getWorldPosition().y;
			if (terrainHeight > cameraHeight) {
				mCamera->move(mCamera->getDerivedOrientation().Inverse() * Ogre::Vector3(0,terrainHeight - cameraHeight,0));
//				mCamera->lookAt(mAvatarCameraRootNode->getPosition());
				
			} else if (cameraHeight != cameraNodeHeight) {
				Ogre::Real newHeight = std::max<Ogre::Real>(terrainHeight, cameraNodeHeight);
				mCamera->move(Ogre::Vector3(0,newHeight - cameraHeight,0));
				mCamera->lookAt(mAvatarCameraRootNode->getWorldPosition());
				
			}
			
		}*/
	
		return true;		
	}
	
void AvatarCamera::runCommand(const std::string &command, const std::string &args)
{
	if(Screenshot == command) {
		//just take a screen shot
		takeScreenshot();
	} else if(SetCameraDistance == command)
	{
		Ember::Tokeniser tokeniser;
		tokeniser.initTokens(args);
		std::string distance = tokeniser.nextToken();
		if (distance != "") {
			float fDistance = Ogre::StringConverter::parseReal(distance);
			setCameraDistance(fDistance);
		}
	} else if (ToggleFullscreen == command){
		SDL_WM_ToggleFullScreen(SDL_GetVideoSurface());
		
	} else if (ToggleRendermode == command) {
		toggleRenderMode();
	} else if (Record == command) {
		mRecorder.startRecording();
	} else if (Record.getInverseCommand() == command) {
		mRecorder.stopRecording();
	}
}
	
void AvatarCamera::updateValuesFromConfig()
{
	if (Ember::EmberServices::getSingletonPtr()->getConfigService()->itemExists("input", "invertcamera")) {
		mInvertCamera = static_cast<bool>(Ember::EmberServices::getSingletonPtr()->getConfigService()->getValue("input", "invertcamera"));
	}
	if (Ember::EmberServices::getSingletonPtr()->getConfigService()->itemExists("input", "cameradegreespersecond")) {
		mDegreeOfPitchPerSecond = mDegreeOfYawPerSecond = (double)Ember::EmberServices::getSingletonPtr()->getConfigService()->getValue("input", "cameradegreespersecond");
	}
	if (Ember::EmberServices::getSingletonPtr()->getConfigService()->itemExists("input", "adjusttoterrain")) {
		mIsAdjustedToTerrain = static_cast<bool>(Ember::EmberServices::getSingletonPtr()->getConfigService()->getValue("input", "adjusttoterrain"));
	}	
}
	
void AvatarCamera::ConfigService_EventChangedConfigItem(const std::string& section, const std::string& key)
{
	if (section == "input") {
		if (key == "invertcamera" || key == "cameradegreespersecond" || key == "adjusttoterrain") {
			updateValuesFromConfig();
		}
	}
}

bool AvatarCamera::frameStarted(const Ogre::FrameEvent& event)
{
	if (mIsAdjustedToTerrain) {
		if (mCamera->getDerivedPosition() != mLastPosition) {
			adjustForTerrain();
		}
	}
	mLastPosition = mCamera->getDerivedPosition();
// #ifndef WIN32
// 	Ember::SoundService* mySoundService = Ember::EmberServices::getSingleton().getSoundService();
// 	{
// 		mySoundService->updateListenerPosition(
// 			Ogre2Atlas(mCamera->getPosition()),
// 			Ogre2Atlas(mCamera->getOrientation()));
// 	}
// #endif
	return true;
}

void AvatarCamera::pushWorldPickListener(IWorldPickListener* worldPickListener)
{
	mPickListeners.push_front(worldPickListener);
}

const std::string AvatarCamera::_takeScreenshot() 
{
	// retrieve current time
	time_t rawtime;
	struct tm* timeinfo;
	time(&rawtime);
	timeinfo = localtime(&rawtime);
	
	// construct filename string
	// padding with 0 for single-digit values
	std::stringstream filename;
	filename << "screenshot_" << ((*timeinfo).tm_year + 1900); // 1900 is year "0"
	int month = ((*timeinfo).tm_mon + 1); // January is month "0"
	if(month <= 9) 
	{
		filename << "0";	
	}
	filename << month;
	int day = (*timeinfo).tm_mday;
	if(day <= 9) 
	{
		filename << "0";	
	}
	filename << day << "_";
	int hour = (*timeinfo).tm_hour;
	if(hour <= 9) 
	{
		filename << "0"; 
	}
	filename << hour;
	int min = (*timeinfo).tm_min;
	if(min <= 9) 
	{
		filename << "0";	 
	}
	filename << min;
	int sec = (*timeinfo).tm_sec;
	if(sec <= 9) 
	{
		filename << "0";
	} 
	filename << sec << ".jpg";

	const std::string dir = Ember::EmberServices::getSingletonPtr()->getConfigService()->getHomeDirectory() + "/screenshots/";
	try {
		//make sure the directory exists
		
		oslink::directory osdir(dir);

		if (!osdir.isExisting()) {
#ifdef __WIN32__
			mkdir(dir.c_str());
#else
			mkdir(dir.c_str(), S_IRWXU);
#endif
		}
	} catch (const std::exception& ex) {
		S_LOG_FAILURE("Error when creating directory for screenshots. Message: " << std::string(ex.what()));
		throw Ember::Exception("Error when saving screenshot. Message: " + std::string(ex.what()));
	}
	
	try {
		// take screenshot
		mWindow->writeContentsToFile(dir + filename.str());
	} catch (const Ogre::Exception& ex) {
		S_LOG_FAILURE("Could not write screenshot to disc. Message: "<< ex.getFullDescription());
		throw Ember::Exception("Error when saving screenshot. Message: " + ex.getDescription());
	}
	return dir + filename.str();
}

void AvatarCamera::takeScreenshot()
{
	try {
		const std::string& result = _takeScreenshot();
		S_LOG_INFO(result);
		Ember::ConsoleBackend::getSingletonPtr()->pushMessage("Wrote image: " + result);
	} catch (const Ember::Exception& ex) {
		Ember::ConsoleBackend::getSingletonPtr()->pushMessage("Error when saving screenshot: " + ex.getError());
	}
}

}





