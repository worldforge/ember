//
// C++ Implementation: FoliageBase
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

#include "FoliageBase.h"

#include "services/EmberServices.h"
#include "services/logging/LoggingService.h"

#include "../EmberOgre.h"
#include "../terrain/TerrainGenerator.h"
#include "../terrain/TerrainArea.h"
#include "../terrain/TerrainShader.h"

#include "../terrain/TerrainLayerDefinition.h"
#include "../terrain/TerrainPageSurfaceLayer.h"
#include "../terrain/TerrainPageSurface.h"
#include "../terrain/TerrainPage.h"
#include "../terrain/TerrainLayerDefinition.h"
#include "../terrain/TerrainLayerDefinitionManager.h"

#include "pagedgeometry/include/PagedGeometry.h"

#include <Mercator/Area.h>

using namespace EmberOgre::Terrain;

namespace EmberOgre {

namespace Environment {

FoliageBase::FoliageBase(const Terrain::TerrainLayerDefinition& terrainLayerDefinition, const Terrain::TerrainFoliageDefinition& foliageDefinition)
: mTerrainLayerDefinition(terrainLayerDefinition)
, mFoliageDefinition(foliageDefinition)
, mPagedGeometry(0)
{
	initializeDependentLayers();
	
	EmberOgre::getSingleton().getTerrainGenerator()->EventLayerUpdated.connect(sigc::mem_fun(*this, &FoliageBase::TerrainGenerator_LayerUpdated));
	EmberOgre::getSingleton().getTerrainGenerator()->EventShaderCreated.connect(sigc::mem_fun(*this, &FoliageBase::TerrainGenerator_EventShaderCreated));
	
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

void FoliageBase::TerrainGenerator_LayerUpdated(Terrain::TerrainShader* shader, std::vector<Terrain::TerrainArea*>* areas)
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
				for (std::vector<Terrain::TerrainArea*>::iterator I = areas->begin(); I != areas->end(); ++I) {
					const Ogre::TRect<Ogre::Real> ogreExtent(Atlas2Ogre((*I)->getArea()->bbox()));
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

void FoliageBase::TerrainGenerator_EventShaderCreated(Terrain::TerrainShader* shader)
{
	///we'll assume that all shaders that are created after this foliage has been created will affect it, so we'll add it to the dependent layers and reload the geometry
	mDependentDefinitions.push_back(shader->getLayerDefinition());
	mPagedGeometry->reloadGeometry();
}

}

}
