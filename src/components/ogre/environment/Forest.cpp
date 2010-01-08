//
// C++ Implementation: Forest
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

#include "Forest.h"
#include "EmberEntityLoader.h"
#include "ExclusiveImposterPage.h"

#include "framework/LoggingInstance.h"
#include "services/config/ConfigService.h"
#include "pagedgeometry/include/PagedGeometry.h"
#include "pagedgeometry/include/TreeLoader3D.h"
#include "pagedgeometry/include/BatchPage.h"
#include "pagedgeometry/include/DummyPage.h"
#include "pagedgeometry/include/PassiveEntityPage.h"
#include "pagedgeometry/include/BatchedGeometry.h"

#include "../Convert.h"
#include "../EmberOgre.h"
#include "../model/ModelRepresentation.h"
#include "../model/Model.h"
#include "../terrain/TerrainInfo.h"
#include "../terrain/TerrainManager.h"
#include "../terrain/ISceneManagerAdapter.h"

namespace EmberOgre
{

namespace Environment
{

Forest::Forest(Terrain::TerrainManager& terrainManager) :
	mTerrainManager(terrainManager), mTrees(0), mTreeLoader(0), mEntityLoader(0), mMaxRange(500)
{
	Ogre::Root::getSingleton().addFrameListener(this);
	mTerrainManager.EventWorldSizeChanged.connect(sigc::mem_fun(*this, &Forest::worldSizeChanged));
}

Forest::~Forest()
{
	delete mEntityLoader;
	delete mTreeLoader;
	delete mTrees;
	Ogre::Root::getSingleton().removeFrameListener(this);
}

void Forest::initialize()
{
	S_LOG_INFO("Initializing forest.");
	const WFMath::AxisBox<2>& worldSize = mTerrainManager.getTerrainInfo().getWorldSizeInIndices();
	if (worldSize.upperBound(0) - worldSize.lowerBound(0) > 0 && worldSize.upperBound(1) - worldSize.lowerBound(1) > 0) {

		Ogre::Camera* camera = EmberOgre::getSingleton().getMainOgreCamera();

		mTrees = new Forests::PagedGeometry();
		mTrees->setCamera(camera); //Set the camera so PagedGeometry knows how to calculate LODs
		mTrees->setPageSize(64); //Set the size of each page of geometry

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
		mTrees->setBounds(ogreBounds);
		// 	mTrees->addDetailLevel<Forests::BatchPage>(150, 50);		//Use batches up to 150 units away, and fade for 30 more units
		//  mTrees->addDetailLevel<Forests::DummyPage>(100, 0);		//Use batches up to 150 units away, and fade for 30 more units
		mTrees->addDetailLevel<Forests::PassiveEntityPage> (150, 0); //Use standard entities up to 150 units away, and don't fade since the PassiveEntityPage doesn't support this (yet)
		mTrees->addDetailLevel<ExclusiveImposterPage> (mMaxRange, 50); //Use impostors up to 400 units, and for for 50 more units

		//Create a new TreeLoader2D object
		mEntityLoader = new EmberEntityLoader(*mTrees, 64);
		// 	mTreeLoader = new Forests::TreeLoader3D(mTrees, Convert::toOgre(worldSize));
		mTrees->setPageLoader(mEntityLoader); //Assign the "treeLoader" to be used to load geometry for the PagedGeometry instance
	}
}

void Forest::addTree(Ogre::Entity *entity, const Ogre::Vector3 &position, Ogre::Degree yaw, Ogre::Real scale)
{
	if (mTreeLoader && mTrees) {
		S_LOG_VERBOSE("Adding tree of entity type " << entity->getMesh()->getName() << " to position x: " << position.x << " y: " << position.y << " z: " << position.z << " and scale " << scale);
		try {
			mTreeLoader->addTree(entity, position, yaw, scale);
		} catch (const std::exception& ex) {
			S_LOG_FAILURE("Error when adding tree." << ex);
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
		} catch (const std::exception& ex) {
			S_LOG_FAILURE("Error when updating forest. Will disable forest."<< ex);
			delete mTreeLoader;
			delete mEntityLoader;
			delete mTrees;
			mTrees = 0;
			mTreeLoader = 0;
			mEntityLoader = 0;
		}
	}
	return true;
}

void Forest::addEmberEntity(Model::ModelRepresentation* modelRepresentation)
{
	if (mEntityLoader) {
		mEntityLoader->addEmberEntity(modelRepresentation);
		modelRepresentation->getModel().setRenderingDistance(mMaxRange);
	}
}

void Forest::removeEmberEntity(EmberEntity* entity)
{
	if (mEntityLoader) {
		mEntityLoader->removeEmberEntity(entity);
	}
}

void Forest::worldSizeChanged()
{
	if (!mEntityLoader) {
		initialize();
	}
}


}

}

