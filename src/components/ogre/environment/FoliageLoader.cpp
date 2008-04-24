//
// C++ Implementation: FoliageLoader
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

#include "FoliageLoader.h"
#include "../terrain/TerrainLayerDefinition.h"

#include "../MathConverter.h"
#include "../EmberOgre.h"
#include "../terrain/TerrainGenerator.h"
#include "../terrain/TerrainPageFoliage.h"
#include "../terrain/TerrainPage.h"
#include "../terrain/TerrainLayerDefinition.h"
#include "services/EmberServices.h"
#include "services/logging/LoggingService.h"
#include <wfmath/intersect.h>


#include <Ogre.h>

using namespace EmberOgre::Terrain;
namespace EmberOgre {

namespace Environment {

FoliageLoader::FoliageLoader(const Terrain::TerrainLayerDefinition& terrainLayerDefinition, const Terrain::TerrainFoliageDefinition& foliageDefinition)
: mTerrainLayerDefinition(terrainLayerDefinition)
, mFoliageDefinition(foliageDefinition)
, mMinScale(1)
, mMaxScale(1)
{
	Ogre::SceneManager* sceneMgr = EmberOgre::getSingleton().getSceneManager();
	mEntity = sceneMgr->createEntity(std::string("shrubbery_") + mFoliageDefinition.getPlantType(), mFoliageDefinition.getParameter("mesh"));
	
	mMinScale = atof(mFoliageDefinition.getParameter("minScale").c_str());
	mMaxScale = atof(mFoliageDefinition.getParameter("maxScale").c_str());

}


FoliageLoader::~FoliageLoader()
{
}

void FoliageLoader::loadPage(::PagedGeometry::PageInfo &page)
{
	///make these static for fast lookup
	static Ogre::Vector2 pos2D;
	static Ogre::ColourValue colour(1,1,1,1);
	static Terrain::TerrainGenerator* terrainGenerator(EmberOgre::getSingleton().getTerrainGenerator());

	TerrainPosition wfPos(Ogre2Atlas_TerrainPosition(page.centerPoint));
	TerrainPage* terrainPage = terrainGenerator->getTerrainPage(wfPos);
	if (terrainPage) {
		Ogre::TRect<float> ogrePageExtent = Atlas2Ogre(terrainPage->getExtent());
		Ogre::TRect<float> adjustedBounds = Ogre::TRect<float>(page.bounds.left - ogrePageExtent.left, page.bounds.top - ogrePageExtent.top, page.bounds.right - ogrePageExtent.left, page.bounds.bottom - ogrePageExtent.top);
		TerrainPageFoliage::PlantStore plants;
		
		unsigned char threshold(100);
		if (mFoliageDefinition.getParameter("threshold") != "") {
			threshold = static_cast<unsigned char>(atoi(mFoliageDefinition.getParameter("threshold").c_str()));
		}
		
		terrainPage->getPageFoliage()->getPlantsForArea(mTerrainLayerDefinition, threshold, mFoliageDefinition.getPlantType(), adjustedBounds, plants);
		for (TerrainPageFoliage::PlantStore::const_iterator I = plants.begin(); I != plants.end(); ++I) {
			Ogre::Vector3 pos(I->x + ogrePageExtent.left, terrainGenerator->getHeight(TerrainPosition(I->x + ogrePageExtent.left, -(I->y + ogrePageExtent.top))), I->y + ogrePageExtent.top);
			
			float scale = Ogre::Math::RangeRandom(mMinScale, mMaxScale);
			pos2D.x = pos.x;
			pos2D.y = pos.z;
// 			terrainGenerator->getShadowColourAt(pos2D, colour);
			
			//Get rotation
			Ogre::Degree angle(Ogre::Math::RangeRandom(0, 360.0f));
			Ogre::Quaternion rot(angle, Ogre::Vector3::UNIT_Y);
			

			addEntity(mEntity, pos, rot, Ogre::Vector3(scale,scale,scale), colour);
		}
	}
}


}

}
