//
// C++ Implementation: EmberEntityLoader
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

#include "EmberEntityLoader.h"
#include "../EmberPhysicalEntity.h"

#include <OgreSceneNode.h>
#include <OgreColourValue.h>

#include "../model/Model.h"
#include "../model/ModelDefinition.h"
#include "../model/SubModel.h"

namespace EmberOgre {

namespace Environment {

EmberEntityLoader::EmberEntityLoader(::PagedGeometry::PagedGeometry *geom, unsigned int batchSize)
: mGeom(geom), mBatchSize(batchSize)
{
}


EmberEntityLoader::~EmberEntityLoader()
{
}

void EmberEntityLoader::addEmberEntity(EmberPhysicalEntity * entity)
{
	Ogre::Vector3 position(entity->getSceneNode()->getWorldPosition());
#if USEBATCH
	const int batchX = Ogre::Math::Floor(position.x / mBatchSize);
	const int batchY = Ogre::Math::Floor(position.y / mBatchSize);
	mEntityLookup[entity] = std::pair<int, int>(batchX, batchY);
	
	EntityMap& entities(mEntities[batchX][batchY]);
#else
	EntityMap& entities(mEntities);
#endif
	
	entities[entity->getId()] = entity;

	///Rebuild geometry if necessary
	mGeom->reloadGeometryPage(position);


}

void EmberEntityLoader::removeEmberEntity(EmberPhysicalEntity * entity)
{
#if USEBATCH
	EntityLookup::iterator I = mEntityLookup.find(entity);
	if (I != mEntityLookup.end()) {
		mEntities[I->second.first][I->second.second].erase(entity->getId());
	}
#else
	mEntities.erase(entity->getId());
#endif
	///Rebuild geometry if necessary
	mGeom->reloadGeometryPage(entity->getSceneNode()->getWorldPosition());
}

void EmberEntityLoader::loadPage(::PagedGeometry::PageInfo & page)
{
	static Ogre::ColourValue colour(1,1,1,1);
	
	const int batchX = Ogre::Math::Floor(page.bounds.left/ mBatchSize);
	const int batchY = Ogre::Math::Floor(page.bounds.top / mBatchSize);
	
#if USEBATCH
	EntityMap& entities(mEntities[batchX][batchY]);
#else
	EntityMap& entities(mEntities);
#endif
	
	for (EntityMap::iterator I = entities.begin(); I != entities.end(); ++I) {
		EmberPhysicalEntity* emberEntity(I->second);
		Ogre::SceneNode* sceneNode(emberEntity->getSceneNode());
		const Ogre::Vector3& pos(sceneNode->getWorldPosition());
		Model::Model* model(emberEntity->getModel());
		if (pos.x > page.bounds.left && pos.x < page.bounds.right && pos.z > page.bounds.top && pos.z < page.bounds.bottom) {
			for (Model::Model::SubModelSet::const_iterator J = model->getSubmodels().begin(); J != model->getSubmodels().end(); ++J) {
// 				if (!(*J)->getEntity()->getParentSceneNode()) {
// 					model->getParentSceneNode()->attachObject((*J)->getEntity());
// 				}
// 				if ((*J)->getEntity()->isVisible()) {
					addEntity((*J)->getEntity(), I->second->getScaleNode()->getWorldPosition(), I->second->getScaleNode()->getWorldOrientation(), 	I->second->getScaleNode()->_getDerivedScale(), colour);
// 					(*J)->getEntity()->setVisible(false);
// 				}
			}
		}
	}
}

}

}




