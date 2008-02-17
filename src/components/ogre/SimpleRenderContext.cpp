//
// C++ Implementation: SimpleRenderContext
//
// Description: 
//
//
// Author: Erik Hjortsberg <erik.hjortsberg@iteam.se>, (C) 2007
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
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "SimpleRenderContext.h"

#include "EmberOgre.h"
#include "GUIManager.h"
#include "model/Model.h"

#include "framework/Exception.h"
#include <OgreBitwise.h>

namespace EmberOgre {

SimpleRenderContextResourceLoader::SimpleRenderContextResourceLoader(SimpleRenderContext& renderContext)
: mRenderContext(renderContext)
{
}

void SimpleRenderContextResourceLoader::loadResource (Ogre::Resource *resource)
{
	if (resource->getLoadingState() == Ogre::Resource::LOADSTATE_UNLOADED) {
		mRenderContext.getRenderTexture()->update();
	}
}


SimpleRenderContext::SimpleRenderContext(const std::string& prefix, int width, int height)
: mMainLight(0), mSceneManager(0), mWidth(width), mHeight(height), mRenderTexture(0), mCameraNode(0), mCameraPitchNode(0), mEntityNode(0), mRootNode(0), mCamera(0), mViewPort(0), mResourceLoader(*this)
{

	S_LOG_VERBOSE("Creating new SimpleRenderContext for prefix " << prefix  << " with w:" << mWidth << " h:" << mHeight);
	mSceneManager = Ogre::Root::getSingleton().createSceneManager(Ogre::ST_GENERIC, prefix + "_sceneManager");
	mSceneManager->setFog(Ogre::FOG_NONE, Ogre::ColourValue::ZERO, 0, 0, 0);

	mRootNode = mSceneManager->getRootSceneNode();
	
	
	mEntityNode = mRootNode->createChildSceneNode();

	///make the cameranode a child of the main entity node
	mCameraNode = mRootNode->createChildSceneNode();
	
	mCameraPitchNode = mCameraNode->createChildSceneNode();
	
	createCamera(prefix);
	createImage(prefix);
	//setVisible(false);
	Ogre::ColourValue colour(0.5, 0.5, 0.5);
	mMainLight = mSceneManager->createLight("MainLight");
  	mMainLight->setType(Ogre::Light::LT_DIRECTIONAL);
	mMainLight->setDirection(Ogre::Vector3(-1,0,0));
	mMainLight->setPowerScale (10);	// REALLY bright.
	mMainLight->setDiffuseColour (colour);
	mMainLight->setSpecularColour (colour);
	mMainLight->setVisible(true);

	mSceneManager->setAmbientLight(colour);
// 	mCameraPitchNode->attachObject(mMainLight);
	
	resetCameraOrientation();
}


SimpleRenderContext::~SimpleRenderContext()
{
	if (mCamera) {
		mSceneManager->destroyCamera(mCamera);
	}
	///we need to do this before the scene manager is destroyed since the destructor for Model relies on the scenemanager existing (and thus can be called in the scene manager's destructor)
	if (mRootNode) {
		mRootNode->removeAndDestroyAllChildren();
		///the root scene node cannot be removed (evar!!)
// 		mSceneManager->destroySceneNode(mSceneManager->getRootSceneNode()->getName());
	}
	///we must make sure that all models are destroyed before the entities are destroyed, else we'll get segfaults in the Model destructor as it tries to access already deleted entities
	mSceneManager->destroyAllMovableObjectsByType(Model::Model::sMovableType);
	Ogre::Root::getSingleton().destroySceneManager(mSceneManager);
}

Ogre::SceneNode* SimpleRenderContext::getSceneNode() const
{
	return mEntityNode;
}

Ogre::Camera* SimpleRenderContext::getCamera() const
{
	return mCamera;
}

void SimpleRenderContext::setActive(bool active)
{
	if (mRenderTexture) {
		mRenderTexture->setActive(active);
	}
}

void SimpleRenderContext::repositionCamera()
{
	mEntityNode->_update(true, true);
	Ogre::AxisAlignedBox bbox = mEntityNode->_getWorldAABB();
	if (!bbox.isInfinite() && !bbox.isNull()) {
		Ogre::Vector3 center = bbox.getCenter();
		Ogre::Vector3 localCenter =  center - mRootNode->getPosition();
		mCameraNode->setPosition(localCenter);
	}
}

void SimpleRenderContext::createCamera(const std::string& prefix)
{	
	mCamera = mSceneManager->createCamera(prefix + "_SimpleRenderContextCamera");

	mCameraPitchNode->attachObject(mCamera);
}

void SimpleRenderContext::pitch(Ogre::Degree degrees)
{
	mCameraPitchNode->pitch(degrees);
}
void SimpleRenderContext::yaw(Ogre::Degree degrees)
{
	mCameraNode->yaw(degrees);
}
void SimpleRenderContext::roll(Ogre::Degree degrees)
{
	mCameraNode->roll(degrees);
}
	
	
float SimpleRenderContext::getCameraDistance() const
{
	if (mDefaultCameraDistance) { 
		return mCamera->getPosition().z / mDefaultCameraDistance;
	}
	return mCamera->getPosition().z;
}

void SimpleRenderContext::setCameraDistance(Ogre::Real distance)
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

// void SimpleRenderContext::setCameraAbsoluteDistance(Ogre::Real distance)
// {
// 	Ogre::Vector3 pos(0,0,distance);
// 	mCamera->setPosition(pos);
// }

float SimpleRenderContext::getAbsoluteCameraDistance() const
{
	return mCamera->getPosition().z;
}

Ogre::Quaternion SimpleRenderContext::getEntityRotation()
{
	return mCamera->getDerivedOrientation().Inverse();
}

void SimpleRenderContext::resetCameraOrientation()
{
	mCameraPitchNode->setOrientation(Ogre::Quaternion::IDENTITY);
	mCameraNode->setOrientation(Ogre::Quaternion::IDENTITY);

}

Ogre::SceneNode* SimpleRenderContext::getCameraRootNode() const
{
	return mCameraNode;
}

Ogre::TexturePtr SimpleRenderContext::getTexture()
{
	return mTexture;
}


void SimpleRenderContext::createImage(const std::string& prefix)
{

	if (mWidth == 0 || mHeight == 0) {
		throw Ember::Exception("Height and width of the image can't be 0.");
	}
	
	Ogre::Real aspectRatio = static_cast<float>(mWidth) / static_cast<float>(mHeight);
	
	///the width and height needs to be multipes of 2
	mWidth = Ogre::Bitwise::firstPO2From(mWidth); 
	mHeight = Ogre::Bitwise::firstPO2From(mHeight); 
	
	
	///first, create a RenderTexture to which the Ogre renderer should render the image
	S_LOG_VERBOSE("Creating new rendertexture " << (prefix + "_SimpleRenderContextRenderTexture") << " with w:" << mWidth << " h:" << mHeight);
	mTexture = Ogre::TextureManager::getSingleton().createManual(prefix + "_SimpleRenderContextRenderTexture", "Gui", Ogre::TEX_TYPE_2D, mWidth, mHeight, 0, Ogre::PF_A8R8G8B8,Ogre::TU_RENDERTARGET, &mResourceLoader);
	if (mTexture.isNull()) {
		S_LOG_WARNING("Could not create a texture.");
		return;
	}
	
	
	mRenderTexture = mTexture->getBuffer()->getRenderTarget();
	mRenderTexture->setAutoUpdated(false);
	
	S_LOG_VERBOSE("Removing viewports.");
	mRenderTexture->removeAllViewports();
	///initially deactivate it until setActive(true) is called
	mRenderTexture->setActive(false);
	S_LOG_VERBOSE("Setting aspect ratio of camera to " << aspectRatio);
	mCamera->setAspectRatio(aspectRatio);
	///make sure the camera renders into this new texture
	S_LOG_VERBOSE("Adding camera.");
	mViewPort = mRenderTexture->addViewport(mCamera);
	///this should preferrably be a transparent background, so that CEGUI could itself decide what to show behind it, but alas I couldn't get it to work, thus black
	mViewPort->setBackgroundColour(Ogre::ColourValue::Black);
//	mViewPort->setBackgroundColour(Ogre::ColourValue::ZERO);
	///don't show the CEGUI
	mViewPort->setOverlaysEnabled(false);
	///the cegui renderer wants a TexturePtr (not a RenderTexturePtr), so we just ask the texturemanager for texture we just created (rttex)
	

}

void SimpleRenderContext::setBackgroundColour(const Ogre::ColourValue& colour)
{
	mViewPort->setBackgroundColour(colour);
}

void SimpleRenderContext::setBackgroundColour(float red, float green, float blue, float alpha)
{
	mViewPort->setBackgroundColour(Ogre::ColourValue(red, green, blue, alpha));
}



void SimpleRenderContext::showFull(const Ogre::MovableObject* object)
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

Ogre::RenderTexture* SimpleRenderContext::getRenderTexture()
{
	return mRenderTexture;
}

Ogre::Light* SimpleRenderContext::getLight()
{
	return mMainLight;
}


}
