//
// C++ Implementation: Map
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

#include "Map.h"
#include "../EmberOgre.h"
#include <OgreCamera.h>

namespace EmberOgre {

namespace Terrain {

Map::Map()
:
mRenderTexture(0)
, mTexturePixelSize(256)
, mMetersPerPixel(1.0f)
, mCamera(*this, EmberOgre::getSingleton().getSceneManager())
, mView(*this, mCamera)
{
}


Map::~Map()
{
}

void Map::initialize()
{
	createTexture();
	setupCamera();
}

void Map::setupCamera()
{
	mCamera.setRenderTarget(mRenderTexture);
	reposition(Ogre::Vector2(0, 0));
}

void Map::createTexture()
{
	///don't use alpha for our map texture
	mTexture = Ogre::TextureManager::getSingleton().createManual("TerrainMap", "Gui", Ogre::TEX_TYPE_2D, mTexturePixelSize, mTexturePixelSize, 0, Ogre::PF_R8G8B8,Ogre::TU_RENDERTARGET);
	mRenderTexture = mTexture->getBuffer()->getRenderTarget();
	mRenderTexture->removeAllViewports();
	
	mRenderTexture->setAutoUpdated(false);
	///initially deactivate it until setActive(true) is called
// 	mRenderTexture->setActive(false);

}

void Map::render()
{
	mCamera.render();
}

void Map::reposition(Ogre::Vector2 pos)
{
	mCamera.reposition(pos);
}

void Map::reposition(float x, float y)
{
	reposition(Ogre::Vector2(x, y));
}


void Map::setDistance(float distance)
{
	mCamera.setDistance(distance);
}

float Map::getDistance() const
{
	return mCamera.getDistance();
}

Ogre::TexturePtr Map::getTexture() const
{
	return mTexture;
}

Ogre::RenderTexture* Map::getRenderTexture() const
{
	return mRenderTexture;
}

float Map::getResolution() const
{
	return mMetersPerPixel;
}

void Map::setResolution(float metersPerPixel)
{
	mMetersPerPixel = metersPerPixel;
	render();
}

float Map::getResolutionMeters() const
{
	return mTexturePixelSize * mMetersPerPixel;
}

MapView& Map::getView()
{
	return mView;
}





MapView::MapView(Map& map, MapCamera& mapCamera)
: mMap(map)
, mMapCamera(mapCamera)
, mViewSize(0.5)
{
	///set it to invalid values so we'll force an update when it's repositioned
	mFullBounds.left = 1;
	mFullBounds.right = -1;
	mFullBounds.top = 1;
	mFullBounds.bottom = -1;
}

bool MapView::reposition(Ogre::Vector2 pos)
{
	int halfViewSizeMeters((mMap.getResolutionMeters() * mViewSize)/ 2);
	///check if we need to reposition the camera
	if (pos.x - halfViewSizeMeters < mFullBounds.left || pos.x + halfViewSizeMeters > mFullBounds.right
		|| pos.y - halfViewSizeMeters < mFullBounds.top || pos.y + halfViewSizeMeters > mFullBounds.bottom) {
		mMapCamera.reposition(pos);
		mMapCamera.render();
		
		mFullBounds.left = static_cast<int>(pos.x - (mMap.getResolutionMeters() / 2));
		mFullBounds.right = static_cast<int>(pos.x + (mMap.getResolutionMeters() / 2));
		mFullBounds.top = static_cast<int>(pos.y - (mMap.getResolutionMeters() / 2));
		mFullBounds.bottom = static_cast<int>(pos.y + (mMap.getResolutionMeters() / 2));
		
		
		
		mVisibleRelativeBounds.left = 0.5f - (mViewSize / 2);
		mVisibleRelativeBounds.right= 0.5f + (mViewSize / 2);
		mVisibleRelativeBounds.top = 0.5f - mViewSize / 2;
		mVisibleRelativeBounds.bottom= 0.5f + (mViewSize / 2);
		mRelativeViewPosition.x = 0.5f;
		mRelativeViewPosition.y = 0.5f;

		return true;
	}
	mRelativeViewPosition.x = (pos.x - mFullBounds.left) / static_cast<float>(mMap.getResolutionMeters());
	mRelativeViewPosition.y = (pos.y - mFullBounds.top) / static_cast<float>(mMap.getResolutionMeters());
	float halfViewSize = mViewSize / 2;
	mVisibleRelativeBounds.left = mRelativeViewPosition.x - halfViewSize;
	mVisibleRelativeBounds.right= mRelativeViewPosition.x + halfViewSize;
	mVisibleRelativeBounds.top = mRelativeViewPosition.y - halfViewSize;
	mVisibleRelativeBounds.bottom= mRelativeViewPosition.y + halfViewSize;
	return false;
	
}

const Ogre::TRect<float>& MapView::getRelativeViewBounds() const
{
	return mVisibleRelativeBounds;
}

const Ogre::Vector2& MapView::getRelativeViewPosition() const
{
	return mRelativeViewPosition;
}





MapCamera::MapCamera(Map& map, Ogre::SceneManager* manager)
: mMap(map), mViewport(0), mDistance(400)
{
	mCamera = manager->createCamera("TerrainMapCamera");
	mCamera->setPosition(Ogre::Vector3(0, 0, 0));
	///look down
	mCamera->pitch(Ogre::Degree(-90));
	///we want really low LOD on this camera
	mCamera->setLodBias(0.0001f);
	
	setDistance(mDistance);
	
// 	mCamera->setFOVy(Ogre::Degree(30));
// 	mCamera->setProjectionType(Ogre::PT_ORTHOGRAPHIC);

	mCamera->addQueryFlags(Ogre::SceneManager::WORLD_GEOMETRY_TYPE_MASK);

}

MapCamera::~MapCamera()
{
}


void MapCamera::setRenderTarget(Ogre::RenderTarget* renderTarget)
{
	mViewport = renderTarget->addViewport(mCamera);
	mViewport->setBackgroundColour(Ogre::ColourValue::White);
	///don't show the CEGUI
	mViewport->setOverlaysEnabled(false);
	mViewport->setShadowsEnabled(false);
	mViewport->setSkiesEnabled(false);
    mViewport->setClearEveryFrame(true);
    
    mViewport->setVisibilityMask(Ogre::SceneManager::WORLD_GEOMETRY_TYPE_MASK);
}

void MapCamera::setDistance(float distance)
{
	mDistance = distance;
	const Ogre::Vector3& position(mCamera->getPosition());
	mCamera->setPosition(position.x, distance, position.z);
}

float MapCamera::getDistance() const
{
	return mDistance;
}

void MapCamera::reposition(Ogre::Vector2 pos)
{
	mCamera->setPosition(pos.x, mDistance, pos.y);
}

void MapCamera::render()
{
	mCamera->setNearClipDistance(1);
	mCamera->setFarClipDistance(mDistance * 200);
	
	mCamera->setProjectionType(Ogre::PT_ORTHOGRAPHIC);
	mCamera->setOrthoWindow(mMap.getResolutionMeters(), mMap.getResolutionMeters());
	mCamera->setAspectRatio(1.0);
	{
		///use a RAII rendering instance so that we're sure to reset all settings of the scene manager that we change, even if something goes wrong here
		Ogre::SceneManager* manager(mCamera->getSceneManager());
		RenderingInstance instance(manager);
		
/*		manager->setSpecialCaseRenderQueueMode(Ogre::SceneManager::SCRQM_INCLUDE); 
		manager->addSpecialCaseRenderQueue(Ogre::RENDER_QUEUE_WORLD_GEOMETRY_1);
		manager->addSpecialCaseRenderQueue(Ogre::RENDER_QUEUE_WORLD_GEOMETRY_2);
		manager->addSpecialCaseRenderQueue(Ogre::RENDER_QUEUE_6); //water*/
		
		
		manager->setFog(Ogre::FOG_NONE, Ogre::ColourValue(1,1,1,1), 0.0f, 10000000.0f, 100000001.0f);
		mViewport->update();
// 		manager->removeSpecialCaseRenderQueue(Ogre::RENDER_QUEUE_WORLD_GEOMETRY_1);
// 		manager->removeSpecialCaseRenderQueue(Ogre::RENDER_QUEUE_WORLD_GEOMETRY_2);
// 		manager->removeSpecialCaseRenderQueue(Ogre::RENDER_QUEUE_6);

	}
}









RenderingInstance::RenderingInstance(Ogre::SceneManager* manager)
: mManager(manager)
, mFogMode(manager->getFogMode())
, mFogColour(manager->getFogColour())
, mFogDensity(manager->getFogDensity())
, mFogStart(manager->getFogStart())
, mFogEnd(manager->getFogEnd())
, mSpecialCaseRenderQueueMode(manager->getSpecialCaseRenderQueueMode())
{
	
}

RenderingInstance::~RenderingInstance()
{
	mManager->setFog(mFogMode, mFogColour, mFogDensity, mFogStart, mFogEnd);
	
	mManager->setSpecialCaseRenderQueueMode(mSpecialCaseRenderQueueMode);
}



}

}
