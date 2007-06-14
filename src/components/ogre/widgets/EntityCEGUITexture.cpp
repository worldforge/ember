//
// C++ Implementation: EntityCEGUITexture
//
// Description: 
//
//
// Author: Erik Hjortsberg <erik@katastrof.nu>, (C) 2005
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
// 
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.//
//
#include "EntityCEGUITexture.h"
#include <CEGUIWindowManager.h>
#include <CEGUIImagesetManager.h> 
#include <CEGUIImageset.h> 

#include "../EmberOgre.h"
#include "../GUIManager.h"
#include "../model/Model.h"

#include "framework/Exception.h"
#include <OgreBitwise.h>


namespace EmberOgre {



EntityCEGUITexture::EntityCEGUITexture(const std::string& imageSetName, int width, int height)
: mWidth(width), mHeight(height), mSceneManager(0), mRenderTexture(0), mRootNode(0), mCamera(0)
{

	S_LOG_VERBOSE("Creating new EntityCEGUITexture for imageset " << imageSetName  << " with w:" << mWidth << " h:" << mHeight);
	mSceneManager = Ogre::Root::getSingleton().createSceneManager(Ogre::ST_GENERIC, imageSetName + "_sceneManager");

	mRootNode = mSceneManager->getRootSceneNode();
	
	
	mEntityNode = mRootNode->createChildSceneNode();

	///make the cameranode a child of the main entity node
	mCameraNode = mRootNode->createChildSceneNode();
	
	mCameraPitchNode = mCameraNode->createChildSceneNode();
	
	createCamera(imageSetName);
	createImage(imageSetName);
	//setVisible(false);
	mMainLight = mSceneManager->createLight("MainLight");
  	mMainLight->setType(Ogre::Light::LT_DIRECTIONAL);
	mMainLight->setDirection(Ogre::Vector3(-1,0,0));
	mSceneManager->setAmbientLight(Ogre::ColourValue(0.5, 0.5, 0.5));
	mCameraPitchNode->attachObject(mMainLight);
	
	resetCameraOrientation();
}


EntityCEGUITexture::~EntityCEGUITexture()
{
/*	if (mCamera) {
		mSceneManager->destroyCamera(mCamera);
	}*/
/*	if (mRootNode) {
		mRootNode->removeAndDestroyAllChildren();
		mSceneManager->destroySceneNode(mRootNode->getName());
	}*/
	Ogre::Root::getSingleton().destroySceneManager(mSceneManager);
}

Ogre::SceneNode* EntityCEGUITexture::getSceneNode() const
{
	return mEntityNode;
}

const CEGUI::Image* EntityCEGUITexture::getImage() const
{
	return mImage;
}

Ogre::Camera* EntityCEGUITexture::getCamera() const
{
	return mCamera;
}

void EntityCEGUITexture::setActive(bool active)
{
	if (mRenderTexture) {
		mRenderTexture->setActive(active);
	}
}

void EntityCEGUITexture::repositionCamera()
{
	mEntityNode->_update(true, true);
	Ogre::AxisAlignedBox bbox = mEntityNode->_getWorldAABB();
	if (!bbox.isInfinite() && !bbox.isNull()) {
		Ogre::Vector3 center = bbox.getCenter();
		Ogre::Vector3 localCenter =  center - mRootNode->getPosition();
		mCameraNode->setPosition(localCenter);
	}
}

void EntityCEGUITexture::createCamera(const std::string& imageSetName)
{	
	mCamera = mSceneManager->createCamera(imageSetName + "_EntityCEGUITextureCamera");

	mCameraPitchNode->attachObject(mCamera);
}

void EntityCEGUITexture::pitch(Ogre::Degree degrees)
{
	mCameraPitchNode->pitch(degrees);
}
void EntityCEGUITexture::yaw(Ogre::Degree degrees)
{
	mCameraNode->yaw(degrees);
}
void EntityCEGUITexture::roll(Ogre::Degree degrees)
{
	mCameraNode->roll(degrees);
}
	
	
float EntityCEGUITexture::getCameraDistance() const
{
	if (mDefaultCameraDistance) { 
		return mCamera->getPosition().z / mDefaultCameraDistance;
	}
	return mCamera->getPosition().z;
}

void EntityCEGUITexture::setCameraDistance(Ogre::Real distance)
{
	if (distance != 0) {
		Ogre::Real cameraDistance = Ogre::Math::Abs(distance) / 100;
		if (cameraDistance > 0) {
			mCamera->setNearClipDistance(Ogre::Math::Abs(distance) / 100);
		} else {
			mCamera->setNearClipDistance(0.01f);
		}
		mCamera->setFarClipDistance((Ogre::Math::Abs(distance) + mDefaultCameraDistance));
	}
	Ogre::Vector3 pos(0,0,distance);
	mCamera->setPosition(pos);
}

Ogre::Quaternion EntityCEGUITexture::getEntityRotation()
{
	return mCamera->getDerivedOrientation().Inverse();
}

void EntityCEGUITexture::resetCameraOrientation()
{
	mCameraPitchNode->setOrientation(Ogre::Quaternion::IDENTITY);
	mCameraNode->setOrientation(Ogre::Quaternion::IDENTITY);

}

void EntityCEGUITexture::createImage(const std::string& imageSetName)
{

	if (mWidth == 0 || mHeight == 0) {
		throw Ember::Exception("Height and width of the image can't be 0.");
	}
	
	Ogre::Real aspectRatio = static_cast<float>(mWidth) / static_cast<float>(mHeight);
	
	///the width and height needs to be multipes of 2
	mWidth = Ogre::Bitwise::firstPO2From(mWidth); 
	mHeight = Ogre::Bitwise::firstPO2From(mHeight); 
	
	
	///first, create a RenderTexture to which the Ogre renderer should render the image
	S_LOG_VERBOSE("Creating new rendertexture " << (imageSetName + "_EntityCEGUITextureRenderTexture") << " with w:" << mWidth << " h:" << mHeight);
	Ogre::TexturePtr texPtr = Ogre::TextureManager::getSingleton().createManual(imageSetName + "_EntityCEGUITextureRenderTexture", "Gui", Ogre::TEX_TYPE_2D, mWidth, mHeight, 0, Ogre::PF_A8R8G8B8,Ogre::TU_RENDERTARGET);
	if (texPtr.isNull()) {
		S_LOG_WARNING("Could not create a texture.");
		return;
	}
	
	
	mRenderTexture = texPtr->getBuffer()->getRenderTarget();
	mRenderTexture->setAutoUpdated(false);
	
	S_LOG_VERBOSE("Removing viewports.");
	mRenderTexture->removeAllViewports();
	///initially deactivate it until setActive(true) is called
	mRenderTexture->setActive(false);
	S_LOG_VERBOSE("Setting aspect ratio of camera to " << aspectRatio);
	mCamera->setAspectRatio(aspectRatio);
	///make sure the camera renders into this new texture
	S_LOG_VERBOSE("Adding camera.");
	Ogre::Viewport *v = mRenderTexture->addViewport(mCamera);
	///this should preferrably be a transparent background, so that CEGUI could itself decide what to show behind it, but alas I couldn't get it to work, thus black
	v->setBackgroundColour(Ogre::ColourValue::Black);
	///don't show the CEGUI
	v->setOverlaysEnabled(false);
	///the cegui renderer wants a TexturePtr (not a RenderTexturePtr), so we just ask the texturemanager for texture we just created (rttex)
	
	///create a CEGUI texture from our Ogre texture
	S_LOG_VERBOSE("Creating new CEGUI texture from Ogre texture.");
	CEGUI::Texture* ceguiTexture = GUIManager::getSingleton().getGuiRenderer()->createTexture(texPtr);
	
	///we need a imageset in order to create GUI elements from the ceguiTexture
	S_LOG_VERBOSE("Creating new CEGUI imageset with name " << imageSetName + "_EntityCEGUITextureImageset");
	mImageSet = CEGUI::ImagesetManager::getSingleton().createImageset(imageSetName + "_EntityCEGUITextureImageset", ceguiTexture);
	
	///we only want one element: the whole texture
	mImageSet->defineImage("full_image", CEGUI::Rect(0,0,mWidth,mHeight), CEGUI::Point(0,0));

	///assign our image element to the StaticImage widget
	mImage = &mImageSet->getImage("full_image");
	

}

void EntityCEGUITexture::showFull(const Ogre::MovableObject* object)
{
	///only do this if there's an active object
	if (object) {
		mEntityNode->_update(true, true);
		Ogre::Real distance = object->getBoundingRadius() / Ogre::Math::Tan(mCamera->getFOVy() / 2);
		///we can't have a distance of 0
		if (distance == 0) {
			distance = 1;
		}
		Ogre::Real distanceNudge = distance / 100;
		distance += distanceNudge;
		
		mDefaultCameraDistance = distance;
		
		setCameraDistance(distance);
	}
	
}

Ogre::RenderTexture* EntityCEGUITexture::getRenderTexture()
{
	return mRenderTexture;
}





}
