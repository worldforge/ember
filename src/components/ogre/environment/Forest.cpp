//
// C++ Implementation: Forest
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

#include "Forest.h"

#include "services/EmberServices.h"
#include "services/logging/LoggingService.h"
#include "services/config/ConfigService.h"
#include "pagedgeometry/include/PagedGeometry.h"
#include "pagedgeometry/include/TreeLoader3D.h"
#include "pagedgeometry/include/BatchPage.h"
#include "pagedgeometry/include/ImpostorPage.h"
#include "pagedgeometry/include/DummyPage.h"
#include "pagedgeometry/include/BatchedGeometry.h"

#include "../AvatarCamera.h"
#include "../EmberOgre.h"
#include "../terrain/TerrainGenerator.h"
#include "../terrain/ISceneManagerAdapter.h"

namespace EmberOgre {

namespace Environment {

Forest::Forest()
: mTrees(0)
, mTreeLoader(0)
{
	Ogre::Root::getSingleton().addFrameListener(this);
}


Forest::~Forest()
{
	delete mTreeLoader;
	delete mTrees;
	Ogre::Root::getSingleton().removeFrameListener(this);
}

void Forest::initialize()
{
	S_LOG_INFO("Initializing forest.");
	const WFMath::AxisBox<2>& worldSize = EmberOgre::getSingleton().getTerrainGenerator()->getTerrainInfo().getWorldSizeInIndices();
	Ogre::Camera* camera = EmberOgre::getSingleton().getMainCamera()->getCamera();
	
	mTrees = new PagedGeometry::PagedGeometry();
	mTrees->setCamera(camera);	//Set the camera so PagedGeometry knows how to calculate LODs
	mTrees->setPageSize(64);	//Set the size of each page of geometry
	mTrees->setInfinite();		//Use infinite paging mode
// 	mTrees->addDetailLevel<PagedGeometry::BatchPage>(150, 50);		//Use batches up to 150 units away, and fade for 30 more units
 	mTrees->addDetailLevel<PagedGeometry::DummyPage>(100, 0);		//Use batches up to 150 units away, and fade for 30 more units
	mTrees->addDetailLevel<PagedGeometry::ImpostorPage>(500, 50);	//Use impostors up to 400 units, and for for 50 more units

	//Create a new TreeLoader2D object
	mTreeLoader = new PagedGeometry::TreeLoader3D(mTrees, Atlas2Ogre(worldSize));
	mTrees->setPageLoader(mTreeLoader);	//Assign the "treeLoader" to be used to load geometry for the PagedGeometry instance	
}

void Forest::addTree(Ogre::Entity *entity, const Ogre::Vector3 &position, Ogre::Degree yaw, Ogre::Real scale)
{
	if (mTreeLoader)
	{
		S_LOG_VERBOSE("Adding tree of entity type " << entity->getMesh()->getName() << " to position x: " << position.x << " y: " << position.y << " z: " << position.z << " and scale " << scale);
		try {
			mTreeLoader->addTree(entity, position, yaw, scale);
		} catch (const std::exception& ex)
		{
			S_LOG_FAILURE("Error when adding tree: " << ex.what());
		}
	} else {
		S_LOG_WARNING("Could not add tree before the forest has been initialized.");
	}
}

bool Forest::frameStarted(const Ogre::FrameEvent & evt)
{
	if (mTrees) {
		try {
			mTrees->update();
		} catch (const Ogre::Exception& ex)
		{
			S_LOG_FAILURE("Error when updating forest. Will disable forest.\n"<< ex.what());
			delete mTreeLoader;
			delete mTrees;
		}
	}
	return true;
}

}

}
