//
// C++ Implementation: Forest
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
// along with this program; if not, write to the Free Software Foundation,
// Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.//
//
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "Forest.h"
#include "EmberEntityLoader.h"
#include "ExclusiveImposterPage.h"

#include "framework/LoggingInstance.h"
#include "services/config/ConfigService.h"
#include "pagedgeometry/include/TreeLoader3D.h"
#include "pagedgeometry/include/BatchPage.h"
#include "pagedgeometry/include/DummyPage.h"
#include "pagedgeometry/include/PassiveEntityPage.h"

#include "../Scene.h"
#include "../model/ModelRepresentation.h"
#include "../model/Model.h"
#include "../terrain/TerrainInfo.h"
#include "../terrain/TerrainManager.h"
#include "../terrain/TerrainHandler.h"
#include "../terrain/ITerrainAdapter.h"

namespace Ember
{
namespace OgreView
{

namespace Environment
{

Forest::Forest(Terrain::TerrainManager& terrainManager) :
	mTerrainManager(terrainManager), mMaxRange(500)
{
	Ogre::Root::getSingleton().addFrameListener(this);
	mTerrainManager.getHandler().EventWorldSizeChanged.connect(sigc::mem_fun(*this, &Forest::worldSizeChanged));
}

Forest::~Forest()
{
	Ogre::Root::getSingleton().removeFrameListener(this);
}

void Forest::initialize()
{
	S_LOG_INFO("Initializing forest.");

	mTrees = std::make_unique<Forests::PagedGeometry>();
	mTrees->setCamera(&mTerrainManager.getScene().getMainCamera()); //Set the camera so PagedGeometry knows how to calculate LODs
	mTrees->setPageSize(128); //Set the size of each page of geometry

	mTrees->setInfinite();
	// 	mTrees->addDetailLevel<Forests::BatchPage>(150, 50);		//Use batches up to 150 units away, and fade for 30 more units
	//  mTrees->addDetailLevel<Forests::DummyPage>(100, 0);		//Use batches up to 150 units away, and fade for 30 more units
	mTrees->addDetailLevel<Forests::PassiveEntityPage> (256, 0); //Use standard entities up to 256 units away, and don't fade since the PassiveEntityPage doesn't support this (yet)
	mTrees->addDetailLevel<ExclusiveImposterPage> (mMaxRange, 50); //Use impostors up to 500 units, and for for 50 more units

	//Create a new TreeLoader2D object
	mEntityLoader = std::make_unique<EmberEntityLoader>(*mTrees, 64);
	// 	mTreeLoader = new Forests::TreeLoader3D(mTrees, Convert::toOgre(worldSize));
	mTrees->setPageLoader(mEntityLoader.get()); //Assign the "treeLoader" to be used to load geometry for the PagedGeometry instance
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
			mTrees.reset();
			mTreeLoader.reset();
			mEntityLoader.reset();
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
}

