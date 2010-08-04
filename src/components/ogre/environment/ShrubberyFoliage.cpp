//
// C++ Implementation: ShrubberyFoliage
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

#include "ShrubberyFoliage.h"

#include "framework/LoggingInstance.h"

#include "FoliageLoader.h"

#include "../Scene.h"
#include "../Convert.h"
#include "../terrain/TerrainInfo.h"
#include "../terrain/TerrainManager.h"
#include "../terrain/TerrainLayerDefinition.h"

#include "../terrain/ISceneManagerAdapter.h"

#include "pagedgeometry/include/PagedGeometry.h"
#include "pagedgeometry/include/BatchPage.h"
#include "pagedgeometry/include/TreeLoader3D.h"

namespace EmberOgre {

namespace Environment {

ShrubberyFoliage::ShrubberyFoliage(Terrain::TerrainManager& terrainManager, const Terrain::TerrainLayerDefinition& terrainLayerDefinition, const Terrain::TerrainFoliageDefinition& foliageDefinition)
: FoliageBase(terrainManager, terrainLayerDefinition, foliageDefinition)
, mLoader(0)
{
}

ShrubberyFoliage::~ShrubberyFoliage()
{
	delete mLoader;
}

void ShrubberyFoliage::initialize()
{
	mPagedGeometry = new ::Forests::PagedGeometry(&mTerrainManager.getScene().getMainCamera(), mTerrainManager.getFoliageBatchSize());
	const WFMath::AxisBox<2>& worldSize = mTerrainManager.getTerrainInfo().getWorldSizeInIndices();

	::Forests::TBounds ogreBounds(Convert::toOgre(worldSize));
	if (ogreBounds.width() != ogreBounds.height()) {
		if (ogreBounds.width() > ogreBounds.height()) {
			float difference = ogreBounds.width() - ogreBounds.height();
			ogreBounds.bottom += difference;
		} else {
			float difference = ogreBounds.height() - ogreBounds.width();
			ogreBounds.right += difference;
		}
	}
	mPagedGeometry->setBounds(ogreBounds);


	mPagedGeometry->addDetailLevel<Forests::BatchPage>(64, 32);

	mLoader = new FoliageLoader(mTerrainManager.getAdapter()->getSceneManager(), mTerrainManager, mTerrainLayerDefinition, mFoliageDefinition, *mPagedGeometry);
 	mPagedGeometry->setPageLoader(mLoader);
}

void ShrubberyFoliage::frameStarted(const Ogre::FrameEvent & evt)
{
	if (mPagedGeometry) {
		try {
			mPagedGeometry->update();
		} catch (const std::exception& ex)
		{
			S_LOG_FAILURE("Error when updating shrubbery for terrain layer " << mTerrainLayerDefinition.getName() << " and areaId " << mTerrainLayerDefinition.getAreaId() << ". Will disable shrubbery."<< ex);
			delete mPagedGeometry;
			delete mLoader;
			mPagedGeometry = 0;
			mLoader = 0;
		}
	}
}



}

}
