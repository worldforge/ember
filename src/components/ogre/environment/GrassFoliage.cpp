//
// C++ Implementation: GrassFoliage
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

#include "GrassFoliage.h"

#include "services/EmberServices.h"
#include "services/logging/LoggingService.h"

#include "FoliageLayer.h"
#include "FoliageArea.h"

#include "components/ogre/EmberOgre.h"
#include "components/ogre/terrain/TerrainGenerator.h"


#include "../AvatarCamera.h"
#include "../EmberOgre.h"
#include "../terrain/TerrainGenerator.h"
#include "../terrain/TerrainLayerDefinition.h"
#include "../terrain/ISceneManagerAdapter.h"

#include "pagedgeometry/include/PagedGeometry.h"
#include "pagedgeometry/include/BatchPage.h"
#include "pagedgeometry/include/GrassLoader.h"

namespace EmberOgre {

namespace Environment {

GrassFoliage::GrassFoliage(const Terrain::TerrainLayerDefinition& terrainLayerDefinition, const Terrain::TerrainFoliageDefinition& foliageDefinition)
: FoliageBase(terrainLayerDefinition, foliageDefinition)
, mGrass(0)
, mGrassLoader(0)
{
}

GrassFoliage::~GrassFoliage()
{
	delete mGrassLoader;
	delete mGrass;
}

void GrassFoliage::initialize()
{
	Ogre::Camera* camera = EmberOgre::getSingleton().getMainCamera()->getCamera();
	mGrass = new ::PagedGeometry::PagedGeometry(camera, 32);
	const WFMath::AxisBox<2>& worldSize = EmberOgre::getSingleton().getTerrainGenerator()->getTerrainInfo().getWorldSizeInIndices();	mGrass->setBounds(Atlas2Ogre(worldSize));
	mGrass->addDetailLevel<PagedGeometry::GrassPage>(96);
	
	//Create a GrassLoader object
	mGrassLoader = new ::PagedGeometry::GrassLoader<FoliageLayer>(mGrass);
 	mGrass->setPageLoader(mGrassLoader);	//Assign the "treeLoader" to be used to load 
	mGrassLoader->setHeightFunction(&getTerrainHeight);

	//Add some grass to the scene with GrassLoader::addLayer()
	FoliageLayer *l = mGrassLoader->addLayer(mFoliageDefinition.getParameter("material"));
	
	l->configure(&mTerrainLayerDefinition, &mFoliageDefinition);
	//Configure the grass layer properties (size, density, animation properties, fade settings, etc.)
	l->setMinimumSize(1.0f, 1.0f);
	l->setMaximumSize(1.5f, 1.5f);
	l->setAnimationEnabled(true);		//Enable animations
	l->setSwayDistribution(10.0f);		//Sway fairly unsynchronized
	l->setSwayLength(0.5f);				//Sway back and forth 0.5 units in length
	l->setSwaySpeed(0.5f);				//Sway 1/2 a cycle every second
// 	l->setDensity(1.5f);				//Relatively dense grass
	l->setFadeTechnique(::PagedGeometry::FADETECH_GROW);	//Distant grass should slowly raise out of the ground when coming in range
	l->setRenderTechnique(::PagedGeometry::GRASSTECH_CROSSQUADS);	//Draw grass as scattered quads

// 	l->setHeightRange(0.001f);
	l->setMapBounds(Atlas2Ogre(worldSize));	//(0,0)-(1500,1500) is the full boundaries of the terrain
}

void GrassFoliage::frameStarted(const Ogre::FrameEvent & evt)
{	
	
	if (mGrass) {
		try {
			mGrass->update();
		} catch (const Ogre::Exception& ex)
		{
			S_LOG_FAILURE("Error when updating grass. Will disable grass.\n"<< ex.what());
			delete mGrassLoader;
			delete mGrass;
		}
	}	
}


}

}
