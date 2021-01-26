//
// C++ Implementation: TerrainPage
//
// Description:
//
//
// Author: Erik Ogenvik <erik@ogenvik.org>, (C) 2005
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

#include "TerrainPage.h"

#include "TerrainPageGeometry.h"
#include "TerrainShader.h"
#include "TerrainPageSurfaceLayer.h"

#include "../EmberOgre.h"
#include "../Convert.h"

#include "TerrainPageSurface.h"
#include "TerrainLayerDefinition.h"

#include "services/config/ConfigService.h"

#include <Mercator/Segment.h>
#include <Mercator/Shader.h>

namespace Ember {
namespace OgreView {
namespace Terrain {

TerrainPage::TerrainPage(const TerrainIndex& index, int pageSize, ICompilerTechniqueProvider& compilerTechniqueProvider) :
		mIndex(index),
		mPageSize(pageSize),
		mPosition(index.first, index.second),
		mTerrainSurface(std::make_unique<TerrainPageSurface>(*this, compilerTechniqueProvider)),
		mExtent(WFMath::Point<2>(mPosition.x() * (getPageSize() - 1), -(mPosition.y() - 1) * (getPageSize() - 1)),
				WFMath::Point<2>((mPosition.x() + 1) * (getPageSize() - 1), -(mPosition.y()) * (getPageSize() - 1))) {

	S_LOG_VERBOSE("Creating TerrainPage at position " << index.first << ":" << index.second);
}

TerrainPage::~TerrainPage() = default;

int TerrainPage::getPageSize() const {
	return mPageSize;
}

int TerrainPage::getVerticeCount() const {
	return (getPageSize() * getPageSize());
}

int TerrainPage::getNumberOfSegmentsPerAxis() const {
	return (getPageSize() - 1) / 64;
}

const TerrainPosition& TerrainPage::getWFPosition() const {
	return mPosition;
}

const TerrainIndex& TerrainPage::getWFIndex() const {
	return mIndex;
}


Ogre::MaterialPtr TerrainPage::getMaterial() const {
	return mTerrainSurface->getMaterial();
}

Ogre::MaterialPtr TerrainPage::getCompositeMapMaterial() const {
	return mTerrainSurface->getCompositeMapMaterial();
}

unsigned int TerrainPage::getBlendMapScale() const {
	ConfigService& configSrv = ConfigService::getSingleton();
	if (configSrv.itemExists("terrain", "scalealphamap")) {
		int value = (int) configSrv.getValue("terrain", "scalealphamap");
		//make sure it can't go below 1
		return std::max<unsigned int>(1, value);
	} else {
		return 1;
	}
}

const WFMath::AxisBox<2>& TerrainPage::getWorldExtent() const {
	return mExtent;
}

const TerrainPageSurface* TerrainPage::getSurface() const {
	return mTerrainSurface.get();
}

void TerrainPage::addShader(const TerrainLayerDefinition& definition, int surfaceIndex, const Mercator::Shader& shader) {
	mTerrainSurface->createSurfaceLayer(definition, surfaceIndex, shader);
	auto I = mTerrainSurface->getLayers().find(surfaceIndex);
	if (I != mTerrainSurface->getLayers().end()) {
		auto& layer = I->second;

		layer->setDiffuseTextureName(definition.mDiffuseTextureName);
		layer->setNormalTextureName(definition.mNormalMapTextureName);
		//get the scale by dividing the total size of the page with the size of each tile
		float scale = (float) getBlendMapSize() / definition.mTileSize;
		layer->setScale(scale);
	}
}

void TerrainPage::updateAllShaderTextures(TerrainPageGeometry& geometry, bool repopulate) {
	auto I = mTerrainSurface->getLayers().begin();
	for (; I != mTerrainSurface->getLayers().end(); ++I) {
		mTerrainSurface->updateLayer(geometry, I->second->getSurfaceIndex(), repopulate);
	}
}

void TerrainPage::updateShaderTexture(const TerrainLayerDefinition& definition,
									  int surfaceIndex,
									  const Mercator::Shader& shader,
									  TerrainPageGeometry& geometry,
									  bool repopulate) {
	auto I = mTerrainSurface->getLayers().find(surfaceIndex);
	if (I == mTerrainSurface->getLayers().end()) {
		addShader(definition, surfaceIndex, shader);
	}
	mTerrainSurface->updateLayer(geometry, surfaceIndex, repopulate);

}


}
}
}
