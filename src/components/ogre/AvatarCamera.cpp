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
#include "EmberEntityFactory.h"
#include "WorldEmberEntity.h"
#include "MathConverter.h"

//#include "EmberSceneManager/include/EmberTerrainSceneManager.h"

#include "services/EmberServices.h"
#include "services/config/ConfigService.h"
#include "services/sound/SoundService.h"

#include "MousePicker.h"
#include "jesus/JesusPickerObject.h"

#include "EmberEntityUserObject.h"
#include "ogreopcode/include/OgreCollisionObject.h"
#include "ogreopcode/include/OgreCollisionManager.h"
#include "ogreopcode/include/OgreCollisionShape.h"
#include "model/Model.h"

#include "SceneManagers/EmberPagingSceneManager/include/OgrePagingLandScapeRaySceneQuery.h"
#include "framework/Tokeniser.h"
#include "framework/ConsoleBackend.h"

#include "GUIManager.h"
#include "input/Input.h"


namespace EmberOgre {
 
const std::string AvatarCamera::SETCAMERADISTANCE("setcameradistance");


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
	mCamera(camera)
//	mLastOrientationOfTheCamera(avatar->getOrientation())
{
	createNodesForCamera();
	setAvatarNode(avatarNode);

	// Register this as a frame listener
	Ogre::Root::getSingleton().addFrameListener(this);

	
	if (mGUIManager && mGUIManager->getInput()) {
		mGUIManager->getInput()->EventMouseMoved.connect(sigc::mem_fun(*this, &AvatarCamera::Input_MouseMoved));
	}

	
	if (Ember::EmberServices::getSingletonPtr()->getConfigService()->itemExists("input", "cameradegreespersecond")) {
		mDegreeOfPitchPerSecond = mDegreeOfYawPerSecond = (double)Ember::EmberServices::getSingletonPtr()->getConfigService()->getValue("input", "cameradegreespersecond");
	}
	
	Ember::ConsoleBackend::getMainConsole()->registerCommand(SETCAMERADISTANCE,this);

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

Ogre::Quaternion AvatarCamera::getOrientation(bool onlyHorizontal) const {
	if (!onlyHorizontal) {
		return getCamera()->getWorldOrientation();
	} else {
		Quaternion quat = getCamera()->getWorldOrientation();
		quat.x = 0;
		quat.z = 0;
		return quat;
	}
}

Ogre::Vector3 AvatarCamera::getPosition() const
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

void AvatarCamera::createViewPort()
{
// 	assert(mCamera);
// 	assert(!mViewPort);
//     // Create 1st person viewport, entire window
//     mViewPort = mWindow->addViewport(mCamera);
//     mViewPort->setBackgroundColour(Ogre::ColourValue(0,0,0));
//     mCamera->setAspectRatio(
// 		Ogre::Real(mViewPort->getActualWidth()) / Ogre::Real(mViewPort->getActualHeight()));

	
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

bool AvatarCamera::pickInTerrain(Ogre::Real mouseX, Ogre::Real mouseY, Ogre::Vector3& resultVector)
{
	// Start a new ray query 
	Ogre::Ray cameraRay = getCamera()->getCameraToViewportRay( mouseX, mouseY ); 
	Ogre::RaySceneQuery *raySceneQuery = EmberOgre::getSingletonPtr()->getSceneManager()->createRayQuery( cameraRay , ~ EmberEntity::CM_AVATAR); 
	
	
	raySceneQuery->setRay(cameraRay);
	RaySceneQueryResult& qryResult = raySceneQuery->execute();
	RaySceneQueryResult::iterator i = qryResult.begin();
	if (i != qryResult.end() && i->worldFragment)
	{
		SceneQuery::WorldFragment* wf = i->worldFragment;
		resultVector = i->worldFragment->singleIntersection;
		return true;
	} 
	return false;
}

std::vector<Ogre::RaySceneQueryResultEntry> AvatarCamera::pickObject(Ogre::Real mouseX, Ogre::Real mouseY, std::vector<Ogre::UserDefinedObject*> exclude, unsigned long querymask )
{
	S_LOG_INFO("Trying to pick an entity at mouse coords: "  << Ogre::StringConverter::toString(mouseX) << ":" << Ogre::StringConverter::toString(mouseY) << ".");

	querymask |= Ogre::RSQ_Entities;
	std::vector<Ogre::RaySceneQueryResultEntry> finalResult;
	
	// Start a new ray query 
	Ogre::Ray cameraRay = getCamera()->getCameraToViewportRay( mouseX, mouseY ); 

	Ogre::RaySceneQuery *raySceneQuery = EmberOgre::getSingletonPtr()->getSceneManager()->createRayQuery( cameraRay , querymask); 
	
	raySceneQuery->setSortByDistance(true);
	raySceneQuery->execute(); 
	Ogre::RaySceneQueryResult result = raySceneQuery->getLastResults(); 
	   
	//T *closestObject = *ptrClosestObject; 
	Ogre::Real closestDistance = mClosestPickingDistance;
	 
	std::list< Ogre::RaySceneQueryResultEntry >::iterator rayIterator; 
	std::list< Ogre::RaySceneQueryResultEntry >::iterator rayIterator_end; 
	
	std::vector<Ogre::UserDefinedObject*>::iterator excludeStart = exclude.begin();
	std::vector<Ogre::UserDefinedObject*>::iterator excludeEnd = exclude.end();

	rayIterator = result.begin( );
	rayIterator_end = result.end( );
	if (rayIterator != rayIterator_end) {
		for ( ; 
			rayIterator != rayIterator_end; 
			rayIterator++ ) {
			//only pick entities that have a userobject attached

			Ogre::MovableObject* movable = ( *rayIterator ).movable;
			
// 			if (movable && movable->getUserObject() != 0 && (movable->getQueryFlags() & ~EmberEntity::CM_AVATAR)) {
			if (movable && movable->isVisible() && movable->getUserObject()) {
				//check that it's not in the exclude list
				bool isNotInInclude = true;
				for (std::vector<Ogre::UserDefinedObject*>::iterator exclude_I = excludeStart; exclude_I != excludeEnd; ++exclude_I) {
					if (*exclude_I == movable->getUserObject()) {
						isNotInInclude = true;
					}
				}
				
				if (isNotInInclude) {
					if ( ( *rayIterator ).distance < mClosestPickingDistance ) { 
						finalResult.push_back(*rayIterator);
					}
				}
			}
		} 
	}

	if ( finalResult.size() == 0 ) {    

		S_LOG_INFO("Picked none.");
	} else { 
		S_LOG_INFO("Picked an entity.");
	} 		
	
	return finalResult;
	
}


EntityPickResult AvatarCamera::pickAnEntity(Ogre::Real mouseX, Ogre::Real mouseY) 
{
	S_LOG_INFO("Trying to pick an entity at mouse coords: "  << Ogre::StringConverter::toString(mouseX) << ":" << Ogre::StringConverter::toString(mouseY) << ".");

	EntityPickResult result;
	result.entity = 0;
	
	Ogre::MovableObject *closestObject; 
	Ogre::Real closestDistance = mClosestPickingDistance;
	 
	
	EmberEntityUserObject* userObject = 0;
	
	EmberEntity* pickedEntity;
	Ogre::Real pickedDistance = mClosestPickingDistance;
	Ogre::Vector3 pickedPosition;
	Ogre::MovableObject* pickedMovable;
	
	
	// Start a new ray query 
	Ogre::Ray cameraRay = getCamera()->getCameraToViewportRay( mouseX, mouseY ); 

	
	Ogre::RaySceneQuery *raySceneQueryTerrain = EmberOgre::getSingletonPtr()->getSceneManager()->createRayQuery( cameraRay, Ogre::RSQ_FirstTerrain); 
	
	
	unsigned long queryMask = Ogre::RSQ_Entities;
	queryMask |= EmberEntity::CM_AVATAR;
	queryMask |= EmberEntity::CM_ENTITY;
	queryMask |= EmberEntity::CM_NATURE;
	queryMask |= EmberEntity::CM_UNDEFINED;
	Ogre::RaySceneQuery *raySceneQueryEntities = EmberOgre::getSingletonPtr()->getSceneManager()->createRayQuery( cameraRay, queryMask); 

	raySceneQueryTerrain->execute(); 
	raySceneQueryEntities->execute(); 
	
	//first check the terrain picking
	Ogre::RaySceneQueryResult queryResult = raySceneQueryTerrain->getLastResults(); 
	
	std::list< Ogre::RaySceneQueryResultEntry >::iterator rayIterator = queryResult.begin( ); 
	std::list< Ogre::RaySceneQueryResultEntry >::iterator rayIterator_end = queryResult.end( );
	if (rayIterator != rayIterator_end) {
		//only need to check the first result since we're using RSQ_FirstTerrain for the terrain picking
		if (rayIterator->worldFragment) {
			SceneQuery::WorldFragment* wf = rayIterator->worldFragment;
			pickedPosition = wf->singleIntersection;
				
			Vector3 distance = cameraRay.getOrigin() - pickedPosition; 
			pickedDistance = distance.length(); 
			pickedEntity = EmberOgre::getSingleton().getEntityFactory()->getWorld();
			
			if (pickedDistance < closestDistance) {
				closestDistance = pickedDistance; 
				result.entity = pickedEntity;
				result.position = pickedPosition;
				result.distance = pickedDistance;
			}
					
		} 		
	}
	   
	//now check the entity picking
	queryResult = raySceneQueryEntities->getLastResults(); 
			
	rayIterator = queryResult.begin( ); 
	rayIterator_end = queryResult.end( );
	if (rayIterator != rayIterator_end) {
		for ( ; rayIterator != rayIterator_end; rayIterator++ ) {

			//check if it's a world fragment (i.e. the terrain) of if it has a MovableObject* attached to it
			
			pickedMovable = rayIterator->movable;
			
//			if (pickedMovable && pickedMovable->isVisible() && pickedMovable->getUserObject() != 0 && (pickedMovable->getQueryFlags() & ~EmberEntity::CM_AVATAR) && pickedMovable->getUserObject()->getTypeName() == "EmberEntityPickerObject") {
			if (pickedMovable && pickedMovable->isVisible() && pickedMovable->getUserObject() != 0 && pickedMovable->getUserObject()->getTypeName() == "EmberEntityPickerObject") {
				if ( rayIterator->distance < closestDistance ) { 
					bool isColliding = false;
					closestObject = pickedMovable; 
					EmberEntityUserObject* aUserObject = static_cast<EmberEntityUserObject*>(pickedMovable->getUserObject());
					
					EmberEntityUserObject::CollisionObjectStore* collisionObjects = aUserObject->getCollisionObjects();
					//only do opcode detection if there's a CollisionObject
					for (EmberEntityUserObject::CollisionObjectStore::iterator I = collisionObjects->begin(); I != collisionObjects->end() && !isColliding; ++I) {
						OgreOpcode::CollisionShape* collisionShape = (*I)->GetShape();
						OgreOpcode::CollisionPair pick_result;
						
						isColliding = collisionShape->RayCheck(OgreOpcode::COLLTYPE_QUICK,aUserObject->getModel()->_getParentNodeFullTransform(),cameraRay, mClosestPickingDistance, pick_result);
						if (isColliding) {
							Vector3 distance = cameraRay.getOrigin() - pick_result.contact ; 
							pickedDistance = distance.length(); 
							pickedEntity = aUserObject->getEmberEntity();
							pickedPosition = pick_result.contact;
						}
					}
				}
			}

			if (pickedDistance < closestDistance) {
				closestDistance = pickedDistance; 
				result.entity = pickedEntity;
				result.position = pickedPosition;
				result.distance = pickedDistance;
			}
			
		} 
	}

 
	return result;
}

	bool AvatarCamera::worldToScreen(Ogre::Vector3& worldPos, Ogre::Vector3& screenPos) 
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
		if(command == SETCAMERADISTANCE)
		{
			Ember::Tokeniser tokeniser;
			tokeniser.initTokens(args);
			std::string distance = tokeniser.nextToken();
			if (distance != "") {
				float fDistance = Ogre::StringConverter::parseReal(distance);
				setCameraDistance(fDistance);
			}
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

}





