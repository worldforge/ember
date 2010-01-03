//
// C++ Implementation: TerrainPage
//
// Description:
//
//
// Author: Erik Hjortsberg <erik.hjortsberg@gmail.com>, (C) 2005
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

#include "TerrainPage.h"


#include "TerrainPageGeometry.h"
#include "TerrainShader.h"
#include "TerrainPageSurfaceLayer.h"
#include "ITerrainPageBridge.h"

#include "../EmberOgre.h"
#include "../Convert.h"

#include "../environment/Foliage.h"
#include "TerrainManager.h"
#include "TerrainInfo.h"
#include "TerrainPageSurfaceLayer.h"
#include "TerrainPageSurface.h"
#include "TerrainLayerDefinition.h"
#include "TerrainPageFoliage.h"
#include "PlantAreaQuery.h"
#include "ISceneManagerAdapter.h"

#include "framework/LoggingInstance.h"
#include "services/config/ConfigService.h"

#include "framework/osdir.h"

#include <wfmath/axisbox.h>
#include <wfmath/vector.h>

#include <Mercator/Segment.h>
#include <Mercator/Shader.h>


using namespace EmberOgre::Environment;

namespace EmberOgre {
namespace Terrain {


TerrainPage::TerrainPage(const TerrainPosition& position, TerrainManager& manager, Mercator::Terrain& terrain)
: mManager(manager)
, mPosition(position)
, mBridge(0)
, mGeometry(new TerrainPageGeometry(*this, -15))
, mTerrainSurface(new TerrainPageSurface(*this, *mGeometry))
, mShadow(*this)
, mShadowTechnique(0)
, mExtent(WFMath::Point<2>(mPosition.x() * (getPageSize() - 1), (mPosition.y() - 1) * (getPageSize() - 1)), WFMath::Point<2>((mPosition.x() + 1) * (getPageSize() - 1), (mPosition.y()) * (getPageSize() - 1))
)
, mPageFoliage(new TerrainPageFoliage(mManager, *this))
{

	S_LOG_VERBOSE("Creating TerrainPage at position " << position.x() << ":" << position.y());
	mGeometry->init(terrain);
	setupShadowTechnique();
	mTerrainSurface->setShadow(&mShadow);
}

TerrainPage::~TerrainPage()
{
	delete mShadowTechnique;
	if (mBridge) {
		///we don't own the bridge, so we shouldn't delete it
		mBridge->unbindFromTerrainPage();
	}
}

float TerrainPage::getMaxHeight() const
{
	return mGeometry->getMaxHeight();
}

int TerrainPage::getPageSize() const
{
	return mManager.getPageIndexSize();
}

int TerrainPage::getVerticeCount() const
{
	return (getPageSize() * getPageSize());
}

int TerrainPage::getNumberOfSegmentsPerAxis() const
{
	return (getPageSize() - 1) / 64;
}

TerrainPageGeometry& TerrainPage::getGeometry()
{
	return *mGeometry;
}

const TerrainPageGeometry& TerrainPage::getGeometry() const
{
	return *mGeometry;
}

void TerrainPage::repopulateGeometry()
{
	mGeometry->repopulate();
}

void TerrainPage::signalGeometryChanged()
{
	if (mBridge) {
		mBridge->updateTerrain();
	}

	Ogre::Vector2 targetPage = Convert::toOgre<Ogre::Vector2>(mPosition);

	///note that we've switched the x and y offset here, since the terraininfo is in WF coords, but we now want Ogre coords
	Ogre::Vector2 adjustedOgrePos(targetPage.x + mManager.getTerrainInfo().getPageOffsetY(), targetPage.y + mManager.getTerrainInfo().getPageOffsetX());

	S_LOG_VERBOSE("Updating terrain page at position x: " << adjustedOgrePos.x << " y: " << adjustedOgrePos.y);
	mManager.getAdapter()->reloadPage(static_cast<unsigned int>(adjustedOgrePos.x), static_cast<unsigned int>(adjustedOgrePos.y));
}

void TerrainPage::setupShadowTechnique()
{
	if (mShadowTechnique) {
		delete mShadowTechnique;
	}
	mShadowTechnique = new SimpleTerrainPageShadowTechnique();
	mShadow.setShadowTechnique(mShadowTechnique);
}

void TerrainPage::createShadowData(const WFMath::Vector<3>& lightDirection)
{
	mGeometry->repopulate();
	mShadow.setLightDirection(lightDirection);
	mShadow.updateShadow(*mGeometry);
}

void TerrainPage::updateShadow(const WFMath::Vector<3>& lightDirection)
{
	mGeometry->repopulate();
	mShadow.setLightDirection(lightDirection);
	mShadow.updateShadow(*mGeometry);
}

void TerrainPage::regenerateCoverageMap()
{
	mPageFoliage->generateCoverageMap();
}


const TerrainPosition& TerrainPage::getWFPosition() const
{
	return mPosition;
}

const Ogre::MaterialPtr TerrainPage::getMaterial() const
{
	return mTerrainSurface->getMaterial();
}

unsigned int TerrainPage::getAlphaMapScale() const
{
	Ember::ConfigService* configSrv = Ember::EmberServices::getSingletonPtr()->getConfigService();
	if (configSrv->itemExists("terrain", "scalealphamap")) {
		int value = (int)configSrv->getValue("terrain", "scalealphamap");
		//make sure it can't go below 1
		return std::max<int>(1, value);
	} else {
		return 1;
	}
}



void TerrainPage::updateOgreHeightData(Ogre::Real* heightData)
{
	if (heightData) {
		mGeometry->updateOgreHeightData(heightData);
		mManager.EventTerrainPageGeometryUpdated.emit(*this);
	}
}

bool TerrainPage::getNormal(const TerrainPosition& localPosition, WFMath::Vector<3>& normal) const
{
	return mGeometry->getNormal(localPosition, normal);
}

void TerrainPage::showFoliage()
{
	prepareFoliage();
}

void TerrainPage::hideFoliage()
{
}

void TerrainPage::destroyFoliage()
{
}

void TerrainPage::prepareFoliage()
{
	mPageFoliage->generatePlantPositions();
	mPageFoliage->generateCoverageMap();
}

const WFMath::AxisBox<2>& TerrainPage::getExtent() const
{
	return mExtent;
}

const TerrainPageSurface* TerrainPage::getSurface() const
{
	return mTerrainSurface.get();
}

//const TerrainPageFoliage* TerrainPage::getPageFoliage() const
//{
//	return mPageFoliage.get();
//}

void TerrainPage::getPlantsForArea(PlantAreaQuery& query) const
{
	if (mManager.isFoliageShown()) {
		mPageFoliage->getPlantsForArea(*mGeometry, query);
	}
}


const TerrainPageShadow& TerrainPage::getPageShadow() const
{
	return mShadow;
}



TerrainPageSurfaceLayer* TerrainPage::addShader(const TerrainShader* shader)
{
	TerrainPageSurfaceLayer* layer = mTerrainSurface->createSurfaceLayer(*shader->getLayerDefinition(), shader->getTerrainIndex(), shader->getShader());
	layer->populate(*mGeometry);
	layer->setDiffuseTextureName(shader->getLayerDefinition()->getDiffuseTextureName());
	layer->setNormalTextureName(shader->getLayerDefinition()->getNormalMapTextureName());
	///get the scale by dividing the total size of the page with the size of each tile
	float scale = getAlphaTextureSize() / shader->getLayerDefinition()->getTileSize();
	layer->setScale(scale);
	return layer;
}

void TerrainPage::updateAllShaderTextures(bool repopulate)
{
	TerrainPageSurface::TerrainPageSurfaceLayerStore::const_iterator I = mTerrainSurface->getLayers().begin();
	for (; I != mTerrainSurface->getLayers().end(); ++I) {
		mTerrainSurface->updateLayer(*mGeometry, I->second->getSurfaceIndex(), repopulate);
	}
}

TerrainPageSurfaceLayer* TerrainPage::updateShaderTexture(const TerrainShader* shader, bool repopulate)
{
	TerrainPageSurfaceLayer* layer;
	TerrainPageSurface::TerrainPageSurfaceLayerStore::const_iterator I = mTerrainSurface->getLayers().find(shader->getTerrainIndex());
	if (I == mTerrainSurface->getLayers().end()) {
		layer = addShader(shader);
	} else {
		layer = I->second;
	}
	mTerrainSurface->updateLayer(*mGeometry, shader->getTerrainIndex(), repopulate);

	return layer;
}

void TerrainPage::registerBridge(ITerrainPageBridge* bridge)
{
	mBridge = bridge;
	mBridge->bindToTerrainPage(this);
}

void TerrainPage::unregisterBridge()
{
	///we're not responsible for this one, so we don't destroy it here
	mBridge = 0;
}

void TerrainPage::notifyBridgePageReady()
{
	mBridge->terrainPageReady();
}




}
}
