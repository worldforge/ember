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
	//this might perhaps be doable in a better way. For now we just position the preview node far, far away
	mRootNode = EmberOgre::getSingleton().getSceneManager()->getRootSceneNode()->createChildSceneNode();
	mRootNode->setPosition(Ogre::Vector3(100000,100000,100000));
	
	mEntityNode = mRootNode->createChildSceneNode();

	//make the cameranode a child of the main entity node
	mCameraNode = mRootNode->createChildSceneNode();
	mCameraNode->setPosition(Ogre::Vector3(0,5,-20));
	createCamera(imageSetName);
	createImage(imageSetName);
	//setVisible(false);

}


EntityCEGUITexture::~EntityCEGUITexture()
{
	if (mCamera) {
		EmberOgre::getSingleton().getSceneManager()->removeCamera(mCamera);
	}
	if (mRootNode) {
		mRootNode->removeAndDestroyAllChildren();
		EmberOgre::getSingleton().getSceneManager()->destroySceneNode(mRootNode->getName());
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


void EntityCEGUITexture::createCamera(const std::string& imageSetName)
{	
	mCamera = EmberOgre::getSingleton().getSceneManager()->createCamera(imageSetName + "_EntityCEGUITextureCamera");

	//track the node containing the model
	mCamera->setAutoTracking(true, mEntityNode);
	mCamera->setNearClipDistance(0.01);
	mCamera->setFarClipDistance(500);
	mCameraNode->attachObject(mCamera);
}

void EntityCEGUITexture::createImage(const std::string& imageSetName)
{


	if (mWidth == 0 || mHeight == 0) {
		throw Ember::Exception("Height and width of the image can't be 0.");
	}
	//first, create a RenderTexture to which the Ogre renderer should render the image
	mRenderTexture = EmberOgre::getSingleton().getOgreRoot()->getRenderSystem()->createRenderTexture(imageSetName + "_EntityCEGUITextureRenderTexture", mWidth, mHeight );
	mRenderTexture->removeAllViewports();
	mRenderTexture->setActive(false);
	
	//make sure the camera renders into this new texture
	Ogre::Viewport *v = mRenderTexture->addViewport(mCamera );
	v->setBackgroundColour(Ogre::ColourValue(0,0,0,0));
	//don't show the CEGUI
	v->setOverlaysEnabled(false);
	//the cegui renderer wants a TexturePtr (not a RenderTexturePtr), so we just ask the texturemanager for texture we just created (rttex)
	Ogre::TexturePtr texPtr = Ogre::TextureManager::getSingleton().getByName(mRenderTexture->getName());
	
	//create a CEGUI texture from our Ogre texture
	CEGUI::Texture* ceguiTexture = GUIManager::getSingleton().getGuiRenderer()->createTexture(texPtr);
	
	//we need a imageset in order to create GUI elements from the ceguiTexture
	mImageSet = CEGUI::ImagesetManager::getSingleton().createImageset(imageSetName + "_EntityCEGUITextureImageset", ceguiTexture);
	
	//we only want one element: the whole texture
	mImageSet->defineImage("full", CEGUI::Rect(0,0,mWidth,mHeight), CEGUI::Point(0,0));
	
	//assign our image element to the StaticImage widget
	mImage = &mImageSet->getImage("full");
	

}

void EntityCEGUITexture::showFull()
{
	mCamera->setAutoTracking(false);
	mEntityNode->_update(true, true);
	Ogre::AxisAlignedBox bbox = mEntityNode->_getWorldAABB();
	Ogre::Vector3 center = bbox.getCenter();
	
	Ogre::Vector3 position = mCameraNode->getPosition();
	
	Ogre::Vector3 localPosition = center - mRootNode->getPosition();
	
	position.y = localPosition.y;
	position.x = localPosition.x;
	position.z = -(bbox.getMaximum().z - center.z) * 2;
	mCameraNode->setPosition(position);
	mCamera->lookAt(center);
	
	
	
	
/*	Ogre::Vector3 screenCoords;
	bool result = false;
	//check what the new position is in screen coords
	result = EmberOgre::getSingletonPtr()->getMainCamera()->worldToScreen(entityWorldCoords, screenCoords);*/
	
}




}
