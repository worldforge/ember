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

#include "Avatar.h"
#include "InputManager.h"
#include "GUIManager.h"
#include "DimeOgre.h"
#include "DimeEntity.h"

#include "AvatarCamera.h"

namespace DimeOgre {

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
	mGUIManager->setMouseMotionListener(this);
}

AvatarCamera::~AvatarCamera()
{}

/*bool AvatarCamera::frameStarted(const Ogre::FrameEvent& event)
{
	*
	 * M.O. is to check if we're aligned behind the avatar.
	 * If not, we'll try to ease ourself into the position.
	 *
	SceneNode* sceneNode = static_cast<SceneNode*>(mCamera->getParentNode());
	//mCameraNode->setOrientation(Ogre::Quaternion::Slerp(0.2, mCameraNode->getOrientation(), mLastOrientationOfTheCamera, false);
	//mLastOrientationOfTheCamera = mCameraNode->getOrientation();
	
	return true;	
}
bool AvatarCamera::frameEnded(const Ogre::FrameEvent& event)
{
	return true;
}
*/


void AvatarCamera::createNodesAndCamera()
{
	//create the nodes for the camera
	
	mAvatarCameraRootNode = static_cast<Ogre::SceneNode*>(mSceneManager->createSceneNode("AvatarCameraRootNode"));
	//we need to adjust for the height of the avatar mesh
	mAvatarCameraRootNode->setPosition(WF2OGRE_VECTOR3(0,2,0));
	//rotate to sync with WF world
    mAvatarCameraRootNode->rotate(Ogre::Vector3::UNIT_Y,(Ogre::Degree)180);

	mAvatarCameraPitchNode = static_cast<Ogre::SceneNode*>(mAvatarCameraRootNode->createChild("AvatarCameraPitchNode"));
	mAvatarCameraPitchNode->setPosition(WF2OGRE_VECTOR3(0,0,0));
	mAvatarCameraNode = static_cast<Ogre::SceneNode*>(mAvatarCameraPitchNode->createChild("AvatarCameraNode"));
	Ogre::Vector3 pos = WF2OGRE_VECTOR3(0,0,10);
	mAvatarCameraNode->setPosition(pos);
	
	mCamera = mSceneManager->createCamera("AvatarCamera");
	mAvatarCameraNode->attachObject(mCamera);
	// Look to the Avatar's head
	//mAvatar3pCamera->setAutoTracking(true, mAvatar1pCameraNode);
	mCamera->setNearClipDistance(0.01);
	mCamera->setFarClipDistance(6000);
	
	createViewPort();
}

void AvatarCamera::createViewPort()
{
	assert(mCamera);
	assert(!mViewPort);
    // Create 1st person viewport, entire window
    mViewPort = mWindow->addViewport(mCamera);
    mViewPort->setBackgroundColour(Ogre::ColourValue(0,0,0));
	
}

void AvatarCamera::setAvatarNode(Ogre::SceneNode* sceneNode)
{
	if (mAvatarNode) {
		mAvatarNode->removeChild(mAvatarCameraRootNode->getName());
	}
	mAvatarNode = sceneNode;
	mAvatarNode->addChild(mAvatarCameraRootNode);
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

void AvatarCamera::mouseMoved (Ogre::MouseEvent *e)
{
	Ogre::Degree diffX(50 * e->getRelX());
	Ogre::Degree diffY(50 * e->getRelY());
//	Ogre::Degree diffX = mDegreeOfYawPerSecond * e->getRelX();
//	Ogre::Degree diffY = mDegreeOfPitchPerSecond * e->getRelY();

	//fprintf(stderr, (std::string("X: ") << diffX.valueDegrees << "\n").c_str() );

	if (diffX.valueDegrees()) {
		this->yaw(-diffX);
//		this->yaw(diffX * e->timeSinceLastFrame);
	}
	if (diffY.valueDegrees()) {
		this->pitch(-diffY);
//		this->pitch(diffY * e->timeSinceLastFrame);
	}
	
	if (diffY.valueDegrees() || diffX.valueDegrees()) {
		MovedCamera.emit(mCamera);
	}
}

DimeEntity* AvatarCamera::pickAnEntity(Ogre::Real mouseX, Ogre::Real mouseY) 
{
	fprintf(stderr, "TRACE - TRYING TO PICK AN ENTITY\n");

	// Start a new ray query 
	Ogre::Ray cameraRay = getCamera()->getCameraToViewportRay( mouseX, mouseY ); 
	//THE GODDAMNED QUERYFLAG DOESN'T WORK!!!!
	//don't know why
	Ogre::RaySceneQuery *raySceneQuery = DimeOgre::getSingletonPtr()->getSceneManager()->createRayQuery( cameraRay , ~ DimeEntity::CM_AVATAR); 
/*
	std::stringstream temp;
	temp << "tjo1: " <<(DimeEntity::CM_PERSONS ||	DimeEntity::CM_CREATURES  || DimeEntity::CM_NATURE || DimeEntity::CM_UNDEFINED) << "\n";
	temp << "tjo2: " <<(DimeEntity::CM_PERSONS |	DimeEntity::CM_CREATURES  | DimeEntity::CM_NATURE | DimeEntity::CM_UNDEFINED) << "\n";
	fprintf(stderr, temp.str().c_str() );
*/	
//	raySceneQuery->setQueryMask(~ DimeEntity::CM_AVATAR);

	raySceneQuery->execute(); 
	Ogre::RaySceneQueryResult result = raySceneQuery->getLastResults(); 
	   
	Ogre::MovableObject *closestObject = NULL; 
	Ogre::Real closestDistance = mClosestPickingDistance;
	 
	std::list< Ogre::RaySceneQueryResultEntry >::iterator rayIterator; 
	std::list< Ogre::RaySceneQueryResultEntry >::iterator rayIterator_end; 
/*
	std::stringstream temp;
	Ogre::SceneManager::EntityIterator iter = DimeOgre::getSingletonPtr()->getSceneManager()->getEntityIterator();
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
			
			if (movable && movable->getUserObject() != NULL && (movable->getQueryFlags() & ~DimeEntity::CM_AVATAR)) {
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
		return  dynamic_cast<DimeEntity*>(closestObject->getUserObject());

	} 		
	
}

void AvatarCamera::updateFromMouseMovement(const Ogre::FrameEvent & event, InputManager* inputManager) {

 	
 	/*this is in percent how much of the border areas that are "hot", i.e. makes the 
 	 * view rotate when the mouse moves over them.
 	 * Think of it as a picture frame.
 	 */
/*	const float sizeOfHotBorder = .25;

	//this is in percent how much of the border that makes the movement max out
	const float sizeOfMaxHotBorder = .05;
	
	// Max movement per second. I guess this is in degrees, but I'm not sure. 
	//  Seemed as a nice value though
	 
	const float maxMovement = 50; 
	
	int screenX = Ogre::Root::getSingleton().getAutoCreatedWindow()->getWidth();
	int screenY = Ogre::Root::getSingleton().getAutoCreatedWindow()->getHeight();
	
	//we must use this obscure method, else all other overlays would "obstruct"
	//the mouse pointer, giving us no data
	float mouseX = inputManager->getMouseX();
	float mouseY = inputManager->getMouseY();
	
	float diffX = 0.0, diffY = 0.0;
	
	
	//this calculates how close the pointer is to the border and determines how
	//much we should move
	if (mouseX <= sizeOfHotBorder) {
		diffX = (mouseX <= sizeOfMaxHotBorder) ? (maxMovement) : maxMovement * ((sizeOfHotBorder - mouseX) / sizeOfHotBorder);
	} else if (mouseX >= 1 - sizeOfHotBorder) {
		diffX = (1.0 - mouseX <= sizeOfMaxHotBorder) ? (-maxMovement) : -((sizeOfHotBorder + (mouseX - 1.0)) / sizeOfHotBorder) * maxMovement;
	}	
	if (mouseY <= sizeOfHotBorder) {
		diffY = (mouseY <= sizeOfMaxHotBorder) ? (maxMovement) : maxMovement * ((sizeOfHotBorder - mouseY) / sizeOfHotBorder);
	} else if (mouseY >= 1 - sizeOfHotBorder) {
		diffY = (1.0 - mouseY <= sizeOfMaxHotBorder) ? (-maxMovement) : -((sizeOfHotBorder + (mouseY - 1.0)) / sizeOfHotBorder) * maxMovement;
	}	
			
//	movementForFrame.rotationDegHoriz = diffX;
//	movementForFrame.rotationDegVert = diffY;
//	mAvatar->attemptRotate(diffX * event.timeSinceLastFrame ,diffY * event.timeSinceLastFrame, event.timeSinceLastFrame);
	


	//we do the camera pitch instantly and correct the avatar to the new position 
	//when it's suitable
	if (diffX) {
		this->yaw(diffX * event.timeSinceLastFrame);
	}
	if (diffY) {
		this->pitch(diffY * event.timeSinceLastFrame);
	}
	
	if (diffY || diffX) {
		MovedCamera.emit(mCamera);
	}
//	movementForFrame.cameraOrientation = mAvatar->getAvatarCamera()->getOrienation();
*/
}

}




