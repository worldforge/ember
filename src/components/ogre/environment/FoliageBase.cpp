//
// C++ Implementation: FoliageBase
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

#include "FoliageBase.h"

#include "../Convert.h"
#include "../terrain/TerrainArea.h"
#include "../terrain/TerrainManager.h"
#include "../terrain/TerrainHandler.h"
#include "../terrain/TerrainShader.h"
#include "../terrain/TerrainPageSurfaceLayer.h"
#include "../terrain/TerrainPageSurface.h"
#include "../terrain/TerrainLayerDefinition.h"
#include "../terrain/TerrainLayerDefinitionManager.h"

#include "pagedgeometry/include/PagedGeometry.h"

using namespace Ember::OgreView::Terrain;

namespace Ember {
namespace OgreView {

namespace Environment {

FoliageBase::FoliageBase(Terrain::TerrainManager& terrainManager,
						 Terrain::TerrainLayerDefinition terrainLayerDefinition,
						 Terrain::TerrainFoliageDefinition foliageDefinition)
		: mTerrainManager(terrainManager),
		  mTerrainLayerDefinition(std::move(terrainLayerDefinition)),
		  mFoliageDefinition(std::move(foliageDefinition)) {

	mTerrainManager.getHandler().EventLayerUpdated.connect(sigc::mem_fun(*this, &FoliageBase::TerrainHandler_LayerUpdated));
	mTerrainManager.getHandler().EventShaderCreated.connect(sigc::mem_fun(*this, &FoliageBase::TerrainHandler_EventShaderCreated));
	mTerrainManager.getHandler().EventAfterTerrainUpdate.connect(sigc::mem_fun(*this, &FoliageBase::TerrainHandler_AfterTerrainUpdate));
	mTerrainManager.EventTerrainShown.connect(sigc::mem_fun(*this, &FoliageBase::TerrainManager_TerrainShown));

}

FoliageBase::~FoliageBase() = default;


void FoliageBase::TerrainHandler_LayerUpdated(const Terrain::TerrainShader& shader, const AreaStore& areas) {
	if (mPagedGeometry) {
		//check if the layer update affects this layer, either if it's the actual layer, or one of the dependent layers
		bool isRelevant = false;
		if (shader.layer.layerDef.index >= mTerrainLayerDefinition.index) {
			isRelevant = true;
		}
		if (isRelevant) {
			for (const auto& area : areas) {
				const Ogre::TRect<Ogre::Real> ogreExtent(Convert::toOgre(area));
				mPagedGeometry->reloadGeometryPages(ogreExtent);
			}
		}
	}
}

void FoliageBase::TerrainHandler_EventShaderCreated(const TerrainLayerDefinition&) {
	//we'll assume that all shaders that are created after this foliage has been created will affect it, so we'll add it to the dependent layers and reload the geometry
	if (mPagedGeometry) {
		mPagedGeometry->reloadGeometry();
	}
}

void FoliageBase::TerrainHandler_AfterTerrainUpdate(const std::vector<WFMath::AxisBox<2>>& areas, const std::set<Terrain::TerrainPage*>&) {
	if (mPagedGeometry) {
		for (const auto& area : areas) {
			const Ogre::TRect<Ogre::Real> ogreExtent(Convert::toOgre(area));

			mPagedGeometry->reloadGeometryPages(ogreExtent);
		}
	}
}

void FoliageBase::TerrainManager_TerrainShown(const std::vector<Ogre::TRect<Ogre::Real>>& areas) {
	if (mPagedGeometry) {
		for (auto& area : areas) {
			mPagedGeometry->reloadGeometryPages(area);
		}
	}
}

void FoliageBase::reloadAtPosition(const WFMath::Point<2>& worldPosition) {
	if (mPagedGeometry) {
		mPagedGeometry->reloadGeometryPage(Ogre::Vector3(worldPosition.x(), 0, worldPosition.y()), true);
	}
}


//Gets the height of the terrain at the specified x/z coordinate
//The userData parameter isn't used in this implementation of a height function, since
//there's no need for extra data other than the x/z coordinates.
float getTerrainHeight(float x, float z, void* userData) {
	auto heightProvider = reinterpret_cast<IHeightProvider*>(userData);
	float height = 0;
	heightProvider->getHeight(TerrainPosition(x, z), height);
	return height;
}

//Gets the height of the terrain at the specified x/z coordinate
//The userData parameter isn't used in this implementation of a height function, since
//there's no need for extra data other than the x/z coordinates.
double getTerrainHeight(double x, double z, void* userData) {
	auto heightProvider = reinterpret_cast<IHeightProvider*>(userData);
	float height = 0;
	heightProvider->getHeight(TerrainPosition(x, z), height);
	return (double) height;
}
}

}
}
