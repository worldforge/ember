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
 along with this program; if not, write to the Free Software
 Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "EmberTerrain.h"

namespace Ember
{
namespace OgreView
{
namespace Terrain
{

EmberTerrain::EmberTerrain(std::function<void()>& unloader,
						   Ogre::SceneManager* sm,
						   sigc::signal<void, Ogre::TRect<Ogre::Real>>& terrainAreaUpdatedSignal,
						   sigc::signal<void, const Ogre::TRect<Ogre::Real>>& terrainShownSignal,
						   Ogre::TerrainMaterialGeneratorPtr materialGenerator) :
		Ogre::Terrain(sm),
		mUnloader(unloader),
		mTerrainAreaUpdatedSignal(terrainAreaUpdatedSignal),
		mTerrainShownSignal(terrainShownSignal)
{
	//This is a hack to prevent the Terrain class from creating blend map textures.
	//Since we provide our own material with its own blend maps we don't want the
	//Ogre::Terrain instance creating new ones.
	//Since most methods in the base class aren't virtual we can't override the
	//createGPUBlendTextures() method. Instead we have to trick it into thinking
	//that all textures already have been created.
	//This of course means that we can't touch _any_ method which in some way interacts
	//with the layers and blend maps.
	mBlendTextureList.push_back(Ogre::TexturePtr());

	mMaterialGenerator = materialGenerator;

}

EmberTerrain::~EmberTerrain()
{
	//Remove the fake blend map texture, else the base class will try to delete it.
	mBlendTextureList.clear();
	mUnloader();
}

void EmberTerrain::setIndex(const IPageDataProvider::OgreIndex& index)
{
	mIndex = index;
}

const IPageDataProvider::OgreIndex& EmberTerrain::getIndex() const
{
	return mIndex;
}

void EmberTerrain::handleResponse(const Ogre::WorkQueue::Response* res, const Ogre::WorkQueue* srcQ)
{

	bool wasUpdatingDerivedData = mDerivedDataUpdateInProgress;
	auto dirtyRect = mDirtyDerivedDataRect;
	bool wasLoaded = mIsLoaded;
	Ogre::Terrain::handleResponse(res, srcQ);
	if (mIsLoaded && !wasLoaded) {
		//The page is now loaded, while it wasn't before
		const Ogre::AxisAlignedBox& bbox = getWorldAABB();
		Ogre::TRect<Ogre::Real> rect(bbox.getMinimum().x, bbox.getMinimum().z, bbox.getMaximum().x, bbox.getMaximum().z);
		mTerrainShownSignal(rect);
	}
	if (!mDerivedDataUpdateInProgress && wasUpdatingDerivedData) {
		//We've finished updating derived data and should signal that we've altered.
		Ogre::TRect<Ogre::Real> rect(dirtyRect.left, dirtyRect.top, dirtyRect.right, dirtyRect.bottom);

		mTerrainAreaUpdatedSignal(rect);
	}
}

void EmberTerrain::regenerateMaterial()
{
	mMaterialDirty = true;
	mMaterialGenerator->requestOptions(this);
	getMaterial();
}


}
}
}
