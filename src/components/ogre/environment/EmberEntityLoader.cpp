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

#include <sigc++/bind.h>

namespace EmberOgre {

namespace Environment {

EmberEntityLoader::EmberEntityLoader(::PagedGeometry::PagedGeometry &geom, unsigned int batchSize)
: mGeom(geom), mBatchSize(batchSize)
{
}


EmberEntityLoader::~EmberEntityLoader()
{
	///When shutting down, make sure to delete all connections.
#if EMBERENTITYLOADER_USEBATCH
	for (EntityStore::iterator I = mEntities.begin(); I != mEntities.end(); ++I) {
		for (EntityColumn::iterator J = I->second.begin(); J != I->second.end(); ++J) {
			for (EntityMap::iterator K = J->second.begin(); K != J->second.end(); ++K)
			{
				K->second.movedConnection.disconnect();
				K->second.beingDeletedConnection.disconnect();
				K->second.visibilityChangedConnection.disconnect();
			}
		}
	}
#else
	for (EntityMap::iterator I = mEntities.begin(); I != mEntities.end(); ++I)
	{
		I->second.movedConnection.disconnect();
		I->second.beingDeletedConnection.disconnect();
		I->second.visibilityChangedConnection.disconnect();
	}
#endif
}

void EmberEntityLoader::addEmberEntity(EmberPhysicalEntity * entity)
{
	if (!entity) {
		S_LOG_WARNING("Tried to add a null ref entity to the paged geometry.");
		return;
	}
	EntityInstance instance;
	instance.movedConnection = entity->Moved.connect(sigc::bind(sigc::mem_fun(*this, &EmberEntityLoader::EmberEntity_Moved), entity));
	instance.beingDeletedConnection = entity->BeingDeleted.connect(sigc::bind(sigc::mem_fun(*this, &EmberEntityLoader::EmberEntity_BeingDeleted), entity));
	instance.visibilityChangedConnection = entity->VisibilityChanged.connect(sigc::bind(sigc::mem_fun(*this, &EmberEntityLoader::EmberEntity_VisibilityChanged), entity));
	instance.entity = entity;
	
	Ogre::Vector3 position(entity->getSceneNode()->getWorldPosition());
	instance.lastPosition = position;
#if EMBERENTITYLOADER_USEBATCH
	const int batchX = Ogre::Math::Floor(position.x / mBatchSize);
	const int batchY = Ogre::Math::Floor(position.y / mBatchSize);
	mEntityLookup[entity] = std::pair<int, int>(batchX, batchY);
	
	EntityMap& entities(mEntities[batchX][batchY]);
#else
	EntityMap& entities(mEntities);
#endif
	
	entities[entity->getId()] = instance;

	///Rebuild geometry if necessary
	mGeom.reloadGeometryPage(position);
	



}

void EmberEntityLoader::removeEmberEntity(EmberPhysicalEntity * entity)
{
	if (!entity) {
		S_LOG_WARNING("Tried to remove a null ref entity from the paged geometry.");
		return;
	}
#if EMBERENTITYLOADER_USEBATCH
	EntityLookup::iterator I = mEntityLookup.find(entity);
	if (I != mEntityLookup.end()) {
		EntityStore::iterator J = mEntities.find(I->second.first);
		if (J != mEntities.end()) {
			EntityColumn& column(J->second);
			EntityColumn::iterator K = column.find(I->second.second);
			if (K != column.end()) {
				EntityMap& entityMap(K->second);
				EntityMap::iterator L = entityMap.find(entity->getId());
				if (L != entityMap.end()) {
					L->second.movedConnection.disconnect();
					L->second.beingDeletedConnection.disconnect();
					entityMap.erase(L);
					mEntityLookup.erase(I);
				}
			}
		}
	}
#else
	EntityMap::iterator I = mEntities.find(entity->getId());
	if (I != mEntities.end()) {
		I->second.movedConnection.disconnect();
		I->second.beingDeletedConnection.disconnect();
		mEntities.erase(I);
	}
	
#endif
	///Rebuild geometry if necessary.
	mGeom.reloadGeometryPage(entity->getSceneNode()->getWorldPosition());
}

EmberEntityLoader::EntityMap* EmberEntityLoader::getStoreForEntity(EmberPhysicalEntity* entity)
{
#if EMBERENTITYLOADER_USEBATCH
	EntityLookup::iterator I = mEntityLookup.find(entity);
	if (I != mEntityLookup.end()) {
		EntityStore::iterator J = mEntities.find(I->second.first);
		if (J != mEntities.end()) {
			EntityColumn& column(J->second);
			EntityColumn::iterator K = column.find(I->second.second);
			if (K != column.end()) {
				EntityMap& entityMap(K->second);
				return &entityMap;
			}
		}
	}
	return 0;
#else
	return &mEntities;
#endif
}


void EmberEntityLoader::loadPage(::PagedGeometry::PageInfo & page)
{
	static Ogre::ColourValue colour(1,1,1,1);
	
	
#if EMBERENTITYLOADER_USEBATCH
	const int batchX = static_cast<int>(Ogre::Math::Floor(page.bounds.left/ mBatchSize));
	const int batchY = static_cast<int>(Ogre::Math::Floor(page.bounds.top / mBatchSize));
	EntityMap& entities(mEntities[batchX][batchY]);
#else
	EntityMap& entities(mEntities);
#endif
	
	for (EntityMap::iterator I = entities.begin(); I != entities.end(); ++I) {
		EntityInstance& instance(I->second);
		EmberPhysicalEntity* emberEntity(instance.entity);
		if (emberEntity->isVisible()) {
			Ogre::SceneNode* sceneNode(emberEntity->getSceneNode());
			const Ogre::Vector3& pos(sceneNode->getWorldPosition());
			Model::Model* model(emberEntity->getModel());
			if (pos.x > page.bounds.left && pos.x < page.bounds.right && pos.z > page.bounds.top && pos.z < page.bounds.bottom) {
				for (Model::Model::SubModelSet::const_iterator J = model->getSubmodels().begin(); J != model->getSubmodels().end(); ++J) {
	// 				if (!(*J)->getEntity()->getParentSceneNode()) {
	// 					model->getParentSceneNode()->attachObject((*J)->getEntity());
	// 				}
	//  				if ((*J)->getEntity()->isVisible()) {
						addEntity((*J)->getEntity(), emberEntity->getScaleNode()->getWorldPosition(), emberEntity->getScaleNode()->getWorldOrientation(), 	emberEntity->getScaleNode()->_getDerivedScale(), colour);
	// 					(*J)->getEntity()->setVisible(false);
	//  				}
				}
			}
		}
	}
}

void EmberEntityLoader::EmberEntity_Moved(EmberPhysicalEntity* entity)
{
	EntityMap* entityMap(getStoreForEntity(entity));
	if (entityMap) {
		EntityMap::iterator I = entityMap->find(entity->getId());
		if (I != entityMap->end()) {
			EntityInstance& instance(I->second);
			mGeom.reloadGeometryPage(instance.lastPosition);
			mGeom.reloadGeometryPage(instance.entity->getSceneNode()->getWorldPosition());
			instance.lastPosition = instance.entity->getSceneNode()->getWorldPosition();
		}
	}
}

void EmberEntityLoader::EmberEntity_BeingDeleted(EmberPhysicalEntity* entity)
{
	removeEmberEntity(entity);
}

void EmberEntityLoader::EmberEntity_VisibilityChanged(bool visible, EmberPhysicalEntity* entity)
{
	///When the visibility changes, we only need to reload the page the entity is on.
	mGeom.reloadGeometryPage(entity->getSceneNode()->getWorldPosition());
}



}

}




