//
// C++ Implementation: SimpleRenderContext
//
// Description:
//
//
// Author: Erik Ogenvik <erik@ogenvik.org>, (C) 2007
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
// along with this program; if not, write to the Free Software Foundation,
// Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.//
//
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "SimpleRenderContext.h"

#include "GUIManager.h"
#include "model/Model.h"

#include <OgreBitwise.h>
#include <OgreRoot.h>
#include <OgreHardwarePixelBuffer.h>
#include <OgreRenderTexture.h>
#include <OgreCamera.h>
#include <OgreSceneNode.h>
#include <OgreTextureManager.h>
#include <OgreViewport.h>

namespace Ember {
namespace OgreView {

SimpleRenderContextResourceLoader::SimpleRenderContextResourceLoader(SimpleRenderContext& renderContext) :
        mRenderContext(renderContext) {
}

void SimpleRenderContextResourceLoader::loadResource(Ogre::Resource* resource) {
    if (resource->getLoadingState() == Ogre::Resource::LOADSTATE_UNLOADED && mRenderContext.getRenderTexture()) {
        try {
            mRenderContext.getRenderTexture()->update();
        } catch (const std::exception& ex) {
            S_LOG_FAILURE("Error when updating render for SimpleRenderContext, after the resource has been reloaded." << ex);
        }
    }
}

SimpleRenderContext::SimpleRenderContext(const std::string& prefix, int width, int height) :
        mMainLight(nullptr),
        mSceneManager(nullptr),
        mDefaultCameraDistance(0),
        mWidth(width),
        mHeight(height),
        mRenderTexture(nullptr),
        mCameraNode(nullptr),
        mCameraPitchNode(nullptr),
        mCameraDistanceNode(nullptr),
        mEntityNode(nullptr),
        mRootNode(nullptr),
        mCamera(nullptr),
        mViewPort(nullptr),
        mResourceLoader(*this),
        mBackgroundColour(Ogre::ColourValue::Black),
        mCameraPositionMode(CPM_OBJECTCENTER), mTextureOwned(true) {

    setupScene(prefix);
    createImage(prefix);
}

SimpleRenderContext::SimpleRenderContext(const std::string& prefix, Ogre::TexturePtr texture) :
        mMainLight(nullptr),
        mSceneManager(nullptr),
        mDefaultCameraDistance(0),
        mWidth(texture->getWidth()),
        mHeight(texture->getHeight()),
        mRenderTexture(nullptr),
        mCameraNode(nullptr),
        mCameraPitchNode(nullptr),
        mCameraDistanceNode(nullptr),
        mEntityNode(nullptr),
        mRootNode(nullptr),
        mCamera(nullptr),
        mViewPort(nullptr),
        mResourceLoader(*this),
        mBackgroundColour(Ogre::ColourValue::Black),
        mCameraPositionMode(CPM_OBJECTCENTER), mTextureOwned(false) {

    setupScene(prefix);
    setTexture(texture);

    Ogre::Real aspectRatio = static_cast<float>(texture->getWidth()) / static_cast<float>(texture->getHeight());

    S_LOG_VERBOSE("Setting aspect ratio of camera to " << aspectRatio);
    mCamera->setAspectRatio(aspectRatio);

}

SimpleRenderContext::~SimpleRenderContext() {
    if (mTextureOwned) {
        Ogre::TextureManager::getSingleton().remove(mTexture->getHandle());
    }
    Ogre::Root::getSingleton().destroySceneManager(mSceneManager);
}

void SimpleRenderContext::setupScene(const std::string& prefix) {
    S_LOG_VERBOSE("Creating new SimpleRenderContext for prefix " << prefix << " with w:" << mWidth << " h:" << mHeight);
    //Make sure we get the default manager; the most simple one.
    mSceneManager = Ogre::Root::getSingleton().createSceneManager(Ogre::DefaultSceneManagerFactory::FACTORY_TYPE_NAME, prefix + "_sceneManager");
    //One might wonder why we're not setting the fog to FOG_NONE. The reason is that it seems that due to a bug in either
    // Ogre or OpenGL when doing that, none of the other fog values would be set.
    // Since we use shaders and in the shaders look for the alpha value of the fog colour to determine whether
    // fog is enabled or not, we need to make sure that the fog colour indeed is set.
    mSceneManager->setFog(Ogre::FOG_EXP2, Ogre::ColourValue(0, 0, 0, 0), 0.0f, 0.0f, 0.0f);
// 	mSceneManager->setFog(Ogre::FOG_NONE, Ogre::ColourValue(1,1,1,1), 0.0f, 10000000.0f, 100000001.0f);

    mRootNode = mSceneManager->getRootSceneNode();

    mEntityNode = mRootNode->createChildSceneNode();

    //make the camera node a child of the main entity node
    mCameraNode = mRootNode->createChildSceneNode();

    mCameraPitchNode = mCameraNode->createChildSceneNode();
    mCameraDistanceNode = mCameraPitchNode->createChildSceneNode();

    createCamera(prefix);
    Ogre::ColourValue colour(0.7f, 0.7f, 0.7f);
    mMainLight = mSceneManager->createLight("MainLight");
    mRootNode->createChildSceneNode()->attachObject(mMainLight);
    mMainLight->setType(Ogre::Light::LT_DIRECTIONAL);
    mMainLight->getParentSceneNode()->setDirection(Ogre::Vector3(-1.0f, 0, 0), Ogre::Node::TS_WORLD);
    mMainLight->setDiffuseColour(colour);
    mMainLight->setSpecularColour(colour);
    mMainLight->setVisible(true);

    mSceneManager->setAmbientLight(Ogre::ColourValue(0.5f, 0.5f, 0.5f));

    resetCameraOrientation();
}

Ogre::SceneNode* SimpleRenderContext::getSceneNode() const {
    return mEntityNode;
}

Ogre::Camera* SimpleRenderContext::getCamera() const {
    return mCamera;
}

void SimpleRenderContext::setActive(bool active) {
    if (mRenderTexture) {
        mRenderTexture->setActive(active);
    }
}

void SimpleRenderContext::repositionCamera() {
    mEntityNode->_update(true, true);
    Ogre::AxisAlignedBox bbox = mEntityNode->_getWorldAABB();
    if (mCameraPositionMode == CPM_OBJECTCENTER) {
        if (!bbox.isInfinite() && !bbox.isNull()) {
            Ogre::Vector3 center = bbox.getCenter();
            Ogre::Vector3 localCenter = center - mRootNode->getPosition();
            mCameraNode->setPosition(localCenter);
        }
    } else if (mCameraPositionMode == CPM_WORLDCENTER) {
        mCameraNode->setPosition(Ogre::Vector3::ZERO);
    } else {
    }
}

void SimpleRenderContext::createCamera(const std::string& prefix) {
    mCamera = mSceneManager->createCamera(prefix + "_SimpleRenderContextCamera");

    mCameraDistanceNode->attachObject(mCamera);
}

void SimpleRenderContext::pitchCamera(Ogre::Degree degrees) {
    mCameraPitchNode->pitch(degrees);
}

void SimpleRenderContext::yawCamera(Ogre::Degree degrees) {
    mCameraNode->yaw(degrees);
}

void SimpleRenderContext::rollCamera(Ogre::Degree degrees) {
    mCameraNode->roll(degrees);
}

float SimpleRenderContext::getCameraDistance() const {
    if (mDefaultCameraDistance) {
        return mCameraDistanceNode->getPosition().z / mDefaultCameraDistance;
    }
    return mCameraDistanceNode->getPosition().z;
}

void SimpleRenderContext::setCameraDistance(Ogre::Real distance) {
    if (distance != 0) {
        Ogre::Real cameraDistance = Ogre::Math::Abs(distance) / 100;
        if (cameraDistance > 0) {
            mCamera->setNearClipDistance(Ogre::Math::Abs(distance) / 100);
        } else {
            mCamera->setNearClipDistance(0.01f);
        }
        mCamera->setFarClipDistance((Ogre::Math::Abs(distance) + mDefaultCameraDistance));
    }
    Ogre::Vector3 pos(0, 0, distance);
    mCameraDistanceNode->setPosition(pos);
}

// void SimpleRenderContext::setCameraAbsoluteDistance(Ogre::Real distance)
// {
// 	Ogre::Vector3 pos(0,0,distance);
// 	mCamera->setPosition(pos);
// }

float SimpleRenderContext::getAbsoluteCameraDistance() const {
    return mCameraNode->_getDerivedPosition().z;
}

Ogre::Quaternion SimpleRenderContext::getCameraOrientation() const {
    return mCamera->getDerivedOrientation();
}


void SimpleRenderContext::resetCameraOrientation() {
    mCameraPitchNode->setOrientation(Ogre::Quaternion::IDENTITY);
    mCameraNode->setOrientation(Ogre::Quaternion::IDENTITY);
}

Ogre::SceneNode* SimpleRenderContext::getCameraRootNode() const {
    return mCameraNode;
}

Ogre::TexturePtr SimpleRenderContext::getTexture() {
    return mTexture;
}

Ogre::Viewport* SimpleRenderContext::getViewport() const {
    return mViewPort;
}

void SimpleRenderContext::createImage(const std::string& prefix) {

    if (mWidth == 0 || mHeight == 0) {
        throw Exception("Height and width of the image can't be 0.");
    }

    Ogre::Real aspectRatio = static_cast<float>(mWidth) / static_cast<float>(mHeight);

    S_LOG_VERBOSE("Setting aspect ratio of camera to " << aspectRatio);
    mCamera->setAspectRatio(aspectRatio);

    //the width and height needs to be multipes of 2
    mWidth = Ogre::Bitwise::firstPO2From(mWidth);
    mHeight = Ogre::Bitwise::firstPO2From(mHeight);

    //first, create a RenderTexture to which the Ogre renderer should render the image
    S_LOG_VERBOSE("Creating new rendertexture " << (prefix + "_SimpleRenderContextRenderTexture") << " with w:" << mWidth << " h:" << mHeight);
    auto& textureMgr = Ogre::TextureManager::getSingleton();
    auto texture = textureMgr.createManual(prefix + "_SimpleRenderContextRenderTexture",
                                           Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
                                           Ogre::TEX_TYPE_2D,
                                           mWidth,
                                           mHeight,
                                           0,
                                           Ogre::PF_A8R8G8B8,
                                           Ogre::TU_RENDERTARGET,
                                           &mResourceLoader);
    if (!texture) {
        S_LOG_WARNING("Could not create a texture.");
        return;
    }

    setTexture(texture);
}

void SimpleRenderContext::setTexture(Ogre::TexturePtr texture) {
    if (texture != mTexture) {
        if (mRenderTexture) {
            mRenderTexture->removeAllViewports();
        }
        mTexture = texture;
        mRenderTexture = texture->getBuffer()->getRenderTarget();
        mRenderTexture->removeAllViewports();

        mRenderTexture->setAutoUpdated(false);
        //initially deactivate it until setActive(true) is called
        mRenderTexture->setActive(false);

        S_LOG_VERBOSE("Adding camera.");
        mViewPort = mRenderTexture->addViewport(mCamera);
        mViewPort->setShadowsEnabled(true);
        //make sure the camera renders into this new texture
        //this should preferrably be a transparent background, so that CEGUI could itself decide what to show behind it, but alas I couldn't get it to work, thus black
        mViewPort->setBackgroundColour(mBackgroundColour);
        //	mViewPort->setBackgroundColour(Ogre::ColourValue::ZERO);
        //don't show the CEGUI
        mViewPort->setOverlaysEnabled(false);
        //the cegui renderer wants a TexturePtr (not a RenderTexturePtr), so we just ask the texturemanager for texture we just created (rttex)

        //Use Medium scheme since High requires PSSM shadows to be setup.
        mViewPort->setMaterialScheme("Medium");
    }
}

SimpleRenderContext::CameraPositioningMode SimpleRenderContext::getCameraPositionMode() const {
    return mCameraPositionMode;
}

void SimpleRenderContext::setCameraPositionMode(SimpleRenderContext::CameraPositioningMode mode) {
    mCameraPositionMode = mode;
    repositionCamera();
}

void SimpleRenderContext::setBackgroundColour(const Ogre::ColourValue& colour) {
    mBackgroundColour = colour;
    if (mViewPort) {
        mViewPort->setBackgroundColour(colour);
    }
}

void SimpleRenderContext::setBackgroundColour(float red, float green, float blue, float alpha) {
    mBackgroundColour = Ogre::ColourValue(red, green, blue, alpha);
    if (mViewPort) {
        mViewPort->setBackgroundColour(mBackgroundColour);
    }
}

void SimpleRenderContext::showFull(float boundingRadius) {
    mEntityNode->_update(true, true);
    Ogre::Real distance = boundingRadius / Ogre::Math::Tan(mCamera->getFOVy() / 2);
    //we can't have a distance of 0
    if (distance == 0) {
        distance = 1;
    }
    Ogre::Real distanceNudge = distance / 100;
    distance += distanceNudge;

    mDefaultCameraDistance = distance;

    setCameraDistance(distance);
}

Ogre::RenderTexture* SimpleRenderContext::getRenderTexture() {
    return mRenderTexture;
}

Ogre::Light* SimpleRenderContext::getLight() {
    return mMainLight;
}

}
}
