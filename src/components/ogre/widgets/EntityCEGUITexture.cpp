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
#include <elements/CEGUIStaticImage.h>

#include "../EmberOgre.h"
#include "../GUIManager.h"
#include "../model/Model.h"

#include "framework/Exception.h"


namespace EmberOgre {



EntityCEGUITexture::EntityCEGUITexture(const std::string& imageSetName, int width, int height)
: mWidth(width), mHeight(height), mCamera(0), mRootNode(0)
{
	S_LOG_VERBOSE("Creating new EntityCEGUITexture for imageset " << imageSetName  << " with w:" << mWidth << " h:" << mHeight);

	mRootNode = mSceneManager.getRootSceneNode();
	
	
	mEntityNode = mRootNode->createChildSceneNode();

	///make the cameranode a child of the main entity node
	mCameraNode = mRootNode->createChildSceneNode();
    ///we need to rotate to make it look at the base when we later change the distance
    mCameraNode->rotate(Ogre::Vector3::UNIT_Y,(Ogre::Degree)-90);
	
	mCameraPitchNode = mCameraNode->createChildSceneNode();
	
	createCamera(imageSetName);
	createImage(imageSetName);
	//setVisible(false);
	mMainLight = mSceneManager.createLight("MainLight");
  	mMainLight->setType(Ogre::Light::LT_DIRECTIONAL);
	mMainLight->setDirection(Ogre::Vector3(-1,0,0));
	mSceneManager.setAmbientLight(Ogre::ColourValue(0.5, 0.5, 0.5));
	mCameraPitchNode->attachObject(mMainLight);
}


EntityCEGUITexture::~EntityCEGUITexture()
{
	if (mCamera) {
		mSceneManager.removeCamera(mCamera);
	}
	if (mRootNode) {
		mRootNode->removeAndDestroyAllChildren();
		mSceneManager.destroySceneNode(mRootNode->getName());
	}
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
	mRenderTexture->setActive(active);
}

void EntityCEGUITexture::repositionCamera()
{
	mEntityNode->_update(true, true);
	Ogre::AxisAlignedBox bbox = mEntityNode->_getWorldAABB();
	Ogre::Vector3 center = bbox.getCenter();
	Ogre::Vector3 localCenter =  center - mRootNode->getPosition();
	mCameraNode->setPosition(localCenter);
}

void EntityCEGUITexture::createCamera(const std::string& imageSetName)
{	
	mCamera = mSceneManager.createCamera(imageSetName + "_EntityCEGUITextureCamera");

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

void EntityCEGUITexture::setCameraDistance(Ogre::Real distance)
{
	if (distance != 0) {
		mCamera->setNearClipDistance(Ogre::Math::Abs(distance) / 100);
		mCamera->setFarClipDistance((Ogre::Math::Abs(distance) + mDefaultCameraDistance));
	}
	Ogre::Vector3 pos(0,0,distance);
	mCamera->setPosition(pos);
}

void EntityCEGUITexture::createImage(const std::string& imageSetName)
{

	if (mWidth == 0 || mHeight == 0) {
		throw Ember::Exception("Height and width of the image can't be 0.");
	}
	
	Ogre::Real aspectRatio = static_cast<float>(mWidth) / static_cast<float>(mHeight);
	
	///the width and height needs to be multipes of 2
	///perhaps there's a better way to do this? bitwise shifts?
	int finalWidth = 1, finalHeight = 1;
	while (finalWidth < mWidth) {
		finalWidth *= 2;
	}
	while (finalHeight < mHeight) {
		finalHeight *= 2;
	}
	mWidth = finalWidth;
	mHeight = finalHeight;
	
	
	///first, create a RenderTexture to which the Ogre renderer should render the image
	S_LOG_VERBOSE("Creating new rendertexture " << (imageSetName + "_EntityCEGUITextureRenderTexture") << " with w:" << mWidth << " h:" << mHeight);
	mRenderTexture = EmberOgre::getSingleton().getOgreRoot()->getRenderSystem()->createRenderTexture(imageSetName + "_EntityCEGUITextureRenderTexture", mWidth, mHeight );
	S_LOG_VERBOSE("Removing viewports.");
	mRenderTexture->removeAllViewports();
	mRenderTexture->setActive(false);
	
	S_LOG_VERBOSE("Setting aspect ratio of camera to " << aspectRatio);
	mCamera->setAspectRatio(aspectRatio);

	///make sure the camera renders into this new texture
	S_LOG_VERBOSE("Adding camera.");
	Ogre::Viewport *v = mRenderTexture->addViewport(mCamera );
	///transparent background
	v->setBackgroundColour(Ogre::ColourValue(0,0,0,0));
	///don't show the CEGUI
	v->setOverlaysEnabled(false);
	///the cegui renderer wants a TexturePtr (not a RenderTexturePtr), so we just ask the texturemanager for texture we just created (rttex)
	S_LOG_VERBOSE("Creating new Ogre texture with name " << mRenderTexture->getName());
	Ogre::TexturePtr texPtr = Ogre::TextureManager::getSingleton().getByName(mRenderTexture->getName());
	
	///create a CEGUI texture from our Ogre texture
	S_LOG_VERBOSE("Creating new CEGUI texture from Ogre texture.");
	CEGUI::Texture* ceguiTexture = GUIManager::getSingleton().getGuiRenderer()->createTexture(texPtr);
	
	///we need a imageset in order to create GUI elements from the ceguiTexture
	S_LOG_VERBOSE("Creating new CEGUI imageset with name " << imageSetName + "_EntityCEGUITextureImageset");
	mImageSet = CEGUI::ImagesetManager::getSingleton().createImageset(imageSetName + "_EntityCEGUITextureImageset", ceguiTexture);
	
	///we only want one element: the whole texture
	mImageSet->defineImage("full", CEGUI::Rect(0,0,mWidth,mHeight), CEGUI::Point(0,0));
	
	///assign our image element to the StaticImage widget
	mImage = &mImageSet->getImage("full");
	

}

void EntityCEGUITexture::showFull(const Ogre::MovableObject* object)
{
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

/*		Ogre::Vector3 position = Ogre::Vector3::ZERO;
		
		position.y = position.y;
		position.x = position.x;
		position.z = distance;
		mDefaultCameraPosition = position;
		mCamera->setPosition(position);*/
	
/*	Ogre::AxisAlignedBox bbox = mEntityNode->_getWorldAABB();
	Ogre::Vector3 center = bbox.getCenter();
	
	mCamera->lookAt(center);*/
	
	
	
	
/*	Ogre::Vector3 screenCoords;
	bool result = false;
	//check what the new position is in screen coords
	result = EmberOgre::getSingletonPtr()->getMainCamera()->worldToScreen(entityWorldCoords, screenCoords);*/
	
}






}
