//
// C++ Implementation: FoliageBase
//
// Description:
//
//
// Author: Erik Hjortsberg <erik.hjortsberg@gmail.com>, (C) 2008
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

#include "FoliageBase.h"

#include "framework/LoggingInstance.h"

#include "../Convert.h"
#include "../terrain/TerrainArea.h"
#include "../terrain/TerrainShader.h"
#include "../terrain/TerrainLayerDefinition.h"
#include "../terrain/TerrainPageSurfaceLayer.h"
#include "../terrain/TerrainPageSurface.h"
#include "../terrain/TerrainPage.h"
#include "../terrain/TerrainLayerDefinition.h"
#include "../terrain/TerrainLayerDefinitionManager.h"

#include "pagedgeometry/include/PagedGeometry.h"

using namespace EmberOgre::Terrain;

namespace EmberOgre {

namespace Environment {

FoliageBase::FoliageBase(Terrain::TerrainManager& terrainManager, const Terrain::TerrainLayerDefinition& terrainLayerDefinition, const Terrain::TerrainFoliageDefinition& foliageDefinition)
: mTerrainManager(terrainManager), mTerrainLayerDefinition(terrainLayerDefinition)
, mFoliageDefinition(foliageDefinition)
, mPagedGeometry(0)
{
	initializeDependentLayers();

	mTerrainManager.EventLayerUpdated.connect(sigc::mem_fun(*this, &FoliageBase::TerrainManager_LayerUpdated));
	mTerrainManager.EventShaderCreated.connect(sigc::mem_fun(*this, &FoliageBase::TerrainManager_EventShaderCreated));
	mTerrainManager.EventAfterTerrainUpdate.connect(sigc::mem_fun(*this, &FoliageBase::TerrainManager_AfterTerrainUpdate));

}

FoliageBase::~FoliageBase()
{
	delete mPagedGeometry;
}

void FoliageBase::initializeDependentLayers()
{
	bool foundLayer(false);
	for (TerrainLayerDefinitionManager::DefinitionStore::const_iterator I = TerrainLayerDefinitionManager::getSingleton().getDefinitions().begin(); I != TerrainLayerDefinitionManager::getSingleton().getDefinitions().end(); ++I) {

		if (foundLayer) {
			mDependentDefinitions.push_back((*I));
		} else if (!foundLayer && (*I) == &mTerrainLayerDefinition) {
			foundLayer = true;
		}
	}
}

void FoliageBase::TerrainManager_LayerUpdated(const Terrain::TerrainShader* shader, const AreaStore* areas)
{
	if (mPagedGeometry) {
		///check if the layer update affects this layer, either if it's the actual layer, or one of the dependent layers
		bool isRelevant(0);
		if (shader->getLayerDefinition() == &mTerrainLayerDefinition) {
			isRelevant = true;
		} else {
			if (std::find(mDependentDefinitions.begin(), mDependentDefinitions.end(), shader->getLayerDefinition()) != mDependentDefinitions.end()) {
				isRelevant = true;
			}
		}
		if (isRelevant) {
			///if there are areas sent, the update only affect those and we only need to update the affected areas
			if (areas) {
				for (AreaStore::const_iterator I = areas->begin(); I != areas->end(); ++I) {
					const Ogre::TRect<Ogre::Real> ogreExtent(Convert::toOgre(*I));
					Ogre::Real pageSize(mPagedGeometry->getPageSize());
					Ogre::Vector3 pos(ogreExtent.left, 0, ogreExtent.top);
					for (; pos.x < ogreExtent.right; pos.x += pageSize) {
						for (; pos.z < ogreExtent.bottom; pos.z += pageSize) {
							mPagedGeometry->reloadGeometryPage(pos);
						}
					}
				}
			} else {
				mPagedGeometry->reloadGeometry();
			}
		}
	}
}

void FoliageBase::TerrainManager_EventShaderCreated(const Terrain::TerrainShader* shader)
{
	///we'll assume that all shaders that are created after this foliage has been created will affect it, so we'll add it to the dependent layers and reload the geometry
	mDependentDefinitions.push_back(shader->getLayerDefinition());
	if (mPagedGeometry) {
		mPagedGeometry->reloadGeometry();
	}
}

void FoliageBase::TerrainManager_AfterTerrainUpdate(const std::vector<TerrainPosition>& terrainPositions, const std::set< ::EmberOgre::Terrain::TerrainPage* >& pages)
{
	if (mPagedGeometry) {
		mPagedGeometry->reloadGeometry();
//HACK: for some reason I couldn't get the code below to work as it should, so we'll go with the brute way of just reloading all geometry. Newer versions of PagedGeometry contains additional methods for reloading sections of the geometry which we shoudl be able to use once we've updated the version used in Ember.
#if 0
		Ogre::Real pageSize(mPagedGeometry->getPageSize());
		for (std::set< ::EmberOgre::Terrain::TerrainPage* >::const_iterator I = pages.begin(); I != pages.end(); ++I) {
			const ::EmberOgre::Terrain::TerrainPage* page(*I);
			const Ogre::TRect<Ogre::Real> ogreExtent(Convert::toOgre(page->getExtent()));

			///update all paged geometry pages that are within the extent of the terrain page
			Ogre::Vector3 pos(ogreExtent.left, 0, ogreExtent.top);
			for (; pos.x < ogreExtent.right; pos.x += pageSize) {
				for (; pos.z < ogreExtent.bottom; pos.z += pageSize) {
					mPagedGeometry->reloadGeometryPage(pos);
				}
			}
		}
#endif
	}
}

//Gets the height of the terrain at the specified x/z coordinate
//The userData parameter isn't used in this implementation of a height function, since
//there's no need for extra data other than the x/z coordinates.
float getTerrainHeight(float x, float z, void* userData)
{
	TerrainManager* terrainManager = static_cast<TerrainManager*>(userData);
	float height = 0;
	terrainManager->getHeight(TerrainPosition(x, -z), height);
	return height;
}


}

}
