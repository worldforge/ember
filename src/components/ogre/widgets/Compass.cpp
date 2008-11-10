//
// C++ Implementation: Compass
//
// Description: 
//
//
// Author: Erik Hjortsberg <erik.hjortsberg@iteam.se>, (C) 2008
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

#include "Compass.h"

#include <Ogre.h>

#include <CEGUIImageset.h>
#include <CEGUIImage.h>
#include <CEGUIRect.h>

#include "../terrain/Map.h"

#include "services/logging/LoggingService.h"
#include "../EmberOgre.h"
#include "../Avatar.h"
#include "../AvatarCamera.h"

using namespace EmberOgre::Terrain;

namespace EmberOgre {

namespace Gui {

Compass::Compass(ICompassImpl* compassImpl)
: mMap(new Map()), mCompassImpl(compassImpl)
{
	mMap->initialize();
	if (compassImpl) {
		compassImpl->setCompass(this);
	}
}


Compass::~Compass()
{
}

Terrain::Map& Compass::getMap()
{
	return *mMap;
}

void Compass::reposition(float x, float y)
{
	if (mCompassImpl) {
		mCompassImpl->reposition(x, y);
	}
}

void Compass::rotate(const Ogre::Degree& degree)
{
	if (mCompassImpl) {
		mCompassImpl->rotate(degree);
	}
}

///Note: duplicate method to make it easier for scripts interacting with the code
void Compass::rotate(const Ogre::Radian& radian)
{
	rotate(Ogre::Degree(radian));
}

ICompassImpl::ICompassImpl()
: mMap(0)
, mCompass(0)
{
}

void ICompassImpl::setCompass(Compass* compass)
{
	mCompass = compass;
	mMap = &compass->getMap();
	_setCompass(compass);
}


CEGUICompassImpl::CEGUICompassImpl()
: mViewImage(0)
{
}

CEGUICompassImpl::~CEGUICompassImpl()
{
}

const CEGUI::Image* CEGUICompassImpl::getViewImage() 
{
	return mViewImage;
}

void CEGUICompassImpl::reposition(float x, float y)
{
// 	S_LOG_VERBOSE("pos x: " << x << " y: " << y);
	mMap->getView().reposition(Ogre::Vector2(x, y));
	const Ogre::TRect<float>& viewBounds(mMap->getView().getRelativeViewBounds());
	CEGUI::Rect& rect = const_cast<CEGUI::Rect&>(mViewImage->getSourceTextureArea());
	int textureWidth = mTexturePair.getOgreTexture()->getWidth();
	rect.setSize(CEGUI::Size(textureWidth * 0.5, textureWidth * 0.5));
// 	const Ogre::Vector2& viewPos(mMap->getView().getRelativeViewPosition());
	rect.setPosition(CEGUI::Point(textureWidth * viewBounds.left, textureWidth * viewBounds.top));
}

void CEGUICompassImpl::rotate(const Ogre::Degree& degree)
{
	///we can't rotate CEGUI windows so we won't do anything
}

void CEGUICompassImpl::_setCompass(Compass* compass)
{
	AssetsManager assetsManager;
	mTexturePair = assetsManager.createTextureImage(mMap->getTexture(), "CompassMap");
	int halfOffset = static_cast<int>(mMap->getTexture()->getWidth() * 0.25f);
	mTexturePair.getTextureImageset()->defineImage("view",  CEGUI::Rect(halfOffset, halfOffset, mMap->getTexture()->getWidth() - halfOffset, mMap->getTexture()->getWidth() - halfOffset), CEGUI::Point(0,0));
	mViewImage = &mTexturePair.getTextureImageset()->getImage("view");
	
}





OverlayCompassImpl::OverlayCompassImpl()
: mCompassOverlay(0)
{
	Ogre::OverlayManager& omgr = Ogre::OverlayManager::getSingleton();
	mCompassOverlay = (Ogre::Overlay*)omgr.getByName("CompassOverlay");
}

OverlayCompassImpl::~OverlayCompassImpl()
{
	if (mCompassOverlay) {
		mCompassOverlay->hide();
/*		Ogre::OverlayManager& omgr = Ogre::OverlayManager::getSingleton();
		omgr.destroy(mCompassOverlay);*/
	}

}

void OverlayCompassImpl::_setCompass(Compass* compass)
{
	if (mCompassOverlay)
	{
 		mCompassMaterial = (Ogre::MaterialPtr)Ogre::MaterialManager::getSingleton().getByName("/ui/compass");
 		mCompassMaterial->getBestTechnique()->getPass(0)->getTextureUnitState(0)->setTextureName(mMap->getTexture()->getName());
		mCompassOverlay->show();
	}
}

void OverlayCompassImpl::reposition(float x, float y)
{
	mMap->getView().reposition(Ogre::Vector2(x, y));
	
	if (!mCompassMaterial.isNull()) {
		Ogre::TextureUnitState* tus(mCompassMaterial->getBestTechnique()->getPass(0)->getTextureUnitState(0));
		const Ogre::Vector2& relPosition(mMap->getView().getRelativeViewPosition());
		tus->setTextureScroll(-0.5f + relPosition.x, -0.5f + relPosition.y);
	}
}

void OverlayCompassImpl::rotate(const Ogre::Degree& degree)
{
	return;
// 	Point<2> center = Ogre2Atlas(mMap->getView().getRelativeViewPosition());
// 	Vector<2> dimensions(0.5, 0.5);
// 	Quaternion qRot;
// 	qRot = qRot.rotation(2, degree);
// 	
// 	RotBox<2> rotbox(center, dimensions, qRot);



	if (!mCompassMaterial.isNull()) {
		Ogre::TextureUnitState* tus(mCompassMaterial->getBestTechnique()->getPass(0)->getTextureUnitState(0));
 		tus->setTextureRotate(degree);
		
//  		tus->setTextureRotate(EmberOgre::getSingleton().getMainCamera()->getYaw());
 	}
}

CompositorCompassImpl::CompositorCompassImpl()
: mRenderTexture(0), mCompositor(0), mCamera(0), mSceneManager(0), mViewport(0), mCompassMaterialMapTUS(0)
{
	mSceneManager = Ogre::Root::getSingleton().createSceneManager(Ogre::ST_GENERIC, "CompositorCompassImpl_sceneManager");
	mSceneManager->setFog(Ogre::FOG_NONE, Ogre::ColourValue(1,1,1,1), 0.0f, 10000000.0f, 100000001.0f);
}

CompositorCompassImpl::~CompositorCompassImpl()
{
	if (mCompositor && mRenderTexture) {
		Ogre::CompositorManager::getSingleton().removeCompositorChain(mRenderTexture->getViewport(0));
	}

	if (!mTexture.isNull()) {
		Ogre::TextureManager::getSingleton().remove(mTexture->getName());
	}
	
	if (mCamera) {
		mSceneManager->destroyCamera(mCamera);
	}
	
	Ogre::Root::getSingleton().destroySceneManager(mSceneManager);
	
}

void CompositorCompassImpl::reposition(float x, float y)
{
	mMap->getView().reposition(Ogre::Vector2(x, y));
	
	if (mCompassMaterialMapTUS) {
		const Ogre::Vector2& relPosition(mMap->getView().getRelativeViewPosition());
   		mCompassMaterialMapTUS->setTextureScroll(-0.5f + relPosition.x, -0.5f + relPosition.y);
		
		///We must mark the chain as dirty, else our changes won't apply
		mCompositor->getChain()->_markDirty();
// 		mViewport->update();
		mRenderTexture->update();
	}
}

void CompositorCompassImpl::rotate(const Ogre::Degree& degree)
{
	///We won't rotate the image.
	return;
}

void CompositorCompassImpl::_setCompass(Compass* compass)
{
	mCompassMaterial = static_cast<Ogre::MaterialPtr>(Ogre::MaterialManager::getSingleton().getByName("/ui/compass"));
	if (!mCompassMaterial.isNull()) {
		if (Ogre::Technique* tech = mCompassMaterial->getBestTechnique()) {
			Ogre::Pass* pass(0);
			if (tech->getNumPasses() && (pass = tech->getPass(0))) {
				if ((mCompassMaterialMapTUS = pass->getTextureUnitState("Background"))) {
					mCompassMaterialMapTUS->setTextureName(mMap->getTexture()->getName());
		
					mTexture = Ogre::TextureManager::getSingleton().createManual("ComposedCompass", "Gui", Ogre::TEX_TYPE_2D, 128, 128, 0, Ogre::PF_A8R8G8B8, Ogre::TU_RENDERTARGET);
					mRenderTexture = mTexture->getBuffer()->getRenderTarget();
					mRenderTexture->removeAllViewports();
					mRenderTexture->setAutoUpdated(false);
					mRenderTexture->setActive(true);
					
					mCamera = mSceneManager->createCamera("ComposedCompassCamera");
					mViewport = mRenderTexture->addViewport(mCamera);
					mViewport->setOverlaysEnabled(false);
					mViewport->setShadowsEnabled(false);
					mViewport->setSkiesEnabled(false);
					mViewport->setClearEveryFrame(true);
					mViewport->setBackgroundColour(Ogre::ColourValue::ZERO);
				
					mCompositor = Ogre::CompositorManager::getSingleton().addCompositor(mViewport, "/ui/compass");
					mCompositor->setEnabled(true);
					
					///Return early since everything is good.
					return;
				}
			}
		}
	} 
	S_LOG_WARNING("Could not load material '/ui/compass' for the compass.");
}

Ogre::TexturePtr CompositorCompassImpl::getTexture()
{
	return mTexture;
}


CompassAnchor::CompassAnchor(Compass& compass, const Ogre::Vector3& position, const Ogre::Quaternion& orientation)
: mCompass(compass)
, mPreviousX(0)
, mPreviousZ(0)
, mPosition(position)
, mOrientation(orientation)
{
	/// Register this as a frame listener
	Ogre::Root::getSingleton().addFrameListener(this);
}

CompassAnchor::~CompassAnchor()
{
	Ogre::Root::getSingleton().removeFrameListener(this);
}

bool CompassAnchor::frameStarted(const Ogre::FrameEvent& event)
{
	mCompass.rotate(-mOrientation.getYaw());
	if (mPosition.x != mPreviousX || mPosition.z != mPreviousZ) {
		mCompass.reposition(mPosition.x, mPosition.z);
		mPreviousX = mPosition.x;
		mPreviousZ = mPosition.z;
	}
	return true;
}




CompassCameraAnchor::CompassCameraAnchor(Compass& compass, Ogre::Camera* camera)
: mAnchor(compass, camera->getDerivedPosition(), camera->getDerivedOrientation())
, mCamera(camera)
{
}

CompassCameraAnchor::~CompassCameraAnchor()
{
}


CompassSceneNodeAnchor::CompassSceneNodeAnchor(Compass& compass, Ogre::SceneNode* sceneNode)
: mAnchor(compass, sceneNode->_getDerivedPosition(), sceneNode->_getDerivedOrientation())
, mSceneNode(sceneNode)
{
}

CompassSceneNodeAnchor::~CompassSceneNodeAnchor()
{
}




}

}
