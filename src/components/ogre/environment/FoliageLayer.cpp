//
// C++ Implementation: FoliageLayer
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
// along with this program; if not, write to the Free Software
// Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.//
//
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "FoliageLayer.h"
#include "../Convert.h"
#include "../terrain/PlantAreaQuery.h"
#include "../terrain/PlantAreaQueryResult.h"
#include "../terrain/TerrainManager.h"
#include "../terrain/TerrainLayerDefinition.h"
#include "../terrain/PlantInstance.h"
#include "framework/LoggingInstance.h"

using namespace Forests;
using namespace Ogre;
using namespace Ember::OgreView::Terrain;
namespace Ember
{
namespace OgreView
{

namespace Environment
{

FoliageLayer::FoliageLayer(::Forests::PagedGeometry *geom, GrassLoader<FoliageLayer> *ldr) :
	mTerrainManager(0), mTerrainLayerDefinition(0), mFoliageDefinition(0), mDensity(1.0f), mLatestPlantsResult(0)
{
	FoliageLayer::geom = geom;
	FoliageLayer::parent = ldr;

	minWidth = 1.0f;
	maxWidth = 1.0f;
	minHeight = 1.0f;
	maxHeight = 1.0f;
	maxSlope = 1000;
	// 	minY = 0; maxY = 0;
	renderTechnique = GRASSTECH_QUAD;
	fadeTechnique = FADETECH_ALPHA;
	animMag = 1.0f;
	animSpeed = 1.0f;
	animFreq = 1.0f;
	waveCount = 0.0f;
	lighting = false;
	animate = true;
	blend = false;
	shaderNeedsUpdate = true;
}

FoliageLayer::~FoliageLayer()
{
}

void FoliageLayer::configure(Terrain::TerrainManager* terrainManager, const Terrain::TerrainLayerDefinition* terrainLayerDefinition, const Terrain::TerrainFoliageDefinition* foliageDefinition)
{
	mTerrainManager = terrainManager;
	mTerrainLayerDefinition = terrainLayerDefinition;
	mFoliageDefinition = foliageDefinition;
	mDensity = std::stof(foliageDefinition->getParameter("density"));
}

unsigned int FoliageLayer::prepareGrass(const Forests::PageInfo& page, float densityFactor, float /*volume*/, bool& isAvailable)
{
	if (mLatestPlantsResult) {
		isAvailable = true;
		return (unsigned int)(mLatestPlantsResult->getStore().size() * densityFactor);
	} else {
		PlantAreaQuery query(*mTerrainLayerDefinition, mFoliageDefinition->getPlantType(), page.bounds, Ogre::Vector2(page.centerPoint.x, page.centerPoint.z));
		sigc::slot<void, const Terrain::PlantAreaQueryResult&> slot = sigc::mem_fun(*this, &FoliageLayer::plantQueryExecuted);

		mTerrainManager->getPlantsForArea(query, slot);
		isAvailable = false;
		return 0;
	}

}

unsigned int FoliageLayer::_populateGrassList(PageInfo /*page*/, float *posBuff, unsigned int grassCount)
{
	unsigned int finalGrassCount = 0;
	if (mLatestPlantsResult) {
		const PlantAreaQueryResult::PlantStore& store = mLatestPlantsResult->getStore();
		for (PlantAreaQueryResult::PlantStore::const_iterator I = store.begin(); I != store.end(); ++I) {
			if (finalGrassCount == grassCount) {
				break;
			}
			*posBuff++ = I->position.x;
			*posBuff++ = I->position.z;
			*posBuff++ = I->scale.x;
			*posBuff++ = I->orientation;
			finalGrassCount++;
		}
	} else {
		S_LOG_CRITICAL("_populateGrassList called without mLatestPlantsResult being set. This should never happen.");
	}
	return finalGrassCount;
}

void FoliageLayer::plantQueryExecuted(const Terrain::PlantAreaQueryResult& queryResult)
{
	mLatestPlantsResult = &queryResult;
	geom->reloadGeometryPage(Ogre::Vector3(queryResult.getQuery().getCenter().x, 0, queryResult.getQuery().getCenter().y), true);
	mLatestPlantsResult = 0;

}

Ogre::uint32 FoliageLayer::getColorAt(float x, float z)
{
	if (mLatestPlantsResult) {
		Ogre::Vector2 pos;
		Ogre::uint32 colour;
		pos.x = x;
		pos.y = z;
		mLatestPlantsResult->getShadowColourAtWorldPosition(pos, colour);
		return colour;
	}
	return geom->getSceneManager()->getAmbientLight().getAsARGB();
}


bool FoliageLayer::isColoursEnabled() const
{
	Ogre::Technique* tech = material->getBestTechnique();
	if (tech) {
		const std::string& name = tech->getName();
		if (Ogre::StringUtil::endsWith(name, "High", false)) {
			return false;
		} else if (Ogre::StringUtil::endsWith(name, "Medium", false)) {
			return false;
		} else {
			return true;
		}
	}
	return true;
}

bool FoliageLayer::isNormalsEnabled() const
{
	Ogre::Technique* tech = material->getBestTechnique();
	if (tech) {
		const std::string& name = tech->getName();
		if (Ogre::StringUtil::endsWith(name, "High", false)) {
			return true;
		} else if (Ogre::StringUtil::endsWith(name, "Medium", false)) {
			return true;
		} else {
			return false;
		}
	}
	return false;
}

bool FoliageLayer::isTangentsEnabled() const
{
	Ogre::Technique* tech = material->getBestTechnique();
	if (tech) {
		const std::string& name = tech->getName();
		if (Ogre::StringUtil::endsWith(name, "High", false)) {
			return true;
		} else if (Ogre::StringUtil::endsWith(name, "Medium", false)) {
			return true;
		} else {
			return false;
		}
	}
	return false;
}

bool FoliageLayer::isCastShadowsEnabled() const
{
	return false;
}

}

}
}
