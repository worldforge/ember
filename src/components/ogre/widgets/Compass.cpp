//
// C++ Implementation: Compass
//
// Description:
//
//
// Author: Erik Ogenvik <erik@ogenvik.org>, (C) 2008
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

#include "Compass.h"

#include "AssetsManager.h"

#include "../EmberOgre.h"
#include "../Avatar.h"
#include "../OgreInfo.h"
#include "../terrain/Map.h"
#include "../terrain/ITerrainAdapter.h"
#include "../terrain/ITerrainObserver.h"

#include <Ogre.h>
#include <OgreRectangle2D.h>

#include <CEGUI/Image.h>
#include <CEGUI/Window.h>

using namespace Ember::OgreView::Terrain;

namespace Ember {
namespace OgreView {

namespace Gui {

DelayedCompassRenderer::DelayedCompassRenderer(Compass& compass) :
		mCompass(compass), mRenderNextFrame(false) {
	Ogre::Root::getSingleton().addFrameListener(this);
}

DelayedCompassRenderer::~DelayedCompassRenderer() {
	Ogre::Root::getSingleton().removeFrameListener(this);
}

bool DelayedCompassRenderer::frameStarted(const Ogre::FrameEvent& event) {
	if (mRenderNextFrame) {
		try {
			mRenderNextFrame = false;
			mCompass.getMap().render();
			mCompass.refresh();
		} catch (const std::exception& ex) {
			S_LOG_WARNING("Error when updating compass." << ex);
		}
	}
	return true;
}

void DelayedCompassRenderer::queueRendering() {
	mRenderNextFrame = true;
}

Compass::Compass(ICompassImpl* compassImpl, Ogre::SceneManager& sceneManager, Terrain::ITerrainAdapter& terrainAdapter) :
		mMap(new Map(sceneManager)),
		mCompassImpl(compassImpl),
		mTerrainObserver(terrainAdapter.createObserver()),
		mDelayedRenderer(*this) {
	mMap->initialize();
	if (compassImpl) {
		compassImpl->setCompass(this);
	}
	updateTerrainObserver();
	mMap->getView().EventBoundsChanged.connect(sigc::mem_fun(*this, &Compass::mapView_BoundsChanged));
	mTerrainObserver->EventAreaShown.connect(sigc::mem_fun(*this, &Compass::terrainObserver_AreaShown));
}

Compass::~Compass() = default;

Terrain::Map& Compass::getMap() {
	return *mMap;
}

void Compass::reposition(float x, float y) {
	if (mCompassImpl) {
		mCompassImpl->reposition(x, y);
	}
}

void Compass::rotate(const Ogre::Degree& degree) {
	if (mCompassImpl) {
		mCompassImpl->rotate(degree);
	}
}

//Note: duplicate method to make it easier for scripts interacting with the code
void Compass::rotate(const Ogre::Radian& radian) {
	rotate(Ogre::Degree(radian));
}

void Compass::refresh() {
	if (mCompassImpl) {
		mCompassImpl->refresh();
	}
}

void Compass::terrainObserver_AreaShown() {
	queueRefresh();
}

void Compass::queueRefresh() {
	mDelayedRenderer.queueRendering();
}

void Compass::mapView_BoundsChanged() {
	updateTerrainObserver();
}

void Compass::updateTerrainObserver() {
	mTerrainObserver->observeArea(mMap->getView().getFullBounds());
}

ICompassImpl::ICompassImpl() :
		mMap(nullptr),
		mCompass(nullptr) {
}

void ICompassImpl::setCompass(Compass* compass) {
	mCompass = compass;
	mMap = &compass->getMap();
	_setCompass(compass);
}

RenderedCompassImpl::RenderedCompassImpl(CEGUI::Window* pointerElement, std::string compassMaterialName) :
		mRenderTexture(nullptr),
		mCamera(nullptr),
		mSceneManager(nullptr),
		mViewport(nullptr),
		mCompassMaterialMapTUS(nullptr),
		mX(0),
		mY(0),
		mMapRectangle(nullptr),
		mPointerElement(pointerElement),
		mMaterialName(std::move(compassMaterialName)) {
	//Make sure we create the most simple scene manager.
	mSceneManager = Ogre::Root::getSingleton().createSceneManager(Ogre::DefaultSceneManagerFactory::FACTORY_TYPE_NAME, "RenderedCompassImpl_sceneManager");
	mSceneManager->setFog(Ogre::FOG_EXP2, Ogre::ColourValue(0, 0, 0, 0), 0.0f, 0.0f, 0.0f);
}

RenderedCompassImpl::~RenderedCompassImpl() {
	//We should probably not do this ourselves, since it will corrupt the material.
	if (mTexture) {
		Ogre::TextureManager::getSingleton().remove(mTexture->getHandle());
	}

	if (mCamera) {
		mSceneManager->destroyCamera(mCamera);
	}

	Ogre::Root::getSingleton().destroySceneManager(mSceneManager);
	if (mCompassMaterial) {
		Ogre::MaterialManager::getSingleton().remove(mCompassMaterial->getHandle());
	}

	OGRE_DELETE mMapRectangle;

}

void RenderedCompassImpl::reposition(float x, float y) {
	mMap->getView().reposition(Ogre::Vector2(x, y));

	if (mCompassMaterialMapTUS) {
		const Ogre::Vector2& relPosition(mMap->getView().getRelativeViewPosition());
		mCompassMaterialMapTUS->setTextureScroll(-0.5f + relPosition.x, -0.5f + relPosition.y);

		try {
			mRenderTexture->update();
		} catch (const std::exception& ex) {
			S_LOG_FAILURE("Error when updating render for compass." << ex);
		}
	}
	mX = x;
	mY = y;
}

void RenderedCompassImpl::rotate(const Ogre::Degree& degree) {
	auto rotation = CEGUI::Quaternion::axisAngleDegrees(CEGUI::Vector3f(0, 0, 1), degree.valueDegrees() + 45.0f);

	mPointerElement->setRotation(rotation);
}

void RenderedCompassImpl::refresh() {
	reposition(mX, mY);
}

void RenderedCompassImpl::_setCompass(Compass* compass) {
	Ogre::MaterialPtr originalMaterial = static_cast<Ogre::MaterialPtr>(Ogre::MaterialManager::getSingleton().getByName(mMaterialName));
	if (originalMaterial) {
		originalMaterial->load();
		mCompassMaterial = originalMaterial->clone(OgreInfo::createUniqueResourceName(originalMaterial->getName()));
		if (Ogre::Technique* tech = mCompassMaterial->getBestTechnique()) {
			Ogre::Pass* pass = nullptr;
			if (tech->getNumPasses() && (pass = tech->getPass(0))) {
				mCompassMaterialMapTUS = pass->getTextureUnitState("Background");
				if (mCompassMaterialMapTUS) {
					//Make sure that the compass material is using the map texture for the base rendering
					mCompassMaterialMapTUS->setTexture(mMap->getTexture());

					mTexture = Ogre::TextureManager::getSingleton().createManual("RenderedCompass", originalMaterial->getGroup(), Ogre::TEX_TYPE_2D, 128, 128, 1, Ogre::PF_A8R8G8B8, Ogre::TU_RENDERTARGET);
					mRenderTexture = mTexture->getBuffer()->getRenderTarget();
					mRenderTexture->removeAllViewports();
					mRenderTexture->setAutoUpdated(false);
					mRenderTexture->setActive(true);

					mCamera = mSceneManager->createCamera("RenderedCompassCamera");
					mViewport = mRenderTexture->addViewport(mCamera);
					mViewport->setOverlaysEnabled(false);
					mViewport->setShadowsEnabled(false);
					mViewport->setSkiesEnabled(false);
					mViewport->setClearEveryFrame(true);
					mViewport->setBackgroundColour(Ogre::ColourValue::ZERO);

					mMapRectangle = OGRE_NEW Ogre::Rectangle2D(true);
					auto mapMaterialPtr = Ogre::MaterialManager::getSingleton().getByName(mCompassMaterial->getName(), originalMaterial->getGroup());
					if (mapMaterialPtr) {
						mMapRectangle->setMaterial(mapMaterialPtr);
					}

					//We need to maximise the rendered texture to cover the whole screen
					Ogre::RenderSystem* rs = Ogre::Root::getSingleton().getRenderSystem();
					Ogre::Real hOffset = rs->getHorizontalTexelOffset() / (0.5f * mViewport->getActualWidth());
					Ogre::Real vOffset = rs->getVerticalTexelOffset() / (0.5f * mViewport->getActualHeight());
					mMapRectangle->setCorners(-1 + hOffset, 1 - vOffset, 1 + hOffset, -1 - vOffset);

					//Since a Rectangle2D instance is a moveable object it won't be rendered unless it's in the frustrum. If we set the axis aligned box to be "infinite" it will always be rendered.
					Ogre::AxisAlignedBox aabInf;
					aabInf.setInfinite();
					mMapRectangle->setBoundingBox(aabInf);

					//We can't attach something to the root node, so we'll attach it to a newly created node. We won't keep a reference to this node since it will be destroyed along with the scene manager when we ourselves are destroyed.
					mSceneManager->getRootSceneNode()->createChildSceneNode()->attachObject(mMapRectangle);

					//Return early since everything is good.
					return;
				}
			}
		}
	}
	S_LOG_WARNING("Could not load material '" << mMaterialName << "' for the compass.");
}

Ogre::TexturePtr RenderedCompassImpl::getTexture() const {
	return mTexture;
}


CompassAnchor::CompassAnchor(Compass& compass, const Ogre::Vector3& position, const Ogre::Quaternion& orientation) :
		mCompass(compass), mPreviousX(0), mPreviousZ(0), mPosition(position), mOrientation(orientation) {
	// Register this as a frame listener
	Ogre::Root::getSingleton().addFrameListener(this);
}

CompassAnchor::~CompassAnchor() {
	Ogre::Root::getSingleton().removeFrameListener(this);
}

bool CompassAnchor::frameStarted(const Ogre::FrameEvent& event) {
	mCompass.rotate(-mOrientation.getYaw());
	if (mPosition.x != mPreviousX || mPosition.z != mPreviousZ) {
		mCompass.reposition(mPosition.x, mPosition.z);
		mPreviousX = mPosition.x;
		mPreviousZ = mPosition.z;
	}
	return true;
}

CompassCameraAnchor::CompassCameraAnchor(Compass& compass, Ogre::Camera* camera) :
		mAnchor(compass, camera->getDerivedPosition(), camera->getDerivedOrientation()), mCamera(camera) {
}

CompassSceneNodeAnchor::CompassSceneNodeAnchor(Compass& compass, Ogre::SceneNode* sceneNode) :
		mAnchor(compass, sceneNode->_getDerivedPosition(), sceneNode->_getDerivedOrientation()), mSceneNode(sceneNode) {
}

CompassThirdPersonCameraAnchor::CompassThirdPersonCameraAnchor(Compass& compass, Ogre::Camera* camera, Ogre::SceneNode* sceneNode) :
		mAnchor(compass, sceneNode->_getDerivedPosition(), camera->getDerivedOrientation()), mCamera(camera), mSceneNode(sceneNode) {
}

}

}
}
