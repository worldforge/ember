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


#include "EmberSceneManager/include/EmberTerrainSceneManager.h"

#include "services/EmberServices.h"
#include "services/config/ConfigService.h"

#include "jesus/JesusPickerObject.h"

namespace EmberOgre {

AvatarCamera::AvatarCamera(Ogre::SceneNode* avatarNode, Ogre::SceneManager* sceneManager, Ogre::RenderWindow* window, GUIManager* guiManager) :
	mSceneManager(sceneManager),
	mWindow(window),
	mGUIManager(guiManager),
	degreePitch(0),
	degreeYaw(0),
	mViewPort(0),
	mDegreeOfPitchPerSecond(50),
	mDegreeOfYawPerSecond(50),
	mClosestPickingDistance(10000),
	mAvatarNode(0)
//	mLastOrientationOfTheCamera(avatar->getOrientation())
{
	createNodesAndCamera();
	setAvatarNode(avatarNode);
	
	if (mGUIManager && mGUIManager->getInput()) {
		mGUIManager->getInput()->MouseMoved.connect(SigC::slot(*this, &AvatarCamera::mouseMoved));
	}

	
	if (Ember::EmberServices::getInstance()->getConfigService()->itemExists("input", "cameradegreespersecond")) {
		mDegreeOfPitchPerSecond = mDegreeOfYawPerSecond = (double)Ember::EmberServices::getInstance()->getConfigService()->getValue("input", "cameradegreespersecond");
	}
	 
}

AvatarCamera::~AvatarCamera()
{}

void AvatarCamera::createNodesAndCamera()
{
	//create the nodes for the camera
	
	mAvatarCameraRootNode = static_cast<Ogre::SceneNode*>(mSceneManager->createSceneNode("AvatarCameraRootNode"));
	//we need to adjust for the height of the avatar mesh
	mAvatarCameraRootNode->setPosition(Ogre::Vector3(0,2,0));
	//rotate to sync with WF world
    mAvatarCameraRootNode->rotate(Ogre::Vector3::UNIT_Y,(Ogre::Degree)-90);

	mAvatarCameraPitchNode = static_cast<Ogre::SceneNode*>(mAvatarCameraRootNode->createChild("AvatarCameraPitchNode"));
	mAvatarCameraPitchNode->setPosition(Ogre::Vector3(0,0,0));
	mAvatarCameraNode = static_cast<Ogre::SceneNode*>(mAvatarCameraPitchNode->createChild("AvatarCameraNode"));
	Ogre::Vector3 pos(0,0,10);
	mAvatarCameraNode->setPosition(pos);
	
	mCamera = mSceneManager->createCamera("AvatarCamera");
	mAvatarCameraNode->attachObject(mCamera);
	// Look to the Avatar's head
	//mAvatar3pCamera->setAutoTracking(true, mAvatar1pCameraNode);
	mCamera->setNearClipDistance(0.01);
	if (Ogre::Root::getSingleton().getRenderSystem()->getCapabilities()->hasCapability(Ogre::RSC_INFINITE_FAR_PLANE))
	{
		mCamera->setFarClipDistance(0);
	} else {
		mCamera->setFarClipDistance(6000);
	}
	
	createViewPort();
}

void AvatarCamera::createViewPort()
{
	assert(mCamera);
	assert(!mViewPort);
    // Create 1st person viewport, entire window
    mViewPort = mWindow->addViewport(mCamera);
    mViewPort->setBackgroundColour(Ogre::ColourValue(0,0,0));
    mCamera->setAspectRatio(
		Ogre::Real(mViewPort->getActualWidth()) / Ogre::Real(mViewPort->getActualHeight()));

	
}

void AvatarCamera::setAvatarNode(Ogre::SceneNode* sceneNode)
{
	if (mAvatarNode) {
		mAvatarNode->removeChild(mAvatarCameraRootNode->getName());
	}
	mAvatarNode = sceneNode;
	mAvatarNode->addChild(mAvatarCameraRootNode);
/*	const Ogre::AxisAlignedBox ogreBoundingBox = mAvatarNode->_getWorldAABB();
	const Ogre::Vector3 ogreMax = ogreBoundingBox.getMaximum();
	
	mAvatarCameraRootNode->setPosition(Ogre::Vector3(0,ogreMax.y,0));*/
}


void AvatarCamera::pitch(Ogre::Degree degrees)
{
	degreePitch += degrees;
	mAvatarCameraPitchNode->pitch(degrees);
}
void AvatarCamera::yaw(Ogre::Degree degrees)
{
	degreeYaw += degrees;
	mAvatarCameraRootNode->yaw(degrees);
	
	std::stringstream ss;
	mAvatarCameraRootNode->needUpdate();
	Ogre::Quaternion q = mAvatarCameraRootNode->_getDerivedOrientation();
	
	
}

void AvatarCamera::mouseMoved(const MouseMotion& motion, bool isInGuimode)
/*(int xPosition, int yPosition, Ogre::Real xRelativeMovement, Ogre::Real yRelativeMovement, Ogre::Real timeSinceLastMovement)*/
{
	if (!isInGuimode) {
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

std::vector<Ogre::RaySceneQueryResultEntry> AvatarCamera::pickObject(Ogre::Real mouseX, Ogre::Real mouseY, std::vector<Ogre::UserDefinedObject*> exclude, unsigned long querymask )
{
	fprintf(stderr, "TRACE - TRYING TO PICK AN ENTITY\n");

	std::vector<Ogre::RaySceneQueryResultEntry> finalResult;
	
	// Start a new ray query 
	Ogre::Ray cameraRay = getCamera()->getCameraToViewportRay( mouseX, mouseY ); 
	//THE GODDAMNED QUERYFLAG DOESN'T WORK!!!!
	//don't know why

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

		fprintf(stderr, "TRACE - PICKED NONE\n");
	} else { 
		fprintf(stderr, "TRACE - PICKED AN OBJECT\n");

	} 		
	
	return finalResult;
	
}


EmberEntity* AvatarCamera::pickAnEntity(Ogre::Real mouseX, Ogre::Real mouseY) 
{
	fprintf(stderr, "TRACE - TRYING TO PICK AN ENTITY\n");

	// Start a new ray query 
	Ogre::Ray cameraRay = getCamera()->getCameraToViewportRay( mouseX, mouseY ); 
	//THE GODDAMNED QUERYFLAG DOESN'T WORK!!!!
	//don't know why
	Ogre::RaySceneQuery *raySceneQuery = EmberOgre::getSingletonPtr()->getSceneManager()->createRayQuery( cameraRay , ~ EmberEntity::CM_AVATAR); 
/*
	std::stringstream temp;
	temp << "tjo1: " <<(EmberEntity::CM_PERSONS ||	EmberEntity::CM_CREATURES  || EmberEntity::CM_NATURE || EmberEntity::CM_UNDEFINED) << "\n";
	temp << "tjo2: " <<(EmberEntity::CM_PERSONS |	EmberEntity::CM_CREATURES  | EmberEntity::CM_NATURE | EmberEntity::CM_UNDEFINED) << "\n";
	fprintf(stderr, temp.str().c_str() );
*/	
//	raySceneQuery->setQueryMask(~ EmberEntity::CM_AVATAR);

	raySceneQuery->execute(); 
	Ogre::RaySceneQueryResult result = raySceneQuery->getLastResults(); 
	   
	Ogre::MovableObject *closestObject = NULL; 
	Ogre::Real closestDistance = mClosestPickingDistance;
	 
	std::list< Ogre::RaySceneQueryResultEntry >::iterator rayIterator; 
	std::list< Ogre::RaySceneQueryResultEntry >::iterator rayIterator_end; 
/*
	std::stringstream temp;
	Ogre::SceneManager::EntityIterator iter = EmberOgre::getSingletonPtr()->getSceneManager()->getEntityIterator();
	int count = 0;
	while (iter.hasMoreElements()) {
		count++;
		Ogre::Entity* ent = iter.getNext();
		temp << ent->getName() << ": " << Ogre::StringConverter::toString(ent->getBoundingBox().getMinimum());
		temp << "/" << Ogre::StringConverter::toString(ent->getWorldBoundingBox().getMinimum());
		temp << " ::: ";
		temp << Ogre::StringConverter::toString(ent->getBoundingBox().getMaximum());
		temp << "/" << Ogre::StringConverter::toString(ent->getWorldBoundingBox().getMaximum());
		temp << "\n";
	}
	fprintf(stderr, temp.str().c_str() );
*/
	//Ogre::UserDefinedObject* avatarObject = dynamic_cast<Ogre::UserDefinedObject*>(mAvatar->mErisAvatarEntity);
	rayIterator = result.begin( );
	rayIterator_end = result.end( );
	if (rayIterator != rayIterator_end) {
		for ( ; 
			rayIterator != rayIterator_end; 
			rayIterator++ ) {
			//only pick entities that have a userobject attached

			Ogre::MovableObject* movable = ( *rayIterator ).movable;
			
			if (movable && movable->isVisible() && movable->getUserObject() != NULL && (movable->getQueryFlags() & ~EmberEntity::CM_AVATAR)) {
				if ( ( *rayIterator ).distance < closestDistance ) { 
					closestObject = movable; 
					closestDistance = ( *rayIterator ).distance ; 
				}
			}
		} 
	}

 
   // No movable clicked 
	if ( closestObject == NULL ) {    

		fprintf(stderr, "TRACE - PICKED NONE\n");
	     return NULL;
		//Root::getSingleton().getAutoCreatedWindow()->setDebugText("none");
	} else { 
		fprintf(stderr, "TRACE - PICKED AN OBJECT\n");
		return  static_cast<EmberEntity*>(closestObject->getUserObject());

	} 		
	
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

}





