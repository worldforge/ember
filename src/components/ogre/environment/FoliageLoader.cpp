//
// C++ Implementation: FoliageLoader
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

#include "FoliageLoader.h"
#include "../terrain/TerrainLayerDefinition.h"

#include "../Convert.h"
#include "../terrain/PlantAreaQuery.h"
#include "../terrain/PlantAreaQueryResult.h"
#include "../terrain/TerrainManager.h"
#include "../terrain/TerrainLayerDefinition.h"
#include "../terrain/PlantInstance.h"
#include "framework/LoggingInstance.h"
#include <wfmath/intersect.h>

#include <Ogre.h>

using namespace EmberOgre::Terrain;
namespace EmberOgre
{

namespace Environment
{

FoliageLoader::FoliageLoader(Ogre::SceneManager& sceneMgr, Terrain::TerrainManager& terrainManager, const Terrain::TerrainLayerDefinition& terrainLayerDefinition, const Terrain::TerrainFoliageDefinition& foliageDefinition, ::Forests::PagedGeometry& pagedGeometry) :
	mTerrainManager(terrainManager), mTerrainLayerDefinition(terrainLayerDefinition), mFoliageDefinition(foliageDefinition), mPagedGeometry(pagedGeometry), mMinScale(1), mMaxScale(1), mLatestPlantsResult(0)
{
	mEntity = sceneMgr.createEntity(std::string("shrubbery_") + mFoliageDefinition.getPlantType(), mFoliageDefinition.getParameter("mesh"));

	mMinScale = atof(mFoliageDefinition.getParameter("minScale").c_str());
	mMaxScale = atof(mFoliageDefinition.getParameter("maxScale").c_str());

}

FoliageLoader::~FoliageLoader()
{
	if (mEntity) {
		mEntity->_getManager()->destroyEntity(mEntity);
	}
}

void FoliageLoader::loadPage(::Forests::PageInfo &page)
{
	Ogre::ColourValue colour(1, 1, 1, 1);

	if (mLatestPlantsResult) {
		const PlantAreaQueryResult::PlantStore& store = mLatestPlantsResult->getStore();
		for (PlantAreaQueryResult::PlantStore::const_iterator I = store.begin(); I != store.end(); ++I) {
			const PlantInstance& plantInstance(*I);
//			Ogre::Vector3 pos(plantInstance.position.x, plantInstance.position.y, plantInstance.position.z);
			//			pos2D.x = pos.x;
			//			pos2D.y = pos.z;
			// 			TerrainManager->getShadowColourAt(pos2D, colour);

			addEntity(mEntity, plantInstance.position, Ogre::Quaternion(Ogre::Degree(plantInstance.orientation), Ogre::Vector3::UNIT_Y), Ogre::Vector3(plantInstance.scale.x, plantInstance.scale.y, plantInstance.scale.x), colour);
		}
	} else {
		PlantAreaQuery query(mTerrainLayerDefinition, mFoliageDefinition.getPlantType(), page.bounds, Ogre::Vector2(page.centerPoint.x, page.centerPoint.z));
		sigc::slot<void, const Terrain::PlantAreaQueryResult&> slot = sigc::mem_fun(*this, &FoliageLoader::plantQueryExecuted);

		mTerrainManager.getPlantsForArea(query, slot);
	}
}

void FoliageLoader::plantQueryExecuted(const Terrain::PlantAreaQueryResult& queryResult)
{
	mLatestPlantsResult = &queryResult;
	mPagedGeometry.reloadGeometryPage(Ogre::Vector3(queryResult.getQuery().getCenter().x, 0, queryResult.getQuery().getCenter().y), true);
	mLatestPlantsResult = 0;

}

}

}
