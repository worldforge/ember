/*
 Copyright (C) 2013 Erik Ogenvik

 This program is free software; you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation; either version 2 of the License, or
 (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program; if not, write to the Free Software Foundation,
 Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <Ogre.h>

#include <utility>
#include "EmberTerrain.h"

namespace Ember {
namespace OgreView {
namespace Terrain {

const Ogre::uint16 EmberTerrain::WORKQUEUE_GEOMETRY_UPDATE_REQUEST = 100;


EmberTerrain::EmberTerrain(std::function<void()>& unloader,
						   Ogre::SceneManager* sm,
						   sigc::signal<void, const Ogre::TRect<Ogre::Real>&>& terrainAreaUpdatedSignal,
						   sigc::signal<void, const Ogre::TRect<Ogre::Real>&>& terrainShownSignal,
						   Ogre::TerrainMaterialGeneratorPtr materialGenerator) :
		Ogre::Terrain(sm),
		mUnloader(unloader),
		mTerrainAreaUpdatedSignal(terrainAreaUpdatedSignal),
		mTerrainShownSignal(terrainShownSignal) {
	//This is a hack to prevent the Terrain class from creating blend map textures.
	//Since we provide our own material with its own blend maps we don't want the
	//Ogre::Terrain instance creating new ones.
	//Since most methods in the base class aren't virtual we can't override the
	//createGPUBlendTextures() method. Instead we have to trick it into thinking
	//that all textures already have been created.
	//This of course means that we can't touch _any_ method which in some way interacts
	//with the layers and blend maps.
	mBlendTextureList.emplace_back();

	mMaterialGenerator = materialGenerator;

}

EmberTerrain::~EmberTerrain() {
	//Remove the fake blend map texture, else the base class will try to delete it.
	mBlendTextureList.clear();

	//The materials are owned by the TerrainPageSurface class, so we shouldn't let Ogre::Terrain delete them.
	mMaterial.reset();
	mCompositeMapMaterial.reset();
	mUnloader();
}

void EmberTerrain::setIndex(const IPageDataProvider::OgreIndex& index) {
	mIndex = index;
}

const IPageDataProvider::OgreIndex& EmberTerrain::getIndex() const {
	return mIndex;
}

bool EmberTerrain::canHandleRequest(const Ogre::WorkQueue::Request* req, const Ogre::WorkQueue* srcQ) {

	if (req->getType() == WORKQUEUE_GEOMETRY_UPDATE_REQUEST) {
		GeometryUpdateRequest innerRequest = Ogre::any_cast<GeometryUpdateRequest>(req->getData());
		// only deal with own requests
		// we do this because if we delete a terrain we want any pending tasks to be discarded
		if (innerRequest.terrain != this) {
			return false;
		}
	}
	return Ogre::Terrain::canHandleRequest(req, srcQ);
}

Ogre::WorkQueue::Response* EmberTerrain::handleRequest(const Ogre::WorkQueue::Request* req, const Ogre::WorkQueue* srcQ) {
	if (req->getType() == WORKQUEUE_GEOMETRY_UPDATE_REQUEST) {
		GeometryUpdateRequest innerRequest = Ogre::any_cast<GeometryUpdateRequest>(req->getData());
		float* heightData = getHeightData();
		//Copy height data
		memcpy(heightData, innerRequest.heightData.get(), sizeof(float) * getSize() * getSize());
		innerRequest.heightData.reset(); //Free up memory as soon as possible
		return new Ogre::WorkQueue::Response(req, true, Ogre::Any(innerRequest));
	} else {
		return Ogre::Terrain::handleRequest(req, srcQ);
	}
}

bool EmberTerrain::canHandleResponse(const Ogre::WorkQueue::Response* res, const Ogre::WorkQueue* srcQ) {
	if (res->getRequest()->getType() == WORKQUEUE_GEOMETRY_UPDATE_REQUEST) {
		GeometryUpdateRequest innerRequest = Ogre::any_cast<GeometryUpdateRequest>(res->getData());
		// only deal with own requests
		// we do this because if we delete a terrain we want any pending tasks to be discarded
		if (innerRequest.terrain != this) {
			return false;
		}
	}
	return Ogre::Terrain::canHandleResponse(res, srcQ);
}

void EmberTerrain::handleResponse(const Ogre::WorkQueue::Response* res, const Ogre::WorkQueue* srcQ) {
	if (res->getRequest()->getType() == WORKQUEUE_GENERATE_MATERIAL_REQUEST) {
		bool wasLoaded = mIsLoaded;
		Ogre::Terrain::handleResponse(res, srcQ);
		if (mIsLoaded && !wasLoaded) {
			//The page is now loaded, while it wasn't before
			const Ogre::AxisAlignedBox& bbox = getWorldAABB();
			Ogre::TRect<Ogre::Real> rect(bbox.getMinimum().x, bbox.getMinimum().z, bbox.getMaximum().x, bbox.getMaximum().z);
			mTerrainShownSignal(rect);
		}
	} else if (res->getRequest()->getType() == WORKQUEUE_DERIVED_DATA_REQUEST) {

		bool wasUpdatingDerivedData = mDerivedDataUpdateInProgress;
		auto dirtyRect = mDirtyDerivedDataRect;
		Ogre::Terrain::handleResponse(res, srcQ);
		if (!mDerivedDataUpdateInProgress && wasUpdatingDerivedData) {
			//We've finished updating derived data and should signal that we've altered.
			Ogre::TRect<Ogre::Real> rect(dirtyRect.left, dirtyRect.top, dirtyRect.right, dirtyRect.bottom);

			mTerrainAreaUpdatedSignal(rect);
		}
	} else if (res->getRequest()->getType() == WORKQUEUE_GEOMETRY_UPDATE_REQUEST) {
		dirty();
		update();
	} else {
		Ogre::Terrain::handleResponse(res, srcQ);
	}
}

void EmberTerrain::regenerateMaterial() {
	mMaterialDirty = true;
	mMaterialGenerator->requestOptions(this);
	getMaterial();
}

void EmberTerrain::scheduleGeometryUpdate(std::shared_ptr<std::vector<float>> heightData) {

	GeometryUpdateRequest req;
	req.terrain = this;
	req.heightData = std::move(heightData);

	Ogre::Root::getSingleton().getWorkQueue()->addRequest(
			mWorkQueueChannel, WORKQUEUE_GEOMETRY_UPDATE_REQUEST,
			Ogre::Any(req), 0, false);
}


}
}
}
