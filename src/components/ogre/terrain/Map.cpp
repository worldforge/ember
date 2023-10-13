//
// C++ Implementation: Map
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
#include "Map.h"
#include <OgreHardwarePixelBuffer.h>
#include <OgreTextureManager.h>
#include <OgreRenderTexture.h>
#include <OgreCamera.h>
#include <OgreRenderTarget.h>
#include <OgreViewport.h>
#include <OgreSceneNode.h>

namespace Ember {
namespace OgreView {

namespace Terrain {

Map::Map(Ogre::SceneManager& sceneManager)
		:
		mRenderTexture(0),
		mTexturePixelSize(256),
		mMetersPerPixel(1.0f),
		mCamera(*this, sceneManager),
		mView(*this, mCamera) {
}


Map::~Map() {
	if (mTexture) {
		Ogre::TextureManager::getSingleton().remove(mTexture->getName(), Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
	}
}

void Map::initialize() {
	createTexture();
	setupCamera();
}

void Map::setupCamera() {
	mCamera.setRenderTarget(mRenderTexture);
	reposition(Ogre::Vector2(0, 0));
}

void Map::createTexture() {
	//don't use alpha for our map texture
	mTexture = Ogre::TextureManager::getSingleton().createManual("TerrainMap", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, Ogre::TEX_TYPE_2D, mTexturePixelSize, mTexturePixelSize, 1, Ogre::PF_R8G8B8, Ogre::TU_RENDERTARGET);
	mRenderTexture = mTexture->getBuffer()->getRenderTarget();
	mRenderTexture->removeAllViewports();

	mRenderTexture->setAutoUpdated(false);
	//initially deactivate it until setActive(true) is called
// 	mRenderTexture->setActive(false);

}

void Map::render() {
	mCamera.render();
}

void Map::reposition(const Ogre::Vector2& pos) {
	mCamera.reposition(pos);
}

void Map::reposition(float x, float y) {
	reposition(Ogre::Vector2(x, y));
}


void Map::setDistance(float distance) {
	mCamera.setDistance(distance);
}

float Map::getDistance() const {
	return mCamera.getDistance();
}

Ogre::TexturePtr Map::getTexture() const {
	return mTexture;
}

Ogre::RenderTexture* Map::getRenderTexture() const {
	return mRenderTexture;
}

float Map::getResolution() const {
	return mMetersPerPixel;
}

void Map::setResolution(float metersPerPixel) {
	if (metersPerPixel > 0) {
		mMetersPerPixel = metersPerPixel;
		mView.recalculateBounds();
	}
}

float Map::getResolutionMeters() const {
	return (float) mTexturePixelSize * mMetersPerPixel;
}

MapView& Map::getView() {
	return mView;
}


MapView::MapView(Map& map, MapCamera& mapCamera)
		:
//set it to invalid values so we'll force an update when it's repositioned
		mFullBounds(1, 1, -1, -1), mMap(map), mMapCamera(mapCamera), mViewSize(0.5) {
}

bool MapView::reposition(const Ogre::Vector2& pos) {
	auto halfViewSizeMeters = ((mMap.getResolutionMeters() * mViewSize) / 2.0);
	//check if we need to reposition the camera
	if (pos.x - halfViewSizeMeters < mFullBounds.left || pos.x + halfViewSizeMeters > mFullBounds.right
		|| pos.y - halfViewSizeMeters < mFullBounds.bottom || pos.y + halfViewSizeMeters > mFullBounds.top) {
		mMapCamera.reposition(pos);
		mMapCamera.render();

		recalculateBounds();

		return true;
	}
	mRelativeViewPosition.x = (pos.x - (Ogre::Real) mFullBounds.left) / static_cast<Ogre::Real>(mMap.getResolutionMeters());
	mRelativeViewPosition.y = (pos.y - (Ogre::Real) mFullBounds.bottom) / static_cast<Ogre::Real>(mMap.getResolutionMeters());
	float halfViewSize = mViewSize / 2;
	mVisibleRelativeBounds.left = mRelativeViewPosition.x - halfViewSize;
	mVisibleRelativeBounds.right = mRelativeViewPosition.x + halfViewSize;
	mVisibleRelativeBounds.bottom = mRelativeViewPosition.y - halfViewSize;
	mVisibleRelativeBounds.top = mRelativeViewPosition.y + halfViewSize;
	return false;

}


const Ogre::Vector2& MapView::getRelativeViewPosition() const {
	return mRelativeViewPosition;
}

const Ogre::TRect<int>& MapView::getFullBounds() const {
	return mFullBounds;
}


void MapView::recalculateBounds() {
	Ogre::Vector2 pos(mMapCamera.getPosition());
	mFullBounds.left = static_cast<int>(pos.x - (mMap.getResolutionMeters() / 2));
	mFullBounds.right = static_cast<int>(pos.x + (mMap.getResolutionMeters() / 2));
	mFullBounds.top = static_cast<int>(pos.y - (mMap.getResolutionMeters() / 2));
	mFullBounds.bottom = static_cast<int>(pos.y + (mMap.getResolutionMeters() / 2));


	mRelativeViewPosition.x = 0.5f;
	mRelativeViewPosition.y = 0.5f;
	EventBoundsChanged();
}


MapCamera::MapCamera(Map& map, Ogre::SceneManager& manager)
		: mMap(map), mCameraNode(nullptr), mViewport(nullptr), mDistance(400), mLightning(manager) {
	mCameraNode = manager.createSceneNode();

	mCamera = manager.createCamera("TerrainMapCamera");
	mCameraNode->attachObject(mCamera);
	mCameraNode->setPosition(Ogre::Vector3(0, 0, 0));
	//look down
	mCameraNode->pitch(Ogre::Degree(-90));
	//we want really low LOD on this camera
	mCamera->setLodBias(0.0001f);

	setDistance(mDistance);

// 	mCamera->setFOVy(Ogre::Degree(30));
// 	mCamera->setProjectionType(Ogre::PT_ORTHOGRAPHIC);

	mCamera->addQueryFlags(Ogre::SceneManager::WORLD_GEOMETRY_TYPE_MASK);

}

MapCamera::~MapCamera() {
	mCameraNode->getCreator()->destroySceneNode(mCameraNode);
	mCamera->getSceneManager()->destroyCamera(mCamera);
}


void MapCamera::setRenderTarget(Ogre::RenderTarget* renderTarget) {
	mViewport = renderTarget->addViewport(mCamera);
	mViewport->setBackgroundColour(Ogre::ColourValue::White);
	//don't show the CEGUI
	mViewport->setOverlaysEnabled(false);
	mViewport->setShadowsEnabled(false);
	mViewport->setSkiesEnabled(false);
	mViewport->setClearEveryFrame(true);
	mViewport->setMaterialScheme("Medium"); //This will disable shadows etc.

	mViewport->setVisibilityMask(Ogre::SceneManager::WORLD_GEOMETRY_TYPE_MASK);
}

void MapCamera::setDistance(float distance) {
	mDistance = distance;
	const Ogre::Vector3& position(mCameraNode->getPosition());
	mCameraNode->setPosition(position.x, distance, position.z);
}

float MapCamera::getDistance() const {
	return mDistance;
}

void MapCamera::reposition(const Ogre::Vector2& pos) {
	mCameraNode->setPosition(pos.x, mDistance, pos.y);
}

Ogre::Vector2 MapCamera::getPosition() const {
	return {mCameraNode->getPosition().x, mCameraNode->getPosition().z};
}


void MapCamera::render() {
	mCamera->setNearClipDistance(1);
	mCamera->setFarClipDistance(mDistance * 200);

	mCamera->setProjectionType(Ogre::PT_ORTHOGRAPHIC);
	mCamera->setOrthoWindow(mMap.getResolutionMeters(), mMap.getResolutionMeters());
	mCamera->setAspectRatio(1.0);
	{
		//use a RAII rendering instance so that we're sure to reset all settings of the scene manager that we change, even if something goes wrong here
		Ogre::SceneManager* manager(mCamera->getSceneManager());
		RenderingInstance instance(manager);
		MapCameraLightningInstance lightningInstance(mLightning);

/*		manager->setSpecialCaseRenderQueueMode(Ogre::SceneManager::SCRQM_INCLUDE);
		manager->addSpecialCaseRenderQueue(Ogre::RENDER_QUEUE_WORLD_GEOMETRY_1);
		manager->addSpecialCaseRenderQueue(Ogre::RENDER_QUEUE_WORLD_GEOMETRY_2);
		manager->addSpecialCaseRenderQueue(Ogre::RENDER_QUEUE_6); //water*/


		manager->setFog(Ogre::FOG_EXP2, Ogre::ColourValue(0, 0, 0, 0), 0.0f, 0.0f, 0.0f);
		mViewport->update();
// 		manager->removeSpecialCaseRenderQueue(Ogre::RENDER_QUEUE_WORLD_GEOMETRY_1);
// 		manager->removeSpecialCaseRenderQueue(Ogre::RENDER_QUEUE_WORLD_GEOMETRY_2);
// 		manager->removeSpecialCaseRenderQueue(Ogre::RENDER_QUEUE_6);

	}
}


MapCameraLightning::MapCameraLightning(Ogre::SceneManager& sceneManager)
		: mSceneManager(sceneManager) {
	mLightNode = sceneManager.createSceneNode();

	mLight = sceneManager.createLight("MapFixedSunLight");
	mLight->setType(Ogre::Light::LT_DIRECTIONAL);

	mLightNode->attachObject(mLight);

	mLightNode->setPosition(Ogre::Vector3(-500, 300, -350));
	Ogre::Vector3 dir = -mLightNode->getPosition();
	dir.normalise();
	mLightNode->setDirection(dir);

	mLight->setDiffuseColour(Ogre::ColourValue(0.8f, 0.8f, 0.6f)); //yellow
	//mSun->setSpecularColour(1, 1, 0.7); //yellow
	mLight->setCastShadows(false);
	mLight->setAttenuation(1000000, 1, 0, 0);

	mLight->setVisible(false);
}


MapCameraLightning::~MapCameraLightning() {
	mSceneManager.destroyLight(mLight);
	mSceneManager.destroySceneNode(mLightNode);
}

Ogre::Light* MapCameraLightning::getLight() {
	return mLight;
}

Ogre::SceneManager& MapCameraLightning::getSceneManager() {
	return mSceneManager;
}


MapCameraLightningInstance::MapCameraLightningInstance(MapCameraLightning& lightning)
		: mLightning(lightning) {

	for (auto& entry: lightning.getSceneManager().getMovableObjects(Ogre::LightFactory::FACTORY_TYPE_NAME)) {
		Ogre::MovableObject* light = entry.second;
		if (light && light != mLightning.getLight()) {
			if (light->getVisible()) {
				mVisibleLights.push_back(light);
				light->setVisible(false);
			}
		}
	}
	mLightning.getLight()->setVisible(true);
	mAmbientColour = mLightning.getSceneManager().getAmbientLight();
	mLightning.getSceneManager().setAmbientLight(mLightning.getLight()->getDiffuseColour());
}

MapCameraLightningInstance::~MapCameraLightningInstance() {
	for (auto light: mVisibleLights) {
		light->setVisible(true);
	}
	mLightning.getLight()->setVisible(false);
	mLightning.getSceneManager().setAmbientLight(mAmbientColour);
}


RenderingInstance::RenderingInstance(Ogre::SceneManager* manager)
		: mManager(manager), mFogMode(manager->getFogMode()), mFogColour(manager->getFogColour()), mFogDensity(manager->getFogDensity()), mFogStart(manager->getFogStart()), mFogEnd(manager->getFogEnd()), mSpecialCaseRenderQueueMode(manager->getSpecialCaseRenderQueueMode()) {

}

RenderingInstance::~RenderingInstance() {
	mManager->setFog(mFogMode, mFogColour, mFogDensity, mFogStart, mFogEnd);

	mManager->setSpecialCaseRenderQueueMode(mSpecialCaseRenderQueueMode);
}


}

}
}
