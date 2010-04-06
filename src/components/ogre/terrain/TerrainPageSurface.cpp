//
// C++ Implementation: TerrainPageSurface
//
// Description:
//
//
// Author: Erik Hjortsberg <erik.hjortsberg@gmail.com>, (C) 2007
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

#include "TerrainPageSurface.h"
#include "TerrainPageShadow.h"
#include "TerrainPageSurfaceLayer.h"
#include "TerrainPageSurfaceCompiler.h"
#include "TerrainPageGeometry.h"
#include "TerrainLayerDefinition.h"
#include "../Convert.h"
#include <OgreMaterialManager.h>
#include <OgreRoot.h>
#include <OgreSceneManager.h>

namespace EmberOgre
{
namespace Terrain
{

TerrainPageSurface::TerrainPageSurface(const TerrainPage& terrainPage) :
	mTerrainPage(terrainPage), mSurfaceCompiler(new TerrainPageSurfaceCompiler())
{
	///create a name for out material
	// 	S_LOG_INFO("Creating a material for the terrain.");
	std::stringstream materialNameSS;
	materialNameSS << "EmberTerrain_Segment";
	materialNameSS << "_" << terrainPage.getWFPosition().x() << "_" << terrainPage.getWFPosition().y();
	mMaterialName = materialNameSS.str();

}

TerrainPageSurface::~TerrainPageSurface()
{
	for (TerrainPageSurfaceLayerStore::iterator I(mLayers.begin()); I != mLayers.end(); ++I) {
		delete I->second;
	}
}

const TerrainPageSurface::TerrainPageSurfaceLayerStore& TerrainPageSurface::getLayers() const
{
	return mLayers;
}

TerrainPageSurfaceLayer* TerrainPageSurface::updateLayer(TerrainPageGeometry& geometry, int layerIndex, bool repopulate)
{
	TerrainPageSurfaceLayerStore::iterator I = mLayers.find(layerIndex);
	if (I != mLayers.end()) {
		if (repopulate) {
			I->second->populate(geometry);
		}
		//		I->second->updateCoverageImage(geometry);
		return I->second;
	}

}

const TerrainPosition& TerrainPageSurface::getWFPosition() const
{
	return mTerrainPage.getWFPosition();
}

int TerrainPageSurface::getNumberOfSegmentsPerAxis() const
{
	return mTerrainPage.getNumberOfSegmentsPerAxis();
}

unsigned int TerrainPageSurface::getPixelWidth() const
{
	return mTerrainPage.getAlphaTextureSize();
}

const Ogre::MaterialPtr TerrainPageSurface::getMaterial() const
{
	std::pair<Ogre::ResourcePtr, bool> result = Ogre::MaterialManager::getSingleton().createOrRetrieve(mMaterialName, "General");
	return static_cast<Ogre::MaterialPtr>(result.first);
}

TerrainPageSurfaceCompilationInstance* TerrainPageSurface::createSurfaceCompilationInstance(const TerrainPageGeometryPtr& geometry) const
{
	//The compiler only works with const surfaces, so we need to create such a copy of our surface map.
	SurfaceLayerStore constLayers;
	for (TerrainPageSurfaceLayerStore::const_iterator I = mLayers.begin(); I != mLayers.end(); ++I) {
		constLayers.insert(SurfaceLayerStore::value_type(I->first, I->second));
	}
	//TODO: Add shadow
	return mSurfaceCompiler->createCompilationInstance(geometry, constLayers, 0);
}

TerrainPageSurfaceLayer* TerrainPageSurface::createSurfaceLayer(const TerrainLayerDefinition& definition, int surfaceIndex, const Mercator::Shader& shader)
{
	TerrainPageSurfaceLayer* terrainSurface = new TerrainPageSurfaceLayer(*this, definition, surfaceIndex, shader);
	mLayers.insert(TerrainPageSurfaceLayerStore::value_type(surfaceIndex, terrainSurface));
	return terrainSurface;
}

}

}
