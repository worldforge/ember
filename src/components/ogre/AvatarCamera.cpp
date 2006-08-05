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

#include "AvatarCamera.h"
#include "Avatar.h"
#include "GUIManager.h"
#include "EmberOgre.h"
#include "EmberEntity.h"
#include "WorldEmberEntity.h"
#include "MathConverter.h"


#include "services/EmberServices.h"
#include "services/config/ConfigService.h"
#include "services/sound/SoundService.h"

#include "MousePicker.h"
#include "jesus/JesusPickerObject.h"

#include "SceneManagers/EmberPagingSceneManager/include/OgrePagingLandScapeRaySceneQuery.h"
#include "framework/Tokeniser.h"
// #include "framework/ConsoleBackend.h"

#include "GUIManager.h"
#include "input/Input.h"

#include "IWorldPickListener.h"

namespace EmberOgre {
 

AvatarCamera::AvatarCamera(Ogre::SceneNode* avatarNode, Ogre::SceneManager* sceneManager, Ogre::RenderWindow* window, GUIManager* guiManager, Ogre::Camera* camera) :
	mSceneManager(sceneManager),
	mWindow(window),
	mGUIManager(guiManager),
	degreePitch(0),
	degreeYaw(0),
	mViewPort(0),
	mDegreeOfPitchPerSecond(50),
	mDegreeOfYawPerSecond(50),
	mClosestPickingDistance(10000),
	mAvatarNode(0),
	mInvertCamera(false),
	mCamera(camera),
	SetCameraDistance("setcameradistance", this, "Set the distance of the camera."),
	ToggleRendermode("toggle_rendermode", this, "Toggle between wireframe and solid render modes."),
	ToggleFullscreen("toggle_fullscreen", this, "Switch between windowed and full screen mode.")
//	mLastOrientationOfTheCamera(avatar->getOrientation())
{
	createNodesForCamera();
	createViewPort();
	setAvatarNode(avatarNode);

	// Register this as a frame listener
	Ogre::Root::getSingleton().addFrameListener(this);

	
	if (mGUIManager) {
		mGUIManager->getInput().EventMouseMoved.connect(sigc::mem_fun(*this, &AvatarCamera::Input_MouseMoved));
	}

	
	if (Ember::EmberServices::getSingletonPtr()->getConfigService()->itemExists("input", "cameradegreespersecond")) {
		mDegreeOfPitchPerSecond = mDegreeOfYawPerSecond = (double)Ember::EmberServices::getSingletonPtr()->getConfigService()->getValue("input", "cameradegreespersecond");
	}
	
	Ember::EmberServices::getSingletonPtr()->getConfigService()->EventChangedConfigItem.connect(sigc::mem_fun(*this, &AvatarCamera::ConfigService_EventChangedConfigItem));
	
	updateValuesFromConfig();
}

AvatarCamera::~AvatarCamera()
{
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
	mCamera->setNearClipDistance(0.1);
	if (Ogre::Root::getSingleton().getRenderSystem()->getCapabilities()->hasCapability(Ogre::RSC_INFINITE_FAR_PLANE))
	{
		mCamera->setFarClipDistance(1000);
	} else {
		mCamera->setFarClipDistance(1000);
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
		return getCamera()->getWorldOrientation();
	} else {
		static Ogre::Quaternion quat;
		quat = getCamera()->getWorldOrientation();
		quat.x = 0;
		quat.z = 0;
		return quat;
	}
}

const Ogre::Vector3& AvatarCamera::getPosition() const
{
	return mCamera->getWorldPosition();
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

// bool AvatarCamera::pickInTerrain(Ogre::Real mouseX, Ogre::Real mouseY, Ogre::Vector3& resultVector)
// {
// 	// Start a new ray query 
// 	Ogre::Ray cameraRay = getCamera()->getCameraToViewportRay( mouseX, mouseY ); 
// 	Ogre::RaySceneQuery *raySceneQuery = EmberOgre::getSingletonPtr()->getSceneManager()->createRayQuery( cameraRay , ~ EmberEntity::CM_AVATAR); 
// 	
// 	
// 	raySceneQuery->setRay(cameraRay);
// 	RaySceneQueryResult& qryResult = raySceneQuery->execute();
// 	RaySceneQueryResult::iterator i = qryResult.begin();
// 	if (i != qryResult.end() && i->worldFragment)
// 	{
// 		SceneQuery::WorldFragment* wf = i->worldFragment;
// 		resultVector = i->worldFragment->singleIntersection;
// 		return true;
// 	} 
// 	return false;
// }

// std::vector<Ogre::RaySceneQueryResultEntry> AvatarCamera::pickObject(Ogre::Real mouseX, Ogre::Real mouseY, std::vector<Ogre::UserDefinedObject*> exclude, unsigned long querymask )
// {
// 	S_LOG_INFO("Trying to pick an entity at mouse coords: "  << Ogre::StringConverter::toString(mouseX) << ":" << Ogre::StringConverter::toString(mouseY) << ".");
// 
// 	querymask |= Ogre::RSQ_Entities;
// 	std::vector<Ogre::RaySceneQueryResultEntry> finalResult;
// 	
// 	// Start a new ray query 
// 	Ogre::Ray cameraRay = getCamera()->getCameraToViewportRay( mouseX, mouseY ); 
// 
// 	Ogre::RaySceneQuery *raySceneQuery = EmberOgre::getSingletonPtr()->getSceneManager()->createRayQuery( cameraRay , querymask); 
// 	
// 	raySceneQuery->setSortByDistance(true);
// 	raySceneQuery->execute(); 
// 	Ogre::RaySceneQueryResult& result = raySceneQuery->getLastResults(); 
// 	   
// 	//T *closestObject = *ptrClosestObject; 
// 	Ogre::Real closestDistance = mClosestPickingDistance;
// 	 
// 	Ogre::RaySceneQueryResult::iterator rayIterator; 
// 	Ogre::RaySceneQueryResult::iterator rayIterator_end; 
// 	
// 	std::vector<Ogre::UserDefinedObject*>::iterator excludeStart = exclude.begin();
// 	std::vector<Ogre::UserDefinedObject*>::iterator excludeEnd = exclude.end();
// 
// 	rayIterator = result.begin( );
// 	rayIterator_end = result.end( );
// 	if (rayIterator != rayIterator_end) {
// 		for ( ; 
// 			rayIterator != rayIterator_end; 
// 			rayIterator++ ) {
// 			///only pick entities that have a userobject attached
// 
// 			Ogre::MovableObject* movable = ( *rayIterator ).movable;
// 			
// // 			if (movable && movable->getUserObject() != 0 && (movable->getQueryFlags() & ~EmberEntity::CM_AVATAR)) {
// 			if (movable && movable->isVisible() && movable->getUserObject()) {
// 				///check that it's not in the exclude list
// 				bool isExcluded = false;
// 				std::vector<Ogre::UserDefinedObject*>::iterator excluded = std::find(excludeStart, excludeEnd, movable->getUserObject());
// 				if (excluded != excludeEnd) {
// 					isExcluded = true;
// 				}
// 				
// 				if (!isExcluded) {
// 					if ( ( *rayIterator ).distance < mClosestPickingDistance ) { 
// 						finalResult.push_back(*rayIterator);
// 					}
// 				}
// 			}
// 		} 
// 	}
// 
// 	if ( finalResult.size() == 0 ) {    
// 
// 		S_LOG_INFO("Picked none.");
// 	} else { 
// 		S_LOG_INFO("Picked an entity.");
// 	} 		
// 	///this must be destroyed by the scene manager
// 	EmberOgre::getSingleton().getSceneManager()->destroyQuery(raySceneQuery);
// 	return finalResult;
// 	
// }


void AvatarCamera::pickInWorld(Ogre::Real mouseX, Ogre::Real mouseY, const MousePickerArgs& mousePickerArgs)
{
	S_LOG_INFO("Trying to pick an entity at mouse coords: "  << Ogre::StringConverter::toString(mouseX) << ":" << Ogre::StringConverter::toString(mouseY) << ".");

// 	EntityPickResult result;
// 	result.entity = 0;
	
// 	Ogre::MovableObject *closestObject; 
// 	Ogre::Real closestDistance = mClosestPickingDistance;
	 
	
// 	EmberEntityUserObject* userObject = 0;
	
// 	EmberEntity* pickedEntity;
// 	Ogre::Real pickedDistance = mClosestPickingDistance;
// 	Ogre::Vector3 pickedPosition;
// 	Ogre::MovableObject* pickedMovable;
	
	/// Start a new ray query 
	Ogre::Ray cameraRay = getCamera()->getCameraToViewportRay( mouseX, mouseY ); 

	
	//Ogre::RaySceneQuery *raySceneQueryTerrain = EmberOgre::getSingletonPtr()->getSceneManager()->createRayQuery( cameraRay, Ogre::RSQ_FirstTerrain); 
	
	
	unsigned long queryMask = Ogre::RSQ_Entities;
	queryMask |= EmberEntity::CM_AVATAR;
	queryMask |= EmberEntity::CM_ENTITY;
	queryMask |= EmberEntity::CM_NATURE;
	queryMask |= EmberEntity::CM_UNDEFINED;
	queryMask |= Ogre::RSQ_FirstTerrain;
	//queryMask |= Ogre::RSQ_AllTerrain;
	
	Ogre::RaySceneQuery *raySceneQuery = EmberOgre::getSingletonPtr()->getSceneManager()->createRayQuery( cameraRay, queryMask); 
	raySceneQuery->setSortByDistance(true);

	raySceneQuery->execute(); 
//	raySceneQueryEntities->execute(); 
	
	///first check the terrain picking
// 	Ogre::RaySceneQueryResult& queryResult = raySceneQueryTerrain->getLastResults(); 
// 	
// 	Ogre::RaySceneQueryResult::iterator rayIterator = queryResult.begin( ); 
// 	Ogre::RaySceneQueryResult::iterator rayIterator_end = queryResult.end( );
// 	if (rayIterator != rayIterator_end) {
// 		///only need to check the first result since we're using RSQ_FirstTerrain for the terrain picking
// 		if (rayIterator->worldFragment) {
// 			SceneQuery::WorldFragment* wf = rayIterator->worldFragment;
// 			pickedPosition = wf->singleIntersection;
// 				
// 			Vector3 distance = cameraRay.getOrigin() - pickedPosition; 
// 			pickedDistance = distance.length(); 
// 			pickedEntity = EmberOgre::getSingleton().getEntityFactory()->getWorld();
// 			
// 			if (pickedDistance < closestDistance) {
// 				closestDistance = pickedDistance; 
// 				result.entity = pickedEntity;
// 				result.position = pickedPosition;
// 				std::stringstream ss;
// 				ss << pickedPosition;
// 				S_LOG_VERBOSE("Picked in terrain: " << ss.str() << " distance: " << pickedDistance);
// 				result.distance = pickedDistance;
// 			}
// 					
// 		} 		
// 	}
	   
	//now check the entity picking
	Ogre::RaySceneQueryResult& queryResult = raySceneQuery->getLastResults(); 
	bool continuePicking = true;
			
	Ogre::RaySceneQueryResult::iterator rayIterator = queryResult.begin( ); 
	Ogre::RaySceneQueryResult::iterator rayIterator_end = queryResult.end( );
	if (rayIterator != rayIterator_end) {
		for ( ; rayIterator != rayIterator_end && continuePicking; rayIterator++ ) {

			///check if it's a world fragment (i.e. the terrain) of if it has a MovableObject* attached to it
			
// 			pickedMovable = rayIterator->movable;
			
//			if (pickedMovable && pickedMovable->isVisible() && pickedMovable->getUserObject() != 0 && (pickedMovable->getQueryFlags() & ~EmberEntity::CM_AVATAR) && pickedMovable->getUserObject()->getTypeName() == "EmberEntityPickerObject") {
// 			if (pickedMovable && pickedMovable->isVisible() && pickedMovable->getUserObject() != 0) {
				for (WorldPickListenersStore::iterator I = mPickListeners.begin(); I != mPickListeners.end(); ++I) {
					(*I)->processPickResult(continuePicking, *rayIterator, cameraRay, mousePickerArgs);
					if (!continuePicking) {
						break;
					}
				}
// 			}
		}
	}
	///this must be destroyed by the scene manager
	EmberOgre::getSingleton().getSceneManager()->destroyQuery(raySceneQuery);
	
/*			
// 				if ( rayIterator->distance < closestDistance ) { 
					bool isColliding = false;
					closestObject = pickedMovable; 
					EmberEntityUserObject* anUserObject = static_cast<EmberEntityUserObject*>(pickedMovable->getUserObject());
					///refit the opcode mesh to adjust for changes in the mesh (for example animations)
					anUserObject->refit();
					EmberEntityUserObject::CollisionObjectStore* collisionObjects = anUserObject->getCollisionObjects();
					///only do opcode detection if there's a CollisionObject
					for (EmberEntityUserObject::CollisionObjectStore::iterator I = collisionObjects->begin(); I != collisionObjects->end() && !isColliding; ++I) {
						OgreOpcode::ICollisionShape* collisionShape = (*I)->getShape();
						OgreOpcode::CollisionPair pick_result;
						
						isColliding = collisionShape->rayCheck(OgreOpcode::COLLTYPE_QUICK,anUserObject->getModel()->_getParentNodeFullTransform(),cameraRay, mClosestPickingDistance, pick_result);
						if (isColliding) {
							pickedDistance = pick_result.distance; 
							pickedEntity = anUserObject->getEmberEntity();
							pickedPosition = pick_result.contact;
							std::stringstream ss;
							ss << pickedPosition;
							S_LOG_VERBOSE("Picked entity: " << ss.str() << " distance: " << pickedDistance);
							
						}
					}
// 				}
			}

			if (pickedDistance < closestDistance) {
				closestDistance = pickedDistance; 
				result.entity = pickedEntity;
				result.position = pickedPosition;
				result.distance = pickedDistance;
			}
			
		} 
	}*/

 
// 	return result;
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
		
		Ogre::RaySceneQuery *raySceneQueryHeight = EmberOgre::getSingletonPtr()->getSceneManager()->createRayQuery( Ogre::Ray(mCamera->getDerivedPosition(), Ogre::Vector3::NEGATIVE_UNIT_Y), Ogre::RSQ_Height); 
		
		
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
				
			}
			 else if (cameraHeight != cameraNodeHeight) {
				Ogre::Real newHeight = std::max(terrainHeight, cameraNodeHeight);
				mCamera->move(Ogre::Vector3(0,newHeight - cameraHeight,0));
				mCamera->lookAt(mAvatarCameraRootNode->getWorldPosition());
				
			}
			
		}
	
		return true;		
	}
	
void AvatarCamera::runCommand(const std::string &command, const std::string &args)
{
	if(SetCameraDistance == command)
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
	}
}
	
void AvatarCamera::updateValuesFromConfig()
{
	if (Ember::EmberServices::getSingletonPtr()->getConfigService()->itemExists("input", "invertcamera")) {
		mInvertCamera = static_cast<bool>(Ember::EmberServices::getSingletonPtr()->getConfigService()->getValue("input", "invertcamera"));
	}
}
	
void AvatarCamera::ConfigService_EventChangedConfigItem(const std::string& section, const std::string& key)
{
	if (section == "input") {
		if (key == "invertcamera") {
			updateValuesFromConfig();
		}
	}
}

bool AvatarCamera::frameStarted(const Ogre::FrameEvent& event)
{
	Ember::SoundService* mySoundService = Ember::EmberServices::getSingleton().getSoundService();
	{
		mySoundService->updateListenerPosition(
			Ogre2Atlas(mCamera->getPosition()),
			Ogre2Atlas(mCamera->getOrientation()));
	}
	return true;
}

void AvatarCamera::pushWorldPickListener(IWorldPickListener* worldPickListener)
{
	mPickListeners.push_front(worldPickListener);
}


}





